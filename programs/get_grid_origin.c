#include <stdlib.h>
#include <stdio.h>
#include "gv_utils.h"

main(int argc, char **argv)
{
  char *site;
  float lat=-1.0, lon=-1.0;

  if (argc < 2) {
	fprintf(stderr, "Usage(%s): Get GV Radar Grid Origin.\n", PROG_VERSION);
	fprintf(stderr, "  %s tsdis_site\n", argv[0]);
	fprintf(stderr, "  where,\n");
	fprintf(stderr, "    tsdis_site is from $GVS_DATA_PATH/gv_radar_site_info.data\n");
	exit(-1);
  }
  site = argv[1];
  gv_utils_get_grid_origin_lat_lon(site, GV_UTILS_SINGLE_RADAR, &lat, &lon);
  fprintf(stdout, "LAT = %f; LON = %f\n", lat, lon);
  exit(0);
}
