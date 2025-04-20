#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Define the available menu options.
enum MenuOption {
    START_GAME,
    INSTRUCTIONS,
    SELECT_LEVEL,
    SCOREBOARD,
    CREDITS,       // New Credits option
    EXIT_GAME,
    MENU_OPTION_COUNT
};

class Menu {
public:
    // Constructor: receives a font and a reference to the window to compute positions.
    Menu(sf::Font &font, sf::RenderWindow &window);

    // Handle events (mouse movements and clicks) for menu interactivity.
    void handleEvent(const sf::Event &event);

    // Update any animations (if needed).
    void update();

    // Draw the menu to the window.
    void draw(sf::RenderWindow &window);

    // Returns true if a menu option was selected; the selected option is stored in selectedOption.
    bool isOptionSelected(MenuOption &selectedOption) const;

private:
    std::vector<sf::Text> menuItems;
    sf::RectangleShape backgroundBox;

    // Background image for the menu.
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    int selectedIndex;
    bool optionClicked;
};

#endif // MENU_H
