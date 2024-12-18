#include "ResourceManager.hpp"
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>

namespace game {

ResourceManager::ResourceManager() {
    if (!loadResources()) {
        spdlog::error("Failed to load game resources!");
    } else {
        spdlog::info("Game resources loaded successfully");
    }
}

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

        // Load fonts
        if (auto fontsTable = config["fonts"].as_table()) {
            if (!loadFonts(*fontsTable)) {
                return false;
            }
        }

        return true;
    } catch (const toml::parse_error& err) {
        spdlog::error("Failed to parse resources config: {}", err.description());
        return false;
    }
}

bool ResourceManager::loadSounds(const toml::table& config) {
    if (auto soundsTable = config["sounds"].as_table()) {
        for (const auto& [key, value] : *soundsTable) {
            if (auto path = value.as_string()) {
                auto buffer = std::make_unique<sf::SoundBuffer>();
                if (!buffer->loadFromFile("assets/" + path->get())) {
                    spdlog::error("Failed to load sound: {}", path->get());
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
                if (!music->openFromFile("assets/" + path->get())) {
                    spdlog::error("Failed to load music: {}", path->get());
                    return false;
                }
                this->music[std::string(key.str())] = std::move(music);
            }
        }
    }
    return true;
}

bool ResourceManager::loadFonts(const toml::table& config) {
    for (const auto& [key, value] : config) {
        if (auto path = value.as_string()) {
            auto font = std::make_unique<sf::Font>();
            if (!font->loadFromFile(std::format("{}{}", "assets/", path->get()))) {
                spdlog::error("Failed to load font: {}", path->get());
                return false;
            }
            fonts[std::string(key.str())] = std::move(font);
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

const sf::Font* ResourceManager::getFont(const std::string& name) const {
    auto it = fonts.find(name);
    return it != fonts.end() ? it->second.get() : nullptr;
}

bool ResourceManager::fileExists(const std::string& path) const {
    return std::filesystem::exists(path);
}

} // namespace core
