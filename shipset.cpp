#include "shipset.hpp"
#include "keyconf.hpp"

ShipSet::ShipSet(Table<char>* display_table) {
   done_filling = false;
   for (int i = 0; i < 10; i++) {
      _set[i] = NULL;
   }

   for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
         table[i][j] = 0;
      }
   }

   _display_table = display_table;
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

   Table<bool> table;

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

void ShipSet::FillTable(Table<char>* target_table) {
   if (! done_filling || target_table != 0) {

      for (int i = 0; i < 10; i++) {
         if (_set[i]) {
            int size;
            Coord *pos = _set[i]->GetPoints(&size);

            for (int j = 0; j < size; j++) {
               if (target_table == 0)
                  table[pos[j].y][pos[j].x] = _set[i];
               else
                  (*target_table)[pos[j].y][pos[j].x] = SHIP_SYMBOL;
            }

            delete[] pos;
         }
      }

      if (target_table == 0)
         done_filling = true;
   }
}

char ShipSet::Hit(Coord pos) {
   FillTable();
   Ship* hit_ship = table[pos.y][pos.x];

   if (hit_ship) {
      (*_display_table)[pos.y][pos.x] = STATE_HIT;
      if (hit_ship->HitDestruct()) {
         return STATE_DESTRUCTION;
      }
      return STATE_HIT;
   }

   (*_display_table)[pos.y][pos.x] = STATE_MISS;
   return STATE_MISS;
}