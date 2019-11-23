#define OLC_PGE_APPLICATION

#include "Game.h"
#include <Windows.h>

int main() {
	FreeConsole();

	Game game;

	if (game.Construct(768, 768, 1, 1)) {
		game.Start();
	}

	return 0;
}