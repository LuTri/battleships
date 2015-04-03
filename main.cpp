#include <iostream>
#include <cstdlib>
#include "screen.hpp"
#include "shipset.hpp"
#include "keyconf.hpp"

using namespace std;

int main(void) {

   char cInput;

   MainScreen MyMainScreen = MainScreen::GetInstance();

   if(!Keyconf::KEYCONF.load()) {
      Keyconf::KEYCONF.config(MyMainScreen.GetStatusScreen());
   }

   ShipSet myset = ShipSet();
   ShipSet enemyset = ShipSet();

   myset.Positioning(MyMainScreen);

   cInput = MyMainScreen.handle_input();
   return 0;
}
