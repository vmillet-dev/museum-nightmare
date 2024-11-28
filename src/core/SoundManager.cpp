#include "SoundManager.hpp"
#include <spdlog/spdlog.h>

namespace core {

SoundManager::SoundManager(ResourceManager& resourceManager)
    : resourceManager(resourceManager) {}

void SoundManager::playSound(const std::string& name) {
    if (const sf::SoundBuffer* buffer = resourceManager.getSoundBuffer(name)) {
        auto sound = std::make_unique<sf::Sound>(*buffer);
        sound->play();
        activeSounds[name] = std::move(sound);
    } else {
        spdlog::error("Sound not found: {}", name);
    }
}

void SoundManager::stopSound(const std::string& name) {
    if (auto it = activeSounds.find(name); it != activeSounds.end()) {
        it->second->stop();
        activeSounds.erase(it);
    }
}

void SoundManager::setSoundVolume(const std::string& name, float volume) {
    if (auto it = activeSounds.find(name); it != activeSounds.end()) {
        it->second->setVolume(volume);
    }
}

void SoundManager::stopAllSounds() {
    for (auto& [name, sound] : activeSounds) {
        sound->stop();
    }
    activeSounds.clear();
}

void SoundManager::playMusic(const std::string& name, bool loop) {
    if (sf::Music* music = resourceManager.getMusic(name)) {
        music->setLoop(loop);
        music->play();
        activeMusic[name] = music;
    } else {
        spdlog::error("Music not found: {}", name);
    }
}

void SoundManager::pauseMusic(const std::string& name) {
    if (auto it = activeMusic.find(name); it != activeMusic.end()) {
        it->second->pause();
    }
}

void SoundManager::stopMusic(const std::string& name) {
    if (auto it = activeMusic.find(name); it != activeMusic.end()) {
        it->second->stop();
        activeMusic.erase(it);
    }
}

void SoundManager::setMusicVolume(const std::string& name, float volume) {
    if (auto it = activeMusic.find(name); it != activeMusic.end()) {
        it->second->setVolume(volume);
    }
}

void SoundManager::stopAllMusic() {
    for (auto& [name, music] : activeMusic) {
        music->stop();
    }
    activeMusic.clear();
}

} // namespace core
