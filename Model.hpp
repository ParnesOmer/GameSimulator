//
// Created by omer on 6/23/2025.
//

#ifndef GAMESIMULATOR_MODEL_HPP
#define GAMESIMULATOR_MODEL_HPP

#include <memory>
#include <tuple>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "Truck.hpp"
#include "Warehouse.hpp"
#include "truckTrip.hpp"
#include "Trackbase.hpp"
#include "Exceptions.hpp"
#include "StateTrooper.hpp"
#include "Chopper.hpp"
#include "View.hpp"

class Model {
public:
    // --- Time Management ---
    static Model& getInstance();
    Model(const Model&) = delete;
    void operator=(const Model&) = delete;

    time_t getTime() const;
    time_t peekNextTime() const;
    void incrementTime();
    std::string getGameTick() const;

    // --- Warehouse Management ---
    std::vector<std::tuple<std::string, double, double, int>> setDepotFile(const std::string &depotFile) const;
    void createWarehouse(std::vector<std::tuple<std::string, double, double, int>> warehousesList);
    const std::unordered_map<std::string, Warehouse>& getWarehouses() const { return warehouses; }
    void addCratesToWarehouse(const std::string& warehouseName, int crates);

    // --- Truck Management ---
    std::vector<truckTrip> setTruckFile(const std::string &truckFile);
    void createTruck(std::string& truck_name, std::vector<truckTrip>& truck_trips);
    void addTripToTruck(std::string source, std::string outTime, std::vector<std::string> columns, std::vector<truckTrip> &trucks);
    std::unordered_map<std::string, Truck>& getTrucks() { return trucks; }

    // --- StateTrooper Management ---
    void createTrooper(const std::string& trooperName, const std::string& startWarehouse);
    const std::unordered_map<std::string, StateTrooper>& getTroopers() const { return troopers; }

    // --- Chopper Management ---
    void createChopper(const std::string& chopperName, const Point& startLocation);
    const std::unordered_map<std::string, Chopper>& getChoppers() const { return choppers; }

    // --- Simulation Control ---
    void advanceAndUpdate();

    // --- View ---
    void fillMapObjects();
    std::shared_ptr<View> getView();
    void printAllVehicles() const;

    // --- Utility ---
    std::vector<std::string> parseLine(const std::string &line) const;

    // General vehicle lookup
    bool isVehicleName(const std::string& name) const;

    // Course
    void setVehicleCourse(const std::string& name, double course, double speed = -1);
    void setVehiclePosition(const std::string& name, const Point& pos, double speed = -1);

    // Destination
    void setVehicleDestination(const std::string& name, const std::string& dest); // for StateTrooper
    void setVehicleDestination(const std::string& name, const Point& dest);       // for Chopper

    // Attack
    bool vehicleAttack(const std::string& chopperName, const std::string& truckName);

    // Stop
    void stopVehicle(const std::string& name);


private:
    Model();
    ~Model();

    // --- Time ---
    time_t time = 0;

    // --- Data Structures ---
    std::unordered_map<std::string, Warehouse> warehouses;
    std::unordered_map<std::string, Truck> trucks;
    std::unordered_map<std::string, StateTrooper> troopers;
    std::unordered_map<std::string, Chopper> choppers;

    // --- View ---
    std::shared_ptr<View> view;
};

#endif // GAMESIMULATOR_MODEL_HPP
