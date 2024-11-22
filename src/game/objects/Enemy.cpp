#include "Enemy.hpp"
#include "../states/ActorStates.hpp"
#include <spdlog/spdlog.h>

namespace game {

Enemy::Enemy(float x, float y, AIBehaviorType initialBehavior) : Actor(x, y) {
    shape.setFillColor(sf::Color::Red);
    setupAIProvider();
    initializeStates();
    setBehavior(initialBehavior);
    spdlog::info("Enemy created at position ({}, {}) with behavior {}", x, y, static_cast<int>(initialBehavior));
}

void Enemy::setupAIProvider() {
    setInputProvider(std::make_unique<AIInputProvider>());
}

void Enemy::initializeStates() {
    handleStateTransition(ActorStateType::Idle);
}

void Enemy::handleStateTransition(ActorStateType newState) {
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

void Enemy::setTarget(const sf::Vector2f& targetPos) {
    if (auto* ai = getAIProvider()) {
        ai->setTarget(targetPos);
    }
}

void Enemy::setBehavior(AIBehaviorType behavior) {
    if (auto* ai = getAIProvider()) {
        ai->setBehavior(behavior);
    }
}

AIInputProvider* Enemy::getAIProvider() {
    return dynamic_cast<AIInputProvider*>(inputProvider.get());
}

} // namespace game
