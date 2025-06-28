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
#include <unordered_map>
#include "Exceptions.hpp"
#include "Warehouse.hpp"
#include "truckTrip.h"

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
    void createWarehouse(std::vector<std::tuple<std::string, double, double, int>> warehouses);

    /// Parses the truck file and returns a vector of tuples containing truck data(sourceX, sourceY, destinationX, destinationY, outTime, arrivalTime, crates).
    std::vector<truckTrip> setTruckFile(const std::string &truckFile);
    /// Creates trucks based on the provided vector of tuples.
    void createTruck(std::vector<truckTrip> trucks);

    void printWarehouses() const;
    void printTrucks() const;

private:
    Model();
    ~Model();

    int time = 0;
    std::unordered_map<std::string, Warehouse> warehouses;
    std::vector<std::string> parseLine(const std::string &line) const;
    void addTripToTruck(std::string source, std::string outTime, std::vector<std::string> columns, vector<truckTrip> &trucks);
};

#endif //GAMESIMULATOR_MODEL_HPP
