#include "Warehouse.hpp"

const Point& Warehouse::getPosition() {
    return position;
}

const std::string& Warehouse::getCurrentState() const {
    return std::to_string(total_crates);
}

const std::string& Warehouse::getState() {
    // TODO: Implement
    return std::to_string(total_crates);
}

void Warehouse::update() {
    // Empty - Model handles all interaction logic
    // Or just basic state validation if needed
}