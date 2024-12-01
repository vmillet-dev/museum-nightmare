#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

template <typename InputType>
class GenericInputDevice : public InputDevice {
public:
    void update() override {
        for (const typename std::unordered_map<InputType, Action>::const_iterator& it = bindings.begin(); it != bindings.end(); ++it) {
            if (states[it->first].current != states[it->first].previous) {
                states[it->first].previous = states[it->first].current;
            }
        }
    }

    bool isActionPressed(Action action) override {
        for (const typename std::unordered_map<InputType, Action>::const_iterator& it = bindings.begin(); it != bindings.end(); ++it) {
            if (it->second == action && states[it->first].current) {
                return true;
            }
        }
        return false;
    }

    bool isActionJustPressed(Action action) override {
        for (const typename std::unordered_map<InputType, Action>::const_iterator& it = bindings.begin(); it != bindings.end(); ++it) {
            const auto& state = states[it->first];
            if (it->second == action && state.current != state.previous && state.current) {
                return true;
            }
        }
        return false;
    }

    bool isActionReleased(Action action) override {
        for (const typename std::unordered_map<InputType, Action>::const_iterator& it = bindings.begin(); it != bindings.end(); ++it) {
            const auto& state = states[it->first];
            if (it->second == action && state.current != state.previous && !state.current) {
                return true;
            }
        }
        return false;
    }

protected:
    std::unordered_map<InputType, Action> bindings;
    std::unordered_map<InputType, ActionState> states;

    void setBinding(InputType input, Action action) {
        bindings[input] = action;
        // Initialize state when binding is set
        if (states.find(input) == states.end()) {
            states[input] = ActionState{false, false};
        }
    }

    void setState(InputType input, bool pressed) {
        if (bindings.count(input)) {
            auto& state = states[input];
            state.previous = state.current;
            state.current = pressed;
        }
    }
};

} // namespace game
