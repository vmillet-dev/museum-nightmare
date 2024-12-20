#include "MenuBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

MenuBuilder::MenuBuilder(tgui::Gui* gui) : m_gui(gui) {
    m_container = tgui::Panel::create();
    m_container->setSize("100%", "100%");
}

MenuBuilder::Ptr MenuBuilder::addButton(const std::string& name, const std::string& text) {
    auto button = tgui::Button::create(text);
    button->setSize(200, 50);
    button->getRenderer()->setTextColor(tgui::Color::White);
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setBorders(2);
    button->setTextSize(24);
    m_widgets[name] = button;
    m_container->add(button, name);
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::addLabel(const std::string& name, const std::string& text) {
    auto label = tgui::Label::create(text);
    label->setTextSize(24);
    label->getRenderer()->setTextColor(tgui::Color::White);
    m_widgets[name] = label;
    m_container->add(label, name);
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::setVerticalLayout() {
    auto layout = tgui::VerticalLayout::create();
    layout->setSize("100%", "100%");
    layout->getRenderer()->setPadding({m_padding});
    layout->getRenderer()->setSpaceBetweenWidgets(m_spacing);

    for (const auto& [name, widget] : m_widgets) {
        layout->add(widget);
    }

    m_container->removeAllWidgets();
    m_container->add(layout);
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::setHorizontalLayout() {
    auto layout = tgui::HorizontalLayout::create();
    layout->setSize("100%", "100%");
    layout->getRenderer()->setPadding({m_padding});
    layout->getRenderer()->setSpaceBetweenWidgets(m_spacing);

    for (const auto& [name, widget] : m_widgets) {
        layout->add(widget);
    }

    m_container->removeAllWidgets();
    m_container->add(layout);
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::setSpacing(float spacing) {
    m_spacing = spacing;
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::setPadding(float padding) {
    m_padding = padding;
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::setResponsive(bool enabled) {
    m_responsive = enabled;
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::onClick(const std::string& widgetName, Callback callback) {
    if (auto widget = m_widgets.find(widgetName); widget != m_widgets.end()) {
        if (auto button = std::dynamic_pointer_cast<tgui::Button>(widget->second)) {
            button->onPress(callback);
        }
    }
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::onHover(const std::string& widgetName, Callback callback) {
    if (auto widget = m_widgets.find(widgetName); widget != m_widgets.end()) {
        widget->second->onMouseEnter(callback);
    }
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::onFocus(const std::string& widgetName, Callback callback) {
    if (auto widget = m_widgets.find(widgetName); widget != m_widgets.end()) {
        widget->second->onFocus(callback);
    }
    return shared_from_this();
}

MenuBuilder::Ptr MenuBuilder::setTheme(const std::string& themePath) {
    m_theme = themePath;
    if (!m_theme.empty()) {
        tgui::Theme::setDefault(m_theme);
    }
    return shared_from_this();
}

MenuBuilder::ContainerPtr MenuBuilder::build() {
    if (m_responsive) {
        m_container->setSize("100%", "100%");
    }
    m_gui->add(m_container);
    return m_container;
}

} // namespace game
