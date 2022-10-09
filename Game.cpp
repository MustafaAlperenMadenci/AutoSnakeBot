#include "Game.h"


Game::Game()
{

}
Game::~Game()
{
	delete[] xLocsGr;
	delete[] yLocsGr;
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, int gridSize, std::string mode, bool fullscreen)
{
	widthOfGame = width;
	heightOfGame = height;
	gridSizeOfGame = gridSize;
	gameMod = mode;

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		
		std::cout << "Subsystems Initialised!..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width+1, height+1, flags); // +1 added in order to offset a little bit so that last grid lines can be seen
		if (window)
		{
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			std::cout << "Renderer created" << std::endl;
		}
		isRunning = true;
		createGrids();
		//Creating Snake
		std::random_device rd;     // Only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, gridSizeOfGame-1); // Guaranteed unbiased
		snakeAll.push_back(std::make_pair(uni(rng), uni(rng))); // buraya random sayý koycam...
		direction.first = 1;
		direction.second = 0;
		handleYemLocation();
		if (gameMod[0] == 'a') {
			bestPath = reasonableShortestPath(snakeAll, yemLoc, direction, gridSizeOfGame);
		}
	}
	else {
		isRunning = false;
	}
}

void Game::createGrids()
{
	// This place need to be corrected !! Every frame i am re calculating grids
	int leftXloc = 0;
	int leftYloc = 0;
	xLocsGr = new int[gridSizeOfGame];
	yLocsGr = new int[gridSizeOfGame];
	cellSizeWidth = widthOfGame / (gridSizeOfGame);
	cellSizeHeight = heightOfGame / (gridSizeOfGame);
	for (int i = 0; i < gridSizeOfGame; i++) {
		xLocsGr[i] = leftXloc + (i * cellSizeWidth);
		yLocsGr[i] = leftYloc + (i * cellSizeHeight);
	}

}
void Game::drawGrids()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 5);
	
	for (int i = 0; i < gridSizeOfGame; i++) {
		SDL_RenderDrawLine(renderer, xLocsGr[i], 0, xLocsGr[i], heightOfGame);
	}
	for (int i = 0; i < gridSizeOfGame; i++) {
		SDL_RenderDrawLine(renderer, 0, yLocsGr[i], widthOfGame, yLocsGr[i]);
	}
	//Creating extra lines to close up the area;
	SDL_RenderDrawLine(renderer, xLocsGr[gridSizeOfGame-1] + cellSizeWidth , 0, xLocsGr[gridSizeOfGame - 1] + cellSizeWidth , heightOfGame);
	SDL_RenderDrawLine(renderer, 0 ,  yLocsGr[gridSizeOfGame-1] + cellSizeHeight, widthOfGame , yLocsGr[gridSizeOfGame-1] + cellSizeHeight);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // back to white

}

void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
				/* Look for a keypress */
			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch (event.key.keysym.sym) {
					case SDLK_LEFT:
						if (direction.second != 1 && eventsGet == false) {
							direction.first = 0;
							direction.second = -1;
							eventsGet = true;
						}
						break;
					case SDLK_RIGHT:
						if (direction.second != -1 && eventsGet == false) {
							direction.first = 0;
							direction.second = 1;
							eventsGet = true;
						}
						break;
					case SDLK_UP:
						if (direction.first != 1 && eventsGet == false) {
							direction.first = -1;
							direction.second = 0;
							eventsGet = true;
						}
						break;
					case SDLK_DOWN:
						if (direction.first != -1 && eventsGet == false) {
							direction.first = 1;
							direction.second = 0;
							eventsGet = true;
						}
						break;
					default:
						break;
			}
		}
	}
	if (gameMod[0] == 'a') {
		if (bestPath.size() == 0) {
			playing = false;
			gameFinishStatus = false;
		}
		else {
			direction.first = bestPath[moveCounter].first;
			direction.second = bestPath[moveCounter].second;
			moveCounter++;
		}
	}
	else {

	}


}

void Game::update()
{
	//cnt++;
	//std::cout << "cnt" << std::endl;
	//std::cout << "scs rakunnnnn" << std::endl;
	handleSnake();
}

