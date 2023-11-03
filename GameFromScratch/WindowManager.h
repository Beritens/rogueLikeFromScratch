#pragma once
#include "SDL.h"
#include <bitset>

class WindowManager
{
public:
	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void Clear();

	void Update();

	void ProcessEvents();

	void Shutdown();

	static SDL_Renderer* renderer;

private:
	std::bitset<8> mButtons;
	SDL_Window* window;
};


