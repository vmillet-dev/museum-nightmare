#pragma once
#include <SFML/Graphics.hpp>
#include "GenericInputDevice.hpp"

namespace game {

class MouseDevice : public GenericInputDevice<sf::Mouse::Button> {
public:
    explicit MouseDevice(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event) override;
    void loadBinding() override;

    sf::Vector2i getMousePosition() const;

private:
    void setButtonState(const sf::Mouse::Button& button, bool pressed);

    sf::RenderWindow& window;
    sf::Vector2i lastMousePos;
};

} // namespace game
