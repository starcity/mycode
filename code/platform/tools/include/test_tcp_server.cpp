#include "data_recv.h"
#include "tcp_server.h"
#include <iostream>


using namespace std;


class Test:public Data_recv
{
public:
    Test()
    {
        server = new Tcp_server("0.0.0.0",12345,this);
    }

    //不能阻塞快速返回
    virtual int32_t do_data_tcp_recv(int fd,const char *data,int32_t len)
    {
        cout<<fd<<" "<<data<<" "<<len<<endl;
        server->send_data(fd,"hello",5);
    }

	virtual int32_t do_data_udp_recv(int fd,const char *data,int32_t len)
    {
        cout<<fd<<" "<<data<<" "<<len<<endl;
        server->send_data(fd,"hello",5);
    }
    //新客户端连接
    virtual int32_t do_sock_connected(int fd,string ip,uint16_t port)
    {
                cout<<fd<<" "<<ip<<" "<<port<<endl;
                server->send_data(fd,"hello",5);
    }

    //客户端主动断开
    virtual int32_t do_sock_closeed(int fd,string ip,uint16_t port,string error)
    {
        cout<<fd<<" "<<ip<<" "<<port<<" "<<error<<endl;
    }

    void run()
    {
        server->run();
    }

private:
     Tcp_server         *server;
};

int main()
{
    Test t;
    t.run();

    return 0;
}
