#include "AIActionController.hpp"

namespace game {

void AIActionController::queueAction(Action action) {
    actionQueue.push(action);
}

bool AIActionController::hasQueuedActions() const {
    return !actionQueue.empty();
}

Action AIActionController::getNextAction() {
    if (actionQueue.empty()) {
        return Action::None; // Assuming we have a None action defined
    }

    Action nextAction = actionQueue.front();
    actionQueue.pop();
    return nextAction;
}

} // namespace game
