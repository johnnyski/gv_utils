/*
 * get_site_info_from_raw_file.c: Gets site info from radar raw data file.
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
 *     May 19, 1998
 * 
 *     Copyright (C) 1998
 *
 ***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rsl.h>
#include "gv_utils.h"

int main (int argc, char **argv)
{
  Radar *radar;
  char *infile=NULL, *callid = NULL;
  int i;

  if (argc < 2) {
	fprintf(stderr, "Usage (%s): Get GV Radar Site Information from Radar File.\n", PROG_VERSION);
	fprintf(stderr, "  %s infile [callid]\n", argv[0]);
	exit(-1);
  }
  i = 1;
  infile = argv[i++];
  if (i < argc)
	callid = argv[i];
  radar = RSL_anyformat_to_radar(infile, callid);
  if (radar == NULL) {
	fprintf(stderr, "Radar is null\n");
	exit(-1);
  }
  fprintf(stdout, "Radar type: %s\n", radar->h.radar_type);
  fprintf(stdout, "Radar name: %s\n", radar->h.radar_name);
  fprintf(stdout, "Name:       %s\n", radar->h.name);
  fprintf(stdout, "city:       %s\n", radar->h.city);
  fprintf(stdout, "state       %s\n", radar->h.state);
  fprintf(stdout, "country:    %s\n", radar->h.country);
  fprintf(stdout, "lat:        %.6f\n", DEG2FLOAT(radar->h.latd, radar->h.latm,
												  radar->h.lats));
  fprintf(stdout, "lon:        %.6f\n", DEG2FLOAT(radar->h.lond, radar->h.lonm,
												  radar->h.lons));
  RSL_free_radar(radar);
  exit(0);
} 



