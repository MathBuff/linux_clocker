#include "clock_entry_registry.h"

//PRIVATE=================================================================================

ClockEntry* ClockEntryRegistry::findActiveClockEntryByID(int userID) {
    for (ClockEntry& entry : data) {
        if (entry.getUserID() == userID && !entry.isCompleted()) {
            return &entry;
        }
    }
    return nullptr; // Not found
}

//PUBLIC==================================================================================

bool ClockEntryRegistry::clockUser (int userID){

    ClockEntry* clockOutEntry = findActiveClockEntryByID(userID);

    if(clockOutEntry != nullptr ){
        clockOutEntry->clockOutUser();
        return true;
        //The User had an active entry and clocked out
    }
    else{
        ClockEntry newEntry;
        newEntry.setUserID(userID);
        newEntry.clockInUser();
        data.insert(data.begin(), newEntry);
        return false;
        //The user had no active entry and clocked in
    }
    
}

std::vector<ClockEntry>* ClockEntryRegistry::getData(){

    std::vector<ClockEntry>* ptrToVector = &data;

    return ptrToVector;
}

void ClockEntryRegistry::print() {
    if (data.empty()) {
        std::cout << "No clock entries available.\n";
        return;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        data[i].print();  // Example usage
    }



}

float ClockEntryRegistry::getUserLifeTimeSeconds(int userID) {
    float totalSeconds = 0.0f;

    for (const ClockEntry& entry : data) {
        if (entry.getUserID() == userID && entry.isCompleted()) {
            totalSeconds += entry.getShiftTimeSeconds();
        }
    }

    return totalSeconds;
}

size_t ClockEntryRegistry::size() const {
    return data.size();
}

bool ClockEntryRegistry::deleteAtPosition(size_t index) {
    if (index >= data.size()) {
        return false; // Invalid index
    }

    data.erase(data.begin() + index);
    return true;
}

void ClockEntryRegistry::add(ClockEntry& newEntry){
    data.push_back(newEntry);
}

float ClockEntryRegistry::getUserCalendarWeekSeconds(int userID) {
    using namespace std::chrono;

    // Get current time
    auto now = system_clock::now();

    // Convert to local time to find start of the week (Monday 00:00)
    std::time_t tt = system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&tt);

    int wday = local_tm.tm_wday; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    int days_from_monday = (wday + 6) % 7; // Sunday (0) -> 6, Monday (1) -> 0

    // Set to midnight today
    local_tm.tm_hour = 0;
    local_tm.tm_min = 0;
    local_tm.tm_sec = 0;
    tt = std::mktime(&local_tm);

    auto monday = system_clock::from_time_t(tt) - hours(24 * days_from_monday);
    auto sunday = monday + hours(24 * 7); // end of week (next Monday 00:00)

    float total_seconds = 0.0f;

    for (const auto &entry : data) {
        if (entry.getUserID() == userID && entry.isCompleted()) {
            auto in = entry.getClockIn();
            auto out = entry.getClockOut();

            // Check if the entry overlaps this week
            if (out > monday && in < sunday) {
                // If fully inside, use stored shift time
                if (in >= monday && out <= sunday) {
                    total_seconds += entry.getShiftTimeSeconds();
                } else {
                    // Clip to week bounds for partial overlap
                    auto start = std::max(in, monday);
                    auto end = std::min(out, sunday);
                    total_seconds += duration_cast<seconds>(end - start).count();
                }
            }
        }
    }

    return total_seconds;
}

ClockEntry ClockEntryRegistry::getLatestClockEntry(int userID) {
    ClockEntry* latest = nullptr;

    for (auto& entry : data) {
        if (entry.getUserID() == userID) {
            if (!latest || entry.getClockIn() > latest->getClockIn()) {
                latest = &entry;
            }
        }
    }

    if (latest) {
        return *latest;  // Return a copy of the latest entry
    }

    throw std::runtime_error("ClockEntryRegistry::getLatestClockEntry says No clock entries found for user ID : " + std::to_string(userID));
}


