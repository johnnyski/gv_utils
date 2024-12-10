/*
 * gv_utilities.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>



/****************************************************************************/
/*                                                                          */
/*                                handler                                   */
/*                                                                          */
/****************************************************************************/
void handler(int sig)
{
  /* Can overwrite this routine by defining this function in your application
   * as static.
   */
  if (sig == SIGINT || sig == SIGKILL || sig == SIGSTOP) {
	exit(-2); /* Interrupted. */
  }
  exit(-1); /* Fatal Error */
}

/****************************************************************************/
/*                                                                          */
/*                      gv_utils_set_signal_handlers                        */
/*                                                                          */
/****************************************************************************/
void gv_utils_set_signal_handlers()
{
  signal(SIGINT, handler);
  signal(SIGFPE, handler);
  signal(SIGKILL, handler);
  signal(SIGILL, handler);
  signal(SIGSTOP, handler);
  signal(SIGSEGV, handler);
}



