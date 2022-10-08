#include "Game.h"


Game* game = nullptr;

int main(int argc, char* argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	int width,height,gridSize;
	std::string mode;
	std::cout << "Enter the Width and Height of the Game As Pixels : " << std::endl;
	std::cin >> width >> height;
	std::cout << "Enter Grid Size : " << std::endl;
	std::cin >> gridSize;
	std::cout << "Enter Game Mod : ('a' for auto & 'm' for manual))" << std::endl;
	std::cin >> mode;

	game = new Game();
	game->init("first_game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, gridSize, mode, false);
	game->gameMod = mode;

	while (game->running()) {

		frameStart = SDL_GetTicks();
		game->eventsGet = false;
		game->handleEvents();
		if (game->playing) {
			game->update();
		}

		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();


	return 0;
}