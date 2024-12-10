/*
 * gv_site.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <IO.h>
#include "gv_utils.h"

#define MAX_LINE_LEN 300 
/****************************************************************************/
/*                                                                          */
/*                          gv_utils_site_name2id                           */
/*                                                                          */
/****************************************************************************/
int gv_utils_site_name2id(char *name)
{
  /* Convert site name to id (defined in the toolkit lib.).
   * Return site id if site name is defined; -1, otherwise.
   */
  int id = -1;
  typedef struct _site_item {
	int id;
	char *name;
  } site_item_t;

#ifndef TK_DARB
#define TK_DARB TK_DARW
#endif

  site_item_t site_list[GV_MAX_SITES] = {
	{TK_DARB, "DARB"},
	{TK_DARW, "DARW"},
	{TK_MELB, "MELB"},
	{TK_MELB, "KMLB"},
	{TK_KWAJ, "KWAJ"},
	{TK_HSTN, "HSTN"},
	{TK_THOM, "THOM"},
	{TK_GUAM, "GUAM"},
	{TK_ISBN, "ISBN"},
	{TK_SAOP, "SAOP"},
	{TK_TWWF, "TWWF"},
	{TK_TAMP, "TAMP"},
	{TK_MIAM, "MIAM"},
	{TK_KEYW, "LKCH"},
	{TK_LKCH, "LKCH"},
	{TK_NWBR, "NWBR"},
	{TK_CORC, "CORC"}
  };

  int i;

  if (name == NULL) return id;

  for (i = 0; i < GV_MAX_SITES; i++) {
	if (site_list[i].name == NULL) break;
	if (strcmp(name, site_list[i].name) == 0) {
	  id = site_list[i].id;
	  break;
	}
  }
  return id;
} /* gv_utils_site_name2id */




