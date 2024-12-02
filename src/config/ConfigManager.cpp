#include "ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include "../core/const.hpp"

namespace game {

ConfigManager::ConfigManager() {
    spdlog::debug("Input config: KB(ZQSD), Ctrl({})", isControllerEnabled());
    loadConfig();
}

void ConfigManager::loadConfig() {
    try {
        config = toml::parse_file("assets/config.toml");
        spdlog::info("Config loaded");

        // Set up logging level from config
        std::string logLevel = config["debug"]["level"].value_or("info");
        if (logLevel == "debug") {
            spdlog::set_level(spdlog::level::debug);
        } else if (logLevel == "info") {
            spdlog::set_level(spdlog::level::info);
        } else if (logLevel == "warn") {
            spdlog::set_level(spdlog::level::warn);
        } else if (logLevel == "error") {
            spdlog::set_level(spdlog::level::err);
        }
    } catch (const toml::parse_error& err) {
        spdlog::error("Failed to parse config file: {}", err.description());
        // Create default config
        createDefaultConfig();
    }
}

void ConfigManager::createDefaultConfig() {
    spdlog::info("Creating default config");
    config = toml::table{
        {"window", toml::table{
            {"width", 800},
            {"height", 600},
            {"title", "SFML Game"}
        }},
        {"player", toml::table{
            {"speed", 200.0},
            {"size", Constants::Physics::ACTOR_SIZE * 2}
        }},
        {"debug", toml::table{
            {"level", "info"}
        }},
        {"actions", toml::table{
            {"MoveUp", toml::table{
                {"keyboard", toml::array{"Z", "Up"}},
                {"mouse", toml::array{}},
                {"controller", toml::array{"LeftStickUp"}}
            }},
            {"MoveDown", toml::table{
                {"keyboard", toml::array{"S", "Down"}},
                {"mouse", toml::array{}},
                {"controller", toml::array{"LeftStickDown"}}
            }},
            {"MoveLeft", toml::table{
                {"keyboard", toml::array{"Q", "Left"}},
                {"mouse", toml::array{}},
                {"controller", toml::array{"LeftStickLeft"}}
            }},
            {"MoveRight", toml::table{
                {"keyboard", toml::array{"D", "Right"}},
                {"mouse", toml::array{}},
                {"controller", toml::array{"LeftStickRight"}}
            }}
        }}
    };
    saveConfig();
}

void ConfigManager::saveConfig() {
    std::ofstream file("assets/config.toml");
    file << config;
    spdlog::info("Config saved");
}

int ConfigManager::getWindowWidth() const {
    return config["window"]["width"].value_or(800);
}

int ConfigManager::getWindowHeight() const {
    return config["window"]["height"].value_or(600);
}

std::string ConfigManager::getWindowTitle() const {
    return config["window"]["title"].value_or("SFML Game");
}

float ConfigManager::getPlayerSpeed() const {
    return config["player"]["speed"].value_or(200.0f);
}

float ConfigManager::getPlayerSize() const {
    return config["player"]["size"].value_or(Constants::Physics::ACTOR_SIZE * 2);
}

std::vector<sf::Keyboard::Key> ConfigManager::getKeyboardBindingsForAction(const std::string& action) const {
    std::vector<sf::Keyboard::Key> keys;
    try {
        const auto& keyArray = config["actions"][action]["keyboard"].as_array();
        for (const auto& key : *keyArray) {
            auto sfKey = KeyMapper::getInstance().fromName(key.value_or(""));
            if (sfKey != sf::Keyboard::Unknown) {
                keys.push_back(sfKey);
            }
        }
        spdlog::debug("Keyboard bindings for {}: {} keys", action, keys.size());
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get keyboard bindings for action {}: {}", action, e.what());
    }
    return keys;
}

std::vector<sf::Mouse::Button> ConfigManager::getMouseBindingsForAction(const std::string& action) const {
    std::vector<sf::Mouse::Button> buttons;
    try {
        const auto& buttonArray = config["actions"][action]["mouse"].as_array();
        for (const auto& button : *buttonArray) {
            auto sfButton = MouseMapper::getInstance().stringToButton(button.value_or(""));
            if (sfButton != sf::Mouse::Button::ButtonCount) {
                buttons.push_back(sfButton);
            }
        }
        spdlog::debug("Mouse bindings for {}: {} buttons", action, buttons.size());
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get mouse bindings for action {}: {}", action, e.what());
    }
    return buttons;
}

std::vector<std::string> ConfigManager::getControllerBindingsForAction(const std::string& action) const {
    std::vector<std::string> controls;
    try {
        const auto& controlArray = config["actions"][action]["controller"].as_array();
        for (const auto& control : *controlArray) {
            std::string controlStr = control.value_or("");
            if (!controlStr.empty()) {
                controls.push_back(controlStr);
            }
        }
        spdlog::debug("Controller bindings for {}: {} controls", action, controls.size());
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get controller bindings for action {}: {}", action, e.what());
    }
    return controls;
}

Action ConfigManager::getActionFromString(const std::string& actionStr) const {
    static const std::unordered_map<std::string, Action> actionMap = {
        {"MoveUp", Action::MoveUp},
        {"MoveDown", Action::MoveDown},
        {"MoveLeft", Action::MoveLeft},
        {"MoveRight", Action::MoveRight},
        {"Pause", Action::Pause},
        {"Confirm", Action::Confirm},
        {"Cancel", Action::Cancel},
        {"Fire", Action::Fire}
    };

    auto it = actionMap.find(actionStr);
    if (it != actionMap.end()) {
        return it->second;
    }
    spdlog::warn("Unknown action string: {}", actionStr);
    return Action::MoveUp; // Default action, could be changed to an Invalid action if needed
}

std::unordered_map<std::string, Action> ConfigManager::getActionMap() const {
    static const std::unordered_map<std::string, Action> actionMap = {
        {"MoveUp", Action::MoveUp},
        {"MoveDown", Action::MoveDown},
        {"MoveLeft", Action::MoveLeft},
        {"MoveRight", Action::MoveRight},
        {"Pause", Action::Pause},
        {"Confirm", Action::Confirm},
        {"Cancel", Action::Cancel},
        {"Fire", Action::Fire}
    };
    return actionMap;
}

float ConfigManager::getControllerDeadzone() const {
    return config["controller"]["deadzone"].value_or(20.0f);
}

float ConfigManager::getControllerSensitivity() const {
    return config["controller"]["sensitivity"].value_or(100.0f);
}

sf::Keyboard::Key ConfigManager::getKeyBinding(const std::string& action) const {
    auto keys = getKeyboardBindingsForAction(action);
    return keys.empty() ? sf::Keyboard::Unknown : keys[0];
}

sf::Mouse::Button ConfigManager::getMouseButton(const std::string& action) const {
    auto buttons = getMouseBindingsForAction(action);
    return buttons.empty() ? sf::Mouse::ButtonCount : buttons[0];
}

unsigned int ConfigManager::getControllerButton(const std::string& action) const {
    auto controls = getControllerBindingsForAction(action);
    if (controls.empty()) return 0;
    try {
        return ControllerMapper::mapButtonName(controls[0]);
    } catch (const std::exception& e) {
        spdlog::warn("Failed to map controller button for action {}: {}", action, e.what());
        return 0;
    }
}

std::string ConfigManager::getControllerAxis(const std::string& action) const {
    auto controls = getControllerBindingsForAction(action);
    return controls.empty() ? "" : controls[0];
}

} // namespace game
