#ifndef TRUCKTRIP_HPP
#define TRUCKTRIP_HPP

#include <string>
#include <utility>
#include "Geometry.hpp"
#include "Time.hpp"

struct truckTrip {
    Point source;            // Source point
    Point destination;       // Destination point
    time_t outTime{};     // Departure time
    time_t arrivalTime{}; // Arrival time
    std::string destinationWarehouse; // Destination warehouse
    int crates{};              // Number of crates to be transported

    truckTrip() = default;
    truckTrip(double srcX, double srcY, double destX, double destY, const std::string &out, const std::string &arr, int c, std::string destWarehouse)
        : source(Point(srcX, srcY)), destination(Point(destX, destY)), outTime(TimeConverter::stringToTime(out)), arrivalTime(TimeConverter::stringToTime(arr)), crates(c), destinationWarehouse(std::move(destWarehouse)) {}
};

#endif // TRUCKTRIP_HPP
