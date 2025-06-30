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
        truckTrip currentTrip = trips[trip_index];
        trackbase.setupByTime(currentTrip.outTime, currentTrip.destination, currentTrip.arrivalTime);
        destination_warehouse = currentTrip.destinationWarehouse;
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
    // need to pay attention that the truck is starting shipment at specified leave_time, on first
    // tick of movement, set it to moving, after last tick of moving set it to parked.
    // need to add set state.
    // need to think about update logic, whether its before tick or after.
    // need to check if it will reach its warehouse, calculate speeds and time of leave to check if continue to move or not.

}

std::string Truck::BroadcastState() const {
    std::ostringstream oss;
    std::string state = getState();
    if (state == "MOVING") {
        oss << "Truck " << getName() << " at " << getPosition().toString();
        if (destination_type == DestinationType::WAREHOUSE) {
            oss << ", Heading to " << destination_warehouse;
        }
        
        oss << ", Crates: " << crates;
    }else{
        oss << "Truck " << getName() << " at " << getPosition().toString();
        oss << ", " << state << std::endl;
    }
    return oss.str();
}
