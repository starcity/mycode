#include "cond.h"


Mythread::Mythread(int32_t nthread)
{
	if(0 == nthread)
		m_thread_number = thread::hardware_concurrency();
	else 
		m_thread_number = nthread;
    m_running = true;	
}

Mythread::~Mythread()
{
	m_running = false;
	m_cond.notify_all();
	for(auto t:m_array_thread)
		t->join();
}

void Mythread::start()
{
	for(int32_t i = 0;i < m_thread_number;i++)
	{
		thread *t(new thread(&Mythread::task_contention,this));
		m_array_thread.push_back(t);
	}
}

void Mythread::task_contention()
{
	while(m_running){
		std::unique_lock<std::mutex> lk(m_mutex);
		while(m_list_task.empty()){
			m_cond.wait(lk);
			if(!m_running)
				break;
		}
		if(!m_list_task.empty()){
			Task task = m_list_task.front();
			m_list_task.pop_front();
			lk.unlock();
			work(task);
		}
	}
}

void Mythread::work(Task &task)
{
	cout<<std::this_thread::get_id()<<":"<<task.value<<endl;
}

void Mythread::add_task(int32_t value)
{
	Task task;
	task.value = value;
	std::unique_lock<std::mutex> lk(m_mutex);
	m_list_task.push_back(task);
	m_cond.notify_one();
	lk.unlock();
}
