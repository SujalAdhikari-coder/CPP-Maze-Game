#include "Maze.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// Helper function to trim whitespace.
static inline void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

Maze::Maze(const std::vector<std::string>& layout, int tileSize)
    : tileSize(tileSize), startPos(sf::Vector2i(1,1))
{
    grid = layout;

    // Load the door texture.
    if (!doorTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\door.png")) {
        std::cerr << "Failed to load door texture." << std::endl;
    }

    // Load the wall texture.
    if (!wallTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\wall.png")) {
        std::cerr << "Failed to load wall texture." << std::endl;
    }

    // Load the grass texture.
    if (!grassTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\grass.png")) {
        std::cerr << "Failed to load grass texture." << std::endl;
    }
}

void Maze::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open maze file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        trim(line);
        if (!line.empty())
            grid.push_back(line);
    }
    file.close();
}

void Maze::draw(sf::RenderWindow &window) {
    // Draw each cell.
    for (size_t row = 0; row < grid.size(); ++row) {
        for (size_t col = 0; col < grid[row].size(); ++col) {
            float posX = static_cast<float>(col * tileSize);
            float posY = static_cast<float>(row * tileSize);

            if (grid[row][col] == '1') {
                // Draw wall.
                sf::Sprite wallSprite;
                wallSprite.setTexture(wallTexture);
                sf::Vector2u texSize = wallTexture.getSize();
                wallSprite.setScale(static_cast<float>(tileSize) / texSize.x,
                                    static_cast<float>(tileSize) / texSize.y);
                wallSprite.setPosition(posX, posY);
                window.draw(wallSprite);
            }
            else if (grid[row][col] == 'E') {
                // Draw exit.
                sf::Sprite doorSprite;
                doorSprite.setTexture(doorTexture);
                sf::Vector2u texSize = doorTexture.getSize();
                doorSprite.setScale(static_cast<float>(tileSize) / texSize.x,
                                    static_cast<float>(tileSize) / texSize.y);
                doorSprite.setPosition(posX, posY);
                window.draw(doorSprite);
            }
            else {
                // Draw open cell as grass.
                sf::Sprite grassSprite;
                grassSprite.setTexture(grassTexture);
                sf::Vector2u texSize = grassTexture.getSize();
                grassSprite.setScale(static_cast<float>(tileSize) / texSize.x,
                                     static_cast<float>(tileSize) / texSize.y);
                grassSprite.setPosition(posX, posY);
                window.draw(grassSprite);
            }
        }
    }
}

bool Maze::isWall(const sf::Vector2i &gridPos) const {
    int row = gridPos.y;
    int col = gridPos.x;
    if (row < 0 || row >= static_cast<int>(grid.size()) ||
        col < 0 || col >= static_cast<int>(grid[0].size()))
        return true;
    return grid[row][col] == '1';
}

bool Maze::isExit(const sf::Vector2i &gridPos) const {
    int row = gridPos.y;
    int col = gridPos.x;
    if (row < 0 || row >= static_cast<int>(grid.size()) ||
        col < 0 || col >= static_cast<int>(grid[0].size()))
        return false;
    return grid[row][col] == 'E';
}

char Maze::getCell(int row, int col) const {
    if (row < 0 || row >= static_cast<int>(grid.size()) ||
        col < 0 || col >= static_cast<int>(grid[0].size()))
        return '1';
    return grid[row][col];
}
