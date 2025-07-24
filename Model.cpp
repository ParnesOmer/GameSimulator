//
// Created by omer on 6/23/2025.
//

#include "Model.hpp"

Model &Model::getInstance() {
    static Model instance; // Guaranteed to be created only once and thread-safe in C++11+
    return instance;
}

Model::Model() = default;

Model::~Model()  = default;

void Model::incrementTime() { time += SECONDS_PER_HOUR; }
time_t Model::getTime() const { return time; }
time_t Model::peekNextTime() const { return time + SECONDS_PER_HOUR; }

// ticks are 1 hour
std::string Model::getGameTick() const {
    return std::to_string(static_cast<int>(time / SECONDS_PER_HOUR));
}


std::vector<std::string> Model::parseLine(const std::string &line) const {
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> columns;
    while (std::getline(ss, item, ',')) {
        columns.push_back(item);
    }

    return columns;
}

std::vector<std::tuple<std::string, double, double, int>> Model::setDepotFile(const std::string &depotFile) const {
    std::ifstream file(depotFile);
    if (!file.is_open()) {
        throw FileNotFoundException();
    }
    std::vector<std::tuple<std::string, double, double, int>> warehouses;
    std::string line;

    int line_num = 0;
    while (std::getline(file, line)) {
        line_num++;
        std::vector<std::string> columns = parseLine(line);
        // Check if the line has exactly 4 arguments
        if (columns.size() != 4) {
            throw FileException(depotFile, line_num);
        }

        // Parse the line into the expected format
        std::string name;
        double x, y;
        int amount;

        if (columns[0].empty() || columns[1].empty() || columns[2].empty() || columns[3].empty()) {
            throw FileException(depotFile, line_num);
        }
        name = columns[0];
        if (name.size() > 12) {
            throw FileException(depotFile, line_num);
        }

        // Ignore leading and trailing parentheses in the coordinates
        size_t open_paren = columns[1].find('(');
        size_t close_paren = columns[2].find(')');
        if (open_paren == std::string::npos || close_paren == std::string::npos) {
            throw FileException(depotFile, line_num);
        }
        if (open_paren != 1 || close_paren != columns[2].size() - 1) {
            throw FileException(depotFile, line_num);
        }

        try {
            std::string x_str = columns[1].substr(open_paren + 1);
            x = std::stod(x_str);
            std::string y_str = columns[2].substr(close_paren);
            y = std::stod(columns[2]);
            amount = std::stoi(columns[3]);
        }
        catch (const std::invalid_argument &e) {
            throw FileException(depotFile, line_num);
        }
        catch (const std::out_of_range &e) {
            throw FileException(depotFile, line_num);
        }

        // Add the parsed data to the warehouses vector
        warehouses.emplace_back(name, x, y, amount);
    }

    file.close();
    return warehouses;
}

void Model::createWarehouse(std::vector<std::tuple<std::string, double, double, int>> warehousesList) {
    for(const auto &warehouse : warehousesList) {
        std::string name;
        double x, y;
        int crates;
        std::tie(name, x, y, crates) = warehouse;

        warehouses.emplace(name, Warehouse(name, x, y, crates));
    }
}


std::vector<truckTrip> Model::setTruckFile(const string &truckFile) {
    std::ifstream file(truckFile);
    if (!file.is_open()) {
        throw FileNotFoundException();
    }
    // Vector to hold truck trips
    std::vector<truckTrip> trucks;

    std::string line;
    // Read the source and outTime for each next trip
    std::string source;
    std::string outTime;
    std::vector<std::string> columns;

    if(!std::getline(file, line)) {
        throw FileException(truckFile, 0);
    }
    columns = parseLine(line);
    if(columns.size() != 2) {
        std::cout << columns.size() << std::endl;
        std::cout << columns[0] << columns[1] << std::endl;
        throw FileException(truckFile, 1);
    }
    source = columns[0];
    outTime = columns[1];

    // Read the rest of the file line by line
    // and keep remembering the source and outTime for the next trip
    int line_num = 1;
    while (std::getline(file, line)) {
        columns = parseLine(line);
        try {
            addTripToTruck(source, outTime, columns, trucks);
        }
        catch (const FileException &e) {
            throw FileException(truckFile, line_num);
        }catch (const WarehouseNotFoundException &e) {
            throw WarehouseNotFoundException(truckFile, line_num);
        }
        source = columns[0];
        outTime = columns[3];

        line_num++;
    }

    file.close();
    return trucks;
}


