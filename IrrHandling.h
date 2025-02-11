#pragma once
#include <irrlicht.h>
#include <irrKlang.h>
#include <windows.h>
#include <psapi.h>
#include <string>
#include "DebugConsole.h"
#include "LuaLime.h"
#include "XEffects.h"

#include <queue>

struct CameraToQueue {
public:
	CameraToQueue(irr::scene::ICameraSceneNode* c, irr::scene::ISceneNode* f, bool d, bool g) : cam(c), forward(f), defaultRendering(d), renderGUI(g) {};

	irr::scene::ICameraSceneNode* cam = nullptr;
	irr::scene::ISceneNode* forward = nullptr;
	bool defaultRendering = false;
	bool renderGUI = false;
};

struct BatchedTransform {
public:
	BatchedTransform(int t, irr::scene::ISceneNode* n, irr::core::vector3df v) : type(t), node(n), transform(v) {};

	int type = 0;
	irr::scene::ISceneNode* node = nullptr;
	irr::core::vector3df transform = irr::core::vector3df();
};

class IrrHandling
{
private:
	int lastTime = 0;
	int frameCount = 0;
public:
	void setDriver(irr::video::E_DRIVER_TYPE type);
	void initScene();
	void capture();
	void makeNewDevice();
	void doFullscreen();
	void doWindowed();
	int getMemUsed();
	void end();
	void appLoop();
	bool writeTextureToFile(irr::video::ITexture* texture, const irr::core::stringw& name);
	void updateFPS();
	void AddCameraToQueue(irr::scene::ICameraSceneNode* cam, irr::scene::ISceneNode* forward, bool defaultRendering, bool renderGUI);
	void AddTransformToQueue(int type, irr::scene::ISceneNode* node, irr::core::vector3df transform);
	void HandleTransformQueue();
	void HandleCameraQueue();
	int m_frameLimit = 60;
	float dt;
	bool didEnd = false;
	bool defaultExclude = false;
	bool renderedGUI = false;
	int lights = 0;
	bool legacyDrawing = false;

	irr::video::E_DRIVER_TYPE driverType = irr::video::EDT_DIRECT3D9;
	int width = 640;
	int height = 480;
	int fullscreen = false;
	int vSync = false;
	int stencil = false;
	int posX = 0;
	int posY = 0;
	int fps = 0;

	std::string key = "";
	bool verbose = false;

	// Render queue
	std::queue<CameraToQueue> cameraQueue;

	// Transform queue
	std::queue<BatchedTransform> transformQueue;

	// XEffects
	E_FILTER_TYPE defaultShadowFiltering = E_FILTER_TYPE::EFT_8PCF;
	int defaultShadowResolution = 1024;

	irr::scene::ISceneNode* skydome = nullptr;
	irr::video::SColor backgroundColor = (255, 100, 101, 140);
};