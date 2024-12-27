#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "LuaLime.h"
#include "Vector2D.h"
#include "Sound.h"

typedef unsigned int u32;

using namespace irr;

namespace Warden {
	inline video::SColor fogColor = video::SColor(255, 138, 125, 81);
	inline video::E_FOG_TYPE fogType = video::EFT_FOG_LINEAR;
	inline float startDistance = 250.0f;
	inline float endDistance = 1000.0f;
	inline float fogDensity = 0.003f;
	inline bool pixelFog = true;
	inline bool rangeFog = false;
	inline std::string caption = "Lime Application";

	void logConsole(const std::string& title, int intensity) {
		dConsole.sendMsg(title.c_str(), intensity);
	}

	// Get/set window caption
	std::string getTitle() {
		return caption;
	}

	void setTitle(const std::string& title) {
		caption = title;
		if (device) {
			device->setWindowCaption(irr::core::stringw(caption.c_str()).c_str());
		}
	}

	// Set window mode
	void fullscreenWindow(bool var) {
		var ? irrHandler->doFullscreen() : irrHandler->doWindowed();
	}

	irr::video::E_DRIVER_TYPE getDriverType(int driverType) {
		switch (driverType) {
		case 0: return irr::video::EDT_NULL;
		case 1: return irr::video::EDT_SOFTWARE;
		case 2: return irr::video::EDT_BURNINGSVIDEO;
		case 3: return irr::video::EDT_DIRECT3D8;
		case 4: return irr::video::EDT_DIRECT3D9;
		case 5: return irr::video::EDT_OPENGL;
		default: return irr::video::EDT_NULL; // Default to null driver if invalid
		}
	}

	// set driver type
	bool setDriverType(int driverType) {
		irr::video::E_DRIVER_TYPE type = getDriverType(driverType);
		if (device->isDriverSupported(type)) {
			irrHandler->setDriver(type);
			return true;
		}
		return false;

		/*
			if (device->isDriverSupported(type)) {
				if (device) {
					device->closeDevice();
					device->drop();
					device = nullptr;
				}

				device = irr::createDevice(type, irr::core::dimension2d<u32>(irrHandler->width, irrHandler->height), 16, false, irrHandler->stencil, irrHandler->vSync, receiver);
			}

			return device != nullptr;
		*/
	}

	// set window size
	bool setWindowSize(Vector2D& sizes) {
		if (sizes.x > 0 && sizes.y > 0) {
			irrHandler->width = sizes.x;
			irrHandler->height = sizes.y;
			return true;
		}
		return false;
	}

