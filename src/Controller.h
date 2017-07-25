//
// Created by l3gume on 24/07/17.
//
#ifndef AES_CONTROLLER_H
#define AES_CONTROLLER_H

#include <iostream>
#include <vector>
#include "Encryptor.h"

class Controller {
public:
    Controller();
    ~Controller();

    void setEncrytor(Encryptor* enc) { en = enc; };
    void start();
private:
    Encryptor* en;
    std::string outputDir;
    bool verbose;

    void process();
    void parseCommand(std::vector<std::string> &in);
    std::vector<std::string> getInput();
    bool clearKey();
    bool setKey();
    bool setMessage(int mode); // 0: raw text, 1: file
    bool getMessage();
    bool clearMessage();
    bool setOutputDir();
    void encrypt();
    // Printing methods
    void printHeader();
    void printHelp();
    void printMessage();
};

#endif //AES_CONTROLLER_H
