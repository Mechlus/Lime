#pragma once
#include <irrlicht.h>
#include <irrKlang.h>
#include <windows.h>
#include <psapi.h>
#include <string>
#include "DebugConsole.h"
#include "LuaLime.h"

class IrrHandling
{
private:
	int lastTime = 0;
	int frameCount = 0;
public:
	void setDriver(irr::video::E_DRIVER_TYPE type);
	void initScene();
	void makeNewDevice();
	void doFullscreen();
	void doWindowed();
	int getMemUsed();
	void end();
	void appLoop();
	bool writeTextureToFile(irr::video::ITexture* texture, const irr::core::stringw& name);
	void updateFPS();
	int m_frameLimit = 60;
	float dt;

	irr::video::E_DRIVER_TYPE driverType = irr::video::EDT_DIRECT3D9;
	int width = 640;
	int height = 480;
	int fullscreen = false;
	int vSync = false;
	int stencil = false;
	int posX = 0;
	int posY = 0;
	int fps = 0;
	irr::scene::ISceneNode* skydome = nullptr;
	irr::video::SColor backgroundColor = (255, 100, 101, 140);
};