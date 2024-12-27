#pragma once
#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include "IrrHandling.h"
#include <sol/sol.hpp>

class DebugConsole
{
private:
	const char* getTime();
	void abruptEnd();
public:
	void makeConsole();
	void sendMsg(const char* msg, int color);
	void writeOutput();

	bool enabled = false;
	bool doOutput = false;

	std::string out;
};

inline DebugConsole dConsole;

