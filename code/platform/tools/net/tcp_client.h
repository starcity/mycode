#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <stdint.h>
#include "data_recv.h"


using namespace std;


class Tcp_client
{
public:
    //常量s
    enum return_code{SUCESS = 0,
          CREATE_SOCKET_FAILED = -1,
          CONNECT_FAILED=-2,
         };
    enum {RECV_BUF = 2048};

    Tcp_client(string ip,uint16_t port,Data_recv *pdata);
    ~Tcp_client();

    void run();
    void set_reconnect();
    void close_connect();
    int  send_data(const char *buf,int len);
    int  get_peer_info(string &ip,uint16_t &port);


private:
    //创建套接字并进行绑定
    int create_socket(string ip,uint16_t port);
    //读处理
    void do_read(int fd,char *buf);


private:
    string      m_ip;
    uint16_t    m_port;
    int         m_fd;
    bool        m_reconnect;//断开重连标识 默认不重连

    Data_recv  *m_pdata;

};



#endif // TCP_CLIENT_H
