/*
 * PMAC_eth.c
 *
 *  Created on: Nov 4, 2015
 *      Author: aliu
 */

#include "defines.h"

int connect_pmac(const char * ip, const int port, const int prot_type);
int readready_pmac(const int fd);
int flush_pmac(const int fd);
int sendline_pmac(const int fd, const char * buff);
int getline_pmac(const int fd, const char * buff, const int len);
int getbuffer_pmac(const int fd, char * buff, int len);
int getresponse_pmac(const int fd, const char * cmd, const char * response, const int len);

int writeable(const int fd);
int readable(const int fd);
int send_all(int fd, const char * buff, const int len);
int recv_all(int fd, const char * buff, const int len);

int connect_pmac(const char * ip, const int port, const int prot_type){
	int fd, size, result, opt;
	fd_set wfds;
	struct sockaddr_in p_addr;
	struct timeval tval;
	socklen_t len = sizeof(int);

	long arg;

	switch(prot_type){
	case TCP:
		fd = socket(AF_INET, SOCK_STREAM, 0);
		break;
	case UDP:
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		break;
	default:
		fd = socket(AF_INET, SOCK_STREAM, 0);
		break;
	}
	if (fd < 0)
		return -1;

    bzero((char *) &p_addr, sizeof(p_addr));
    p_addr.sin_addr.s_addr = inet_addr(ip);
    p_addr.sin_port = htons(port);
    p_addr.sin_family = AF_INET;
    size = sizeof(p_addr);

    arg = fcntl(fd, F_SETFL, NULL);
    if (arg < 0)
    	return -2;
    arg |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, arg) < 0)
    	return -2;

    result = connect(fd, (struct sockaddr *)&p_addr, size);
    if (result < 0) {
    	if (errno == EINPROGRESS){
			do{
				FD_ZERO(&wfds);
				FD_SET(fd, &wfds);
				tval.tv_sec = s_TIMEOUT;
				tval.tv_usec = u_TIMEOUT;

				result = select(fd+1, NULL, &wfds, NULL, &tval);
				if (result < 0 && errno != EINTR) {
					return -3; // select() failed
				} else if (result > 0){
					if(getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&opt), &len)< 0)
						return -4;  // getsockopt() failed
					if (opt)
						return -5; //connection error
					break;
				} else
					return -6; // timeout
			} while(1);
    	} else
    		return -3; // connect failed
    }

    result = flush_pmac(fd);
	if(result < 0){
		printf("Flush Failed!\n");
		return result;
	}
    printf("Connection Successful!\n");
    return fd;
}

int readready_pmac(const int fd){
	int result;
	ETHERNETCMD ethcmd;
	char buff[READREADY_RETSIZE];

	ethcmd.RequestType = VR_UPLOAD;
	ethcmd.Request     = VR_PMAC_READREADY;
	ethcmd.wValue      = 0;
	ethcmd.wIndex      = 0;
	ethcmd.wLength     = htons(READREADY_RETSIZE);

	result = send_all(fd, (char *) &ethcmd, ETHERNETCMDSIZE);
	if (result < 0)
		return result;

	result = recv_all(fd, buff, READREADY_RETSIZE);
	if (result < 0)
		return result;

	int first_byte;
	first_byte = (int) buff[0];
	if (first_byte == 0){
		return -9; //no data to read
	}

	return 0;
}

int flush_pmac(const int fd){
	int result;
	ETHERNETCMD ethcmd;

	ethcmd.RequestType = VR_DOWNLOAD;
	ethcmd.Request 	   = VR_PMAC_FLUSH;
	ethcmd.wValue 	   = 0;
	ethcmd.wIndex      = 0;
	ethcmd.wLength     = 0;

	result = send_all(fd, (char *) &ethcmd, ETHERNETCMDSIZE);
	if (result < 0)
		return result;

	result = recv_all(fd, (char *) &ethcmd, FLUSH_RETSIZE);
	if (result < 0)
		return result;

	return 0;
}

int sendline_pmac(const int fd, const char * buff){
	int result;
	ETHERNETCMD ethcmd;

	size_t len = strlen(buff);
	ethcmd.RequestType = VR_DOWNLOAD;
	ethcmd.Request     = VR_PMAC_SENDLINE;
	ethcmd.wValue      = 0;
	ethcmd.wIndex      = 0;
	ethcmd.wLength     = htons(len);
	strncpy((char*)&ethcmd.bData[0], buff, len);

	result = send_all(fd, (char *) &ethcmd, ETHERNETCMDSIZE + len);
	if (result < 0)
		return result;

	result = recv_all(fd, (char *) &ethcmd, SENDLINE_RETSIZE);
	if (result < 0)
		return result;

	return 0;
}

