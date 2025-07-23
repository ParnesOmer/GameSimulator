#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstring>

class TimeConverter {
public:
    static time_t stringToTime(const std::string& timeStr) {
        int hours = 0, minutes = 0;

        if (sscanf(timeStr.c_str(), "%d:%d", &hours, &minutes) != 2) {
            throw std::invalid_argument("Invalid time format: " + timeStr);
        }
        return hours * 3600 + minutes * 60;
    }
    
    static std::string timeToString(time_t time) {
        int hours = static_cast<int>(time / 3600);
        int minutes = static_cast<int>((time % 3600) / 60);
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes;
        return oss.str();
    }
};

#endif // TIME_HPP
