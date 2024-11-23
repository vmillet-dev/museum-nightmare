#pragma once
#include <memory>
#include <stack>
#include "Screen.hpp"
#include <spdlog/spdlog.h>

namespace game {

class Game;  // Forward declaration

class ScreenManager {
public:
    explicit ScreenManager(Game& game);
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    Screen* getCurrentScreen();
    bool isEmpty() const;

private:
    Game& game;
    std::stack<std::unique_ptr<Screen>> screens;
};

} // namespace game
