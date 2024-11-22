#include "Player.hpp"
#include "../states/ActorStates.hpp"
#include "../../input/HumanInputProvider.hpp"
#include <spdlog/spdlog.h>

namespace game {

Player::Player(float x, float y) : Actor(x, y) {
    shape.setFillColor(sf::Color::Green);
    setupInputProvider();
    initializeStates();
    spdlog::info("Player created at position ({}, {})", x, y);
}

void Player::setupInputProvider() {
    setInputProvider(std::make_unique<HumanInputProvider>());
}

void Player::initializeStates() {
    // Start in idle state
    handleStateTransition(ActorStateType::Idle);
}

void Player::handleStateTransition(ActorStateType newState) {
    switch (newState) {
        case ActorStateType::Idle:
            setState(std::make_unique<IdleState>());
            break;
        case ActorStateType::Walking:
            setState(std::make_unique<WalkingState>());
            break;
        default:
            Actor::handleStateTransition(newState);
            break;
    }
}

} // namespace game
