#include "Credits.h"
#include <iostream>
#include <sstream>

Credits::Credits(const sf::Vector2u &winSize)
    : windowSize(winSize), offset(static_cast<float>(winSize.y)), speed(50.f)
{
    // Populate credits lines.
    creditsLines.push_back("Images provided by leonardo.ai");
    creditsLines.push_back("Code by Sujal Adhikari");
    creditsLines.push_back("Music by NCS");
    creditsLines.push_back(""); // blank line for spacing
    creditsLines.push_back("Thank you for playing!");
    // The text objects are created on-demand in draw() to capture the current font.
}

Credits::~Credits() {
    // Nothing dynamic to free.
}

void Credits::update(sf::Time delta) {
    // Move the credits upward.
    offset -= speed * delta.asSeconds();
    // Estimate the total height of credits (line spacing = 50 pixels).
    float totalHeight = creditsLines.size() * 50.f;
    // Reset offset when credits have completely scrolled.
    if (offset < -totalHeight) {
        offset = static_cast<float>(windowSize.y);
    }
}

void Credits::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    // Currently, no specific event handling is implemented.
}

void Credits::draw(sf::RenderWindow &window, sf::Font &font) {
    // Initialize and cache text objects on the first draw call.
    if (creditsTexts.empty()) {
        for (size_t i = 0; i < creditsLines.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(creditsLines[i]);
            text.setCharacterSize(48);
            text.setFillColor(sf::Color::White);
            // Center the text horizontally using its local bounds.
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.f,
                           textRect.top + textRect.height / 2.f);
            // Set a base vertical position (will be offset each frame).
            text.setPosition(static_cast<float>(windowSize.x) / 2.f, i * 50.f);
            creditsTexts.push_back(text);
        }
    }

    // Draw each cached text object with an updated y position using the current offset.
    for (size_t i = 0; i < creditsTexts.size(); ++i) {
        // Create a local copy, update its position, and draw.
        sf::Text text = creditsTexts[i];
        text.setPosition(static_cast<float>(windowSize.x) / 2.f, offset + i * 50.f);
        window.draw(text);
    }
}
