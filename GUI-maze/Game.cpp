#include "Game.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "SelectLevel.h"
#include "Credits.h"
#include "Collectables.h"
#include <iostream>
#include <cctype>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

// Define the static snake texture.
sf::Texture Game::mSnakeTexture;

Game::Game()
    : mWindow(sf::VideoMode::getDesktopMode(), "Maze Snake Game", sf::Style::Fullscreen)
    , mFont()
    , mMouseIconTexture()
    , mMouseIconSprite()
    , mInsTexture()
    , mInsSprite()
    , mNameTexture()
    , mNameSprite()
    , mMuteTexture()
    , mMuteSprite()
    , mBackgroundMusic()
    , mMusicMuted(false)
    , mMoveTimer(sf::Time::Zero)
    , mGameClock()
    , mPopupClock()
    , mPopupDuration(3.f)
    , mWaitingForName(false)
    , mUserName("")
    , mMenu(nullptr)
    , mLevel(nullptr)
    , mSnake(nullptr)
    , mScoreboard(nullptr)
    , selectLevel(nullptr)
    , mCredits(nullptr)
    , mCollectables(nullptr)
    , mCurrentScore(0)
    , nextLevelButton()
    , nextLevelButtonText()
    , mainMenuButton()
    , mainMenuButtonText()
    , mCurrentLevel(1)
    , mGameState(MAIN_MENU)
{
    mWindow.setFramerateLimit(60);
    mWindow.setKeyRepeatEnabled(false);

    // Load font.
    if (!mFont.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\shlop rg.otf"))
        std::cerr << "Error: Failed to load font." << std::endl;

    // Create main menu.
    mMenu = new Menu(mFont, mWindow);

    // Load custom mouse icon.
    if (!mMouseIconTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\mouse-icon.png"))
        std::cerr << "Failed to load mouse icon." << std::endl;
    mMouseIconSprite.setTexture(mMouseIconTexture);
    mWindow.setMouseCursorVisible(false);

    // Load instructions background.
    if (!mInsTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\ins-img.jpg"))
        std::cerr << "Failed to load instructions background." << std::endl;
    mInsSprite.setTexture(mInsTexture);
    {
        sf::Vector2u texSize = mInsTexture.getSize();
        sf::Vector2u winSize = mWindow.getSize();
        mInsSprite.setScale(static_cast<float>(winSize.x) / texSize.x,
                            static_cast<float>(winSize.y) / texSize.y);
    }

    // Load name prompt background.
    if (!mNameTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\name-image.jpg"))
        std::cerr << "Failed to load name prompt background." << std::endl;
    mNameSprite.setTexture(mNameTexture);
    {
        sf::Vector2u texSize = mNameTexture.getSize();
        sf::Vector2u winSize = mWindow.getSize();
        mNameSprite.setScale(static_cast<float>(winSize.x) / texSize.x,
                             static_cast<float>(winSize.y) / texSize.y);
    }

    // Load and play background music.
    if (!mBackgroundMusic.openFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\BG.wav"))
        std::cerr << "Failed to load background music." << std::endl;
    else {
        mBackgroundMusic.setLoop(true);
        mBackgroundMusic.setVolume(20.f);
        mBackgroundMusic.play();
    }

    // Load mute icon.
    if (!mMuteTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\mute.png"))
        std::cerr << "Failed to load mute icon." << std::endl;
    mMuteSprite.setTexture(mMuteTexture);
    {
        sf::Vector2u winSize = mWindow.getSize();
        float muteIconSize = winSize.y * 0.05f;
        sf::Vector2u iconTexSize = mMuteTexture.getSize();
        mMuteSprite.setScale(muteIconSize / iconTexSize.x, muteIconSize / iconTexSize.y);
        mMuteSprite.setPosition(winSize.x - muteIconSize - 10.f, winSize.y - muteIconSize - 10.f);
    }

    // Load snake texture.
    if (!mSnakeTexture.loadFromFile("E:\\C++\\GUI\\GUI-maze\\assets\\snake.png"))
        std::cerr << "Failed to load snake texture." << std::endl;

    // Credits module is created when selected from the main menu.
}

Game::~Game() {
    if (mMenu) delete mMenu;
    if (mLevel) delete mLevel;
    if (mSnake) delete mSnake;
    if (mScoreboard) delete mScoreboard;
    if (mCredits) delete mCredits;
    if (selectLevel) delete selectLevel;
    if (mCollectables) delete mCollectables;
}

void Game::run() {
    sf::Clock clock;
    while(mWindow.isOpen()){
        sf::Time delta = clock.restart();
        processEvents();
        update(delta);
        render();
    }
}

void Game::initLevel() {
    if (mLevel) {
        delete mLevel;
        mLevel = nullptr;
    }
    // Create level based on current level number.
    if (mCurrentLevel == 1)
        mLevel = new Level1(mWindow.getSize());
    else if (mCurrentLevel == 2)
        mLevel = new Level2(mWindow.getSize());
    else if (mCurrentLevel == 3)
        mLevel = new Level3(mWindow.getSize());

    // Initialize snake.
    if (mSnake) {
        delete mSnake;
        mSnake = nullptr;
    }
    mSnake = new Snake(mLevel->getMaze()->getStartPosition(), mLevel->getMaze()->getTileSize());

    // Initialize coin collectibles based on level.
    std::vector<sf::Vector2i> coinPositions;
    if (mCurrentLevel == 1) {
        coinPositions = { sf::Vector2i(3, 3), sf::Vector2i(10, 5), sf::Vector2i(15, 10) };
    }
    else if (mCurrentLevel == 2) {
        coinPositions = { sf::Vector2i(2, 2), sf::Vector2i(8, 6), sf::Vector2i(12, 10), sf::Vector2i(20, 4) };
    }
    else if (mCurrentLevel == 3) {
        coinPositions = { sf::Vector2i(4, 4), sf::Vector2i(9, 7), sf::Vector2i(14, 8), sf::Vector2i(18, 3) };
    }
    if (mCollectables) {
        delete mCollectables;
        mCollectables = nullptr;
    }
    mCollectables = new Collectables(coinPositions, mLevel->getMaze()->getTileSize());

    // Reset the current coin score.
    mCurrentScore = 0;
}

void Game::processEvents() {
    sf::Event event;
    while(mWindow.pollEvent(event)){
        if(event.type == sf::Event::Closed)
            mWindow.close();

        // Mute toggle.
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                  static_cast<float>(event.mouseButton.y));
            if(mMuteSprite.getGlobalBounds().contains(mousePos)){
                mMusicMuted = !mMusicMuted;
                mBackgroundMusic.setVolume(mMusicMuted ? 0.f : 20.f);
            }
        }

        // Global Escape handling for LEVEL_SELECT_STATE, SCOREBOARD_STATE, or CREDITS_STATE.
        if ((mGameState == LEVEL_SELECT_STATE || mGameState == SCOREBOARD_STATE || mGameState == CREDITS_STATE)
            && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            mGameState = MAIN_MENU;
            if(mGameState == LEVEL_SELECT_STATE && selectLevel){
                delete selectLevel;
                selectLevel = nullptr;
            }
        }

        if(mGameState == PLAYING && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
            if(!mWaitingForName && (mCurrentLevel == 2 || mCurrentLevel == 3)){
                mWindow.close();
                continue;
            }
            else{
                mGameState = MAIN_MENU;
                if(mLevel){ delete mLevel; mLevel = nullptr; }
                if(mSnake){ delete mSnake; mSnake = nullptr; }
                if(mScoreboard){ delete mScoreboard; mScoreboard = nullptr; }
                mUserName = "";
                mWaitingForName = false;
                continue;
            }
        }

        if(mGameState == MAIN_MENU && mMenu){
            mMenu->handleEvent(event);
        }
        else if(mGameState == INSTRUCTIONS_SCREEN){
            if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
               || (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left))
            {
                mGameState = MAIN_MENU;
            }
        }
        else if(mGameState == PLAYING){
            if(mWaitingForName){
                if(event.type == sf::Event::TextEntered){
                    if(event.text.unicode < 128){
                        char c = static_cast<char>(event.text.unicode);
                        if(c == '\b'){
                            if(!mUserName.empty())
                                mUserName.pop_back();
                        }
                        else if(c == '\r' || c == '\n'){
                            mWaitingForName = false;
                            mGameClock.restart();
                            mMoveTimer = sf::Time::Zero;
                            mCurrentLevel = 1;
                            initLevel();
                            if(mScoreboard == nullptr)
                                mScoreboard = new Scoreboard("E:\\C++\\GUI\\GUI-maze\\scoreboard.txt");
                        }
                        else if(std::isprint(c)){
                            mUserName.push_back(c);
                        }
                    }
                }
            }
            else{
                if(event.type == sf::Event::KeyPressed){
                    sf::Vector2i newDir(0,0);
                    if(event.key.code == sf::Keyboard::W)
                        newDir = sf::Vector2i(0,-1);
                    else if(event.key.code == sf::Keyboard::S)
                        newDir = sf::Vector2i(0,1);
                    else if(event.key.code == sf::Keyboard::A)
                        newDir = sf::Vector2i(-1,0);
                    else if(event.key.code == sf::Keyboard::D)
                        newDir = sf::Vector2i(1,0);
                    mSnake->setDirection(newDir);
                }
            }
        }
        else if(mGameState == LEVEL_COMPLETE_PROMPT){
            if(event.type == sf::Event::MouseButtonPressed &&
               event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                handlePopupMouseClick(mousePos);
            }
        }
        else if(mGameState == LEVEL_SELECT_STATE){
            if(selectLevel)
                selectLevel->handleEvent(event);
        }
        else if(mGameState == SCOREBOARD_STATE && mScoreboard){
            mScoreboard->handleEvent(event, mWindow);
        }
        else if(mGameState == CREDITS_STATE && mCredits){
            mCredits->handleEvent(event, mWindow);
        }
    }
}

void Game::handlePopupMouseClick(const sf::Vector2f &mousePos) {
    if(nextLevelButton.getGlobalBounds().contains(mousePos)){
        if(mCurrentLevel < 3){
            mCurrentLevel++;
            initLevel();
            mGameClock.restart();
            mMoveTimer = sf::Time::Zero;
            mGameState = PLAYING;
        }
        else{
            mGameState = MAIN_MENU;
            if(mLevel){ delete mLevel; mLevel = nullptr; }
            if(mSnake){ delete mSnake; mSnake = nullptr; }
            mUserName = "";
        }
    }
    else if(mainMenuButton.getGlobalBounds().contains(mousePos)){
        mGameState = MAIN_MENU;
        if(mLevel){ delete mLevel; mLevel = nullptr; }
        if(mSnake){ delete mSnake; mSnake = nullptr; }
        mUserName = "";
    }
}

void Game::update(sf::Time delta) {
    if(mGameState == MAIN_MENU && mMenu){
        mMenu->update();
        MenuOption selectedOption;
        if(mMenu->isOptionSelected(selectedOption)){
            switch(selectedOption){
                case START_GAME:
                    mGameState = PLAYING;
                    mWaitingForName = true;
                    mUserName = "";
                    break;
                case INSTRUCTIONS:
                    mGameState = INSTRUCTIONS_SCREEN;
                    break;
                case SELECT_LEVEL:
                    mGameState = LEVEL_SELECT_STATE;
                    if(!selectLevel)
                        selectLevel = new SelectLevel(mFont, mWindow);
                    break;
                case SCOREBOARD:
                    mGameState = SCOREBOARD_STATE;
                    if(!mScoreboard)
                        mScoreboard = new Scoreboard("E:\\C++\\GUI\\GUI-maze\\scoreboard.txt");
                    break;
                case CREDITS:
                    mGameState = CREDITS_STATE;
                    if(!mCredits)
                        mCredits = new Credits(mWindow.getSize());
                    break;
                case EXIT_GAME:
                    mWindow.close();
                    break;
                default:
                    break;
            }
        }
    }
    else if(mGameState == LEVEL_SELECT_STATE){
        if(selectLevel){
            selectLevel->update();
            if(selectLevel->isLevelSelected()){
                int levelNum = selectLevel->getSelectedLevel();
                mCurrentLevel = levelNum;
                if(levelNum == 1)
                    mLevel = new Level1(mWindow.getSize());
                else if(levelNum == 2)
                    mLevel = new Level2(mWindow.getSize());
                else if(levelNum == 3)
                    mLevel = new Level3(mWindow.getSize());
                mGameState = PLAYING;
                delete selectLevel;
                selectLevel = nullptr;
                if(mSnake){
                    delete mSnake;
                    mSnake = nullptr;
                }
                mSnake = new Snake(mLevel->getMaze()->getStartPosition(), mLevel->getMaze()->getTileSize());
                mGameClock.restart();
                mMoveTimer = sf::Time::Zero;
            }
        }
    }
    else if(mGameState == SCOREBOARD_STATE){
        // Scoreboard state: no dynamic update.
    }
    else if(mGameState == CREDITS_STATE && mCredits){
        mCredits->update(delta);
    }
    else if(mGameState == PLAYING && !mWaitingForName){
        mMoveTimer += delta;
        if(mMoveTimer >= sf::seconds(0.5)){
            mMoveTimer = sf::Time::Zero;
            if(mSnake && mLevel){
                mSnake->update(*(mLevel->getMaze()));
                // Update coin animation.
                if(mCollectables)
                    mCollectables->update(delta);
                // Check coin collection.
                if(mCollectables){
                    int points = mCollectables->collectAt(mSnake->getHeadPosition());
                    if(points > 0)
                        mCurrentScore += points;
                }
                sf::Vector2i headPos = mSnake->getHeadPosition();
                std::cout << "Snake head at (" << headPos.x << ", " << headPos.y << ")\n";
                if(mLevel->getMaze()->isExit(headPos)){
                    float elapsedTime = mGameClock.getElapsedTime().asSeconds();
                    if(mScoreboard)
                        mScoreboard->addEntry(mUserName, elapsedTime, mCurrentScore);
                    mGameState = LEVEL_COMPLETE_PROMPT;

                    sf::Vector2f buttonSize(200.f, 50.f);
                    nextLevelButton.setSize(buttonSize);
                    nextLevelButton.setFillColor(sf::Color(100, 250, 50));
                    nextLevelButton.setOutlineColor(sf::Color::White);
                    nextLevelButton.setOutlineThickness(2.f);

                    mainMenuButton.setSize(buttonSize);
                    mainMenuButton.setFillColor(sf::Color(250, 100, 50));
                    mainMenuButton.setOutlineColor(sf::Color::White);
                    mainMenuButton.setOutlineThickness(2.f);

                    sf::Vector2u winSize = mWindow.getSize();
                    nextLevelButton.setPosition(winSize.x/2.f - buttonSize.x - 20.f, winSize.y/2.f + 50.f);
                    mainMenuButton.setPosition(winSize.x/2.f + 20.f, winSize.y/2.f + 50.f);

                    nextLevelButtonText.setFont(mFont);
                    nextLevelButtonText.setString("Next Level");
                    nextLevelButtonText.setCharacterSize(24);
                    nextLevelButtonText.setFillColor(sf::Color::Black);
                    {
                        sf::FloatRect textRect = nextLevelButtonText.getLocalBounds();
                        nextLevelButtonText.setOrigin(textRect.left + textRect.width/2.f,
                                                      textRect.top + textRect.height/2.f);
                        nextLevelButtonText.setPosition(nextLevelButton.getPosition().x + buttonSize.x/2.f,
                                                        nextLevelButton.getPosition().y + buttonSize.y/2.f);
                    }

                    mainMenuButtonText.setFont(mFont);
                    mainMenuButtonText.setString("Main Menu");
                    mainMenuButtonText.setCharacterSize(24);
                    mainMenuButtonText.setFillColor(sf::Color::Black);
                    {
                        sf::FloatRect textRect = mainMenuButtonText.getLocalBounds();
                        mainMenuButtonText.setOrigin(textRect.left + textRect.width/2.f,
                                                     textRect.top + textRect.height/2.f);
                        mainMenuButtonText.setPosition(mainMenuButton.getPosition().x + buttonSize.x/2.f,
                                                       mainMenuButton.getPosition().y + buttonSize.y/2.f);
                    }
                }
            }
        }
    }
}

void Game::render() {
    mWindow.clear(sf::Color::Black);

    if(mGameState == MAIN_MENU && mMenu){
        mMenu->draw(mWindow);
    }
    else if(mGameState == INSTRUCTIONS_SCREEN){
        mWindow.draw(mInsSprite);
        sf::Text instructionsText;
        instructionsText.setFont(mFont);
        instructionsText.setCharacterSize(24);
        instructionsText.setFillColor(sf::Color::White);
        instructionsText.setString("GAME INSTRUCTIONS:\n\n"
                                   "1. You have 3 seconds to study the maze before the game starts.\n"
                                   "2. The snake moves automatically every 0.5 seconds.\n"
                                   "3. Use the W, A, S, D keys to change direction.\n"
                                   "4. Avoid walls and collect coins (+10 points each) while reaching the exit.\n"
                                   "5. Your completion time and coin score are recorded on the scoreboard.\n\n"
                                   "Press Escape or click anywhere to return to the main menu.");
        sf::FloatRect textRect = instructionsText.getLocalBounds();
        instructionsText.setOrigin(textRect.left + textRect.width/2.f,
                                   textRect.top + textRect.height/2.f);
        instructionsText.setPosition(mWindow.getSize().x/2.f, mWindow.getSize().y/2.f);
        mWindow.draw(instructionsText);
    }
    else if(mGameState == PLAYING){
        if(mWaitingForName){
            mWindow.draw(mNameSprite);
            sf::Text namePrompt;
            namePrompt.setFont(mFont);
            namePrompt.setCharacterSize(24);
            namePrompt.setFillColor(sf::Color::White);
            namePrompt.setString("Enter your name: " + mUserName + "\nPress Enter when done.");
            sf::FloatRect rect = namePrompt.getLocalBounds();
            namePrompt.setOrigin(rect.left + rect.width/2.f,
                                 rect.top + rect.height/2.f);
            namePrompt.setPosition(mWindow.getSize().x/2.f, mWindow.getSize().y/2.f);
            mWindow.draw(namePrompt);
        }
        else {
            if(mLevel) mLevel->draw(mWindow);
            if(mCollectables) mCollectables->draw(mWindow);
            if(mSnake) mSnake->draw(mWindow);
            // Draw the score overlay.
            sf::Text scoreText;
            scoreText.setFont(mFont);
            scoreText.setCharacterSize(28);
            scoreText.setFillColor(sf::Color::Yellow);
            scoreText.setStyle(sf::Text::Bold | sf::Text::Italic);
            scoreText.setString("Score: " + std::to_string(mCurrentScore));
            scoreText.setPosition(15.f, 15.f);
            mWindow.draw(scoreText);
        }
    }
    else if(mGameState == LEVEL_COMPLETE_PROMPT){
        if(mLevel) mLevel->draw(mWindow);
        if(mSnake) mSnake->draw(mWindow);
        sf::Text popupText;
        popupText.setFont(mFont);
        popupText.setCharacterSize(36);
        popupText.setFillColor(sf::Color::Yellow);
        popupText.setStyle(sf::Text::Bold);
        popupText.setString("Congratulations! Level " + std::to_string(mCurrentLevel) + " Completed!");
        sf::FloatRect rect = popupText.getLocalBounds();
        popupText.setOrigin(rect.left + rect.width/2.f,
                            rect.top + rect.height/2.f);
        popupText.setPosition(mWindow.getSize().x/2.f, mWindow.getSize().y/2.f - 50.f);
        mWindow.draw(popupText);
        mWindow.draw(nextLevelButton);
        mWindow.draw(nextLevelButtonText);
        mWindow.draw(mainMenuButton);
        mWindow.draw(mainMenuButtonText);
    }
    else if(mGameState == LEVEL_SELECT_STATE && selectLevel){
        selectLevel->draw(mWindow);
    }
    else if(mGameState == SCOREBOARD_STATE && mScoreboard){
        mScoreboard->display(mWindow, mFont);
    }
    else if(mGameState == CREDITS_STATE && mCredits){
        mCredits->draw(mWindow, mFont);
    }

    mWindow.draw(mMuteSprite);
    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    mMouseIconSprite.setPosition(static_cast<float>(mousePos.x),
                                 static_cast<float>(mousePos.y));
    mWindow.draw(mMouseIconSprite);

    mWindow.display();
}
