//
// Created by l3gume on 24/07/17.
//
#include <sstream>
#include <iterator>
#include <algorithm>
#include "Controller.h"

// TODO: DUMB IDEA HEHE
// Have a constexpr dictionary of command help messages
// So whenever the --help option is passed to a command,
// the help message has been fetched at compile time and is ready
// to be displayed

Encryptor Controller::en;

Controller::Controller(): outputDir(""), verbose(false) {
    init();
}
// Nothing to write here
Controller::~Controller() {}

void Controller::init() {
    Command helpcom("help", printHelp);
    Command enccom("encrypt", encrypt);
    Command deccom("decrypt", decrypt);
    Command setkey("setkey", setKey);
    Command getkey("getkey", getKey);
    commands.push_back(helpcom);
    commands.push_back(enccom);
    commands.push_back(deccom);
    commands.push_back(setkey);
    commands.push_back(getkey);
}

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
    getline(std::cin, rawInput);
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
    std::string commandName;
    commandName = in.at(0);

    if (commandName == "printmessage") {
        printMessage();
    } else if (commandName == "clear") {
        std::cout << std::string( 100, '\n' ); // lol
    } else if (commandName == "exit") {
        std::cout << "See ya!" << '\n';
    } else {
        auto command = std::find_if(commands.begin(), commands.end(),
                                    [commandName](const Command& c){ return c.name == commandName; });
        if (command != commands.end()) {
            command->func(in); // pass the input to the command's method for args.
        } else {
            std::cout << "Command not found, type \"help\" for information." << '\n';
        }
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

void Controller::printHelp(std::vector<std::string>& args) {
    printHeader();
    std::cout << '\n'
              << "Command list:\n"
              << '\n'
              << "encrypt\n"
              << "decrypt\n"
              << "setmessage\n"
              << "printmessage\n"
              << "setfilepath\n"
              << "getfilepath\n"
              << "setoutputdir\n"
              << "getoutputdir\n"
              << "status\n"
              << "clear\n"
              << "more to come...\n"
              << '\n'
              << "For further information on a command, type:\n"
              << "[command] help (not implemented yet)\n";

}

void Controller::printMessage() {
    en.printMessage();
}

void Controller::encrypt(std::vector<std::string>& args) {
    bool verbose = false;
    bool readFromFile = false;
    bool rawText = false;
    std::string message;

    int nb_lines = 0;
    int nb_blocks = 0;

    for (auto s : args) {
        if (s != args.at(0)) {
            if (s == "-v" || s == "--verbose") {
                verbose = true;
            } else if (s == "-f" || s == "--file") {
                readFromFile = true;
            } else if (s == "-r" || s == "--rawtext") {
                rawText = true;
            } else {
                message = s;
            }
        }
    }

    // Not too sure about that one
    /*if (!s) {
        std::cout << "No message or file to encrypt!" << '\n';
        return;
    }*/

    if (readFromFile != !rawText) {
        std::cout << "Can't use raw text and read from file at the same time!" << '\n';
        return;
    }

    if (readFromFile) {
        message = readFile(message, nb_lines);
        if (!message.empty()) {
            en.parseString(message, false, nb_blocks);
            en.encrypt("", verbose);
        } else {
            std::cout << "No file to encrypt!" << '\n';
            return;
        }
    } else if (rawText) {
        if (!message.empty()) {
            en.encrypt(message, verbose);
        } else {
            std::cout << "No message to encrypt!" << '\n';
            return;
        }

    }
}

void Controller::decrypt(std::vector<std::string> &args) {
    bool verbose = false;
    bool readFromFile = false;
    bool rawText = false;
    bool useBuffer = false;
    std::string message;

    int nb_lines = 0;
    int nb_blocks = 0;

    for (auto s : args) {
        if (s != args.at(0)) {
            if (s == "-v" || s == "--verbose") {
                verbose = true;
            } else if (s == "-f" || s == "--file") {
                readFromFile = true;
            } else if (s == "-r" || s == "--rawtext") {
                rawText = true;
            } else if (s == "-b" || s == "--buffer") {
                useBuffer = true;
                readFromFile = false;
                rawText = false;
            } else {
                message = s;
            }
        }
    }

    // Not too sure about that one
    /*if (!s) {
        std::cout << "No message or file to encrypt!" << '\n';
        return;
    }*/

    if ((readFromFile != !rawText) && !useBuffer) {
        std::cout << "Can't use raw text and read from file at the same time!" << '\n';
        return;
    }

    if (readFromFile) {
        message = readFile(message, nb_lines);
        if (!message.empty()) {
            en.parseString(message, false, nb_blocks);
            en.decrypt("", verbose);
        } else {
            std::cout << "No file to encrypt!" << '\n';
            return;
        }
    } else if (rawText) {
        if (!message.empty()) {
            en.decrypt(message, verbose);
        } else {
            std::cout << "No message to encrypt!" << '\n';
            return;
        }
    } else if (useBuffer) {
        if (!en.getMessage().empty()) {
            en.decrypt("", verbose);
        } else {
            std::cout << "No message loaded!" << '\n';
            return;
        }
    }
}

void Controller::setKey(std::vector<std::string> &args) {
    std::string key;
    bool gen_rand = false;
    bool use_arg = false;

    for (auto s : args) {
        if (s != args.at(0)) {
            if (s == "-r" || s == "--random") {
                gen_rand = true;
                // Generate random key
            } else {
                key = s;
                use_arg = true;
            }
        }
    }

    if (gen_rand != !use_arg) {
        std::cout << "Can't use the input key and generate a random one, make up your mind!" << '\n';
        return;
    }

    if (use_arg) {
        en.setKey(key);
    } else if (gen_rand) {
        //...
    }
}

void Controller::getKey(std::vector<std::string> &args) {
    en.printStringFromDeque(true);
}
