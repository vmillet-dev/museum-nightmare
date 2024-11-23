#include "ScreenManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

ScreenManager::ScreenManager(Game& game) : game(game) {}

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen) {
    if (!screens.empty()) {
        screens.top()->pause();
    }
    screens.push(std::move(screen));
    spdlog::info("Pushed new screen. Total screens: {}", screens.size());
}

void ScreenManager::popScreen() {
    if (!screens.empty()) {
        auto currentScreen = std::move(screens.top());
        screens.pop();
        spdlog::info("Popped screen. Remaining screens: {}", screens.size());

        // Only resume the previous screen if it exists
        if (!screens.empty()) {
            screens.top()->resume();
            spdlog::info("Resumed previous screen");
        }
    }
}

void ScreenManager::handleInput(const sf::Event& event) {
    if (!screens.empty()) {
        screens.top()->handleInput(event);
    }
}

void ScreenManager::update(float deltaTime) {
    if (!screens.empty()) {
        screens.top()->update(deltaTime);
    }
}

void ScreenManager::render(sf::RenderWindow& window) {
    if (!screens.empty()) {
        screens.top()->render(window);
    }
}

Screen* ScreenManager::getCurrentScreen() {
    return screens.empty() ? nullptr : screens.top().get();
}

bool ScreenManager::isEmpty() const {
    return screens.empty();
}

} // namespace game
