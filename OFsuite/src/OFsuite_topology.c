/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include "math.h"
#include "OFsuite_topology.h"
#include "OFsuite_log.h"

uint16_t n_spine = 0;
void create_topology(int n_fakesw, topology *OFsuite_topo, uint8_t topo_mode) {
	uint32_t i,j,k;
    int depth = 0;
    OFsuite_topo->sw_num = n_fakesw;
    OFsuite_syslog(LV_INFO, "OFsuite_topology","Creating topology...");
	switch(topo_mode) {
	case TOPO_LINEAR:
        OFsuite_syslog(LV_INFO,"OFsuite_topology","linear topology");
		for(i = 0; i < n_fakesw; i++) {
			for(j = 0; j < n_fakesw; j++) {
				if(j-i==1 || i-j==1)
					OFsuite_topo->matrix[i][j] = 1;
			}
		}
		break;
	case TOPO_RING:
        if (n_fakesw > 1) {
            OFsuite_syslog(LV_INFO, "OFsuite_topology", "ring topology");
            for(i = 0; i < n_fakesw; i++) {
                for(j = 0; j < n_fakesw; j++) {
                    if(j-i==1 || i-j==1)
                        OFsuite_topo->matrix[i][j] = 1;
                }
            }
            OFsuite_topo->matrix[0][n_fakesw - 1] = 1;
            OFsuite_topo->matrix[n_fakesw - 1][0] = 1;
        }
        else {
            OFsuite_syslog(LV_ERROR,"OFsuite_topology","At least 2 switches for ring topology!");
            OFsuite_exit();
            exit(1);
        }
        break;
    case TOPO_FULL_MESH:
        OFsuite_syslog(LV_INFO, "OFsuite_topology", "full-mesh topology");
        if (n_fakesw > 255) {
            OFsuite_syslog(LV_ERROR, "OFsuite_topology","Too many switch nodes, Quit now!");
            OFsuite_exit();
            exit(1);
        }
        else if (n_fakesw < 2) {
            OFsuite_syslog(LV_ERROR,"OFsuite_topology", "at least 2 switches for full-mesh topology!");
            OFsuite_exit();
            exit(1);
        }
        else {
            for(i = 0; i < n_fakesw; i++) {
                for(j = 0; j < n_fakesw; j++) {
                    if(i != j)
                        OFsuite_topo->matrix[i][j] = 1;
                }
            }
        }
        break;
    case TOPO_LEAF_SPINE:
        if (n_fakesw >= 3) {
            OFsuite_syslog(LV_INFO, "OFsuite_topology","leaf-spine topology");
            while (n_fakesw > pow(8, n_spine)) {
                n_spine++;
            }
            n_spine++;
            for (i = 1; i <= n_spine; i++) {
                OFsuite_topo->matrix[i][0] = 1;
                OFsuite_topo->matrix[0][i] = 1;
                for (j = 1; j < n_fakesw - n_spine; j++) {
                    OFsuite_topo->matrix[i][j + n_spine] = 1;
                    OFsuite_topo->matrix[j + n_spine][i] = 1;
                }
            }
        }
        else {
            OFsuite_syslog(LV_ERROR,"OFsuite_topology", "at least 3 switches for leaf-spine topology!");
            OFsuite_exit();
            exit(1);
        }
        break;
    case TOPO_TREE:
        if (n_fakesw >= 3) {
            OFsuite_syslog(LV_INFO,"OFsuite_topology","tree topology");
            while (n_fakesw >= pow(2, depth)) {
                depth++;
            }
            for (i = 0; i <= depth; i++) {
                k = 0;
                for (j = (int)pow(2, i+1); j < ((int)pow(2, i+2) - 1); j+=2) {
                    if (j <= n_fakesw) {
                        OFsuite_topo->matrix[((int)pow(2, i)-1)+k][j - 1] = 1;
                        OFsuite_topo->matrix[j - 1][((int)pow(2, i)-1)+k] = 1;
                    }
                    if (j+1 <= n_fakesw) {
                        OFsuite_topo->matrix[((int)pow(2, i)-1)+k][j] = 1;
                        OFsuite_topo->matrix[j][((int)pow(2, i)-1)+k] = 1;
                    }
                    k++;
                }
            }  
        }
        else {
            OFsuite_syslog(LV_ERROR,"OFsuite_topology","at least 3 switches for leaf-spine topology!");
            OFsuite_exit();
            exit(1);
        }
        break;
	default:
        OFsuite_syslog(LV_INFO, "OFsuite_topology","No topology now");
		break;
	}
    OFsuite_syslog(LV_INFO,"OFsuite_topology","Setting topology done!");
    /*
    for (i = 0; i < n_fakesw; i++) {
        for (j = 0; j < n_fakesw; j++) {
            printf("%d  ", OFsuite_topo->matrix[i][j]);
        }
        printf("\n");
    }*/
}

void update_topology(int cur_n_sw, int add_n_sw, topology *OFsuite_topo, uint8_t topo_mode) {
    uint32_t i,j;
    switch(topo_mode) {
    case TOPO_LINEAR:
        for(i = 0; i <= add_n_sw; i++) {
            for(j = 0; j <= add_n_sw; j++) {
                if(j-i==1 || i-j==1)
                    OFsuite_topo->matrix[i + cur_n_sw -1][j + cur_n_sw -1] = 1;
            }
        }
        break;
    case TOPO_RING:
        OFsuite_topo->matrix[0][cur_n_sw - 1] = 0;
        OFsuite_topo->matrix[cur_n_sw - 1][0] = 0;
        for(i = 0; i < cur_n_sw + add_n_sw; i++) {
            for(j = 0; j < cur_n_sw + add_n_sw; j++) {
                if(j-i==1 || i-j==1)
                    OFsuite_topo->matrix[i][j] = 1;
            }
        }
        OFsuite_topo->matrix[0][cur_n_sw + add_n_sw - 1] = 1;
        OFsuite_topo->matrix[cur_n_sw + add_n_sw - 1][0] = 1;
        break;
    case TOPO_FULL_MESH:
        for(i = 0; i < cur_n_sw + add_n_sw; i++) {
            for(j = 0; j < cur_n_sw + add_n_sw; j++) {
                if(i != j)
                    OFsuite_topo->matrix[i][j] = 1;
            }
        }
        break;
    case TOPO_LEAF_SPINE:
        for (i = 0; i < n_spine; i++) {
            for (j = 0; j < add_n_sw; j++) {
                OFsuite_topo->matrix[i][j + cur_n_sw] = 1;
                OFsuite_topo->matrix[j + cur_n_sw][i] = 1;
            }
        }
        break;
    default:
        break;
    }
}
