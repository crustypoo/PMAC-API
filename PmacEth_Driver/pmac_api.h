/*
 * pmac_api.h
 *
 *  Created on: Nov 13, 2015
 *      Author: aliu
 */

#ifndef PMAC_API_H_
#define PMAC_API_H_

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

#endif /* PMAC_API_H_ */
