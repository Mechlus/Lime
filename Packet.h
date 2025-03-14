#pragma once

#include "IrrHandling.h"
#include <enet\enet.h>

class Packet {
private:

public:
	Packet();
	Packet(const void* data, size_t size, int sender);
	Packet(ENetPacket* p, int id);
	~Packet();

	void append(int type, sol::object data); // Append data to packet
	void clear(); // Clears binary

	int getSize(); // Returns size

	// In Only
	int getSenderID();
	sol::object get(int type, size_t bytePos); // does not modify buffer
	sol::object getNext(int type);
	void setPosition(int pos);

	int getPosition();

	bool writeToFile(int bytePos, std::string path);

	ENetPacket* p = nullptr;
	int originalID = -1;
	int pos = 0;
};

void bindPacket();