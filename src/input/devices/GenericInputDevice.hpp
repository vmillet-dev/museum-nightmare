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

    virtual ~GenericInputDevice() override = default;

    void update() override {
        for (const auto& [input, action] : bindings) {
            auto stateIt = states.find(input);
            if (stateIt != states.end() && stateIt->second.current != stateIt->second.previous) {
                stateIt->second.previous = stateIt->second.current;
            }
        }
    }

    bool isActionPressed(Action action) override {
        for (const auto& [input, boundAction] : bindings) {
            if (boundAction == action) {
                auto stateIt = states.find(input);
                if (stateIt != states.end() && stateIt->second.current) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isActionJustPressed(Action action) override {
        for (const auto& [input, boundAction] : bindings) {
            if (boundAction == action) {
                auto stateIt = states.find(input);
                if (stateIt != states.end() &&
                    stateIt->second.current &&
                    !stateIt->second.previous) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isActionReleased(Action action) override {
        for (const auto& [input, boundAction] : bindings) {
            if (boundAction == action) {
                auto stateIt = states.find(input);
                if (stateIt != states.end() &&
                    !stateIt->second.current &&
                    stateIt->second.previous) {
                    return true;
                }
            }
        }
        return false;
    }

protected:
    ActionMap bindings;
    StateMap states;

    void setBinding(InputType input, Action action) {
        bindings[input] = action;
        if (states.find(input) == states.end()) {
            states[input] = ActionState{false, false};
        }
    }

    void setState(InputType input, bool pressed) {
        auto stateIt = states.find(input);
        if (bindings.count(input) > 0 && stateIt != states.end()) {
            stateIt->second.previous = stateIt->second.current;
            stateIt->second.current = pressed;
        }
    }
};

} // namespace game
