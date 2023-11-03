#ifndef Game_h
#define Game_h
#include "SDL.h"
#include <stdio.h>


class Game {
public:
	Game();
	~Game();

	void init();

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	
private:
	int cnt = 0;
	bool isRunning;
	SDL_Window* window;
};

#endif /* Game_hpp*/
