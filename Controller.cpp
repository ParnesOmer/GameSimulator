//
// Created by omer on 6/23/2025.
//
#include "Controller.hpp"

Controller::Controller() = default;


Controller::~Controller() = default;


void Controller::parseArguments(int argc, char **argv) {
    std::string depotFile;
    std::vector<std::string> truckFiles;
    std::string error_msg = "Invalid arguments format \n" \
                      "Should be at least one warehouse file and one truck file\n" \
                      "Usage: \"./simDepot -w depot.dat -t <truckfile1> [<truckfile2> ...]\"";
    if(argc < 5){
        throw InvalidArguments(error_msg);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-w" && i + 1 < argc) {
            depotFile = argv[++i];
        } else if (arg == "-t" && i + 1 < argc) {
            // The rest of the arguments are truck files
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                truckFiles.push_back(argv[++i]);
            }
        } else {
            std::string error_msg_2 = "Unknown or malformed argument: " + arg;
            throw InvalidArguments(error_msg_2);
        }
    }

    // Check if the input is valid
    if (depotFile.empty() || truckFiles.empty()) {
        throw InvalidArguments(error_msg);
    }

    Model &model = Model::getInstance();
    // Analyze the depot file
    std::vector<std::tuple<std::string, double, double, int>> warehouses = model.setDepotFile(depotFile);
    // Create the warehouses objects in the model
    model.createWarehouse(warehouses);

    // Analyze the truck files
    for (auto &truckFile : truckFiles) {
        std::vector<truckTrip> truck = model.setTruckFile(truckFile);
        // Create the trucks objects in the model
        model.createTruck(truckFile,truck);
    }

}

void Controller::parseVehicleCreation(const std::string& line) {
    std::vector<std::string> columns = Model::getInstance().parseLine(line);
    
    // Check if we have exactly 3 arguments: vehicle_name, vehicle_type, warehouse_name
    if (columns.size() != 3) {
        throw std::invalid_argument("Invalid vehicle creation format. Expected: vehicle_name,vehicle_type,warehouse_name");
    }
    
    std::string vehicleName = columns[0];
    std::string vehicleType = columns[1];
    std::string warehouseName = columns[2];
    
    // Check if it's a State_trooper
    if (vehicleType == "State_trooper") {
        try {
            Model::getInstance().createTrooper(vehicleName, warehouseName);
        } catch (const std::exception& e) {
            std::cerr << "Error creating StateTrooper: " << e.what() << std::endl;
        }
    } else {
        throw std::invalid_argument("Unknown vehicle type: " + vehicleType);
    }
}

void Controller::run(int argc, char **argv) {
    parseArguments(argc, argv);
    std::cout << "Game Simulator started." << std::endl;
    std::string line;
    Model &model = Model::getInstance();

    for(;;){
        std::cout << "Time " << model.getGameTick() << ": Enter command: ";
        std::getline(std::cin, line);

        if(line.empty()) continue;
        try {
            executeCommand(line);
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            continue;
        }
    }
}

void Controller::executeCommand(const std::string &line) {
    Model &model = Model::getInstance();
    if (line == "exit") {
        exit(0);
    } else if (line == "print") {
        model.printWarehouses();
        model.printTrucks();
        model.printTroopers();
    } else if (line == "go"){
        model.advanceAndUpdate();
    } else if (line.substr(0, 6) == "create") {
        std::string vehicleData = line.substr(7); // Remove "create " prefix
        parseVehicleCreation(vehicleData);
    } else {
        std::cout << "Invalid command" << std::endl;
    }
}