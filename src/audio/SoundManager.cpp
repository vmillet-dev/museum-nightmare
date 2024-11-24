#include "SoundManager.hpp"
#include <stdexcept>

namespace game {

SoundManager::SoundManager() {
    loadResources();
}

SoundManager::~SoundManager() {
    stopMusic();
    activeSounds.clear();
}

void SoundManager::loadResources() {
    const auto& config = ConfigManager::getInstance();

    // Load sound effects
    const std::pair<SoundId, std::string> soundMappings[] = {
        {SoundId::PlayerMove, "player_move"},
        {SoundId::PlayerCollision, "player_collision"},
        {SoundId::EnemyCollision, "enemy_collision"}
    };

    for (const auto& [id, configId] : soundMappings) {
        if (auto resource = config.getSoundResource(configId)) {
            sf::SoundBuffer buffer;
            if (!buffer.loadFromFile(resource->filepath)) {
                throw std::runtime_error("Failed to load sound: " + resource->filepath);
            }
            soundBuffers[id] = buffer;
        }
    }
}

void SoundManager::cleanupStoppedSounds() {
    activeSounds.remove_if([](const auto& sound) {
        return sound->getStatus() == sf::Sound::Stopped;
    });
}

void SoundManager::playSound(SoundId id) {
    cleanupStoppedSounds();

    if (activeSounds.size() >= MAX_SOUNDS) {
        return;
    }

    auto it = soundBuffers.find(id);
    if (it == soundBuffers.end()) return;

    const auto& config = ConfigManager::getInstance();
    const auto resource = config.getSoundResource(getSoundConfigId(id));
    if (!resource) return;

    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(it->second);
    sound->setVolume(resource->volume * config.getSoundVolume() * config.getMasterVolume() / 100.f);
    sound->setMinDistance(resource->minDistance);
    sound->setAttenuation(resource->attenuation);
    sound->play();
    activeSounds.push_back(std::move(sound));
}

void SoundManager::playMusic(MusicId id) {
    stopMusic();

    const auto& config = ConfigManager::getInstance();
    const auto resource = config.getMusicResource(getMusicConfigId(id));
    if (!resource) return;

    currentMusic = std::make_unique<sf::Music>();
    if (!currentMusic->openFromFile(resource->filepath)) {
        throw std::runtime_error("Failed to load music: " + resource->filepath);
    }

    currentMusic->setVolume(resource->volume * config.getMusicVolume() * config.getMasterVolume() / 100.f);
    currentMusic->setLoop(resource->looping);
    currentMusic->play();
    currentMusicId = id;
}

void SoundManager::stopMusic() {
    if (currentMusic) {
        currentMusic->stop();
        currentMusic.reset();
        currentMusicId.reset();
    }
}

void SoundManager::pauseMusic() {
    if (currentMusic) {
        currentMusic->pause();
    }
}

void SoundManager::resumeMusic() {
    if (currentMusic) {
        currentMusic->play();
    }
}

void SoundManager::setMasterVolume(float volume) {
    auto& config = ConfigManager::getInstance();
    config["audio"]["master_volume"] = volume;
    config.saveConfig();
    updateAllVolumes();
}

void SoundManager::setMusicVolume(float volume) {
    auto& config = ConfigManager::getInstance();
    config["audio"]["music_volume"] = volume;
    config.saveConfig();
    updateAllVolumes();
}

void SoundManager::setSoundVolume(float volume) {
    auto& config = ConfigManager::getInstance();
    config["audio"]["sound_volume"] = volume;
    config.saveConfig();
    updateAllVolumes();
}

std::string SoundManager::getSoundConfigId(SoundId id) const {
    switch (id) {
        case SoundId::PlayerMove: return "player_move";
        case SoundId::PlayerCollision: return "player_collision";
        case SoundId::EnemyCollision: return "enemy_collision";
        default: return "";
    }
}

std::string SoundManager::getMusicConfigId(MusicId id) const {
    switch (id) {
        case MusicId::MainTheme: return "main_theme";
        case MusicId::BattleTheme: return "battle_theme";
        default: return "";
    }
}

void SoundManager::updateAllVolumes() {
    const auto& config = ConfigManager::getInstance();

    // Update all active sounds
    for (const auto& sound : activeSounds) {
        for (const auto& [id, buffer] : soundBuffers) {
            if (sound->getBuffer() == &buffer) {
                if (auto resource = config.getSoundResource(getSoundConfigId(id))) {
                    sound->setVolume(resource->volume * config.getSoundVolume() * config.getMasterVolume() / 100.f);
                }
                break;
            }
        }
    }

    // Update current music
    if (currentMusic && currentMusicId) {
        if (auto resource = config.getMusicResource(getMusicConfigId(*currentMusicId))) {
            currentMusic->setVolume(resource->volume * config.getMusicVolume() * config.getMasterVolume() / 100.f);
        }
    }
}

} // namespace game
