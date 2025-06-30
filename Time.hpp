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
        int hours, minutes;
        char colon;
        std::istringstream iss(timeStr);
        
        if (iss >> hours >> colon >> minutes && colon == ':' && 
            hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59) {
            
            time_t now = time(NULL);
            struct tm timeInfo = *localtime(&now);
            timeInfo.tm_hour = hours;
            timeInfo.tm_min = minutes;
            timeInfo.tm_sec = 0;
            return mktime(&timeInfo);
        }
        return 0;
    }
    
    static std::string timeToString(time_t time) {
        struct tm* timeInfo = localtime(&time);
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << timeInfo->tm_hour << ":"
            << std::setfill('0') << std::setw(2) << timeInfo->tm_min;
        return oss.str();
    }
};

#endif // TIME_HPP