/****************************************************************************/
/*                                                                          */
/*                      gv_utils_get_radar_site_info                        */
/*                                                                          */
/****************************************************************************/
gv_utils_radar_site_info_t *
gv_utils_get_radar_site_info(gv_utils_radar_site_info_type_t what,
							 char *value)
{
  /* Get radar site info based on 'what' and its value from 
   * $GVS_DATA_PATH/gv_radar_site_info.data. 
   * Return an allocated structure contained all of the matched entries from 
   * that data file or NULL for error.
   * Note: If 'what' is  'ALL', this routine will select every
   * entry from the file. The caller should free the memory of the returned 
   * structure by calling gv_utils_free_radar_site_info().
   */
  gv_utils_radar_site_info_t *last_site_info = NULL, *site_info = NULL,
	*new_site_info = NULL;
  char fname[MAX_FILENAME_LEN];
  char *path, *tok;
  FILE *fp;
  char line[MAX_LINE_LEN];
  char tsdis_name[GV_UTILS_MAX_NAME_LEN], wsr88d_id[GV_UTILS_MAX_NAME_LEN],
	city[GV_UTILS_MAX_NAME_LEN], state[GV_UTILS_MAX_NAME_LEN], 
	country[GV_UTILS_MAX_NAME_LEN], radar_name[GV_UTILS_MAX_NAME_LEN];
  float lat, lon, fval;
  int sprint_id, ival;
  int match;

  if (value == NULL && what != GV_UTILS_ALL) return NULL;
  path = getenv("GVS_DATA_PATH");
  if (path == NULL)
	path = "/usr/local/trmm/GVBOX";
  memset(fname, '\0', MAX_FILENAME_LEN);
  sprintf(fname, "%s/gv_radar_site_info.data", path);
  if ((fp = fopen(fname, "r")) == NULL) {
	fprintf(stderr, "Error: Failed to open file <%s>\n", fname);
	return NULL;
  }
  while (!feof(fp)) {
	/* Line's format: 
	 *  TSDIS_Name, WSR88D_ID, City, State, Country, radar->h.name, Lat, Lon, SPRINT_radar_ID
	 */
	memset(line, '\0', MAX_LINE_LEN);
	fgets(line, MAX_LINE_LEN, fp);
	if (line[0] == '#' || strlen(line) == 0)
	  continue;  
	/*
fprintf(stderr, "line<<%s>>\n", line);
*/
	if (line[strlen(line)-1] == '\n')
	  line[strlen(line)-1] = '\0';

	memset(tsdis_name, '\0', GV_UTILS_MAX_NAME_LEN);
	memset(wsr88d_id, '\0', GV_UTILS_MAX_NAME_LEN);
	memset(city, '\0', GV_UTILS_MAX_NAME_LEN);
	memset(state, '\0', GV_UTILS_MAX_NAME_LEN);
	memset(country, '\0', GV_UTILS_MAX_NAME_LEN);
	memset(radar_name, '\0', GV_UTILS_MAX_NAME_LEN);
	lat = 0.0;
	lon = 0.0;
	sprint_id = 0;

	/* Can't use sscanf since ',' is considered part of a string. 
	 */
	/* TSDIS name */
	tok = strtok(line, ",");
	if (tok)
	  strcpy(tsdis_name, gv_utils_remove_leading_trailing_blanks(tok));
	else
	  continue;
	/* wsr88d ID */
	tok = strtok(NULL, ",");
	if (tok)
	  strcpy(wsr88d_id, gv_utils_remove_leading_trailing_blanks(tok));
	else
	  continue;
	/* City */
	tok = strtok(NULL, ",");
	if (tok)
	  strcpy(city, gv_utils_remove_leading_trailing_blanks(tok));
	else
	  continue;
	/* State */
	tok = strtok(NULL, ",");
	if (tok)
	  strcpy(state, gv_utils_remove_leading_trailing_blanks(tok));
	else
	  continue;
	/* Country */
	tok = strtok(NULL, ",");
	if (tok)
	  strcpy(country, gv_utils_remove_leading_trailing_blanks(tok));
	else
	  continue;
	/* Radar Name */
	tok = strtok(NULL, ",");
	if (tok)
	  strcpy(radar_name, gv_utils_remove_leading_trailing_blanks(tok));
	else
	  continue;
	/* Lat */
	tok = strtok(NULL, ",");
	if (tok)
	  lat = atof(tok);
	else
	  continue;
	/* Lon */
	tok = strtok(NULL, ",");
	if (tok) {
	  gv_utils_remove_leading_trailing_blanks(tok);
	  lon = atof(tok);
	}
	else
	  continue;
	/* SPRINT ID */
	tok = strtok(NULL, ",");
	if (tok) {
	  gv_utils_remove_leading_trailing_blanks(tok);
	  sprint_id = atoi(tok);
	}
	else
	  continue;
	/*
fprintf(stderr, "what: %d, value %s\n", what, (char *) value);
*/
	match = 0;
	switch (what) {
	case GV_UTILS_TSDIS_NAME:
	  if (strcasecmp(tsdis_name, (char *) value) == 0)
		match = 1;
	  break;
	case GV_UTILS_WSR88D_ID:
	  if (strcasecmp(wsr88d_id, (char *) value) == 0)
		match = 1;
	  break;
	case GV_UTILS_CITY:
	  if (strcasecmp(city, (char *) value) == 0)
		match = 1;
	  break;
	case GV_UTILS_STATE:
	  if (strcasecmp(state, (char *) value) == 0)
		match = 1;
	  break;
	case GV_UTILS_COUNTRY:
	  if (strcasecmp(country, (char *) value) == 0)
		match = 1;
	  break;
	case GV_UTILS_RADAR_NAME:
	  if (strcasecmp(radar_name, (char *) value) == 0)
		match = 1;
	  break;
	case GV_UTILS_LAT:
	  fval = atof((char *) value);
	  if (lat == fval)
		match = 1;
	  break;
	case GV_UTILS_LON:
	  fval = atof((char *) value);
	  if (lon == fval) 
		match = 1;
	  break;
	case GV_UTILS_SPRINT_ID:
	  ival = atoi((char *) value);
	  if (sprint_id == ival)
		match = 1;
	  break;
	case GV_UTILS_ALL:
	  match = 1;
	  break;
	default:
	  goto DONE;
	  break;
	}
	if (match) {
	  /* Create a new list entry */
	  new_site_info = (gv_utils_radar_site_info_t*) 
		calloc(1, sizeof(gv_utils_radar_site_info_t));
	  if (new_site_info == NULL) {
		perror("Calloc gv_utils_radar_site_info_t");
		gv_utils_free_radar_site_info(site_info);
		fclose(fp);
		return NULL;
	  }
	  /* Set fields' values */
	  strcpy(new_site_info->fields.tsdis_name, tsdis_name);
	  strcpy(new_site_info->fields.wsr88d_id, wsr88d_id);
	  strcpy(new_site_info->fields.city, city);
	  strcpy(new_site_info->fields.state, state);
	  strcpy(new_site_info->fields.country, country);
	  strcpy(new_site_info->fields.radar_name, radar_name);
	  new_site_info->fields.lat = lat;
	  new_site_info->fields.lon = lon;
	  new_site_info->fields.sprint_id = sprint_id;

	  /* Add new entry to list */
	  new_site_info->next = NULL;
	  if (site_info == NULL) {
		/* List does not exist yet. */
		site_info = new_site_info;
		last_site_info = site_info;
	  }
	  else {
		/* List exists */
		last_site_info->next = new_site_info;
		last_site_info = new_site_info;
	  }
	}
  } /* while */
DONE:
  fclose (fp);
  return site_info;
} /* gv_utils_get_radar_site_info */

/****************************************************************************/
/*                                                                          */
/*                     gv_utils_free_radar_site_info                        */
/*                                                                          */
/****************************************************************************/
void gv_utils_free_radar_site_info(
								   gv_utils_radar_site_info_t *site_info)
{
  /* Free memory for list. */
  gv_utils_radar_site_info_t *curr_site_info;

  while (site_info) {
	curr_site_info = site_info;
	site_info = site_info->next;
	free(curr_site_info);
  }
}  /* gv_utils_free_radar_site_info */


/****************************************************************************/
/*                                                                          */
/*                     gv_utils_get_grid_origin_lat_lon                     */
/*                                                                          */
/****************************************************************************/

int gv_utils_get_grid_origin_lat_lon(char *tsdis_name, 
									 gv_utils_radar_type_t type,
									 float *lat, float *lon)
{
  /* Get grid origin's lat and lon for the specified tsdis_name.  The
   * origin depends on type.
   * Return 1 for successful; -1, otherwise.
   */
  int rc = -1;
  gv_utils_radar_site_info_t *site_info;

  if (tsdis_name == NULL || lat == NULL || lon == NULL) return -1;

  site_info = gv_utils_get_radar_site_info(GV_UTILS_TSDIS_NAME, tsdis_name);
  if (site_info == NULL) 
	return -1;
  switch (type) {
  case GV_UTILS_SINGLE_RADAR:
	/* Get the lat,lon from the first entry found in list.
	 */
	*lat = site_info->fields.lat;
	*lon = site_info->fields.lon;
	rc = 1;
	break;
  default:
	break;
  }
  gv_utils_free_radar_site_info(site_info);
  return rc;
} /* gv_utils_get_grid_origin_lat_lon */


