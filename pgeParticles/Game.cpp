#include "Game.h"
#include <random>

Game::Game()
{
}

Game::~Game()
{
}

bool Game::OnUserCreate() {

	// create an RNG
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_real_distribution<float> directions(0, 1); // guaranteed unbiased
	std::uniform_real_distribution<float> positionsY(0, ScreenHeight());
	std::uniform_real_distribution<float> positionsX(0, ScreenWidth());
	std::uniform_real_distribution<float> speeds(9, 36);

	for (int i = 0; i < 350; i++) {
		Particle particle;

		float random_x = positionsX(rng);
		float random_y = positionsY(rng);
		float speed = speeds(rng);
		float direction_x = directions(rng);
		float direction_y = directions(rng);

		particle.x = random_x;
		particle.y = random_y;
		particle.directionX = (direction_x * 2) - 1;
		particle.directionY = (direction_y * 2) - 1;
		particle.originalDirectionX = direction_x;
		particle.originalDirectionY = direction_y;
		particle.speed = 12.5f;
		particle.colour.r = rand() % 255;
		particle.colour.g = rand() % 255;
		particle.colour.b = rand() % 255;
		particle.colour.a = 255;

		particles.push_back(particle);
	}

	intersectThreshold = 75.0f;

	mouseDrawDistance = 105.0f;
	particleDrawDistance = 37.5f;

	return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
	Clear(olc::Pixel(75, 75, 75, 255));

	for (auto &particle : particles) {
		// we need to 'push away' particles, which we can do by changing the direction
		// decide whether to draw the line or not
		float mouseX = GetMouseX();
		float mouseY = GetMouseY();

		// how far is the pixel from the mouse?
		float diffX, diffY;

		diffX = particle.x - mouseX;
		diffY = particle.y - mouseY;

		float distance = sqrt((diffX * diffX) + (diffY * diffY));

		// handle 'boosting'
		if (particle.boosted) {
			particle.boostTime -= fElapsedTime;

			if (particle.boostTime <= 0.0f) {
				particle.boosted = false;
				particle.boostTime = 0.3f;

				particle.directionX = particle.originalDirectionX;
				particle.directionY = particle.originalDirectionY;
			}
		}

		if (GetMouse(0).bPressed) {
			// figure out what the direction should be...

			if (distance <= mouseDrawDistance) {
				float length = sqrt((diffX * diffX) + (diffY * diffY));
				float newDirectionX = (diffX / length) * (4.0f * (mouseDrawDistance / distance));
				float newDirectionY = (diffY / length) * (4.0f * (mouseDrawDistance / distance));

				/*particle.originalDirectionX = particle.directionX;
				particle.originalDirectionY = particle.directionY;*/

				particle.directionX = newDirectionX;
				particle.directionY = newDirectionY;
				particle.boosted = true;
			}
		}

		float nextX = particle.x + ((particle.speed * fElapsedTime) * particle.directionX);
		float nextY = particle.y + ((particle.speed * fElapsedTime) * particle.directionY);

		if (nextX <= 0) {
			nextX = 0;
			particle.directionX = -particle.directionX;
			if (particle.boosted) {
				particle.directionX = -particle.originalDirectionX;
			}
		} else if (nextX >= ScreenWidth()) {
			nextX = ScreenWidth();
			particle.directionX = -particle.directionX;
			if (particle.boosted) {
				particle.directionX = -particle.originalDirectionX;
			}
		}

		if (nextY <= 0) {
			nextY = 0;
			particle.directionY = -particle.directionY;
			if (particle.boosted) {
				particle.directionX = -particle.originalDirectionY;
			}
		} else if (nextY >= ScreenHeight()) {
			nextY = ScreenHeight();
			particle.directionY = -particle.directionY;
			if (particle.boosted) {
				particle.directionX = -particle.originalDirectionY;
			}
		}

		particle.x = nextX;
		particle.y = nextY;

		FillCircle(particle.x, particle.y, 2, olc::Pixel(particle.colour.r, particle.colour.g, particle.colour.b, particle.colour.a));

		// Find the distance with sqrt((x1-x2)^2+(y1-y2)^2)


		

		if (distance <= mouseDrawDistance) {
			// use distance to determine the colour of the pixel
			Colour mouseLine;

			mouseLine.r = particle.colour.r;
			mouseLine.g = particle.colour.g;
			mouseLine.b = particle.colour.b;


			float rawAlpha = 255 - (distance * (255 / mouseDrawDistance));

			mouseLine.a = ceil(rawAlpha);

			// draw line
			SetPixelMode(olc::Pixel::ALPHA);
			DrawLine(particle.x, particle.y, mouseX, mouseY, olc::Pixel(mouseLine.r, mouseLine.g, mouseLine.b, mouseLine.a));
			SetPixelMode(olc::Pixel::NORMAL);
		}

		for (auto otherParticle : particles) {
			// how far is the otherParticle from the particle
			float pDiffX = particle.x - otherParticle.x;
			float pDiffY = particle.y - otherParticle.y;

			float pDistance = sqrt((pDiffX * pDiffX) + (pDiffY * pDiffY));

			if (pDistance <= particleDrawDistance) {
				// use distance to determine the colour of the pixel
				Colour particleLine;

				particleLine.r = 255;
				particleLine.g = 255;
				particleLine.b = 255;


				float rawAlpha = 255 - (pDistance * (255 / particleDrawDistance));

				particleLine.a = ceil(rawAlpha);

				SetPixelMode(olc::Pixel::ALPHA);
				DrawLine(particle.x, particle.y, otherParticle.x, otherParticle.y, olc::Pixel(particleLine.r, particleLine.g, particleLine.b, particleLine.a));
				SetPixelMode(olc::Pixel::NORMAL);
			}
		}
	}

	return true;
}