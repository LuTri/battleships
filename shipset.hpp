#ifndef _SHIPSET_HPP
#define _SHIPSET_HPP

#include "ships.hpp"
#include "table.hpp"

#define STATE_HIT 1
#define STATE_MISS 2
#define STATE_DESTRUCTION 3

class ShipSet {
private:
   Ship *_set[10];

   Table<Ship*> table;
   bool done_filling;

   Table<char>* _display_table;

public:
   void Positioning(MainScreen& mainscreen);

   Coord GetNextValid(Ship& ship, bool* success, char direction);

   bool Collision(void);
   bool Collision(Coord point);

   ShipSet(Table<char>* display_table);
   ~ShipSet(void);

   void FillTable(Table<char>* target_table = 0);

   char Hit(Coord pos);
};

#endif
