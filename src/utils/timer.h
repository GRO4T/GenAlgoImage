//
// Created by DamianPC on 7/7/2020.
//

#ifndef UNTITLED_TIMER_H
#define UNTITLED_TIMER_H

#include <iostream>
#include <chrono>

namespace gen_algo_image{
    typedef std::chrono::steady_clock::time_point timepoint;

    class Timer{
    public:
        void Start(){
            startTime = std::chrono::steady_clock::now();
        }
        void Stop(){
            endTime = std::chrono::steady_clock::now();
        }
        void Time(){
            std::cout << "time: " <<
                      std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()
                      << "ms" << std::endl;
        }
    private:
        timepoint startTime;
        timepoint endTime;
    };
}

#endif //UNTITLED_TIMER_H
