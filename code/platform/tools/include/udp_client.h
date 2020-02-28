#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H
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

class Udp_client
{
public:
    enum {MAX_BUF=2048};

    Udp_client(string ip,uint16_t port,Data_recv *pdata);
    ~Udp_client();

    void    run();
    int     send_data(const char *buf,int len);

private:
    int socket_create(string ip,uint16_t port);

private:
    string      m_ip;
    uint16_t    m_port;
    int         m_fd;

    Data_recv   *m_pdata;
    struct sockaddr_in m_server;
    socklen_t    m_addrlen;
};

#endif // UDP_CLIENT_H
