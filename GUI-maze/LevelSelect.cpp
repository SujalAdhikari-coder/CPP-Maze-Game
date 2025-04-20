#include "LevelSelect.h"
#include <iostream>

LevelSelect::LevelSelect(sf::Font &font, sf::RenderWindow &window)
    : selectedIndex(-1), backSelected(false)
{
    // Example: Populate with a few level options.
    std::vector<std::string> levels = {"Level 1", "Level 2", "Level 3"};

    float x = window.getSize().x / 2.f;
    float startY = window.getSize().y / 4.f;
    float spacing = 60.f;

    for (size_t i = 0; i < levels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(levels[i]);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);

        // Center the text
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.f,
                       textRect.top + textRect.height / 2.f);
        text.setPosition(x, startY + i * spacing);
        levelItems.push_back(text);
    }

    // Setup the "Back" option
    backOption.setFont(font);
    backOption.setString("Back");
    backOption.setCharacterSize(30);
    backOption.setFillColor(sf::Color::Yellow);
    sf::FloatRect textRect = backOption.getLocalBounds();
    backOption.setOrigin(textRect.left + textRect.width / 2.f,
                         textRect.top + textRect.height / 2.f);
    backOption.setPosition(x, startY + levels.size() * spacing + 40.f);
}

void LevelSelect::handleEvent(const sf::Event &event)
{
    // A basic implementation: use mouse events to detect hover and clicks.
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                              static_cast<float>(event.mouseButton.y));
        // Check each level option
        for (size_t i = 0; i < levelItems.size(); ++i)
        {
            if (levelItems[i].getGlobalBounds().contains(mousePos))
            {
                selectedIndex = i;
                std::cout << "Level " << i+1 << " selected." << std::endl;
            }
        }

        // Check if "Back" was clicked
        if (backOption.getGlobalBounds().contains(mousePos))
        {
            backSelected = true;
        }
    }
}

void LevelSelect::update()
{
    // Add any animation or update logic if necessary.
}

void LevelSelect::draw(sf::RenderWindow &window)
{
    for (const auto &item : levelItems)
    {
        window.draw(item);
    }
    window.draw(backOption);
}

int LevelSelect::getSelectedLevel() const {
    return selectedIndex;
}

bool LevelSelect::isBackSelected() const {
    return backSelected;
}