void Game::handleSnake()
{
	
	int newPosY = 0;
	int newPosX = 0;
	snakeSize = (int)snakeAll.size();
	int HI = snakeSize - 1; // index of head
	int TI = 0; // index of tail
	// Handle Head 
	if (snakeAll[HI].first == gridSizeOfGame - 1 || snakeAll[HI].first == 0 || snakeAll[HI].second == gridSizeOfGame - 1 || snakeAll[HI].second == 0) {
		if (snakeAll[HI].first == gridSizeOfGame - 1 && direction.first == 1) {
			newPosY = 0;
			newPosX = snakeAll[HI].second;
		}
		else if (snakeAll[HI].first == 0 && direction.first == -1) {
			newPosY = gridSizeOfGame - 1;
			newPosX = snakeAll[HI].second;
		}
		else if (snakeAll[HI].second == gridSizeOfGame - 1 && direction.second == 1) {
			newPosY = snakeAll[HI].first;
			newPosX = 0;
		}
		else if (snakeAll[HI].second == 0 && direction.second == -1) {
			newPosY = snakeAll[HI].first;
			newPosX = gridSizeOfGame - 1;
		}
		else {
			newPosY = snakeAll[HI].first + direction.first;
			newPosX = snakeAll[HI].second + direction.second;
		}
	}
	else {
		newPosY = snakeAll[HI].first + direction.first;
		newPosX = snakeAll[HI].second + direction.second;
	}

	// Handle Tail
	if (newPosY != yemLoc.first || newPosX != yemLoc.second) {
		lastTailLoc.first = snakeAll[TI].first;
		lastTailLoc.second = snakeAll[TI].second;
		snakeAll.pop_front();
		snakeSize = snakeSize - 1;
	}
	for (int i = 0; i < snakeSize; i++) {
		if (newPosY == snakeAll[i].first && newPosX == snakeAll[i].second) {
			playing = false;
			break;
		}
	}
	snakeAll.push_back(std::make_pair(newPosY, newPosX));
	if (newPosY == yemLoc.first && newPosX == yemLoc.second) {
		handleYemLocation();
		moveCounter = 0;
		if (gameMod[0] == 'a'){
			bestPath = reasonableShortestPath(snakeAll, yemLoc, direction, gridSizeOfGame);
		}
		
	}
}

void Game::drawSnake()
{
	int snakeSize = (int)snakeAll.size();
	int snakeIndexX, snakeIndexY;
	SDL_Rect rect{};
	
	for (int i = 0; i < snakeSize; i++) {
		snakeIndexX = snakeAll[i].second;
		snakeIndexY = snakeAll[i].first;


		if (i == snakeSize - 1) {
			SDL_SetRenderDrawColor(renderer, 255, 204, 38, 255);
			rect.x = xLocsGr[snakeIndexX] + cellSizeWidth / 10;
			rect.y = yLocsGr[snakeIndexY] + cellSizeHeight / 10;
			rect.w = cellSizeWidth / 10 * 8;
			rect.h = cellSizeHeight / 10 * 8;
		}
		else if (i == 0 && snakeSize > 0) 
		{
			SDL_SetRenderDrawColor(renderer, 255, 170, 2, 255);
			rect.x = xLocsGr[snakeIndexX] + cellSizeWidth / 10 * 2;
			rect.y = yLocsGr[snakeIndexY] + cellSizeHeight / 10 * 2;
			rect.w = cellSizeWidth / 10 * 6;
			rect.h = cellSizeHeight / 10 * 6;
		}
		else {
			SDL_SetRenderDrawColor(renderer, 255, 170, 2, 255);
			rect.x = xLocsGr[snakeIndexX] + cellSizeWidth/10;
			rect.y = yLocsGr[snakeIndexY] + cellSizeHeight/10;
			rect.w = cellSizeWidth/10*8;
			rect.h = cellSizeHeight/10*8;
		}
		
		SDL_RenderFillRect(renderer, &rect);
	}
	
}

