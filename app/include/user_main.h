/*
 * user_main.h
 *
 *  Created on: 2017年1月18日
 *      Author: Michael
 */

#ifndef WIFI_TCP_CLIENT_APP_INCLUDE_USER_MAIN_H_
#define WIFI_TCP_CLIENT_APP_INCLUDE_USER_MAIN_H_


#include "osapi.h"
#include "user_interface.h"

ETSTimer nConnect_Timer;// 定时器结构,不能使用指针！切记
ETSTimer g_Station_Check_Timer;

typedef enum
{
    STATION = 0x01,
    SOFT_AP,
    STATION_AP
} WifiOpMode;


#endif /* WIFI_TCP_CLIENT_APP_INCLUDE_USER_MAIN_H_ */
