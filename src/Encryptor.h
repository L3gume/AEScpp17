#ifndef ENCODER_H
#define ENCODER_H

#include <deque>
#include <string>
#include "AESUtils.h"
#include <memory>

class Encryptor {
public:
	// singleton class stuff, we only want one encryptor. (probably useless)
	static Encryptor& getInstance() {
		static Encryptor instance;
		return instance;
	}
	Encryptor(Encryptor const&) = delete;
	void operator=(Encryptor const&) = delete;

	void printStringFromDeque(bool isKey);

	Encryptor() {}
	~Encryptor() {}
	void encrypt(std::string s, bool verbose);
	void decrypt(std::string s, bool verbose);
	bool setKey(std::string new_key);
	std::string getKey();
    bool parseString(std::string s, bool isKey, int& n);
    void printMessage();
    std::deque<std::shared_ptr<Block>> getMessage();
private:
	std::string key_string;

	const int rounds = 10; // will change eventually
	std::deque<std::shared_ptr<Block>> message;
	std::deque<std::shared_ptr<Block>> key;
	std::string generateRandomKey(); // Will require libSodium or another external library for good random numbers.
	// inline all the things!
	// operations.

    // turns out inlining these methods doesn't really make anything faster.
	/*inline*/ void subBytes();
    /*inline*/ void shiftRows();
    /*inline*/ void mixColumns();
	// inverse operations.
    /*inline*/ void invSubBytes();
    /*inline*/ void invShiftRows();
    /*inline*/ void invMixColumns();
	// addRoundKey is just a XOR, which is its own inverse.
	inline void addRoundKey(unsigned int round) {
		for (auto value: message) {
            *value ^= *key[round]; // Overloaded operator^=, because I can.
		}
	}
	bool generateSubKeys();
};
#endif
