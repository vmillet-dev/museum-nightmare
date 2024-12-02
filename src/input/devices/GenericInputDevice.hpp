#pragma once
#include "InputDevice.hpp"
#include <unordered_map>
#include <SFML/Window.hpp>

namespace game {

template <typename KeyType>
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
    std::unordered_map<KeyType, Action> bindings;
    std::unordered_map<KeyType, ActionState> states;

    void setBinding(KeyType key, Action action) {
        bindings[key] = action;
    }

    bool hasBinding(KeyType key) const {
        return bindings.count(key) > 0;
    }

    void setState(KeyType key, bool pressed) {
        if (hasBinding(key)) {
            auto& state = states[key];
            state.previous = state.current;
            state.current = pressed;
        }
    }
};

} // namespace game
