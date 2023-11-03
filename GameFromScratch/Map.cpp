#include "Map.h"
#include "TextureManager.h"
#include "Core/Types.hpp"
#include "Core/Coordinator.hpp"
#include "Components/Transform.h"
#include "Components/Renderable.h"
#include "Components/BoxCollider.h"

extern Coordinator gCoordinator;

int lvl1[20][25] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,2,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

Map::Map() {
	dirt = TextureManager::LoadTexture("Assets/dirt.png");
	grass = TextureManager::LoadTexture("Assets/grass.png");
	water = TextureManager::LoadTexture("Assets/water.png");

	LoadMap(lvl1);

	src.h = src.w =  32;
	src.x = src.y = 0;
	dest.h = dest.w = 32;
}

void Map::LoadMap(int arr[20][25]) {
	Entity entities[20*25];
	for (int row = 0; row < 20; row++) {
		for (int column = 0; column < 25; column++) {
			entities[row*20+column] = gCoordinator.CreateEntity();

			gCoordinator.AddComponent<Transform>(
				entities[row * 20 + column],
				{ static_cast<float>(column*32),static_cast<float>(row*32),0,1 });
			SDL_Texture* tex;
			switch (arr[row][column])
			{
			case 0:
				tex = water;
				break;
			case 1:
				tex = grass;
				gCoordinator.AddComponent<BoxCollider>(entities[row * 20 + column],
					{ 16,16 });
				break;
			case 2:
				tex = dirt;
				gCoordinator.AddComponent<BoxCollider>(entities[row * 20 + column],
					{ 16,16 });
				break;
			default:
				tex = dirt;
				gCoordinator.AddComponent<BoxCollider>(entities[row * 20 + column],
					{ 16,16 });
				break;
			}
			gCoordinator.AddComponent<Renderable>(
				entities[row * 20 + column],
				{ tex });

			map[row][column] = arr[row][column];
		}
	}
}
