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
#include "Truck.hpp"
#include "Warehouse.hpp"
#include "StateTrooper.hpp"
#include "truckTrip.hpp"
#include "Trackbase.hpp"
#include "Exceptions.hpp"

class Model {
public:
    /// Provide a global point of access to the instance
    static Model& getInstance();

    /// Delete copy constructor and assignment operator to prevent copies
    Model(const Model&) = delete;
    void operator=(const Model&) = delete;

    //getters and setters
    time_t getTime() const;
    time_t peekNextTime() const;
    void incrementTime();
    std::string getGameTick() const;

    void advanceAndUpdate(); // TODO: for testing

    /// Parses the depot file and returns a vector of tuples containing warehouse data(name, x, y, amount).
    std::vector<std::tuple<std::string, double, double, int>> setDepotFile(const std::string &depotFile) const;

    /// Parses the truck file and returns a vector of tuples containing truck data(sourceX, sourceY, destinationX, destinationY, outTime, arrivalTime, crates).
    std::vector<truckTrip> setTruckFile(const std::string &truckFile);
    /// Creates trucks based on the provided vector of tuples.
    void createTruck(std::string& truck_name, std::vector<truckTrip>& truck_trips);
    void createTrooper(const std::string& trooperName, const std::string& startWarehouse);
    /// Creates warehouses based on the provided vector of tuples.
    void createWarehouse(std::vector<std::tuple<std::string, double, double, int>> warehouses);


    const std::unordered_map<std::string, Warehouse>& getWarehouses() const;
    void printWarehouses() const;
    void printTrucks() const;
    void printTroopers() const;

    std::vector<std::string> parseLine(const std::string &line) const;
private:
    Model();
    ~Model();

    time_t time = 0;
    std::unordered_map<std::string, Warehouse> warehouses;
    std::unordered_map<std::string, Truck> trucks;
    std::unordered_map<std::string, StateTrooper> troopers;
    void addTripToTruck(std::string source, std::string outTime, std::vector<std::string> columns, vector<truckTrip> &trucks);
};

#endif //GAMESIMULATOR_MODEL_HPP
