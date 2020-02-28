#include <iostream>
#include <string>

int main()
{
    std::string str("\0abc\0abc",8);
    std::string tmp = str;
    std::cout<<str.length()<<" "<<tmp.length()<<std::endl;
    const char *pp = str.c_str();
    char *p = const_cast<char *>(pp);
     std::cout<<p[0]<<std::endl;
     std::cout<<p[1]<<std::endl;
     std::cout<<p[2]<<std::endl;
     std::cout<<p[3]<<std::endl;
     std::cout<<p[4]<<std::endl;
    return 0;
}

