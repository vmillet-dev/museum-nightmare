#include "MenuBuilder.hpp"

namespace game {

// WidgetBuilder Implementation
WidgetBuilder::WidgetBuilder(MenuBuilder& parent, tgui::Widget::Ptr widget)
    : parent(parent), widget(widget) {}

WidgetBuilder& WidgetBuilder::setSize(tgui::Layout width, tgui::Layout height) {
    widget->setSize(width, height);
    return *this;
}

WidgetBuilder& WidgetBuilder::setPosition(tgui::Layout x, tgui::Layout y) {
    widget->setPosition(x, y);
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
    }
    else if (auto label = std::dynamic_pointer_cast<tgui::Label>(widget)) {
        label->setText(text);
    }
    return *this;
}

MenuBuilder& WidgetBuilder::build() {
    return parent;
}

// MenuBuilder Implementation
MenuBuilder::MenuBuilder(tgui::Gui* gui) : m_gui(gui) {
    m_container = tgui::Panel::create();
    m_container->setSize(tgui::Layout("100%"), tgui::Layout("100%"));

    // Create a default theme to ensure no invalid properties are inherited
    tgui::Theme::setDefault("");
}

WidgetBuilder MenuBuilder::addButton(const std::string& text, std::function<void()> callback) {
    auto button = tgui::Button::create(text);
    button->setSize(tgui::Layout(200), tgui::Layout(50));
    button->getRenderer()->setTextColor(tgui::Color::White);
    button->getRenderer()->setBorderColor(tgui::Color::White);
    button->getRenderer()->setBorders(2);
    button->setTextSize(24);
    button->onClick(callback);
    m_widgets.push_back(button);
    m_container->add(button);
    return WidgetBuilder(*this, button);
}

WidgetBuilder MenuBuilder::addLabel(const std::string& text) {
    auto label = tgui::Label::create(text);
    label->setSize(tgui::Layout(200), tgui::Layout(50));

    // Set alignment properties explicitly before theme application
    label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);

    // Set default properties that would normally come from theme
    label->getRenderer()->setTextColor(tgui::Color::White);
    label->setTextSize(24);

    m_widgets.push_back(label);
    m_container->add(label);
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
    m_theme = themePath;
    if (!m_theme.empty()) {
        // Load theme directly to widgets instead of setting as default
        tgui::Theme theme(m_theme);
        for (auto& widget : m_widgets) {
            theme.apply(widget);
        }
    }
    return *this;
}

void MenuBuilder::build() {
    arrangeWidgets();
    if (m_responsive) {
        m_container->setSize(tgui::Layout("100%"), tgui::Layout("100%"));
    }
    m_gui->add(m_container);
}

void MenuBuilder::arrangeWidgets() {
    float y = m_padding;
    float x = m_padding;

    for (auto& widget : m_widgets) {
        switch (m_layout) {
            case game::LayoutType::Vertical:
                widget->setPosition(tgui::Layout("50%"), tgui::Layout(y));
                widget->setOrigin(0.5f, 0.0f);
                y += widget->getSize().y + m_spacing;
                break;
            case game::LayoutType::Horizontal:
                widget->setPosition(tgui::Layout(x), tgui::Layout("50%"));
                widget->setOrigin(0.0f, 0.5f);
                x += widget->getSize().x + m_spacing;
                break;
            case game::LayoutType::Grid:
                // Grid layout implementation can be added here
                break;
        }
    }
}

} // namespace game
