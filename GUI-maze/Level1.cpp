#include "Level1.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

Level1::Level1(sf::Vector2u windowSize)
    : maze(nullptr)
{
    // Easy maze: simple, big, with multiple false paths.
    // (Each row is 31 characters wide.)
    std::vector<std::string> layout = {
        "1111111111111111111111111111111",
        "1     1       1     1         1",
        "1 111 1 11111 1 111 1 11111   1 1",
        "1 1   1     1 1   1 1   1     1 1",
        "1 1   1111111 1   1 1   11111  1",
        "1 1         1 1   1 1         1",
        "1 111111111 1 111 1 111111111  1",
        "1         1     1           1 1",
        "1 111111  11111 111 11111   1 1",
        "1       1       1       1     1",
        "1 11111 1 11111 1 11111  1 111 1",
        "1     1       1     1    1   E1",
        "1111111111111111111111111111111"
    };

    // Calculate a tile size that makes the maze fit into the window.
    unsigned int numRows = layout.size();
    unsigned int numCols = layout[0].size();
    int tileSize = std::min(static_cast<int>(windowSize.x) / static_cast<int>(numCols),
                            static_cast<int>(windowSize.y) / static_cast<int>(numRows));

    maze = new Maze(layout, tileSize);
}

Level1::~Level1() {
    if (maze) delete maze;
}

void Level1::draw(sf::RenderWindow &window) {
    maze->draw(window);
}

Maze* Level1::getMaze() {
    return maze;
}
