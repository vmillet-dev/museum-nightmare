#include "ActorState.hpp"
#include "ActorStates.hpp"
#include "../objects/Actor.hpp"

namespace game {

bool ActorState::canTransitionTo(ActorStateType newState) const {
    // Default implementation allows all transitions
    // Derived states can override this to restrict transitions
    return true;
}

void ActorState::requestStateTransition(Actor* actor, ActorStateType newState) {
    if (canTransitionTo(newState)) {
        actor->handleStateTransition(newState);
    }
}

} // namespace game
