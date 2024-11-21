#include "ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <fstream>

namespace game {

ConfigManager::ConfigManager() {
    // Initialize key name mapping
    keyNameMap = {
        {"Z", sf::Keyboard::Z}, {"Q", sf::Keyboard::Q},
        {"S", sf::Keyboard::S}, {"D", sf::Keyboard::D},
        {"Escape", sf::Keyboard::Escape},
        {"Return", sf::Keyboard::Return},
        {"BackSpace", sf::Keyboard::BackSpace}
    };

    // Initialize axis name mapping
    axisNameMap = {
        {"X", sf::Joystick::X},
        {"Y", sf::Joystick::Y},
        {"Z", sf::Joystick::Z},
        {"R", sf::Joystick::R},
        {"U", sf::Joystick::U},
        {"V", sf::Joystick::V},
        {"PovX", sf::Joystick::PovX},
        {"PovY", sf::Joystick::PovY}
    };

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
        createDefaultConfig();
    }
}

void ConfigManager::createDefaultConfig() {
    spdlog::info("Creating default config with controller settings");
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
        {"input", toml::table{
            {"keyboard", toml::table{
                {"move_up", "Z"},
                {"move_down", "S"},
                {"move_left", "Q"},
                {"move_right", "D"},
                {"pause", "Escape"},
                {"confirm", "Return"},
                {"cancel", "BackSpace"}
            }},
            {"controller", toml::table{
                {"enabled", true},
                {"deadzone", 20.0},
                {"sensitivity", 100.0},
                {"buttons", toml::table{
                    {"move_up", 3},
                    {"move_down", 1},
                    {"move_left", 2},
                    {"move_right", 0},
                    {"pause", 7},
                    {"confirm", 4},
                    {"cancel", 5}
                }},
                {"axes", toml::table{
                    {"horizontal", "X"},
                    {"vertical", "Y"}
                }}
            }}
        }}
    };
    saveConfig();
}

void ConfigManager::saveConfig() {
    std::ofstream file("assets/config.toml");
    file << config;
    spdlog::info("Config saved with controller settings: enabled={}, deadzone={:.1f}, sensitivity={:.1f}",
        isControllerEnabled(), getControllerDeadzone(), getControllerSensitivity());
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

sf::Keyboard::Key ConfigManager::stringToKey(const std::string& keyName) const {
    auto it = keyNameMap.find(keyName);
    return it != keyNameMap.end() ? it->second : sf::Keyboard::Unknown;
}

sf::Keyboard::Key ConfigManager::getKeyBinding(const std::string& action) const {
    std::string keyName = config["input"]["keyboard"][action].value_or("");
    auto key = stringToKey(keyName);
    spdlog::debug("Key binding: {} -> {}", action, keyName);
    return key;
}

bool ConfigManager::isControllerEnabled() const {
    bool enabled = config["input"]["controller"]["enabled"].value_or(true);
    spdlog::debug("Controller enabled: {}", enabled);
    return enabled;
}

float ConfigManager::getControllerDeadzone() const {
    float deadzone = config["input"]["controller"]["deadzone"].value_or(20.0f);
    spdlog::debug("Controller deadzone: {:.1f}", deadzone);
    return deadzone;
}

float ConfigManager::getControllerSensitivity() const {
    float sensitivity = config["input"]["controller"]["sensitivity"].value_or(100.0f);
    spdlog::debug("Controller sensitivity: {:.1f}", sensitivity);
    return sensitivity;
}

unsigned int ConfigManager::getControllerButton(const std::string& action) const {
    unsigned int button = static_cast<unsigned int>(config["input"]["controller"]["buttons"][action].value_or(0));
    spdlog::debug("Controller button for {}: {}", action, button);
    return button;
}

void ConfigManager::loadInputConfig() {
    try {
        // Load or create default config if not already loaded
        if (config.empty()) {
            loadConfig();
        }

        spdlog::info("Loading input configuration");

        // Verify input configuration sections exist
        if (!config["input"]["keyboard"] || !config["input"]["controller"]) {
            spdlog::warn("Input configuration sections missing, creating defaults");
            createDefaultConfig();
        }

        // Log loaded configurations
        spdlog::debug("Keyboard bindings loaded");
        spdlog::debug("Controller settings - enabled: {}, deadzone: {}, sensitivity: {}",
            isControllerEnabled(),
            getControllerDeadzone(),
            getControllerSensitivity());

    } catch (const std::exception& e) {
        spdlog::error("Error loading input config: {}", e.what());
        createDefaultConfig();
    }
}

} // namespace game
