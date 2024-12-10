/* 
 * gv_utilities.h
 *
 */

#ifndef __GV_UTILITIES_H__
#define __GV_UTILITIES_H__ 1


typedef enum {
   P1B51, P1C51, P2A52, P2A53, P2A54, P2A55, P2A56, P2A57, P3A53, P3A54, P3A55
} gv_utils_product_type_t;


/* gv_utils_set_signal_handlers: set signals: SIGINT, SIGFPE, SIGKILL, SIGILL,
 * SIGSTOP, SIGSEGV
 * the handler function is void handler(int sig) -- can be redefined by 
 * the caller (make sure to define it as 'static' in your application).
 */
void gv_utils_set_signal_handlers();



/* Note:
 * The following defitions are here for compatibility with the earlier version.
 * Applications should not use these.
 */

typedef gv_utils_product_type_t product_type_t;

#define set_signal_handlers gv_utils_set_signal_handlers 



#endif
