#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

class GuiBuilder;

class ButtonBuilder {
private:
    tgui::Button::Ptr button;
    GuiBuilder& parent;

public:
    ButtonBuilder(tgui::Button::Ptr btn, GuiBuilder& p) : button(btn), parent(p) {
        button->setSize({ "50%", "35%" });
        setHorizontalAlignment(tgui::HorizontalAlignment::Center);
        setTextSize(20);
    }

    ButtonBuilder& setHorizontalAlignment(tgui::HorizontalAlignment align) {
        switch (align) {
        case tgui::HorizontalAlignment::Center:
            button->setPosition("(parent.innersize - size) / 2", "");
            break;
        case tgui::HorizontalAlignment::Right:
            button->setPosition("parent.innersize - size", "");
            break;
        case tgui::HorizontalAlignment::Left:
        default:
            button->setPosition("0%", "");
            break;
        }
        return *this;
    }

    ButtonBuilder& setTextSize(unsigned int size) {
        button->setTextSize(size);
        return *this;
    }

    ButtonBuilder& setBackgroundColor(const tgui::Color& color) {
        button->getRenderer()->setBackgroundColor(color);
        return *this;
    }

    GuiBuilder& endButton() {
        return parent;
    }
};