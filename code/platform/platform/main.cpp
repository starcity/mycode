#include "httpserver.h"
//#include "MDSParser.h"

CLog    gClog;

int main(int argc, char *argv[])
{
 	WRITELOGINFO("programm start.....!");
        httpServer server;

    server.start();

    return 0;
}

