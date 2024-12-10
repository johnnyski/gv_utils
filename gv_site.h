/*
 * gv_site.h
 *
 */

#ifndef __GV_SITE_H__
#define __GV_SITE_H__ 1

#define DEG2FLOAT(deg, min, sec)  (deg + min/60.0 + sec/3600.0)

typedef enum {
  GV_UTILS_SINGLE_RADAR /*, GV_UTILS_MULTI_RADAR */
}  gv_utils_radar_type_t;


#define GV_UTILS_MAX_NAME_LEN 256
typedef struct {
  char  tsdis_name[GV_UTILS_MAX_NAME_LEN];
  char  wsr88d_id[GV_UTILS_MAX_NAME_LEN];
  char  city[GV_UTILS_MAX_NAME_LEN];
  char  state[GV_UTILS_MAX_NAME_LEN];
  char  country[GV_UTILS_MAX_NAME_LEN]; 
  char  radar_name[GV_UTILS_MAX_NAME_LEN];
  float lat;
  float lon;
  int   sprint_id;
} gv_utils_radar_site_fields_t;

typedef struct _gv_utils_radar_site_info_t{
  gv_utils_radar_site_fields_t fields;
  struct _gv_utils_radar_site_info_t *next;
} gv_utils_radar_site_info_t;

typedef enum {
  UNKNOWN_SITE=-1,
  GV_UTILS_TSDIS_NAME, GV_UTILS_WSR88D_ID, GV_UTILS_CITY, GV_UTILS_STATE, 
  GV_UTILS_COUNTRY, GV_UTILS_RADAR_NAME, GV_UTILS_LAT, GV_UTILS_LON, 
  GV_UTILS_SPRINT_ID,  GV_UTILS_ALL
} gv_utils_radar_site_info_type_t;


#define GV_MAX_SITES                     17 /* Number of radar sites */

/* gv_utils_get_radar_site_info: 
 * Get radar site info based on 'what' and its value from 
 * $GVS_DATA_PATH/gv_radar_site_info.data. 
 * Return an allocated structure contained all of the matched entries from that data file 
 * or NULL for error or not found.
 * Note: If 'what' is 'ALL', this routine will select every entry from the file.
 * The caller should free the memory of the returned structure by calling
 * gv_utils_free_radar_site_info().
 */
gv_utils_radar_site_info_t *
gv_utils_get_radar_site_info(gv_utils_radar_site_info_type_t what, char *value);
/* gv_utils_free_radar_site_info: Free memory for site_info list 
 */
void gv_utils_free_radar_site_info(
								   gv_utils_radar_site_info_t *site_info);

/* gv_utils_site_name2id: Convert site name to id (defined in the toolkit lib.).
 * Return site id if site name is defined; -1, otherwise.
 */
int gv_utils_site_name2id(char *name);

/* gv_utils_get_grid_origin_lat_lon:
 * Get grid origin's lat and lon for the specified tsdis_name.  The
 * origin depends on type.
 * Return 1 for successful; -1, otherwise.
 */
int gv_utils_get_grid_origin_lat_lon(char *tsdis_name, 
									 gv_utils_radar_type_t type,
									 float *lat, float *lon);

/* Note:
 * The following defitions are here for compatibility with the earlier version.
 * Applications should not use these.
 */

#define site_name2id gv_utils_site_name2id 

#endif
