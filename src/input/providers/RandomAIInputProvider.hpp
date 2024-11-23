#pragma once
#include "InputProvider.hpp"
#include <random>
#include <chrono>

namespace game {

class RandomAIInputProvider : public InputProvider {
private:
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;
    std::chrono::steady_clock::time_point lastChangeTime;
    sf::Vector2f currentMovement;
    float movementDuration;

public:
    RandomAIInputProvider();
    sf::Vector2f getMovementVector() override;
    bool isActionActive(Action action) override;
    void updateMovement();
};

} // namespace game
