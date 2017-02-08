/*
 * Client.h
 *
 *  Created on: 2017年1月22日
 *      Author: LiXian
 */

#ifndef APP_INCLUDE_CLIENT_H_
#define APP_INCLUDE_CLIENT_H_

#include "user_main.h"
#include "espconn.h"
#include "mem.h"

char g_Buffer[1024];



#define HEAD "GET / HTTP/1.1\r\nAccept: */*\r\nHost: www.baidu.com\r\nConnection: Keep-Alive\r\n\r\n\0"

#define GET "GET /%s HTTP/1.1\r\nAccept: */*\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n"
#define POST "POST /%s HTTP/1.1\r\nAccept: */*\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n%s"



/*%S 文件名 %s域名*/


struct espconn user_tcp_client_conn;

void my_station_client_init(struct ip_addr *remote_ip, struct ip_addr *local_ip, int remote_port);
#endif /* APP_INCLUDE_CLIENT_H_ */
