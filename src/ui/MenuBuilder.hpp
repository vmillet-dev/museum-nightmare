#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "../input/InputManager.hpp"

namespace game {

// Layout type enum
enum class LayoutType {
    Vertical,
    Horizontal,
    Grid
};

class MenuBuilder;

// WidgetBuilder class for configuring individual widgets
class WidgetBuilder {
public:
    WidgetBuilder(MenuBuilder& parent, tgui::Widget::Ptr widget);

    // Setter methods with chaining
    WidgetBuilder& setSize(tgui::Layout width, tgui::Layout height);
    WidgetBuilder& setPosition(tgui::Layout x, tgui::Layout y);
    WidgetBuilder& setColor(const tgui::Color& color);
    WidgetBuilder& setText(const std::string& text);

    // Finish configuration and return to MenuBuilder
    MenuBuilder& build();

private:
    MenuBuilder& parent;
    tgui::Widget::Ptr widget;
};

// MenuBuilder class for creating and managing widgets
class MenuBuilder {
public:
    // Constructor
    explicit MenuBuilder(tgui::Gui* gui);

    // Widget creation methods returning WidgetBuilder for chaining
    WidgetBuilder addButton(const std::string& text, std::function<void()> callback);
    WidgetBuilder addLabel(const std::string& text);

    // Layout management
    MenuBuilder& setLayout(LayoutType type);
    MenuBuilder& setSpacing(float spacing);
    MenuBuilder& setPadding(float padding);
    MenuBuilder& setResponsive(bool enabled = true);

    // Theme management
    MenuBuilder& setTheme(const std::string& themePath);

    // Input handling
    void handleInput(const InputManager& inputManager);
    void handleEvent(const sf::Event& event);

    // Build method
    void build();

    // Friend declaration to allow WidgetBuilder access
    friend class WidgetBuilder;

private:
    tgui::Gui* m_gui;
    tgui::Container::Ptr m_container;
    std::vector<tgui::Widget::Ptr> m_widgets;
    std::vector<tgui::Widget::Ptr> m_focusableWidgets;
    size_t m_currentFocusIndex{0};
    float m_spacing{20.f};
    float m_padding{50.f};
    bool m_responsive{false};
    std::string m_theme;
    LayoutType m_layout{LayoutType::Vertical};

    void arrangeWidgets();
    void updateFocus();
};

} // namespace game
