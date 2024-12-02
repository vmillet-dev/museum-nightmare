#pragma once
#include <SFML/Graphics.hpp>
#include "GenericInputDevice.hpp"

namespace game {

class MouseDevice : public GenericInputDevice<sf::Mouse::Button> {
public:
    explicit MouseDevice(sf::RenderWindow& window);
    void init() override;
    void handleEvent(const sf::Event& event) override;
    sf::Vector2i getMousePosition() const;

private:
    void setButtonBinding(sf::Mouse::Button button, Action action);
    void setButtonState(sf::Event::MouseButtonEvent buttonEvent, bool pressed);

    sf::RenderWindow& window;
    sf::Vector2i lastMousePos;
};

} // namespace game
