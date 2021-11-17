/*
 * main.c
 *
 *  Created on: Nov 5, 2015
 *      Author: aliu
 */

#include "PMAC_eth.h"
#include "pmac_api.h"

int main(){
	pmac_value pv;
	bzero((char*) &pv, sizeof(pmac_value));

	int fd = connect_pmac(DEFAULT_IP, DEFAULT_PORT, TCP);
	if (fd < 0){
		printf("error\n");
	};

	halt_axis(fd, 3);
	//disable_tension_zone(fd, ZONE1);
	//disable_tension_zone(fd, ZONE2);

	//reduce_slack(fd, ZONE1);
	//usleep(500000);
	//reduce_slack(fd, ZONE2);

	//set_tension(fd, 25, ZONE1);
	//usleep(500000);
	//set_tension(fd, 2, ZONE2);
	//usleep(100000);
	//char buff[] = "I222 = 3";
	//sendline_pmac(fd, buff);
	set_linspeed(fd, 4);
	//begin_jog(fd, 3, NEG_DIR);
	/*
	int i;
	while(i < 1000){
		report_vel(fd, 1, (pmac_value*) &pv);
		report_vel(fd, 3, (pmac_value*) &pv);
		printf("%f, %f\n", pv.velocity[0],pv.velocity[2]);
		i++;
	}
	*/
	close (fd);
	return 0;
}
