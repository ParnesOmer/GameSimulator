//
// Created by omer on 6/23/2025.
//

#include "Model.hpp"

Model &Model::getInstance() {
    static Model instance; // Guaranteed to be created only once and thread-safe in C++11+
    return instance;
}

Model::Model() {

}

Model::~Model() {

}

int Model::getTime() const { return time; }

void Model::incrementTime() { time++; }


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
        if (name.size() > 16) {
            throw FileException(depotFile, line_num);
        }

        // Ignore leading and trailing parentheses in the coordinates
        size_t open_paren = columns[1].find('(');
        size_t close_paren = columns[2].find(')');
        if (open_paren == std::string::npos || open_paren != 1) {
            throw FileException(depotFile, line_num);
        }
        if (close_paren == std::string::npos || close_paren != columns[2].size() - 1) {
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

void Model::createWarehouse(std::vector<std::tuple<std::string, double, double, int>> warehouses) const {
    for(const auto &warehouse : warehouses) {
        std::string name;
        double x, y;
        int amount;
        std::tie(name, x, y, amount) = warehouse;

        // Here you would typically create a Warehouse object or similar
        // For now, we just print the warehouse data
        std::cout << "Creating warehouse: " << name << " at (" << x << ", " << y << ") with amount: " << amount << std::endl;
    }
}

