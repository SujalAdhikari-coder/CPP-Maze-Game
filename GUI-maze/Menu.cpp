#include "Menu.h"
#include <iostream>

Menu::Menu(sf::Font &font, sf::RenderWindow &window)
    : selectedIndex(-1), optionClicked(false)
{
    // Load the background image using your specified asset path.
    if (!backgroundTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\menu-bg.jpg.jpg"))
    {
        std::cerr << "Failed to load background image from E:\\C++\\GUI\\GUI-maze\\assets\\menu-bg.jpg.jpg" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u texSize = backgroundTexture.getSize();
    sf::Vector2u winSize = window.getSize();
    backgroundSprite.setScale(static_cast<float>(winSize.x) / texSize.x,
                                static_cast<float>(winSize.y) / texSize.y);

    // Define the list of menu options (including Credits).
    std::vector<std::string> options = {
        "Start Game", "Instructions", "Select Level", "Scoreboard", "Credits", "Exit"
    };

    // Create and position menu items.
    menuItems.resize(options.size());
    float x = window.getSize().x / 2.f;
    float startY = window.getSize().y / 3.f;
    float spacing = 60.f;

    for (size_t i = 0; i < options.size(); ++i)
    {
        menuItems[i].setFont(font);
        menuItems[i].setString(options[i]);
        menuItems[i].setCharacterSize(48); // High-resolution text.
        menuItems[i].setFillColor(sf::Color::White);

        sf::FloatRect textRect = menuItems[i].getLocalBounds();
        menuItems[i].setOrigin(textRect.left + textRect.width / 2.f,
                               textRect.top + textRect.height / 2.f);
        menuItems[i].setPosition(x, startY + i * spacing);
    }

    // Set up a semi-transparent background box for the menu.
    sf::Vector2f boxSize(window.getSize().x * 0.8f, window.getSize().y * 0.6f);
    backgroundBox.setSize(boxSize);
    backgroundBox.setFillColor(sf::Color(50, 50, 50, 200));
    backgroundBox.setOutlineColor(sf::Color::White);
    backgroundBox.setOutlineThickness(2.f);
    backgroundBox.setOrigin(boxSize.x / 2.f, boxSize.y / 2.f);
    backgroundBox.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
}

void Menu::handleEvent(const sf::Event &event)
{
    optionClicked = false;
    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                              static_cast<float>(event.mouseMove.y));
        for (size_t i = 0; i < menuItems.size(); ++i)
        {
            if (menuItems[i].getGlobalBounds().contains(mousePos))
            {
                selectedIndex = i;
                // Optionally animate (e.g., scale up) the hovered item.
                menuItems[i].setScale(1.1f, 1.1f);
            }
            else
            {
                menuItems[i].setScale(1.f, 1.f);
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed &&
             event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                              static_cast<float>(event.mouseButton.y));
        for (size_t i = 0; i < menuItems.size(); ++i)
        {
            if (menuItems[i].getGlobalBounds().contains(mousePos))
            {
                selectedIndex = i;
                optionClicked = true;
            }
        }
    }
}

void Menu::update()
{
    // Additional animations can be added here if needed.
}

void Menu::draw(sf::RenderWindow &window)
{
    window.draw(backgroundSprite);
    window.draw(backgroundBox);
    for (const auto &item : menuItems)
    {
        window.draw(item);
    }
}

bool Menu::isOptionSelected(MenuOption &selectedOption) const
{
    if (optionClicked && selectedIndex >= 0 && selectedIndex < MENU_OPTION_COUNT)
    {
        selectedOption = static_cast<MenuOption>(selectedIndex);
        return true;
    }
    return false;
}
