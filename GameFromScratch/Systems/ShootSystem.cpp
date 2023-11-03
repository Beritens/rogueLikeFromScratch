#include "ShootSystem.h"
#include "../Core/Coordinator.hpp"
#include "../Components/Transform.h"
#include <random>
#include "../Components/Renderable.h"
#include "../TextureManager.h"
#include "../Components/PhysicsObject.h"
#include "../Components/Player.h"

extern Coordinator gCoordinator;

void ShootSystem::Init() {
	bulletSprite = TextureManager::LoadTexture("Assets/dirt.png");
}

void ShootSystem::Update(float dt, std::bitset<8> mButtons) {

	/*std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(-1, 1);*/


	for (auto const& entity : mEntities)
	{

		auto& transform = gCoordinator.GetComponent<Transform>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		player.cooldown -= dt;
		/*transform.x += dist(rng);
		transform.y += dist(rng);*/
		if (mButtons.test(static_cast<std::size_t>(InputButtons::E)) && player.cooldown<0) {
			Entity ent = gCoordinator.CreateEntity();
			gCoordinator.AddComponent<Renderable>(ent, { bulletSprite });
			gCoordinator.AddComponent<Transform>(ent, { { transform.pos.x , transform.pos.y}, 0,0.5 });
			gCoordinator.AddComponent<PhysicsObject>(ent, {300,-200});
			player.cooldown = 0.025f;
		}
	}

}