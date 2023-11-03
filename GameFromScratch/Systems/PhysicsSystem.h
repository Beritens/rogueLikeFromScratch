#pragma once
#include "../Core/System.hpp"
#include "../Core/Types.hpp"

class Event;

class PhysicsSystem : public System
{
public:
	void Init();
	void Update(float dt, std::set<Entity> colliders);
	void InputListener(Event& event);
private:
	std::bitset<8> mButtons;
};
