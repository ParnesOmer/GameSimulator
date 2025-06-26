#include "Trackbase.hpp"


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

bool Trackbase::is_nearby(const Point& destination, double radius) const {
    if (position == destination) {
        return true;
    }
    Cartesian_vector cartesian_v;
    cartesian_v.delta_x = destination.x - position.x;
    cartesian_v.delta_y = destination.y - position.y;

    Polar_vector polar_v = cartesian_v;
    return polar_v.r <= radius;
}


















