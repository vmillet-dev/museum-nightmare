#pragma once
#include <toml++/toml.h>
#include <string>
#include <spdlog/spdlog.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>
#include <vector>
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

    // Window settings
    void setWindowWidth(int width);
    void setWindowHeight(int height);
    void setWindowResolution(int width, int height);

    // Input binding methods
    toml::v3::array* getKeyboardBindingsFromAction(const std::string& action);
    toml::v3::array* getMouseBindingsFromAction(const std::string& action);
    toml::v3::array* getControllerBindingsFromAction(const std::string& action);

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
