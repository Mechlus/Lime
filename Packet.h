#pragma once

#include "IrrManagers.h"

class Packet {
private:

public:
	Packet();

	void append(int type, sol::object data);
	void clear();

	int getSize();

	void compress();
	void decompress();

	void encrypt();
	void decrypt();

	// In Only
	int getSenderID();
	sol::object pop(int type); // removes type off of buffer
	sol::object get(int type, int bytePos); // does not modify buffer
};

void bindPacket();