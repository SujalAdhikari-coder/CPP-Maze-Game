#include "Level3.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

Level3::Level3(sf::Vector2u windowSize)
    : maze(nullptr)
{
    // Insane maze: highly intricate, with very many false paths.
    std::vector<std::string> layout = {
        "1111111111111111111111111111111",
        "1   1     1   1   1   1   1   1",
        "1 1 1 111 1 1 1 1 1 1 1 111 1 1",
        "1 1     1   1   1   1     1 1 1",
        "1 11111 111 111 111 111 111 1 1",
        "1       1       1       1   1 1",
        "1 11111 1 1111111 11111  1 1 1 1",
        "1     1 1       1     1 1 1 1 1",
        "1 111 1 111 111 1 111 1 1 1 1 1",
        "1   1     1   1 1   1 1     1E1",
        "1111111111111111111111111111111"
    };

    unsigned int numRows = layout.size();
    unsigned int numCols = layout[0].size();
    int tileSize = std::min(static_cast<int>(windowSize.x) / static_cast<int>(numCols),
                            static_cast<int>(windowSize.y) / static_cast<int>(numRows));

    maze = new Maze(layout, tileSize);
}

Level3::~Level3() {
    if (maze) delete maze;
}

void Level3::draw(sf::RenderWindow &window) {
    maze->draw(window);
}

Maze* Level3::getMaze() {
    return maze;
}
