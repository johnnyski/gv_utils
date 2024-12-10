/* gv_time.c: 
 *    Contains routines to manipulate date/time(.
 * 
 *
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <IO.h>
#include <gv_time.h>

/* Accumulation of the number of days total in that month.
 */
static int julindex[13] =
{
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};
static int julindex_leap[13] =
{
  0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};
	
static int daytable[2][13] = {
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, /* non-leap year */
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  /* leap year */
};

/****************************************************************************/
/*                                                                          */
/*                gv_utils_construct_time                                   */
/*                                                                          */
/****************************************************************************/
time_t gv_utils_construct_time(int yr, int mon, int day, int hr, int min, int sec)
{
  /* yr: yyyy|yy
   * Return time in seconds
   */
  struct tm time_item;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  memset(&time_item, '\0', sizeof(struct tm));
  /* Adjust year -- The number of years since 1900.
   */
  time_item.tm_year = yr - 1900;
  /* adjust accordingly since tm_mon starts at 0 */
  time_item.tm_mon = mon - 1;
  time_item.tm_mday = day;

  time_item.tm_hour = hr;
  time_item.tm_min = min;
  time_item.tm_sec =  sec;
  time_item.tm_isdst = 0;
  /* daylight savings time flag: no adjustment. */
  time_item.tm_isdst = 0;


  return (mktime(&time_item));
}


/****************************************************************************/
/*                                                                          */
/*               gv_utils_adjust_dateNtime                                  */
/*                                                                          */
/****************************************************************************/
void gv_utils_adjust_dateNtime(DATE_STR *mdate, TIME_STR *mtime, 
					  long adjusted_sec)
{


  /* Change mdate, mtime appropriately based on adjusted_sec.
   * adjusted_min: +/- seconds.
   */
  time_t secs;
  struct tm *time_i;

  if (mdate == NULL || mtime == NULL || adjusted_sec == 0) return;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  /* Convert to secs */
  secs = gv_utils_construct_time(mdate->tkyear, mdate->tkmonth, mdate->tkday,
						mtime->tkhour, mtime->tkminute, mtime->tksecond);

  /* Add adjusted_sec */
  secs += adjusted_sec;

  /* Convert back to mdate and mtime */
  time_i = gmtime(&secs);

  if (time_i == NULL) return;
  mdate->tkyear = time_i->tm_year + 1900;
  mdate->tkmonth = time_i->tm_mon + 1;
  mdate->tkday = time_i->tm_mday;
  mtime->tkhour = time_i->tm_hour;
  mtime->tkminute = time_i->tm_min;
  mtime->tksecond = time_i->tm_sec;

} /* gv_utils_adjust_dateNtime */


/****************************************************************************/
/*                                                                          */
/*                    gv_utils_adjust_date                                  */
/*                                                                          */
/****************************************************************************/
void gv_utils_adjust_date(DATE_STR *date, int adjusted_day)
{
  /* Add adjusted_day to date. Modify date.
   */
  TIME_STR time;

  if (date == NULL) return;

  time.tkhour = 0;
  time.tkminute = 0;
  time.tksecond = 0;
  gv_utils_adjust_dateNtime(date, &time, adjusted_day * 24 * 60 * 60);

}


/*************************************************************/
/*                                                           */
/*                gv_utils_jul2cal                           */
/* This routine was copied from rsl/hdf_to_radar.c && modified  */
/*************************************************************/
void gv_utils_jul2cal(int *month, int *day, int year, int julday)
{
	/* Converts Julian date (year, julday) to calendar date (month, year).
	 * If invalid Julian date is passed to this function, it will return 
	 * a month and day of zero.
	 * year: YYYY|yy
	*/
	int j, *index;

	*month = 0;
	*day = 0;	
	
	year = gv_utils_year_2to4_digit(year);

	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
	  /* Leap year? */
	  index = julindex_leap;
	  if (julday > 366 || julday <= 0) 
		return;
	}
	else {
	  index = julindex;   /* not Leap year */
	  if (julday > 365 || julday <= 0) 
		return;
	}
	for (j=1; j<13; j++) {
	  if (julday <= index[j])
		{
		  *month = j;
		  *day = julday - index[j-1];
		  break;
		}
	}

}