void Game::handleYemLocation()
{
	std::vector<std::pair<int, int> > avaYemLocs;
	snakeSize = (int)snakeAll.size();
	bool inside = false;
	for (int i = 0; i < gridSizeOfGame; i++) {
		for (int j = 0; j < gridSizeOfGame; j++) {
			inside = false;
			for (int k = 0; k < snakeSize; k++) {
				if (i == snakeAll[k].first && j == snakeAll[k].second) {
					inside = true;
					break;
				}
			}
			if (!inside) {
				avaYemLocs.push_back(std::make_pair(i,j));
			}
		}
	}
	std::random_device rd;     // Only used once to initialise (seed) engine
	std::mt19937 rng(rd());
	if ((int)avaYemLocs.size() == 0) {
		playing = false;
		gameFinishStatus = true;
		gameWon();
	}
	else {
		std::uniform_int_distribution<int> yemUni(0, (int)avaYemLocs.size() - 1); // Guaranteed unbiased
		int indexOfYem = yemUni(rng);
		yemLoc.first = avaYemLocs[indexOfYem].first;
		yemLoc.second = avaYemLocs[indexOfYem].second;
	}
	
	
}

void Game::drawYem()
{

	SDL_Rect rect{};

	rect.x = xLocsGr[yemLoc.second] + cellSizeWidth / 10 * 2;
	rect.y = yLocsGr[yemLoc.first] + cellSizeHeight / 10 * 2;
	rect.w = cellSizeWidth / 10 * 6;
	rect.h = cellSizeHeight / 10 * 6;
	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
	SDL_RenderFillRect(renderer, &rect);

}

