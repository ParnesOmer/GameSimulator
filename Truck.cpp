#include "Truck.hpp"
#include "Model.hpp"

void Truck::setupTruck() {
    trackbase.setPosition(trips[0].source);
    countCrates();
    destination_type = DestinationType::WAREHOUSE;
    state = VehicleState::STOPPED;
}

void Truck::countCrates() {
    crates = 0;
    for (const auto& trip : trips) {
        crates += trip.crates;
    }
}

void Truck::loadTrip() {
    if (trip_index < trips.size()) {
        current_trip = trips[trip_index];

        std::cout << "[DEBUG] setupByTime arguments: outTime=" << current_trip.outTime 
                  << ", destination=" << current_trip.destination.toString()
                  << ", arrivalTime=" << current_trip.arrivalTime << std::endl;

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
    time_t tick_start = Model::getInstance().getTime();
    time_t tick_end = tick_start + SECONDS_PER_HOUR;
    time_t now = tick_start;

    std::cout << "\n[DEBUG] Tick: " << TimeConverter::timeToString(tick_start)
              << " - " << TimeConverter::timeToString(tick_end)
              << ", State: " << getState() << std::endl;

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
            std::cout << "[DEBUG] Arrived at " << trip.destination.toString()
                      << " at " << TimeConverter::timeToString(now)
                      << ", Unloading crates: " << crates << " -> ";
            unloadCrates();
            std::cout << crates << std::endl;
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
                std::cout << "[DEBUG] Waiting at " << next_trip.source.toString()
                          << " until " << TimeConverter::timeToString(next_trip.outTime) << std::endl;
                now = next_trip.outTime;
                state = VehicleState::MOVING;
                trackbase.setupByTime(next_trip.outTime, next_trip.destination, next_trip.arrivalTime);
                continue;
            }
            // It's time to depart
            std::cout << "[DEBUG] Departing from " << next_trip.source.toString()
                      << " at " << TimeConverter::timeToString(now)
                      << " to " << next_trip.destination.toString() << std::endl;
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
    if (destination_type == DestinationType::WAREHOUSE) {
        oss << ", Heading to " << current_trip.destinationWarehouse;
    }
    oss << ", State: " << state;
    oss << ", Crates: " << crates;
    return oss.str();
}
