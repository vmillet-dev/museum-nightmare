#pragma once
#include <memory>
#include <stack>
#include "Screen.hpp"
#include <spdlog/spdlog.h>

namespace game {

class ScreenManager {
public:
    static ScreenManager& getInstance() {
        static ScreenManager instance;
        return instance;
    }

    void pushScreen(std::unique_ptr<Screen> screen) {
        spdlog::info("Pushing new screen");
        if (!screens.empty()) {
            screens.top()->pause();
        }
        screens.push(std::move(screen));
    }

    void popScreen() {
        if (!screens.empty()) {
            spdlog::info("Popping screen");
            screens.pop();
            if (!screens.empty()) {
                screens.top()->resume();
            }
        }
    }

    void handleInput(const sf::Event& event) {
        if (!screens.empty()) {
            screens.top()->handleInput(event);
        }
    }

    void update(float deltaTime) {
        if (!screens.empty()) {
            screens.top()->update(deltaTime);
        }
    }

    void render(sf::RenderWindow& window) {
        if (!screens.empty()) {
            screens.top()->render(window);
        }
    }

    Screen* getCurrentScreen() {
        return screens.empty() ? nullptr : screens.top().get();
    }

    bool isEmpty() const { return screens.empty(); }

private:
    ScreenManager() = default;
    std::stack<std::unique_ptr<Screen>> screens;
};

} // namespace game
