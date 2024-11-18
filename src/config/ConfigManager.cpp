#include "ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

namespace game {

ConfigManager::ConfigManager() {
    loadConfig();
}

void ConfigManager::loadConfig() {
    try {
        config = toml::parse_file("resources/config.toml");
        spdlog::info("Config file loaded successfully");

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
            {"size", 32.0}
        }},
        {"debug", toml::table{
            {"level", "info"}
        }}
    };

    saveConfig();
}

void ConfigManager::saveConfig() {
    std::ofstream file("resources/config.toml");
    file << config;
    spdlog::info("Config saved successfully");
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
    return config["player"]["size"].value_or(32.0f);
}

} // namespace game
