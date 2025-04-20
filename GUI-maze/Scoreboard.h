#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Structure to hold a score record.
struct ScoreEntry {
    std::string name;
    float time;
    int score;  // New field for coin score
};

class Scoreboard {
public:
    // Constructor: loads background image and sets up the scrollable area.
    Scoreboard(const std::string &filename);

    // Add a new score entry and reload entries.
    // Now takes a third parameter for the coin-based score.
    void addEntry(const std::string &name, float time, int score);
    // Load all score entries from file.
    void loadEntries();

    // Handle events: scrolling, toggling deletion mode, and confirmation clicks.
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);
    // Display the scoreboard: background, title, score entries, delete button, and if active, deletion confirmation popup.
    void display(sf::RenderWindow &window, sf::Font &font);

private:
    // Update the maximum allowed scroll offset given the total text height.
    void updateMaxScroll(const sf::RenderWindow &window, float totalTextHeight);
    // Draw the deletion confirmation overlay.
    void drawConfirmation(sf::RenderWindow &window, sf::Font &font);

    std::string fileName;
    std::vector<ScoreEntry> entries;

    // Background image and sprite.
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Scrolling variables.
    float scrollOffset;         // Current vertical scroll offset.
    float maxScrollOffset;      // Maximum scroll offset allowed.
    sf::FloatRect scrollableArea;  // The area where the score text is drawn.

    // Deletion mode and confirmation.
    bool deletionMode;
    bool confirmationActive;
    int selectedIndexToDelete;

    // Delete button.
    sf::RectangleShape deleteButton;
    sf::Text deleteButtonText;

    // Confirmation popup buttons.
    sf::RectangleShape yesButton;
    sf::Text yesButtonText;
    sf::RectangleShape noButton;
    sf::Text noButtonText;
};

#endif // SCOREBOARD_H
