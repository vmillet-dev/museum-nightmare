#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace game {

class MenuBuilder;

// WidgetBuilder class for configuring individual widgets
class WidgetBuilder {
public:
    WidgetBuilder(MenuBuilder& parent, tgui::Widget::Ptr widget);

    // Setter methods with chaining
    WidgetBuilder& setSize(const std::string& width, const std::string& height);
    WidgetBuilder& setPosition(const std::string& x, const std::string& y);
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

    // Build method
    void build();

    // Friend declaration to allow WidgetBuilder access
    friend class WidgetBuilder;

private:
    tgui::Gui* m_gui;
    tgui::Container::Ptr m_container;
    std::vector<tgui::Widget::Ptr> m_widgets;
    float m_spacing{20.f};
    float m_padding{50.f};
    bool m_responsive{false};
    std::string m_theme;

    // Layout type enum
    enum class LayoutType {
        Vertical,
        Horizontal,
        Grid
    } m_layout{LayoutType::Vertical};

    void arrangeWidgets();
};

} // namespace game
