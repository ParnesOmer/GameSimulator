//
// Created by omer on 6/23/2025.
//

#ifndef GAMESIMULATOR_MODEL_HPP
#define GAMESIMULATOR_MODEL_HPP
#include <tuple>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Exceptions.hpp"

class Model {
public:
    /// Provide a global point of access to the instance
    static Model& getInstance();

    /// Delete copy constructor and assignment operator to prevent copies
    Model(const Model&) = delete;
    void operator=(const Model&) = delete;

    //getters and setters
    int getTime() const;
    void incrementTime();

    /// Parses the depot file and returns a vector of tuples containing warehouse data(name, x, y, amount).
    std::vector<std::tuple<std::string, double, double, int>> setDepotFile(const std::string &depotFile) const;
    /// Creates warehouses based on the provided vector of tuples.
    void createWarehouse(std::vector<std::tuple<std::string, double, double, int>> warehouses) const;

private:
    Model();
    ~Model();

    int time = 0;

    std::vector<std::string> parseLine(const std::string &line) const;
};

#endif //GAMESIMULATOR_MODEL_HPP
