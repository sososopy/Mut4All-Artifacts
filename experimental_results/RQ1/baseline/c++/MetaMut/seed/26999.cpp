#include <atomic>

class LogEntries
{
public:
    char                sDateTime [20];
    std::atomic<bool>   bReadyToFlush;
};

class LogThreads
{
public:
    static LogEntries   leLogEntries [10];
};

LogEntries LogThreads::leLogEntries [10] {};

