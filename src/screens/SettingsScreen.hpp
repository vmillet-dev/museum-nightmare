#pragma once
#include "Screen.hpp"
#include "../ui/MenuBuilder.hpp"
#include <vector>

namespace game {

class SettingsScreen : public Screen {
public:
    explicit SettingsScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    MenuBuilder menuBuilder_;
    Button* backButton_;
    Button* applyButton_;
    Dropdown* resolutionDropdown_;

    std::vector<std::string> getAvailableResolutions();
    void applySettings();
};

} // namespace game
