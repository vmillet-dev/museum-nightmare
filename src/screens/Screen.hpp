#pragma once
#include <SFML/Graphics.hpp>

namespace game {

class Screen {
public:
    virtual ~Screen() = default;
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void pause() {}
    virtual void resume() {}
};

} // namespace game
