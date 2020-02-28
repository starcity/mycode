#include <signal.h>
#include "udp_client.h"

Udp_client::Udp_client(string ip, uint16_t port, Data_recv *pdata):m_ip(ip),m_port(port),m_pdata(pdata)
{
   socket_create(ip,port);
}

Udp_client::~Udp_client()
{

}

void Udp_client::run()
{
    char  buf[MAX_BUF] = {0};
    int   len = 0;

    signal(SIGPIPE,SIG_IGN);
//    signal(SIGALRM,SIG_IGN);
//    signal(SIGINT,SIG_IGN);

    while(1)
    {
        len = recvfrom(m_fd, buf,sizeof(buf),0,(struct sockaddr *)&m_server ,&m_addrlen);
        if(len < 0 )
        {
            sleep(1);
            socket_create(m_ip,m_port);
            continue;
        }

        m_pdata->do_data_udp_recv(m_fd,buf,len);
    }
    close(m_fd);
}

int Udp_client::send_data(const char *buf, int len)
{
    return sendto(m_fd, buf,len,0,(struct sockaddr *)&m_server,m_addrlen);
}


int Udp_client::socket_create(string ip,uint16_t port)
{
    if((m_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Creatingsocket failed.");
        exit(1);
    }

    bzero(&m_server,sizeof(m_server));
    m_server.sin_family=AF_INET;
    m_server.sin_port=htons(port);
    inet_pton(AF_INET,ip.c_str(),&m_server.sin_addr);
    m_addrlen = sizeof(m_server);

    return 0;
}



