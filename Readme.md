# Maze Snake Game with Collectibles

## Overview

Maze Snake Game is a 2D maze game built using SFML in C++. Players control a snake navigating through a maze, collecting coins to increase their score while avoiding walls. The game includes multiple levels with increasing difficulty, a scoreboard saved to file, and detailed game state management. This project was developed as part of a college assessment to demonstrate file I/O operations, 2D array manipulation, user input handling, collision detection, and object-oriented design.

## Game Features

- **Multi-Level Maze:**  
  Three distinct levels (Level1, Level2, Level3) with progressively challenging maze layouts.

- **Player Movement & Collision Detection:**  
  The snake is controlled using W, A, S, and D keys. The game detects collisions with walls, ensuring the snake can only travel through open paths.

- **Collectible Items:**  
  Special coin collectibles are scattered throughout the maze. Each coin increases the player's score by 10 points when collected. Coins feature a smooth bouncing (drop) animation.

- **Game States & Menus:**  
  The game uses multiple states including:
  - **Main Menu** – Offers options for Start Game, Instructions, Level Select, Scoreboard, Credits, and Exit.
  - **Instructions Screen** – Explains game controls and rules.
  - **Level Select** – Allows players to choose a level before starting the game.
  - **Name Entry** – Prompts players to enter their name (for scoreboard purposes) before gameplay.
  - **Playing State** – Actual gameplay where snake movement, coin collection, and collision detection occur.
  - **Scoreboard & Credits** – Displays high scores and credits respectively.
  
- **Score Tracking:**  
  The current score is displayed on-screen during gameplay, and when a level is completed, the player's name, completion time, and score are recorded.

- **File I/O Operations:**  
  The game loads maze layouts and saves scoreboard entries to a file, demonstrating basic file I/O in C++.

## Requirements

- **Operating System:**  
  Windows (the game uses full-screen mode; however, it can be adapted for other platforms).

- **Dependencies:**  
  - SFML (Simple and Fast Multimedia Library) version 2.5.1 or later  
    [SFML download and installation instructions](https://www.sfml-dev.org/download.php)

- **Compiler:**  
  A modern C++ compiler that supports C++11 or newer (e.g., Visual Studio, MinGW, or Clang).

## Installation & Building

1. **Clone the Repository:**  
   ```bash
   git clone https://github.com/SujalAdhikari-coder/CPP-Maze-Game.git
