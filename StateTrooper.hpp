#ifndef STATE_TROOPER_HPP
#define STATE_TROOPER_HPP

#include "Vehicle.hpp"


class StateTrooper : public Vehicle {
    public:
        StateTrooper(std::string obj_name, const Point& pos): Vehicle(obj_name, pos) {};
        ~StateTrooper() = default;

        void update() override;
        std::string BroadcastState() const override;
        
    private:
        std::string destination;
        
};



#endif