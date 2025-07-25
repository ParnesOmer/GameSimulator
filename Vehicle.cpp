#include "Vehicle.hpp"

// --- Constructors ---
Vehicle::Vehicle(std::string obj_name): 
    Sim_obj(obj_name), trackbase(Trackbase()), state(VehicleState::STOPPED), destinationType(DestinationType::None) {}

Vehicle::Vehicle(std::string obj_name, const Point& pos): 
    Sim_obj(obj_name), trackbase(Trackbase(pos)), state(VehicleState::STOPPED), destinationType(DestinationType::None) {}

// --- Position Methods ---
const Point& Vehicle::getPosition() const { 
    return trackbase.getPosition(); 
}

void Vehicle::setPosition(double x, double y) { 
    trackbase.setPosition(x, y); 
}

void Vehicle::setPosition(const Point& pos) { 
    trackbase.setPosition(pos.x, pos.y); 
}

// --- Speed Methods ---
double Vehicle::getSpeed() const { 
    return trackbase.getSpeed(); 
}

void Vehicle::setSpeed(double speed) { 
    trackbase.setSpeed(speed); 
}

// --- Course Methods ---
double Vehicle::getCourse() const { 
    return trackbase.getCourse(); 
}

void Vehicle::setCourse(double course) { 
    trackbase.setCourse(course); 
}

void Vehicle::setCourse(const Point& destination) { 
    trackbase.setCourse(destination); 
}

// --- Movement Methods ---
void Vehicle::updatePosition() { 
    trackbase.update(); 
}

void Vehicle::moveTo(const Point& destination) {
    setCourse(destination);
}

void Vehicle::stop() {
    setSpeed(NO_SPEED);
    setCourse(NO_COURSE);
}

// --- State Methods ---
std::string Vehicle::getState() const { 
    return vehicleStateToString(state); 
}

void Vehicle::setState(VehicleState newState){
    this->state = newState;
}

// --- Static Helpers ---
std::string Vehicle::vehicleStateToString(VehicleState state) {
    switch (state) {
        case VehicleState::STOPPED: return "Stopped";
        case VehicleState::PARKED: return "Parked";
        case VehicleState::OFFROAD: return "Off Road";
        case VehicleState::MOVING: return "Moving";
        default: return "Unknown";
    }
}