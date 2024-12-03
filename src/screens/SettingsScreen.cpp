#include "SettingsScreen.hpp"
#include <spdlog/spdlog.h>

namespace game {

SettingsScreen::SettingsScreen(Game& game) : Screen(game), menuBuilder(game) {
    // Common resolutions
    availableResolutions = {
        {800, 600},
        {1024, 768},
        {1280, 720},
        {1366, 768},
        {1920, 1080}
    };

    initializeMenu();
}

void SettingsScreen::initializeMenu() {
    // Convert resolutions to strings
    std::vector<std::string> resolutionStrings;
    for (const auto& res : availableResolutions) {
        resolutionStrings.push_back(std::to_string(res.x) + "x" + std::to_string(res.y));
    }

    float centerX = game.getWindow().getSize().x / 2.0f;
    float startY = game.getWindow().getSize().y / 2.0f - 100.0f;

    // Add resolution dropdown
    menuBuilder.addDropdown("Resolution", centerX, startY, resolutionStrings,
        [this](size_t index) { handleResolutionChange(index); });

    // Add back button
    menuBuilder.addButton("Back", centerX, startY + 100,
        [this]() {
            spdlog::info("Returning to main menu");
            game.getScreenManager().setState(GameState::MainMenu);
        });

    widgets = menuBuilder.build();
}

void SettingsScreen::handleResolutionChange(size_t index) {
    if (index < availableResolutions.size()) {
        const auto& newRes = availableResolutions[index];
        spdlog::info("Changing resolution to {}x{}", newRes.x, newRes.y);
        game.getWindow().setSize(newRes);

        // Center the window after resizing
        auto desktopMode = sf::VideoMode::getDesktopMode();
        sf::Vector2i screenSize(desktopMode.width, desktopMode.height);
        game.getWindow().setPosition(
            sf::Vector2i((screenSize.x - newRes.x) / 2, (screenSize.y - newRes.y) / 2)
        );

        // Reinitialize menu for new window size
        initializeMenu();
    }
}

void SettingsScreen::update(InputManager& inputManager) {
    menuBuilder.update(inputManager);
}

void SettingsScreen::render(sf::RenderWindow& window) {
    menuBuilder.render(window);
}

} // namespace game
