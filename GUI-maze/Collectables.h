#ifndef COLLECTABLES_H
#define COLLECTABLES_H

#include <SFML/Graphics.hpp>
#include <vector>

class Collectables {
public:
    // Each coin gives 10 points.
    static constexpr int COIN_VALUE = 10;

    // Represents a coin collectible.
    struct Coin {
        sf::Vector2i position;  // Grid coordinate of the coin.
        bool collected;         // Whether the coin has been collected.
    };

    // Constructor: takes a list of coin positions (in grid coordinates) and the maze's tile size.
    Collectables(const std::vector<sf::Vector2i>& positions, int tileSize);

    // Update the coin animation.
    void update(sf::Time delta);

    // Draws all coins that have not yet been collected.
    void draw(sf::RenderWindow& window);

    // Checks if a coin exists at the snake's head.
    // If so, marks it as collected and returns COIN_VALUE; otherwise returns 0.
    int collectAt(const sf::Vector2i& snakeHead);

private:
    std::vector<Coin> coins;    // List of coins.
    int tileSize;               // Maze tile size (in pixels).
    sf::Texture coinTexture;    // Texture for the coin image.
    float bounceTimer;          // Timer used for the bounce animation.

    // Bounce animation constants.
    static constexpr float BOUNCE_AMPLITUDE = 3.0f;   // Maximum vertical offset in pixels.
    static constexpr float BOUNCE_FREQUENCY = 3.0f;    // Bounce cycles per second.

    // Optimized: reusable sprite for coin and precomputed scale factor.
    sf::Sprite coinSprite;
    sf::Vector2f coinScale;
};

#endif // COLLECTABLES_H
