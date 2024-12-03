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

toml::v3::array* ConfigManager::getKeyboardBindingsForAction(const std::string& action) {
    try {
        return config["actions"][action]["keyboard"].as_array();
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get keyboard bindings for action {}: {}", action, e.what());
    }
}

toml::v3::array* ConfigManager::getMouseBindingsForAction(const std::string& action) {
    std::vector<sf::Mouse::Button> buttons;
    try {
        return config["actions"][action]["mouse"].as_array();
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get mouse bindings for action {}: {}", action, e.what());
    }
}

toml::v3::array* ConfigManager::getControllerBindingsForAction(const std::string& action) {
    std::vector<std::string> controls;
    try {
        return config["actions"][action]["controller"].as_array();
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get controller bindings for action {}: {}", action, e.what());
    }

}

float ConfigManager::getControllerDeadzone() const {
    return config["controller"]["deadzone"].value_or(20.0f);
}

float ConfigManager::getControllerSensitivity() const {
    return config["controller"]["sensitivity"].value_or(100.0f);
}

} // namespace game
