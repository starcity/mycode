#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "tcp_client.h"




Tcp_client::Tcp_client(string ip, uint16_t port, Data_recv *pdata):m_ip(ip),m_port(port),m_pdata(pdata)
{
    m_reconnect = false;
    if(create_socket(ip,port) < 0)
    {
        perror("scoket failed");
        exit(0);
    }
}

Tcp_client::~Tcp_client()
{

}

void Tcp_client::run()
{
    char buf[RECV_BUF] = {0};
    int length = 0;

    signal(SIGPIPE,SIG_IGN);
//    signal(SIGALRM,SIG_IGN);
 //   signal(SIGINT,SIG_IGN);

    while(1)
    {
        length = recv(m_fd,buf,RECV_BUF,0);
        if (length < 0)
        {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK))    //没有数据了
            {
               continue ;
            }
        }
        else if (length == 0)
        {
            close(m_fd);
            m_pdata->do_sock_closeed(m_fd,m_ip,m_port,strerror(errno));
            if(m_reconnect)
            {
                sleep(1);
                create_socket(m_ip,m_port);
            }
            else break;
        }
        else
        {
            m_pdata->do_data_tcp_recv(m_fd,buf,length);
        }
    }
}

void Tcp_client::set_reconnect()
{
    m_reconnect = true;
}

void Tcp_client::close_connect()
{
    close(m_fd);
}

int Tcp_client::send_data(const char *buf, int len)
{
    return write(m_fd,buf,len);
}

int Tcp_client::get_peer_info(string &ip, uint16_t &port)
{
    if(m_fd < 0)return -1;
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    getpeername(m_fd, (struct sockaddr *)&sa, &len);
    ip = inet_ntoa(sa.sin_addr);
    port = ntohs(sa.sin_port);

    return 0;
}

int Tcp_client::create_socket(string ip, uint16_t port)
{
    //创建用于internet的流协议(TCP)socket,用client_socket代表客户机socket
    m_fd = socket(AF_INET,SOCK_STREAM,0);
    if( m_fd < 0)
    {
        return CREATE_SOCKET_FAILED;
    }

    //设置一个socket地址结构server_addr,代表服务器的internet地址, 端口
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_aton(ip.c_str(),&server_addr.sin_addr);
    server_addr.sin_port = htons(port);
    socklen_t server_addr_length = sizeof(server_addr);
    //向服务器发起连接,连接成功后client_socket代表了客户机和服务器的一个socket连接
    if(connect(m_fd,(struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
       return CONNECT_FAILED;
    }
    return SUCESS;
}
