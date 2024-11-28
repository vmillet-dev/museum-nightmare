#ifndef PARALLAXBACKGROUND_HPP
#define PARALLAXBACKGROUND_HPP

#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class ParallaxBackground : public game::GameObject {
public:
    ParallaxBackground(const sf::Texture& texture, const sf::Vector2f& position, float depth);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) const override;  // Made const
    void initPhysics(b2WorldId worldId) override;

    // Methods for parallax support
    float getParallaxFactor() const { return depth; }
    const sf::Vector2f& getBasePosition() const { return initialPosition; }
    void setPosition(const sf::Vector2f& position) { sprite.setPosition(position); }
    void setViewCenter(const sf::Vector2f& center);

private:
    sf::Sprite sprite;
    float depth; // Parallax depth factor (0 = fixed on screen, 1 = moves with world)
    sf::Vector2f initialPosition;
};

} // namespace game

#endif // PARALLAXBACKGROUND_HPP
