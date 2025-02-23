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

ENetHost* NetworkHandler::getHost() {
	return server;
}

ENetPeer* NetworkHandler::getClient() {
	return client;
}

void NetworkHandler::setTimeoutLength(int ms) {
	if (!initialized) {
		if (verbose) dConsole.sendMsg("Networking WARNING: Timeout length cannot be set; ENet is not initialized", MESSAGE_TYPE::NETWORK_VERBOSE);
	}
	else {
		timeoutLength = ms;
		if (verbose) {
			std::string msg = "Networking timeout length updated (";
			msg += ms;
			msg += " milliseconds)";
			dConsole.sendMsg(msg.c_str(), MESSAGE_TYPE::NETWORK_VERBOSE);
		}
	}
}

int NetworkHandler::getTimeoutLength() {
	return timeoutLength;
}

void netBody(NetworkHandler* n)
{
	ENetEvent* event;

	while (!n->finished) {
		if (!n->initialized || !(n->getHost())) {
			std::this_thread::yield();
			continue;
		}

		while (enet_host_service(n->getHost(), event, n->getTimeoutLength())) {
			switch (event->type) {
			case ENET_EVENT_TYPE_CONNECT:
				if ((*lua)["NetworkServer"]["OnPeerConnect"].get_type() == sol::type::function) {
					sol::protected_function_result result = (*lua)["NetworkServer"]["OnPeerConnect"];
				}
				else {
					if (n->verbose) dConsole.sendMsg("Networking WARNING: A peer connected but NetworkServer.OnPeerConnect is not declared", MESSAGE_TYPE::NETWORK_VERBOSE);
				}

				if (n->verbose) {
					std::string msg = "Peer joined presuming ID ";
					msg += event->peer->incomingPeerID;
					msg += " from IP ";
					msg += event->peer->address.host;
					dConsole.sendMsg(msg.c_str(), MESSAGE_TYPE::NETWORK_VERBOSE);
				}
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				if ((*lua)["NetworkServer"]["OnPeerDisconnect"].get_type() == sol::type::function) {
					sol::protected_function_result result = (*lua)["NetworkServer"]["OnPeerDisconnect"];
				}
				else {
					if (n->verbose) dConsole.sendMsg("Networking WARNING: A peer disconnected but NetworkServer.OnPeerDisconnect is not declared", MESSAGE_TYPE::NETWORK_VERBOSE);
				}

				if (n->verbose) {
					std::string msg = "Peer disconnected abandoning ID ";
					msg += event->peer->incomingPeerID;
					msg += " from IP ";
					msg += event->peer->address.host;
					dConsole.sendMsg(msg.c_str(), MESSAGE_TYPE::NETWORK_VERBOSE);
				}
				break;
			}
		}
	}
}

void NetworkHandler::hostServer(std::string ip, int maxClients, int maxChannels) {
	if (!initialized) {
		if (verbose) dConsole.sendMsg("Networking WARNING: Host cannot be created; ENet is not initialized", MESSAGE_TYPE::NETWORK_VERBOSE);
		return;
	}

	if (server) {
		if (verbose) dConsole.sendMsg("Networking WARNING: Host cannot be created; there is an ongoing host", MESSAGE_TYPE::NETWORK_VERBOSE);
		return;
	}

	ENetAddress address;
	address.host = ENET_HOST_ANY; // Hosts on localhost by default
	address.port = 1234;

	enet_address_set_host(&address, ip.c_str());

	// Put this in thread
	server = enet_host_create(&address, maxClients, maxChannels, 0, 0);

	char ipString[64];
	int out = enet_address_get_host_ip(&server->address, ipString, sizeof(ipString));

	if (!server || out != 0) {
		std::string ms = "";
		ms = "Networking WARNING: Server could not be hosted on IP ";
		ms += ipString;
		if (verbose) dConsole.sendMsg(ms.c_str(), MESSAGE_TYPE::NETWORK_VERBOSE);

		if ((*lua)["NetworkServer"]["OnHostFail"].get_type() == sol::type::function) {
			sol::protected_function_result result = (*lua)["NetworkServer"]["OnHostFail"];
		}
		else if (verbose) {
			dConsole.sendMsg("Networking WARNING: The server failed to host but NetworkServer.OnHostFail is not declared", MESSAGE_TYPE::NETWORK_VERBOSE);
		}

		if (verbose) {
			dConsole.sendMsg("Networking WARNING: The server is being hosted but NetworkServer.OnHosted is not declared", MESSAGE_TYPE::NETWORK_VERBOSE);
		}

		if (server)
			enet_host_destroy(server);
		server = nullptr;
		return;
	}

	if ((*lua)["NetworkServer"]["OnHosted"].get_type() == sol::type::function) {
		sol::protected_function_result result = (*lua)["NetworkServer"]["OnHosted"];
		if (verbose) {
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
	else if (verbose) {
		dConsole.sendMsg("Networking WARNING: The server is being hosted but NetworkServer.OnHosted is not declared", MESSAGE_TYPE::NETWORK_VERBOSE);
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

bool NetworkHandler::isHosting() {
	return server;
}

int NetworkHandler::getServerIP() {
	return server ? server->address.host : 0;
}

int NetworkHandler::getPort() {
	return server ? server->address.port : 0;
}

void NetworkHandler::setUseRangeEncoder(bool enable) {
	if (server && enable)
		enet_host_compress_with_range_coder(server);
	else if (server)
		enet_host_compress(server, nullptr);

	if (!server && verbose) dConsole.sendMsg("Networking WARNING: Enabling/disabling compression must be done after hosting a server", MESSAGE_TYPE::NETWORK_VERBOSE);
	else if (verbose) {
		if (enable) dConsole.sendMsg("Range-encoding compressor enabled", MESSAGE_TYPE::NETWORK_VERBOSE);
		else dConsole.sendMsg("Range-encoding compressor disabled", MESSAGE_TYPE::NETWORK_VERBOSE);
	}
}
