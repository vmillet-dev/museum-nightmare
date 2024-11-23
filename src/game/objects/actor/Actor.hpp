#pragma once
#include "../GameObject.hpp"
#include "../../../input/providers/InputProvider.hpp"
#include <memory>

namespace game {

class Actor : public GameObject {
protected:
    std::unique_ptr<InputProvider> inputProvider;
    float speed;

public:
    Actor(float x, float y, std::unique_ptr<InputProvider> provider, float speed = 200.0f);
    virtual ~Actor() = default;

    void update(float deltaTime) override;
    void move(float x, float y, float deltaTime);
};

} // namespace game
