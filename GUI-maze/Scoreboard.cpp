#include "Scoreboard.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

// Set line spacing (adjust if necessary)
static const float LINE_SPACING = 42.f;

Scoreboard::Scoreboard(const std::string &filename)
    : fileName(filename), scrollOffset(0.f), maxScrollOffset(0.f),
      deletionMode(false), confirmationActive(false), selectedIndexToDelete(-1)
{
    // Load the background image.
    if (!backgroundTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\hall.jpg")) {
        std::cerr << "Failed to load scoreboard background from hall.jpg" << std::endl;
    } else {
        backgroundTexture.setSmooth(true);
        backgroundSprite.setTexture(backgroundTexture);
    }

    // Define the scrollable area.
    scrollableArea = sf::FloatRect(400.f, 200.f, 600.f, 500.f);

    // Setup the Delete button.
    deleteButton.setSize(sf::Vector2f(160.f, 60.f));
    deleteButton.setFillColor(sf::Color(180, 0, 0, 230));
    deleteButton.setPosition(scrollableArea.left + 20.f,
                             scrollableArea.top + scrollableArea.height - deleteButton.getSize().y - 20.f);
    deleteButtonText.setString("Delete");
    deleteButtonText.setCharacterSize(32);
    deleteButtonText.setFillColor(sf::Color::White);

    // Setup confirmation popup buttons.
    yesButton.setSize(sf::Vector2f(160.f, 60.f));
    yesButton.setFillColor(sf::Color(0, 160, 0, 230));
    noButton.setSize(sf::Vector2f(160.f, 60.f));
    noButton.setFillColor(sf::Color(160, 0, 0, 230));
    yesButtonText.setString("Yes");
    yesButtonText.setCharacterSize(40);
    yesButtonText.setFillColor(sf::Color::White);
    noButtonText.setString("No");
    noButtonText.setCharacterSize(40);
    noButtonText.setFillColor(sf::Color::White);

    loadEntries();
}

void Scoreboard::addEntry(const std::string &name, float time, int score) {
    std::ofstream file(fileName, std::ios::app);
    if (!file) {
        std::cerr << "Failed to open scoreboard file: " << fileName << std::endl;
        return;
    }
    file << name << " " << time << " " << score << "\n";
    file.close();
    loadEntries();
}

void Scoreboard::loadEntries() {
    entries.clear();
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Failed to open scoreboard file: " << fileName << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        ScoreEntry entry;
        if (iss >> entry.name >> entry.time >> entry.score) {
            entries.push_back(entry);
        }
    }
    file.close();

    // Sort entries: higher score first; if equal, lower time wins.
    std::sort(entries.begin(), entries.end(), [](const ScoreEntry &a, const ScoreEntry &b) {
        if(a.score == b.score)
            return a.time < b.time;
        return a.score > b.score;
    });
}

void Scoreboard::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (confirmationActive) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f clickPos = window.mapPixelToCoords(pixelPos);
            if (yesButton.getGlobalBounds().contains(clickPos)) {
                if (selectedIndexToDelete >= 0 && selectedIndexToDelete < static_cast<int>(entries.size())) {
                    entries.erase(entries.begin() + selectedIndexToDelete);
                    std::ofstream ofs(fileName);
                    if (ofs) {
                        for (const auto &entry : entries) {
                            ofs << entry.name << " " << entry.time << " " << entry.score << "\n";
                        }
                    }
                }
                confirmationActive = false;
                deletionMode = false;
                selectedIndexToDelete = -1;
            } else if (noButton.getGlobalBounds().contains(clickPos)) {
                confirmationActive = false;
            }
        }
        return;
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        float delta = event.mouseWheelScroll.delta * 20.f;
        scrollOffset -= delta;
        if (scrollOffset < 0.f)
            scrollOffset = 0.f;
        if (scrollOffset > maxScrollOffset)
            scrollOffset = maxScrollOffset;
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        if (deleteButton.getGlobalBounds().contains(worldPos)) {
            deletionMode = !deletionMode;
            return;
        }
        if (deletionMode) {
            sf::View scoreboardView(sf::FloatRect(
                scrollableArea.left,
                scrollableArea.top + scrollOffset,
                scrollableArea.width,
                scrollableArea.height
            ));
            sf::Vector2f clickPos = window.mapPixelToCoords(pixelPos, scoreboardView);
            float baseY = scrollableArea.top + 40.f + 100.f;
            int index = static_cast<int>((clickPos.y - baseY) / LINE_SPACING);
            if (index >= 0 && index < static_cast<int>(entries.size())) {
                selectedIndexToDelete = index;
                confirmationActive = true;
            }
        }
    }
}

