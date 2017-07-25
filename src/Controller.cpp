//
// Created by l3gume on 24/07/17.
//
#include <sstream>
#include <iterator>
#include "Controller.h"
#include "Encryptor.h"

Controller::Controller() : outputDir(""), verbose(false) {
}
// Nothing to write here
Controller::~Controller() {}

void Controller::start() {
    printHeader();
    process();
}

void Controller::process() {
    bool exit_loop = false;
    std::vector<std::string> input;
    while (!exit_loop) {
        input.clear(); // clear vector
        input = getInput();
        if (input.size() > 0) {
            parseCommand(input);
        }

        if (input.at(0) == "exit") {
            exit_loop = true;
        }
    }

}

std::vector<std::string> Controller::getInput() {
    std::string rawInput;
    std::cout << "> ";
    std::cin >> rawInput;
    std::cin.clear();

    // Make into vector
    std::stringstream ss(rawInput);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> output(begin, end);

    return output;
}

void Controller::parseCommand(std::vector<std::string> &in) {
    // temporary, might implement a "dictionnary" of command structs containing their own help message
    // and their own execution method/conditions
    // could also be a class
    if (in.at(0) == "printmessage") {
        printMessage();
    } else if (in.at(0) == "clear") {
        std::cout << std::string( 100, '\n' ); // lol
        printHeader();
    }
}

void Controller::printHeader() {
    std::cout << "AES128 encryption tool" << '\n'
              << "Version: 0.0.1" << '\n'
              << "by: Justin Tremblay" << '\n'
              << "2017"<< '\n'
              << '\n'
              << "For information on how to use this tool, enter \"help\"" << '\n'
              << '\n';
}

void Controller::printHelp() {

}

void Controller::printMessage() {
    en->printMessage();
}
