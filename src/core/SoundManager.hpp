#pragma once

#include "ResourceManager.hpp"
#include "SoundId.hpp"
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace core {

class SoundManager {
public:
    explicit SoundManager(ResourceManager& resourceManager);
    ~SoundManager();  // Custom destructor to clean up resources

    // Sound effects
    void playSound(SoundId id);
    void stopSound(SoundId id);
    void setSoundVolume(SoundId id, float volume);
    void stopAllSounds();

    // Background music
    void playMusic(MusicId id, bool loop = true);
    void pauseMusic(MusicId id);
    void stopMusic(MusicId id);
    void setMusicVolume(MusicId id, float volume);
    void stopAllMusic();

private:
    // Convert enums to resource names
    static std::string getSoundName(SoundId id);
    static std::string getMusicName(MusicId id);

    ResourceManager& resourceManager;
    std::unordered_map<SoundId, std::unique_ptr<sf::Sound>> activeSounds;
    std::unordered_map<MusicId, sf::Music*> activeMusic;
};

} // namespace core
