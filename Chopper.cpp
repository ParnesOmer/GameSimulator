#include "Chopper.hpp"
#include "Model.hpp"
#include <iostream>
#include <cmath>

// --- Constructor and Initialization ---
Chopper::Chopper(const std::string& name, const Point& startLocation)
    : Vehicle(name, startLocation)
{
    if (name.length() > 12) {
        throw std::invalid_argument("Chopper name too long (max 12 characters)");
    }
    setSpeed(170.0);
    state = VehicleState::STOPPED;
    attackRadius = 2.0;
}

// --- Public Interface ---

void Chopper::update() {
    // Basic movement logic - chopper can move around the map
    if (state == VehicleState::MOVING) {
        updatePosition();
    }
}

std::string Chopper::broadcastState() const {
    std::ostringstream oss;
    oss << "Chopper " << getName() << " at " << getPosition().toString()
        << ", State: " << getState()
        << ", Attack Radius: " << attackRadius << "km";
    return oss.str();
}

bool Chopper::attack(const std::string& truckName) {
    // Find the target truck
    auto& trucks = Model::getInstance().getTrucks();
    auto& targetTruck = trucks.at(truckName);
    Point truckLocation = targetTruck.getPosition();
    // Check if truck is in attack radius
    if (!isInRange(truckLocation)) {
        adjustAttackRadius(false);
        state = VehicleState::STOPPED;
        return false;
    }
    // Check if there's a state trooper nearby
    if (checkStateTrooperNearby(truckLocation)) {
        adjustAttackRadius(false);
        state = VehicleState::STOPPED;
        return false;
    }
    // Attack successful!
    // Set truck to OFFROAD and remove crates
    targetTruck.setCrates(0);
    targetTruck.setState(VehicleState::OFFROAD);
    // Set chopper to STOPPED at truck location
    setPosition(truckLocation);
    state = VehicleState::STOPPED;
    adjustAttackRadius(true);
    return true;
}

bool Chopper::isInRange(const Point& target) const {
    double distance = Point::distance(getPosition(), target);
    return distance <= attackRadius / 100.0;
}

// --- Private Helpers ---

bool Chopper::checkStateTrooperNearby(const Point& location) const {
    const auto& troopers = Model::getInstance().getTroopers();
    for (const auto& trooper : troopers) {
        double distance = Point::distance(location, trooper.second.getPosition());
        if (distance <= STATETROOPER_CHECK_RADIUS) {
            return true; // State trooper found nearby
        }
    }
    return false; // No state trooper nearby
}

void Chopper::adjustAttackRadius(bool attackSuccess) {
    if (attackSuccess) {
        // Increase radius by 1km, up to a max of 20km
        attackRadius += 1.0;
        if (attackRadius > 20.0) {
            attackRadius = 20.0;
        }
    } else {
        // Decrease radius by 1km, down to a min of 2km
        attackRadius -= 1.0;
        if (attackRadius < 2.0) {
            attackRadius = 2.0;
        }
    }
}