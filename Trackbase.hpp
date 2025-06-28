#ifndef TRACKBASE_HPP
#define TRACKBASE_HPP

#include "Geometry.hpp"

class Trackbase {
    public:
        Trackbase(): position(Point(0, 0)), course(-1), speed(0) {};
        Trackbase(double x, double y): position(Point(x, y)), course(-1), speed(0) {};
        Trackbase(const Point& position): position(position), course(-1), speed(0) {};
        ~Trackbase() = default;

        void setPosition(double x, double y);
        void setCourse(int course);
        void setCourse(const Point& destination);
        void setSpeed(int speed);

        // Getters
        const Point& getPosition() const { return position; }
        int getCourse() const            { return course; }
        int getSpeed() const             { return speed; }

        // Update position
        void update();

        bool is_nearby(const Point& destination, double radius) const;

    private:
        Point position;
        int course;
        int speed;
};

#endif // TRACKBASE_HPP

