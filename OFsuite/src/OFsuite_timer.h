/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include <sys/time.h>
#include <signal.h>
#include <time.h>

double get_msec(void);
double get_usec(void);
void set_timer_sec(long);
void set_timer_usec(long);
void set_timer_nsec(long);
struct timespec set_outtime(long);