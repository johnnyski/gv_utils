/* 
 * gv_file.c
 *
 *
 *-----------------------------------------------------------------------------
 *
 * By: Thuy Nguyen
 *     Science Systems and Applications, Inc. (SSAI)
 *     NASA/TRMM Office
 *     Copyright (C) 1996, 1997
 *
 *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <IO.h>

#include "gv_file.h"

#define MAX_SITE_NAME_LEN 5
#define MAX_STR_LEN       5
#define MAX_CMD_LEN       256

/**********************************************************************/
/*                                                                    */
/*               gv_utils_get_info_from_filename                      */
/*                                                                    */
/**********************************************************************/
void gv_utils_get_info_from_filename(char *fname, int verbose, 
							gv_utils_filename_info_type_t itype, 
							void *info)
{
  /* Get info from filename. filename's format: 
   *   [path]<4prod-char>.<yymmdd>.<granule#>.<site>.<TSDIS_product#>.HDF
   */
  char *fname_ptr;
  char tmp_str[MAX_STR_LEN];

  if (fname == NULL || info == NULL) return;

  
  if (strrchr(fname, '/') != NULL) 
	fname_ptr = strrchr(fname, '/') +1;   /* Skip pathname */
  else fname_ptr = fname;

  /* fname_ptr: <4prod-char>.<yymmdd>.<granule#>.<site>.<TSDIS_product#>.HDF */
  if (strchr(fname_ptr, '.') == NULL) 
	goto INVALID_FORMAT;

  fname_ptr = strchr(fname_ptr, '.') + 1;   /* Skip to date */
  if (strchr(fname_ptr, '.') == NULL) 
	goto INVALID_FORMAT;

  if (itype == GV_UTILS_DATE_INFO) {
	memset(tmp_str, '\0', MAX_STR_LEN);
	strncpy(tmp_str, fname_ptr, 2);
	((DATE_STR *)info)->tkyear = atoi(tmp_str) + 1900;

	strncpy(tmp_str, fname_ptr+2, 2);
	((DATE_STR *)info)->tkmonth = atoi(tmp_str);
	strncpy(tmp_str, fname_ptr+4, 2);
	((DATE_STR *)info)->tkday = atoi(tmp_str);
	return;
  }

  fname_ptr = strchr(fname_ptr, '.') + 1;   /*  Skip to granule# */

  if (strchr(fname_ptr, '.') == NULL) 
	goto INVALID_FORMAT;
  fname_ptr = strchr(fname_ptr, '.') + 1;   /*  Skip to site */
  if (itype == GV_UTILS_SITE_INFO) {
	if (strchr(fname_ptr, '.') == NULL) 
	  goto INVALID_FORMAT;
	strncpy((char *)info, fname_ptr, strlen(fname_ptr) - strlen(strchr(fname_ptr, '.')));
	return;
  }
  
INVALID_FORMAT:

  if (verbose) 
	fprintf(stderr, "Unrecognized filename's format: %s. Filename should follow TSDIS file naming convention.\n", fname_ptr);
  

} /* gv_utils_get_info_from_filename */



/****************************************************************************/
/*                                                                          */
/*                    gv_utils_uncompress_file                              */
/*                                                                          */
/****************************************************************************/
void gv_utils_uncompress_file(char *fname, char *new_fname) 
{
  /* Uncompress file and store the new content to 
   * new_fname if new_fname is not NULL.
   */
  char cmd[MAX_CMD_LEN];

  if (fname == NULL) return;

  if (new_fname != NULL) 
	sprintf(cmd, "gunzip -cf %s > %s", fname, new_fname);
  else
	sprintf(cmd, "gunzip -f %s", fname);
  system(cmd);
}

/****************************************************************************/
/*                                                                          */
/*                        gv_utils_compress_file                                */
/*                                                                          */
/****************************************************************************/
void gv_utils_compress_file (char *fname, char *new_fname) 
{
  /* Compress file. Store content to new_fname if new_fname != NULL.
   */
  char cmd[MAX_CMD_LEN];

  if (fname == NULL) return;

  if (new_fname != NULL)
	sprintf(cmd, "gzip -cf %s>%s", fname, new_fname);
  else
	sprintf(cmd, "gzip -f %s", fname);
  system(cmd);

}

