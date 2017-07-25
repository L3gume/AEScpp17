#include <iostream>
#include <vector>
#include "Encryptor.h"

void Encryptor::encrypt(std::string s, bool verbose) {
	unsigned int i = 0;
    /*if (s != "") {
        //std::cout << s << "\n";
        parseString(s, false);
    }*/
	//begin
	addRoundKey(i++);
    if (verbose)
        printStringFromDeque(false);
	while (i < rounds) {
		subBytes();
		shiftRows();
		mixColumns();
		addRoundKey(i++);
        if (verbose)
            printStringFromDeque(false);
    }
	subBytes();
	shiftRows();
	addRoundKey(i); // i should be 10
    if (verbose)
        printStringFromDeque(false);
}

void Encryptor::decrypt(std::string s, bool verbose) {
	unsigned int i = rounds;
    /*if (s != "") {
        std::cout << s << "\n";
        parseString(s, false);
    }*/
    if (verbose)
        std::cout << "Decrypt:\n" << "\n";
	addRoundKey(i--);
    if (verbose)
        printStringFromDeque(false);
    while (i > 0) {
        invShiftRows();
        invSubBytes();
		addRoundKey(i--);
        invMixColumns();
        if (verbose)
            printStringFromDeque(false);
    }
    invShiftRows();
	invSubBytes();
	addRoundKey(i); // i should be 0
    if (verbose)
        printStringFromDeque(false);
}

bool Encryptor::setKey(std::string new_key) {
	bool success = true;
    int n;
	if (new_key.length() != 16) { // for aes 128, will implement the other versions once 128 is working.
		std::cout << "SetKey: invalid key length, enter a key of 16 characters." << "\n";
		return false;
	}
	parseString(new_key, true, n);// That should never fail if the key is ok.
    generateSubKeys();

	return success; // success
}

bool Encryptor::parseString(std::string s, bool isKey, int& n) {
	int nbBlocks = 0;
	unsigned char temp[16];
	std::deque<Block*> *p = isKey ? &key : &message; // get the reference to either the message of the key.
    p->clear(); // empty the key/message.
	std::deque<unsigned char> bytes(s.begin(), s.end()); // convert the string to an unsigned char queue.
	nbBlocks = (bytes.size() % 16 == 0) ? static_cast<int>(bytes.size()/16) : static_cast<int>((bytes.size()/16) + 1);
    n = nbBlocks;
	while (nbBlocks-- > 0) {
		for (int i = 0; i < 16; i++) {
			if (bytes.size() > 0) {
				temp[i] = bytes.front();
				bytes.pop_front();
			} else { // If the queue is empty, fill the rest of the block with 0s.
				temp[i] = 0x00;
			}
		}
		(*p).push_back(new Block(temp)); // Push the block
	}
	return true; // return the success status. (the method might end up being void)
}

/*inline*/ void Encryptor::subBytes() {
	for (auto iter: message) {
		for (int i = 0; i < 16; i++) {
			iter->state[0][i] = tableLookup(iter->state[0][i], false);
		}
	}
}

/*inline*/ void Encryptor::shiftRows() {
	unsigned char temp;
	for (auto iter: message) {
		temp = iter->state[1][0];
		iter->state[1][0] = iter->state[1][1];
		iter->state[1][1] = iter->state[1][2];
		iter->state[1][2] = iter->state[1][3];
		iter->state[1][3] = temp;
		for (int i = 0; i < 2; i++) {
			temp = iter->state[2][0];
			iter->state[2][0] = iter->state[2][1];
			iter->state[2][1] = iter->state[2][2];
			iter->state[2][2] = iter->state[2][3];
			iter->state[2][3] = temp;
		}
		for (int i = 0; i < 3; i++) {
			temp = iter->state[3][0];
			iter->state[3][0] = iter->state[3][1];
			iter->state[3][1] = iter->state[3][2];
			iter->state[3][2] = iter->state[3][3];
			iter->state[3][3] = temp;
		}
	}
}

/*inline*/ void Encryptor::mixColumns() {
	/*
	|s'1| = |2	3	1	1| |s1|
	|s'2|   |1	2	3	1| |s2|
	|s'3|   |1	1	2	3| |s3|
	|s'4|   |3	1	1	2| |s4|
	*/

    unsigned char tmp[4];

	for (auto iter: message) {
		for (int i = 0; i < 4; i++) {

            for (int j = 0; j < 4; j++) {
                tmp[j] = iter->state[j][i];
            }

			iter->state[0][i] = galloisMult(tmp[0], 0x02) ^ galloisMult(tmp[1], 0x03) ^ tmp[2] ^ tmp[3];
			iter->state[1][i] = tmp[0] ^ galloisMult(tmp[1], 0x02) ^ galloisMult(tmp[2], 0x03) ^ tmp[3];
			iter->state[2][i] = tmp[0] ^ tmp[1] ^ galloisMult(tmp[2], 0x02) ^ galloisMult(tmp[3], 0x03);
			iter->state[3][i] = galloisMult(tmp[0], 0x03) ^ tmp[1] ^ tmp[2] ^ galloisMult(tmp[3], 0x02);
		}
	}
}

