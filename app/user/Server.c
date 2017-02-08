/*
 * Server.c
 *
 *  Created on: 2017年1月27日
 *      Author: LiXian
 */

#include "Server.h"


void ICACHE_FLASH_ATTR
user_tcp_server_recv_cb(void *arg, char *pdata, unsigned short len)
{

    os_printf("\r\n串口先显示：服务器ESP已经收到来自客户端的数据: %s", pdata);


    espconn_sent((struct espconn *) arg, "PC显示：服务器ESP已经收到PC发送的数据", strlen("PC显示：服务器ESP已经收到PC发送的数据"));


}

void ICACHE_FLASH_ATTR
user_tcp_server_sent_cb(void *arg)
{

    os_printf("\r\n 串口显示:\r成功发送到客户端\r\n");
}



void ICACHE_FLASH_ATTR
user_tcp_server_discon_cb(void *arg)
{

    os_printf("\r\n 串口显示：\r连接已经断开！\r\n");

}



void ICACHE_FLASH_ATTR
user_tcp_server_connect_cb(void *arg)
{
    /*1.注册接收数据的回调函数*/

    struct espconn *pespconn = arg;

    espconn_regist_recvcb(pespconn, user_tcp_server_recv_cb);

    /*2.注册发送数据成功的回调函数*/
    espconn_regist_sentcb(pespconn, user_tcp_server_sent_cb);
    /*3.注册断开连接的回调函数*/

    espconn_regist_disconcb(pespconn, user_tcp_server_discon_cb);

}


void ICACHE_FLASH_ATTR
user_tcp_server_reconect_cb(void *arg, sint8 err)
{
    os_printf("\r\n串口显示：\r连接错误！ 错误代码为：%d \n", err);

}



void ICACHE_FLASH_ATTR
my_station_server_init(struct ip_addr *local_ip, int local_port)
{

    /*1.依据工作协议初始化ESPCONN参数*/
    LOCAL struct espconn esp_conn;

    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;

    esp_conn.proto.tcp = (esp_tcp *)(os_zalloc(sizeof(esp_tcp)));

    os_memcpy(esp_conn.proto.tcp->local_ip, local_ip, 4);
    esp_conn.proto.tcp->local_port = local_port;

    /*2.注册连接成功的回调函数和连接失败重连回调函数*/

    /*c注册连接成功回调函数*/
    espconn_regist_connectcb(&esp_conn, user_tcp_server_connect_cb);

    /*注册重新连接回调函数*/
    espconn_regist_reconcb(&esp_conn, user_tcp_server_reconect_cb);


    /*3.调用espconn_accept侦听TCP连接*/
    espconn_accept(&esp_conn);


}