void Scoreboard::updateMaxScroll(const sf::RenderWindow &window, float totalTextHeight) {
    float visibleHeight = scrollableArea.height;
    if (totalTextHeight <= visibleHeight) {
        maxScrollOffset = 0.f;
        scrollOffset = 0.f;
    } else {
        maxScrollOffset = totalTextHeight - visibleHeight;
        if (scrollOffset > maxScrollOffset)
            scrollOffset = maxScrollOffset;
    }
}

void Scoreboard::drawConfirmation(sf::RenderWindow &window, sf::Font &font) {
    sf::Vector2u winSize = window.getSize();
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(winSize.x * 0.6f, winSize.y * 0.4f));
    overlay.setFillColor(sf::Color(0, 0, 0, 220));
    overlay.setPosition(winSize.x * 0.2f, winSize.y * 0.3f);
    window.draw(overlay);

    sf::Text confirmText;
    confirmText.setFont(font);
    std::ostringstream ss;
    if (selectedIndexToDelete >= 0 && selectedIndexToDelete < static_cast<int>(entries.size())) {
        ss << "Are you sure you want to delete:\n"
           << (selectedIndexToDelete + 1) << ". " << entries[selectedIndexToDelete].name
           << " - " << std::fixed << std::setprecision(2) << entries[selectedIndexToDelete].time
           << "s, Score: " << entries[selectedIndexToDelete].score;
    }
    confirmText.setString(ss.str());
    confirmText.setCharacterSize(40);
    confirmText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = confirmText.getLocalBounds();
    confirmText.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
    confirmText.setPosition(overlay.getPosition().x + overlay.getSize().x / 2.f,
                            overlay.getPosition().y + overlay.getSize().y / 2.f - 60.f);
    window.draw(confirmText);

    yesButton.setPosition(overlay.getPosition().x + overlay.getSize().x * 0.25f,
                          overlay.getPosition().y + overlay.getSize().y - 100.f);
    window.draw(yesButton);
    yesButtonText.setFont(font);
    yesButtonText.setString("Yes");
    yesButtonText.setCharacterSize(40);
    yesButtonText.setFillColor(sf::Color::White);
    sf::FloatRect yesRect = yesButtonText.getLocalBounds();
    yesButtonText.setOrigin(yesRect.width / 2.f, yesRect.height / 2.f);
    yesButtonText.setPosition(yesButton.getPosition().x + yesButton.getSize().x / 2.f,
                                yesButton.getPosition().y + yesButton.getSize().y / 2.f);
    window.draw(yesButtonText);

    noButton.setPosition(overlay.getPosition().x + overlay.getSize().x * 0.65f,
                         overlay.getPosition().y + overlay.getSize().y - 100.f);
    window.draw(noButton);
    noButtonText.setFont(font);
    noButtonText.setString("No");
    noButtonText.setCharacterSize(40);
    noButtonText.setFillColor(sf::Color::White);
    sf::FloatRect noRect = noButtonText.getLocalBounds();
    noButtonText.setOrigin(noRect.width / 2.f, noRect.height / 2.f);
    noButtonText.setPosition(noButton.getPosition().x + noButton.getSize().x / 2.f,
                             noButton.getPosition().y + noButton.getSize().y / 2.f);
    window.draw(noButtonText);
}