	// set window position
	void setWindowPosition(Vector2D& position, irr::IrrlichtDevice* device) {
		if (device) {

#ifdef _WIN32
			HWND hwnd = reinterpret_cast<HWND>(device->getVideoDriver()->getExposedVideoData().OpenGLWin32.HWnd);
			if (hwnd) {
				SetWindowPos(hwnd, nullptr, static_cast<int>(position.x), static_cast<int>(position.y), 0, 0,
					SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
#endif
		}
	}

	void recreateDevice() {
		irrHandler->makeNewDevice();
	}

	Vector2D getMonitorSize() {
#ifdef _WIN32
		// set irrHandler pos perhaps
		return Vector2D(
			static_cast<float>(GetSystemMetrics(SM_CXSCREEN)),
			static_cast<float>(GetSystemMetrics(SM_CYSCREEN))
		);
#endif
	}

	// Get/set frame rate
	int getFrameRate() {
		if (device && irrHandler)
			return irrHandler->m_frameLimit;
		return 0;
	}

	void setFrameRate(int fps) {
		if (device && fps >= 0 && irrHandler)
			irrHandler->m_frameLimit = fps;
	}

	// Get memory usage
	int getMemoryUsage() {
		if (device && irrHandler)
			return irrHandler->getMemUsed();
		return 0;
	}

	// Get elapsed time in miliseconds
	int getElapsedTime() {
		if (device)
			return device->getTimer()->getTime();
		return 0;
	}

	// Exit application
	void endApplication() {
		if (irrHandler)
			irrHandler->end();
	}

	// Is window focused?
	bool isFocused() {
		if (device)
			return device->isWindowFocused();
		return false;
	}

	// Allow window to be resizable?
	void makeResizable(bool enable) {
		if (device)
			device->setResizable(enable);
	}

	// Set background color
	void setBackgroundColor(Vector3D& color) {
		if (driver && irrHandler) {
			irrHandler->backgroundColor = irr::video::SColor(255, color.x, color.y, color.z);
		}
	}

	// Set skydome
	void setSkydome(Texture& sky) {
		if (!irrHandler || !device)
			return;
		if (irrHandler->skydome) {
			irrHandler->skydome->remove();
		}

		irrHandler->skydome = smgr->addSkyDomeSceneNode(sky.texture, 16, 8, 0.9, 2.0, 100.0);
	}

	void setSkydomeParams(u32 resX, u32 resY, float texPercent, float spherePercent, float radius) {
		irr::video::ITexture* cur = nullptr;
		if (!irrHandler || !device)
			return;
		if (irrHandler->skydome) {
			if (irrHandler->skydome->getMaterialCount() > 0) {
				cur = irrHandler->skydome->getMaterial(0).getTexture(0);
			}
			irrHandler->skydome->remove();
		}
		if (cur)
			irrHandler->skydome = smgr->addSkyDomeSceneNode(cur, resX, resY, texPercent, spherePercent, radius);
	}

	// Get number of objects
	int getObjectCount() {
		if (!irrHandler || !device)
			return 0;

		return smgr->getRootSceneNode()->getChildren().getSize();
	}

	// Show cursor
	void showCursor(bool var) {
		if (device)
			device->getCursorControl()->setVisible(var);
	}

	// Move cursor
	void setCursorPosition(Vector2D pos) {
		if (device)
			device->getCursorControl()->setPosition(irr::core::vector2di(pos.x, pos.y));
	}

	bool addArchive(const std::string& path) {
		if (device) {
			device->getFileSystem()->addFileArchive(path.c_str());
			return true;
		}
		return false;
	}

	sol::table fireRaypick(Vector3D start, Vector3D end) {
		scene::ISceneCollisionManager* collisionManager = smgr->getSceneCollisionManager();
		core::line3d<f32> ray(core::vector3df(start.x, start.y, start.z), core::vector3df(end.x, end.y, end.z));

		core::vector3df hitPosition;
		core::triangle3df hitTriangle;
		scene::ISceneNode* pickedNode = collisionManager->getSceneNodeAndCollisionPointFromRay(
			ray, hitPosition, hitTriangle);

		sol::table result = lua->create_table();

		if (pickedNode) {
			Vector3D normal = Vector3D(hitTriangle.getNormal().X, hitTriangle.getNormal().Y, hitTriangle.getNormal().Z);
			video::SMaterial material = pickedNode->getMaterial(0);
			Vector3D hit = Vector3D(hitPosition.X, hitPosition.Y, hitPosition.Z);
			result["ID"] = pickedNode->getID();
			result["normal"] = normal;
			result["materialID"] = material.ID;
			result["hitPosition"] = hit;
		}
		else {
			result["ID"] = -1;
			result["normal"] = Vector3D(0,1,0);
			result["materialID"] = -1;
			result["hitPosition"] = end;
		}

		return result;
	}

	sol::table fireRaypick2D(Vector2D screenCoord, Vector3D end) {
		scene::ISceneCollisionManager* collisionManager = smgr->getSceneCollisionManager();
		core::line3d<f32> ray = collisionManager->getRayFromScreenCoordinates(
			core::position2di(screenCoord.x, screenCoord.y), smgr->getActiveCamera());
		ray.end = core::vector3df(end.x, end.y, end.z);

		core::vector3df hitPosition;
		core::triangle3df hitTriangle;
		scene::ISceneNode* pickedNode = collisionManager->getSceneNodeAndCollisionPointFromRay(
			ray, hitPosition, hitTriangle);

		sol::table result = lua->create_table();

		if (pickedNode) {
			Vector3D normal = Vector3D(hitTriangle.getNormal().X, hitTriangle.getNormal().Y, hitTriangle.getNormal().Z);
			video::SMaterial material = pickedNode->getMaterial(0);
			Vector3D hit = Vector3D(hitPosition.X, hitPosition.Y, hitPosition.Z);
			result["ID"] = pickedNode->getID();
			result["normal"] = normal;
			result["materialID"] = material.ID;
			result["hitPosition"] = hit;
		}
		else {
			result["ID"] = -1;
			result["normal"] = Vector3D(0, 1, 0);
			result["materialID"] = -1;
			result["hitPosition"] = end;
		}

		return result;
	}

	void showConsole(bool var) {
		dConsole.enabled = var;
	}

	void writeConsoleOutput(bool var) {
		dConsole.doOutput = var;
	}

	// INPUT

	// Input binds
	sol::table getMouseState() {
		if (receiver) {
			return receiver->getMouseState();
		}
		return sol::table();
	}

	sol::table getControllerState() {
		if (receiver) {
			return receiver->getControllerState();
		}
		return sol::table();
	}

	bool isKeyDown(int c) {
		if (receiver && c >= 0 && c < irr::KEY_KEY_CODES_COUNT) {
			return receiver->isKeyDown(static_cast<irr::EKEY_CODE>(c));
		}
		return false;
	}

	void isControllerConnected() {
		core::array<SJoystickInfo> joystickInfo;
		device->activateJoysticks(joystickInfo);
	}

	// FOG

	// Apply fog settings
	void applyFogSettings() {
		driver->setFog(
			fogColor,
			fogType,
			startDistance,
			endDistance,
			fogDensity,
			pixelFog,
			rangeFog
		);
	}

	// Set fog distances (start and end)
	void setFogDistances(Vector2D dist) {
		startDistance = dist.x;
		endDistance = dist.y;
		applyFogSettings();
	}

	// Set fog color
	void setFogColor(Vector3D color) {
		fogColor = video::SColor(255, static_cast<u32>(color.x), static_cast<u32>(color.y), static_cast<u32>(color.z));
		applyFogSettings();
	}

	// Set fog type (linear, exponential, or exponential)
	void setFogType(int i) {
		switch (i) {
		case 0: fogType = video::EFT_FOG_LINEAR; break;
		case 1: fogType = video::EFT_FOG_EXP; break;
		case 2: fogType = video::EFT_FOG_EXP2; break;
		default: fogType = video::EFT_FOG_LINEAR; break;
		}
		applyFogSettings();
	}

	// Set fog density (for exponential fog types)
	void setFogDensity(float density) {
		fogDensity = density;
		applyFogSettings();
	}

	// Enable or disable per-pixel fog
	void setFogPixel(bool var) {
		pixelFog = var;
		applyFogSettings();
	}

	// Use range-based fog for better depth calculations, more expensive however
	void useDistanceForFog(bool var) {
		rangeFog = var;
		applyFogSettings();
	}

	void setFogSettings(Vector2D dist, Vector3D color, int i, float density, bool var, bool var1) {
		startDistance = dist.x;
		endDistance = dist.y;
		fogColor = video::SColor(255, static_cast<u32>(color.x), static_cast<u32>(color.y), static_cast<u32>(color.z));
		switch (i) {
		case 0: fogType = video::EFT_FOG_LINEAR; break;
		case 1: fogType = video::EFT_FOG_EXP; break;
		case 2: fogType = video::EFT_FOG_EXP2; break;
		default: fogType = video::EFT_FOG_LINEAR; break;
		}
		fogDensity = density;
		pixelFog = var;
		rangeFog = var1;
		applyFogSettings();
	}

	// Sound
	int play2DSound(const std::string& filePath, bool loop = false) {
		return soundManager->play2DSound(filePath, loop);
	}

	int play3DSound(const std::string& filePath, Vector3D src, bool loop = false) {
		return soundManager->play3DSound(filePath, src, loop);
	}

	void play2DSoundOnChannel(int channel, const std::string& filePath, bool loop = false) {
		soundManager->play2DSoundOnChannel(channel, filePath, loop);
	}

	void play3DSoundOnChannel(int channel, const std::string& filePath, Vector3D src, bool loop = false) {
		soundManager->play3DSoundOnChannel(channel, filePath, src, loop);
	}

	void stopChannel(int channel) {
		soundManager->stopChannel(channel);
	}

	void pauseChannel(int channel, bool paused) {
		soundManager->pauseChannel(channel, paused);
	}

	void loopChannel(int channel, bool loop) {
		soundManager->loopChannel(channel, loop);
	}

	void stopAllSounds() {
		soundManager->stopAllSounds();
	}

	void preloadSound() {
		soundManager->preloadSound();
	}

	void setListenerPosition(Vector3D pos, Vector3D forward) {
		soundManager->setListenerPosition(pos, forward);
	}

	bool isChannelFree(int channel) {
		return soundManager->isChannelFree(channel);
	}

	int getNextAvailableChannel() {
		return soundManager->getNextAvailableChannel();
	}

	void resetChannelFX(int channel) {
		soundManager->resetChannelFX(channel);
	}

	void setPitch(int channel, float pitch) {
		soundManager->setPitch(channel, pitch);
	}

	void setVolume(int channel, float volume) {
		soundManager->setVolume(channel, volume);
	}

	void setPan(int channel, float pan) {
		soundManager->setPan(channel, pan);
	}

	void setDistortionEffect(int channel, int effect, bool enable, sol::table params) {
		soundManager->setDistortionEffect(channel, effect, enable, params);
	}

	int validChannel(int i) {
		return soundManager->validChannel(i);
	}

	std::string printChannelList() {
		return soundManager->printChannelList();
	}
};

void bindWarden(sol::table application, sol::table world, sol::table sound, sol::table gui, sol::table input) {
	// application
	application["SetDriverType"] = &Warden::setDriverType;
	application["SetFullscreen"] = &Warden::fullscreenWindow;
	application["SetCaption"] = &Warden::setTitle;
	application["GetCaption"] = &Warden::getTitle;
	application["SetWindowPosition"] = &Warden::setWindowPosition;
	application["SetWindowSize"] = &Warden::setWindowSize;
	application["GetMonitorSize"] = &Warden::getMonitorSize;
	application["EndApplication"] = &Warden::endApplication;
	application["IsWindowFocused"] = &Warden::isFocused;
	application["SetResizable"] = &Warden::makeResizable;
	application["GetElapsedTime"] = &Warden::getElapsedTime;
	application["Log"] = &Warden::logConsole;
	application["AddArchiveToMemory"] = &Warden::addArchive;
	application["SetShowConsole"] = &Warden::showConsole;
	application["SetWriteConsole"] = &Warden::writeConsoleOutput;
	application["RecreateDevice"] = &Warden::recreateDevice;
	
	// world
	world["GetFrameRate"] = &Warden::getFrameRate;
	world["SetFrameRate"] = &Warden::setFrameRate;
	world["GetMemoryUsage"] = &Warden::getMemoryUsage;
	world["SetSkydome"] = &Warden::setSkydome;
	world["SetSkydomeParameters"] = &Warden::setSkydomeParams;
	world["SetBackgroundColor"] = &Warden::setBackgroundColor;
	world["GetObjectCount"] = &Warden::getObjectCount;
	world["FireRaypick3D"] = &Warden::fireRaypick;
	world["FireRaypick2D"] = &Warden::fireRaypick2D;
	world["SetFogDistances"] = &Warden::setFogDistances;
	world["SetFogColor"] = &Warden::setFogColor;
	world["SetFogType"] = &Warden::setFogType;
	world["SetFogDensity"] = &Warden::setFogDensity;
	world["SetPixelFog"] = &Warden::setFogPixel;
	world["SetRangeFog"] = &Warden::useDistanceForFog;
	world["SetFogParameters"] = &Warden::setFogSettings;

	// gui/2D images/text

	// sound
	sound["PlaySound2D"] = &Warden::play2DSound;
	sound["PlaySound3D"] = &Warden::play3DSound;
	sound["PlaySound2DOnChannel"] = &Warden::play2DSoundOnChannel;
	sound["PlaySound3DOnChannel"] = &Warden::play3DSoundOnChannel;
	sound["StopChannel"] = &Warden::stopChannel;
	sound["SetChannelPaused"] = &Warden::pauseChannel;
	sound["SetChannelLooped"] = &Warden::loopChannel;
	sound["StopAllChannels"] = &Warden::stopAllSounds;
	sound["PreloadSound"] = &Warden::preloadSound;
	sound["SetListenerPosition"] = &Warden::setListenerPosition;
	sound["ClearChannelEffects"] = &Warden::resetChannelFX;
	sound["SetChannelEffect"] = &Warden::setDistortionEffect;
	sound["SetChannelVolume"] = &Warden::setVolume;
	sound["SetChannelPitch"] = &Warden::setPitch;
	sound["SetChannelPan"] = &Warden::setPan;
	sound["GetChannelList"] = &Warden::printChannelList;

	// input
	input["IsKeyDown"] = &Warden::isKeyDown;
	input["GetMouseState"] = &Warden::getMouseState;
	input["GetControllerState"] = &Warden::getControllerState;
	input["CheckControllers"] = &Warden::isControllerConnected;
	input["SetMouseVisibility"] = &Warden::showCursor;
	input["SetMousePosition"] = &Warden::setCursorPosition;
}