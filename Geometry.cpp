#include "Geometry.hpp"
#include <cmath>
#include <ctgmath>

const double pi = 2. * atan2(1., 0.);

double to_radians(double theta_d) {
    return theta_d * pi / 180.0;
}

double to_degrees(double theta_r) {
    return theta_r * 180.0 / pi;
}


double Point::distance(const Point& p1, const Point& p2) {
    Cartesian_vector cv;
    cv.delta_x = p2.x - p1.x;
    cv.delta_y = p2.y - p1.y;
    
    Polar_vector pv(cv);
    return pv.r;  // r is the distance
}

// construct a Cartesian_vector from a Polar_vector
Cartesian_vector::Cartesian_vector(const Polar_vector &pv) {
    delta_x = pv.r * cos(pv.theta);
    delta_y = pv.r * sin(pv.theta);
}

Cartesian_vector::Cartesian_vector() {
    delta_x = 0.0;
    delta_y = 0.0;
}

void Cartesian_vector::operator=(const Polar_vector &pv) {
    delta_x = pv.r * cos(pv.theta);
    delta_y = pv.r * sin(pv.theta);
}

// construct a Polar_vector from a Cartesian_vector
Polar_vector::Polar_vector(const Cartesian_vector &cv) {
    r = sqrt((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));
    /* atan2 will return a negative angle for Quadrant III, IV, must translate to I, II */
    theta = atan2(cv.delta_y, cv.delta_x);
    if (theta < 0.)
        theta = 2. * pi + theta; // normalize theta positive
}

Polar_vector::Polar_vector() {
    r = 0.0;
    theta = 0.0;
}

void Polar_vector::operator=(const Cartesian_vector &cv) {
    r = sqrt((cv.delta_x * cv.delta_x) + (cv.delta_y * cv.delta_y));
    /* atan2 will return a negative angle for Quadrant III, IV, must translate to I, II */
    theta = atan2(cv.delta_y, cv.delta_x);
    if (theta < 0.)
        theta = 2. * pi + theta; // normalize theta positive
}

Point::Point(double x, double y) : x(x), y(y) {

}

Point::Point() {
    x = 0.0;
    y = 0.0;
}

void Point::print() const {
    cout << setprecision(2) << "(" << x << ", " << y << ")";
}

std::string Point::toString() const {
    std::ostringstream oss;
    oss << "(" << x << ", " << y << ")";
    return oss.str();
}

bool Point::operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
}