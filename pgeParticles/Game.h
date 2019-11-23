#pragma once

#include "olcPixelGameEngine.h"
#include <vector>
#include "Particle.h"

class Game : public olc::PixelGameEngine
{
public:
	Game();
	~Game();

protected:
	std::vector<Particle> particles;
	float speed, mouseDrawDistance, particleDrawDistance;
	int directions[2];
	float intersectThreshold;

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
};

