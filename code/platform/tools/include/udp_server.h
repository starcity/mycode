#ifndef UDP_SERVER_H
#define UDP_SERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <map>
#include <stdint.h>
#include "data_recv.h"


using namespace std;

typedef map<int,struct sockaddr_in> Mfd_addr;


class map_fd_addr_find
{
public:
    map_fd_addr_find(struct sockaddr_in addr):m_addr(addr){}
    bool operator ()(Mfd_addr::value_type &value)
    {
        return ((value.second.sin_port == m_addr.sin_port) && (value.second.sin_addr.s_addr == m_addr.sin_addr.s_addr));
    }
private:
    struct sockaddr_in m_addr;
};

class Udp_server
{
public:
    enum {MAX_BUF=2048};
    enum {FD_START=3};


    Udp_server(string ip,uint16_t port,Data_recv *pdata);
    ~Udp_server();

    int     socket_bind(string ip,uint16_t port);
    void    run();
    int     send_data(int fd,const char *buf,int len);
    int     get_peer_info(int fd,string &ip,uint16_t &port);


private:
    string      m_ip;
    uint16_t    m_port;
    int         m_fd;

    Data_recv  *m_pdata;
    Mfd_addr    m_mfd_addr;
    int         m_socketfd;
};

#endif // UDP_SERVER_H
