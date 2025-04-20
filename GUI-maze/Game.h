#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Scoreboard.h"
#include "Level.h"
#include "Maze.h"
#include "SelectLevel.h"
#include "Credits.h"    // Credits module
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Snake.h"
#include "Collectables.h"  // New module for coin collectibles

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(sf::Time delta);
    void render();
    void initLevel();
    void handlePopupMouseClick(const sf::Vector2f &mousePos);

    // Window and font.
    sf::RenderWindow mWindow;
    sf::Font mFont;

    // UI images.
    sf::Texture mMouseIconTexture;
    sf::Sprite  mMouseIconSprite;
    sf::Texture mInsTexture;
    sf::Sprite  mInsSprite;
    sf::Texture mNameTexture;
    sf::Sprite  mNameSprite;
    sf::Texture mMuteTexture;
    sf::Sprite  mMuteSprite;

    // Music and sound control.
    sf::Music mBackgroundMusic;
    bool mMusicMuted;

    // Timing.
    sf::Time mMoveTimer;
    sf::Clock mGameClock;
    sf::Clock mPopupClock;
    float mPopupDuration;

    // Player name input.
    bool mWaitingForName;
    std::string mUserName;

    // Game screens/objects.
    Menu* mMenu;
    Level* mLevel;
    Snake* mSnake;
    Scoreboard* mScoreboard;
    SelectLevel* selectLevel;
    Credits* mCredits;  // Credits screen

    // New collectible coins.
    Collectables* mCollectables;  // Pointer to coin collectibles

    // Current coin-based score.
    int mCurrentScore;            // Current score earned from coins

    // Static snake texture used by the Snake class.
    static sf::Texture mSnakeTexture;

    // Buttons for level complete prompt.
    sf::RectangleShape nextLevelButton;
    sf::Text nextLevelButtonText;
    sf::RectangleShape mainMenuButton;
    sf::Text mainMenuButtonText;

    // Game state enumeration.
    enum GameState {
        MAIN_MENU,
        INSTRUCTIONS_SCREEN,
        PLAYING,
        LEVEL_COMPLETE_PROMPT,
        LEVEL_SELECT_STATE,
        SCOREBOARD_STATE,
        CREDITS_STATE  // Credits state
    } mGameState;

    int mCurrentLevel;
};

#endif // GAME_H
