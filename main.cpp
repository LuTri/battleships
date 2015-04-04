#include <iostream>
#include <cstdlib>
#include "game.hpp"

using namespace std;

int main(void) {
   Game& the_game = Game::GetGame();

   the_game.Run();

   return 0;
}