int getline_pmac(const int fd, const char * buff, const int len){
	int result;
	ETHERNETCMD ethcmd;

	ethcmd.RequestType = VR_UPLOAD;
	ethcmd.Request     = VR_PMAC_GETLINE;
	ethcmd.wValue      = 0;
	ethcmd.wIndex      = 0;

	result = send_all(fd, (char *) &ethcmd, ETHERNETCMDSIZE);
	if (result < 0)
		return result;

	result = recv_all(fd, buff, len);
	if (result < 0)
		return result;

	return result;
}

int getbuffer_pmac(const int fd, char * buff,  const int len){
	int result;
	ETHERNETCMD ethcmd;

	ethcmd.RequestType = VR_UPLOAD;
	ethcmd.Request 	   = VR_PMAC_GETBUFFER;
	ethcmd.wValue 	   = 0;
	ethcmd.wIndex      = 0;
	ethcmd.wLength     = htons(len);

	result = send_all(fd, (char *) &ethcmd, ETHERNETCMDSIZE);
	if (result < 0)
		return result;

	result = recv_all(fd, buff, len);
	if (result < 0)
		return result;

	return 0;
}

int getresponse_pmac(const int fd, const char * cmd, const char * response, const int len){
	int result, len_c;
	ETHERNETCMD ethcmd;

	len_c = strlen(cmd);
	ethcmd.RequestType = VR_DOWNLOAD;
	ethcmd.Request     = VR_PMAC_GETRESPONSE;
	ethcmd.wValue      = 0;
	ethcmd.wIndex      = 0;
	ethcmd.wLength     = htons(len_c);
	strncpy((char *) &ethcmd.bData[0], cmd, len_c);

	result = flush_pmac(fd);
	if(result < 0)
		return result;

	result = send_all(fd, (char *) &ethcmd, ETHERNETCMDSIZE + len_c);
	if (result < 0)
		return result;

	result = recv_all(fd, response, len);
	if (result < 0)
		return result;

	return result;
}



int writeable (const int fd){
	fd_set wfds;
	struct timeval tval;
	int result, opt;
	socklen_t len = sizeof(int);

	do{
		FD_ZERO(&wfds);
		FD_SET(fd, &wfds);
		tval.tv_sec = s_TIMEOUT;
		tval.tv_usec = u_TIMEOUT;

		result = select(fd + 1, NULL, &wfds, NULL, &tval);
		if (result < 0){
			if (errno != EINTR || errno != EWOULDBLOCK || errno != EAGAIN){
				return -3;
			}
		} else if (result == 0){
			return -6;
		} else
			if(getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&opt), &len)< 0)
				return -4;  // getsockopt() failed
			if (opt)
				return -5; //connection error
			break;
	}while(1);

	return 0;
}

int readable (const int fd){
	fd_set rfds;
	struct timeval tval;
	int result, opt;
	socklen_t len = sizeof(int);

	do{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		tval.tv_sec = s_TIMEOUT;
		tval.tv_usec = u_TIMEOUT;

		result = select(fd + 1, &rfds, NULL, NULL, &tval);
		if (result < 0){
			if (errno != EINTR || errno != EWOULDBLOCK || errno != EAGAIN){
				return -3;
			}
		} else if (result == 0){
			return -6;
		} else
			if(getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&opt), &len)< 0)
				return -4;  // getsockopt() failed
			if (opt)
				return -5; //connection error
			result = 0;
			break;
	}while(1);

	return 0;
}

int send_all(int fd, const char * buff, const int len){
	int result, size, track;

	char * ptr = buff;
	size = len;
	track = 0;
	do{
		result = writeable(fd);
		if (result < 0){
			if(track == 0){
				return result;
			}
			if (result == -6 && track > 0){
				break;
			}
		} else {
			result = send(fd, ptr, size, 0);
			if (result < 0){
				if(errno != EINTR || errno != EAGAIN || errno != EWOULDBLOCK)
					return -7;
			} else if (result == size){
				track = result;
				break;
			} else if (result >= 0 && result < size){
				ptr = (ptr + result - 1);
				size = size - result;
				track += result;
			}
		}
	}while(1);
	return track;
}

int recv_all(const int fd, const char * buff, const int len){
	int result, size, track;

	char * ptr = buff;
	size = len;
	track = 0;
	do{
		result = readable(fd);
		if (result < 0){
			if (track == 0)
				return result;
			if (result == -6 && track > 0)
				break;
		} else {
			result = recv(fd, ptr, size, 0);
			if(result < 0){
				if (errno != EINTR || errno != EAGAIN || errno != EWOULDBLOCK)
					return -8;
			} else if (result == size){
				track = result;
				break;
			} else if (result >= 0 && result < size){
				ptr = (ptr + result - 1);
				size = size - result;
				track += result;
			}
		}
	}while(1);

	return track;
}

