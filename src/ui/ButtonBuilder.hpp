#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <spdlog/spdlog.h>

namespace game {

class GuiBuilder;

class ButtonBuilder {
private:
    tgui::Button::Ptr button;
    GuiBuilder& parent;

public:
    ButtonBuilder(tgui::Button::Ptr btn, GuiBuilder& p) : button(btn), parent(p) {
        button->setSize({ "50%", "35%" });
        setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        setTextSize(20);
        spdlog::debug("Initialized ButtonBuilder with default settings");
    }

    ButtonBuilder& setHorizontalAlignment(tgui::HorizontalAlignment align) {
        switch (align) {
            case tgui::HorizontalAlignment::Center:
                button->setPosition("(parent.innersize - size) / 2", "");
                spdlog::debug("Set button alignment to center");
                break;
            case tgui::HorizontalAlignment::Right:
                button->setPosition("parent.innersize - size", "");
                spdlog::debug("Set button alignment to right");
                break;
            case tgui::HorizontalAlignment::Left:
            default:
                button->setPosition("0%", "");
                spdlog::debug("Set button alignment to left");
                break;
        }
        return *this;
    }

    ButtonBuilder& setTextSize(unsigned int size) {
        button->setTextSize(size);
        spdlog::debug("Set button text size to: {}", size);
        return *this;
    }

    ButtonBuilder& setBackgroundColor(const tgui::Color& color) {
        button->getRenderer()->setBackgroundColor(color);
        spdlog::debug("Set button background color");
        return *this;
    }

    GuiBuilder& endButton() {
        spdlog::debug("Finished button configuration");
        return parent;
    }
};

} // namespace game
