#ifndef  Tcp_server_H
#define  Tcp_server_H

#include <string>
#include <stdint.h>
#include "data_recv.h"


using namespace std;


class Tcp_server
{
public:
    //常量s
    enum {FDSIZE = 1024,
          EPOLLEVENTS =1024,
          MAXSIZE=2048,
          LISTENQ=100
         };

    Tcp_server(string ip,uint16_t port,Data_recv *pdata);
    ~Tcp_server();

    void run();
    void stop();
    void close_client(int fd);
    int  send_data(int fd,const char *buf,int len);
    int  get_peer_info(int fd,string &ip,uint16_t &port);


private:
    //创建套接字并进行绑定
    int socket_bind(string ip,uint16_t port);
    //IO多路复用epoll
    void do_epoll();
    //事件处理函数
    void handle_events(struct epoll_event *events,int num,char *buf);
    //处理接收到的连接
    void handle_accpet();
    //读处理
    void do_read(int fd,char *buf);
    //写处理
    void do_write(int fd,char *buf);
    //添加事件
    void add_event(int fd,int state);
    //修改事件
    void modify_event(int fd,int state);
    //删除事件
    void delete_event(int fd,int state);
    //设置套接字为非阻塞
    void set_non_blocking(int sockfd);
    void set_reuse_addr(int sockfd);

private:
    string      m_ip;
    uint16_t    m_port;
    int         m_fd;
    int         m_epollfd;

    Data_recv  *m_pdata;

    bool         m_bstart;
    bool         m_bstop;
};



#endif
