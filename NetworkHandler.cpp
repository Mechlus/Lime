#include "NetworkHandler.h"
#include <thread>

NetworkHandler::NetworkHandler() {}

void NetworkHandler::setVerbose(bool enable) {
	verbose = enable;
}

bool NetworkHandler::initialize() {
	if (initialized) {
		if (verbose) dConsole.sendMsg("Networking WARNING: ENet is already initialized", MESSAGE_TYPE::NETWORK_VERBOSE);
		return true;
	}

	if (enet_initialize() == 0) {
		initialized = true;
		if (verbose) dConsole.sendMsg("ENet initialized successfully", MESSAGE_TYPE::NETWORK_VERBOSE);
		return true;
	}

	if (verbose) dConsole.sendMsg("Networking WARNING: Failed to initialize ENet", MESSAGE_TYPE::NETWORK_VERBOSE);
	return false;
}

bool NetworkHandler::shutdown() {
	if (!initialized) {
		if (verbose) dConsole.sendMsg("Networking WARNING: ENet cannot be shutdown; ENet is not initialized", MESSAGE_TYPE::NETWORK_VERBOSE);
		return false;
	}

	initialized = false;
	finished = true;
	netLoop.join();

	if (server)
		enet_host_destroy(server);

	enet_deinitialize();
	return true;
}

void NetworkHandler::handle() {
	netLoop = std::thread(netBody, this);
}

void netBody(NetworkHandler* n)
{
	while (!n->finished) {
		if (!n->initialized) {
			std::this_thread::yield();
			continue;
		}
		// Process networking
	}
}

void NetworkHandler::hostServer(std::string ip, int maxClients, int maxChannels) {
	ENetAddress address;
	address.host = ENET_HOST_ANY; // Hosts on localhost by default
	address.port = 1234;

	enet_address_set_host(&address, ip.c_str());

	// Put this in thread
	server = enet_host_create(&address, maxClients, maxChannels, 0, 0);

	char ipString[64];
	int out = enet_address_get_host_ip(&server->address, ipString, sizeof(ipString));

	if (!server || out != 0 || ipString != ip) {
		std::string ms = "";
		ms = "Networking WARNING: Server could not be hosted on IP ";
		ms += ip;
		if (verbose) dConsole.sendMsg(ms.c_str(), MESSAGE_TYPE::NETWORK_VERBOSE);
		if (server)
			enet_host_destroy(server);
		server = nullptr;
		return;
	}

	sol::function f = (*lua)["NetworkServer"]["OnServerHosted"];
	if (f.get_type() == sol::type::function) {
		sol::protected_function_result result = f();
		if (verbose && !result.valid())
		{
			dConsole.sendMsg("Networking WARNING: The server is being hosted but NetworkServer.OnServerHosted is not declared", MESSAGE_TYPE::NETWORK_VERBOSE);
		}
		else if (verbose) {
			std::string msg = "Hosting server on ";
			msg += ipString;
			msg += " for ";
			msg += std::to_string(maxClients);
			msg += " clients with ";
			msg += std::to_string(maxChannels);
			msg += " channels";
			dConsole.sendMsg(msg.c_str(), MESSAGE_TYPE::NETWORK_VERBOSE);
		}
	}
}

bool NetworkHandler::stopHosting() {
	if (server) {
		enet_host_destroy(server);
		if (verbose) dConsole.sendMsg("Server hosting closed", MESSAGE_TYPE::NETWORK_VERBOSE);
		return true;
	}
	else {
		if (verbose) dConsole.sendMsg("Networking WARNING: NetworkServer.StopHosting was called but there is no server to stop hosting", MESSAGE_TYPE::NETWORK_VERBOSE);
		return false;
	}
}

void NetworkHandler::setBandwidthLimit(int incoming, int outgoing) {
	if (!server) {
		if (verbose) dConsole.sendMsg("Networking WARNING: Setting bandwidth limits must be done after hosting a server", MESSAGE_TYPE::NETWORK_VERBOSE);
		return;
	}

	enet_host_bandwidth_limit(server, incoming, outgoing); // in bytes per second
}