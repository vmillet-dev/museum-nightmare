#pragma once
#include "Actor.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class Player : public Actor {
public:
    Player(float x, float y);

protected:
    // Override Actor's state transition handling
    void handleStateTransition(ActorStateType newState) override;

private:
    // Player-specific state handling
    void initializeStates();
    void setupInputProvider();
};

} // namespace game
