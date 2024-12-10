/* 
 * construct_time_in_seconds.c
 *     Construct time in seconds and write the result to stdout.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "gv_time.h"

int main(int argc, char **argv)
{
  int yr, mon, day, hr, min, sec;
  int i;
  time_t time_secs;

  if (argc != 7) {
  USAGE:
	fprintf(stderr, "Construct time in seconds and write the result to stdout.\n");
	fprintf(stderr,"Usage (%s)\n", PROG_VERSION);
	fprintf(stderr, " %s hour minute second month day year\n", argv[0]);
	exit(-1);
  }
  i = 1;
  hr = atoi(argv[i++]);
  min = atoi(argv[i++]);
  sec = atoi(argv[i++]);
  mon = atoi(argv[i++]);
  day = atoi(argv[i++]);
  yr = atoi(argv[i]);
  if (strlen(argv[i]) == 2)
    if (yr > 80)
	yr += 1900;
    else
	yr += 2000;
  time_secs = gv_utils_construct_time(yr, mon, day, hr, min, sec);
  fprintf(stdout, "%d\n", time_secs);
  exit(0);
}






