#ifndef TRACKBASE_HPP
#define TRACKBASE_HPP

#include "Geometry.hpp"
#include "Time.hpp"

const int NO_COURSE = -1;
const int NO_SPEED = 0;
const int SECONDS_PER_MINUTE = 60;
const int SECONDS_PER_HOUR = 3600;
const int MINUTES_PER_HOUR = 60;
const int DEFAULT_UPDATE_TIME = SECONDS_PER_HOUR;

class Trackbase {
    public:
        Trackbase(): position(Point(0, 0)), course(NO_COURSE), speed(NO_SPEED) {};
        Trackbase(double x, double y): position(Point(x, y)), course(NO_COURSE), speed(NO_SPEED) {};
        Trackbase(const Point& pos): position(pos), course(NO_COURSE), speed(NO_SPEED) {};
        ~Trackbase() = default;

        // Getters
        const Point& getPosition() const { return position; }
        double getCourse() const            { return course; }
        double getSpeed() const             { return speed; }

        // Setters
        void setPosition(double x, double y);
        void setPosition(const Point& pos);
        void setCourse(double course);
        void setCourse(const Point& destination);
        void setSpeed(double speed);

        // Setup Speed and Course by Time and Destination point.
        void setupByTime(time_t leave_time, const Point& destination, time_t arrive_time);

        // Update position with time factor (default 60 minutes)
        void update(int time_factor_minutes = DEFAULT_UPDATE_TIME);

        bool isNearby(const Point& destination, double radius) const;

    private:
        Point position;
        double course;
        double speed;  // km/h
};

#endif // TRACKBASE_HPP

