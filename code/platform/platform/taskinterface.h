#ifndef PLATFORMINTERFACE
#define PLATFORMINTERFACE
#include <evhttp.h>
#include <string>
#include <stdint.h>

using namespace std;

class TaskInterface
{
public:
    virtual int32_t init() = 0;
    virtual int32_t doPlatform(struct evkeyvalq &params,string &reInfo) = 0;
};

#endif // PLATFORMINTERFACE

