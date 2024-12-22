#include "MenuBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

// WidgetBuilder Implementation
WidgetBuilder::WidgetBuilder(MenuBuilder& parent, tgui::Widget::Ptr widget)
    : parent(parent), widget(widget) {}

WidgetBuilder& WidgetBuilder::setSize(tgui::Layout width, tgui::Layout height) {
    widget->setSize(width, height);
    spdlog::debug("Widget size set to ({}, {})", width.toString().toStdString(), height.toString().toStdString());
    return *this;
}

WidgetBuilder& WidgetBuilder::setPosition(tgui::Layout x, tgui::Layout y) {
    widget->setPosition(x, y);
    spdlog::debug("Widget position set to ({}, {})", x.toString().toStdString(), y.toString().toStdString());
    return *this;
}

WidgetBuilder& WidgetBuilder::setColor(const tgui::Color& color) {
    if (auto button = std::dynamic_pointer_cast<tgui::Button>(widget)) {
        button->getRenderer()->setBackgroundColor(color);
    }
    return *this;
}

WidgetBuilder& WidgetBuilder::setText(const std::string& text) {
    if (auto button = std::dynamic_pointer_cast<tgui::Button>(widget)) {
        button->setText(text);
        spdlog::debug("Button text set to: {}", text);
    }
    else if (auto label = std::dynamic_pointer_cast<tgui::Label>(widget)) {
        label->setText(text);
        spdlog::debug("Label text set to: {}", text);
    }
    return *this;
}

MenuBuilder& WidgetBuilder::build() {
    return parent;
}

// MenuBuilder Implementation
MenuBuilder::MenuBuilder(tgui::Gui* gui) : m_gui(gui) {
    spdlog::info("Initializing MenuBuilder with GUI");
    m_container = tgui::Panel::create();
    m_container->setSize(tgui::Layout("100%"), tgui::Layout("100%"));
    spdlog::debug("Created container panel with 100% size");

    tgui::Theme::setDefault("");
    spdlog::debug("Set default TGUI theme");
}

WidgetBuilder MenuBuilder::addButton(const std::string& text, std::function<void()> callback) {
    spdlog::debug("Adding button with text: {}", text);
    auto button = tgui::Button::create(text);
    button->setSize(tgui::Layout(200), tgui::Layout(50));
    button->getRenderer()->setTextColor(tgui::Color::White);
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setBorders(2);
    button->setTextSize(24);
    button->onClick(callback);
    m_widgets.push_back(button);
    m_container->add(button);
    spdlog::debug("Button created and added to container");
    return WidgetBuilder(*this, button);
}

WidgetBuilder MenuBuilder::addLabel(const std::string& text) {
    spdlog::debug("Adding label with text: {}", text);
    auto label = tgui::Label::create(text);
    label->setSize(tgui::Layout(200), tgui::Layout(50));
    label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    label->setVerticalAlignment(tgui::VerticalAlignment::Center);
    label->getRenderer()->setTextColor(tgui::Color::White);
    label->setTextSize(24);
    m_widgets.push_back(label);
    m_container->add(label);
    spdlog::debug("Label created and added to container");
    return WidgetBuilder(*this, label);
}

MenuBuilder& MenuBuilder::setLayout(game::LayoutType type) {
    m_layout = type;
    return *this;
}

MenuBuilder& MenuBuilder::setSpacing(float spacing) {
    m_spacing = spacing;
    return *this;
}

MenuBuilder& MenuBuilder::setPadding(float padding) {
    m_padding = padding;
    return *this;
}

MenuBuilder& MenuBuilder::setResponsive(bool enabled) {
    m_responsive = enabled;
    return *this;
}

MenuBuilder& MenuBuilder::setTheme(const std::string& themePath) {
    spdlog::info("Applying theme from path: {}", themePath);
    m_theme = themePath;
    if (!m_theme.empty()) {
        try {
            tgui::Theme theme(m_theme);
            for (auto& widget : m_widgets) {
                widget->setRenderer(theme.getRenderer(widget->getWidgetType()));
                spdlog::debug("Applied theme to widget of type: {}", widget->getWidgetType().toStdString());
            }
        }
        catch (const tgui::Exception& e) {
            spdlog::error("Failed to load theme: {}", e.what());
        }
    }
    return *this;
}

void MenuBuilder::build() {
    spdlog::info("Building menu with {} widgets", m_widgets.size());
    arrangeWidgets();
    if (m_responsive) {
        m_container->setSize(tgui::Layout("100%"), tgui::Layout("100%"));
        spdlog::debug("Set container to responsive mode");
    }
    m_gui->add(m_container);
    spdlog::info("Menu built and added to GUI");
}

void MenuBuilder::arrangeWidgets() {
    spdlog::debug("Arranging widgets with layout type: {}", static_cast<int>(m_layout));
    float y = m_padding;
    float x = m_padding;

    for (auto& widget : m_widgets) {
        switch (m_layout) {
            case LayoutType::Vertical:
                widget->setPosition(tgui::Layout("50%"), tgui::Layout(y));
                widget->setOrigin(0.5f, 0.0f);
                y += widget->getSize().y + m_spacing;
                spdlog::debug("Positioned widget vertically at y: {}", y);
                break;
            case LayoutType::Horizontal:
                widget->setPosition(tgui::Layout(x), tgui::Layout("50%"));
                widget->setOrigin(0.0f, 0.5f);
                x += widget->getSize().x + m_spacing;
                spdlog::debug("Positioned widget horizontally at x: {}", x);
                break;
            case LayoutType::Grid:
                spdlog::debug("Grid layout not implemented yet");
                break;
        }
    }
}

} // namespace game
