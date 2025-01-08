#include "IrrHandling.h"
#include "IrrManagers.h"
#include "LimeReceiver.h"
#include "Sound.h"

#include <filesystem>
namespace fs = std::filesystem;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct windowState {
	int curWidth;
	int curHeight;
};

windowState window = { 640, 480 };

std::string getMainPath(const std::string& searchDirectory) {
	try {
		for (const auto& entry : fs::recursive_directory_iterator(searchDirectory)) {
			if (entry.is_regular_file() && entry.path().filename() == "main.lua") {
				return entry.path().string(); // Return the full path to main.lua
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error during directory traversal: " << e.what() << std::endl;
	}
	return "";
}

void IrrHandling::setDriver(irr::video::E_DRIVER_TYPE type) {
	driverType = type;
}

void IrrHandling::initScene()
{
	driverType = irr::video::EDT_DIRECT3D9;
	width = 640;
	height = 480;
	fullscreen = false;
	vSync = false;
	stencil = false;
	dConsole.enabled = false;
	dConsole.doOutput = false;
	posX = 0;
	posY = 0;

	window.curWidth = width;
	window.curHeight = height;

	LuaLime l;
	l.initLua(smgr, driver);

	// Is main.lua safe?
	std::string mainPath = getMainPath(".");
	if (mainPath == "") {
		dConsole.doOutput = true;
		std::string err = "main.lua could not be found!";
		dConsole.sendMsg(err.c_str(), 1);
		end();
		return;
	}
	sol::protected_function_result exec_result = lua->safe_script_file(mainPath);

	if (dConsole.enabled)
		dConsole.makeConsole();

	dConsole.sendMsg("Lime started", 0);

	receiver = new LimeReceiver();
	sound = irrklang::createIrrKlangDevice();
	soundManager = new SoundManager();

	device = irr::createDevice(driverType, dimension2d<u32>(width, height), 16, false, stencil, vSync, receiver);

	device->setWindowCaption(L"Lime Application");

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	driver->getGPUProgrammingServices(); // for shaders, need to do in Material.h

	appLoop();
}

void IrrHandling::makeNewDevice() {
	if (device) {
		device->drop();
	}
	device = irr::createDevice(driverType, dimension2d<u32>(width, height), 16, false, stencil, vSync, receiver);
}

void IrrHandling::doFullscreen() {
	if (device) {
		device->maximizeWindow();
		HWND hwnd = reinterpret_cast<HWND>(device->getVideoDriver()->getExposedVideoData().OpenGLWin32.HWnd);
		LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
		style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		style |= WS_POPUP;
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		ShowWindow(hwnd, SW_MAXIMIZE);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
}

void IrrHandling::doWindowed() {
	if (device) {
		HWND hwnd = reinterpret_cast<HWND>(device->getVideoDriver()->getExposedVideoData().OpenGLWin32.HWnd);
		LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
		style |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
		style &= ~WS_POPUP;
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, HWND_TOP, 100, 100, window.curWidth, window.curHeight, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		ShowWindow(hwnd, SW_RESTORE);
		device->restoreWindow();
	}
}

int IrrHandling::getMemUsed() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	int physMemUsedMB = physMemUsedByMe / (1024.0 * 1024.0);
	return physMemUsedMB;
}

void IrrHandling::end() {
	if (device) {
		device->drop();
	}

	dConsole.sendMsg("Ending application...", 0);

	if (dConsole.doOutput) { // write console output to output.txt if enabled
		dConsole.writeOutput();
	}
}

void IrrHandling::appLoop() {

	// Events
	sol::protected_function luaOnStart = (*lua)["Lime"]["OnStart"];
	sol::protected_function luaOnUpdate = (*lua)["Lime"]["OnUpdate"];
	sol::protected_function luaOnEnd = (*lua)["Lime"]["OnEnd"];

	// Call start in main
	if ((*lua)["Lime"]["OnStart"].get_type() == sol::type::function) {
		sol::protected_function_result result = luaOnStart(dt);
		if (!result.valid())
		{
			sol::error err = result;
			dConsole.sendMsg(std::string(err.what()).c_str(), 1);
		}
	}

	int lastfps = -1;
	u32 then = device->getTimer()->getTime();

	f32 const frameDur = 1000.f / m_frameLimit;

	ILightSceneNode* pLight = device->getSceneManager()->addLightSceneNode();
	SLight& l = pLight->getLightData();
	l.Type = ELT_DIRECTIONAL;
	ISceneNode* pNode = device->getSceneManager()->addEmptySceneNode();
	pLight->setPosition(vector3df(1, 0, 1)); //default is (1,1,0) for directional lights
	pLight->setParent(pNode);
	//now rotate pNode to set light angle

	while (device->run()) {
		const u32 now = device->getTimer()->getTime();
		dt = (now - then) / 16.667f;
		then = now;

		// Set debug window caption
		if (dConsole.enabled) {
			stringw tmp(L"Lime Debugger [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += lastfps;
			tmp += " | mem: ";
			tmp += getMemUsed();
			tmp += " MB";
			SetConsoleTitle(tmp.c_str());
		}

		// Call update in main
		if ((*lua)["Lime"]["OnUpdate"].get_type() == sol::type::function) {
			sol::protected_function_result result = luaOnUpdate(dt);
			if (!result.valid())
			{
				sol::error err = result;
				dConsole.sendMsg(std::string(err.what()).c_str(), 1);
			}
		}

		if (mainCamera) {
			mainCamera->setTarget(mainCameraForward->getAbsolutePosition());
		}

		driver->beginScene(true, true, backgroundColor);
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		lastfps = driver->getFPS();

		u32 frameTime = device->getTimer()->getTime() - now;
		if (frameTime < frameDur) {
			device->sleep(frameDur - frameTime);
		}
	}

	// Call end in main
	if ((*lua)["Lime"]["OnEnd"].get_type() == sol::type::function) {
		sol::protected_function_result result = luaOnEnd();
		if (!result.valid())
		{
			sol::error err = result;
			dConsole.sendMsg(std::string(err.what()).c_str(), 1);
		}
	}
	end();
}
