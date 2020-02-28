#include "base_message.h"


class Test_message:public Base_message
{
  public:
    Test_message();
    void operator()(const void *data,uint32_t len);
};
