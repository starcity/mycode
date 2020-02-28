#include "message_manager.h"
#include "test_message.h"


int main(int argc,char *argv[])
{
    Test_message test_message1;
    Test_message test_message2;
    Message_manager manager;

    manager.init();
    manager.register_message(TEST_MESSAGE1,&test_message1);
    manager.register_message(TEST_MESSAGE2,&test_message2);

    manager.on_message(TEST_MESSAGE1,"hello message1",12);
    manager.on_message(TEST_MESSAGE2,"hello message2",12);

}
