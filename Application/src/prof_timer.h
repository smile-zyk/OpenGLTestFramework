#pragma once

#include <string>
#include <chrono>

class ProfTimer {
    public:
        explicit ProfTimer(const std::string& name = "Timer") ;
    
        void Start(); 

        void Stop();

        template<class DURATION>
        DURATION SinceStart() const
        {
            auto t = stop_time_;
            if(is_running_ == true)
                t = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<DURATION>(
                       t - start_time_);
        }
    
        double SinceStartSecs();

        double SinceStartMicroSecs();

        double SinceStartMilliSecs();

        double SinceStartNanoSecs();
    
        ~ProfTimer() {}
    
    private:
        std::string name_;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
        std::chrono::time_point<std::chrono::high_resolution_clock> stop_time_;
        bool is_running_{false};
    };