void Game::gameOver() 
{
	TTF_Init();
	TTF_Font* myFont = TTF_OpenFont("NotoSansMono-ExtraBold.ttf", 24);
	SDL_Color Red = { 255, 0 , 0 };
	SDL_Surface* surfaceOverMessage = TTF_RenderText_Solid(myFont, "GODDAMIT RAKUN !" , Red);
	SDL_Texture* overMessage = SDL_CreateTextureFromSurface(renderer, surfaceOverMessage);
	SDL_Rect overMessageRect{};
	overMessageRect.x = widthOfGame/10;
	overMessageRect.y = heightOfGame/3;
	overMessageRect.w = (widthOfGame / 10) * 8;
	overMessageRect.h = heightOfGame / 4;
	SDL_RenderCopy(renderer, overMessage, NULL, &overMessageRect);
	SDL_FreeSurface(surfaceOverMessage);
	SDL_DestroyTexture(overMessage);
	TTF_CloseFont(myFont);
}
void Game::gameWon()
{
	TTF_Init();
	TTF_Font* myFont = TTF_OpenFont("NotoSansMono-ExtraBold.ttf", 24);
	SDL_Color Red = { 255, 0 , 0 };
	SDL_Surface* surfaceOverMessage = TTF_RenderText_Solid(myFont, "Congrats RAKUN !", Red);
	SDL_Texture* overMessage = SDL_CreateTextureFromSurface(renderer, surfaceOverMessage);
	SDL_Rect overMessageRect{};
	overMessageRect.x = widthOfGame / 10;
	overMessageRect.y = heightOfGame / 3;
	overMessageRect.w = (widthOfGame / 10) * 8;
	overMessageRect.h = heightOfGame / 4;
	SDL_RenderCopy(renderer, overMessage, NULL, &overMessageRect);
	SDL_FreeSurface(surfaceOverMessage);
	SDL_DestroyTexture(overMessage);
	TTF_CloseFont(myFont);
}
void Game::scorePrint()
{
	int sizeOfSnake = (int)snakeAll.size();
	std::string sizeOfSnakeStr = std::to_string(sizeOfSnake);
	std::string printString = "Score is: " + sizeOfSnakeStr;
	TTF_Init();
	TTF_Font* myFont = TTF_OpenFont("NotoSansMono-ExtraBold.ttf", 24);
	SDL_Color Red = { 255, 0 , 0 };
	SDL_Surface* surfaceOverMessage = TTF_RenderText_Solid(myFont, printString.c_str(), Red);
	SDL_Texture* overMessage = SDL_CreateTextureFromSurface(renderer, surfaceOverMessage);
	SDL_Rect overMessageRect{};
	overMessageRect.x = widthOfGame / 10 * 7;
	overMessageRect.y = heightOfGame / 10 * 9;
	overMessageRect.w = (widthOfGame / 10) * 2;
	overMessageRect.h = heightOfGame / 10 * 1;
	SDL_RenderCopy(renderer, overMessage, NULL, &overMessageRect);
	SDL_FreeSurface(surfaceOverMessage);
	SDL_DestroyTexture(overMessage);
	TTF_CloseFont(myFont);
}
void Game::render()
{
	SDL_RenderClear(renderer);
	// This is where we would add stuff to render
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	drawGrids();
	scorePrint();
	drawSnake();
	drawYem();
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	if (playing == false) {
		if (gameFinishStatus) {
			gameWon();
		}
		else {
			gameOver();
		}
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{

	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

std::deque <std::pair <int, int> > reasonableShortestPath(std::deque <std::pair <int, int> > snakeBody ,  std::pair <int,int>& yemLoc , std::pair <int, int> currentDir, int& mapSize)
{
	int checkHead = 0;
	std::deque <std::pair <int, int> > newBodyLoc;
	std::pair <int, int> newHeadLoc;
	int possibleDirections[4][2] = { {1,0},{0,1},{-1,0},{0,-1} };
	std::vector<NodeInfo>::iterator it;
	std::vector<NodeInfo>::iterator it2;
	int currentDepth = 0;
	int limitDepth = 2000; // Bura deðiþecek
	int equiavelentNodesCount;
	std::deque <std::pair <int, int> > firstPath; //???
	std::vector<NodeInfo> allNodesNow;
	std::vector<NodeInfo> allNodesNext;
	NodeInfo firstNode;
	NodeInfo resultNode;
	NodeInfo insertNode;
	int snakeSize = (int)snakeBody.size();
	int HI = snakeSize - 1; // index of head
	firstNode.headLoc = snakeBody[HI];
	snakeBody.pop_back();
	firstNode.currentDir = currentDir;
	firstNode.currentPath = firstPath;
	firstNode.bodyLoc = snakeBody;
	allNodesNow.push_back(firstNode);
	bool status = false;
	bool repeatStatus = false;
	while (status == false) {
		//ActiveNodes = []
		currentDepth++;
		if (currentDepth > limitDepth) {
			break;
		}
		equiavelentNodesCount = 0;
		//deleteSameCharacterNodes(allNodesNow);
		//std::cout << "Current Depth is :" << currentDepth << " limitDepth is: " << limitDepth << std::endl;
		if (allNodesNow.size() == 0) {
			//std::cout << "I could not find" << std::endl;
			break;
		}
		for (it = allNodesNow.begin(); it != allNodesNow.end();) {
			for (int i = 0; i < 4; i++) {
				if (it->currentDir.first == -1*possibleDirections[i][0] && it->currentDir.second == -1*possibleDirections[i][1]) {
					continue;
				}
				//CheckNextNewAlgo fonksiyonu olan yer....
				newHeadLoc = moveHead(it->headLoc, std::make_pair(possibleDirections[i][0], possibleDirections[i][1]), mapSize);
				repeatStatus = false;
				for (it2 = allNodesNext.begin(); it2 != allNodesNext.end(); ++it2) {
					if (newHeadLoc == it2->headLoc && std::make_pair(possibleDirections[i][0], possibleDirections[i][1]) == it2->currentDir) {
						repeatStatus = true;
						break;
					}
				}
				if (repeatStatus) {
					continue;
				}
				newBodyLoc = moveBody(it->bodyLoc,it->headLoc);
				checkHead = checkNextAlgo(newHeadLoc, yemLoc, newBodyLoc);
				if (checkHead == 0) {
					//std::cout << "Duvar Bu line silki perf artsýn" << std::endl;
				}
				else if (checkHead == 2) {
					if (snakeSize == 1) {
						newBodyLoc.push_front(it->headLoc); // since when it does not have any size, previous head location is actualy its tail
					}
					else {
						newBodyLoc.push_front(it->bodyLoc[0]);
					}
					//Buraya tail kontrolü gelecek
					if (reasonableShortestPathTail(newBodyLoc, newHeadLoc, newBodyLoc[0], std::make_pair(possibleDirections[i][0], possibleDirections[i][1]), mapSize)) {
						resultNode.headLoc = newHeadLoc;
						resultNode.currentDir = std::make_pair(possibleDirections[i][0], possibleDirections[i][1]);
						resultNode.findStatus = true;
						resultNode.bodyLoc = newBodyLoc;
						it->currentPath.push_back(resultNode.currentDir);
						resultNode.currentPath = it->currentPath;
						//std::cout << "You find your yem !!! " << std::endl;
						break;
					}
					
				}
				else {
					
					insertNode.bodyLoc = newBodyLoc;
					insertNode.currentDir = std::make_pair(possibleDirections[i][0], possibleDirections[i][1]);
					insertNode.currentPath = it->currentPath;
					insertNode.currentPath.push_back(insertNode.currentDir);
					insertNode.findStatus = false;
					insertNode.headLoc = newHeadLoc;
					//allNodesNext.push_back(insertNode);
					allNodesNext.emplace_back(insertNode);
				}

			}
			if (resultNode.findStatus) {
				break;
			}
			//it = allNodesNow.erase(it);
			it++;
		}
		if (resultNode.findStatus) {
			break;
		}
		allNodesNow = std::move(allNodesNext);
		allNodesNext.clear();
	}


	return resultNode.currentPath;
}
bool reasonableShortestPathTail(std::deque <std::pair <int, int> > snakeEngel , std::pair <int, int > snakeHead, std::pair <int, int>& yemLoc, std::pair <int, int> currentDir, int& mapSize)
{
	int checkHead = 0;
	std::deque <std::pair <int, int> > newBodyLoc;
	std::pair <int, int> newHeadLoc;
	int possibleDirections[4][2] = { {1,0},{0,1},{-1,0},{0,-1} };
	std::vector<NodeInfo>::iterator it;
	std::vector<NodeInfo>::iterator it2;
	int currentDepth = 0;
	int limitDepth = 2000; // Bura deðiþecek
	int equiavelentNodesCount;
	std::deque <std::pair <int, int> > firstPath; //???
	std::vector<NodeInfo> allNodesNow;
	std::vector<NodeInfo> allNodesNext;
	NodeInfo firstNode;
	NodeInfo resultNode;
	NodeInfo insertNode;
	//int snakeSize = (int)snakeBody.size();
	//int HI = snakeSize - 1; // index of head
	firstNode.headLoc = snakeHead;
	//snakeBody.pop_back();
	firstNode.currentDir = currentDir;
	firstNode.currentPath = firstPath;
	firstNode.bodyLoc = snakeEngel;
	allNodesNow.push_back(firstNode);
	bool status = false;
	bool repeatStatus = false;
	while (status == false) {
		//ActiveNodes = []
		currentDepth++;

		if (currentDepth > limitDepth) {
			break;
		}
		//deleteSameCharacterNodes(allNodesNow);
		//std::cout << "Current Depth is :" << currentDepth << " limitDepth is: " << limitDepth << std::endl;
		if (allNodesNow.size() == 0) {
			//std::cout << "I could not find" << std::endl;
			break;
		}
		for (it = allNodesNow.begin(); it != allNodesNow.end();) {
			for (int i = 0; i < 4; i++) {
				if (it->currentDir.first == -1 * possibleDirections[i][0] && it->currentDir.second == -1 * possibleDirections[i][1]) {
					continue;
				}
				//CheckNextNewAlgo fonksiyonu olan yer....
				newHeadLoc = moveHead(it->headLoc, std::make_pair(possibleDirections[i][0], possibleDirections[i][1]), mapSize);
				repeatStatus = false;
				for (it2 = allNodesNext.begin(); it2 != allNodesNext.end(); ++it2) {
					if (newHeadLoc == it2->headLoc && std::make_pair(possibleDirections[i][0], possibleDirections[i][1]) == it2->currentDir) {
						repeatStatus = true;
						break;
					}
				}
				if (repeatStatus) {
					continue;
				}
				newBodyLoc = moveBody(it->bodyLoc, it->headLoc);
				checkHead = checkNextAlgo(newHeadLoc, yemLoc, newBodyLoc);
				if (checkHead == 0) {
					//std::cout << "Duvar Bu line silki perf artsýn" << std::endl;
				}
				else if (checkHead == 2) {
					newBodyLoc.push_front(it->bodyLoc[0]);
					//Buraya tail kontrolü gelecek
					resultNode.headLoc = newHeadLoc;
					resultNode.currentDir = std::make_pair(possibleDirections[i][0], possibleDirections[i][1]);
					resultNode.findStatus = true;
					resultNode.bodyLoc = newBodyLoc;
					it->currentPath.push_back(resultNode.currentDir);
					resultNode.currentPath = it->currentPath;
					std::cout << "You find your yem !!! " << std::endl;
					break;
				}
				else {
					insertNode.bodyLoc = newBodyLoc;
					insertNode.currentDir = std::make_pair(possibleDirections[i][0], possibleDirections[i][1]);
					insertNode.currentPath = it->currentPath;
					insertNode.currentPath.push_back(insertNode.currentDir);
					insertNode.findStatus = false;
					insertNode.headLoc = newHeadLoc;
					allNodesNext.push_back(insertNode);
				}

			}
			if (resultNode.findStatus) {
				break;
			}
			//it = allNodesNow.erase(it);
			it++;
		}
		if (resultNode.findStatus) {
			break;
		}
		allNodesNow = std::move(allNodesNext);
		allNodesNext.clear();
	}


	return resultNode.findStatus;
}

int checkNextAlgo(const std::pair<int, int>& newHeadLoc, const std::pair<int, int>& yemLoc, const std::deque<std::pair<int, int> >& newBodyLoc)
{
	int sizeOfBody = (int)newBodyLoc.size();
	for (int i = 0; i < sizeOfBody; i++) {
		if (newHeadLoc.first == newBodyLoc[i].first && newHeadLoc.second == newBodyLoc[i].second) {
			return 0;
		}
	}
	if (newHeadLoc.first == yemLoc.first && newHeadLoc.second == yemLoc.second) {
		return 2;
	}
	return 1; // meaning no action
}

std::deque<std::pair<int, int>> moveBody(std::deque<std::pair<int, int>> bodyLoc, std::pair <int, int> oldHeadLoc)
{
	bodyLoc.push_back(oldHeadLoc);
	bodyLoc.pop_front();
	return bodyLoc;
}

std::pair<int, int> moveHead(std::pair<int, int> headLoc, const std::pair<int, int>& currentDir, int& mapSize)
{
	if (currentDir.first < 0 || currentDir.first > 0) {
		if (currentDir.first < 0) {
			if (headLoc.first == 0) {
				headLoc.first = mapSize - 1;
			}
			else {
				headLoc.first = headLoc.first + currentDir.first;
			}
		}
		else {
			if (headLoc.first == mapSize - 1) {
				headLoc.first = 0;
			}
			else {
				headLoc.first = headLoc.first + currentDir.first;
			}
		}
	}
	else {
		if (currentDir.second < 0) {
			if (headLoc.second == 0) {
				headLoc.second = mapSize - 1;
			}
			else {
				headLoc.second = headLoc.second + currentDir.second;
			}
		}
		else {
			if (headLoc.second == mapSize - 1) {
				headLoc.second = 0;
			}
			else {
				headLoc.second = headLoc.second + currentDir.second;
			}
		}
	}
	return headLoc;
}




void deleteSameCharacterNodes(std::vector<NodeInfo>& allNodes)
{
	// Bu fonksiyonu denemedim hiç :D
	std::vector<NodeInfo>::iterator it1;
	std::vector<NodeInfo>::iterator it2;
	for (it1 = allNodes.begin(); it1 != allNodes.end();) {
		for (it2 = it1 + 1; it2 != allNodes.end();) {
			if (it1->headLoc == it2->headLoc && it1->currentDir == it2->currentDir) {
				it2 = allNodes.erase(it2);
			}
			else {
				++it2;
			}
		}
		++it1;

	}
	int size = (int)allNodes.size();
}


