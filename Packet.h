#pragma once

#include "IrrManagers.h"

class Packet {
private:
	bool inPacket;

	net::SOutPacket outp;
	net::SInPacket inp; // Hidden from user
public:
	Packet();
	Packet(const net::SInPacket out);

	void append(int type, sol::object data);
	void clear();

	int getSize();

	void compress();
	void decompress();

	void encrypt();
	void decrypt();

	bool isValid();

	// In Only
	int getSenderID();
	sol::object pop(int type); // removes type off of buffer
	sol::object get(int type, int index); // does not modify buffer
};

void bindPacket();