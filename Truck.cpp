#include "Truck.hpp"

void Truck::setupTruck() {
    trackbase.setPosition(trips[0].source);
    countCrates();
    loadTrip();
    destination_type = DestinationType::WAREHOUSE;
    state = VehicleState::MOVING;
}

void Truck::countCrates() {
    crates = 0;
    for (const auto& trip : trips) {
        crates += trip.crates;
    }
}

void Truck::setTrips(const std::vector<truckTrip>& trips) {
    this->trips = trips;
    trip_index = 0;
}

void Truck::loadTrip() {
    if (trip_index < trips.size()) {
        trackbase.setupByTime(current_trip.outTime, current_trip.destination, current_trip.arrivalTime);
        destination_type = DestinationType::WAREHOUSE;
    }
}

void Truck::moveToNextTrip() {
    if (trip_index < trips.size() - 1) {
        trip_index++;
        loadTrip();
    }else{
        state = VehicleState::PARKED;
        destination_type = DestinationType::None;
    }
}

void Truck::unloadCrates() {
    crates -= trips[trip_index].crates;
}

const truckTrip& Truck::getCurrentTrip() const {
    return trips[trip_index];
}

void Truck::update() {
    time_t current_time = Model::getInstance().getTime();
    time_t next_time = Model::getInstance().peekNextTime();
    
    if (state == VehicleState::OFFROAD || state == VehicleState::PARKED) {
        return;
    }

    // Check if it's time to start the current trip
    if (state == VehicleState::STOPPED && current_trip.outTime <= next_time) {
        state = VehicleState::MOVING;
    }

    // If we're moving, process trips
    if (state == VehicleState::MOVING) {
        // Process trips while we have time and trips remaining
        while (trip_index < trips.size()) {
            // Check if we can start this trip
            if (current_trip.outTime <= next_time) {
                // Setup speed and course for this specific trip segment
                trackbase.setupByTime(current_trip.outTime, current_trip.destination, current_trip.arrivalTime);
                
                // Calculate how much of this trip we can complete
                time_t trip_duration = current_trip.arrivalTime - current_trip.outTime;
                time_t driving_time = 0;
                
                if (current_trip.arrivalTime <= next_time) {
                    // We can complete the entire trip
                    driving_time = trip_duration;
                    
                    // Update position for the full trip
                    trackbase.update(driving_time);
                    
                    // Move to next trip using class function
                    moveToNextTrip();
                    
                    // Check if we moved to next trip or parked
                    if (state == VehicleState::PARKED) {
                        break; // No more trips
                    }
                } else {
                    // We can only complete part of this trip
                    driving_time = next_time - current_trip.outTime;
                    
                    // Update position for the partial trip
                    trackbase.update(driving_time);
                    
                    break;
                }
            } else {
                // Can't start this trip yet, wait
                break;
            }
        }
    }
}

std::string Truck::BroadcastState() const {
    std::ostringstream oss;
    std::string state = getState();
    if (state == "MOVING") {
        oss << "Truck " << getName() << " at " << getPosition().toString();
        if (destination_type == DestinationType::WAREHOUSE) {
            oss << ", Heading to " << current_trip.destinationWarehouse;
        }
        
        oss << ", Crates: " << crates;
    }else{
        oss << "Truck " << getName() << " at " << getPosition().toString();
        oss << ", " << state << std::endl;
    }
    return oss.str();
}
