#pragma once
#include <SFML/Graphics.hpp>

namespace game {

class Game;  // Forward declaration

class Screen {
public:
    explicit Screen(Game& gameRef) : game(gameRef) {}
    virtual ~Screen() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void pause() {}
    virtual void resume() {}

protected:
    Game& game;
};

} // namespace game
