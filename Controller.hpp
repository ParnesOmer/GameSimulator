//
// Created by omer on 6/23/2025.
//

#ifndef GAMESIMULATOR_CONTRILLER_HPP
#define GAMESIMULATOR_CONTRILLER_HPP
#include <iostream>
#include <vector>
#include "Model.hpp"
#include "truckTrip.h"
#include "Exceptions.hpp"

/**
 * Controller
 * This class is responsible for controlling the Model and View according to interactions
 * with the user.
*/

class Controller {
public:	
	Controller();
	~Controller();

  void parseArguments(int argc, char* argv[]);
  void executeCommand(const std::string &line);

	/// creates View object, runs the program by accepting user commands, then destroys View object
	void run(int argc, char **argv);

private:
};

#endif //GAMESIMULATOR_CONTRILLER_HPP
