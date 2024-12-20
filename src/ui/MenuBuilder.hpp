#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <spdlog/spdlog.h>

namespace game {

class MenuBuilder : public std::enable_shared_from_this<MenuBuilder> {
public:
    using Ptr = std::shared_ptr<MenuBuilder>;
    using WidgetPtr = tgui::Widget::Ptr;
    using ContainerPtr = tgui::Container::Ptr;
    using Callback = std::function<void()>;

    // Constructor
    explicit MenuBuilder(tgui::Gui* gui);

    // Basic widget creation
    Ptr addButton(const std::string& name, const std::string& text);
    Ptr addLabel(const std::string& name, const std::string& text);

    // Layout management
    Ptr setVerticalLayout();
    Ptr setHorizontalLayout();
    Ptr setSpacing(float spacing);
    Ptr setPadding(float padding);
    Ptr setResponsive(bool enabled = true);

    // Event handling
    Ptr onClick(const std::string& widgetName, Callback callback);
    Ptr onHover(const std::string& widgetName, Callback callback);
    Ptr onFocus(const std::string& widgetName, Callback callback);

    // Theme management
    Ptr setTheme(const std::string& themePath);

    // Build method
    ContainerPtr build();

private:
    tgui::Gui* m_gui;
    ContainerPtr m_container;
    std::unordered_map<std::string, WidgetPtr> m_widgets;
    float m_spacing{20.f};
    float m_padding{50.f};
    bool m_responsive{false};
    std::string m_theme;
};

} // namespace game
