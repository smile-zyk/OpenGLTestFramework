#include "prof_timer.h"
#include <chrono>
#include <ratio>


ProfTimer::ProfTimer(const std::string& name): name_(name) 
{
    Start();
}

void ProfTimer::Start()
{
    is_running_ = true;
    start_time_ = std::chrono::high_resolution_clock::now();
}

void ProfTimer::Stop()
{
    if(!is_running_) return;
    stop_time_ = std::chrono::high_resolution_clock::now();
    is_running_ = false;
}

double ProfTimer::SinceStartSecs()
{
    using DUR = std::chrono::duration<double>;
    return SinceStart<DUR>().count();
}

double ProfTimer::SinceStartMicroSecs()
{
    using DUR = std::chrono::duration<double, std::micro>;
    return SinceStart<DUR>().count();
}

double ProfTimer::SinceStartMilliSecs()
{
    using DUR = std::chrono::duration<double, std::milli>;
    return SinceStart<DUR>().count();
}

double ProfTimer::SinceStartNanoSecs()
{
    using DUR = std::chrono::duration<double, std::nano>;
    return SinceStart<DUR>().count();
}
