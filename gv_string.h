/*
 * gv_string.h
 */


#ifndef __GV_STRING_H__
#define __GV_STRING_H__ 1

/* gv_utils_str_to_lowercases: 
 * Convert str to lower cases str. Return a pointer to a new string.
 * The caller needs to deallocate memory for the new string.
 */
char *gv_utils_str_to_lowercases(char *str);

/* gv_utils_remove_leading_trailing_spaces: Remove the leading and trailing spaces
 * s will be modified. Also return the modified s.
 */
char * gv_utils_remove_leading_trailing_blanks(char *s);

/* Note:
 * The following defitions are here for compatibility with the earlier version.
 * Applications should not use these.
 */

#define str_to_lowercases gv_utils_str_to_lowercases 
#endif
