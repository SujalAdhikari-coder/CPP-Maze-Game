#ifndef LEVEL2_H
#define LEVEL2_H

#include "Level.h"

class Level2 : public Level {
public:
    Level2(sf::Vector2u windowSize);
    ~Level2();
    void draw(sf::RenderWindow &window) override;
    Maze* getMaze() override;

private:
    Maze* maze;
};

#endif // LEVEL2_H
