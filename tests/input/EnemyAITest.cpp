#include <gtest/gtest.h>
#include "../../src/game/objects/Enemy.hpp"
#include "../../src/input/AIInputProvider.hpp"
#include <SFML/System/Clock.hpp>

namespace game {
namespace test {

class EnemyAITest : public ::testing::Test {
protected:
    void SetUp() override {
        enemy = std::make_unique<Enemy>(400.f, 300.f, AIBehaviorType::Patrol);
    }

    std::unique_ptr<Enemy> enemy;
};

TEST_F(EnemyAITest, PatrolBehavior) {
    float deltaTime = 1.0f/60.f;
    sf::Vector2f initialPos = enemy->getPosition();

    // Update multiple frames to see patrol movement
    for (int i = 0; i < 60; ++i) {
        enemy->update(deltaTime);
    }

    sf::Vector2f newPos = enemy->getPosition();
    EXPECT_NE(initialPos.x, newPos.x) << "Enemy should move during patrol";
}

TEST_F(EnemyAITest, ChaseBehavior) {
    enemy->setBehavior(AIBehaviorType::Chase);
    sf::Vector2f targetPos(500.f, 300.f);
    enemy->setTarget(targetPos);

    float deltaTime = 1.0f/60.f;
    sf::Vector2f initialPos = enemy->getPosition();

    // Update multiple frames to see chase movement
    for (int i = 0; i < 30; ++i) {
        enemy->update(deltaTime);
    }

    sf::Vector2f newPos = enemy->getPosition();
    float distanceToTarget = std::sqrt(
        std::pow(targetPos.x - newPos.x, 2) +
        std::pow(targetPos.y - newPos.y, 2)
    );
    float initialDistance = std::sqrt(
        std::pow(targetPos.x - initialPos.x, 2) +
        std::pow(targetPos.y - initialPos.y, 2)
    );

    EXPECT_LT(distanceToTarget, initialDistance) << "Enemy should move closer to target when chasing";
}

TEST_F(EnemyAITest, StateTransitions) {
    EXPECT_EQ(enemy->getCurrentStateType(), ActorStateType::Idle);

    float deltaTime = 1.0f/60.f;
    enemy->setBehavior(AIBehaviorType::Chase);
    enemy->setTarget(sf::Vector2f(500.f, 300.f));

    // Update to trigger state transition
    enemy->update(deltaTime);

    EXPECT_EQ(enemy->getCurrentStateType(), ActorStateType::Walking)
        << "Enemy should transition to walking state when moving";
}

} // namespace test
} // namespace game
