/*
 * gv_string.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/**********************************************************************/
/*                                                                    */
/*               gv_utils_str_to_lowercases                          */
/*                                                                    */
/**********************************************************************/
char *gv_utils_str_to_lowercases(char *str)
{
  /* Convert str to lower cases str. Return a pointer to a new string.
   * The caller needs to deallocate memory for the new string.
   */
  int i;
  char *new_str;

  if (str == NULL) return NULL;

  new_str = (char *) strdup(str);
  if (new_str == NULL) return NULL;
  
  for (i = 0; i < strlen(str); i++) {
	if (isupper(str[i])) 
	  new_str[i] = tolower(str[i]);
  }
  return new_str;
}


/***************************************************************************/
/*                                                                         */
/*                     gv_utils_remove_leading_trailing_blanks                     */
/*                                                                         */
/***************************************************************************/
char * gv_utils_remove_leading_trailing_blanks(char *s)
{
/* remove_leading_trailing_spaces: Remove the leading and trailing spaces
 * s will be modified. Also return the modified s.
 */
  int i;

  /* remove leading blanks */
  while (s) {
	if (*s == ' ') s++; 
	else break;
  }
  i = strlen(s) - 1;

  while (i >= 0) {
	if (s[i] == ' ')
	  s[i] = '\0';
	else
	  break;
	i--;
  }
  return s;

}
