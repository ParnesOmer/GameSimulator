#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Sim_obj.hpp"
#include "Trackbase.hpp"

// --- Enums ---
enum class DestinationType {
    WAREHOUSE,
    Point,
    None,
};

enum class VehicleState {
    STOPPED,
    PARKED,
    OFFROAD,
    MOVING,
};

// --- Class Declaration ---
class Vehicle : public Sim_obj {
public:
    Vehicle(std::string obj_name);
    Vehicle(std::string obj_name, const Point& pos);
    virtual ~Vehicle() = default;
    Vehicle(const Vehicle& other) = default;
    Vehicle& operator=(const Vehicle& other) = default;
    Vehicle(Vehicle&& other) = default;
    Vehicle& operator=(Vehicle&& other) = default;

    const Point& getPosition() const override;
    void setPosition(double x, double y);
    void setPosition(const Point& pos);

    double getSpeed() const;
    void setSpeed(double speed);

    double getCourse() const;
    void setCourse(double course);
    void setCourse(const Point& destination);

    void updatePosition();

    void moveTo(const Point& destination);
    void stop();
    std::string getState() const override;
    void setState(VehicleState newState);
    void update() override = 0;
    std::string broadcastState() const override = 0;

protected:
    Trackbase trackbase;
    VehicleState state;
    DestinationType destinationType;

private:
    static std::string vehicleStateToString(VehicleState state);
};

#endif // VEHICLE_HPP
