#include "./audio_manager.hpp"

breakout::AudioManager::AudioManager(/* args */)
{
    this->soundEngine = irrklang::createIrrKlangDevice();
}

breakout::AudioManager::~AudioManager()
{
}

void breakout::AudioManager::PlaySound(const std::string &mediaFile, bool loop)
{
    this->soundEngine->play2D(mediaFile.c_str(), loop);
}