#include "test_message.h"
#include <iostream>

using namespace std;

Test_message::Test_message()
{
}

void Test_message::operator()(const void *data,uint32_t len)
{
       cout<<(char *)data<<endl;
}
