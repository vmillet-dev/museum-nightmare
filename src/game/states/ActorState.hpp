#pragma once
#include <SFML/System/Vector2.hpp>

namespace game {

// Forward declaration
class Actor;

enum class ActorStateType {
    Idle,
    Walking,
    Jumping,
    Attacking,
    Stunned,
    Dead
};

class ActorState {
public:
    virtual ~ActorState() = default;

    virtual void enter(Actor* actor) = 0;
    virtual void update(Actor* actor, float deltaTime) = 0;
    virtual void exit(Actor* actor) = 0;
    virtual ActorStateType getType() const = 0;

protected:
    // Utility functions for state implementations
    bool canTransitionTo(ActorStateType newState) const;
    void requestStateTransition(Actor* actor, ActorStateType newState);
};

} // namespace game
