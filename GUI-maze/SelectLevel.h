#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SelectLevel {
public:
    // Constructor loads the background image for level selection.
    SelectLevel(sf::Font &font, sf::RenderWindow &window);
    void handleEvent(const sf::Event &event);
    void update();
    void draw(sf::RenderWindow &window);
    bool isLevelSelected() const;
    int getSelectedLevel() const;

private:
    struct Button {
        sf::RectangleShape shape;
        sf::Text label;
    };

    std::vector<Button> buttons;
    int selectedIndex;
    bool optionClicked;

    // Background texture and sprite for level selection.
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

#endif // SELECTLEVEL_H
