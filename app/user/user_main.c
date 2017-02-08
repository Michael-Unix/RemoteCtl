

/*
 * user_main.c
 *
 *  Created on: 2017��1��18��
 *      Author: Michael
 */

/*1.TCP Serverģʽ��ESP8266������stationģʽ�� ����Ҫȷ��ESP8266�Ѿ����䵽
 * IP��ַ��������server����*/

/*2.ESP8266������softapģʽ�£�����ֱ������server����*/

#include "driver/uart.h"
#include "user_main.h"
#include "Client.h"
#include "Server.h"
#include "my_HTTP.h"
//#define WORK


static uint8 nCount = 0;

extern char g_host[32];
extern char g_FileName[128];
extern unsigned short g_Port;



void ICACHE_FLASH_ATTR
Wificonnect(void *arg)
{
    struct ip_info info;
    const char remote_ip[4] = {192, 168, 31, 196};

    nCount++;
    os_timer_disarm (&nConnect_Timer);
    /*��ȡ��ǰWIFI����״̬*/
    if(STATION_GOT_IP == wifi_station_get_connect_status())
    {

        os_printf("\n \r���� AP�ɹ�!!! \r ׼����ʼ��TCP Client ����...... \n");

        /**/
        /*��ȡ����IP��ַ....��ʼ��TCP Client����*/
        wifi_get_ip_info(STATION_IF, &info);

        // my_station_client_init((struct ip_addr*)remote_ip,&info.ip,1025);

        //  my_station_server_init(&info.ip,1213);

        os_printf("\n \r����Զ��IP��ַ������Զ��PORT�˿����\n");

       // HttpReadFile("www.baidu.com");
        HttpReadFile("http://mick0866.hkhost38.asia/post.php","POST","",g_host,g_FileName,&g_Port);
        return ;
    }
    else if(7 <= nCount)
    {
        nCount = 0;
        os_printf("\n \r����APʧ��!!!\n");
        return ;
    }

    os_printf("\n \r������������AP!!! \n");
    os_timer_arm(&nConnect_Timer, 2000, NULL);
}


void ICACHE_FLASH_ATTR
scan_done(void *arg, STATUS status)
{
    uint8 ssid[33];
    struct station_config stationConf;

    if (status == OK)
    {
        struct bss_info *bss_link = (struct bss_info *)arg;
        bss_link = bss_link->next.stqe_next;		//ignore firstΪ��Ҫ���Ե�һ����

        while (bss_link != NULL)
        {
            os_memset(ssid, 0, 33);
            if (os_strlen(bss_link->ssid) <= 32)
            {
                os_memcpy(ssid, bss_link->ssid, os_strlen(bss_link->ssid));
            }
            else
            {
                os_memcpy(ssid, bss_link->ssid, 32);
            }
            os_printf("+CWLAP:(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n",
                      bss_link->authmode, ssid, bss_link->rssi,
                      MAC2STR(bss_link->bssid), bss_link->channel);

            bss_link = bss_link->next.stqe_next;
        }
        os_printf("\n\r׼�����ӵ� MIWIFI! \n");
        /*��ɨ�赽���е�WIFI AP֮�󣬿���������Ҫ���ӵ�AP*/
        os_memcpy(&stationConf.ssid, "MIWIFI", 32);
        os_printf("\n\r SSID������� \n");
        os_memcpy(&stationConf.password, "05577881585", 64);
        os_printf("\n\r ���뿽����� \n");
        wifi_station_set_config_current (&stationConf);
        os_printf("\n\r  ������...... \n");
        wifi_station_connect();

        os_timer_setfn(&nConnect_Timer, Wificonnect, NULL);
        os_timer_arm(&nConnect_Timer, 2000, NULL);
    }
}



void ICACHE_FLASH_ATTR
to_scan(void)
{

    wifi_station_scan(NULL, scan_done);
}


void user_init()
{

    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    wifi_set_opmode(STATION);

    system_init_done_cb(to_scan);
}


void user_rf_pre_init()
{
}

