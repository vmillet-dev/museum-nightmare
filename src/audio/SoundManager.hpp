#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <list>
#include <optional>
#include "../config/ConfigManager.hpp"
#include "AudioTypes.hpp"

namespace game {

class SoundManager {
public:
    explicit SoundManager();
    ~SoundManager();

    // Non-copyable
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    // Audio control
    void playSound(SoundId id);
    void playMusic(MusicId id);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();

    // Volume control
    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);

private:
    void loadResources();
    void cleanupStoppedSounds();
    void updateAllVolumes();
    std::string getSoundConfigId(SoundId id) const;
    std::string getMusicConfigId(MusicId id) const;

    std::unordered_map<SoundId, sf::SoundBuffer> soundBuffers;
    std::list<std::unique_ptr<sf::Sound>> activeSounds;
    std::unique_ptr<sf::Music> currentMusic;
    std::optional<MusicId> currentMusicId;  // Track current music ID

    static constexpr size_t MAX_SOUNDS = 256;
};

} // namespace game
