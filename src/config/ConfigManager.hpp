#pragma once
#include <toml++/toml.h>
#include <string>
#include <spdlog/spdlog.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
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
    sf::Joystick::Axis getControllerAxis(const std::string& action) const;

    // New configuration methods
    void setKeyBinding(const std::string& action, sf::Keyboard::Key key);
    void setControllerButton(const std::string& action, unsigned int button);
    void setControllerAxis(const std::string& action, sf::Joystick::Axis axis);
    void setControllerDeadzone(float value);
    void setControllerSensitivity(float value);
    void setControllerEnabled(bool enabled);

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    sf::Keyboard::Key stringToKey(const std::string& keyName) const;
    std::string keyToString(sf::Keyboard::Key key) const;
    sf::Joystick::Axis stringToAxis(const std::string& axisName) const;
    std::string axisToString(sf::Joystick::Axis axis) const;

    toml::table config;
    std::unordered_map<std::string, sf::Keyboard::Key> keyNameMap;
    std::unordered_map<std::string, sf::Joystick::Axis> axisNameMap;
};

} // namespace game
