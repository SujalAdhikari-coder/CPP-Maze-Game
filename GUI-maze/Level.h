#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include "Maze.h"

class Level {
public:
    virtual ~Level() {}
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual Maze* getMaze() = 0;
};

#endif // LEVEL_H
