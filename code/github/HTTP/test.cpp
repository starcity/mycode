#include "slog.h"

int main() 
{
    std::string  log_file =  "logs/main.log";
    CSingletonSLog::instance().init(log_file);
    LOG_WARN("{}:{} This is my log",__FILE__,__LINE__);
}
