#include <iostream>
#include <chrono>
#include "Encryptor.h"
#include "Controller.h"

using namespace std; // necessary? probly not.

int main(int argc, char* argv[]) {
    Controller controller;
    chrono::time_point<chrono::high_resolution_clock> start, end;
    chrono::duration<double> elapsed_seconds;
    int nb_lines, nb_blocks;
    string text;

	// Lets do the aes implementation for real.
	Encryptor en /*= new Encryptor()*/;
    start = chrono::high_resolution_clock::now();
    en.setKey("abcdefghijklmnop");
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    cout << "Computed 10 subkeys in: " << elapsed_seconds.count() << "s\n";

    start = chrono::high_resolution_clock::now();
    text = readFile(nb_lines);
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    cout << "Read file in: " << elapsed_seconds.count() << "s\n";

    start = chrono::high_resolution_clock::now();
    en.parseString(text, false, nb_blocks);
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    cout << "Converted "<< nb_lines <<" lines of text into "<< nb_blocks <<" blocks in: " << elapsed_seconds.count() << "s\n";
    cout << "Total message length: " << nb_blocks * 16 << " characters." << "\n";

    /*start = chrono::high_resolution_clock::now();
    en.encrypt(text, false);
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    cout << "Encrypted the message in: " << elapsed_seconds.count() << "s\n";

    start = chrono::high_resolution_clock::now();
    en.decrypt("", false);
    end = chrono::high_resolution_clock::now();
    elapsed_seconds = end-start;
    cout << "Decrypted the message in: " << elapsed_seconds.count() << "s\n";*/

    controller.setEncrytor(&en);
    controller.start();
	return 0;
}
