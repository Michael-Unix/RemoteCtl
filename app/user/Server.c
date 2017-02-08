/*
 * Server.c
 *
 *  Created on: 2017��1��27��
 *      Author: LiXian
 */

#include "Server.h"


void ICACHE_FLASH_ATTR
user_tcp_server_recv_cb(void *arg, char *pdata, unsigned short len)
{

    os_printf("\r\n��������ʾ��������ESP�Ѿ��յ����Կͻ��˵�����: %s", pdata);


    espconn_sent((struct espconn *) arg, "PC��ʾ��������ESP�Ѿ��յ�PC���͵�����", strlen("PC��ʾ��������ESP�Ѿ��յ�PC���͵�����"));


}

void ICACHE_FLASH_ATTR
user_tcp_server_sent_cb(void *arg)
{

    os_printf("\r\n ������ʾ:\r�ɹ����͵��ͻ���\r\n");
}



void ICACHE_FLASH_ATTR
user_tcp_server_discon_cb(void *arg)
{

    os_printf("\r\n ������ʾ��\r�����Ѿ��Ͽ���\r\n");

}



void ICACHE_FLASH_ATTR
user_tcp_server_connect_cb(void *arg)
{
    /*1.ע��������ݵĻص�����*/

    struct espconn *pespconn = arg;

    espconn_regist_recvcb(pespconn, user_tcp_server_recv_cb);

    /*2.ע�ᷢ�����ݳɹ��Ļص�����*/
    espconn_regist_sentcb(pespconn, user_tcp_server_sent_cb);
    /*3.ע��Ͽ����ӵĻص�����*/

    espconn_regist_disconcb(pespconn, user_tcp_server_discon_cb);

}


void ICACHE_FLASH_ATTR
user_tcp_server_reconect_cb(void *arg, sint8 err)
{
    os_printf("\r\n������ʾ��\r���Ӵ��� �������Ϊ��%d \n", err);

}



void ICACHE_FLASH_ATTR
my_station_server_init(struct ip_addr *local_ip, int local_port)
{

    /*1.���ݹ���Э���ʼ��ESPCONN����*/
    LOCAL struct espconn esp_conn;

    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;

    esp_conn.proto.tcp = (esp_tcp *)(os_zalloc(sizeof(esp_tcp)));

    os_memcpy(esp_conn.proto.tcp->local_ip, local_ip, 4);
    esp_conn.proto.tcp->local_port = local_port;

    /*2.ע�����ӳɹ��Ļص�����������ʧ�������ص�����*/

    /*cע�����ӳɹ��ص�����*/
    espconn_regist_connectcb(&esp_conn, user_tcp_server_connect_cb);

    /*ע���������ӻص�����*/
    espconn_regist_reconcb(&esp_conn, user_tcp_server_reconect_cb);


    /*3.����espconn_accept����TCP����*/
    espconn_accept(&esp_conn);


}
