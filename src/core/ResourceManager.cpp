#include "ResourceManager.hpp"
#include <filesystem>
#include <iostream>

namespace core {

ResourceManager::ResourceManager() = default;

ResourceManager::~ResourceManager() = default;

bool ResourceManager::loadResources(const std::string& configPath) {
    try {
        auto config = toml::parse_file(configPath);

        // Load audio resources
        if (auto audioTable = config["audio"].as_table()) {
            if (!loadSounds(*audioTable) || !loadMusic(*audioTable)) {
                return false;
            }
        }

        return true;
    } catch (const toml::parse_error& err) {
        std::cerr << "Failed to parse resources config: " << err.description() << std::endl;
        return false;
    }
}

bool ResourceManager::loadSounds(const toml::table& config) {
    if (auto soundsTable = config["sounds"].as_table()) {
        for (const auto& [key, value] : *soundsTable) {
            if (auto path = value.as_string()) {
                auto buffer = std::make_unique<sf::SoundBuffer>();
                if (!buffer->loadFromFile(path->get())) {
                    std::cerr << "Failed to load sound: " << path->get() << std::endl;
                    return false;
                }
                soundBuffers[std::string(key.str())] = std::move(buffer);
            }
        }
    }
    return true;
}

bool ResourceManager::loadMusic(const toml::table& config) {
    if (auto musicTable = config["music"].as_table()) {
        for (const auto& [key, value] : *musicTable) {
            if (auto path = value.as_string()) {
                auto music = std::make_unique<sf::Music>();
                if (!music->openFromFile(path->get())) {
                    std::cerr << "Failed to load music: " << path->get() << std::endl;
                    return false;
                }
                this->music[std::string(key.str())] = std::move(music);
            }
        }
    }
    return true;
}

const sf::SoundBuffer* ResourceManager::getSoundBuffer(const std::string& name) const {
    auto it = soundBuffers.find(name);
    return it != soundBuffers.end() ? it->second.get() : nullptr;
}

sf::Music* ResourceManager::getMusic(const std::string& name) {
    auto it = music.find(name);
    return it != music.end() ? it->second.get() : nullptr;
}

bool ResourceManager::fileExists(const std::string& path) const {
    return std::filesystem::exists(path);
}

} // namespace core