/*************************************************************/
/*                                                           */
/*                gv_utils_cal2jul                           */
/*                                                           */
/*************************************************************/
void gv_utils_cal2jul(int month, int day, int year, int *julday)
{
	/* Converts calendar date to Julian date.
	 * year = YYYY|yy.
	 * It will set julday to zero if invalid date is passed in. 
	*/
	int *index;
	if (year < 0 || month < 0 || month > 12 || day < 0 ||
		day > 31) {
	  *julday = 0;
	  return;
	}

	year = gv_utils_year_2to4_digit(year);
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)  /* Leap year? */
	  index = julindex_leap;
	else   
	  index = julindex;   /* not Leap year */

	*julday = index[month - 1];

	*julday += day;
}


/****************************************************************************/
/*                                                                          */
/*                 gv_utils_date_time2system_time                           */
/*                                                                          */
/****************************************************************************/
void gv_utils_date_time2system_time(DATE_STR *date, TIME_STR *time, time_t *sys_time)
{
  struct tm tmp_tm;

  if (date == NULL || time == NULL || sys_time == NULL) return;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  memset(&tmp_tm, '\0', sizeof(time_t));
  tmp_tm.tm_year = date->tkyear -1900;
  tmp_tm.tm_mon = date->tkmonth - 1;
  tmp_tm.tm_mday = date->tkday;
  tmp_tm.tm_hour = time->tkhour;
  tmp_tm.tm_min = time->tkminute;
  tmp_tm.tm_sec = time->tksecond;
  tmp_tm.tm_isdst = 0; /* no adjustment. */
  *sys_time = mktime(&tmp_tm);
  /*
fprintf(stderr," @@@**** %d/%d/%d %d:%d %s\n", date->tkmonth, date->tkday, date->tkyear, time->tkhour, time->tkminute, ctime(sys_time));
*/
} /* date_time2system_time */



/****************************************************************************/
/*                                                                          */
/*                gv_utils_time_secs2date_and_time                          */
/*                                                                          */
/****************************************************************************/
void gv_utils_time_secs2date_and_time(time_t time_secs, DATE_STR *date, TIME_STR *time)
{
  struct tm *tmp_tm;

  if (date == NULL || time == NULL) return;
  tmp_tm = gmtime(&time_secs);
  date->tkyear = tmp_tm->tm_year + 1900;
  date->tkmonth = tmp_tm->tm_mon + 1;
  date->tkday = tmp_tm->tm_mday;
  time->tkhour = tmp_tm->tm_hour;
  time->tkminute = tmp_tm->tm_min;
  time->tksecond = tmp_tm->tm_sec;
} /* time_secs2date_and_time */


#if 0
void gv_utils_gauge_time2system_time(Gauge_time *gtime, time_t *sys_time) 
{
  struct tm tmp_tm;

  if (gtime == NULL || sys_time == NULL) return;
  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  memset(&tmp_tm, '\0', sizeof(time_t));
  tmp_tm.tm_year = gtime->year - 1900;
  tmp_tm.tm_mon = gtime->month - 1;
  tmp_tm.tm_yday = gtime->day;
  tmp_tm.tm_hour = gtime->hour; 
  tmp_tm.tm_min = gtime->minute;
  tmp_tm.tm_sec = gtime->sec;
  tmp_tm.tm_isdst = 0;
  *sys_time = mktime(&tmp_tm);

} /* gv_utils_gauge_time2system_time */
#endif

