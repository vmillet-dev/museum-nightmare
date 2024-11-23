#pragma once
#include "InputProvider.hpp"
#include "../../input/InputManager.hpp"

namespace game {

class PlayerInputProvider : public InputProvider {
private:
    InputManager& inputManager;

public:
    PlayerInputProvider(InputManager& inputMgr);
    sf::Vector2f getMovementVector() override;
    bool isActionActive(Action action) override;
};

} // namespace game
