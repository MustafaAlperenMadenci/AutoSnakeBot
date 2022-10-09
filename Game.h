#pragma once
#include "SDL2\include\SDL.h"
#include "SDL2\include\SDL_ttf.h"
#include <stdio.h>
#include <iostream>
#include <deque>
#include <random>
#include <vector>
#include <string>


class Game {

public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos,int width, int height, int gridSize, std::string mode,  bool fullscreen);
	
	void handleEvents();
	void update();
	void render();
	void clean();
	void drawGrids();
	void createGrids();
	void handleSnake();
	void drawSnake();
	void handleYemLocation();
	void drawYem();
	void gameOver();
	void gameWon();
	void scorePrint();
	bool running() { return isRunning; }
	bool playing = true;
	bool eventsGet = false;
	std::string gameMod;
private:

	int cnt = 0;
	int widthOfGame = 0;
	int heightOfGame = 0;
	int gridSizeOfGame = 0;
	int cellSizeWidth = 0;
	int cellSizeHeight = 0;
	int snakeSize = 0;
	int moveCounter = 0;
	bool gameFinishStatus = false;
	std::pair<int, int> yemLoc;
	std::pair<int, int> direction;
	std::pair<int, int> lastTailLoc;
	
	std::deque <std::pair <int,int> > snakeAll;
	std::deque <std::pair <int, int> > bestPath;
	int* xLocsGr;
	int* yLocsGr;
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;

};
struct NodeInfo {             // Structure declaration
	std::pair <int, int> headLoc; // Y and X
	std::pair <int, int> currentDir; // Y and X
	std::deque <std::pair <int, int> > currentPath; // Y and X
	std::deque <std::pair <int, int> > bodyLoc; // Y and X
	bool findStatus = false;

};
std::deque <std::pair <int, int> > reasonableShortestPath(std::deque <std::pair <int, int> > snakeBody, std::pair <int, int>& yemLoc, std::pair <int, int> currentDir, int& mapSize);
bool reasonableShortestPathTail(std::deque <std::pair <int, int> > snakeEngel, std::pair <int, int > snakeHead, std::pair <int, int>& yemLoc, std::pair <int, int> currentDir, int& mapSize);
void deleteSameCharacterNodes(std::vector<NodeInfo>& allNodes);
std::deque <std::pair <int, int> > moveBody(std::deque <std::pair <int, int> > bodyLoc , std::pair <int, int> oldHeadLoc);
std::pair<int, int> moveHead(std::pair <int, int> headLoc  , const std::pair <int, int>&  currentDir, int& mapSize);
int checkNextAlgo(const std::pair<int, int>& newHeadLoc, const std::pair<int, int>& yemLoc, const std::deque<std::pair<int, int>>& newBodyLoc);