/****************************************************************************/
/*                                                                          */
/*                 gv_utils_date_time_strs2date_and_time                    */
/*                                                                          */
/****************************************************************************/
void gv_utils_date_time_strs2date_and_time(char *date_str, char *time_str,
								  DATE_STR *date, TIME_STR *time)
{
  /*  Change date and time strings to date and time.
   *    date_str: mm/dd/{yyyy|yy}
   *    time_str: hh:mm:ss
   *    If NULL is passed for either date* or time*, this routine will not set
   *    the infor for it.
   */
  int mon, day, yr, hr,min, sec;

  if (date_str != NULL && date != NULL) {
	sscanf(date_str, "%d/%d/%d", &mon, &day, &yr);
	date->tkmonth = mon;
	date->tkday = day;
	date->tkyear = gv_utils_year_2to4_digit(yr);
  }
  if (time_str != NULL && time != NULL) {
	sscanf(time_str, "%d:%d:%d", &hr, &min, &sec);
	time->tkhour = hr;
	time->tkminute = min;
	time->tksecond = sec;
  }
} /* gv_utils_date_time_strs2date_and_time */

/****************************************************************************/
/*                                                                          */
/*                gv_utils_date_time_strs2date_and_time2                    */
/*                                                                          */
/****************************************************************************/
void gv_utils_date_time_strs2date_and_time2(char *date_str, char *time_str,
								  DATE_STR *date, TIME_STR *time)
{
  /*  Change date and time strings to date and time.
   *    date_str: {yy|yyyy}-mm-dd
   *    time_str: hh:mm:ss.sss
   *    If NULL is passed for either date* or time*, this routine will not set
   *    the infor for it.
   */
  int mon, day, yr, hr,min;
  float sec;

  if (date_str != NULL && date != NULL) {
	sscanf(date_str, "%d-%d-%d", &yr, &mon, &day);
	date->tkmonth = mon;
	date->tkday = day;
	date->tkyear = gv_utils_year_2to4_digit(yr);
  }
  if (time_str != NULL && time != NULL) {
	sscanf(time_str, "%d:%d:%f", &hr, &min, &sec);
	time->tkhour = hr;
	time->tkminute = min;
	time->tksecond = (int) sec;
  }
} /* gv_utils_date_time_strs2date_and_time2 */

/****************************************************************************/
/*                                                                          */
/*                gv_utils_get_date_for_end_of_monthly_period               */
/*                                                                          */
/****************************************************************************/
void gv_utils_get_date_for_end_of_monthly_period(DATE_STR *begin_date, 
										TIME_STR *begin_time,
										DATE_STR *end_date,
										TIME_STR *end_time)
{
  /* Get the date/time for the end of the monthly period starting with
   * begin_date. Store it in end_date and end_time -- 
   * it should be the first day of the month followed the begin_date's month
   * at 00:00:00.
   */

  if (begin_date == NULL || end_date == NULL || end_time == NULL) return;

  memcpy(end_date, begin_date, sizeof(DATE_STR));

  end_date->tkmonth++;
  if (end_date->tkmonth > 12) {
	/* Jump to the next year */
    end_date->tkmonth = 1;
	end_date->tkyear++;
  }
  end_date->tkday = 1;
  memset(end_time, '\0', sizeof(TIME_STR));
} /* gv_utils_get_date_for_end_of_monthly_period */

/****************************************************************************/
/*                                                                          */
/*                gv_utils_get_date_for_end_of_granule                      */
/*                                                                          */
/****************************************************************************/
void gv_utils_set_granule_dateNtime(DATE_STR *begin_date, 
						   TIME_STR *begin_time,
						   DATE_STR *end_date,
						   TIME_STR *end_time)
{
  /* Set the date/time of a granule period that has the specified
   * begin_date and begin_time. Modify begin date/time and end date/time.
   */

  if (begin_date == NULL || end_date == NULL || end_time == NULL) return;
  /* Begin time: hr:0:0 */
  begin_time->tkminute = 0;
  begin_time->tksecond = 0;
  memcpy(end_date, begin_date, sizeof(DATE_STR));
  /* End time: hr:59:59 */
  end_time->tkhour = begin_time->tkhour;
  end_time->tkminute = 59;
  end_time->tksecond = 59;

} /* gv_utils_get_date_for_end_of_monthly_period */

