/*
 * gv_time.h
 *     Contains routines to manipulate date/time.
 */


#ifndef __GV_TIME_H__
#define __GV_TIME_H__ 1

#include <IO.h>


/* gv_utils_construct_time: Construct time. Note: yr = {yy|yyyy}.
 */
time_t gv_utils_construct_time(int yr, int mon, int day, int hr, int min, int sec);

/* gv_utils_construct_time_from_jday: Construct time in second from julian day 
 *  yr = yy|yyyy
 **/
void gv_utils_construct_time_from_jday(int yr, int jday, int hr, 
						   int min, int sec, time_t *stime);


/* gv_utils_adjust_dateNtime: Change mdate, mtime appropriately based on adjusted_sec.
 * adjusted_min: +/- seconds.
 */
void gv_utils_adjust_dateNtime(DATE_STR *mdate, TIME_STR *mtime, 
					  long adjusted_sec);


/* gv_utils_adjust_date: Add adjusted_day to date. Modify date.
 */
void gv_utils_adjust_date(DATE_STR *date, int adjusted_day);

/* gv_utils_jul2cal: 
 * Converts Julian date (year, julday) to calendar date (month, year).
 * If invalid Julian date is passed to this function, it will return 
 * a month and day of zero. 
 * year: yy|YYYY
 */
void gv_utils_jul2cal(int *month, int *day, int year, int julday);

/* gv_utils_cal2jul: Converts calendar date to Julian date.
 * year = yy|YYYY.
 * It will set julday to zero if invalid date is passed in. 
 */
void gv_utils_cal2jul(int month, int day, int year, int *julday);

/* gv_utils_date_time2system_time:
 * Change date and time to unix time (seconds).  sys_time must be allocated 
 * by caller.
 */
void gv_utils_date_time2system_time(DATE_STR *date, TIME_STR *time, time_t *sys_time);


/* gv_utils_time_secs2date_and_time: Change time in seconds (unix time) to date and
 * time.
 */
void gv_utils_time_secs2date_and_time(time_t time_secs, DATE_STR *date, TIME_STR *time);

/* gv_utils_date_time_strs2date_and_time: Change date and time strings to date and time.
 *    date_str: mm/dd/{yyyy|yy}
 *    time_str: hh:mm:ss
 */
void gv_utils_date_time_strs2date_and_time(char *date_str, char *time_str,
								  DATE_STR *date, TIME_STR *time);

/* gv_utils_date_time_strs2date_and_time2: Change date and time strings to date and 
 * time.
 *    date_str: {yy|yyyy}-mm-dd
 *    time_str: hh:mm:ss.sss
 */
void gv_utils_date_time_strs2date_and_time2(char *date_str, char *time_str,
								  DATE_STR *date, TIME_STR *time);

/* gv_utils_date_time_strs2seconds: Change date and time strings to time in secs.
 *    date_str: mm/dd/{yyyy|yy}
 *    time_str: hh:mm:ss.ss
 */
void gv_utils_date_time_strs2seconds(char *date_str, char *time_str, 
								time_t *time_sec);


/* gv_utils_date_time_strs2seconds2: Change date and time strings to time in secs.
 *    date_str: {yy}yyyy}-mm-dd
 *    time_str: hh:mm:ss.sss
 */
void gv_utils_date_time_strs2seconds2(char *date_str, char *time_str, time_t *time_sec);

/* gv_utils_get_date_for_end_of_monthly_period:
 * Get the date/time for the end of the monthly period starting with
 * begin_date. Store it in end_date and end_time -- 
 * it should be the first day of the month followed the begin_date's month
 * at 00:00:00.
 */

void gv_utils_get_date_for_end_of_monthly_period(DATE_STR *begin_date, TIME_STR *begin_time, 
		DATE_STR *end_date, TIME_STR *end_time);

/* gv_utils_date2pentad_number: Return the pentad number for the specified date. */
int gv_utils_date2pentad_number(int mon, int day, int yr);

/* gv_utils_set_granule_dateNtime: 
 *   Set the date/time of a granule period that has the specified
 *   begin_date and begin_time. Modify begin date/time and end date/time.
 */
void gv_utils_set_granule_dateNtime(DATE_STR *begin_date, 
						   TIME_STR *begin_time,
						   DATE_STR *end_date,
						   TIME_STR *end_time);

