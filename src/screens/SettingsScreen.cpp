#include "SettingsScreen.hpp"
#include "../core/Game.hpp"
#include <SFML/Window/VideoMode.hpp>

namespace game {

SettingsScreen::SettingsScreen(Game& game) : Screen(game) {
    // Create resolution options
    std::vector<std::string> resolutions = getAvailableResolutions();

    // Create widgets with consistent spacing and sizing
    const float buttonWidth = 200.f;
    const float buttonHeight = 50.f;
    const float dropdownWidth = 300.f;
    const float spacing = 20.f;

    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create resolution dropdown
    resolutionDropdown_ = menuBuilder_.addDropdown(
        "Resolution",
        sf::Vector2f(centerX, startY),
        sf::Vector2f(dropdownWidth, buttonHeight),
        resolutions
    );

    // Create Apply button
    applyButton_ = menuBuilder_.addButton(
        "Apply",
        sf::Vector2f(centerX, startY + buttonHeight + spacing),
        sf::Vector2f(buttonWidth, buttonHeight)
    );

    // Create Back button
    backButton_ = menuBuilder_.addButton(
        "Back",
        sf::Vector2f(centerX, startY + (buttonHeight + spacing) * 2),
        sf::Vector2f(buttonWidth, buttonHeight)
    );
}

void SettingsScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());

    if (backButton_->isClicked()) {
        game.getScreenManager().setState(GameState::MainMenu);
    }

    if (applyButton_->isClicked()) {
        applySettings();
    }
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

void SettingsScreen::applySettings() {
    const std::string& selectedRes = resolutionDropdown_->getSelectedOption();
    size_t xPos = selectedRes.find('x');
    if (xPos != std::string::npos) {
        unsigned int width = std::stoul(selectedRes.substr(0, xPos));
        unsigned int height = std::stoul(selectedRes.substr(xPos + 1));

        // Update window size
        game.getWindow().setSize(sf::Vector2u(width, height));

        // Center window on screen
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        game.getWindow().setPosition(sf::Vector2i(
            (desktop.width - width) / 2,
            (desktop.height - height) / 2
        ));
    }
}

} // namespace game
