#pragma once
#include <toml++/toml.h>
#include <string>
#include <spdlog/spdlog.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>
#include <vector>
#include "KeyMapper.hpp"
#include "MouseMapper.hpp"
#include "../input/Action.hpp"

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

    // Input binding methods
    std::vector<sf::Keyboard::Key> getKeyboardBindingsForAction(const std::string& action) const;
    std::vector<sf::Mouse::Button> getMouseBindingsForAction(const std::string& action) const;
    std::vector<std::string> getControllerBindingsForAction(const std::string& action) const;
    Action getActionFromString(const std::string& actionStr) const;

    // Legacy methods for backward compatibility
    sf::Keyboard::Key getKeyBinding(const std::string& action) const;
    sf::Mouse::Button getMouseButton(const std::string& action) const;
    unsigned int getControllerButton(const std::string& action) const;
    std::string getControllerAxis(const std::string& action) const;

    // Controller settings
    bool isControllerEnabled() const { return config["controller"]["enabled"].value_or(false); }
    float getControllerDeadzone() const;
    float getControllerSensitivity() const;

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    toml::table config;
};

} // namespace game