/* gv_utils_get_dates_times_for_pentad_period:
 * Get the begin/end date/time of the pentad period for the specified
 * date/time. Pentad starts at the beginning of the year.
 * Set: begin_date to the start date of the pentad period.
 *      begin_time to 00:00:00
 *      end_date to the start date of the next pentad period.
 *      end_time to 00:00:00
 */
void gv_utils_get_dates_times_for_pentad_period(DATE_STR *date, TIME_STR *time,
			   DATE_STR *begin_date, TIME_STR *begin_time,
			   DATE_STR *end_date, TIME_STR *end_time);



/* gv_utils_pentad_number_to_datesNtimes:
 * Get the begin/end date/time of the pentad period for the specified
 * pentad number. Pentad starts at the beginning of the year.
 * Set: begin_date to the start date of the pentad period.
 *      begin_time to 00:00:00
 *      end_date to the start date of the next pentad period.
 *      end_time to 00:00:00
 *    If NULL is passed for either *date or *time, this routine will not set
 *    the infor for it.
 */
void gv_utils_pentad_number_to_datesNtimes(int pentad, int year,
								  DATE_STR *begin_date, TIME_STR *begin_time,
								  DATE_STR *end_date, TIME_STR *end_time);

/* gv_utils_year_2to4_digit: COnvert 2 digit year to 4 digit.
 * If yr > 80, add 1900; else add 2000 to it.
 */
int gv_utils_year_2to4_digit(int yr);

/* gv_utils_year_4to2_digit: COnvert 4 digit year to 2 digit.
 * If yr < 2000, subtract it with 1900; else subtract 2000 with it.
 */
int gv_utils_year_4to2_digit(int yr);

/* gv_utils_round_time_to_the_minute:
 * Round time to the minute -- so the second should be 0.
 */
void gv_utils_round_time_to_the_minute(time_t time_sec, time_t *rounded_time_sec);

/*gv_utils_time_secs2date_time_strs: set date_str: mm/dd/yyyy
 *     time_str: hh:mm[:ss], :ss if sec_flag is set.
 *  Note: don't add leading '0' if show_leading_zero is not set.
 */
void gv_utils_time_secs2date_time_strs(time_t time_sec, int show_leading_zero, 
							  int sec_flag, 
							  char *date_str, char *time_str);
 /* gv_utils_get_month_begin_time:
  * Return the beginning time of the month based on the based_time.
  *  month/1/year 0:0:0
  */
time_t gv_utils_get_month_begin_time(time_t based_time);

/* gv_utils_get_month_end_time:
 * Return the end time of the month based on the based_time.
 *  month/last_day/year 23:59:59
 */
time_t gv_utils_get_month_end_time(time_t based_time);

/* gv_utils_get_month_year_for_time:
 * Get month and year (4digits) for the specified time. mon or year can be NULL.
 */
void gv_utils_get_month_year_for_time(time_t time_sec, int *mon, int *year);

/* Note:
 * The following defitions are here for compatibility with the earlier version.
 * Applications should not use these.
 */
#define construct_time gv_utils_construct_time
#define construct_time_from_jday gv_utils_construct_time_from_jday
#define adjust_dateNtime gv_utils_adjust_dateNtime
#define adjust_date gv_utils_adjust_date
#define jul2cal gv_utils_jul2cal
#define cal2jul gv_utils_cal2jul
#define date_time2system_time gv_utils_date_time2system_time
#define time_secs2date_and_time gv_utils_time_secs2date_and_time
#define date_time_strs2date_and_time gv_utils_date_time_strs2date_and_time
#define date_time_strs2date_and_time2 gv_utils_date_time_strs2date_and_time2
#define date_time_strs2seconds gv_utils_date_time_strs2seconds
#define date_time_strs2seconds2 gv_utils_date_time_strs2seconds2
#define get_date_for_end_of_monthly_period gv_utils_get_date_for_end_of_monthly_period
#define date2pentad_number gv_utils_date2pentad_number
#define set_granule_dateNtime gv_utils_set_granule_dateNtime
#define get_dates_times_for_pentad_period gv_utils_get_dates_times_for_pentad_period
#define pentad_number_to_datesNtimes gv_utils_pentad_number_to_datesNtimes
#define year_2to4_digit gv_utils_year_2to4_digit
#define year_4to2_digit gv_utils_year_4to2_digit
#define round_time_to_the_minute gv_utils_round_time_to_the_minute
#define time_secs2date_time_strs gv_utils_time_secs2date_time_strs


#endif
