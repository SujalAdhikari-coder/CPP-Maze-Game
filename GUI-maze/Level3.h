#ifndef LEVEL3_H
#define LEVEL3_H

#include "Level.h"

class Level3 : public Level {
public:
    Level3(sf::Vector2u windowSize);
    ~Level3();
    void draw(sf::RenderWindow &window) override;
    Maze* getMaze() override;

private:
    Maze* maze;
};

#endif // LEVEL3_H
