/*
 * PMAC_eth.h
 *
 *  Created on: Nov 4, 2015
 *      Author: aliu
 */

#ifndef PMAC_ETH_H_
#define PMAC_ETH_H_

#include "defines.h"

int connect_pmac(char * ip, int port, int prot_type);
int sendline_pmac(const int fd, const char * buff);
int getline_pmac(const int fd, const char * buff, const int len);
int getbuffer_pmac(const int fd, char * buff, int len);
int getresponse_pmac(const int fd, const char * cmd, const char * response, const int len);

#endif /* PMAC_ETH_H_ */
