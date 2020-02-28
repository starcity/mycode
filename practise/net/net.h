#include "base_callback.h"
#include "base_socket.h"
#include "ring_buffer.h"
#include <memory>
#include <map>
#include <thread>
#include <vector>

using namespace std;

typedef struct task
{
	int32_t				event;
	shared_ptr<char *>  ptr;   
}task;


class net
{
	public:
		net();
		~net();

		int32_t init(uint32_t nthread);
		void	run();

	private:
		void	work();

	private:
		bool                m_running;
		int					m_epfd;
		struct epoll_event  m_event;
		base_socket			*m_pbase_socket;
		base_callback		*m_pbase_callback;
		vector<thread *>    m_array_thread; 
		map<int32_t,task>	m_map_task;

};