void Model::addTripToTruck(std::string source, std::string outTime, std::vector<std::string> columns, vector<truckTrip> &trucks) {
    if( columns.size() != 4) {
        throw FileException("", 0);
    }
    
    if( source.empty() || outTime.empty() || columns[0].empty() || columns[1].empty() || columns[2].empty() || columns[3].empty()) {
        throw FileException("", 0);
    }
    std::string destination = std::move(columns[0]);

    // Check if the source and destination are valid warehouses
    auto it_to_source = warehouses.find(source);
    auto it_to_des = warehouses.find(destination);
    if(it_to_source == warehouses.end() || it_to_des == warehouses.end()) {
        throw WarehouseNotFoundException("", 0);
    }
    double sourceX, sourceY, destinationX, destinationY;
    std::string arrivalTime = std::move(columns[1]);
    int crates;

    try {
        sourceX = it_to_source->second.getPosition().x;
        sourceY = it_to_source->second.getPosition().y;
        destinationX = it_to_des->second.getPosition().x;
        destinationY = it_to_des->second.getPosition().y;
        std::string destinationWarehouse = std::move(destination);
        crates = std::stoi(columns[2]);
        if(crates < 0) {
            throw FileException("", 0);
        }

        // Create a truckTrip object and add it to the trucks vector
        trucks.emplace_back(sourceX, sourceY, destinationX, destinationY, outTime, arrivalTime, crates, destinationWarehouse);

    } catch (const std::invalid_argument &e) {
        throw FileException("", 0);
    } catch (const std::out_of_range &e) {
        throw FileException("", 0);
    }
}

void Model::createTruck(std::string& truck_name, std::vector<truckTrip>& truck_trips) {
    Point& pos = truck_trips[0].source;
    trucks.emplace(truck_name, Truck(truck_name, pos, std::move(truck_trips)));
}

void Model::printWarehouses() const {
    for (const auto &warehouse : warehouses) {
        std::cout << warehouse.second.broadcastState() << std::endl;
    }
}

void Model::printTrucks() const {
    for (const auto &truck : trucks) {
        std::cout << truck.second.broadcastState() << std::endl;
    }
}

void Model::printTroopers() const {
    for (const auto& trooper : troopers) {
        std::cout << trooper.second.broadcastState() << std::endl;
    }
}

void Model::advanceAndUpdate() {

    for (auto& truck : trucks) {
        Truck& crr_truck = truck.second;

        crr_truck.update();         // Update truck's state for the new time
        std::cout << crr_truck.broadcastState() << std::endl; // Print or broadcast the truck's state
    }
    
    for (auto& trooper : troopers) {
        StateTrooper& crr_trooper = trooper.second;
        crr_trooper.update();
        std::cout << crr_trooper.broadcastState() << std::endl;
    }
    
    incrementTime();
}

void Model::createTrooper(const std::string& trooperName, const std::string& startWarehouse) {
    // Check name length
    if (trooperName.length() > 12) {
        throw std::invalid_argument("Trooper name too long (max 12 characters)");
    }
    
    // Check if warehouse exists
    auto it = warehouses.find(startWarehouse);
    if (it == warehouses.end()) {
        throw std::invalid_argument("Invalid warehouse name: " + startWarehouse);
    }
    
    // Create and add trooper
    troopers.emplace(trooperName, StateTrooper(trooperName, startWarehouse));
}

const std::unordered_map<std::string, Warehouse>& Model::getWarehouses() const {
    return warehouses;
}


