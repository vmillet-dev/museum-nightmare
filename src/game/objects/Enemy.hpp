#pragma once
#include "Actor.hpp"
#include "../../input/AIInputProvider.hpp"
#include <SFML/Graphics.hpp>

namespace game {

class Enemy : public Actor {
public:
    Enemy(float x, float y, AIBehaviorType initialBehavior = AIBehaviorType::Patrol);

    void setTarget(const sf::Vector2f& targetPos);
    void setBehavior(AIBehaviorType behavior);

protected:
    void handleStateTransition(ActorStateType newState) override;

private:
    void initializeStates();
    void setupAIProvider();
    AIInputProvider* getAIProvider();
};

} // namespace game
