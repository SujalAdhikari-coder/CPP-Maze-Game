#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Maze {
public:
    // Constructs the maze from an array of strings and tile size.
    Maze(const std::vector<std::string>& layout, int tileSize);

    void draw(sf::RenderWindow &window);
    bool isWall(const sf::Vector2i &gridPos) const;
    bool isExit(const sf::Vector2i &gridPos) const;
    int getTileSize() const { return tileSize; }
    sf::Vector2i getStartPosition() const { return startPos; }
    char getCell(int row, int col) const;

private:
    void loadFromFile(const std::string &filename);

    std::vector<std::string> grid;  // Maze layout as strings.
    int tileSize;                   // Pixel size of each cell.
    sf::Vector2i startPos;          // Starting position (must be open); we assume (1,1) here.

    sf::Texture doorTexture;        // For exit.
    sf::Texture wallTexture;        // For walls.
    sf::Texture grassTexture;       // For open path.
};

#endif // MAZE_H
