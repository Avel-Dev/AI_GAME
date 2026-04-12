#include "Game.hpp"

#include <raylib.h>

int main() {
	Game game;

	game.Init();
	game.Start();
	game.End();
	return 0;
}
