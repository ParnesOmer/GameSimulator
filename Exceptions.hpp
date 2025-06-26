//
// Created by omer on 6/23/2025.
//

#ifndef GAMESIMULATOR_EXCEPTIONS_H
#define GAMESIMULATOR_EXCEPTIONS_H

#include <stdexcept>
#include <string>

/**
 * @brief Base exception class for custom exceptions in the application.
 */
class Exception : public std::exception {
protected:
    std::string message;
public:
    /**
     * @brief Constructs an Exception with a specific message.
     * @param msg The exception message.
     */
    explicit Exception(const std::string& msg)
            : message(msg) {}
    /**
     * @brief Returns the exception message.
     * @return C-string with the exception message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }
};

/// Exception for file-related input errors.
class InvalidArguments : public Exception {
public:
    explicit InvalidArguments(const std::string& msg) : Exception(msg) {
        if (msg.empty()) {
            message = "Invalid arguments format \n" \
                      "Should be at least one warehouse file and one truck file\n" \
                      "Usage: \"./simDepot -w depot.dat -t <truckfile1> [<truckfile2> ...]\"";
        } else {
            message = msg;
        }
    }
};

/// Exception for file-related input errors.
class FileException : public Exception {
public:
    explicit FileException(const std::string& filename, int line)
            : Exception("Invalid input in file " + filename + " at line "+ std::to_string(line) + ".\n") {}
};

/// Exception in case file not found.
class FileNotFoundException : public Exception {
public:
    explicit FileNotFoundException()
            : Exception("ERROR opening/reading the specified file.\n") {}
};

#endif //GAMESIMULATOR_EXCEPTIONS_H
