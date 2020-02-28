#include <algorithm>
#include "udp_server.h"
#include <signal.h>


Udp_server::Udp_server(string ip,uint16_t port,Data_recv *pdata):m_ip(ip),m_port(port),m_pdata(pdata)
{
    m_socketfd = FD_START;

    socket_bind(ip,port);
}

Udp_server::~Udp_server()
{

}

int Udp_server::socket_bind(string ip,uint16_t port)
{
    struct sockaddr_in server;

    if((m_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Creatingsocket failed.");
        exit(1);
    }

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    inet_pton(AF_INET,ip.c_str(),&server.sin_addr);

    if(bind(m_fd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Bind()error.");
        exit(1);
    }
    return 0;
}

void Udp_server::run()
{
    struct sockaddr_in client;
    socklen_t  addrlen = sizeof(client);
    char  buf[MAX_BUF] = {0};
    int   len = 0;
    int   client_fd = 0;

    signal(SIGPIPE,SIG_IGN);
//    signal(SIGALRM,SIG_IGN);
//    signal(SIGINT,SIG_IGN);

    while(1)
    {
        len = recvfrom(m_fd, buf,sizeof(buf),0,(struct sockaddr *)&client ,&addrlen);
        if(len < 0 )
        {
            close(m_fd);
            sleep(1);
            socket_bind(m_ip,m_port);
            continue;
        }

        Mfd_addr::iterator iter = find_if(m_mfd_addr.begin(),m_mfd_addr.end(),map_fd_addr_find(client));
        if(iter == m_mfd_addr.end())
        {
            client_fd = m_socketfd++;
            m_mfd_addr[client_fd] = client;
        }
        m_pdata->do_data_udp_recv(client_fd,buf,len);
    }
    close(m_fd);
}

int Udp_server::send_data(int fd,const char *buf,int len)
{
    Mfd_addr::iterator iter = m_mfd_addr.find(fd);
    if(iter == m_mfd_addr.end())return -1;

    socklen_t  addrlen = sizeof(struct sockaddr);
    int ret = sendto(m_fd,buf,len,0,(struct sockaddr *)&iter->second,addrlen);
    if(ret < 0)
    {
        m_mfd_addr.erase(iter);
        return -1;
    }
    return ret;
}

int Udp_server::get_peer_info(int fd,string &ip, uint16_t &port)
{
    if(fd < 0)return -1;

    Mfd_addr::iterator iter = m_mfd_addr.find(fd);
    if(iter == m_mfd_addr.end())return -1;

    ip = inet_ntoa(iter->second.sin_addr);
    port = ntohs(iter->second.sin_port);

    return 0;
}

