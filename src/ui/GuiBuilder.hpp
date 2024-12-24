#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Layout.hpp>
#include "ButtonBuilder.hpp"
#include <spdlog/spdlog.h>
#include <map>

namespace game {

class GuiBuilder {
private:
    tgui::Group::Ptr mainGroup;
    tgui::Group* currentParent;
    tgui::Gui* gui;
    std::map<tgui::Widget::Ptr, tgui::AutoLayout> autoLayouts;

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

    ButtonBuilder addButton(const std::string& text, const std::function<void()>& callback) {
        auto button = tgui::Button::create(text);
        button->onPress(callback);

        // Set default size with 16:9 ratio
        button->setSize({"50%", "28.125%"});  // 16:9 ratio
        currentParent->add(button);

        return ButtonBuilder(button, *this).preserveAspectRatio();
    }

    GuiBuilder& setAutoLayout(tgui::AutoLayout layout) {
        if (currentParent && !currentParent->getWidgets().empty()) {
            auto lastWidget = currentParent->getWidgets().back();
            autoLayouts[lastWidget] = layout;
            updateWidgetLayout(lastWidget, layout);
            spdlog::debug("Set AutoLayout for widget: {}", static_cast<int>(layout));
        }
        return *this;
    }

    void build() {
        gui->add(mainGroup);
        spdlog::debug("Built GUI with all widgets");
    }

private:
    void updateWidgetLayout(tgui::Widget::Ptr widget, tgui::AutoLayout layout) {
        switch (layout) {
            case tgui::AutoLayout::Top:
                widget->setPosition("0%", "0%");
                widget->setWidth("100%");
                spdlog::debug("Applied Top layout");
                break;
            case tgui::AutoLayout::Bottom:
                widget->setPosition("0%", "100% - height");
                widget->setWidth("100%");
                spdlog::debug("Applied Bottom layout");
                break;
            case tgui::AutoLayout::Left:
                widget->setPosition("0%", "0%");
                widget->setHeight("100%");
                spdlog::debug("Applied Left layout");
                break;
            case tgui::AutoLayout::Right:
                widget->setPosition("100% - width", "0%");
                widget->setHeight("100%");
                spdlog::debug("Applied Right layout");
                break;
            case tgui::AutoLayout::Fill:
                widget->setPosition("0%", "0%");
                widget->setSize("100%", "100%");
                spdlog::debug("Applied Fill layout");
                break;
            default:
                spdlog::debug("Applied Manual layout");
                break;
        }
    }
};

} // namespace game
