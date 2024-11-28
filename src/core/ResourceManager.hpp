#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <toml++/toml.h>

namespace core {

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Load all resources from config file
    bool loadResources(const std::string& configPath = "assets/resources.toml");

    // Audio getters
    const sf::SoundBuffer* getSoundBuffer(const std::string& name) const;
    const sf::Music* getMusic(const std::string& name) const;

    // Future resource getters can be added here

private:
    // Load specific resource types
    bool loadSounds(const toml::table& config);
    bool loadMusic(const toml::table& config);

    // Resource storage
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;

    // Helper functions
    bool fileExists(const std::string& path) const;
};

} // namespace core
