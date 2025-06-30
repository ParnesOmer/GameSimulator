#ifndef TRACKBASE_HPP
#define TRACKBASE_HPP

#include "Geometry.hpp"
#include "Time.hpp"

class Trackbase {
    public:
        Trackbase(): position(Point(0, 0)), course(-1), speed(0) {};
        Trackbase(double x, double y): position(Point(x, y)), course(-1), speed(0) {};
        Trackbase(const Point& pos): position(pos), course(-1), speed(0) {};
        ~Trackbase() = default;

        // Getters
        const Point& getPosition() const { return position; }
        int getCourse() const            { return course; }
        int getSpeed() const             { return speed; }

        // Setters
        void setPosition(double x, double y);
        void setPosition(const Point& pos);
        void setCourse(int course);
        void setCourse(const Point& destination);
        void setSpeed(int speed);

        // Setup Speed and Course by Time and Destination point.
        void setupByTime(time_t leave_time, const Point& destination, time_t arrive_time);

        // Update position
        void update();

        bool isNearby(const Point& destination, double radius) const;

    private:
        Point position;
        int course;
        int speed;
};

#endif // TRACKBASE_HPP

