#include "Level2.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

Level2::Level2(sf::Vector2u windowSize)
    : maze(nullptr)
{
    // Hard maze: more complex, with many false paths and twists.
    std::vector<std::string> layout = {
        "1111111111111111111111111111111",
        "1   1     1     1   1     1   1",
        "1 1 1 111 1 111 1 1 1 111 1 1 1",
        "1 1   1   1   1 1 1   1   1 1 1",
        "1 111 1 11111 1 111 1 11111 1 1",
        "1     1       1     1       1 1",
        "1 111111 111111 111 111111 111 1",
        "1       1     1   1     1     1",
        "1 111 1 1 111 111 1 111 1 111  1",
        "1   1 1   1     1   1   1   1 1",
        "1 1 111111 1 111 111 111111 1 1",
        "1 1       1   1       1     1E1",
        "1111111111111111111111111111111"
    };

    unsigned int numRows = layout.size();
    unsigned int numCols = layout[0].size();
    int tileSize = std::min(static_cast<int>(windowSize.x) / static_cast<int>(numCols),
                            static_cast<int>(windowSize.y) / static_cast<int>(numRows));

    maze = new Maze(layout, tileSize);
}

Level2::~Level2() {
    if (maze) delete maze;
}

void Level2::draw(sf::RenderWindow &window) {
    maze->draw(window);
}

Maze* Level2::getMaze() {
    return maze;
}
