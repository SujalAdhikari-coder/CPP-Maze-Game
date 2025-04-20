#include "Collectables.h"
#include <iostream>
#include <cmath>

Collectables::Collectables(const std::vector<sf::Vector2i>& positions, int tileSize)
    : tileSize(tileSize), bounceTimer(0.f)
{
    // Load the coin texture from the specified file path.
    if (!coinTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets/coin.png")) {
        std::cerr << "Failed to load coin texture from: E:\\C++\\GUI\\GUI-maze\\assets/coin.png" << std::endl;
    }

    // Precompute the coin's scaling factors.
    sf::Vector2u texSize = coinTexture.getSize();
    float scaleFactor = 0.5f;
    coinScale.x = (tileSize * scaleFactor) / static_cast<float>(texSize.x);
    coinScale.y = (tileSize * scaleFactor) / static_cast<float>(texSize.y);

    // Setup the reusable coin sprite.
    coinSprite.setTexture(coinTexture);
    coinSprite.setScale(coinScale);

    // Initialize each coin with its grid position and mark as not collected.
    for (const auto& pos : positions) {
        Coin coin;
        coin.position = pos;
        coin.collected = false;
        coins.push_back(coin);
    }
}

void Collectables::update(sf::Time delta) {
    // Increment the bounce animation timer.
    bounceTimer += delta.asSeconds();
}

void Collectables::draw(sf::RenderWindow& window) {
    // Calculate vertical bounce offset using a sine function.
    float bounceOffset = BOUNCE_AMPLITUDE * std::sin(bounceTimer * BOUNCE_FREQUENCY * 2 * 3.14159f);

    // Draw each coin that has not been collected using the preallocated sprite.
    for (const auto& coin : coins) {
        if (!coin.collected) {
            // Calculate centered coin position based on the grid cell.
            float posX = coin.position.x * tileSize + (tileSize * (1 - 0.5f)) / 2.0f;
            float posY = coin.position.y * tileSize + (tileSize * (1 - 0.5f)) / 2.0f;
            coinSprite.setPosition(posX, posY + bounceOffset);
            window.draw(coinSprite);
        }
    }
}

int Collectables::collectAt(const sf::Vector2i& snakeHead) {
    int pointsGained = 0;
    for (auto& coin : coins) {
        if (!coin.collected && coin.position == snakeHead) {
            coin.collected = true;
            pointsGained += COIN_VALUE;
        }
    }
    return pointsGained;
}
