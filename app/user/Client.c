/*
 * Client.c
 *
 *  Created on: 2017年1月22日
 *      Author: LiXian
 */



#include "Client.h"



/*发送成功回到函数*/
void ICACHE_FLASH_ATTR
user_tcp_sent_cb(void *arg)
{

    os_printf("发送到主机成功！\r\n");

}


/*断开连接回调函数*/
void ICACHE_FLASH_ATTR
user_tcp_discon_cb(void *arg)
{

    os_printf("断开主机连接成功！\r\n");

}



void ICACHE_FLASH_ATTR
user_tcp_recv_cb(void *arg, char *pdata, unsigned short len)
{
    struct espconn *pespconn = arg;
    os_printf("收到数据：%s\r\n", pdata);

    /*需要延迟一段时间才能断开*/
    os_delay_us(300);

//    espconn_disconnect((struct espconn *)pespconn);
}


/*失败重连回调函数*/
void ICACHE_FLASH_ATTR
user_tcp_client_reconect_cb(void *arg, sint8 err)
{
    os_printf("连接到主机失败！错误代码为：%d \r\n", err);
    espconn_connect((struct espconn *)arg);
}



/*连接成功回调函数*/
void ICACHE_FLASH_ATTR
user_tcp_client_connect_cb(void *arg)
{
    struct espconn *pespconn = arg;
    os_printf("\n \r进入连接成功回调函数\n");
    os_printf("连接到主机成功！\r\n");
    /*1.连接成功后 注册接收数据的回调函数和断开连接的回调函数和断开连接的回调函数*/

    //struct espconn *espconn, espconn_recv_callback recv_cb);
    espconn_regist_recvcb(pespconn, user_tcp_recv_cb);
    espconn_regist_sentcb(pespconn, user_tcp_sent_cb);
    espconn_regist_disconcb(pespconn, user_tcp_discon_cb);

    //(struct espconn *espconn, uint8 *psent, uint16 length);
    espconn_sent(pespconn, HEAD, strlen(HEAD));
}



void ICACHE_FLASH_ATTR
my_station_client_init(struct ip_addr *remote_ip, struct ip_addr *local_ip, int remote_port)
{
    os_printf("\n \r进入 Mt_Starion_Client_init()  函数\n");

    /*1.依据工作协议是初始化espconn参数*/
    user_tcp_client_conn.type = ESPCONN_TCP;
    user_tcp_client_conn.state = ESPCONN_NONE;

    user_tcp_client_conn.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));

    os_memcpy(user_tcp_client_conn.proto.tcp->remote_ip, remote_ip, 4);

    os_memcpy(user_tcp_client_conn.proto.tcp->local_ip, local_ip, 4);


    user_tcp_client_conn.proto.tcp->local_port = espconn_port();
    user_tcp_client_conn.proto.tcp->remote_port = remote_port;


    os_printf("\n \r初始化ESPCONN参数完成\n");
    /*2.注册连接成功的回调函数和连接失败重连的回调函数*/
    //espconn_connect_callback
    espconn_regist_connectcb(&user_tcp_client_conn, user_tcp_client_connect_cb);
    espconn_regist_reconcb(&user_tcp_client_conn, user_tcp_client_reconect_cb);

    /*3.调用espconn_connect建立与TCP Server的连接*/

    os_printf("\n \r调用espconn_connect建立与TCP Server的连接\n");
    espconn_connect(&user_tcp_client_conn);
}

