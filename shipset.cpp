#include "shipset.hpp"
#include "keyconf.hpp"

ShipSet::ShipSet(void) {
   for (int i = 0; i < 10; i++) {
      _set[i] = NULL;
   }
}

ShipSet::~ShipSet(void) {
   for (int i = 0; i < 10; i++) {
      delete _set[i];
   }
}

bool ShipSet::Collision(void) {
   Ship* act_ship;
   int idx = 0;

   bool collision = false;

   bool table[10][10] = { false };
   while ((act_ship = _set[idx]) != NULL && idx < 10 && !collision) {
      int size;
      Coord* points;

      points = act_ship->GetPoints(&size);

      for (int i = 0; i < size; i++) {
         if (!table[points[i].y][points[i].x]) {
            table[points[i].y][points[i].x] = true;
         } else {
            collision = true;
         }
      }

      delete[] points;
      idx++;
   }
   return collision;
}

Coord ShipSet::GetNextValid(Ship& ship, bool* success, char direction) {
   int cnt = 0;

   *success = true;

   if (Collision()) {
      Coord old_pos = ship.GetPosition();

      while(Collision()) {
         if (cnt % 10 == 0 && cnt != 0) {
            char newdir;
            if (direction == Keyconf::KEYCONF[BTN_UP])
               newdir = Keyconf::KEYCONF[BTN_RIGHT];
            else if (direction == Keyconf::KEYCONF[BTN_RIGHT])
               newdir = Keyconf::KEYCONF[BTN_DOWN];
            else if (direction == Keyconf::KEYCONF[BTN_DOWN])
               newdir = Keyconf::KEYCONF[BTN_LEFT];
            else
               newdir = Keyconf::KEYCONF[BTN_UP];
            ship.Move(newdir);
         } else {
            ship.Move(direction);
         }
         
         if (++cnt == 99) {
            *success = false;
            break;
         }
      }
   }
   return ship.GetPosition();
}
void ShipSet::Positioning(MainScreen& mainscreen) {
   int idx;
   int input = 0;
   bool success;
   for (idx = 0; idx < 10; idx++) {
      Screen& status = mainscreen.GetStatusScreen();
      input = 0;

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

      _set[idx]->Move(GetNextValid(*_set[idx], &success, Keyconf::KEYCONF[BTN_RIGHT]));
      _set[idx]->Draw(mainscreen.GetMyTable());


      while (input != Keyconf::KEYCONF[BTN_RETURN]) {
         input = mainscreen.GetMyTable().handle_input();

         if (input == Keyconf::KEYCONF[BTN_ROTATE]) {
            _set[idx]->Erase(mainscreen.GetMyTable());
            _set[idx]->Rotate();
            _set[idx]->Move(GetNextValid(*_set[idx], &success, input));
            _set[idx]->Draw(mainscreen.GetMyTable());
         } else if (input == Keyconf::KEYCONF[BTN_DOWN] || input == Keyconf::KEYCONF[BTN_UP] || input == Keyconf::KEYCONF[BTN_LEFT] || input == Keyconf::KEYCONF[BTN_RIGHT]) {
            _set[idx]->Erase(mainscreen.GetMyTable());
            _set[idx]->Move(input);
            _set[idx]->Move(GetNextValid(*_set[idx], &success, input));
            _set[idx]->Draw(mainscreen.GetMyTable());
         }
      }
   }
}
