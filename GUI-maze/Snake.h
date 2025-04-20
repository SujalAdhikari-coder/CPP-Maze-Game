#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Maze.h"  // For collision checking

class Snake {
public:
    // Initialize the snake at a starting grid position with given tile size.
    Snake(const sf::Vector2i& startPos, int tileSize);

    // Update methods.
    void update();                 // Update without maze collision check.
    void update(const Maze &maze); // Update with maze collision check.

    // Draw the snake (head + body textures).
    void draw(sf::RenderWindow &window);

    // Set the snake's current movement direction; prevents direct reversal.
    void setDirection(const sf::Vector2i& dir);

    // Return the grid position of the snake's head.
    sf::Vector2i getHeadPosition() const;

    // Load textures for the snake from the given file paths.
    static bool loadTextures(const std::string &headPath, const std::string &bodyPath);

private:
    int tileSize;
    std::vector<sf::Vector2i> body;  // Each segment's grid coordinate.
    sf::Vector2i direction;          // Current movement direction.

    // Static textures for the snake's appearance.
    static sf::Texture headTexture;
    static sf::Texture bodyTexture;
    static bool texturesLoaded;
};

#endif // SNAKE_H
