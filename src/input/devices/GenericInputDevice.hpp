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
            if (binding.second == action && states[binding.first].isPressed()) {
                return true;
            }
        }
        return false;
    }

    bool isActionJustPressed(Action action) override {
        for (const auto& binding : bindings) {
            const auto& state = states[binding.first];
            if (binding.second == action && !state.wasPressed() && state.isPressed()) {
                return true;
            }
        }
        return false;
    }

    bool isActionReleased(Action action) override {
        for (const auto& binding : bindings) {
            const auto& state = states[binding.first];
            if (binding.second == action && state.wasPressed() && !state.isPressed()) {
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

    void setState(KeyType key, float value) {
        if (hasBinding(key)) {
            auto& state = states[key];
            state.previous = state.current;
            state.current = value;
        }
    }

    // Convenience method for digital inputs
    void setDigitalState(KeyType key, bool pressed) {
        setState(key, pressed ? 1.0f : 0.0f);
    }

    void clearBindingsMap() { bindings.clear(); }
    void clearStatesMap() { states.clear(); }

    void clearBindingsAndStatesMap() {
        bindings.clear();
        states.clear();
    }
};

} // namespace game
