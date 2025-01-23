#include "Sound.h"
#include "IrrManagers.h"

SoundManager::SoundManager()
{
    for (int i = 0; i < MAX_CHANNELS; ++i)
    {
        channels[i] = nullptr;
    }
}

SoundManager::~SoundManager()
{
    //stopAllSounds();
}

int SoundManager::play2DSound(const std::string& filePath, bool loop)
{
    int channel = getNextAvailableChannel();
    play2DSoundOnChannel(channel, filePath, loop);
    return channel;
}

int SoundManager::play3DSound(const std::string& filePath, Vector3D src, bool loop)
{
    int channel = getNextAvailableChannel();
    play3DSoundOnChannel(channel, filePath, src, loop);
    return channel;
}

void SoundManager::play2DSoundOnChannel(int channel, const std::string& filePath, bool loop)
{
    channel = validChannel(channel);
    stopChannel(channel);
    channels[channel] = sound->play2D(filePath.c_str(), loop, false, true);
}

void SoundManager::play3DSoundOnChannel(int channel, const std::string& filePath, Vector3D src, bool loop)
{
    channel = validChannel(channel);
    stopChannel(channel);
    channels[channel] = sound->play3D(filePath.c_str(), vec3df(static_cast<f32>(src.x), static_cast<f32>(src.y), static_cast<f32>(src.z)), loop, false, true);
}

void SoundManager::stopChannel(int channel)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->stop();
        channels[channel] = nullptr;
    }
}

void SoundManager::pauseChannel(int channel, bool paused)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setIsPaused(paused);
    }
}

void SoundManager::loopChannel(int channel, bool loop)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setIsLooped(loop);
    }
}

void SoundManager::stopAllSounds()
{
    for (int i = 0; i < MAX_CHANNELS; ++i)
    {
        stopChannel(i);
    }
}

bool SoundManager::preloadSound(std::string path)
{
    ISoundSource* s = sound->addSoundSourceFromFile(path.c_str());
    return s != nullptr;
}

void SoundManager::setDopplerParameters(float dopFactor, float distFactor) {
    sound->setDopplerEffectParameters(dopFactor, distFactor);
}

void SoundManager::SetChannelVelocity(int channel, const Vector3D& velocity) {
    channel = validChannel(channel);
    if (channels[channel]) {
        channels[channel]->setVelocity(vec3df(static_cast<f32>(velocity.x), static_cast<f32>(velocity.y), static_cast<f32>(velocity.z)));
    }
}

void SoundManager::setChannelPosition3D(int channel, const Vector3D& pos) {
    channel = validChannel(channel);
    if (channels[channel]) {
        channels[channel]->setPosition(vec3df(static_cast<f32>(pos.x), static_cast<f32>(pos.y), static_cast<f32>(pos.z)));
    }
}

void SoundManager::setListenerPosition(const Vector3D& pos, const Vector3D& forward)
{
    sound->setListenerPosition(vec3df(static_cast<f32>(pos.x), static_cast<f32>(pos.y), static_cast<f32>(pos.z)),
        vec3df(static_cast<f32>(forward.x), static_cast<f32>(forward.y), static_cast<f32>(forward.z)));
}

bool SoundManager::isChannelFree(int channel)
{
    channel = validChannel(channel);
    if (!channels[channel])
        return true;
    if (channels[channel]->isFinished())
        return true;
    return false;
}

int SoundManager::getNextAvailableChannel()
{
    for (int i = 0; i < MAX_CHANNELS; ++i)
    {
        if (isChannelFree(i))
        {
            return i;
        }
    }
    return MAX_CHANNELS - 1; // Default to last channel
}

void SoundManager::resetChannelFX(int channel)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setPlaybackSpeed(1.0f);
        channels[channel]->setVolume(1.0f);
        channels[channel]->setPan(0.0f);
    }
}

void SoundManager::setPitch(int channel, float pitch)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setPlaybackSpeed(pitch);
    }
}

void SoundManager::setVolume(int channel, float volume)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setVolume(volume);
    }
}

void SoundManager::setPlaybackSpeed(int channel, float spd)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setPlaybackSpeed(spd);
    }
}

void SoundManager::setPlayPosition(int channel, int time)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setPlayPosition(time);
    }
}

void SoundManager::setPan(int channel, float pan)
{
    channel = validChannel(channel);
    if (channels[channel])
    {
        channels[channel]->setPan(pan);
    }
}

void SoundManager::setDistortionEffect(int channel, int effect, bool enable, sol::table params)
{
    ISoundEffectControl* fx = 0;
    channel = validChannel(channel);
    if (channels[channel])
    {
        fx = channels[channel]->getSoundEffectControl();
        if (!fx)
        {
            return;
        }
        if (effect < 0) effect = 0;
        if (effect > 2) effect = 2;

        switch (effect)
        {
        case 0:
            if (enable)
            {
                u32 gain = -18.0f;
                u32 edge = 15.0f;
                if (params["gain"].valid()) gain = params["gain"];
                if (params["edge"].valid()) edge = params["edge"];
                fx->enableDistortionSoundEffect(gain, edge);
            }
            else
            {
                fx->disableDistortionSoundEffect();
            }
            break;

        case 1:
            if (enable)
            {
                u32 wetDry = 50.0f;
                u32 feedback = 50.0f;
                u32 delay = 500.0f;
                if (params["wetDry"].valid()) wetDry = params["wetDry"];
                if (params["feedback"].valid()) feedback = params["feedback"];
                if (params["delay"].valid()) delay = params["delay"];
                fx->enableEchoSoundEffect(wetDry, feedback, delay, delay, 0);
            }
            else
            {
                fx->disableEchoSoundEffect();
            }
            break;

        case 2:
            if (enable)
            {
                u32 inputGain = 0.0f;
                u32 reverbMix = 0.0f;
                u32 reverbTime = 1000.0f;
                u32 frequencyRatio = 0.001;
                if (params["inputGain"].valid()) inputGain = params["inputGain"];
                if (params["reverbMix"].valid()) reverbMix = params["reverbMix"];
                if (params["reverbTime"].valid()) reverbTime = params["reverbTime"];
                if (params["frequencyRatio"].valid()) frequencyRatio = params["frequencyRatio"];
                fx->enableWavesReverbSoundEffect(inputGain, reverbMix, reverbTime, frequencyRatio);
            }
            else
            {
                fx->disableWavesReverbSoundEffect();
            }
            break;
        }
    }
}

int SoundManager::validChannel(int i)
{
    if (i < 0) return 0;
    if (i >= MAX_CHANNELS) return MAX_CHANNELS - 1;
    return i;
}

std::string SoundManager::printChannelList()
{
    std::string result;
    for (int i = 0; i < MAX_CHANNELS; ++i)
    {
        std::string status = std::to_string(i) + ": ";
        if (channels[i] && !channels[i]->isFinished())
        {
            status += channels[i]->getSoundSource()->getName();
        }
        result += status + "\n"; // Append to the result string with a newline
    }
    return result;
}
