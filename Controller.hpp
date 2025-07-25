#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include "Model.hpp"

class Controller {
public:
    Controller();
    ~Controller();

    void parseArguments(int argc, char **argv);
    void run(int argc, char **argv);
    void executeCommand(const std::string &line);
    void parseVehicleCreation(const std::string& line);

private:
    // Sub-functions for executeCommand
    void handleBasicCommands(const std::string& cmd, std::istringstream& iss);
    void handleMapCommands(const std::string& cmd, std::istringstream& iss);
    void handleVehicleCommands(const std::string& name, const std::string& command, std::istringstream& iss);
    void handleVehicleCourse(const std::string& name, std::istringstream& iss);
    void handleVehiclePosition(const std::string& name, std::istringstream& iss);
    void handleVehicleDestination(const std::string& name, std::istringstream& iss);
    void handleVehicleAttack(const std::string& name, std::istringstream& iss);
    void handleVehicleStop(const std::string& name);
};

#endif // CONTROLLER_HPP
