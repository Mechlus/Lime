#pragma once

#include "IrrHandling.h"
#include <thread>
#include <enet\enet.h>

class NetworkHandler
{
public:
	NetworkHandler();

	bool initialize(); // Initialize ENet handling
	bool shutdown(); // Shutdown networking system, disconnect from servers, stop hosting etc.
	void handle(); // Handle events if initialized
	void setVerbose(bool enable); // Enable/disable networking warnings

	// Server/Hosting
	bool hostServer(std::string ip, int maxClients, int maxChannels);

	// Client

	bool initialized = false;
	bool verbose = false;
	bool finished = false;
private:
	// Server
	ENetHost* server = nullptr;

	// Client
	ENetHost* host = nullptr;
	ENetPeer* peer = nullptr;
	
	std::thread netLoop;
};

void netBody(NetworkHandler* n);