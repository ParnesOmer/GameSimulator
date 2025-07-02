#include "Vehicle.hpp"

// Constructor implementations
Vehicle::Vehicle(std::string obj_name): 
    Sim_obj(obj_name), trackbase(Trackbase()), state(VehicleState::STOPPED), destinationType(DestinationType::None) {
}

Vehicle::Vehicle(std::string obj_name, const Point& pos): 
    Sim_obj(obj_name), trackbase(Trackbase(pos)), state(VehicleState::STOPPED), destinationType(DestinationType::None) {
}

// Position methods
const Point& Vehicle::getPosition() const { 
    return trackbase.getPosition(); 
}

void Vehicle::setPosition(double x, double y) { 
    trackbase.setPosition(x, y); 
}

void Vehicle::setPosition(const Point& pos) { 
    trackbase.setPosition(pos.x, pos.y); 
}

// Speed methods
int Vehicle::getSpeed() const { 
    return trackbase.getSpeed(); 
}

void Vehicle::setSpeed(int speed) { 
    trackbase.setSpeed(speed); 
}

// Course methods
int Vehicle::getCourse() const { 
    return trackbase.getCourse(); 
}

void Vehicle::setCourse(int course) { 
    trackbase.setCourse(course); 
}

void Vehicle::setCourse(const Point& destination) { 
    trackbase.setCourse(destination); 
}

// Movement methods
void Vehicle::updatePosition() { 
    trackbase.update(); 
}

bool Vehicle::isNearby(const Point& destination, double radius) const { 
    return trackbase.isNearby(destination, radius); 
}

void Vehicle::moveTo(const Point& destination) {
    setCourse(destination);
}

void Vehicle::stop() {
    setSpeed(0);
    setCourse(-1);
}

// State methods
std::string Vehicle::getState() const { 
    return vehicleStateToString(state); 
}

// Static private function implementation
std::string Vehicle::vehicleStateToString(VehicleState state) {
    switch (state) {
        case VehicleState::STOPPED: return "Stopped";
        case VehicleState::PARKED: return "Parked";
        case VehicleState::OFFROAD: return "Off Road";
        case VehicleState::MOVING: return "Moving";
        default: return "Unknown";
    }
}