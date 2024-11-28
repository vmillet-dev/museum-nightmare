#pragma once

#include "ResourceManager.hpp"
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace core {

class SoundManager {
public:
    explicit SoundManager(ResourceManager& resourceManager);
    ~SoundManager() = default;

    // Sound effects
    void playSound(const std::string& name);
    void stopSound(const std::string& name);
    void setSoundVolume(const std::string& name, float volume);
    void stopAllSounds();

    // Background music
    void playMusic(const std::string& name, bool loop = true);
    void pauseMusic(const std::string& name);
    void stopMusic(const std::string& name);
    void setMusicVolume(const std::string& name, float volume);
    void stopAllMusic();

private:
    ResourceManager& resourceManager;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> activeSounds;
    std::unordered_map<std::string, sf::Music*> activeMusic;  // Removed const to allow modifications
};

} // namespace core
