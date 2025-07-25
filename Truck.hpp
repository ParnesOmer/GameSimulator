#ifndef TRUCK_HPP
#define TRUCK_HPP

#include <utility>
#include <vector>
#include "Vehicle.hpp"
#include "truckTrip.hpp"

class Truck : public Vehicle {
public:
    Truck(std::string obj_name, const Point& pos, const std::vector<truckTrip>& trips);
    Truck(std::string obj_name, const Point& pos, int crates);
    Truck(const Truck& other) = default;
    Truck& operator=(const Truck& other) = default;
    Truck(Truck&& other) = default;
    Truck& operator=(Truck&& other) = default;
    ~Truck() override = default;

    void update() override;
    std::string broadcastState() const override;
    void setCrates(int crates);
    const truckTrip& getCurrentTrip() const;

private:
    int trip_index;
    std::vector<truckTrip> trips;
    int crates;
    truckTrip current_trip;

    void loadTrip();
    void moveToNextTrip();
    void unloadCrates();
    void countCrates();
    void setupTruck();
};

#endif // TRUCK_HPP