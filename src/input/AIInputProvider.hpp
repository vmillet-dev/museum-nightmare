#pragma once
#include "IInputProvider.hpp"
#include <SFML/System/Clock.hpp>

namespace game {

enum class AIBehaviorType {
    Patrol,      // Move between points
    Chase,       // Follow target
    Wander,      // Random movement
    Idle        // Stand still
};

class AIInputProvider : public IInputProvider {
public:
    AIInputProvider(AIBehaviorType behavior = AIBehaviorType::Patrol);

    sf::Vector2f getMovementInput() override;
    bool isActionTriggered(Action action) override;
    float getActionStrength(Action action) override;
    void update() override;
    void handleInputSourceChange() override;

    void setBehavior(AIBehaviorType newBehavior);
    void setTarget(const sf::Vector2f& targetPos);

private:
    AIBehaviorType currentBehavior;
    sf::Vector2f targetPosition;
    sf::Vector2f currentMovement;
    sf::Clock behaviorTimer;
    float wanderAngle;

    void updatePatrol();
    void updateChase();
    void updateWander();
    void updateIdle();
};

} // namespace game
