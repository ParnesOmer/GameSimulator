#include "Model.hpp"
#include <memory>
#include "View.hpp"

Model &Model::getInstance() {
    static Model instance; // Guaranteed to be created only once and thread-safe in C++11+
    return instance;
}

Model::Model() : view(std::make_shared<View>()) {
    // Guaranteed to be created only once and thread-safe in C++11+
}

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
            std::string y_str = columns[2].substr(0, close_paren);
            y = std::stod(y_str);
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

void Model::addCratesToWarehouse(const std::string& warehouseName, int crates) {
    auto it = warehouses.find(warehouseName);
    if (it != warehouses.end()) {
        it->second.addCrates(crates);
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


void Model::advanceAndUpdate() {

    for (auto& truck : trucks) {
        Truck& crr_truck = truck.second;

        crr_truck.update();
    }
    
    for (auto& trooper : troopers) {
        StateTrooper& crr_trooper = trooper.second;
        crr_trooper.update();
    }
    
    for (auto& chopper : choppers) {
        Chopper& crr_chopper = chopper.second;
        crr_chopper.update();
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

void Model::createChopper(const std::string& chopperName, const Point& startLocation) {
    if (chopperName.length() > 12) {
        throw std::invalid_argument("Chopper name too long (max 12 characters)");
    }
    
    choppers.emplace(chopperName, Chopper(chopperName, startLocation));
}

void Model::printAllVehicles() const {
    std::cout << "=== ALL VEHICLES INFORMATION ===" << std::endl;
    
    // Print warehouses
    std::cout << "\n--- WAREHOUSES ---" << std::endl;
    for (const auto& warehousePair : warehouses) {
        const Warehouse& warehouse = warehousePair.second;
        std::cout << warehouse.broadcastState() << std::endl;
    }
    
    // Print trucks
    std::cout << "\n--- TRUCKS ---" << std::endl;
    for (const auto& truckPair : trucks) {
        const Truck& truck = truckPair.second;
        std::cout << truck.broadcastState() << std::endl;
    }
    
    // Print StateTroopers
    std::cout << "\n--- STATE TROOPERS ---" << std::endl;
    for (const auto& trooperPair : troopers) {
        const StateTrooper& trooper = trooperPair.second;
        std::cout << trooper.broadcastState() << std::endl;
    }
    
    // Print Choppers
    std::cout << "\n--- CHOPPERS ---" << std::endl;
    for (const auto& chopperPair : choppers) {
        const Chopper& chopper = chopperPair.second;
        std::cout << chopper.broadcastState() << std::endl;
    }
    
    std::cout << "================================" << std::endl;
}

bool Model::isVehicleName(const std::string& name) const {
    return trucks.count(name) || troopers.count(name) || choppers.count(name);
}

void Model::setVehicleCourse(const std::string& name, double course, double speed) {
    if (choppers.count(name)) {
        auto& chopper = choppers.at(name);
        chopper.setCourse(course);
        if (speed >= 0) chopper.setSpeed(speed);
        if (chopper.getSpeed() > 0) chopper.setState(VehicleState::MOVING);
    } else if (troopers.count(name)) {
        troopers.at(name).setCourse(course);
    } else if (trucks.count(name)) {
        trucks.at(name).setCourse(course);
    } else {
        throw std::invalid_argument("No such vehicle: " + name);
    }
}

void Model::setVehiclePosition(const std::string& name, const Point& pos, double speed) {
    if (choppers.count(name)) {
        auto& chopper = choppers.at(name);
        chopper.setPosition(pos);
        if (speed >= 0) chopper.setSpeed(speed);
        if (chopper.getSpeed() > 0) chopper.setState(VehicleState::MOVING);
    } else if (troopers.count(name)) {
        troopers.at(name).setPosition(pos);
    } else if (trucks.count(name)) {
        trucks.at(name).setPosition(pos);
    } else {
        throw std::invalid_argument("No such vehicle: " + name);
    }
}

void Model::setVehicleDestination(const std::string& name, const std::string& warehouseName) {
    if (troopers.count(name)) {
        const auto& warehouses = getWarehouses();
        auto it = warehouses.find(warehouseName);
        if (it == warehouses.end()) throw std::invalid_argument("Invalid warehouse: " + warehouseName);
        troopers.at(name).setCourse(it->second.getPosition());
    } else {
        throw std::invalid_argument("Only StateTrooper can use warehouse destination");
    }
}

void Model::setVehicleDestination(const std::string& name, const Point& dest) {
    if (choppers.count(name)) {
        choppers.at(name).setCourse(dest);
    } else {
        throw std::invalid_argument("Only Chopper can use point destination");
    }
}

bool Model::vehicleAttack(const std::string& chopperName, const std::string& truckName) {
    if (choppers.count(chopperName)) {
        return choppers.at(chopperName).attack(truckName);
    }
    throw std::invalid_argument("No such chopper: " + chopperName);
}

void Model::stopVehicle(const std::string& name) {
    Vehicle* v = nullptr;
    if (choppers.count(name)) {
        v = &choppers.at(name);
    } else if (troopers.count(name)) {
        v = &troopers.at(name);
    } else if (trucks.count(name)) {
        v = &trucks.at(name);
    } else {
        throw std::invalid_argument("No such vehicle: " + name);
    }
    v->setSpeed(0);
    v->setState(VehicleState::STOPPED);
}

std::shared_ptr<View> Model::getView() {
    return view;
}

void Model::fillMapObjects() {
    // Clear the map first
    view->clear();
    
    // Add all trucks to the map
    for (const auto& truckPair : trucks) {
        const Truck& truck = truckPair.second;
        Point location = truck.getPosition();
        view->insert_obj(location.x, location.y, truckPair.first.substr(0, 2));
    }
    
    // Add all StateTroopers to the map
    for (const auto& trooperPair : troopers) {
        const StateTrooper& trooper = trooperPair.second;
        Point location = trooper.getPosition();
        view->insert_obj(location.x, location.y, trooperPair.first.substr(0, 2));
    }
    
    // Add all Choppers to the map
    for (const auto& chopperPair : choppers) {
        const Chopper& chopper = chopperPair.second;
        Point location = chopper.getPosition();
        view->insert_obj(location.x, location.y, chopperPair.first.substr(0, 2));
    }
    
    // Add all warehouses to the map
    for (const auto& warehousePair : warehouses) {
        const Warehouse& warehouse = warehousePair.second;
        Point location = warehouse.getPosition();
        view->insert_obj(location.x, location.y, warehousePair.first.substr(0, 2));
    }
}



