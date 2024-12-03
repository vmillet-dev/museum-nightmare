#include "MenuBuilder.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

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

void MenuBuilder::centerHorizontally(float y, float spacing) {
    if (widgets_.empty()) return;

    // Calculate total width of all widgets
    float totalWidth = 0;
    for (const auto& widget : widgets_) {
        totalWidth += widget->getSize().x;
    }
    totalWidth += spacing * (widgets_.size() - 1);

    // Get window width (assuming first widget has valid window reference)
    float windowWidth = widgets_[0]->getPosition().x * 2;  // Approximate window width
    float startX = (windowWidth - totalWidth) / 2;

    // Position widgets
    float currentX = startX;
    for (auto& widget : widgets_) {
        widget->setPosition(sf::Vector2f(currentX, y));
        currentX += widget->getSize().x + spacing;
    }
}

void MenuBuilder::alignVertically(float x, float spacing) {
    if (widgets_.empty()) return;

    float currentY = widgets_[0]->getPosition().y;
    for (auto& widget : widgets_) {
        widget->setPosition(sf::Vector2f(x, currentY));
        currentY += widget->getSize().y + spacing;
    }
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
