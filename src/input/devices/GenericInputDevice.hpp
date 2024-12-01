#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

template <typename InputType>
class GenericInputDevice : public InputDevice {
public:
    void update() override {
        for (const auto& binding : bindings) {
            if (states[binding.first].current != states[binding.first].previous) {
                states[binding.first].previous = states[binding.first].current;
            }
        }
    }

    bool isActionPressed(Action action) override {
        for (const auto& binding : bindings) {
            if (binding.second == action && states[binding.first].current) {
                return true;
            }
        }
        return false;
    }

    bool isActionJustPressed(Action action) override {
        for (const auto& binding : bindings) {
            const auto& state = states[binding.first];
            if (binding.second == action && state.current != state.previous && state.current) {
                return true;
            }
        }
        return false;
    }

    bool isActionReleased(Action action) override {
        for (const auto& binding : bindings) {
            const auto& state = states[binding.first];
            if (binding.second == action && state.current != state.previous && !state.current) {
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
