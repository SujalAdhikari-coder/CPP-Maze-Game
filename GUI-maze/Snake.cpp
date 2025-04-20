#include "Snake.h"
#include <iostream>

// Define static members.
sf::Texture Snake::headTexture;
sf::Texture Snake::bodyTexture;
bool Snake::texturesLoaded = false;

Snake::Snake(const sf::Vector2i& startPos, int tileSize)
    : tileSize(tileSize), direction(0, 1) // Default direction: down (matches the image orientation)
{
    // If textures haven't been loaded yet, load them now.
    if (!texturesLoaded) {
        if (!loadTextures("E:\\C++\\GUI\\GUI-maze\\assets\\sn-head.png",
                          "E:\\C++\\GUI\\GUI-maze\\assets\\sn-body.png"))
        {
            std::cerr << "Error: Failed to load snake textures." << std::endl;
        }
    }
    // Initialize the snake with a length of 3 segments.
    body.push_back(startPos);
    body.push_back(sf::Vector2i(startPos.x, startPos.y - 1));
    body.push_back(sf::Vector2i(startPos.x, startPos.y - 2));
}

bool Snake::loadTextures(const std::string &headPath, const std::string &bodyPath) {
    if (!headTexture.loadFromFile(headPath)) {
        std::cerr << "Failed to load head texture from: " << headPath << std::endl;
        return false;
    }
    if (!bodyTexture.loadFromFile(bodyPath)) {
        std::cerr << "Failed to load body texture from: " << bodyPath << std::endl;
        return false;
    }
    texturesLoaded = true;
    return true;
}

void Snake::update() {
    // Simple update: add a new head based on current direction, remove the tail.
    sf::Vector2i newHead = body.front() + direction;
    body.insert(body.begin(), newHead);
    body.pop_back();
}

void Snake::update(const Maze &maze) {
    // Collision-aware update: move only if the next cell is not a wall.
    sf::Vector2i newHead = body.front() + direction;
    if (!maze.isWall(newHead)) {
        body.insert(body.begin(), newHead);
        body.pop_back();
    }
    // Otherwise, do nothing or add collision logic.
}

void Snake::draw(sf::RenderWindow &window) {
    for (size_t i = 0; i < body.size(); i++) {
        sf::Sprite sprite;
        // If this is the head (i == 0), use head texture and rotate for direction.
        if (i == 0) {
            sprite.setTexture(headTexture);

            // Set origin to the center for proper rotation around the center.
            sf::Vector2u texSize = headTexture.getSize();
            sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);

            // Scale the sprite to fit the tile size.
            sprite.setScale(
                static_cast<float>(tileSize) / texSize.x,
                static_cast<float>(tileSize) / texSize.y
            );

            // Position the sprite in the center of its cell.
            float posX = (body[i].x * tileSize) + (tileSize / 2.f);
            float posY = (body[i].y * tileSize) + (tileSize / 2.f);
            sprite.setPosition(posX, posY);

            // Adjust rotation based on direction.
            // Since image faces DOWN by default, we set 0° rotation for (0, 1).
            if (direction == sf::Vector2i(0, 1))
                sprite.setRotation(0.f);      // facing down
            else if (direction == sf::Vector2i(0, -1))
                sprite.setRotation(180.f);    // facing up
            else if (direction == sf::Vector2i(1, 0))
                sprite.setRotation(90.f);     // facing right
            else if (direction == sf::Vector2i(-1, 0))
                sprite.setRotation(270.f);    // facing left
        }
        else {
            // For body segments, use body texture without rotation.
            sprite.setTexture(bodyTexture);
            sf::Vector2u texSize = bodyTexture.getSize();
            sprite.setScale(
                static_cast<float>(tileSize) / texSize.x,
                static_cast<float>(tileSize) / texSize.y
            );
            // Position at the top-left corner of the cell (no rotation).
            sprite.setPosition(body[i].x * tileSize, body[i].y * tileSize);
        }
        window.draw(sprite);
    }
}

void Snake::setDirection(const sf::Vector2i& dir) {
    // Prevent immediate reversal.
    sf::Vector2i opposite(-direction.x, -direction.y);
    if (dir == opposite) {
        return;
    }
    direction = dir;
}

sf::Vector2i Snake::getHeadPosition() const {
    return body.front();
}