/****************************************************************************/
/*                                                                          */
/*                          gv_utils_file_exist                                 */
/*                                                                          */
/****************************************************************************/
int gv_utils_file_exist(char *fname)
{
  /* Return 1 if file exist; 0, otherwise.
   */
  struct stat buf;

  if (fname && stat(fname, &buf) == 0) return 1;
  return 0;
}
/****************************************************************************/
/*                                                                          */
/*                            gv_utils_is_a_file                            */
/*                                                                          */
/****************************************************************************/
int gv_utils_is_a_file(char *name)
{
  /* Return 1 if it's a file; 0, otherwise.
   */
  struct stat buf;

  if (name && stat(name, &buf) == 0) {
	if (S_ISREG(buf.st_mode)) return 1;
  }
  return 0;
}

/****************************************************************************/
/*                                                                          */
/*                                 gv_utils_is_a_dir                        */
/*                                                                          */
/****************************************************************************/
int gv_utils_is_a_dir(char *name)
{
  /* Return 1 if it's a dir; 0, otherwise.
   */
  struct stat buf;

  if (name && stat(name, &buf) == 0) {
	if (S_ISDIR(buf.st_mode)) return 1;
  }
  return 0;
}

/****************************************************************************/
/*                                                                          */
/*                          gv_utils_get_file_size                          */
/*                                                                          */
/****************************************************************************/
int gv_utils_get_file_size(char *fname)
{
  /* Return the number of bytes in the file; -1, for failure.
   */
  struct stat buf;

  if (fname && stat(fname, &buf) == 0) {
	return buf.st_size;
  }
  return -1;
}

/****************************************************************************/
/*                                                                          */
/*                        gv_utils_get_info_from_gauge_filename              */
/*                                                                          */
/****************************************************************************/
void gv_utils_get_info_from_gauge_filename(char *fname, char *netID,
										   char *gaugeID, int *yr)
{
  /* Get info from gauge's filename: fname's format:
   *        [path]/<netID><gaugeID>_yy.gmin, i.e., KSC0025_95.gmin
   * netID, gaugeID, or yr may be NULL.
   */
  char *fname_ptr;
  char yr_str[3];

  if (fname == NULL) return;

  if (strrchr(fname, '/') != NULL) 
	fname_ptr = strrchr(fname, '/') +1;   /* Skip pathname */
  else fname_ptr = fname;
  /* fname_ptr: <3-charnetID><gaugeID>_yy.gmin */
  if (netID != NULL) {
	strncpy(netID, fname_ptr, 3);
	netID[3] = '\0';
  }
  if (gaugeID != NULL) {
	strncpy(gaugeID, fname_ptr + 3, 4);
	gaugeID[4] = '\0';
  }
  if (yr != NULL) {
	strncpy(yr_str, fname_ptr + 8, 2);
	yr_str[2] = '\0';
	*yr = atoi(yr_str);
  }
  
} /* gv_utils_get_info_from_gauge_filename */

/****************************************************************************/
/*                                                                          */
/*                  gv_utils_create_tmp_filename                             */
/*                                                                          */
/****************************************************************************/
void gv_utils_create_tmp_filename(char *filename, char *dir, char *tmp_filename)
{
  /* Create a tmp filename based on the specified filename.
   * tmp filename has dir as prefix.dir must ends with '/' if is not "".
   * tmp filename will not contain '.gz|.Z from the specified filename.
   */
  char *str, *str2;

  if (filename == NULL || dir == NULL || tmp_filename == NULL) return;

  str = strrchr(filename, '/');
  if (str == NULL)
	str = filename;
  else str++;        /* Skip '/' */
  if ((str2 = strstr(str, ".gz")) != NULL || (str2 = strstr(str, ".Z"))) 
	*str2 = '\0'; /* Remove compression extension */

  sprintf(tmp_filename, "%s%s.tmp", dir, str);
} /* gv_utils_create_tmp_filename */


