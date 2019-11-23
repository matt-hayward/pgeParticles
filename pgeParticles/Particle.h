#pragma once

#include "Colour.h"

class Particle
{
public:
	Particle();
	~Particle();

public:
	float x, y, speed;
	float directionX, directionY;
	float originalDirectionX, originalDirectionY;
	bool boosted = false;
	float boostTime = 0.5;
	Colour colour;
};