/****************************************************************************/
/*                                                                          */
/*                    gv_utils_date2pentad_number                           */
/*                                                                          */
/****************************************************************************/
int gv_utils_date2pentad_number(int mon, int day, int yr)
{
  /* Return the pentad number for the specified date. */

  int julday=0, pentad=0;

  cal2jul(mon, day, yr, &julday);
  pentad = (int) (julday / 5);
  if ((julday % 5) > 0) pentad++;
  return pentad;
} /* gv_utils_date2pentad_number */



/****************************************************************************/
/*                                                                          */
/*             gv_utils_get_dates_times_for_pentad_period                   */
/*                                                                          */
/****************************************************************************/
void gv_utils_get_dates_times_for_pentad_period(DATE_STR *date, TIME_STR *time,
									   DATE_STR *begin_date, TIME_STR *begin_time,
									   DATE_STR *end_date, TIME_STR *end_time)
{
  /* Get the begin/end date/time of the pentad period for the specified
   * date/time. Pentad starts at the beginning of the year.
   * Set: begin_date to the start date of the pentad period.
   *      begin_time to 00:00:00
   *      end_date to the start date of the next pentad period.
   *      end_time to 00:00:00
   */

  int pentad;

  if (date == NULL || begin_date == NULL || begin_time == NULL ||
	  end_date == NULL || end_time == NULL) return;
  pentad = gv_utils_date2pentad_number(date->tkmonth, date->tkday, date->tkyear);
  gv_utils_pentad_number_to_datesNtimes(pentad, date->tkyear, begin_date, begin_time,
							   end_date, end_time);
} /* gv_utils_get_dates_times_for_pentad_period */


/****************************************************************************/
/*                                                                          */
/*                 gv_utils_pentad_number_to_datesNtimes                    */
/*                                                                          */
/****************************************************************************/
void gv_utils_pentad_number_to_datesNtimes(int pentad, int year,
								  DATE_STR *begin_date, TIME_STR *begin_time,
								  DATE_STR *end_date, TIME_STR *end_time)
{
  /* Get the begin/end date/time of the pentad period for the specified
   * pentad number. Pentad starts at the beginning of the year.
   * Set: begin_date to the start date of the pentad period.
   *      begin_time to 00:00:00
   *      end_date to the start date of the next pentad period.
   *      end_time to 00:00:00
   *
   *    If NULL is passed for either *date or *time, this routine will not set
   *    the infor for it.
   */
  int julday;
  int last_pentad_of_year = 73;   /* The last pentad in a year. */
  int mon, day;

  if (pentad < 1 || pentad > last_pentad_of_year) return;


  /* Get begin date/time */
  julday = ((pentad - 1) * 5) + 1; /* First day of the pentad period.*/

  gv_utils_jul2cal(&mon, &day, year, julday);
  if (begin_date) {
	begin_date->tkyear = gv_utils_year_2to4_digit(year);
	begin_date->tkmonth = mon;
	begin_date->tkday = day;
  }
  if (begin_time)
	memset(begin_time, '\0', sizeof(TIME_STR));

  if (end_date) {
	/* Get end date/time */
	if (pentad == last_pentad_of_year) {
	  /* Last pentad of the year. Set end date to next year */
	  end_date->tkyear = gv_utils_year_2to4_digit(year + 1);
	  end_date->tkmonth = 1;
	  end_date->tkday = 1;
	}	
	else {
	  /* Use the first day of the next pentad period. */
	  julday = ((pentad) * 5) + 1; /* First day of the pentad period.*/
	  gv_utils_jul2cal(&mon, &day, year, julday);
	  end_date->tkyear = gv_utils_year_2to4_digit(year);
	  end_date->tkmonth = mon;
	  end_date->tkday = day;
	}
  }
  if (end_time)
	memset(end_time, '\0', sizeof(TIME_STR));

} /* gv_utils_pentad_number_to_datesNtimes */

