#ifndef TRUCK_HPP
#define TRUCK_HPP

#include <vector>
#include "Vehicle.hpp"
#include "truckTrip.hpp"


class Truck : public Vehicle {
    public:
        Truck(std::string obj_name, const Point& pos, const std::vector<truckTrip>& trips): Vehicle(obj_name, pos), trips(trips), trip_index(0){
            setupTruck();
        };
        Truck(std::string obj_name, const Point& pos, int crates): Vehicle(obj_name, pos), trips(std::vector<truckTrip>()), trip_index(0), crates(crates) {};
        Truck(const Truck& other) = default;
        Truck& operator=(const Truck& other) = default;
        Truck(Truck&& other) = default;
        Truck& operator=(Truck&& other) = default;
        ~Truck() = default;

        void loadTrip();
        void moveToNextTrip();
        void unloadCrates();

        const truckTrip& getCurrentTrip() const;
        void setTrips(const std::vector<truckTrip>& trips);

        void update() override;
        std::string BroadcastState() const override;

    private:
        int trip_index;
        std::vector<truckTrip> trips;
        int crates;
        std::string destination_warehouse;
        DestinationType destination_type;

        void countCrates();
        void setupTruck();
};


#endif // TRUCK_HPP