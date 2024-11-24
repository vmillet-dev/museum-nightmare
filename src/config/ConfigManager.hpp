#pragma once
#include <toml++/toml.h>
#include <string>
#include <spdlog/spdlog.h>
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>
#include <optional>
#include "KeyMapper.hpp"

namespace game {

class ConfigManager {
public:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    void loadConfig();
    void createDefaultConfig();
    void saveConfig();

    int getWindowWidth() const;
    int getWindowHeight() const;
    std::string getWindowTitle() const;
    float getPlayerSpeed() const;
    float getPlayerSize() const;

    // New control settings methods
    sf::Keyboard::Key getKeyBinding(const std::string& action) const;
    bool isControllerEnabled() const;
    float getControllerDeadzone() const;
    float getControllerSensitivity() const;
    unsigned int getControllerButton(const std::string& action) const;

    // Audio configuration methods
    float getMasterVolume() const;
    float getMusicVolume() const;
    float getSoundVolume() const;
    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);

    struct SoundResource {
        std::string filepath;
        float volume;
        float minDistance;
        float attenuation;
    };

    struct MusicResource {
        std::string filepath;
        float volume;
        bool looping;
    };

    std::optional<SoundResource> getSoundResource(const std::string& id) const;
    std::optional<MusicResource> getMusicResource(const std::string& id) const;

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    toml::table config;
};

} // namespace game
