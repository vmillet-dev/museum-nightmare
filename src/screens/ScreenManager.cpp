#include "ScreenManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

ScreenManager& ScreenManager::getInstance() {
    static ScreenManager instance;
    return instance;
}

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen) {
    if (!screens.empty()) {
        screens.top()->pause();
    }
    screens.push(std::move(screen));
    spdlog::info("Pushed new screen. Total screens: {}", screens.size());
}

void ScreenManager::popScreen() {
    if (!screens.empty()) {
        screens.pop();
        spdlog::info("Popped screen. Remaining screens: {}", screens.size());
        if (!screens.empty()) {
            screens.top()->resume();
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
