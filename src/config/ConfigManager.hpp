#pragma once
#include <toml++/toml.h>
#include <string>
#include <spdlog/spdlog.h>
#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

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

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    sf::Keyboard::Key stringToKey(const std::string& keyName) const;
    toml::table config;
    std::unordered_map<std::string, sf::Keyboard::Key> keyNameMap;
};

} // namespace game
