/*
 * defines.h
 *
 *  Created on: Nov 4, 2015
 *      Author: aliu
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define DEFAULT_IP 	  			"192.6.94.5"
#define DEFAULT_PORT  			1025
#define TCP 					0
#define UDP						1

#define u_TIMEOUT 				250000
#define s_TIMEOUT               1

#define ETHERNETCMDSIZE         8
#define VR_DOWNLOAD             0x40
#define VR_UPLOAD               0xC0

#define FLUSH_RETSIZE           1
#define SENDLINE_RETSIZE        1
#define READREADY_RETSIZE       2

#define VR_PMAC_SENDLINE 		0xB0
#define VR_PMAC_GETLINE 		0xB1
#define VR_PMAC_FLUSH 			0xB3
#define VR_PMAC_GETMEM 			0xB4
#define VR_PMAC_SETMEM 			0xB5
#define VR_PMAC_SETBIT 			0xBA
#define VR_PMAC_SETBITS 		0xBB
#define VR_PMAC_PORT 			0xBE
#define VR_PMAC_GETRESPONSE 	0xBF
#define VR_PMAC_READREADY 		0xC2
#define VR_CTRL_RESPONSE 		0xC4
#define VR_PMAC_GETBUFFER 		0xC5
#define VR_PMAC_WRITEBUFFER 	0xC6
#define VR_PMAC_WRITEERROR 		0xC7
#define VR_FWDOWNLOAD 			0xCB
#define VR_IPADDRESS 			0xE0
#define TIMEOUT_ERR 			-6

typedef struct tagEthernetCmd
{
  u_int8_t   RequestType;
  u_int8_t   Request;
  u_int16_t  wValue;
  u_int16_t  wIndex;
  u_int16_t  wLength;
  u_int8_t   bData[1492];
} ETHERNETCMD,*PETHERNETCMD;



#endif /* DEFINES_H_ */
