#pragma once
#include "../Core/System.hpp"
#include "../Core/Types.hpp"


class MovementSystem : public System
{
public:
	void Init();
	void Update(float dt, std::bitset<8> mButtons);

};
