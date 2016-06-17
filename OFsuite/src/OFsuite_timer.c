/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include "OFsuite_timer.h"

double get_msec(void) {

    struct timeval tv;
    double mseconds;
    gettimeofday(&tv,NULL);
    mseconds = (tv.tv_sec * 1000000 + tv.tv_usec)/1000.0;
    return mseconds;
}

double get_usec(void) {

    struct timeval tv;
    double useconds;
    gettimeofday(&tv,NULL);
    useconds = (tv.tv_sec * 1000000 + tv.tv_usec);
    return useconds;
}

void set_timer_sec(long second) {
  	struct itimerval itv;  
  	itv.it_interval.tv_sec = second;
  	itv.it_interval.tv_usec = 0;
  	itv.it_value.tv_sec = second;
  	itv.it_value.tv_usec = 0;
  	setitimer(ITIMER_REAL, &itv, NULL);
}

void set_timer_usec(long usecond) {
    struct itimerval itv;  
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = usecond;
    itv.it_value.tv_sec = 0;  
    itv.it_value.tv_usec = usecond;
    setitimer(ITIMER_REAL, &itv, NULL);
}

void set_timer_nsec(long nsecond) {
    struct itimerspec its;
    timer_t timerid;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = nsecond;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = nsecond;
    timer_create(CLOCK_REALTIME, NULL, &timerid);
    timer_settime(timerid, CLOCK_REALTIME, &its, NULL);
}

struct timespec set_outtime(long second) {
    struct timeval now;
    struct timespec outtime;
    gettimeofday(&now, NULL);
    outtime.tv_sec = now.tv_sec + second;
    outtime.tv_nsec = now.tv_usec * 1000;
    return outtime;
}