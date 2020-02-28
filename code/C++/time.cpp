#include <iostream>
#include <iomanip>
#include <ctime>
#include <ratio>
#include <chrono>



int main()
{
		using namespace std::chrono;

		steady_clock::time_point t1 = steady_clock::now();

		for(auto i = 0;i < 1000;i++)
			system("ls -l > /dev/null");

		steady_clock::time_point t2 = steady_clock::now();

		duration<double> time_span = duration_cast<duration<double> >(t2 -t1);

		std::cout<<"run time:"<<time_span.count()<<std::endl;

		system_clock::time_point now = system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));

		std::cout<<" "<<std::put_time(std::localtime(&now_c),"%F %T") <<std::endl;

		    std::time_t t = std::time(nullptr);
			    std::tm tm = *std::localtime(&t);
				    std::cout.imbue(std::locale("ru_RU.utf8"));
					    std::cout << "ru_RU: " << std::put_time(&tm, "%c %Z") << '\n';
						    std::cout.imbue(std::locale("ja_JP.utf8"));
							    std::cout << "ja_JP: " << std::put_time(&tm, "%c %Z") << '\n';
}
