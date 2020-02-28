#include "data_recv.h"
#include "tcp_client.h"
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;



class client:Data_recv
{
public:
    client()
    {
        m_client = new Tcp_client("127,0,0,1",20000,this);
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

    //新客户端连接
    virtual int32_t do_sock_connected(int fd,string ip,uint16_t port)
    {
        cout<<"connect"<<fd<<" "<<ip<<" "<<port<<endl;
    }

    //客户端主动断开
    virtual int32_t do_sock_closeed(int fd,string ip,uint16_t port,string error)
    {
        cout<<"close"<<fd<<" "<<ip<<" "<<port<<" "<<error<<endl;
    }
	int32_t send_data(const  char *buf,int len)
	{
		return m_client->send_data(buf,len);
	}

    void run(){m_client->run();}
private:
    Tcp_client   *m_client;
};

client c;

void run()
{
    c.run();
}

int main()
{
	thread  t(run);
    t.detach();

	char data[9];
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0x07;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 0x09;
	data[7] = 0x09;
	data[8] = 0x01;

	int i = 0;
	int total = 0;
	while(i != 500000){
		total +=c.send_data(data,9);
		usleep(1);
		i++;
	}
	printf("total:%d %d\n",i,total);
	sleep(120);
}
