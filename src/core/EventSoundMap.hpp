#pragma once
#include <unordered_map>
#include <string>
#include "Action.hpp"

namespace core {

struct SoundEvent {
    std::string soundName;
    bool isMusic;
    bool loop;
};

class EventSoundMap {
public:
    static const std::unordered_map<game::Action, SoundEvent> actionSoundMap;
};

// Define the mapping between actions and sounds
const std::unordered_map<game::Action, SoundEvent> EventSoundMap::actionSoundMap = {
    {game::Action::Confirm, {"slide", false, false}},      // Play slide sound effect
    {game::Action::Pause, {"menu_theme", true, true}}      // Play menu theme music
};

} // namespace core
