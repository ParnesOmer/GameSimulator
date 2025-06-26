#include "Sim_obj.hpp"

#ifndef WAREHOUSE_HPP
#define WAREHOUSE_HPP

class Warehouse : public Sim_obj {
    public:
        Warehouse(std::string obj_name, double x, double y, int crates): Sim_obj(obj_name), position(Point(x, y)), total_crates(crates) {};
        ~Warehouse() = default;

        void addCrates(int crates) { total_crates += crates; }

        const Point& getPosition() override;
        const std::string& getCurrentState() const override;
        const std::string& getState() override;
        void update() override;

    private:
        Point position;
        int total_crates;
};

#endif // WAREHOUSE_HPP

