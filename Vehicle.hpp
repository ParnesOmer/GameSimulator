#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Sim_obj.hpp"
#include "Trackbase.hpp"

enum class DestinationType {
    WAREHOUSE,
    Point,
    None,
};
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
        Vehicle(std::string obj_name): 
            Sim_obj(obj_name), trackbase(Trackbase()), state(VehicleState::STOPPED), destinationType(DestinationType::None) {};

        Vehicle(std::string obj_name, const Point& pos): 
            Sim_obj(obj_name), trackbase(Trackbase(pos)), state(VehicleState::STOPPED), destinationType(DestinationType::None) {};

        virtual ~Vehicle() = default;
        Vehicle(const Vehicle& other) = default;
        Vehicle& operator=(const Vehicle& other) = default;
        Vehicle(Vehicle&& other) = default;
        Vehicle& operator=(Vehicle&& other) = default;

        const Point& getPosition() const override { return trackbase.getPosition(); }
        void setPosition(double x, double y)      { trackbase.setPosition(x, y); }
        void setPosition(const Point& pos)        { trackbase.setPosition(pos.x, pos.y); }
        
        int getSpeed() const        { return trackbase.getSpeed(); }
        void setSpeed(int speed)    { trackbase.setSpeed(speed); }
        
        int getCourse() const       { return trackbase.getCourse(); }
        void setCourse(int course)  { trackbase.setCourse(course); }
        void setCourse(const Point& destination) { trackbase.setCourse(destination); }
        
        void updatePosition() { trackbase.update(); }
        bool isNearby(const Point& destination, double radius) const { 
            return trackbase.isNearby(destination, radius); 
        }
        
        void moveTo(const Point& destination) {
            setCourse(destination);
        }
        
        void stop() {
            setSpeed(0);
            setCourse(-1);
        }
        
        std::string getState() const override { return vehicleStateToString(state); }
        void update() override = 0;
        std::string BroadcastState() const override = 0;
    protected:
        Trackbase trackbase;
        VehicleState state;
        DestinationType destinationType;
};

#endif // VEHICLE_HPP
