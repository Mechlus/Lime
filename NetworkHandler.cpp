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

	enet_deinitialize();
	return true;
}

void NetworkHandler::handle() {
	if (!initialized) return;

	while (true) {

	}
}