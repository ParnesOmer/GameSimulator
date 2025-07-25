#include "StateTrooper.hpp"
#include "Model.hpp"
#include <iostream>
#include <limits>

// --- Constructor and Initialization ---
StateTrooper::StateTrooper(const std::string& name, const std::string& startWarehouse)
    : Vehicle(name), startWarehouseName(startWarehouse), currentTargetIdx(-1)
{
    setSpeed(90.0);
    setupWarehousesAndPosition(startWarehouse);
}

// --- Public Interface ---

void StateTrooper::update() {
    if (state == VehicleState::PARKED || state == VehicleState::OFFROAD) return;

    time_t tick_start = Model::getInstance().getTime();
    time_t tick_end = tick_start + SECONDS_PER_HOUR;
    time_t now = tick_start;

    while (now < tick_end && state != VehicleState::PARKED) {
        Point currentPos = getPosition();
        Point targetPos;

        if (!returningHome && currentTargetIdx != -1) {
            const auto& warehouses = Model::getInstance().getWarehouses();
            targetPos = warehouses.at(warehousesToVisit[currentTargetIdx]).getPosition();
        } else if (returningHome) {
            targetPos = startWarehousePos;
        } else {
            break;
        }

        double distanceToTarget = Point::distance(currentPos, targetPos);
        double hours_left = static_cast<double>(tick_end - now) / SECONDS_PER_HOUR;
        double moveDistance = getSpeed() * hours_left; // km/h * hours

        if (moveDistance >= distanceToTarget) {
            // Arrive at target within this tick
            double hoursToTarget = distanceToTarget / getSpeed();
            time_t secondsToTarget = static_cast<time_t>(hoursToTarget * SECONDS_PER_HOUR);
            trackbase.update(secondsToTarget);
            setPosition(targetPos);
            now += secondsToTarget;

            if (!returningHome && currentTargetIdx != -1) {
                state = VehicleState::STOPPED;
                warehousesToVisit.erase(warehousesToVisit.begin() + currentTargetIdx);
                moveToNextWarehouse();
            } else if (returningHome) {
                state = VehicleState::PARKED;
                returningHome = false;
                break;
            }
        } else {
            // Won't arrive this tick, move for the rest of the tick
            trackbase.update(tick_end - now);
            now = tick_end;
        }
    }
}

std::string StateTrooper::broadcastState() const {
    std::ostringstream oss;
    oss << "StateTrooper " << getName() << " at " << getPosition().toString()
        << ", State: " << getState();
    if (!returningHome && currentTargetIdx != -1) {
        oss << ", Heading to warehouse";
    } else if (returningHome) {
        oss << ", Returning to start";
    }
    return oss.str();
}

// --- Private Helpers ---

void StateTrooper::setupWarehousesAndPosition(const std::string& startWarehouse) {
    const auto& warehouses = Model::getInstance().getWarehouses();
    auto it = warehouses.find(startWarehouse);
    if (it == warehouses.end()) throw std::runtime_error("Invalid start warehouse for StateTrooper");
    startWarehousePos = it->second.getPosition();
    setPosition(startWarehousePos);

    warehousesToVisit.clear();
    for (const auto& pair : warehouses) {
        if (pair.first != startWarehouse) {
            warehousesToVisit.push_back(pair.first);
        }
    }
    moveToNextWarehouse();
}

void StateTrooper::findClosestWarehouse() {
    const auto& warehouses = Model::getInstance().getWarehouses();
    double minDist = std::numeric_limits<double>::max();
    int minIdx = -1;
    Point currentPos = getPosition();

    for (size_t i = 0; i < warehousesToVisit.size(); ++i) {
        const auto& wh = warehouses.at(warehousesToVisit[i]);
        double dist = Point::distance(currentPos, wh.getPosition());
        if (dist < minDist) {
            minDist = dist;
            minIdx = static_cast<int>(i);
        }
    }
    currentTargetIdx = minIdx;
}

void StateTrooper::moveToNextWarehouse() {
    if (warehousesToVisit.empty()) {
        returningHome = true;
        setCourse(startWarehousePos);
        state = VehicleState::MOVING;
        return;
    }
    findClosestWarehouse();
    if (currentTargetIdx != -1) {
        const auto& warehouses = Model::getInstance().getWarehouses();
        setCourse(warehouses.at(warehousesToVisit[currentTargetIdx]).getPosition());
        state = VehicleState::MOVING;
    }
}
