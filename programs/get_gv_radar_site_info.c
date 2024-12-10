/* 
 *
 * get_gv_radar_site_info.c: Queries the radar info from 
 *        $GVS_DATA_PATH/gv_radar_site_info.data.
 *
 *
 *--------------------------------------------------------------------------
 *
 *  By:
 *
 *     Ngoc-Thuy Nguyen
 *     Science Systems and Applications, Inc. (SSAI)
 *     NASA/TRMM Office
 *     nguyen@trmm.gsfc.nasa.gov
 *     May 4, 1998
 * 
 *     Copyright (C) 1998
 *
 ***********************************************************************/

#include <stdio.h>
#include <string.h>

#include <gv_utils.h>

typedef struct {
  char *name;
  gv_utils_radar_site_info_type_t id;
} site_fields_t;

int verbose = 0;

void usage(char *prog)
{
  if (prog == NULL)
	prog = "";

  fprintf(stderr, "Usage (%s): Get GV Radar Site Information.\n", PROG_VERSION);
  fprintf(stderr, "   %s [-v] what value\n"
                  "    where,\n"
		          "       what  = TSDIS_NAME | WSR88D_ID | CITY | STATE | COUNTRY |\n"
                  "               RADAR_NAME | LAT | LON | SPRINT_ID | ALL \n"
		          "       value = Value of the field or \"\" for ALL\n", prog);
  fprintf(stderr, "    Note: \n"
                  "      1. This program will read the data from $GVS_DATA_PATH/gv_radar_site_info.data\n"
                  "      2. All field names and values are case insensitive.\n");
  exit(-1);


}


int main (int argc, char **argv) 
{
  extern char *optarg;
  extern int optind, opterr, optopt;
  char *value, *what_str;
  int c, i;
  gv_utils_radar_site_info_t *site_info, *save_site_info;
  gv_utils_radar_site_info_type_t what_id = UNKNOWN_SITE;
  site_fields_t site_fields[] = {
	{ "TSDIS_NAME", GV_UTILS_TSDIS_NAME },
	{ "WSR88D_ID", GV_UTILS_WSR88D_ID },
	{ "CITY", GV_UTILS_CITY },
	{ "STATE", GV_UTILS_STATE },
	{ "COUNTRY", GV_UTILS_COUNTRY },
	{ "RADAR_NAME", GV_UTILS_RADAR_NAME },
	{ "LAT", GV_UTILS_LAT },
	{ "LON", GV_UTILS_LON },
	{ "SPRINT_ID", GV_UTILS_SPRINT_ID },
	{ "ALL", GV_UTILS_ALL},
	{ NULL, GV_UTILS_ALL}
  };
  if (argc < 3) 
	usage(argv[0]);
  if (argc == 3) {
	i = 1;
	goto NON_OPTIONS;
  }

  while ((c = getopt(argc, argv, ":v")) != -1 ) {
	switch (c) {
	case 'v':
	  verbose = 1;
	  break;
	case '?': fprintf(stderr, "option -%c is undefined\n", optopt);
	  usage(argv[0]);
    case ':': fprintf(stderr, "option -%c requires an argument\n",optopt);
	  usage(argv[0]);
    default: break;
    }
	if ((argc - optind) <= 2) break;
  }

  /* must have 2 parameters */
  if (argc - optind != 2) usage(argv[0]);
  i = optind;
NON_OPTIONS:
  what_str = argv[i++];
  value = argv[i++];

  if (verbose) 
	fprintf(stderr, "what: %s, value: %s\n", what_str, value);

  i = 0;
  while (site_fields[i].name != NULL) {
	if (strcasecmp(site_fields[i].name, what_str) == 0) {
	  what_id = site_fields[i].id;
	  break;
	}
	i++;
  }
	
  if (what_id == UNKNOWN_SITE) {
	fprintf(stderr, "<%s> is unknown\n", what_str);
	exit(-1);
  } 
  
  site_info = gv_utils_get_radar_site_info(what_id, value);
  save_site_info = site_info;
  while (site_info) {
	fprintf(stdout, "TSDIS_name: %-5s ", site_info->fields.tsdis_name);
	fprintf(stdout, "wsr88d_id: %-5s ", site_info->fields.wsr88d_id);
	fprintf(stdout, "city: %-15s ", site_info->fields.city);
	fprintf(stdout, "state: %-4s ", site_info->fields.state);
	fprintf(stdout, "country: %-15s ", site_info->fields.country);
	fprintf(stdout, "radar_name: %-15s ", site_info->fields.radar_name);
	fprintf(stdout, "lat: %11.6f ", site_info->fields.lat);
	fprintf(stdout, "lon: %11.6f ", site_info->fields.lon);
	fprintf(stdout, "Sprint_id: %-3d ", site_info->fields.sprint_id);
	fprintf(stdout, "\n");
	site_info = site_info->next;
  }
  gv_utils_free_radar_site_info(save_site_info);
  exit(0);
}
