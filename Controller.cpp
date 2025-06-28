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
    for (const auto &truckFile : truckFiles) {
        std::vector<truckTrip> trucks = model.setTruckFile(truckFile);
        // Create the trucks objects in the model
        model.createTruck(trucks);
    }

}

void Controller::run(int argc, char **argv) {
    parseArguments(argc, argv);
    std::cout << "Game Simulator started." << std::endl;
    std::string line;
    Model &model = Model::getInstance();

    for(;;){
        std::cout << "Time " << model.getTime() << ": Enter command: ";
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
    if(line == "exit") {
        std::cout << "Exiting the game simulator." << std::endl;
        exit(0);
    } else if (line == "print") {
        // Here you would call a method to print the current state of the model
        std::cout << "Printing current state..." << std::endl;
        model.printWarehouses();
        model.printTrucks();
    } else {
        // Handle other commands, e.g., loading data, processing input, etc.
        std::cout << "Executing command: " << line << std::endl;
    }
}
