#pragma once
#include <SFML/Graphics.hpp>

namespace game {

class Game; // Forward declaration

class Screen {
protected:
    Game& game;

public:
    Screen(Game& g) : game(g) {}
    virtual ~Screen() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void pause() {}
    virtual void resume() {}
};

} // namespace game
