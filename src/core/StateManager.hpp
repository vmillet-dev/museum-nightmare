#pragma once
#include "GameState.hpp"
#include "../screens/Screen.hpp"
#include <memory>

namespace game {

class Game;  // Forward declaration

class StateManager {
public:
    explicit StateManager(Game& game);
    void transitionTo(GameState newState);
    GameState getCurrentState() const { return currentState; }

private:
    Game& game;
    GameState currentState;
};

} // namespace game
