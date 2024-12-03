#include "MenuBuilder.hpp"

namespace game {

Button* MenuBuilder::addButton(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size,
                             const WidgetStyle& style) {
    auto button = std::make_unique<Button>(text, position, size, style);
    Button* buttonPtr = button.get();
    widgets_.push_back(std::move(button));
    updateSelection();
    return buttonPtr;
}

Dropdown* MenuBuilder::addDropdown(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size,
                                 const std::vector<std::string>& options, const WidgetStyle& style) {
    auto dropdown = std::make_unique<Dropdown>(label, position, size, options, style);
    Dropdown* dropdownPtr = dropdown.get();
    widgets_.push_back(std::move(dropdown));
    updateSelection();
    return dropdownPtr;
}

void MenuBuilder::update(InputManager& inputManager) {
    // Handle navigation
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        selectPrevious();
    }
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        selectNext();
    }

    // Update all widgets
    for (auto& widget : widgets_) {
        widget->update(inputManager);
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
