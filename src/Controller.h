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
    std::string outputDir;
    bool verbose;

    void process();
    void parseCommand(std::vector<std::string> &in);
    std::vector<std::string> getInput();
    // command methods
    static void clearKey(std::vector<std::string>& args);
    static void setKey(const std::vector<std::string>& args);
    static void getKey(const std::vector<std::string> &args);
    static void setMessage(const std::vector<std::string>& args); // 0: raw text, 1: file
    static void getMessage(const std::vector<std::string>& args);
    static void clearMessage(std::vector<std::string>& args);
    static void setOutputDir(std::vector<std::string>& args);
    static void encrypt(const std::vector<std::string>& args);
    static void decrypt(const std::vector<std::string>& args);
    // Printing methods
    static void printHeader();
    static void printHelp();
    void printMessage();

    // TODO: implement a way to read multiple options after a single '-' ex: '-rv' instead of '-r -v' (read from file + verbose)

    using Args = const std::vector<std::string>&;
    typedef void (*commandFunction)(Args);

    struct Command {
        Command(std::string n, commandFunction f) : name(n), func(f) {};
        const std::string name;
        const commandFunction func;
    };

    std::vector<Command> commands;
};

#endif //AES_CONTROLLER_H
