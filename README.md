----DEFEND THE KEEP----

--Introduction--
Defend the Keep is a Tower Defense game built using C++ and SFML. Inspired by Plants vs. Zombies and Clash Royale, the goal is to manage your gold, place defenses, and protect your castle from endless waves of enemies. You must use your gold wisely to construct defensive towers. If your Castle's health reaches zero, the game is over.
There are three types of towers:
	1. Archer Tower: Fast firing speed, single target. Good for weak enemies.
	2. Canon Tower: Area of effect damage. Great for groups.
	3. Mage Tower: High magic damage. Best for tough enemies.
The enemies coming in waves are Grunts, fast Scouts, and tanky Brutes.

--Controls--

1. Mouse Left Click: Select towers from the UI and place them on the green plots.
2. Mouse Left Click: To click on options from the UI for upgrades and place them on towers of choice.

--Instructions to run the game--

How to Build & Run

 1. Prerequisites

You need:

- A C++ compiler with C++17 support  
  - On Linux/WSL: `g++` (from GCC)  
- SFML 2.5 (or higher) development libraries:
  - graphics
  - window
  - system
  - audio

 On Ubuntu / WSL

```bash
sudo apt update
sudo apt install g++ libsfml-dev


2. Execute the main.cpp file in "src" folder from this following command: g++ -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer     -o game *.cpp     -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio.
3. To run the game, make sure you are in the Defend the Keep OOP folder. From there, in the terminal, type the following command: ./src/game	


GitHub link:  https://github.com/Khanx007/Defend-the-Keep-OOP

 


