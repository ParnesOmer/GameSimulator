#include "Trackbase.hpp"
#include <cmath>

void Trackbase::setPosition(double x, double y) {
    position.x = x;
    position.y = y;
}

void Trackbase::setPosition(const Point& pos) {
    position = pos;
}

void Trackbase::setCourse(int course) {
    if (course < 0 || course > 359) {
        course = -1;
    }
    this->course = course;
}

void Trackbase::setSpeed(int speed) {
    if (speed < 0) {
        speed = 0;
    }
    this->speed = speed;
}

void Trackbase::setCourse(const Point& destination) {
    // Course can be set with 2 points 
    if (position == destination) {
        course = -1;
    } else {
        Cartesian_vector cartesian_v;
        cartesian_v.delta_x = destination.x - position.x;
        cartesian_v.delta_y = destination.y - position.y;

        Polar_vector polar_v = cartesian_v;
        course = polar_v.theta;
    }
}

void Trackbase::setupByTime(time_t leave_time, const Point& destination, time_t arrive_time) {
    // Calculate the time difference between the leave time and the arrival time
    time_t time_diff = arrive_time - leave_time;
    setCourse(destination);

    // Calculate the speed
    double distance = Point::distance(position, destination);
    speed = distance / time_diff;

    setSpeed(speed);
}

void Trackbase::update() {
    if (course == -1 || speed == 0) {
        return;
    }
    double course_radians = to_radians(course);
    Polar_vector polar_v;
    polar_v.theta = course_radians;
    polar_v.r = speed;

    Cartesian_vector cartesian_v(polar_v);
    position.x += cartesian_v.delta_x;
    position.y += cartesian_v.delta_y;
    
}

bool Trackbase::isNearby(const Point& destination, double radius) const {
    if (position == destination) {
        return true;
    }
    Cartesian_vector cartesian_v;
    cartesian_v.delta_x = destination.x - position.x;
    cartesian_v.delta_y = destination.y - position.y;

    Polar_vector polar_v = cartesian_v;
    return polar_v.r <= radius;
}