#include "mc_tcp.h"

#define		LISTEN_EVENTS	512

mc_tcp::mc_tcp(mc_tcp::TYPE type,const char *ip,uint16_t port)
{
	m_type = type;
	m_ip = inet_addr(ip);
	m_port = port;

	m_fd = socket(AF_INET, SOCK_STREAM, 0);
}

mc_tcp::~mc_tcp()
{
}

int32_t mc_tcp::init()
{
	if( m_fd < 0 )
		return m_fd;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = m_ip;

	if(type == TCP_SERVER){
			int ret = bind(m_fd,(struct sockaddr *)&addr,sizeof(addr));
		if( ret < 0 )
			return ret;

		ret = listen(m_fd,LISTEN_EVENTS);
		if( ret < 0 )
			return ret;
	}
	else {
		connect(m_fd,(struct sockaddr *)&addr,sizeof(addr));
	}
	return 0;
}

int32_t mc_tcp::get_socket_fd()
{
	return m_fd;
}
