#pragma once

#include <irrlicht.h>
#include <sol/sol.hpp>
#include "LuaLime.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include <array>
#include <string>

#pragma comment(lib, "irrKlang.lib")

typedef unsigned int u32;

using namespace irrklang;

class SoundManager
{
private:
    static const int MAX_CHANNELS = 64;
    std::array<ISound*, MAX_CHANNELS> channels;

public:
    SoundManager();
    ~SoundManager();

    int play2DSound(const std::string& filePath, bool loop = false);
    int play3DSound(const std::string& filePath, Vector3D src, bool loop = false);
    void play2DSoundOnChannel(int channel, const std::string& filePath, bool loop = false);
    void play3DSoundOnChannel(int channel, const std::string& filePath, Vector3D src, bool loop = false);
    void stopChannel(int channel);
    void pauseChannel(int channel, bool paused);
    void loopChannel(int channel, bool loop);
    void stopAllSounds();
    void preloadSound();
    void setListenerPosition(Vector3D pos, Vector3D forward);
    bool isChannelFree(int channel);
    int getNextAvailableChannel();
    void resetChannelFX(int channel);
    void setPitch(int channel, float pitch);
    void setVolume(int channel, float volume);
    void setPan(int channel, float pan);
    void setDistortionEffect(int channel, int effect, bool enable, sol::table params);
    int validChannel(int i);
    std::string printChannelList();
};
