#include "SettingsScreen.hpp"
#include "../core/Game.hpp"
#include "../config/ConfigManager.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>

namespace game {

SettingsScreen::SettingsScreen(Game& game) : Screen(game), menuBuilder_(game) {
    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Get available resolutions and current resolution
    auto resolutions = getAvailableResolutions();
    std::string currentRes = std::to_string(windowSize.x) + "x" + std::to_string(windowSize.y);

    // Create resolution dropdown
    resolutionDropdown_ = menuBuilder_.addDropdown(
        "Resolution",
        sf::Vector2f(300, startY),
        sf::Vector2f(200.f, 50.f),
        resolutions
    );

    // Set current resolution as selected
    auto it = std::find(resolutions.begin(), resolutions.end(), currentRes);
    if (it != resolutions.end()) {
        resolutionDropdown_->setSelectedIndex(std::distance(resolutions.begin(), it));
    }

    // Create menu with automatic spacing
    menuBuilder_.setSpacing(100)
        .addButton("Apply", 300, startY + 100,
            [this, &game]() {
                const std::string& selectedRes = resolutionDropdown_->getSelectedOption();
                size_t xPos = selectedRes.find('x');
                if (xPos != std::string::npos) {
                    try {
                        unsigned int width = std::stoul(selectedRes.substr(0, xPos));
                        unsigned int height = std::stoul(selectedRes.substr(xPos + 1));

                        // Update config and window
                        ConfigManager::getInstance().setWindowResolution(width, height);
                        game.getWindow().setSize(sf::Vector2u(width, height));

                        // Center window on screen
                        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
                        game.getWindow().setPosition(sf::Vector2i(
                            (desktop.width - width) / 2,
                            (desktop.height - height) / 2
                        ));

                        spdlog::info("Resolution changed to {}x{}", width, height);
                    } catch (const std::exception& e) {
                        spdlog::error("Failed to parse resolution: {}", e.what());
                    }
                }
            })
        .addButton("Back", 300, startY + 200,
            [this, &game]() {
                game.getScreenManager().setState(GameState::MainMenu);
            });

    // Store buttons and selected index
    buttons_ = menuBuilder_.build();
    selectedButtonIndex_ = menuBuilder_.getSelectedIndex();
}

void SettingsScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());
}

void SettingsScreen::render(sf::RenderWindow& window) {
    menuBuilder_.render(window);
}

std::vector<std::string> SettingsScreen::getAvailableResolutions() {
    std::vector<std::string> resolutions;
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

    for (const auto& mode : modes) {
        if (mode.bitsPerPixel == 32) {  // Only add modes with 32 bits per pixel
            resolutions.push_back(
                std::to_string(mode.width) + "x" + std::to_string(mode.height)
            );
        }
    }

    return resolutions;
}

} // namespace game
