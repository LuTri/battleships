#include <iostream>
#include <cstdlib>
#include "screen.hpp"
#include "shipset.hpp"

using namespace std;

void PositioningLoop(Screen* screen, Ship *ships[10], MainScreen& debug) {
}

int main(void) {

   char cInput;
   MainScreen MyMainScreen = MainScreen::GetInstance();

   ShipSet myset = ShipSet();
   ShipSet enemyset = ShipSet();

   myset.Positioning(MyMainScreen);

   cInput = MyMainScreen.handle_input();
   return 0;
}
