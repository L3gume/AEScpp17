//
// Created by l3gume on 24/07/17.
//
#ifndef AES_CONTROLLER_H
#define AES_CONTROLLER_H

#include <iostream>
#include <vector>
#include "Encryptor.h"
#include "AESUtils.h"

class Controller {
public:
    Controller();
    ~Controller();

    void start();
private:
    void init();
    static Encryptor en;
    std::string outputDir;
    bool verbose;

    void process();
    void parseCommand(std::vector<std::string> &in);
    std::vector<std::string> getInput();
    // command methods
    static void clearKey(std::vector<std::string>& args);
    static void setKey(std::vector<std::string>& args);
    static void setMessage(std::vector<std::string>& args); // 0: raw text, 1: file
    static void getMessage(std::vector<std::string>& args);
    static void clearMessage(std::vector<std::string>& args);
    static void setOutputDir(std::vector<std::string>& args);
    static void encrypt(std::vector<std::string>& args);
    static void decrypt(std::vector<std::string>& args);
    // Printing methods
    static void printHeader();
    static void printHelp(std::vector<std::string>& args);
    void printMessage();

    // TODO: implement a way to read multiple options after a single '-' ex: '-rv' instead of '-r -v' (read from file + verbose)

    typedef void (*commandFunction)(std::vector<std::string>&);

    struct Command {
        Command() : name(""), func(nullptr){};
        Command(std::string n, commandFunction f) : name(n), func(f) {};
        std::string name;
        commandFunction func;
    };

    std::vector<Command> commands;
};

#endif //AES_CONTROLLER_H
