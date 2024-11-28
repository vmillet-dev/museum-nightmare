#include "SoundManager.hpp"
#include <spdlog/spdlog.h>

namespace core {

SoundManager::SoundManager(ResourceManager& resourceManager)
    : resourceManager(resourceManager) {}

SoundManager::~SoundManager() {
    stopAllSounds();
    stopAllMusic();
}

std::string SoundManager::getSoundName(SoundId id) {
    switch (id) {
        case SoundId::Slide: return "slide";
        default: return "";
    }
}

std::string SoundManager::getMusicName(MusicId id) {
    switch (id) {
        case MusicId::MenuTheme: return "menu_theme";
        default: return "";
    }
}

void SoundManager::playSound(SoundId id) {
    const std::string name = getSoundName(id);
    if (const sf::SoundBuffer* buffer = resourceManager.getSoundBuffer(name)) {
        auto sound = std::make_unique<sf::Sound>(*buffer);
        sound->play();
        activeSounds[id] = std::move(sound);
    } else {
        spdlog::error("Sound not found: {}", name);
    }
}

void SoundManager::stopSound(SoundId id) {
    if (auto it = activeSounds.find(id); it != activeSounds.end()) {
        it->second->stop();
        activeSounds.erase(it);
    }
}

void SoundManager::setSoundVolume(SoundId id, float volume) {
    if (auto it = activeSounds.find(id); it != activeSounds.end()) {
        it->second->setVolume(volume);
    }
}

void SoundManager::stopAllSounds() {
    for (auto& [id, sound] : activeSounds) {
        sound->stop();
    }
    activeSounds.clear();
}

void SoundManager::playMusic(MusicId id, bool loop) {
    const std::string name = getMusicName(id);
    if (sf::Music* music = resourceManager.getMusic(name)) {
        music->setLoop(loop);
        music->play();
        activeMusic[id] = music;
    } else {
        spdlog::error("Music not found: {}", name);
    }
}

void SoundManager::pauseMusic(MusicId id) {
    if (auto it = activeMusic.find(id); it != activeMusic.end()) {
        it->second->pause();
    }
}

void SoundManager::stopMusic(MusicId id) {
    if (auto it = activeMusic.find(id); it != activeMusic.end()) {
        it->second->stop();
        activeMusic.erase(it);
    }
}

void SoundManager::setMusicVolume(MusicId id, float volume) {
    if (auto it = activeMusic.find(id); it != activeMusic.end()) {
        it->second->setVolume(volume);
    }
}

void SoundManager::stopAllMusic() {
    for (auto& [id, music] : activeMusic) {
        music->stop();
    }
    activeMusic.clear();
}

} // namespace core
