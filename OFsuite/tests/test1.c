/* file minunit_example.c */
#include <stdio.h>
#include "../src/minunit.h"
#include "../src/flow.h"


 
 //int tests_run = 0;

struct ofp11_match* match = NULL;
struct ofp11_instruction_actions* instructions = NULL;
struct flow_entry flow_entry;

void init_elements()
{
	match = (struct ofp11_match*)malloc(sizeof(struct ofp11_match));
	memset(match,0, sizeof(struct ofp11_match));
	instructions = (struct ofp11_instruction_actions*)malloc(sizeof(struct ofp11_instruction_actions));
	memset(instructions,0, sizeof(struct ofp11_instruction_actions));
	flow_entry.cookie = 1001;
	flow_entry.cookie_mask = 5050;
	flow_entry.table_id = 1;
	flow_entry.match = (void*)&match;
}

//struct flows* flows = init_flows();

static char* test_init_flows()
{
	mu_assert(init_flows() != NULL, "Init failed");
	return 0;
}

static char* test_free_flows()
{
	struct flows* flows = init_flows();
	free_flows(flows);
	printf("%ld\n",(long)flows);
	mu_assert(flows->flow_entry_head == NULL, "Free failed");
	return 0;
}

static char* test_add_flow_entry()
{
	struct flows* flows = init_flows();
	mu_assert(add_flow_entry(flows, &flow_entry) == 1, "add flow entry failed");
	mu_assert(add_flow_entry(flows, &flow_entry) == 2, "add flow entry failed");
	return 0;
}


 int foo = 7;
 int bar = 4;
 
 static char * test_foo() {
     mu_assert(foo == 7, "error, foo != 7");
     return 0;
 }
 
 static char * test_bar() {
     mu_assert(bar == 5, "error, bar != 5");
     return 0;
 }
 
 char *all_tests() {
    mu_suite_start();
    init_elements();
    mu_run_test(test_foo);
    mu_run_test(test_init_flows);
    mu_run_test(test_free_flows);
    mu_run_test(test_add_flow_entry);
    flow_entry_print(&flow_entry);
    mu_run_test(test_bar);
    return NULL;
}

RUN_TESTS(all_tests);
 