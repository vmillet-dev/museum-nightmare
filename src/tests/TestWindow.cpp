#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "../ui/GuiBuilder.hpp"
#include <spdlog/spdlog.h>
#include <cassert>
#include <thread>
#include <chrono>

void setWidgetSizeWithRatio(tgui::Widget::Ptr widget, float ratio, float containerWidth) {
    float width = containerWidth;
    float height = width / ratio;
    widget->setSize({width, height});

    spdlog::debug("Setting widget '{}' size to {}x{} (ratio: {})",
        widget->getWidgetName().toStdString(), width, height, ratio);
}

void updateWidgetPosition(tgui::Widget::Ptr widget, tgui::AutoLayout layout, tgui::Vector2f containerSize) {
    switch (layout) {
        case tgui::AutoLayout::Top:
            widget->setPosition({containerSize.x / 2.f - widget->getSize().x / 2.f, 10.f});
            break;
        case tgui::AutoLayout::Fill:
            widget->setPosition({10.f, containerSize.y / 4.f});
            break;
        case tgui::AutoLayout::Left:
            widget->setPosition({10.f, containerSize.y / 2.f});
            break;
        case tgui::AutoLayout::Right:
            widget->setPosition({containerSize.x - widget->getSize().x - 10.f, containerSize.y / 2.f});
            break;
        default:
            break;
    }

    spdlog::debug("Updated widget '{}' position to {}x{}",
        widget->getWidgetName().toStdString(), widget->getPosition().x, widget->getPosition().y);
}

void verifyAspectRatio(tgui::Widget::Ptr widget, float expectedRatio, float tolerance = 0.01f) {
    auto size = widget->getSize();
    if (size.x <= 0 || size.y <= 0) {
        spdlog::warn("Widget '{}' has invalid size: {}x{}",
            widget->getWidgetName().toStdString(), size.x, size.y);
        return;
    }

    float actualRatio = size.x / size.y;
    float difference = std::abs(actualRatio - expectedRatio);
    if (difference > tolerance) {
        spdlog::error("Aspect ratio mismatch for widget '{}': expected {}, got {} (size: {}x{})",
            widget->getWidgetName().toStdString(), expectedRatio, actualRatio, size.x, size.y);
        assert(false && "Aspect ratio preservation failed");
    } else {
        spdlog::debug("Widget '{}' ratio verified: {} (size: {}x{})",
            widget->getWidgetName().toStdString(), actualRatio, size.x, size.y);
    }
}

void verifyAutoLayout(tgui::Widget::Ptr widget, tgui::AutoLayout layout, tgui::Vector2f containerSize) {
    auto pos = widget->getPosition();
    auto size = widget->getSize();

    switch (layout) {
        case tgui::AutoLayout::Top:
            assert(pos.y <= 20.f && "Top layout verification failed");
            break;
        case tgui::AutoLayout::Fill:
            assert(pos.x >= 0 && pos.x + size.x <= containerSize.x && "Fill layout width verification failed");
            break;
        case tgui::AutoLayout::Left:
            assert(pos.x <= 20.f && "Left layout verification failed");
            break;
        case tgui::AutoLayout::Right:
            assert(pos.x >= containerSize.x - size.x - 20.f && "Right layout verification failed");
            break;
        default:
            break;
    }

    spdlog::debug("Layout position verified for widget '{}' at {}x{}",
        widget->getWidgetName().toStdString(), pos.x, pos.y);
}

void initializeWidget(tgui::Widget::Ptr widget, const std::string& name, float ratio, float containerWidth, tgui::AutoLayout layout, tgui::Vector2f containerSize) {
    widget->setWidgetName(name);
    setWidgetSizeWithRatio(widget, ratio, containerWidth);
    updateWidgetPosition(widget, layout, containerSize);

    verifyAspectRatio(widget, ratio);
    verifyAutoLayout(widget, layout, containerSize);
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Starting TGUI ratio test window");

    sf::RenderWindow window(sf::VideoMode(800, 600), "TGUI Ratio Test");
    tgui::Gui gui;
    gui.setTarget(window);

    struct TestWidgets {
        tgui::Label::Ptr label;
        tgui::Button::Ptr button169;
        tgui::Button::Ptr button43;
        tgui::Button::Ptr button11;
    } widgets;

    auto container = tgui::Group::create();
    container->setSize(window.getSize().x, window.getSize().y);
    gui.add(container);

    widgets.label = tgui::Label::create("Ratio Test - Top");
    widgets.label->setWidgetName("HeaderLabel");
    widgets.label->setTextSize(20);
    container->add(widgets.label);

    widgets.button169 = tgui::Button::create("16:9 Button");
    widgets.button43 = tgui::Button::create("4:3 Button");
    widgets.button11 = tgui::Button::create("1:1 Button");

    container->add(widgets.button169);
    container->add(widgets.button43);
    container->add(widgets.button11);

    tgui::Vector2f containerSize = container->getSize();

    updateWidgetPosition(widgets.label, tgui::AutoLayout::Top, containerSize);

    initializeWidget(widgets.button169, "Button169", 16.0f/9.0f, containerSize.x - 20.f,
                    tgui::AutoLayout::Fill, containerSize);
    initializeWidget(widgets.button43, "Button43", 4.0f/3.0f, containerSize.x / 3,
                    tgui::AutoLayout::Left, containerSize);
    initializeWidget(widgets.button11, "Button11", 1.0f, containerSize.x / 3,
                    tgui::AutoLayout::Right, containerSize);

    spdlog::info("Test window created. Monitoring widget properties during resize...");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                containerSize = {static_cast<float>(event.size.width),
                               static_cast<float>(event.size.height)};
                container->setSize(containerSize);
                window.setView(sf::View(sf::FloatRect(0, 0, containerSize.x, containerSize.y)));
                gui.setTarget(window);

                spdlog::debug("Window resized to {}x{}", containerSize.x, containerSize.y);

                updateWidgetPosition(widgets.label, tgui::AutoLayout::Top, containerSize);

                initializeWidget(widgets.button169, "Button169", 16.0f/9.0f, containerSize.x - 20.f,
                               tgui::AutoLayout::Fill, containerSize);
                initializeWidget(widgets.button43, "Button43", 4.0f/3.0f, containerSize.x / 3,
                               tgui::AutoLayout::Left, containerSize);
                initializeWidget(widgets.button11, "Button11", 1.0f, containerSize.x / 3,
                               tgui::AutoLayout::Right, containerSize);
            }
        }

        window.clear(sf::Color(64, 64, 64));
        gui.draw();
        window.display();
    }

    return 0;
}
