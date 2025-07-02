#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Sim_obj.hpp"
#include "Trackbase.hpp"

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
        
        int getSpeed() const;
        void setSpeed(int speed);
        
        int getCourse() const;
        void setCourse(int course);
        void setCourse(const Point& destination);
        
        void updatePosition();
        bool isNearby(const Point& destination, double radius) const;
        
        void moveTo(const Point& destination);
        void stop();
        
        std::string getState() const override;
        void update() override = 0;
        std::string BroadcastState() const override = 0;
        
    protected:
        Trackbase trackbase;
        VehicleState state;
        DestinationType destinationType;
        
    private:
        // Static private function declaration
        static std::string vehicleStateToString(VehicleState state);
};

#endif // VEHICLE_HPP
