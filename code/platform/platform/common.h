#ifndef     COMMON_H
#define     COMMON_H

#include "log/Log.h"
#include <stdio.h>



using namespace std;



//日志宏
#define   WRITELOGERROR(fmt,...)    gClog.WriteLog(CLog::LL_ERROR,"%s:%d:" fmt,__FILE__,__LINE__,## __VA_ARGS__)


#define   WRITELOGINFO(fmt,...)     gClog.WriteLog(CLog::LL_INFORMATION,"%s:%d:" fmt,__FILE__,__LINE__,## __VA_ARGS__)





#endif
