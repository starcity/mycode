#include "udp_server.h"
#include <iostream>

using namespace std;


class Test:public Data_recv
{
public:
    Test()
    {
        m_server = new Udp_server("0.0.0.0",12345,this);
    }

    //不能阻塞快速返回
    virtual int32_t do_data_tcp_recv(int fd,const char *data,int32_t len)
    {
        cout<<fd<<" "<<data<<" "<<len<<endl;
        string ip;
        uint16_t port;
        m_server->get_peer_info(fd,ip,port);
        cout<<ip<<" "<<port<<endl;
        m_server->send_data(fd,"54321",5);
    }

   virtual int32_t do_data_udp_recv(int fd,const char *data,int32_t len)
    {
        cout<<fd<<" "<<data<<" "<<len<<endl;
        string ip;
        uint16_t port;
        m_server->get_peer_info(fd,ip,port);
        cout<<ip<<" "<<port<<endl;
        m_server->send_data(fd,"54321",5);
    }


    //新客户端连接 只对tcp有用
    virtual int32_t do_sock_connected(int fd,string ip,uint16_t port)
    {

    }

    //客户端主动断开 只对tcp有用
    virtual int32_t do_sock_closeed(int fd,string ip,uint16_t port,string error)
    {

    }
    void run()
    {
        m_server->run();
    }

private:
    Udp_server *m_server;
};

int main()
{
    Test t;
    t.run();
}
