#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H
#include <stdint.h>
#include <map>
#include "base_message.h"

using namespace std;

class Message_manager
{
  public:
    Message_manager();
    ~Message_manager();

    bool init();
    int32_t  register_message(MESSAGE_TYPE type,Base_message *base);
    void on_message(MESSAGE_TYPE type,const void *data,uint32_t len);

private:
    map<uint32_t,Base_message *> m_map_handler;
};

#endif // MESSAGE_MANAGER_H
