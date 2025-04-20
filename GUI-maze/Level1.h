#ifndef LEVEL1_H
#define LEVEL1_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Level.h"
#include "Maze.h"

class Level1 : public Level {
public:
    Level1(sf::Vector2u windowSize);
    ~Level1();
    void draw(sf::RenderWindow &window) override;
    Maze* getMaze() override;

private:
    Maze* maze;
};

#endif // LEVEL1_H
