#include <stdint.h>


class base_socket
{
	public:
		virtual int32_t init() = 0;
		virtual int32_t get_socket_fd() = 0;
};
