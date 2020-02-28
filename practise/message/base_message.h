#ifndef BASE_MESSAGE_H
#define BASE_MESSAGE_H
#include <stdint.h>


enum MESSAGE_TYPE
{
    TEST_MESSAGE1 = 0,
    TEST_MESSAGE2 = 1,
};

class Base_message
{
public:
    virtual ~Base_message(){}
    /**********该函数不能为阻塞型函数**************************/
    virtual void operator()(const void *data,uint32_t len) = 0;
};


#endif // BASE_MESSAGE_H
