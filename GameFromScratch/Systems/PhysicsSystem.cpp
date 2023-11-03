#include "PhysicsSystem.h"
#include "../Core/Coordinator.hpp"
#include "../Components/Renderable.h"
#include "../Components/Transform.h"
#include "../TextureManager.h"
#include "../Components/PhysicsObject.h"
#include <climits>
#include <cstdlib>
#include <iostream>
#include "../Components/BoxCollider.h"



extern Coordinator gCoordinator;

void PhysicsSystem::Init() {


}

void PhysicsSystem::Update(float dt, std::set<Entity> colliders) {
	for (auto const& entity : mEntities)
	{
		auto& transform = gCoordinator.GetComponent<Transform>(entity);
		auto& phyObj = gCoordinator.GetComponent<PhysicsObject>(entity);
		transform.pos.x += phyObj.vel.x*dt;
		transform.pos.y += phyObj.vel.y*dt;
		
		if (transform.pos.y > 640 ) {
			transform.pos.y = 640;
			phyObj.vel.y = -std::abs(phyObj.vel.y) * 1.025;
		}
		else if (transform.pos.y < 0) {
			transform.pos.y = 0;
			phyObj.vel.y = std::abs(phyObj.vel.y) * 1.025;
		}
		if (transform.pos.x > 800 ) {
			transform.pos.x = 800;
			phyObj.vel.x = -std::abs(phyObj.vel.x) * 1.025;
		}
		else if (transform.pos.x < 0) {
			transform.pos.x  = 0;
			phyObj.vel.x = std::abs(phyObj.vel.x) * 1.025;
		}

		for (auto const& collider : colliders) {
			auto& colTransform = gCoordinator.GetComponent<Transform>(collider);
			auto& colBoxCollider = gCoordinator.GetComponent<BoxCollider>(collider);

			
			//bald AABB, aber jetzt erstmal dashier
			float xOff = transform.pos.x - colTransform.pos.x;
			float yOff = transform.pos.y - colTransform.pos.y;
			if (abs(xOff) <= colBoxCollider.bounds.x && abs(yOff) <= colBoxCollider.bounds.y) {
				//collided
				
				Vector2 normal = { 0,0 };
				if (abs(xOff) < abs(yOff)) {
					normal = { 0,signbit(yOff) ? 1.0f : -1.0f };
					phyObj.vel.y *= -1;
					
				}
				else{
					normal = {signbit(xOff) ? 1.0f : -1.0f, 0 };
					phyObj.vel.x *= -1;
				}

				//do stuff with normal
			}
		}

		phyObj.vel.y += 1000 * dt;
	}
}

