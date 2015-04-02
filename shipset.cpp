#include "shipset.hpp"

ShipSet::ShipSet(void) {
   for (int i = 0; i < 10; i++) {
      _set[i] = NULL;
      if (i < MAX_BATTLE)
         _set[i] = new Battleship();
      else if (i < MAX_BATTLE + MAX_CRUISER)
         _set[i] = new Cruiser();
      else if (i < MAX_BATTLE + MAX_CRUISER + MAX_DESTROYER)
         _set[i] = new Destroyer();
      else
         _set[i] = new Submarine();
   }
}

ShipSet::~ShipSet(void) {
   for (int i = 0; i < 10; i++) {
      delete _set[i];
   }
}

Coord ShipSet::GetNextValid(Ship& ship) {
   return Coord(0,0);
}

void ShipSet::Positioning(MainScreen& mainscreen) {
   int idx;
   char input = 0;
   for (idx = 0; idx < 10; idx++) {
      Screen& status = mainscreen.GetStatusScreen();
      input = 0;
      _set[idx]->Draw(mainscreen.GetMyTable());

      status.Clear();

      status << "Das " << idx + 1 << "te Schiff setzen: ";
      if (idx < MAX_BATTLE) {
         _set[idx] = new Battleship();
        status << "Das " << (idx) + 1 << "te Schlachtschiff.";
      } else if (idx < MAX_BATTLE + MAX_CRUISER) {
         _set[idx] = new Cruiser();
        status << "Der " << (idx - MAX_BATTLE)+ 1 << "te Kreuzer.";
      } else if (idx < MAX_BATTLE + MAX_CRUISER + MAX_DESTROYER) {
         _set[idx] = new Destroyer();
         status << "Der " << (idx - (MAX_BATTLE + MAX_CRUISER)) + 1 << "te Zerstoerer.";
      } else {
         _set[idx] = new Submarine();
         status << "Das " << (idx - (MAX_BATTLE + MAX_CRUISER + MAX_DESTROYER)) + 1 << "te UBoot.";
      }

      status.Refresh();

      while (input != 13) {
         input = mainscreen.GetMyTable().handle_input();

         switch (input) {
            case 'r':
               _set[idx]->Erase(mainscreen.GetMyTable());
               _set[idx]->Rotate();
               _set[idx]->Move(GetNextValid(*_set[idx]));
               _set[idx]->Draw(mainscreen.GetMyTable());
               break;
            case BTN_DOWN:
            case BTN_UP:
            case BTN_LEFT:
            case BTN_RIGHT:
               _set[idx]->Erase(mainscreen.GetMyTable());
               _set[idx]->Move(input);
               _set[idx]->Move(GetNextValid(*_set[idx]));
               _set[idx]->Draw(mainscreen.GetMyTable());
               break;
            default:
               break;
         }
      }
   }
}