#include "Game.h"


int main(int argc, char** argv)
{
	Game game;

	game.Init();
	game.Run();
	game.Free();

	return 0;
}