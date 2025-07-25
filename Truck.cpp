#include "Truck.hpp"
#include "Model.hpp"

// --- Constructor and Initialization ---
Truck::Truck(std::string obj_name, const Point& pos, const std::vector<truckTrip>& trips)
    : Vehicle(obj_name, pos), trips(trips), trip_index(0) {
    setupTruck();
    current_trip = trips[0]; // Initialize current_trip with the first trip
}

Truck::Truck(std::string obj_name, const Point& pos, int crates)
    : Vehicle(std::move(obj_name), pos), trips(std::vector<truckTrip>()), crates(crates), trip_index(0) {}

void Truck::setupTruck() {
    trackbase.setPosition(trips[0].source);
    countCrates();
    destinationType = DestinationType::WAREHOUSE;
    state = VehicleState::STOPPED;
}

void Truck::countCrates() {
    crates = 0;
    for (const auto& trip : trips) {
        crates += trip.crates;
    }
}

// --- Public Interface ---

void Truck::update() {
    time_t tick_start = Model::getInstance().getTime();
    time_t tick_end = tick_start + SECONDS_PER_HOUR;
    time_t now = tick_start;

    while (now < tick_end && state != VehicleState::PARKED) {
        const truckTrip& trip = getCurrentTrip();

        if (state == VehicleState::MOVING) {
            time_t arrival = trip.arrivalTime;
            if (arrival > tick_end) {
                // Won't arrive this tick, move for the rest of the tick
                trackbase.update(tick_end - now);
                now = tick_end;
                break;
            }
            // Arrive within this tick
            time_t move_time = arrival - now;
            trackbase.update(move_time);
            now = arrival;
            trackbase.setPosition(trip.destination);
            unloadCrates();

            int crates_dropped = trips[trip_index].crates;
            Model::getInstance().addCratesToWarehouse(trip.destinationWarehouse, crates_dropped);

            moveToNextTrip();
            // After moveToNextTrip, state may be STOPPED or PARKED
            const truckTrip& next_trip = getCurrentTrip();
            if (state != VehicleState::PARKED && now < next_trip.outTime) {
                state = VehicleState::STOPPED;
            }
            continue;
        }

        if (state == VehicleState::STOPPED) {
            const truckTrip& next_trip = getCurrentTrip();
            if (now < next_trip.outTime) {
                if (next_trip.outTime >= tick_end) {
                    // Not enough time to start next trip in this tick, just wait
                    now = tick_end;
                    break;
                }
                // Wait until outTime, then start moving
                now = next_trip.outTime;
                state = VehicleState::MOVING;
                trackbase.setupByTime(next_trip.outTime, next_trip.destination, next_trip.arrivalTime);
                continue;
            }
            // It's time to depart
            state = VehicleState::MOVING;
            trackbase.setupByTime(next_trip.outTime, next_trip.destination, next_trip.arrivalTime);
            continue;
        }

        // If we reach here, state is PARKED or OFFROAD
        break;
    }
}

std::string Truck::broadcastState() const {
    std::ostringstream oss;
    std::string state = getState();
    oss << "Truck " << getName() << " at " << getPosition().toString();
    if (destinationType == DestinationType::WAREHOUSE) {
        oss << ", Heading to " << current_trip.destinationWarehouse;
    }
    oss << ", State: " << state;
    oss << ", Crates: " << crates;
    return oss.str();
}

void Truck::setCrates(int crates) {
    this->crates = crates;
}

const truckTrip& Truck::getCurrentTrip() const {
    return trips[trip_index];
}

// --- Private Helpers ---

void Truck::loadTrip() {
    if (trip_index < trips.size()) {
        current_trip = trips[trip_index];

        trackbase.setupByTime(current_trip.outTime, current_trip.destination, current_trip.arrivalTime);
        destinationType = DestinationType::WAREHOUSE;
    }
}

void Truck::moveToNextTrip() {
    if (trip_index < trips.size() - 1) {
        trip_index++;
        loadTrip();
    }else{
        state = VehicleState::PARKED;
        destinationType = DestinationType::None;
    }
}

void Truck::unloadCrates() {
    crates -= trips[trip_index].crates;
}