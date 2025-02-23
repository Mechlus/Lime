#pragma once

#include "IrrHandling.h"
#include <thread>
#include <enet\enet.h>

#include "Packet.h"

class NetworkHandler
{
public:
	NetworkHandler();

	bool initialize(); // Initialize ENet handling
	bool shutdown(); // Shutdown networking system, disconnect from servers, stop hosting etc.
	void handle(); // Handle events if initialized
	void setVerbose(bool enable); // Enable/disable networking warnings

	// Server/Hosting
	void hostServer(std::string ip, int maxClients, int maxChannels); // Calls NetworkServer.OnHosted on completion, NetworkServer.OnHostFail on fail
	bool stopHosting();
	void setBandwidthLimit(int incoming, int outgoing);
	bool isHosting();
	int getServerIP();
	int getPort();
	void setUseRangeEncoder(bool enable);
	ENetHost* getHost();
	ENetPeer* getClient();
	void setTimeoutLength(int ms);
	int getTimeoutLength();

	// Client

	bool initialized = false;
	bool verbose = false;
	bool finished = false;
private:
	// Server
	ENetHost* server = nullptr;

	// Client
	ENetPeer* client = nullptr;
	
	std::thread netLoop;
	int timeoutLength = 1000;
};

void netBody(NetworkHandler* n);