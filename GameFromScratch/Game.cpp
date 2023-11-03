//#include "Game.h"
//#include <iostream>
//#include "TextureManager.h"
//#include "GameObject.h"
//#include "Map.h"
//#include "Core/Coordinator.hpp"
//#include "Components/Player.h"
//#include "Components/Transform.h"
//#include "Components/Renderable.h"
//#include "Systems/RenderSystem.h"
//
//#include <random>
//#include "Systems/PlayerControlSystem.h"
//
//
//
//Map* map;
////https://austinmorlan.com/posts/entity_component_system/
//Coordinator gCoordinator;
//
////Systems
//std::shared_ptr<RenderSystem> renderSystem;
//std::shared_ptr<PlayerControlSystem> playerControlSystem;
//
//
//
//Game::Game() {
//
//}
//Game::~Game() {
//
//}
//
//void Game::init() {
//	gCoordinator.Init();
//	
//
//	
//	
//	gCoordinator.RegisterComponent<Player>();
//	gCoordinator.RegisterComponent<Renderable>();
//	gCoordinator.RegisterComponent<Transform>();
//
//
//	//initiate Systems
//
//	renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
//	{
//		Signature signature;
//		signature.set(gCoordinator.GetComponentType<Renderable>());
//		signature.set(gCoordinator.GetComponentType<Transform>());
//		gCoordinator.SetSystemSignature<RenderSystem>(signature);
//	}
//	renderSystem->Init();
//
//	playerControlSystem = gCoordinator.RegisterSystem<PlayerControlSystem>();
//	{
//		Signature signature;
//		signature.set(gCoordinator.GetComponentType<Player>());
//		signature.set(gCoordinator.GetComponentType<Transform>());
//		gCoordinator.SetSystemSignature<PlayerControlSystem>(signature);
//	}
//	playerControlSystem->Init();
//
//	//Add Entities
//
//
//	map = new Map();
//
//	Entity entities[100];
//	for (int i = 0; i < 100; i++) {
//		entities[i] = gCoordinator.CreateEntity();
//		gCoordinator.AddComponent(entities[i], Player{});
//
//		gCoordinator.AddComponent<Transform>(
//			entities[i],
//			{ 400,300,0,2 });
//		SDL_Texture* playerTexture = TextureManager::LoadTexture("Assets/redBall.png");
//		gCoordinator.AddComponent<Renderable>(
//			entities[i],
//			{ playerTexture });
//
//	}
//		
//	
//
//}
//
//void Game::handleEvents() {
//	
//}
//
//void Game::update() {
//	playerControlSystem->Update();
//}
//
//void Game::render() {
//	SDL_RenderClear(renderer);
//	renderSystem->Update();
//	SDL_RenderPresent(renderer);
//
//}
//
