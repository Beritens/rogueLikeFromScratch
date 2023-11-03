#include "RenderSystem.h"
#include "../Core/Coordinator.hpp"
#include "../Components/Renderable.h"
#include "../Components/Transform.h"
#include "../TextureManager.h"



extern Coordinator gCoordinator;

void RenderSystem::Init() {
	

}

void RenderSystem::Update() {
	SDL_Rect src;
	src.x = src.y = 0;
	src.w = src.h = 32;
	SDL_Rect dest;
	dest.w = dest.h = 32;
	for (auto const& entity : mEntities)
	{
		
		auto const& transform = gCoordinator.GetComponent<Transform>(entity);
		auto const& renderable = gCoordinator.GetComponent<Renderable>(entity);
		dest.x = transform.pos.x;
		dest.y = transform.pos.y;
		dest.h = 32 * transform.scale;
		dest.w = 32 * transform.scale;
		TextureManager::Draw(renderable.sprite, &src, &dest);
	}
}