/****************************************************************************/
/*                                                                          */
/*                  gv_utils_date_time_strs2seconds2                        */
/*                                                                          */
/****************************************************************************/
void gv_utils_date_time_strs2seconds2(char *date_str, char *time_str, 
								time_t *time_sec)
{
/* date_time_strs2seconds2: Change date and time strings to time in secs.
 *    date_str: {yyyy|yy}-mm-dd
 *    time_str: hh:mm:ss.sss
 */
  DATE_STR date;
  TIME_STR time;

  date_time_strs2date_and_time2(date_str, time_str, &date, &time);
  date_time2system_time(&date, &time, time_sec);
}

/****************************************************************************/
/*                                                                          */
/*                  gv_utils_date_time_strs2seconds                         */
/*                                                                          */
/****************************************************************************/
void gv_utils_date_time_strs2seconds(char *date_str, char *time_str, 
								time_t *time_sec)
{
/* date_time_strs2seconds: Change date and time strings to time in secs.
 *    date_str: mm/dd/{yyyy|yy}
 *    time_str: hh:mm:ss.ss
 */
  DATE_STR date;
  TIME_STR time;

  gv_utils_date_time_strs2date_and_time(date_str, time_str, &date, &time);
  gv_utils_date_time2system_time(&date, &time, time_sec);
}

/****************************************************************************/
/*                                                                          */
/*                     gv_utils_year_2to4_digit                             */
/*                                                                          */
/****************************************************************************/
int gv_utils_year_2to4_digit(int yr)
{
  /* Convert 2 digit year to 4 digit.
   * If yr > 80, add 1900; else add 2000 to it.
   */
  if (yr >= 1000) return yr; /* already in 4 digit */
  if (yr > 80)
	yr += 1900;
  else
	yr += 2000;
  return yr;
}


/****************************************************************************/
/*                                                                          */
/*                      gv_utils_year_4to2_digit                            */
/*                                                                          */
/****************************************************************************/
int gv_utils_year_4to2_digit(int yr)
{
/* gv_utils_year_4to2_digit: COnvert 4 digit year to 2 digit.
 * If yr < 2000, subtract it with 1900; else subtract 2000 with it.
 */
  if (yr < 100) return yr; /* already in 2 digit */
	
  if (yr < 2000)
	yr -= 1900;
  else yr -= 2000;
  return yr;
}




/**********************************************************************/
/*                                                                    */
/*                  gv_utils_construct_time_from_jday                 */
/*                                                                    */
/**********************************************************************/
void gv_utils_construct_time_from_jday(int yr, int jday, int hr, 
						   int min, int sec, time_t *stime)
{
  /* Construct system time from julian day
   * yr = yy|yyyy
   */
  struct tm tmp_tm;
  int mon, day;

  if (stime == NULL) return;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  gv_utils_jul2cal(&mon, &day,  gv_utils_year_2to4_digit(yr), jday);
  memset(&tmp_tm, '\0', sizeof(time_t));
  tmp_tm.tm_year = yr - 1900;
  tmp_tm.tm_yday = jday;
  tmp_tm.tm_mon = mon - 1;
  tmp_tm.tm_mday = day;
  tmp_tm.tm_hour = hr;
  tmp_tm.tm_min = min;
  tmp_tm.tm_sec = sec;
  tmp_tm.tm_isdst = 0;
  *stime = mktime(&tmp_tm);

} /* gv_utils_construct_time_from_jday */

/**********************************************************************/
/*                                                                    */
/*                gv_utils_round_time_to_the_minute                   */
/*                                                                    */
/**********************************************************************/
void gv_utils_round_time_to_the_minute(time_t time_sec, time_t *rounded_time_sec)
{
  /* Round time to the minute -- so the second should be 0.
   */
  DATE_STR date;
  TIME_STR time;

  if (rounded_time_sec == NULL) return;
  memset(&date, '\0', sizeof(DATE_STR));
  memset(&time, '\0', sizeof(TIME_STR));
  time_secs2date_and_time(time_sec, &date, &time);
  time.tksecond = 0;
  
  gv_utils_date_time2system_time(&date, &time, rounded_time_sec); /* turn back to sec.*/
} /* gv_utils_round_time_to_the_minute */

