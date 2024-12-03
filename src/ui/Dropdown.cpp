#include "Dropdown.hpp"
#include <spdlog/spdlog.h>

namespace game {

Dropdown::Dropdown(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size,
                 const std::vector<std::string>& options, const WidgetStyle& style)
    : style_(style), options_(options) {

    // Load font
    if (!font_.loadFromFile(style_.fontPath)) {
        spdlog::error("Failed to load font in Dropdown!");
    }

    // Setup main box
    mainBox_.setSize(size);
    mainBox_.setPosition(position);
    mainBox_.setFillColor(style_.defaultColor);
    mainBox_.setOutlineThickness(style_.outlineThickness);
    mainBox_.setOutlineColor(style_.outlineColor);

    // Setup dropdown box (hidden initially)
    float dropdownHeight = size.y * std::min(options_.size(), size_t(5));
    dropdownBox_.setSize(sf::Vector2f(size.x, dropdownHeight));
    dropdownBox_.setPosition(position.x, position.y + size.y);
    dropdownBox_.setFillColor(style_.defaultColor);
    dropdownBox_.setOutlineThickness(style_.outlineThickness);
    dropdownBox_.setOutlineColor(style_.outlineColor);

    // Setup label text
    labelText_.setFont(font_);
    labelText_.setString(label + ":");
    labelText_.setCharacterSize(style_.characterSize);
    labelText_.setFillColor(style_.textColor);

    // Setup selected text
    selectedText_.setFont(font_);
    selectedText_.setCharacterSize(style_.characterSize);
    selectedText_.setFillColor(style_.textColor);

    // Store widget properties
    position_ = position;
    size_ = size;

    if (!options_.empty()) {
        setSelectedIndex(0);
    }

    updateTextPositions();
}

void Dropdown::update(InputManager& inputManager) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));

    // Update hover state for main box
    isHovered_ = isMouseOver(mousePos);

    // Update colors based on state
    mainBox_.setFillColor(isHovered_ || isSelected_ ? style_.selectedColor : style_.defaultColor);

    bool isPressed = inputManager.isActionPressed(Action::Fire) || inputManager.isActionPressed(Action::Confirm);

    if (isPressed && !wasPressed_) {
        if (isHovered_) {
            toggleExpanded();
        } else if (isExpanded_) {
            size_t hoveredIndex = getHoveredOptionIndex(mousePos);
            if (hoveredIndex < options_.size()) {
                setSelectedIndex(hoveredIndex);
                toggleExpanded();
            }
        }
    }

    wasPressed_ = isPressed;
}

void Dropdown::render(sf::RenderWindow& window) {
    window.draw(mainBox_);
    window.draw(labelText_);
    window.draw(selectedText_);

    if (isExpanded_) {
        window.draw(dropdownBox_);

        // Draw options
        sf::Text optionText;
        optionText.setFont(font_);
        optionText.setCharacterSize(style_.characterSize);
        optionText.setFillColor(style_.textColor);

        for (size_t i = 0; i < options_.size(); ++i) {
            optionText.setString(options_[i]);
            optionText.setPosition(
                dropdownBox_.getPosition().x + 5,
                dropdownBox_.getPosition().y + (i * size_.y) + (size_.y - style_.characterSize) / 2
            );
            window.draw(optionText);
        }
    }
}

void Dropdown::setOptions(const std::vector<std::string>& options) {
    options_ = options;
    selectedIndex_ = 0;
    if (!options_.empty()) {
        selectedText_.setString(options_[selectedIndex_]);
    }
    updateTextPositions();
}

void Dropdown::setSelectedIndex(size_t index) {
    if (index < options_.size()) {
        selectedIndex_ = index;
        selectedText_.setString(options_[selectedIndex_]);
        updateTextPositions();
    }
}

void Dropdown::updateTextPositions() {
    // Position label text
    sf::FloatRect labelBounds = labelText_.getLocalBounds();
    labelText_.setPosition(
        position_.x + 5,
        position_.y + (size_.y - labelBounds.height) / 2
    );

    // Position selected text
    sf::FloatRect selectedBounds = selectedText_.getLocalBounds();
    selectedText_.setPosition(
        position_.x + size_.x - selectedBounds.width - 25,  // Leave space for dropdown arrow
        position_.y + (size_.y - selectedBounds.height) / 2
    );
}

size_t Dropdown::getHoveredOptionIndex(const sf::Vector2f& mousePos) const {
    if (!isExpanded_) return options_.size();

    sf::FloatRect dropdownBounds = dropdownBox_.getGlobalBounds();
    if (!dropdownBounds.contains(mousePos)) return options_.size();

    float relativeY = mousePos.y - dropdownBounds.top;
    size_t index = static_cast<size_t>(relativeY / size_.y);

    return (index < options_.size()) ? index : options_.size();
}

void Dropdown::toggleExpanded() {
    isExpanded_ = !isExpanded_;
}

} // namespace game
