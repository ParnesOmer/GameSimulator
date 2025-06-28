//
// Created by omer on 6/28/2025.
//

#ifndef GAMESIMULATOR_TRUCKTRIP_HPP
#define GAMESIMULATOR_TRUCKTRIP_HPP
#include <string>

struct truckTrip {
    double sourceX;        // Source X coordinate
    double sourceY;        // Source Y coordinate
    double destinationX;   // Destination X coordinate
    double destinationY;   // Destination Y coordinate
    std::string outTime;   // Departure time
    std::string arrivalTime; // Arrival time
    int crates;            // Number of crates to be transported

    truckTrip(double srcX, double srcY, double destX, double destY, const std::string &out, const std::string &arr, int c)
        : sourceX(srcX), sourceY(srcY), destinationX(destX), destinationY(destY), outTime(out), arrivalTime(arr), crates(c) {}
};

#endif //GAMESIMULATOR_TRUCKTRIP_HPP
