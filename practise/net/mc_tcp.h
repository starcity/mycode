#include "base_socket.h"


class mc_tcp:public base_socket
{
	public:
		enum TYPE {
			TCP_SERVER = 0，
			TCP_CLIETN,
		};

		mc_tcp();
		~mc_tcp();
	
		int32_t init();
		int32_t connect();
		int32_t get_socket_fd();

	private:
		TYPE			m_type;
		int				m_fd;
		uint32_t        m_ip;
		uint16_t 		m_port;
};
