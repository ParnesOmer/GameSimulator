#ifndef TRUCK_HPP
#define TRUCK_HPP

#include <utility>
#include <vector>
#include "Vehicle.hpp"
#include "truckTrip.hpp"

class Truck : public Vehicle {
    public:
        Truck(std::string obj_name, const Point& pos, const std::vector<truckTrip>& trips): Vehicle(obj_name, pos), trips(trips){
            setupTruck();
        };
        Truck(std::string obj_name, const Point& pos, int crates): Vehicle(std::move(obj_name), pos), trips(std::vector<truckTrip>()), crates(crates) {};
        Truck(const Truck& other) = default;
        Truck& operator=(const Truck& other) = default;
        Truck(Truck&& other) = default;
        Truck& operator=(Truck&& other) = default;
        ~Truck() override = default;

        void loadTrip();
        void moveToNextTrip();
        void unloadCrates();

        const truckTrip& getCurrentTrip() const;

        void update() override;
        std::string broadcastState() const override;

    private:
        int trip_index;
        std::vector<truckTrip> trips;
        int crates;
        truckTrip current_trip;


        void countCrates();
        void setupTruck();
};


#endif // TRUCK_HPP