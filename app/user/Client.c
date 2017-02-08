/*
 * Client.c
 *
 *  Created on: 2017��1��22��
 *      Author: LiXian
 */



#include "Client.h"



/*���ͳɹ��ص�����*/
void ICACHE_FLASH_ATTR
user_tcp_sent_cb(void *arg)
{

    os_printf("���͵������ɹ���\r\n");

}


/*�Ͽ����ӻص�����*/
void ICACHE_FLASH_ATTR
user_tcp_discon_cb(void *arg)
{

    os_printf("�Ͽ��������ӳɹ���\r\n");

}



void ICACHE_FLASH_ATTR
user_tcp_recv_cb(void *arg, char *pdata, unsigned short len)
{
    struct espconn *pespconn = arg;
    os_printf("�յ����ݣ�%s\r\n", pdata);

    /*��Ҫ�ӳ�һ��ʱ����ܶϿ�*/
    os_delay_us(300);

//    espconn_disconnect((struct espconn *)pespconn);
}


/*ʧ�������ص�����*/
void ICACHE_FLASH_ATTR
user_tcp_client_reconect_cb(void *arg, sint8 err)
{
    os_printf("���ӵ�����ʧ�ܣ��������Ϊ��%d \r\n", err);
    espconn_connect((struct espconn *)arg);
}



/*���ӳɹ��ص�����*/
void ICACHE_FLASH_ATTR
user_tcp_client_connect_cb(void *arg)
{
    struct espconn *pespconn = arg;
    os_printf("\n \r�������ӳɹ��ص�����\n");
    os_printf("���ӵ������ɹ���\r\n");
    /*1.���ӳɹ��� ע��������ݵĻص������ͶϿ����ӵĻص������ͶϿ����ӵĻص�����*/

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
    os_printf("\n \r���� Mt_Starion_Client_init()  ����\n");

    /*1.���ݹ���Э���ǳ�ʼ��espconn����*/
    user_tcp_client_conn.type = ESPCONN_TCP;
    user_tcp_client_conn.state = ESPCONN_NONE;

    user_tcp_client_conn.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));

    os_memcpy(user_tcp_client_conn.proto.tcp->remote_ip, remote_ip, 4);

    os_memcpy(user_tcp_client_conn.proto.tcp->local_ip, local_ip, 4);


    user_tcp_client_conn.proto.tcp->local_port = espconn_port();
    user_tcp_client_conn.proto.tcp->remote_port = remote_port;


    os_printf("\n \r��ʼ��ESPCONN�������\n");
    /*2.ע�����ӳɹ��Ļص�����������ʧ�������Ļص�����*/
    //espconn_connect_callback
    espconn_regist_connectcb(&user_tcp_client_conn, user_tcp_client_connect_cb);
    espconn_regist_reconcb(&user_tcp_client_conn, user_tcp_client_reconect_cb);

    /*3.����espconn_connect������TCP Server������*/

    os_printf("\n \r����espconn_connect������TCP Server������\n");
    espconn_connect(&user_tcp_client_conn);
}

