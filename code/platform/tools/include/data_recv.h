#ifndef DATA_RECV_H
#define DATA_RECV_H
#include <stdint.h>
#include <string>



using namespace std;

class Data_recv
{
public:
    //不能阻塞快速返回
    virtual int32_t do_data_tcp_recv(int fd,const char *data,int32_t len) = 0;
    //不能阻塞快速返回
    virtual int32_t do_data_udp_recv(int fd,const char *data,int32_t len) = 0;
    //新客户端连接 只对tcp有用
    virtual int32_t do_sock_connected(int fd,string ip,uint16_t port) = 0;
    //客户端主动断开 只对tcp有用
    virtual int32_t do_sock_closeed(int fd,string ip,uint16_t port,string error) = 0;
};




#endif // DATA_RECV_H
