#include "net.h"

#define	  MAX_EPOLL_SIZE		500 


net::net(base_callback *pbcb,base_socket *pbs):m_pbase_callback(pbcb),m_pbase_socket(pbs)
{
}

net::~net()
{
	for(auto t:m_array_thread)
		t->join();
}

int32_t net::init(uint32_t nthread)
{
	m_epfd = epoll_create(MAX_EPOLL_SIZE);
	if( m_epfd < 0 )
		return m_epfd;

	for( int i = 0 ; i < nthread;i ++){
		thread *t(new thread(&net::work,this));
		m_array_thread.push_back(t);
	}
}

void net::run()
{  
	int nfds,connfd,fd;
	struct epoll_event	events[MAX_EPOLL_SIZE];
	struct epoll_event	ev;
	struct sockaddr_in	client_addr;
	int					client_len = sizeof(client_addr);

	int listen_fd = m_pbase_socket->get_socket_fd();

	for( ; ; ){
		nfds = epoll_wait(m_epfd,events,MAX_EPOLL_SIZE,0);  
		for(int i = 0; i < nfds ; i++ )  
		{  
			fd = events[i].data.fd;
			if(m_map_task.find(fd) == m_map_task.end())
				continue;

			if(fd == listen_fd) 
			{  
				connfd = accept(listen_fd,(sockaddr *)&client_addr, &clilen_len); //accept这个连接  
				ev.data.fd = connfd;  
				ev.events = EPOLLIN|EPOLLET;  
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //将新的fd添加到epoll的监听队列中  
			}  
			else if( events[i].events & EPOLLIN ) //接收到数据
			{  
				n = read(sockfd, line, MAXLINE)) < 0    //读  
					ev.data.ptr = md;     //md为自定义类型，添加数据  
				ev.events=EPOLLOUT|EPOLLET;  
				epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);//修改标识符，等待下一个循环时发送数据，异步处理的精髓  
			}  
			else if(events[i].events&EPOLLOUT) //有数据待发送，写socket  
			{  
				struct myepoll_data* md = (myepoll_data*)events[i].data.ptr;    //取数据  
				sockfd = md->fd;  
				send( sockfd, md->ptr, strlen((char*)md->ptr), 0 );        //发送数据  
				ev.data.fd=sockfd;  
				ev.events=EPOLLIN|EPOLLET;  
				epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改标识符，等待下一个循环时接收数据  
			}  
			else  
			{  
				//其他的处理  
			}  
		}  
	}
} 
