#include "ScreenManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

ScreenManager& ScreenManager::getInstance() {
    static ScreenManager instance;
    return instance;
}

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen) {
    if (!screens.empty()) {
        screens.back()->pause();
    }
    screens.push_back(std::move(screen));
    spdlog::info("Pushed new screen. Total screens: {}", screens.size());
}

void ScreenManager::popScreen() {
    if (!screens.empty()) {
        screens.pop_back();
        spdlog::info("Popped screen. Remaining screens: {}", screens.size());
        if (!screens.empty()) {
            screens.back()->resume();
        }
    }
}

void ScreenManager::handleInput(const sf::Event& event) {
    if (!screens.empty()) {
        screens.back()->handleInput(event);
    }
}

void ScreenManager::update(float deltaTime) {
    if (!screens.empty()) {
        screens.back()->update(deltaTime);
    }
}

void ScreenManager::render(sf::RenderWindow& window) {
    // Render all screens from bottom to top
    // This allows for transparent overlays
    for (const auto& screen : screens) {
        screen->render(window);
    }
}

Screen* ScreenManager::getCurrentScreen() {
    return screens.empty() ? nullptr : screens.back().get();
}

bool ScreenManager::isEmpty() const {
    return screens.empty();
}

} // namespace game
