#include "Warehouse.hpp"

const Point& Warehouse::getPosition() const {
    return position;
}


std::string Warehouse::getState() const {
    // TODO: Implement
    return std::to_string(total_crates);
}

void Warehouse::update() {
    // Empty - Model handles all interaction logic
    // Or just basic state validation if needed
}