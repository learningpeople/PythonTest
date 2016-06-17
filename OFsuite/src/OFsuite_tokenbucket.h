/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */


#ifndef OFSUITE_TOKENBUCKET_H
#define OFSUITE_TOKENBUCKET_H 1

#include <limits.h>
#include <stdbool.h>
#include <signal.h>
#include <stdint.h>
#include "OFsuite_timer.h"


#define NONE_TOKEN 0

struct token_bucket {
    uint64_t rate;
    uint16_t tokens;
};

void init_token_bucket(void *);
void set_token_bucket(struct token_bucket *,  unsigned int, unsigned int);
bool withdraw_token_bucket(struct token_bucket *, unsigned int);
void token_gen(int);


#endif