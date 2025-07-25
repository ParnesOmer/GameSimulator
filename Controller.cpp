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
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() != 3) {
        throw std::invalid_argument("Invalid vehicle creation format. Expected: name State_trooper warehousename OR name Chopper (x,y)");
    }

    std::string vehicleName = tokens[0];
    std::string vehicleType = tokens[1];
    std::string locationData = tokens[2];

    if (vehicleType == "State_trooper") {
        try {
            Model::getInstance().createTrooper(vehicleName, locationData);
        } catch (const std::exception& e) {
            std::cerr << "Error creating StateTrooper: " << e.what() << std::endl;
        }
    } else if (vehicleType == "Chopper") {
        try {
            // Parse coordinates from locationData (format: (x,y))
            size_t openParen = locationData.find('(');
            size_t closeParen = locationData.find(')');
            size_t comma = locationData.find(',');

            if (openParen == std::string::npos || closeParen == std::string::npos || comma == std::string::npos) {
                throw std::invalid_argument("Invalid coordinate format. Expected: (x,y)");
            }

            std::string xStr = locationData.substr(openParen + 1, comma - openParen - 1);
            std::string yStr = locationData.substr(comma + 1, closeParen - comma - 1);

            double x = std::stod(xStr);
            double y = std::stod(yStr);

            Point startLocation(x, y);
            Model::getInstance().createChopper(vehicleName, startLocation);
        } catch (const std::exception& e) {
            std::cerr << "Error creating Chopper: " << e.what() << std::endl;
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
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    // Handle basic commands (exit, print, go, create)
    if (cmd == "exit" || cmd == "print" || cmd == "go" || cmd == "create") {
        handleBasicCommands(cmd, iss);
        return;
    }

    // Handle map commands (default, size, zoom, pan, show)
    if (cmd == "default" || cmd == "size" || cmd == "zoom" || cmd == "pan" || cmd == "show") {
        handleMapCommands(cmd, iss);
        return;
    }

    // Handle vehicle commands
    std::string name = cmd;
    std::string command;
    iss >> command;

    if (name.empty() || command.empty()) {
        std::cerr << "Error: Missing vehicle name or command." << std::endl;
        return;
    }

    if (!model.isVehicleName(name)) {
        std::cerr << "Error: Invalid vehicle name: " << name << std::endl;
        return;
    }

    handleVehicleCommands(name, command, iss);
}

void Controller::handleBasicCommands(const std::string& cmd, std::istringstream& iss) {
    Model &model = Model::getInstance();
    
    if (cmd == "exit") {
        exit(0);
    } else if (cmd == "print") {
        model.printAllVehicles();
    } else if (cmd == "go") {
        model.advanceAndUpdate();
    } else if (cmd == "create") {
        std::string restOfLine;
        std::getline(iss, restOfLine);
        if (restOfLine.empty()) {
            std::cerr << "Error: Missing arguments for create command." << std::endl;
            return;
        }
        parseVehicleCreation(restOfLine.substr(1)); // skip leading space
    }
}

void Controller::handleMapCommands(const std::string& cmd, std::istringstream& iss) {
    if (cmd == "default") {
        Model::getInstance().getView()->default_size();
    } else if (cmd == "size") {
        int newSize;
        if (!(iss >> newSize)) {
            std::cerr << "Error: Missing or invalid size argument." << std::endl;
            return;
        }
        Model::getInstance().getView()->new_size(newSize);
    } else if (cmd == "zoom") {
        double newZoom;
        if (!(iss >> newZoom)) {
            std::cerr << "Error: Missing or invalid zoom argument." << std::endl;
            return;
        }
        Model::getInstance().getView()->new_zoom(newZoom);
    } else if (cmd == "pan") {
        double newY, newX;
        if (!(iss >> newY >> newX)) {
            std::cerr << "Error: Missing or invalid pan arguments." << std::endl;
            return;
        }
        Model::getInstance().getView()->pan(newX, newY);
    } else if (cmd == "show") {
        Model::getInstance().fillMapObjects();
        Model::getInstance().getView()->show();
    }
}

void Controller::handleVehicleCommands(const std::string& name, const std::string& command, std::istringstream& iss) {
    try {
        if (command == "course") {
            handleVehicleCourse(name, iss);
        } else if (command == "position") {
            handleVehiclePosition(name, iss);
        } else if (command == "destination") {
            handleVehicleDestination(name, iss);
        } else if (command == "attack") {
            handleVehicleAttack(name, iss);
        } else if (command == "stop") {
            handleVehicleStop(name);
        } else {
            std::cerr << "Error: Unknown vehicle command: " << command << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Controller::handleVehicleCourse(const std::string& name, std::istringstream& iss) {
    Model &model = Model::getInstance();
    
    double course;
    if (!(iss >> course)) {
        std::cerr << "Error: Missing or invalid course argument." << std::endl;
        return;
    }
    
    double speed = -1;
    if (model.getChoppers().count(name) > 0) {
        if (!(iss >> speed)) {
            std::cerr << "Error: Missing speed argument for chopper." << std::endl;
            return;
        }
    }
    
    model.setVehicleCourse(name, course, speed);
}

void Controller::handleVehiclePosition(const std::string& name, std::istringstream& iss) {
    Model &model = Model::getInstance();
    
    char c;
    double x, y, speed = -1;
    if (!(iss >> c) || c != '(' || !(iss >> x) || !(iss >> c) || c != ',' || !(iss >> y) || !(iss >> c) || c != ')') {
        std::cerr << "Error: Invalid or missing position format. Use (x,y)" << std::endl;
        return;
    }
    
    if (model.getChoppers().count(name) > 0) {
        if (!(iss >> speed)) {
            std::cerr << "Error: Missing speed argument for chopper." << std::endl;
            return;
        }
    }
    
    model.setVehiclePosition(name, Point(x, y), speed);
}

void Controller::handleVehicleDestination(const std::string& name, std::istringstream& iss) {
    Model &model = Model::getInstance();
    
    if (model.getChoppers().count(name) > 0) {
        char c;
        double x, y;
        if (!(iss >> c) || c != '(' || !(iss >> x) || !(iss >> c) || c != ',' || !(iss >> y) || !(iss >> c) || c != ')') {
            std::cerr << "Error: Invalid or missing destination format for chopper. Use (x,y)" << std::endl;
            return;
        }
        model.setVehicleDestination(name, Point(x, y));
    } else if (model.getTroopers().count(name) > 0) {
        std::string warehouse;
        if (!(iss >> warehouse)) {
            std::cerr << "Error: Missing warehouse name for state trooper destination." << std::endl;
            return;
        }
        model.setVehicleDestination(name, warehouse);
    } else {
        std::cerr << "Error: Only choppers and state troopers can use the destination command." << std::endl;
    }
}

void Controller::handleVehicleAttack(const std::string& name, std::istringstream& iss) {
    Model &model = Model::getInstance();
    
    if (model.getChoppers().count(name) == 0) {
        std::cerr << "Error: Only choppers can attack." << std::endl;
        return;
    }
    
    std::string targetTruck;
    if (!(iss >> targetTruck)) {
        std::cerr << "Error: Missing target truck name for attack." << std::endl;
        return;
    }
    
    if (!model.getTrucks().count(targetTruck)) {
        std::cerr << "Error: Target truck does not exist: " << targetTruck << std::endl;
        return;
    }
    
    bool success = model.vehicleAttack(name, targetTruck);
    if (success) {
        std::cout << "Attack successful!" << std::endl;
    } else {
        std::cout << "Attack failed." << std::endl;
    }
}

void Controller::handleVehicleStop(const std::string& name) {
    Model &model = Model::getInstance();
    model.stopVehicle(name);
}