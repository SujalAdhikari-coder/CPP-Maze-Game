#ifndef CREDITS_H
#define CREDITS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Credits {
public:
    // Constructor. Pass the window size so that the credits can be centered and loop properly.
    Credits(const sf::Vector2u &windowSize);

    // Destructor.
    ~Credits();

    // Update credits scrolling offset (delta: elapsed time).
    void update(sf::Time delta);

    // Handle any credit-specific events (e.g., exiting credits; optional).
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);

    // Draw the credits animation to the provided window using the given font.
    void draw(sf::RenderWindow &window, sf::Font &font);

private:
    std::vector<std::string> creditsLines;  // The list of credit lines.
    std::vector<sf::Text> creditsTexts;       // Cached SFML text objects.
    float offset;        // Current vertical offset for the scrolling credits.
    float speed;         // Scrolling speed (pixels per second).
    sf::Vector2u windowSize; // To center and loop the credits.
};

#endif // CREDITS_H
