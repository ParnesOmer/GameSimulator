#ifndef SIM_OBJ_HPP
#define SIM_OBJ_HPP

#include <string>
#include <utility>
#include "Geometry.hpp"

/*
    Sim_obj is a base abstract class for all objects in the simulation.
    It is used to create new objects in the simulation - Warehouse, Truck, etc.
    As well as to update the state of the objects.
 */
class Sim_obj {
    public:
        Sim_obj(std::string obj_name): name(obj_name) {};

        Sim_obj(const Sim_obj& other) = default;
        Sim_obj& operator=(const Sim_obj& other) = default;
        Sim_obj(Sim_obj&& other) = default;
        Sim_obj& operator=(Sim_obj&& other) = default;
        virtual ~Sim_obj() = default;

        const std::string& getName() const { return name; }

        virtual std::string getState() const = 0;
        virtual std::string BroadcastState() const = 0;

        virtual const Point& getPosition() const = 0;
        virtual void update() = 0;

    protected:
        std::string name;
};

#endif // SIM_OBJ_HPP
