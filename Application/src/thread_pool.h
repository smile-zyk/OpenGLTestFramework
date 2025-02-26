#pragma once

#include <BS_thread_pool.hpp>

using ThreadPool = BS::thread_pool<>;

ThreadPool& GetThreadPool();