#include "ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

namespace game {

ConfigManager::ConfigManager() {
    spdlog::debug("Input config: KB(ZQSD), Ctrl({})", isControllerEnabled());
    loadConfig();
}

void ConfigManager::loadConfig() {
    try {
        config = toml::parse_file("assets/config.toml");
        spdlog::info("Config loaded - Controller(enabled:{},deadzone:{:.1f},sens:{:.1f})", isControllerEnabled(), getControllerDeadzone(), getControllerSensitivity());

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
    spdlog::info("Creating default config with controller and audio settings");
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
        }},
        {"controls", toml::table{
            {"move_up", "Z"},
            {"move_down", "S"},
            {"move_left", "Q"},
            {"move_right", "D"},
            {"pause", "Escape"},
            {"confirm", "Return"},
            {"cancel", "BackSpace"},
            {"controller_enabled", true},
            {"controller_deadzone", 20.0},
            {"controller_sensitivity", 100.0},
            {"controller_move_up", 0},
            {"controller_move_down", 1},
            {"controller_move_left", 2},
            {"controller_move_right", 3},
            {"controller_pause", 7},
            {"controller_confirm", 4},
            {"controller_cancel", 5}
        }},
        {"audio", toml::table{
            {"master_volume", 100.0},
            {"music_volume", 80.0},
            {"sound_volume", 100.0},
            {"sounds", toml::table{
                {"player_move", toml::table{
                    {"path", "assets/audio/sfx/player_move.ogg"},
                    {"volume", 100.0},
                    {"min_distance", 1.0},
                    {"attenuation", 1.0}
                }},
                {"player_collision", toml::table{
                    {"path", "assets/audio/sfx/player_collision.ogg"},
                    {"volume", 100.0},
                    {"min_distance", 1.0},
                    {"attenuation", 1.0}
                }},
                {"enemy_collision", toml::table{
                    {"path", "assets/audio/sfx/enemy_collision.ogg"},
                    {"volume", 100.0},
                    {"min_distance", 1.0},
                    {"attenuation", 1.0}
                }}
            }},
            {"music", toml::table{
                {"main_theme", toml::table{
                    {"path", "assets/audio/music/main_theme.ogg"},
                    {"volume", 80.0},
                    {"looping", true}
                }},
                {"battle_theme", toml::table{
                    {"path", "assets/audio/music/battle_theme.ogg"},
                    {"volume", 80.0},
                    {"looping", true}
                }}
            }}
        }}
    };
    saveConfig();
}

void ConfigManager::saveConfig() {
    std::ofstream file("assets/config.toml");
    file << config;
    spdlog::info("Config saved with controller settings: enabled={}, deadzone={:.1f}, sensitivity={:.1f}", isControllerEnabled(), getControllerDeadzone(), getControllerSensitivity());
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

sf::Keyboard::Key ConfigManager::getKeyBinding(const std::string& action) const {
    std::string keyName = config["controls"][action].value_or("");
    auto key = KeyMapper::getInstance().fromName(keyName);
    spdlog::debug("Key binding: {} -> {}", action, keyName);
    return key;
}

bool ConfigManager::isControllerEnabled() const {
    bool enabled = config["controls"]["controller_enabled"].value_or(true);
    spdlog::debug("Controller enabled: {}", enabled);
    return enabled;
}

float ConfigManager::getControllerDeadzone() const {
    float deadzone = config["controls"]["controller_deadzone"].value_or(20.0f);
    spdlog::debug("Controller deadzone: {:.1f}", deadzone);
    return deadzone;
}

float ConfigManager::getControllerSensitivity() const {
    float sensitivity = config["controls"]["controller_sensitivity"].value_or(100.0f);
    spdlog::debug("Controller sensitivity: {:.1f}", sensitivity);
    return sensitivity;
}

unsigned int ConfigManager::getControllerButton(const std::string& action) const {
    unsigned int button = static_cast<unsigned int>(config["controls"][action].value_or(0));
    spdlog::debug("Controller button for {}: {}", action, button);
    return button;
}

float ConfigManager::getMasterVolume() const {
    return config["audio"]["master_volume"].value_or(100.0f);
}

float ConfigManager::getMusicVolume() const {
    return config["audio"]["music_volume"].value_or(80.0f);
}

float ConfigManager::getSoundVolume() const {
    return config["audio"]["sound_volume"].value_or(100.0f);
}

std::optional<ConfigManager::SoundResource> ConfigManager::getSoundResource(const std::string& id) const {
    auto sounds = config["audio"]["sounds"];
    if (!sounds || !sounds.is_table() || !sounds[id]) {
        return std::nullopt;
    }

    auto sound = sounds[id];
    SoundResource resource;
    resource.filepath = sound["path"].value_or("");
    resource.volume = sound["volume"].value_or(100.0f);
    resource.minDistance = sound["min_distance"].value_or(1.0f);
    resource.attenuation = sound["attenuation"].value_or(1.0f);
    return resource;
}

std::optional<ConfigManager::MusicResource> ConfigManager::getMusicResource(const std::string& id) const {
    auto music = config["audio"]["music"];
    if (!music || !music.is_table() || !music[id]) {
        return std::nullopt;
    }

    auto track = music[id];
    MusicResource resource;
    resource.filepath = track["path"].value_or("");
    resource.volume = track["volume"].value_or(80.0f);
    resource.looping = track["looping"].value_or(true);
    return resource;
}

} // namespace game
