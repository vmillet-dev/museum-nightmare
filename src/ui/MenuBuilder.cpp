#include "MenuBuilder.hpp"
#include "../core/Game.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace game {

MenuBuilder::MenuBuilder(Game& game) : game_(game) {
    startX_ = game.getWindow().getSize().x / 2.f;  // Center horizontally
}

MenuBuilder& MenuBuilder::addButton(const std::string& text, float x, float y, ButtonCallback callback) {
    const sf::Vector2f size(200.f, 50.f);  // Default button size
    // Use the provided y position instead of nextY_
    auto button = std::make_unique<Button>(text, sf::Vector2f(x, y), size);
    Button* buttonPtr = button.get();
    widgets_.push_back(std::move(button));
    callbacks_.push_back(std::move(callback));
    updateSelection();
    return *this;
}

Dropdown* MenuBuilder::addDropdown(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size,
                                 const std::vector<std::string>& options, const WidgetStyle& style) {
    auto dropdown = std::make_unique<Dropdown>(label, position, size, options, style);
    Dropdown* dropdownPtr = dropdown.get();
    widgets_.push_back(std::move(dropdown));
    callbacks_.push_back(nullptr);  // No callback for dropdown
    updateSelection();
    return dropdownPtr;
}

std::vector<Button*> MenuBuilder::build() {
    std::vector<Button*> buttons;
    for (const auto& widget : widgets_) {
        if (auto button = dynamic_cast<Button*>(widget.get())) {
            buttons.push_back(button);
        }
    }
    return buttons;
}

void MenuBuilder::update(InputManager& inputManager) {
    // Handle navigation
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        selectPrevious();
    }
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        selectNext();
    }

    // Update all widgets and handle callbacks
    for (size_t i = 0; i < widgets_.size(); ++i) {
        widgets_[i]->update(inputManager);
        if (auto button = dynamic_cast<Button*>(widgets_[i].get())) {
            if (button->isClicked()) {
                executeCallback(i);
            }
        }
    }
}

void MenuBuilder::executeCallback(size_t index) {
    if (index < callbacks_.size() && callbacks_[index]) {
        callbacks_[index]();
    }
}

void MenuBuilder::render(sf::RenderWindow& window) {
    for (auto& widget : widgets_) {
        widget->render(window);
    }
}

void MenuBuilder::selectNext() {
    if (widgets_.empty()) return;

    size_t startIndex = selectedIndex_;
    do {
        selectedIndex_ = (selectedIndex_ + 1) % widgets_.size();
    } while (!widgets_[selectedIndex_]->isFocusable() && selectedIndex_ != startIndex);

    updateSelection();
}

void MenuBuilder::selectPrevious() {
    if (widgets_.empty()) return;

    size_t startIndex = selectedIndex_;
    do {
        selectedIndex_ = (selectedIndex_ == 0) ? widgets_.size() - 1 : selectedIndex_ - 1;
    } while (!widgets_[selectedIndex_]->isFocusable() && selectedIndex_ != startIndex);

    updateSelection();
}

Widget* MenuBuilder::getSelectedWidget() {
    return widgets_.empty() ? nullptr : widgets_[selectedIndex_].get();
}

void MenuBuilder::updateSelection() {
    for (size_t i = 0; i < widgets_.size(); ++i) {
        widgets_[i]->setSelected(i == selectedIndex_);
    }
}

} // namespace game
