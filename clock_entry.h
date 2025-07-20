#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "clock.h"

class ClockEntry{
    private:
        bool completedEntry = false;
        int id = 0; //Shows who clocked in
        std::chrono::system_clock::time_point clockIn;
        std::chrono::system_clock::time_point clockOut;    
        //^These are automatically initialized as 0
        float shiftTimeSeconds;

    public:
        void setUserID(int newID);
        int getUserID() const;

        void setEntryComplete();
        void setEntryInComplete(); //<---The change

        void clockInUser();
        std::chrono::system_clock::time_point getClockIn() const ;

        void clockOutUser();
        std::chrono::system_clock::time_point getClockOut() const;


        bool isCompleted() const;

        float calculateShiftTimeSeconds();
        float getShiftTimeSeconds() const;

        std::string timePointToString(const std::chrono::system_clock::time_point& tp, const std::string& fallback = "[Not Set]") const;

        void print();

        std::string toString() const;

        void setClockIn(const std::chrono::system_clock::time_point& in);

        void setClockOut(const std::chrono::system_clock::time_point& out);


};


