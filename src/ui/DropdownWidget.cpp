#include "DropdownWidget.hpp"

namespace game {

DropdownWidget::DropdownWidget(const sf::Vector2f& position, const sf::Vector2f& size,
                             const std::vector<std::string>& options)
    : Widget(position, size), options(options) {

    if (!font.loadFromFile("assets/fonts/pixelated.ttf")) {
        spdlog::error("Failed to load font in DropdownWidget!");
    }

    // Setup main box
    mainBox.setSize(size);
    mainBox.setPosition(position - sf::Vector2f(size.x / 2, size.y / 2));
    mainBox.setFillColor(backgroundColor);
    mainBox.setOutlineThickness(2);
    mainBox.setOutlineColor(sf::Color::White);

    // Setup dropdown box
    dropdownBox.setSize(sf::Vector2f(size.x, optionHeight * options.size()));
    dropdownBox.setPosition(mainBox.getPosition() + sf::Vector2f(0, size.y));
    dropdownBox.setFillColor(backgroundColor);
    dropdownBox.setOutlineThickness(2);
    dropdownBox.setOutlineColor(sf::Color::White);

    // Setup current selection text
    currentText.setFont(font);
    currentText.setCharacterSize(20);
    currentText.setFillColor(textColor);

    // Setup option texts
    for (const auto& option : options) {
        sf::Text text;
        text.setFont(font);
        text.setString(option);
        text.setCharacterSize(20);
        text.setFillColor(textColor);
        optionTexts.push_back(text);
    }

    // Set initial selection
    if (!options.empty()) {
        setCurrentSelection(0);
    }
}

void DropdownWidget::update(InputManager& inputManager) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(inputManager.getWindow()));
    bool wasHovered = hovered;
    hovered = isMouseOver(mousePos);

    // Handle main box click
    if (hovered && inputManager.isActionJustPressed(Action::Fire)) {
        toggleDropdown();
    }

    // Handle option selection when dropdown is open
    if (isOpen) {
        hoveredOption = static_cast<size_t>(-1);
        for (size_t i = 0; i < options.size(); ++i) {
            if (isOptionHovered(i, mousePos)) {
                hoveredOption = i;
                if (inputManager.isActionJustPressed(Action::Fire)) {
                    setCurrentSelection(i);
                    toggleDropdown();
                    if (onSelectionChanged) {
                        onSelectionChanged(i);
                    }
                }
                break;
            }
        }
    }

    updateVisualState();
}

void DropdownWidget::render(sf::RenderWindow& window) {
    window.draw(mainBox);
    window.draw(currentText);

    if (isOpen) {
        window.draw(dropdownBox);
        for (size_t i = 0; i < optionTexts.size(); ++i) {
            window.draw(optionTexts[i]);
        }
    }
}

void DropdownWidget::setCurrentSelection(size_t index) {
    if (index < options.size()) {
        currentSelection = index;
        currentText.setString(options[currentSelection]);

        // Center the text in the main box
        sf::FloatRect textBounds = currentText.getLocalBounds();
        currentText.setPosition(
            position.x - textBounds.width / 2,
            position.y - textBounds.height / 2
        );
    }
}

void DropdownWidget::updateVisualState() {
    mainBox.setFillColor(hovered ? hoverColor : backgroundColor);

    // Update option positions and colors
    for (size_t i = 0; i < optionTexts.size(); ++i) {
        sf::Vector2f optionPos(
            position.x - size.x / 2 + 10,  // Add padding
            position.y + size.y / 2 + i * optionHeight + optionHeight / 2
        );
        optionTexts[i].setPosition(optionPos);

        if (i == hoveredOption) {
            optionTexts[i].setFillColor(selectedColor);
        } else {
            optionTexts[i].setFillColor(textColor);
        }
    }
}

void DropdownWidget::toggleDropdown() {
    isOpen = !isOpen;
    spdlog::debug("Dropdown toggled: {}", isOpen ? "opened" : "closed");
}

bool DropdownWidget::isOptionHovered(size_t index, const sf::Vector2f& mousePos) const {
    sf::Vector2f optionPos = mainBox.getPosition() + sf::Vector2f(0, size.y + index * optionHeight);
    return mousePos.x >= optionPos.x &&
           mousePos.x <= optionPos.x + size.x &&
           mousePos.y >= optionPos.y &&
           mousePos.y <= optionPos.y + optionHeight;
}

} // namespace game
