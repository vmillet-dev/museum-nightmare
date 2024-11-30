#pragma once
#include <SFML/Graphics.hpp>
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class MouseDevice : public InputDevice {
public:
    explicit MouseDevice(sf::RenderWindow& window);

    void init() override;  // No initialization needed for mouse
    void update() override;
    bool isActionPressed(Action action) override;
    bool isActionJustPressed(Action action) override;
    bool isActionReleased(Action action) override;
    void handleEvent(const sf::Event& event) override;

    sf::Vector2i getMousePosition() const;

private:
    std::unordered_map<sf::Mouse::Button, Action> buttonBindings;
    std::unordered_map<sf::Mouse::Button, ActionState> buttonStates;

    void setButtonBinding(sf::Mouse::Button, Action action);
    void setButtonState(sf::Event::MouseButtonEvent, bool pressed);

    sf::RenderWindow& window;
    bool leftButtonPressed;
    bool rightButtonPressed;
    bool leftButtonJustPressed;
    bool rightButtonJustPressed;
    sf::Vector2i lastMousePos;
};

} // namespace game
