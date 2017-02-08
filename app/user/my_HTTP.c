/*
 * HTTP.c
 *
 *  Created on: 2017年1月29日
 *      Author: LiXian
 */


#include "my_HTTP.h"
#include "c_types.h"
#include "Client.h"


char g_host[32];
char g_FileName[128];
unsigned short g_Port;


void ICACHE_FLASH_ATTR
http_parse_request_url(char *URL,char * pHost,char * pFileName, unsigned short * pPort)
{
	char *PA;
	char *PB;

	memset(pHost,0,sizeof(pHost));

	memset(pFileName,0,sizeof(pFileName));

	*pPort=0;

	if(!(*URL))
	{
		return;
	}

	PA=URL;

	if(!strncmp(PA,"http://",strlen("http://")))
	{
		PA=URL+strlen("http://");
	}
	else if(!strncmp(PA,"https://",strlen("https://")))
	{
		PA=URL+strlen("https://");
	}

	PB=strchr(PA,'/');

	if(PB)
	{
		memcpy(pHost,PA,strlen(PA)-strlen(PB));
		if(PB+1)
		{
			memcpy(pFileName,PB+1,strlen(PB-1));
			pFileName[strlen(PB)-1]=0;
		}
		pHost[strlen(PA)-strlen(PB)]=0;
	}
	else
	{
		memcpy(pHost,PA,strlen(PA));
		pHost[strlen(PA)]=0;
	}

	PA=strchr(pHost,':');

	if(PA)
	{
		*pPort=atoi(PA+1);
	}
	else
	{
		*pPort=80;
	}

}

void ICACHE_FLASH_ATTR
user_esp_platform_dns_found(const char *name, ip_addr_t *ipaddr, void *arg)
{

	/*1.调用my_station_client_init*/
	 struct ip_info info;

	 wifi_get_ip_info(STATION_IF, &info);

	my_station_client_init(ipaddr,&info.ip,g_Port);
}



void ICACHE_FLASH_ATTR
HttpReadFile(char *URL,char *method, char * posdata,char* pHost,char* pFileName, unsigned short* pPort)
{

	struct ip_addr esp_server_ip;

	memset(g_Buffer,0,1024);

	/*1.接续URL,获取域名，端口号，文件名*/

	http_parse_request_url(URL,pHost,pFileName,pPort);


	if(strcmp(method,"GET") == 0)
	{
		os_sprintf(g_Buffer,GET,pFileName,pHost,posdata);

	}
	else
	{
		os_sprintf(g_Buffer,POST,pFileName,strlen(posdata),pHost);
	}


/*2.使用espconn_gethostbyname获取服务器IP地址*/

	espconn_gethostbyname(&user_tcp_client_conn,pHost, &esp_server_ip,user_esp_platform_dns_found);

}
