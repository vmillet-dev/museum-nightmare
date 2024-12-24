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
    float aspectRatio;
    tgui::AutoLayout currentLayout;

public:
    ButtonBuilder(tgui::Button::Ptr btn, GuiBuilder& p)
        : button(btn), parent(p), aspectRatio(0.0f), currentLayout(tgui::AutoLayout::Manual) {
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

    ButtonBuilder& setAutoLayout(tgui::AutoLayout layout) {
        currentLayout = layout;
        switch (layout) {
            case tgui::AutoLayout::Top:
                button->setPosition("0%", "0%");
                button->setWidth("100%");
                break;
            case tgui::AutoLayout::Bottom:
                button->setPosition("0%", "100% - height");
                button->setWidth("100%");
                break;
            case tgui::AutoLayout::Left:
                button->setPosition("0%", "0%");
                button->setHeight("100%");
                break;
            case tgui::AutoLayout::Right:
                button->setPosition("100% - width", "0%");
                button->setHeight("100%");
                break;
            case tgui::AutoLayout::Fill:
                button->setPosition("0%", "0%");
                button->setSize("100%", "100%");
                break;
            default:
                break;
        }
        spdlog::debug("Set button auto layout to: {}", static_cast<int>(layout));
        return *this;
    }

    ButtonBuilder& preserveAspectRatio(float ratio = 0.0f) {
        aspectRatio = (ratio > 0.0f) ? ratio : button->getSize().x / button->getSize().y;
        button->onSizeChange([this](tgui::Vector2f newSize) {
            if (aspectRatio > 0) {
                float height = newSize.x / aspectRatio;
                button->setSize({newSize.x, height});
                spdlog::debug("Button size adjusted to maintain aspect ratio: {}:{}", newSize.x, height);
            }
        });
        spdlog::debug("Button aspect ratio set to: {}", aspectRatio);
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
