OFsuite_performance-Openflow SDN Controller Performance Test Tool

OFsuite is a software suite of Openflow SDN test tools. Currenty it consists of Openflow 1.3 conformance test module and Openflow SDN controller performance test module. The code here is written especially for the latter module.

This test tool aims to perform benchmarking test for SDN controllers which speak Openflow protocol. To be specific, this test tool simulates a topology consists of thousands of Openflow switches and all kinds of Openflow/SDN network events, measure how much time does a device under test(Openflow SDN controller) take in order to finish its specific task.

The test cases were designed based on real SDN deployment scenario, emphasize on the controller's process capability of various kinds of Openflow messages, such as Packet_in, Packet_out, flow_mod and status messages. The test result will provide the controller/network user with quantitative performance analysis of the controller they are using or going to deploy.

This test tool is meant to be effective, low cost, friendly to user and powerful enough. The software will keep updating, improvements will be implemented and new features will be added.