void Scoreboard::display(sf::RenderWindow &window, sf::Font &font) {
    sf::Vector2u winSize = window.getSize();
    backgroundSprite.setScale(
        (winSize.x * 0.8f) / backgroundTexture.getSize().x,
        (winSize.y * 0.8f) / backgroundTexture.getSize().y
    );
    backgroundSprite.setPosition(winSize.x * 0.1f, winSize.y * 0.1f);
    window.draw(backgroundSprite);

    sf::Text title;
    title.setFont(font);
    title.setString("HALL OF FAME");
    title.setCharacterSize(48);
    title.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleRect = title.getLocalBounds();
    float startY = scrollableArea.top + 40.f;
    float xMid = scrollableArea.left + scrollableArea.width / 2.f;
    title.setOrigin(titleRect.width / 2.f, 0.f);
    title.setPosition(xMid, startY);

    float totalTextHeight = 100.f + entries.size() * LINE_SPACING;
    updateMaxScroll(window, totalTextHeight);

    sf::View originalView = window.getView();
    sf::View scoreboardView(sf::FloatRect(
        scrollableArea.left,
        scrollableArea.top + scrollOffset,
        scrollableArea.width,
        scrollableArea.height
    ));
    window.setView(scoreboardView);

    window.draw(title);

    for (size_t i = 0; i < entries.size(); ++i) {
        sf::Text entryText;
        entryText.setFont(font);
        std::ostringstream ss;
        ss << std::setw(2) << (i + 1) << ". " << entries[i].name
           << " - Time: " << std::fixed << std::setprecision(2) << entries[i].time
           << "s, Score: " << entries[i].score;
        entryText.setString(ss.str());
        entryText.setCharacterSize(40);
        entryText.setFillColor(sf::Color::White);
        sf::FloatRect eRect = entryText.getLocalBounds();
        entryText.setOrigin(eRect.width / 2.f, 0.f);
        float entryY = startY + 100.f + i * LINE_SPACING;
        entryText.setPosition(xMid, entryY);

        sf::Vector2f mousePosInView = window.mapPixelToCoords(sf::Mouse::getPosition(window), scoreboardView);
        if (entryText.getGlobalBounds().contains(mousePosInView)) {
            entryText.setFillColor(sf::Color::Cyan);
            entryText.setScale(1.1f, 1.1f);
        }
        window.draw(entryText);
    }

    window.setView(originalView);

    deleteButton.setPosition(scrollableArea.left + 20.f,
                             scrollableArea.top + scrollableArea.height - deleteButton.getSize().y - 20.f);
    window.draw(deleteButton);
    deleteButtonText.setFont(font);
    deleteButtonText.setString("Delete");
    deleteButtonText.setCharacterSize(32);
    deleteButtonText.setFillColor(sf::Color::White);
    sf::FloatRect dRect = deleteButtonText.getLocalBounds();
    deleteButtonText.setOrigin(dRect.width / 2.f, dRect.height / 2.f);
    deleteButtonText.setPosition(deleteButton.getPosition().x + deleteButton.getSize().x / 2.f,
                                 deleteButton.getPosition().y + deleteButton.getSize().y / 2.f);
    window.draw(deleteButtonText);

    if (deletionMode && !confirmationActive) {
        sf::Text modeText;
        modeText.setFont(font);
        modeText.setString("Deletion mode active.\nClick on a score to delete.");
        modeText.setCharacterSize(32);
        modeText.setFillColor(sf::Color::Red);
        sf::Vector2f pos = deleteButton.getPosition();
        modeText.setPosition(pos.x + deleteButton.getSize().x + 20.f, pos.y + deleteButton.getSize().y / 2.f - 16.f);
        window.draw(modeText);
    }

    if (confirmationActive) {
        drawConfirmation(window, font);
    }
}
