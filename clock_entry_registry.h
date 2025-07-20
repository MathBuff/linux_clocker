#pragma once
#include <optional>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include "clock_entry.h"


class ClockEntryRegistry{
    private:
        std::vector<ClockEntry> data;

        ClockEntry* findActiveClockEntryByID(int userID);

    public:
            bool clockUser (int userID);
            std::vector<ClockEntry>* getData();
            void print();
            float getUserLifeTimeSeconds(int userID);
            size_t size() const;
            bool deleteAtPosition(size_t index);
            void add(ClockEntry& newEntry);
            float getUserCalendarWeekSeconds(int userID);
            ClockEntry getLatestClockEntry(int userID); 
            void saveToFile(const std::string& filename = "clock_data.bin");
            void loadFromFile(const std::string& filename = "clock_data.bin");
};
