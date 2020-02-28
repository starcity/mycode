#include <list>
#include <vector>
#include <stdint.h>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>


using namespace std;

typedef struct Task
{
	int value;
}Task;


class Mythread
{
	public:
		Mythread(int32_t nthread);
		~Mythread();
		
		void start();
		void add_task(int32_t value);

	private:
		void task_contention();
		void work(Task &task);

	private:
		mutex				m_mutex;
		condition_variable  m_cond;
		list<Task>			m_list_task;
		vector<thread *>    m_array_thread;
		int32_t			    m_thread_number;
		bool				m_running;
};
