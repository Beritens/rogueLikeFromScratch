#include "MovementSystem.h"
#include "../Core/Coordinator.hpp"
#include "../Components/Transform.h"
#include <random>

extern Coordinator gCoordinator;

void MovementSystem::Init() {

}
void MovementSystem::Update(float dt, std::bitset<8> mButtons) {

	/*std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(-1, 1);*/
	float speed = dt * 500;


	for (auto const& entity : mEntities)
	{

		auto& transform = gCoordinator.GetComponent<Transform>(entity);
		/*transform.x += dist(rng);
		transform.y += dist(rng);*/
		if (mButtons.test(static_cast<std::size_t>(InputButtons::W))) {
			transform.pos.y -= speed;
		}
		if (mButtons.test(static_cast<std::size_t>(InputButtons::A))) {
			transform.pos.x-= speed;
		}
		if (mButtons.test(static_cast<std::size_t>(InputButtons::S))) {
			transform.pos.y+= speed;
		}
		if (mButtons.test(static_cast<std::size_t>(InputButtons::D))) {
			transform.pos.x+= speed;
		}
	}

}