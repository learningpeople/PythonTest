/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include <stdlib.h>
#include <stdio.h>
#include "../OFsuite_performance.h"
#include "../src/OFsuite_parser.h"
#include "../src/OFsuite_log.h"

int main(int argc, char *argv[]) {

	OFsuite_log_init(LOG_FLOW_MOD_AUXI_RATE, "OFsuite_flow_setup_auxi_rate");
    parse_command(argc, argv);
    init_controller();
    fakesw_create_connect();
    parse_user_command();
    OFsuite_exit();
    OFsuite_log_close();
    return 0;
}