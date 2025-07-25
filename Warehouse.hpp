#ifndef WAREHOUSE_HPP
#define WAREHOUSE_HPP

#include "Sim_obj.hpp"

class Warehouse : public Sim_obj {
    public:
        Warehouse(std::string obj_name, double x, double y, int crates): Sim_obj(obj_name), position(Point(x, y)), total_crates(crates) {};
        ~Warehouse() = default;

        void addCrates(int crates) { if (crates >= 0) total_crates += crates; }

        const Point& getPosition() const override;
        std::string getState() const override;

        std::string broadcastState() const override;
        void update() override;

    private:
        Point position;
        int total_crates;
};

#endif // WAREHOUSE_HPP

