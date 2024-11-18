#pragma once
#include <memory>
#include <stack>
#include "Screen.hpp"
#include <spdlog/spdlog.h>

namespace game {

class ScreenManager {
public:
    static ScreenManager& getInstance();
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    Screen* getCurrentScreen();
    bool isEmpty() const;

private:
    ScreenManager() = default;
    std::stack<std::unique_ptr<Screen>> screens;
    // Delete copy constructor and assignment operator
    ScreenManager(const ScreenManager&) = delete;
    ScreenManager& operator=(const ScreenManager&) = delete;
};

} // namespace game
