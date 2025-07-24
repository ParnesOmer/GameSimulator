#ifndef STATETROOPER_HPP
#define STATETROOPER_HPP

#include "Vehicle.hpp"
#include <vector>
#include <string>

class StateTrooper : public Vehicle {
public:
    StateTrooper(const std::string& name, const std::string& startWarehouse);

    void update() override;
    std::string broadcastState() const override;

private:
    std::string startWarehouseName;
    Point startWarehousePos;
    std::vector<std::string> warehousesToVisit;
    int currentTargetIdx;
    bool returningHome = false;

    void findClosestWarehouse();
    void moveToNextWarehouse();
    void setupWarehousesAndPosition(const std::string& startWarehouse);
};

#endif // STATETROOPER_HPP