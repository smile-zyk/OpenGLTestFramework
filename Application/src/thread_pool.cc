#include "thread_pool.h"

ThreadPool& GetThreadPool()
{
    static ThreadPool tp;
    return tp;
}