/*inline*/ void Encryptor::invSubBytes() {
	for (auto iter: message) {
		for (int i = 0; i < 16; i++) {
			iter->state[0][i] = tableLookup(iter->state[0][i], true);
		}
	}
}

/*inline*/ void Encryptor::invShiftRows() {
	unsigned char temp;
	for (auto iter: message) {
		temp = iter->state[1][3];
		iter->state[1][3] = iter->state[1][2];
		iter->state[1][2] = iter->state[1][1];
		iter->state[1][1] = iter->state[1][0];
		iter->state[1][0] = temp;
		for (int i = 0; i < 2; i++) {
			temp = iter->state[2][3];
			iter->state[2][3] = iter->state[2][2];
			iter->state[2][2] = iter->state[2][1];
			iter->state[2][1] = iter->state[2][0];
			iter->state[2][0] = temp;
		}
		for (int i = 0; i < 3; i++) {
			temp = iter->state[3][3];
			iter->state[3][3] = iter->state[3][2];
			iter->state[3][2] = iter->state[3][1];
			iter->state[3][1] = iter->state[3][0];
			iter->state[3][0] = temp;
		}
	}
}

/*inline*/ void Encryptor::invMixColumns() {
	/*
    |s'1| = |0e	0b	0d	09| |s1|
    |s'2|   |09	0e	0b	0d| |s2|
    |s'3|   |0d	09	0e	0b| |s3|
    |s'4|   |0b	0d	09	0e| |s4|
    */

    unsigned char tmp[4];

	for (auto iter: message) {
		for (int i = 0; i < 4; i++) {

            for (int j = 0; j < 4; j++) {
                tmp[j] = iter->state[j][i];
            }

			iter->state[0][i] = galloisMult(tmp[0], 0x0e) ^ galloisMult(tmp[1], 0x0b) ^ galloisMult(tmp[2], 0x0d) ^ galloisMult(tmp[3], 0x09);
			iter->state[1][i] = galloisMult(tmp[0], 0x09) ^ galloisMult(tmp[1], 0x0e) ^ galloisMult(tmp[2], 0x0b) ^ galloisMult(tmp[3], 0x0d);
			iter->state[2][i] = galloisMult(tmp[0], 0x0d) ^ galloisMult(tmp[1], 0x09) ^ galloisMult(tmp[2], 0x0e) ^ galloisMult(tmp[3], 0x0b);
			iter->state[3][i] = galloisMult(tmp[0], 0x0b) ^ galloisMult(tmp[1], 0x0d) ^ galloisMult(tmp[2], 0x09) ^ galloisMult(tmp[3], 0x0e);
		}
	}
}

bool Encryptor::generateSubKeys() {
	unsigned char col4PrevKey[4];
	unsigned char newKey[4][4];

	if (!key.at(0)) // If the starting key wasn't parsed correctly.
		return false;

	for (int i = 1; i <= rounds; i++) {
		// Column rotation
		col4PrevKey[0] = key[i-1]->state[3][1];
		col4PrevKey[1] = key[i-1]->state[3][2];
		col4PrevKey[2] = key[i-1]->state[3][3];
		col4PrevKey[3] = key[i-1]->state[3][0];
		// table lookup on the column.
		for (int j = 0; j < 4; j++) {
			col4PrevKey[j] = tableLookup(col4PrevKey[j], false);
		}
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				if (j == 0) {
					col4PrevKey[k] = col4PrevKey[k] ^ rcon[k][i - 1];
					col4PrevKey[k] = key[i-1]->state[j][k] ^ col4PrevKey[k];
				} else {
					col4PrevKey[k] = key[i-1]->state[j][k] ^ col4PrevKey[k];
				}
				newKey[j][k] = col4PrevKey[k];
			}
		}
		key.push_back(new Block(newKey));
	}
    std::cout << "Key expansion:" << "\n";
    printStringFromDeque(true);
	return true;
}

void Encryptor::printStringFromDeque(bool isKey) {
    std::deque<Block*> *p = isKey ? &key : &message;
    std::string ret = "";
    for (auto iter: *p) {
        for (int i = 0; i < 16; i++) {
            ret += iter->state[0][i];
        }
        if (isKey)
            ret += "\n";
    }
    std::cout << ret << "\n";
}

void Encryptor::printMessage() {
    printStringFromDeque(false);
}