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
    spdlog::info("Config saved with window dimensions: {}x{}",
        config["window"]["width"].value_or(800),
        config["window"]["height"].value_or(600));
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

void ConfigManager::setWindowWidth(int width) {
    if (!config.contains("window")) {
        config.insert_or_assign("window", toml::table{});
    }
    auto& window_table = *config.get("window")->as_table();
    window_table.insert_or_assign("width", width);
    spdlog::debug("Window width updated in config: {}", width);
}

void ConfigManager::setWindowHeight(int height) {
    if (!config.contains("window")) {
        config.insert_or_assign("window", toml::table{});
    }
    auto& window_table = *config.get("window")->as_table();
    window_table.insert_or_assign("height", height);
    spdlog::debug("Window height updated in config: {}", height);
}

void ConfigManager::setWindowResolution(int width, int height) {
    setWindowWidth(width);
    setWindowHeight(height);
    saveConfig();
    spdlog::info("Window resolution set to: {}x{}", width, height);
}

toml::v3::array* ConfigManager::getKeyboardBindingsFromAction(const std::string& action) {

    try {
        return config["actions"][action]["keyboard"].as_array();
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get keyboard bindings for action {}: {}", action, e.what());
    }
}

toml::v3::array* ConfigManager::getMouseBindingsFromAction(const std::string& action) {
    try {
        return config["actions"][action]["mouse"].as_array();
    } catch (const std::exception& e) {
        spdlog::warn("Failed to get mouse bindings for action {}: {}", action, e.what());
    }
}

toml::v3::array* ConfigManager::getControllerBindingsFromAction(const std::string& action) {
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
