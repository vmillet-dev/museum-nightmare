#pragma once
#include "GenericInputDevice.hpp"
#include <SFML/Graphics.hpp>
#include "../../config/ConfigManager.hpp"

namespace game {

class MouseDevice : public GenericInputDevice<sf::Mouse::Button> {
public:
    explicit MouseDevice(sf::RenderWindow& window);

    void init() override;  // No initialization needed for mouse
    void handleEvent(const sf::Event& event) override;

    sf::Vector2i getMousePosition() const;

private:
    void setButtonBinding(sf::Mouse::Button button, Action action);
    void setButtonState(sf::Event::MouseButtonEvent buttonEvent, bool pressed);

    sf::RenderWindow& window;
    sf::Vector2i lastMousePos;
    ConfigManager& config = ConfigManager::getInstance();
};

} // namespace game
