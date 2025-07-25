#include "Trackbase.hpp"
#include <cmath>

void Trackbase::setPosition(double x, double y) {
    position.x = x;
    position.y = y;
}

void Trackbase::setPosition(const Point& pos) {
    position = pos;
}

void Trackbase::setCourse(double course) {
    if (course < 0 || course > 359) {
        course = NO_COURSE;
    }
    this->course = course;
}

void Trackbase::setSpeed(double speed) {
    if (speed <= NO_SPEED) {
        speed = NO_SPEED;
    }
    this->speed = speed / 100.0;
}

void Trackbase::setCourse(const Point& destination) {
    // Course can be set with 2 points 
    if (position == destination) {
        course = NO_COURSE;
    } else {
        Cartesian_vector cartesian_v;
        cartesian_v.delta_x = destination.x - position.x;
        cartesian_v.delta_y = destination.y - position.y;

        Polar_vector polar_v = cartesian_v;
        course = to_degrees(polar_v.theta);
    }
}

void Trackbase::setupByTime(time_t leave_time, const Point& destination, time_t arrive_time) {
    // Calculate the time difference between the leave time and the arrival time
    time_t time_diff = arrive_time - leave_time;
    setCourse(destination);

    // Calculate the speed
    double distance = Point::distance(position, destination);
    speed = (distance / time_diff) * SECONDS_PER_HOUR;
    setSpeed(speed * 100.0);
}

void Trackbase::update(int time_factor_seconds) {
    if (course == NO_COURSE || speed == NO_SPEED) {
        return;
    }
    
    // Convert seconds to hours using constants
    double hours_factor = static_cast<double>(time_factor_seconds) / SECONDS_PER_HOUR;
    double distance_to_move = speed * hours_factor;  // km/h * hours = km
    
    double course_radians = to_radians(course);
    Polar_vector polar_v;
    polar_v.theta = course_radians;
    polar_v.r = distance_to_move;

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