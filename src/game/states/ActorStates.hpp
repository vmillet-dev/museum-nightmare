#pragma once
#include "ActorState.hpp"
#include "../objects/Actor.hpp"
#include <spdlog/spdlog.h>

namespace game {

class IdleState : public ActorState {
public:
    void enter(Actor* actor) override {
        spdlog::debug("Entering Idle state");
    }

    void update(Actor* actor, float deltaTime) override {
        // Check for movement input to transition to walking
        if (actor->inputProvider) {
            sf::Vector2f movement = actor->inputProvider->getMovementInput();
            if (movement.x != 0 || movement.y != 0) {
                requestStateTransition(actor, ActorStateType::Walking);
            }
        }
    }

    void exit(Actor* actor) override {
        spdlog::debug("Exiting Idle state");
    }

    ActorStateType getType() const override { return ActorStateType::Idle; }
};

class WalkingState : public ActorState {
public:
    void enter(Actor* actor) override {
        spdlog::debug("Entering Walking state");
    }

    void update(Actor* actor, float deltaTime) override {
        // Check for no movement input to transition to idle
        if (actor->inputProvider) {
            sf::Vector2f movement = actor->inputProvider->getMovementInput();
            if (movement.x == 0 && movement.y == 0) {
                requestStateTransition(actor, ActorStateType::Idle);
            }
        }
    }

    void exit(Actor* actor) override {
        spdlog::debug("Exiting Walking state");
    }

    ActorStateType getType() const override { return ActorStateType::Walking; }
};

} // namespace game
