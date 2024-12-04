#pragma once
#include "Screen.hpp"
#include "../ui/Dropdown.hpp"
#include "../ui/MenuBuilder.hpp"
#include <memory>
#include <vector>

namespace game {

class TestScreen : public Screen {
public:
    explicit TestScreen(Game& game);
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    MenuBuilder menuBuilder_;
    std::vector<Button*> buttons_;
    size_t selectedButtonIndex_;
    Dropdown* resolutionDropdown_;

    std::vector<std::string> getAvailableResolutions() {
        return {
            "800x600",
            "1024x768",
            "1280x720",
            "1920x1080"
        };
    }
};

} // namespace game
