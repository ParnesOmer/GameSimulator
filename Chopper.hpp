#ifndef CHOPPER_HPP
#define CHOPPER_HPP

#include "Vehicle.hpp"
#include <string>

class Chopper : public Vehicle {
public:
    Chopper(const std::string& name, const Point& startLocation);

    void update() override;
    std::string broadcastState() const override;
    bool attack(const std::string& truckName);
    double getAttackRadius() const { return attackRadius; }
    bool isInRange(const Point& target) const;

private:
    double attackRadius = 2.0; 
    const double MAX_SPEED = 170.0;
    const double MAX_RANGE = 20.0;
    const double MIN_ATTACK_RADIUS = 2.0;
    const double STATETROOPER_CHECK_RADIUS = 10.0;

    bool checkStateTrooperNearby(const Point& location) const;
    void adjustAttackRadius(bool attackSuccess);
};

#endif // CHOPPER_HPP