/**********************************************************************/
/*                                                                    */
/*               gv_utils_time_secs2date_time_strs                    */
/*                                                                    */
/**********************************************************************/
void gv_utils_time_secs2date_time_strs(time_t time_sec, int show_leading_zero, 
							  int sec_flag, 
							  char *date_str, char *time_str)
{
  /* set date_str: mm/dd/yyyy 
   *     time_str: hh:mm[:ss], :ss if sec_flag is set.
   *  Note: don't add leading '0' if show_leading_zero is not set.
   */
  DATE_STR date;
  TIME_STR time;

  gv_utils_time_secs2date_and_time(time_sec, &date, &time);
  if (date_str) {
	if (show_leading_zero)
	  sprintf(date_str, "%2.2d/%2.2d/%4.4d", date.tkmonth, date.tkday, date.tkyear);
	else
	  sprintf(date_str, "%d/%d/%d", date.tkmonth, date.tkday, date.tkyear);
  }

  if (time_str) {
	if (show_leading_zero)
	  sprintf(time_str, "%2.2d:%2.2d", time.tkhour, time.tkminute);
	else
	  sprintf(time_str, "%d:%d", time.tkhour, time.tkminute);

	if (sec_flag) {
	  if (show_leading_zero)
		sprintf(time_str, "%s:%2.2d", time_str, time.tksecond);
	  else
		sprintf(time_str, "%s:%d", time_str, time.tksecond);
	}
  }
	  
} /* gv_utils_time_secs2date_time_strs( */

/**********************************************************************/
/*                                                                    */
/*              gv_utils_get_month_end_time                           */
/*                                                                    */
/**********************************************************************/
time_t gv_utils_get_month_end_time(time_t based_time)
{
  /* Return the end time of the month based on the based_time.
   *  month/last_day/year 23:59:59
   */
  struct tm *tmp_time;
  time_t etime;
  int year;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  tmp_time = gmtime(&based_time);

  year = tmp_time->tm_year+1900;
  if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) 
	tmp_time->tm_mday = daytable[1][tmp_time->tm_mon+1];
  else
	tmp_time->tm_mday = daytable[0][tmp_time->tm_mon+1];

  tmp_time->tm_sec = 59;
  tmp_time->tm_min = 59;
  tmp_time->tm_hour = 23;
  /* daylight savings time flag: no adjustment. */
  tmp_time->tm_isdst = 0;

  etime = mktime(tmp_time);
  return (etime);
} /* gv_utils_get_month_end_time */

/**********************************************************************/
/*                                                                    */
/*              gv_utils_get_month_begin_time                         */
/*                                                                    */
/**********************************************************************/
time_t gv_utils_get_month_begin_time(time_t based_time)
{
  /* Return the beginning time of the month based on the based_time.
   *  month/1/year 0:0:0
   */
  struct tm *tmp_time;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  tmp_time = gmtime(&based_time);
  tmp_time->tm_sec = 0;
  tmp_time->tm_min = 0;
  tmp_time->tm_hour = 0;
  tmp_time->tm_mday = 1;
  /* daylight savings time flag: no adjustment. */
  tmp_time->tm_isdst = 0;


  return (mktime(tmp_time));
} /* gv_utils_get_month_begin_time */

/**********************************************************************/
/*                                                                    */
/*                     gv_utils_get_month_year_for_time               */
/*                                                                    */
/**********************************************************************/
void gv_utils_get_month_year_for_time(time_t time_sec, int *mon, int *year)
{
  /* Get month and year (4digits) for the specified time.
   */
  struct tm *tmp_time;

  putenv("TZ=UT"); /* No local time daylight savings adjustment. */
  tmp_time = gmtime(&time_sec);
  if (mon)
	*mon = tmp_time->tm_mon+1;
  if (year)
	*year = tmp_time->tm_year + 1900;
} /* gv_utils_get_month_for_time */
