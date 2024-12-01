#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

template <typename InputType>
class GenericInputDevice : public InputDevice {
public:
    using ActionMap = std::unordered_map<InputType, Action>;
    using StateMap = std::unordered_map<InputType, ActionState>;
    using ActionIterator = typename ActionMap::const_iterator;
    using StateIterator = typename StateMap::iterator;
    using ConstStateIterator = typename StateMap::const_iterator;

    void update() override {
        for (ActionIterator it = bindings.cbegin(); it != bindings.cend(); ++it) {
            StateIterator stateIt = states.find(it->first);
            if (stateIt != states.end() && stateIt->second.current != stateIt->second.previous) {
                stateIt->second.previous = stateIt->second.current;
            }
        }
    }

    bool isActionPressed(Action action) override {
        for (ActionIterator it = bindings.cbegin(); it != bindings.cend(); ++it) {
            ConstStateIterator stateIt = states.find(it->first);
            if (it->second == action && stateIt != states.end() && stateIt->second.current) {
                return true;
            }
        }
        return false;
    }

    bool isActionJustPressed(Action action) override {
        for (ActionIterator it = bindings.cbegin(); it != bindings.cend(); ++it) {
            ConstStateIterator stateIt = states.find(it->first);
            if (stateIt != states.end() && it->second == action &&
                stateIt->second.current != stateIt->second.previous && stateIt->second.current) {
                return true;
            }
        }
        return false;
    }

    bool isActionReleased(Action action) override {
        for (ActionIterator it = bindings.cbegin(); it != bindings.cend(); ++it) {
            ConstStateIterator stateIt = states.find(it->first);
            if (stateIt != states.end() && it->second == action &&
                stateIt->second.current != stateIt->second.previous && !stateIt->second.current) {
                return true;
            }
        }
        return false;
    }

protected:
    ActionMap bindings;
    StateMap states;

    void setBinding(InputType input, Action action) {
        bindings[input] = action;
        // Initialize state when binding is set
        if (states.find(input) == states.end()) {
            states[input] = ActionState{false, false};
        }
    }

    void setState(InputType input, bool pressed) {
        StateIterator stateIt = states.find(input);
        if (bindings.count(input) > 0 && stateIt != states.end()) {
            stateIt->second.previous = stateIt->second.current;
            stateIt->second.current = pressed;
        }
    }
};

} // namespace game
