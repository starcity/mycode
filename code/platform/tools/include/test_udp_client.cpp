#include <string>
#include <iostream>
#include "udp_client.h"
#include "data_recv.h"


using namespace std;



class Test:public Data_recv
{
public:
    Test()
    {
        client = new Udp_client("127.0.0.1",12345,this);
    }
    //不能阻塞快速返回
    virtual int32_t do_data_tcp_recv(int fd,const char *data,int32_t len)
    {
        cout<<fd<<" "<<data<<" "<<len<<endl;
    }

   virtual int32_t do_data_udp_recv(int fd,const char *data,int32_t len)
    {
        cout<<fd<<" "<<data<<" "<<len<<endl;
    }


    //新客户端连接 只对tcp有用
    virtual int32_t do_sock_connected(int fd,string ip,uint16_t port){}
    //客户端主动断开 只对tcp有用
    virtual int32_t do_sock_closeed(int fd,string ip,uint16_t port,string error) {}

    void send_data()
    {
        client->send_data("12345",5);
    }

    void run()
    {
        client->run();
    }

private:
    Udp_client *client;
};

int main()
{
    Test t;
    t.send_data();
    t.run();
}
