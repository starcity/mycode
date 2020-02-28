#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "tcp_server.h"

#define IPADDRESS   "0.0.0.0"




Tcp_server::Tcp_server(string ip,uint16_t port,Data_recv *pdata):m_ip(ip),m_port(port),m_pdata(pdata)
{
    m_bstart = true;//重新监测
    m_bstop  = false;//停止工作

}

Tcp_server::~Tcp_server()
{
    close(m_fd);
}

void Tcp_server::run()
{
    signal(SIGPIPE,SIG_IGN);
//    signal(SIGALRM,SIG_IGN);
//    signal(SIGINT,SIG_IGN);

    while(!m_bstop)
    {
        socket_bind(m_ip,m_port);
        do_epoll();
    }
}

void Tcp_server::stop()
{
    m_bstop = true;
}

void Tcp_server::close_client(int fd)
{
    if(fd <= 0)return ;
    epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

int Tcp_server::send_data(int fd, const char *buf, int len)
{
    int ret = 0;
    int n = 0;
    while (n < len) {
        ret = write(fd,buf + n,len - n);
        if(ret < (len - n))
        {
            if ((ret == -1) && (errno != EAGAIN))
            {
                string   ip;
                uint16_t port;
                get_peer_info(fd,ip,port);
                m_pdata->do_sock_closeed(fd,ip,port,strerror(errno));
                close_client(fd);
                return -1;
            }
            break;
        }
        n += ret;
    }

    return n;
}

int Tcp_server::get_peer_info(int fd,string &ip, uint16_t &port)
{
    if(fd < 0)return -1;
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    getpeername(fd, (struct sockaddr *)&sa, &len);
    ip = inet_ntoa(sa.sin_addr);
    port = ntohs(sa.sin_port);

    return 0;
}

int Tcp_server::socket_bind(string ip,uint16_t port)
{
    struct sockaddr_in servaddr;
    m_fd = socket(AF_INET,SOCK_STREAM,0);
    if (m_fd == -1)
    {
        perror("socket error:");
        exit(1);
    }

    //把监听socket设置为非阻塞方式
    set_non_blocking(m_fd);
    //设置监听socket为端口重用
    set_reuse_addr(m_fd);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip.c_str(),&servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(m_fd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }

    listen(m_fd,LISTENQ);
    return m_fd;
}

void Tcp_server::do_epoll()
{
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    //创建一个描述符
    m_epollfd = epoll_create(FDSIZE);//自从Linux2.6.8版本以后，size值其实是没什么用的，不过要大于0，因为内核可以动态的分配大小，所以不需要size这个提示了。
    //添加监听描述符事件
    add_event(m_fd,EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
    while(m_bstart && (m_bstop == false))
    {
        //获取已经准备好的描述符事件
        ret = epoll_wait(m_epollfd,events,EPOLLEVENTS,-1);
        handle_events(events,ret,buf);
    }
    close(m_epollfd);
}

void Tcp_server::handle_events(struct epoll_event *events,int num,char *buf)
{
    int fd;
    //进行选好遍历
    for (int i = 0;i < num;i++)
    {
        fd = events[i].data.fd;
        //根据描述符的类型和事件类型进行处理
        if ((fd == m_fd) &&(events[i].events & EPOLLIN))
            handle_accpet();
        else if (events[i].events & EPOLLIN)
            do_read(fd,buf);
        else if (events[i].events & EPOLLOUT)
            do_write(fd,buf);
    }
}

void Tcp_server::handle_accpet()
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen = sizeof(cliaddr);
    while((clifd = accept(m_fd,(struct sockaddr*)&cliaddr,&cliaddrlen)) > 0)
    {
        set_non_blocking(clifd);
        //添加一个客户描述符和事件
        add_event(clifd,EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET);
        m_pdata->do_sock_connected(clifd,inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
    }
    if (clifd == -1)
    {
        if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
        {
            m_bstart = false;
            close(m_fd);
        }
    }
}

//设置socket连接为非阻塞模式
void Tcp_server::set_non_blocking(int sockfd)
{
    int opts;

    opts = fcntl(sockfd, F_GETFL);
    if(opts < 0) {
        perror("fcntl(F_GETFL)\n");
        exit(1);
    }
    opts = (opts | O_NONBLOCK);
    if(fcntl(sockfd, F_SETFL, opts) < 0) {
        perror("fcntl(F_SETFL)\n");
        exit(1);
    }
}

void Tcp_server::set_reuse_addr(int sockfd)
{
    int opt;
    opt = 1;
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(&opt)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
}

void Tcp_server::do_read(int fd,char *buf)
{
    int nread = 0;
    while((nread = read(fd,buf + index,MAXSIZE)) > 0)
    {
        m_pdata->do_data_recv(fd,buf,nread);
    }
    if (nread < 0)
    {
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))    //没有数据了
        {
           return ;
        }
        string   ip;
        uint16_t port;
        get_peer_info(fd,ip,port);
        m_pdata->do_sock_closeed(fd,ip,port,strerror(errno));

        close_client(fd);
    }
    else if (nread == 0)
    {
        string   ip;
        uint16_t port;
        get_peer_info(fd,ip,port);
        m_pdata->do_sock_closeed(fd,ip,port,strerror(errno));
        close_client(fd);
    }
}

void Tcp_server::do_write(int fd,char *buf)
{
//    int nwrite;
//    nwrite = write(fd,buf,strlen(buf));
//    if (nwrite == -1)
//    {
//        perror("write error:");
//        close_client(fd);
//        return ;
//    }
}

void Tcp_server::add_event(int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,&ev);
}

void Tcp_server::delete_event(int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(m_epollfd,EPOLL_CTL_DEL,fd,&ev);
}

void Tcp_server::modify_event(int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(m_epollfd,EPOLL_CTL_MOD,fd,&ev);
}
