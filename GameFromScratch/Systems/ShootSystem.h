#pragma once
#include "../Core/System.hpp"
#include "../Core/Event.hpp"
#include "SDL_image.h"


class ShootSystem : public System
{
public:
	void Init();
	void Update(float dt, std::bitset<8> mButtons);
private:
	SDL_Texture* bulletSprite;
	
};