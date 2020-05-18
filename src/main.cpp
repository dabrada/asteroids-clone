#include "game.h"
#include <cstdlib>

int main()
{
	srand(time(nullptr));
    Game game;
    game.run();
    return 0;
}
