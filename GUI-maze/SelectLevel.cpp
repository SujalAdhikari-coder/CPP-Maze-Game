#include "SelectLevel.h"
#include <iostream>

SelectLevel::SelectLevel(sf::Font &font, sf::RenderWindow &window)
    : selectedIndex(-1), optionClicked(false)
{
    // Load the background image for level selection.
    if (!backgroundTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\board.jpg")) {
        std::cerr << "Failed to load level selection background from E:\\C++\\GUI\\GUI-maze\\assets\\board.jpg" << std::endl;
    } else {
        backgroundSprite.setTexture(backgroundTexture);
        // You can adjust scaling in the draw() method.
    }

    std::vector<std::string> options = {"Level 1 - Easy", "Level 2 - Hard", "Level 3 - Insane"};
    int numOptions = options.size();
    buttons.resize(numOptions);

    sf::Vector2f buttonSize(300.f, 60.f);
    float spacing = 20.f;
    float totalHeight = numOptions * buttonSize.y + (numOptions - 1) * spacing;
    sf::Vector2u winSize = window.getSize();
    float startY = (winSize.y - totalHeight) / 2.f;

    for (int i = 0; i < numOptions; ++i) {
        buttons[i].shape.setSize(buttonSize);
        buttons[i].shape.setFillColor(sf::Color(100, 100, 100));
        buttons[i].shape.setOutlineColor(sf::Color::White);
        buttons[i].shape.setOutlineThickness(2.f);

        float x = (winSize.x - buttonSize.x) / 2.f;
        float y = startY + i * (buttonSize.y + spacing);
        buttons[i].shape.setPosition(x, y);

        buttons[i].label.setFont(font);
        buttons[i].label.setString(options[i]);
        buttons[i].label.setCharacterSize(28);
        buttons[i].label.setFillColor(sf::Color::White);
        sf::FloatRect textRect = buttons[i].label.getLocalBounds();
        buttons[i].label.setOrigin(textRect.left + textRect.width / 2.f,
                                     textRect.top + textRect.height / 2.f);
        buttons[i].label.setPosition(x + buttonSize.x / 2.f, y + buttonSize.y / 2.f);
    }
}

void SelectLevel::handleEvent(const sf::Event &event) {
    optionClicked = false;

    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].shape.getGlobalBounds().contains(mousePos)) {
                selectedIndex = i;
                buttons[i].shape.setFillColor(sf::Color(150, 150, 150));
            } else {
                buttons[i].shape.setFillColor(sf::Color(100, 100, 100));
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].shape.getGlobalBounds().contains(mousePos)) {
                selectedIndex = i;
                optionClicked = true;
            }
        }
    }
}

void SelectLevel::update() {
    // You can add any per-frame updates or animations here.
}

void SelectLevel::draw(sf::RenderWindow &window) {
    // Draw the background image scaled to fill the window.
    sf::Vector2u winSize = window.getSize();
    backgroundSprite.setScale(
        static_cast<float>(winSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(winSize.y) / backgroundTexture.getSize().y);
    backgroundSprite.setPosition(0, 0);
    window.draw(backgroundSprite);

    // Draw each button.
    for (const auto &button : buttons) {
        window.draw(button.shape);
        window.draw(button.label);
    }
}

bool SelectLevel::isLevelSelected() const {
    return optionClicked && selectedIndex >= 0 && selectedIndex < static_cast<int>(buttons.size());
}

int SelectLevel::getSelectedLevel() const {
    if (isLevelSelected())
        return selectedIndex + 1;
    return -1;
}
