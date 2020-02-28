#include "cond.h"
#include <unistd.h>


int main()
{
	Mythread m(0);

	m.start();
	for(int j = 0;j < 100;j++){
	for(int i = 0 ;i < 10000;i++)
		m.add_task(i);
		usleep(1000);
	}

	sleep(30);
}
