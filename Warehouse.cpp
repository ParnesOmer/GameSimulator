#include "Warehouse.hpp"
#include <sstream>
#include <iomanip>

const Point& Warehouse::getPosition() const {
    return position;
}

std::string Warehouse::getState() const {
    return std::to_string(total_crates);
}

std::string Warehouse::BroadcastState() const {
    return "Warehouse " + getName() + " at position " + position.toString() + ", Inventory: " + std::to_string(total_crates);
}

void Warehouse::update() {
    // Empty - Model handles all interaction logic
    // Or just basic state validation if needed
}