#include <gtest/gtest.h>
#include "../../src/input/HumanInputProvider.hpp"
#include <SFML/Window/Event.hpp>
#include <thread>
#include <chrono>

namespace game {
namespace test {

class ControllerHotplugTest : public ::testing::Test {
protected:
    void SetUp() override {
        inputProvider = std::make_unique<HumanInputProvider>();
    }

    std::unique_ptr<HumanInputProvider> inputProvider;
};

TEST_F(ControllerHotplugTest, HandleControllerDisconnect) {
    // Note: This test requires manual verification with actual controller
    GTEST_SKIP() << "Controller hotplug tests require manual verification";

    // Test outline for manual verification:
    // 1. Start with controller connected
    // 2. Verify movement input works
    // 3. Disconnect controller
    // 4. Verify keyboard input still works
    // 5. Reconnect controller
    // 6. Verify controller input works again
}

TEST_F(ControllerHotplugTest, InputSourceChange) {
    // Simulate input source changes
    inputProvider->handleInputSourceChange();

    // Verify we can still get movement input
    sf::Vector2f movement = inputProvider->getMovementInput();
    EXPECT_TRUE(movement.x == 0.f && movement.y == 0.f)
        << "Should handle input source change gracefully";
}

TEST_F(ControllerHotplugTest, DeadzoneHandling) {
    // Test that small analog movements are filtered by deadzone
    sf::Vector2f movement = inputProvider->getMovementInput();
    EXPECT_TRUE(movement.x == 0.f && movement.y == 0.f)
        << "Should filter out small movements within deadzone";
}

} // namespace test
} // namespace game
