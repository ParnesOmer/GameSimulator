#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Sim_obj.hpp"
#include "Trackbase.hpp"

enum class VehicleState {
    STOPPED,
    PARKED,
    OFFROAD,
    MOVING,
};

// Helper function to convert VehicleState to string
std::string vehicleStateToString(VehicleState state) {
    switch (state) {
        case VehicleState::STOPPED: return "Stopped";
        case VehicleState::PARKED: return "Parked";
        case VehicleState::OFFROAD: return "Off Road";
        case VehicleState::MOVING: return "Moving";
        default: return "Unknown";
    }
}

class Vehicle : public Sim_obj {
    public:
        Vehicle(std::string obj_name, VehicleState state=VehicleState::STOPPED): Sim_obj(obj_name), trackbase(Trackbase()), state(state) {};
        Vehicle(std::string obj_name, double x, double y, VehicleState state=VehicleState::STOPPED): Sim_obj(obj_name), trackbase(Trackbase(x, y)), state(state) {};

        virtual ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle& other) = default;
        Vehicle(Vehicle&& other) = default;
        Vehicle& operator=(Vehicle&& other) = default;

        const Point& getPosition() const override { return trackbase.getPosition(); }
        std::string getState() const override { return vehicleStateToString(state); }
        void setState(VehicleState state) { this->state = state; }

        std::string BroadcastState() const = 0;
        void update() override = 0;

    protected:
        Trackbase trackbase;
        VehicleState state;
};

#endif // VEHICLE_HPP
