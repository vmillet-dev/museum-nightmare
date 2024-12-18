#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <toml++/toml.h>

namespace game {

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Load all resources from config file
    bool loadResources(const std::string& configPath = "assets/resources.toml");

    // Audio getters
    const sf::SoundBuffer* getSoundBuffer(const std::string& name) const;
    sf::Music* getMusic(const std::string& name);

    // Font getter
    const sf::Font* getFont(const std::string& name) const;

private:
    // Load specific resource types
    bool loadSounds(const toml::table& config);
    bool loadMusic(const toml::table& config);
    bool loadFonts(const toml::table& config);

    // Resource storage
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;

    // Helper functions
    bool fileExists(const std::string& path) const;
};

} // namespace core
