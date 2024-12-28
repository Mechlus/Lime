#include <iostream>
#include <windows.h>
#include <sstream>
#include <direct.h>
#include <fstream>
#include <psapi.h>

#include "LuaLime.h"
#include "IrrHandling.h"
#include "IrrManagers.h"
#include "DebugConsole.h"

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "assimp-vc143-mt.lib")

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace sol;
using namespace std;

int main()
{
	IrrHandling i;
	irrHandler = &i;
	i.initScene(); // app loop
}