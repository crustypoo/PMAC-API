/*
 * pmac_api.c
 *
 *  Created on: Nov 11, 2015
 *      Author: aliu
 */

#include "PMAC_eth.h"
#include "physical_defines.h"

int set_linspeed(int fd, double linspeed);
int begin_jog(int fd, int axis, int dir);
int report_pos(int fd, int axis, pmac_value * pv);
int report_vel(int fd, int axis, pmac_value * pv);
int report_tension(int fd, int zone, pmac_value * pv);
int reduce_slack(int fd, int zone);
int set_tension(int fd, double tens_in_lbs, int zone);
int disable_tension_zone(int fd, int zone);
int halt_axis(int fd, int axis);

int init_pmac(){
	int fd;
	fd = connect_pmac(DEFAULT_IP, DEFAULT_PORT, TCP);
	if (fd < 0)
		return -1;

	return fd;
}

int set_linspeed(int fd, double linspeed){
	int result;

	if (linspeed > MAX_LINSPEED)
		return -1;

	double dia = (double) DRIVE_AXIS_DIAMETER;
	double circumference = PI * dia;
	double cnts = (double) (CNTS_PER_ROTATION * GEAR_RATIO);

	double holder = linspeed / circumference; //  (rotations per sec)
	holder = holder * cnts;      //  (counts per sec)
	holder = holder / 1000.0;	 //  (counts per msec)

	char buff[256];
	char * ptr = DRIVE_VEL_CMD;
	sprintf(buff, "%s%f", ptr, holder);

	result = sendline_pmac(fd, buff);
	if (result < 0)
		return -2;

	return 0;
}

int begin_jog(int fd, int axis, int dir){
	int result;
	char buff[256];
	char * ptr;
	bzero(buff, 256);

	if (dir == NEG_DIR)
		ptr = JOG_NEG;
	else
		ptr = JOG_POS;

	sprintf(buff, "#%d%s", axis, ptr);

	result = sendline_pmac(fd, buff);
	if (result < 0)
		return -2;

	return 0;
}

int report_pos(int fd, int axis, pmac_value * pv){
	int result;
	char buff[256];
	char retrn[256];
	char * ptr = GET_POS;
	bzero(buff, 256);
	bzero(retrn, 256);
	sprintf(buff, "#%d %s", axis, ptr);

	result = getresponse_pmac(fd, buff, retrn, 256);
	if (result < 0)
		return -3;

	char * ptr1 = strchr(retrn, '\006');
	if (ptr1 != NULL){
		*ptr1 = '\0';
	}
	ptr1 = strchr(retrn, '\r');
	if (ptr1 != NULL){
		*ptr1 = '\0';
	}

	if(retrn[0] == '-')
		pv->position[axis - 1] = -(strtod(retrn + 1, NULL));
	else
		pv->position[axis - 1] = strtod(retrn, NULL);

	return 0;
}

int report_vel(int fd, int axis, pmac_value * pv){
	int result;
	char buff[256];
	char retrn[256];
	char * ptr = GET_VEL;
	bzero(buff, 256);
	bzero(retrn, 256);
	sprintf(buff, "#%d %s", axis, ptr);

	result = getresponse_pmac(fd, buff, retrn, 256);
	if (result < 0)
		return -3;

	char * ptr1 = strchr(retrn, '\006');
	if (ptr1 != NULL){
		*ptr1 = '\0';
	}
	ptr1 = strchr(retrn, '\r');
	if (ptr1 != NULL){
		*ptr1 = '\0';
	}

	if(retrn[0] == '-')
		pv->velocity[axis - 1] = -(strtod(retrn + 1, NULL));
	else
		pv->velocity[axis - 1] = strtod(retrn, NULL);

	return 0;
}

int report_tension(int fd, int zone, pmac_value * pv){
	int result, axis, index;
	switch (zone) {
	case ZONE1:
		axis = ZONE1_MAXIS;
		index = ZONE1 - 1;
		break;
	case ZONE2:
		axis = ZONE2_MAXIS;
		index = ZONE2 - 1;
		break;
	}

	result = report_pos(fd, axis, pv);
	if (result < 0)
		return -6;

	pv->tension[index] = pv->position[axis - 1] / TENSION_SCALING;
	return 0;
}


int reduce_slack(int fd, int zone){
	int result;
	double init_tension;

	switch (zone){
	case ZONE1:
		init_tension = (double) INIT_TENS1;
		break;
	case ZONE2:
		init_tension = (double) INIT_TENS2;
		break;
	}

	result = set_tension(fd, init_tension, zone);
	if (result < 0)
		return -5;

	return 0;
}

int set_tension(int fd, double tens_in_lbs, int zone){
	int result;
	double holder, conv_factor;
	char buff[256];
	bzero(buff, 256);

	result = sendline_pmac(fd, CLOSE_LOOPS);
	if (result < 0)
		return -2;

	switch (zone){
	case ZONE1:
		if (tens_in_lbs <= MAX_TENSION_ZONE1 && tens_in_lbs >= MIN_TENSION_ZONE1){
			result = sendline_pmac(fd, TENS_ZONE1_MFOL);
			if (result < 0)
				return -2;

			conv_factor = (double) TENSION_SCALING;
			holder = tens_in_lbs * conv_factor;

			sprintf(buff, "#%d%s%f", ZONE1_MAXIS, JOG_TO, holder);
			result = sendline_pmac(fd, buff);
			if (result < 0)
				return -2;
		} else if (tens_in_lbs < MIN_TENSION_ZONE1 || tens_in_lbs > MAX_TENSION_ZONE1){
			return -4;
		}
		break;
	case ZONE2:
		if (tens_in_lbs <= MAX_TENSION_ZONE2 && tens_in_lbs >= MIN_TENSION_ZONE2){
			result = sendline_pmac(fd, TENS_ZONE2_MFOL);
			if (result < 0)
				return -2;

			conv_factor = (double) TENSION_SCALING;
			holder = tens_in_lbs * conv_factor;

			sprintf(buff, "#%d%s%f", ZONE2_MAXIS, JOG_TO, holder);
			result = sendline_pmac(fd, buff);
			if (result < 0)
				return -2;
		} else if (tens_in_lbs < MIN_TENSION_ZONE2 || tens_in_lbs > MAX_TENSION_ZONE2){
			return -4;
		}
		break;
	}

	return 0;
}

int disable_tension_zone(int fd, int zone){
	int result;
	char * cmd;

	switch (zone){
	case ZONE1:
		cmd = TENS_ZONE1_DFOL;
		break;
	case ZONE2:
		cmd = TENS_ZONE2_DFOL;
		break;
	}

	result = sendline_pmac(fd, cmd);
	if (result < 0)
		return -2;
	return 0;
}

int halt_axis(int fd, int axis){
	int result;
	char cmd[10];

	sprintf(cmd, "#%d%s", axis, STOP_AXIS);
	result = sendline_pmac(fd, cmd);
	if (result < 0)
		return -2;
	return 0;
}

