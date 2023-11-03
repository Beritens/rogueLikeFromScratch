#include "SDL.h"
#include "Game.h"
#include "Core/Coordinator.hpp"
#include "Components/Player.h"
#include "Components/Renderable.h"
#include "Components/Transform.h"
#include "Map.h"
#include "TextureManager.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "WindowManager.h"
#include <random>
#include "Systems/ShootSystem.h"
#include "Components/PhysicsObject.h"
#include "Systems/PhysicsSystem.h"
#include "Components/BoxCollider.h"
#include "Systems/ColliderTag1.h"
#include "Core/System.hpp"

//https://austinmorlan.com/posts/entity_component_system/
Coordinator gCoordinator;

static bool quit = false;
std::bitset<8> mButtons;

void QuitHandler(Event& event)
{
	quit = true;
}
void InputListener(Event& event)
{
	mButtons = event.GetParam<std::bitset<8>>(Events::Window::Input::INPUT);
}



int main(int argc, char* argv[]) {
	gCoordinator.Init();
	WindowManager windowManager;
	windowManager.Init("testEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

	gCoordinator.AddEventListener(FUNCTION_LISTENER(Events::Window::QUIT, QuitHandler));
	gCoordinator.AddEventListener(FUNCTION_LISTENER(Events::Window::INPUT, InputListener));

	// Query the current display mode of the primary monitor (0)
	/*SDL_DisplayMode current;
	if (SDL_GetCurrentDisplayMode(0, &current) != 0) {
		printf("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	const int FPS = current.refresh_rate;*/
	const int frameDelay = 1000 / 60;
	Uint32 frameStart;
	int frameTime;

	


	




	gCoordinator.RegisterComponent<Player>();
	gCoordinator.RegisterComponent<Renderable>();
	gCoordinator.RegisterComponent<Transform>();
	gCoordinator.RegisterComponent<PhysicsObject>();
	gCoordinator.RegisterComponent<BoxCollider>();


	//initiate Systems
	auto shootSystem = gCoordinator.RegisterSystem<ShootSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Player>());
		signature.set(gCoordinator.GetComponentType<Transform>());
		gCoordinator.SetSystemSignature<ShootSystem>(signature);
	}
	shootSystem->Init();
	

	auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Renderable>());
		signature.set(gCoordinator.GetComponentType<Transform>());
		gCoordinator.SetSystemSignature<RenderSystem>(signature);
	}
	renderSystem->Init();

	auto movementSystem = gCoordinator.RegisterSystem<MovementSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Player>());
		signature.set(gCoordinator.GetComponentType<Transform>());
		gCoordinator.SetSystemSignature<MovementSystem>(signature);
	}
	movementSystem->Init();

	auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<PhysicsObject>());
		signature.set(gCoordinator.GetComponentType<Transform>());
		gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
	}
	physicsSystem->Init();


	//tags, this is just done to have a list of entities with a component. Not sure if this is good or bad
	auto collider1 = gCoordinator.RegisterSystem<ColliderTag1>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<BoxCollider>());
		gCoordinator.SetSystemSignature<ColliderTag1>(signature);
	}

	//Add Entities


	Map* map = new Map();
	SDL_Texture* playerTexture = TextureManager::LoadTexture("Assets/redBall.png");

	

	Entity e = gCoordinator.CreateEntity();
	gCoordinator.AddComponent(e, Player{0.5f});

	gCoordinator.AddComponent<Transform>(
		e,
		{ {400,300},0,2 });
	gCoordinator.AddComponent<Renderable>(
		e,
		{ playerTexture });
		

	
	float dt = 0.006;
	
	while (!quit) {
		frameStart = SDL_GetTicks();

		/*game->handleEvents();
		game->update();
		game->render();*/
		windowManager.ProcessEvents();
		movementSystem->Update(dt, mButtons);
		shootSystem->Update(dt, mButtons);
		physicsSystem->Update(dt, collider1->mEntities);
		windowManager.Clear();
		renderSystem->Update();
		windowManager.Update();

		
		
		
		frameTime = SDL_GetTicks() - frameStart;
		printf("look at how bad this game runs: %d \n", frameTime);
		

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
		dt = static_cast<float>(SDL_GetTicks() - frameStart) / 1000.0;
	}
	windowManager.Shutdown();
	
	
	return 0;
}