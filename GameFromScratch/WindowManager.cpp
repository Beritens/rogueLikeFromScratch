#include "WindowManager.h"
#include <iostream>
#include "Core/Types.hpp"
#include "Core/Coordinator.hpp"

extern Coordinator gCoordinator;
SDL_Renderer* WindowManager::renderer = nullptr;

void WindowManager::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystem Initialised!..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
	}
	else {
		std::cout << "not runnig" << std::endl;
	}
}
void WindowManager::Clear() {
	SDL_RenderClear(renderer);
}
void WindowManager::Update() {
	SDL_RenderPresent(renderer);
}

void WindowManager::Shutdown() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}

void WindowManager::ProcessEvents()
{
	SDL_Event event;
	bool updateKey = false;
	while (SDL_PollEvent(&event)) { // This will loop until all events in the queue have been processed
		Event _event(Events::Window::INPUT);
		switch (event.type) {
		case SDL_QUIT:
			gCoordinator.SendEvent(Events::Window::QUIT);
			break;
		case SDL_KEYDOWN:
			updateKey = false;
			switch (event.key.keysym.sym) {
			case SDLK_w:
				mButtons.set(static_cast<std::size_t>(InputButtons::W), true);
				updateKey = true;
				break;
			case SDLK_a:
				mButtons.set(static_cast<std::size_t>(InputButtons::A), true);
				updateKey = true;
				break;
			case SDLK_s:
				mButtons.set(static_cast<std::size_t>(InputButtons::S), true);
				updateKey = true;
				break;
			case SDLK_d:
				mButtons.set(static_cast<std::size_t>(InputButtons::D), true);
				updateKey = true;
				break;
			case SDLK_e:
				mButtons.set(static_cast<std::size_t>(InputButtons::E), true);
				updateKey = true;
				break;
			default:
				break;
			}
			if (updateKey) {
				_event.SetParam(Events::Window::Input::INPUT, mButtons);
				gCoordinator.SendEvent(_event);
			}
			
			break;
		case SDL_KEYUP:
			updateKey = false;
			switch (event.key.keysym.sym) {
			case SDLK_w:
				mButtons.set(static_cast<std::size_t>(InputButtons::W), false);
				updateKey = true;
				break;
			case SDLK_a:
				mButtons.set(static_cast<std::size_t>(InputButtons::A), false);
				updateKey = true;
				break;
			case SDLK_s:
				mButtons.set(static_cast<std::size_t>(InputButtons::S), false);
				updateKey = true;
				break;
			case SDLK_d:
				mButtons.set(static_cast<std::size_t>(InputButtons::D), false);
				updateKey = true;
				break;
			case SDLK_e:
				mButtons.set(static_cast<std::size_t>(InputButtons::E), false);
				updateKey = true;
				break;
			default:
				break;
			}
			if (updateKey) {
				_event.SetParam(Events::Window::Input::INPUT, mButtons);
				gCoordinator.SendEvent(_event);
			}

			break;

		default:
			break;
		}
	}
}