/*
 * physical_defines.h
 *
 *  Created on: Nov 13, 2015
 *      Author: aliu
 */

#ifndef PHYSICAL_DEFINES_H_
#define PHYSICAL_DEFINES_H_

#define DEFAULT_IP 	  		"192.6.94.5"
#define DEFAULT_PORT  		1025
#define TCP 				0
#define UDP					1

#define GET_VEL             "V"
#define GET_POS				"P"
#define JOG_POS				"J+"
#define JOG_NEG             "J-"
#define JOG_TO				"J="
#define STOP_AXIS			"J/"
#define CLOSE_LOOPS			"#1J/#2J/#3J/#5J/#6J/"

#define I10        			3713991	// I10 value (denominator of Velocity factor)
#define VEL_CONST  			8388608 // Numerator of Velocity factor
#define CNTS_PER_ROTATION   1000   // 10K CPR
#define DRIVE_AXIS_DIAMETER 10 		// diameter of drive roller (10 cm)
#define GEAR_RATIO          100     // 100:1
#define PI                  3.14159265359 // PI
#define TENSION_SCALING     615.38 // cnts/pound

#define MAX_LINSPEED		15      // 15 cm/s
#define MAX_TENSION_ZONE1	25 		// 25 lbs
#define MAX_TENSION_ZONE2	6 		// 6 lbs
#define MIN_TENSION_ZONE1   1		// 1 lbs
#define MIN_TENSION_ZONE2	1		// 1 lbs
#define INIT_TENS1          2   	// 2 lbs
#define INIT_TENS2          1  	// 2 lbs

#define NUM_AXIS            8
#define NUM_TZONES			2
#define ZONE1_FAXIS			1
#define ZONE2_FAXIS			2
#define DRIVE_AXIS			3
#define ZONE1_MAXIS			5
#define ZONE2_MAXIS			6
#define TENS_ZONE1_MFOL	    "I106=3"
#define TENS_ZONE2_MFOL	    "I206=3"
#define TENS_ZONE1_DFOL		"I106=0"
#define TENS_ZONE2_DFOL		"I206=0"

#define POS_DIR             1
#define NEG_DIR             0
#define ZONE1               1
#define ZONE2               2
#define ZONE3				3

#define DRIVE_VEL_CMD       "I322="
#define MOT1_VEL_CMD        "I122="
#define MOT2_VEL_CMD		"I222="
#define VIRT_AXIS1_VEL_CMD  "I522="
#define VIRT_AXIS2_VEL_CMD  "I622="

typedef struct pmac_values{
	double position[NUM_AXIS];
	double velocity[NUM_AXIS];
	double tension[NUM_TZONES];
} pmac_value;

#endif /* PHYSICAL_DEFINES_H_ */
