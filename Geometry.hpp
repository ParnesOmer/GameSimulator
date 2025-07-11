#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>
#include <iomanip>


using namespace std;

// angle units conversion functions
double to_radians(double theta_d);

double to_degrees(double theta_r);

// class declarations
typedef struct Point {
	double x;
	double y;
	Point(double x, double y);
	Point();
	void print() const;
  	std::string toString() const;
	static double distance(const Point& p1, const Point& p2);
    bool operator==(const Point &rhs) const;
} Point;


typedef struct Cartesian_vector {
    Cartesian_vector(const struct Polar_vector &pv);

    Cartesian_vector();

    void operator=(const struct Polar_vector &pv);

    double delta_x;
    double delta_y;
} Cartesian_vector;


typedef struct Polar_vector {

	Polar_vector(const Cartesian_vector & cv);
	Polar_vector();
	void operator=(const Cartesian_vector & cv);
	double r;
	double theta;
} Polar_vector;

#endif // GEOMETRY_HPP
