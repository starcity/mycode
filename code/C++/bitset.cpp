#include <iostream>
#include <bitset>



int main()
{
	const int bit = 30000000;
	std::bitset<bit> bt(std::string("010101010"));
	
	for(int i = 0;i < bit;i+=10000)
		std::cout<<bt[i]<<" ";
	std::cout<<std::endl;

}
