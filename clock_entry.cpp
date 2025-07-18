#include "clock_entry.h"

void ClockEntry::setUserID(int newID) {
    id = newID;
}

int ClockEntry::getUserID()  const {
    return id;
}

void ClockEntry::setEntryComplete(){
    completedEntry = true;
}

void ClockEntry::clockInUser() {
    //Saftey Check
    if(completedEntry == true){
        return;
    }
    //--

    Clock clk;
    clockIn = clk.getCurrentTimePoint();
    completedEntry = false;
}

std::chrono::system_clock::time_point ClockEntry::getClockIn() const { 
    return clockIn; 
}


void ClockEntry::clockOutUser() {
    //Saftey Check
    if(completedEntry == true){
        return;
    }
    //--

    Clock clk;
    clockOut = clk.getCurrentTimePoint();
    completedEntry = true;
    calculateShiftTimeSeconds();
}

std::chrono::system_clock::time_point ClockEntry::getClockOut() const { 
    return clockOut; 
}

bool ClockEntry::isCompleted() const { 
    return completedEntry; 
}

float ClockEntry::calculateShiftTimeSeconds(){

    std::chrono::duration<float> diff = clockOut - clockIn;
    
    float result = diff.count();

    shiftTimeSeconds = result;

    return result;
}

float ClockEntry::getShiftTimeSeconds() const {
    if (!completedEntry) return 0.0f;

    std::chrono::duration<float> duration = clockOut - clockIn;
    return duration.count();
}

std::string ClockEntry::timePointToString(const std::chrono::system_clock::time_point& tp, const std::string& fallback) const {
    if (tp.time_since_epoch().count() == 0) {
        return fallback;
    }

    std::time_t timeT = std::chrono::system_clock::to_time_t(tp);
    std::tm* tmPtr = std::localtime(&timeT);
    if (!tmPtr) return "[Corrupt]";

    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void ClockEntry::print() {
    std::cout << toString() << std::endl;
}


std::string ClockEntry::toString() const {
    std::ostringstream out;
    out << id << " "
        << (completedEntry ? "true" : "false") << " "
        << timePointToString(clockIn) << " "
        << timePointToString(clockOut, "[Shift_Active]") << " ";

    if (completedEntry) {
        out << std::fixed << std::setprecision(2) << shiftTimeSeconds << " sec";
    } else {
        out << "[Elapsed_Time_n/a]";
    }

    return out.str();
}

void ClockEntry::setClockIn(const std::chrono::system_clock::time_point& in) {
    clockIn = in;
}

void ClockEntry::setClockOut(const std::chrono::system_clock::time_point& out) {
    clockOut = out;
    completedEntry = true;
    shiftTimeSeconds = calculateShiftTimeSeconds();
}

