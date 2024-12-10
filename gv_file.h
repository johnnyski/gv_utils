/*
 * gv_file.h
 *
 */

#ifndef __GV_FILE_H__
#define __GV_FILE_H__ 1

#define MAX_FILENAME_LEN 256
typedef enum {
  GV_UTILS_DATE_INFO=0, GV_UTILS_SITE_INFO
} gv_utils_filename_info_type_t;


/* gv_utils_get_info_from_filename:
 * Filename format: 4char_prod.yymmdd.granule#.site.version#.HDF 
 */
void gv_utils_get_info_from_filename(char *fname, int verbose, 
							gv_utils_filename_info_type_t itype, 
							void *info);

/* gv_utils_uncompress_file: Uncompress file and store content to new_fname if 
 * new_fname != NULL.
 */
void gv_utils_uncompress_file(char *fname, char *new_fname);

/* gv_utils_compress_file: Compress file. Store content to new_fname 
 * if new_fname != NULL.
 */
void gv_utils_compress_file (char *fname, char *new_fname);

/* gv_utils_file_exist: Return 1 if file exist; 0, otherwise.
 */
int gv_utils_file_exist(char *fname);

/* gv_utils_is_a_dir: Return 1 if name is a dir; 0, otherwise.
 */
int gv_utils_is_a_dir(char *name);
/* gv_utils_is_a_file: Return 1 if name is a file; 0, otherwise.
 */
int gv_utils_is_a_file(char *name);


/* gv_utils_get_info_from_gauge_filename:
 *    Get info from gauge's filename: fname's format:
 *        [path]/<netID><gaugeID>_yy.gmin, i.e., KSC0025_95.gmin
 * netID, gaugeID, or yr may be NULL.
 */
void gv_utils_get_info_from_gauge_filename(char *fname, char *netID, char *gaugeID, 
								  int *yr);

/* gv_utils_get_file_size: Return the number of bytes in the file; -1, for failure.
 */
int gv_utils_get_file_size(char *fname);


/* gv_utils_create_tmp_filename: Create a tmp filename based on the specified filename.
 * tmp filename has dir as prefix. dir must ends with '/' if is not "".
 * tmp filename will not contain '.gz|.Z from the specified filename.
 */
void gv_utils_create_tmp_filename(char *filename, char *dir, char *tmp_filename);


/* Note:
 * The following defitions are here for compatibility with the earlier version.
 * Applications should not use these.
 */

typedef gv_utils_filename_info_type_t filename_info_type_t;

#define get_info_from_filename gv_utils_get_info_from_filename
#define uncompress_file        gv_utils_uncompress_file
#define compress_file          gv_utils_compress_file
#define file_exist             gv_utils_file_exist
#define is_a_dir               gv_utils_is_a_dir
#define is_a_file              gv_utils_is_a_file
#define get_info_from_gauge_filename gv_utils_get_info_from_gauge_filename
#define get_file_size          gv_utils_get_file_size
#define create_tmp_filename    gv_utils_create_tmp_filename


#endif





