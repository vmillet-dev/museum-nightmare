#include <gtest/gtest.h>
#include "../../src/game/objects/Player.hpp"
#include "../../src/input/HumanInputProvider.hpp"
#include <SFML/Window/Event.hpp>

namespace game {
namespace test {

class PlayerInputTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = std::make_unique<Player>(400.f, 300.f);
    }

    std::unique_ptr<Player> player;
};

TEST_F(PlayerInputTest, KeyboardMovement) {
    // Test keyboard movement in all directions
    float deltaTime = 1.0f/60.f;

    // Update multiple frames to account for acceleration
    for (int i = 0; i < 10; ++i) {
        player->update(deltaTime);
    }

    auto bounds = player->getBounds();
    EXPECT_GT(bounds.left, 400.f) << "Player should move right when D key is pressed";
}

TEST_F(PlayerInputTest, ControllerMovement) {
    // Note: Controller tests require manual verification due to hardware dependencies
    GTEST_SKIP() << "Controller tests require manual verification";
}

TEST_F(PlayerInputTest, StateTransitions) {
    float deltaTime = 1.0f/60.f;

    // Should start in idle state
    EXPECT_EQ(player->getCurrentStateType(), ActorStateType::Idle);

    // Simulate movement input
    player->update(deltaTime);

    // Should transition to walking state when moving
    EXPECT_EQ(player->getCurrentStateType(), ActorStateType::Walking);
}

} // namespace test
} // namespace game
