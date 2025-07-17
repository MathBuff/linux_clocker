#pragma once
#include <chrono>
#include <ctime>
#include <iostream>

class Clock{

    public:

        std::chrono::time_point<std::chrono::system_clock> getCurrentTimePoint(); 
            //returns time_point object at current time
            //It’s better to do arithmetic with time_point and duration objects from <chrono>.
            // If you want to keep a list of times, storing std::chrono::time_point objects is the best practice.

        std::time_t timePointToTimeT(const std::chrono::time_point<std::chrono::system_clock>& tp);
            //Converts time_point object to time_t

};


