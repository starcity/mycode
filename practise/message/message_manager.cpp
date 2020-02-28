#include <stddef.h>
#include "message_manager.h"
#include "return_code.h"


Message_manager::Message_manager()
{

}

Message_manager::~Message_manager()
{

}

bool Message_manager::init()
{
}

int32_t Message_manager::register_message(MESSAGE_TYPE type,Base_message *base)
{
    if(m_map_handler.find(type) != m_map_handler.end())
        return MESSAGE_TYPE_ALREADY_REGISTER;

    m_map_handler[type] = base;
}

void Message_manager::on_message(MESSAGE_TYPE type,const void *data,uint32_t len)
{
    if(m_map_handler.find(type) == m_map_handler.end())
        return ;
    (*m_map_handler[type])(data,len);
}
