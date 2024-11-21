#pragma once
#include "InputManager.hpp"
#include <queue>

namespace game {

class AIActionController {
public:
    AIActionController() = default;
    virtual ~AIActionController() = default;

    // Core AI interface
    virtual void update() = 0;
    virtual void queueAction(Action action);
    virtual bool hasQueuedActions() const;
    virtual Action getNextAction();

protected:
    std::queue<Action> actionQueue;
};

// Example implementation of a simple AI controller
class SimpleAIController : public AIActionController {
public:
    void update() override {
        // Simple example: Queue random movement actions
        if (!hasQueuedActions()) {
            Action actions[] = {Action::MoveLeft, Action::MoveRight, Action::MoveUp, Action::MoveDown};
            queueAction(actions[rand() % 4]);
        }
    }
};

} // namespace game
