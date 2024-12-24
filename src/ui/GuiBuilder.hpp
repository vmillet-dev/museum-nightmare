#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "ButtonBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

class GuiBuilder {
private:
    tgui::Group::Ptr mainGroup;
    tgui::Group* currentParent;
    tgui::Gui* gui;

public:
    GuiBuilder(tgui::Gui& gui) : gui(&gui) {
        mainGroup = tgui::Group::create();
        currentParent = mainGroup.get();
        tgui::Theme::setDefault("assets/themes/app.theme");
        spdlog::debug("GuiBuilder initialized with main group");
    }

    GuiBuilder& addVerticalLayout(const std::string& name = "") {
        auto layout = tgui::VerticalLayout::create();
        if (!name.empty()) {
            layout->setWidgetName(name);
            spdlog::debug("Created vertical layout with name: {}", name);
        }
        currentParent->add(layout);
        currentParent = layout.get();
        return *this;
    }

    GuiBuilder& addHorizontalLayout(const std::string& name = "") {
        auto layout = tgui::HorizontalLayout::create();
        if (!name.empty()) {
            layout->setWidgetName(name);
            spdlog::debug("Created horizontal layout with name: {}", name);
        }
        currentParent->add(layout);
        currentParent = layout.get();
        return *this;
    }

    GuiBuilder& addLabel(const std::string& text, const std::string& name = "") {
        auto label = tgui::Label::create(text);
        if (!name.empty()) {
            label->setWidgetName(name);
        }
        label->setTextSize(20);
        label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        currentParent->add(label);
        spdlog::debug("Added label with text: {}", text);
        return *this;
    }

    ButtonBuilder addButton(const std::string& text, const std::function<void()>& callback, const std::string& name = "") {
        auto button = tgui::Button::create(text);
        auto group = tgui::Group::create();
        group->setSize({ "100%", "100%" });
        group->add(button);
        if (!name.empty()) {
            button->setWidgetName(name);
        }
        button->onPress(callback);
        currentParent->add(group);
        spdlog::debug("Added button with text: {}", text);
        return ButtonBuilder(button, *this);
    }

    void build() {
        gui->add(mainGroup);
        spdlog::debug("Built GUI with all widgets");
    }
};

} // namespace game
