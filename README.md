#  Block Bash

**Block Bash** is a 2D brick breaker-style arcade game developed in **C++** using the **SFML (Simple and Fast Multimedia Library)**. Players control a paddle to bounce a ball and break rows of colorful blocks. The game features dynamic level progression, real-time collision detection, sound effects, and high score tracking — all wrapped in a modular object-oriented design.

---

##  Table of Contents

- [Features](#features)
- [Gameplay Overview](#gameplay-overview)
- [Tools & Technologies](#tools--technologies)
- [How to Play](#how-to-play)
- [Controls](#controls)
- [Project Structure](#project-structure)
- [Conclusion](#conclusion)

---

##  Features

-  Classic block-breaking gameplay
-  Level-up every 100 points
-  Randomized ball angle after each level to avoid repetition
-  Integrated sound and music (hit and game over effects)
-  Real-time scoring with high score saving
-  Smooth paddle and ball physics
-  File-based high score persistence
-  Clean, extensible object-oriented design

  ## Gameplay Overview


###  Game Entities:
- **Paddle** – Controlled with arrow keys; reflects the ball.
- **Ball** – Bounces autonomously off paddle, walls, and blocks.
- **Block** – Breakable objects that increase score when hit.

###  Ball Mechanics:
- Starts diagonally.
- Bounces on paddle/walls.
- On every level-up (each 100 points):
  - Ball speed increases
  - Direction changes randomly to prevent vertical loops

###  Level Progression:
- Each block gives +10 points.
- New level every 100 points.
- More rows of blocks added each level.

###  Game Over:
- Happens when the ball hits the bottom.
- Game Over screen shown with option to restart.


##  Tools & Technologies

 Tool - Purpose 

 **C++** - Core programming 
 **SFML** - Rendering, input, sound 
 **Code::Blocks / Visual Studio / Clion** - IDE/Compiler 
 **Textures & Audio** - ball.png, paddle.png, background.jpg, block textures, `music.ogg`, `hit.wav`, `gameover.wav` 
 **Font** - arial.ttf` 
 **File I/O** -`highscore.txt` for persistent storage 



##  How to Play

1. Launch the game executable.
2. Use arrow keys to move the paddle.
3. Bounce the ball to destroy all blocks.
4. Earn points and progress through levels.
5. If the ball hits the bottom, it's game over.
6. Press `R` to restart after a game over.



##  Controls

| Key | Action |
|-----|--------|
| ← / → | Move Paddle |
| R     | Restart Game (after Game Over) |


##  Project Structure


BlockBash/
├── main.cpp                # Main game source code
├── resources/              # All textures, sounds, fonts
│   ├── ball.png
│   ├── paddle.png
│   ├── block.png, block02.png, ...
│   ├── background.jpg
│   ├── music.ogg
│   ├── hit.wav
│   ├── gameover.wav
│   └── arial.ttf
├── highscore.txt           # Stores high score
├── README.md               # GitHub documentation
├── Block Bash Documentation.pdf  # Formal project report



##  Conclusion

**Block Bash** demonstrates how to build an interactive arcade-style game using **C++** and **SFML**. It covers fundamental game development concepts including physics, file handling, real-time input, graphics rendering, and audio integration. With a modular OOP structure, this project serves as a strong foundation for future enhancements like power-ups, combo scoring, and multi-ball gameplay.



###  Submitted By:
**Hiranaya Yadav (CS&DF)**  
 Kathmandu, Nepal  
 April 21, 2025



### Submitted To:
**Aasish Acharya Sir**  
The British College, Kathmandu


