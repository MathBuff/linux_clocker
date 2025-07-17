#include "clock.h"

std::chrono::time_point<std::chrono::system_clock> Clock::getCurrentTimePoint() {
    return std::chrono::system_clock::now();
}

std::time_t Clock::timePointToTimeT(const std::chrono::time_point<std::chrono::system_clock>& tp) {
    return std::chrono::system_clock::to_time_t(tp);
}
