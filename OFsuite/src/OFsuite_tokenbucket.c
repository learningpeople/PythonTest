/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "OFsuite_tokenbucket.h"

struct token_bucket tb;
extern pthread_mutex_t mutx;
extern pthread_cond_t cond;

void init_token_bucket(void *arg) {
    struct thread_token_arg {
        uint64_t rate;
    }t_arg;
    t_arg.rate = ((struct thread_token_arg *)arg)->rate;
    tb.rate = t_arg.rate;
    tb.tokens = NONE_TOKEN;
    free(arg);
    if(tb.rate) {
        if (tb.rate == 1) {
            set_timer_sec(1);
            signal(SIGALRM, token_gen);
        }
        else {
            set_timer_nsec(1000000000/tb.rate);
            signal(SIGALRM, token_gen);
        }
    }
}

void token_gen(int signo) {

    __sync_fetch_and_add(&tb.tokens, 1);

}
