#ifndef _SHIPSET_HPP
#define _SHIPSET_HPP

#include "ships.hpp"

class ShipSet {
private:
   Ship *_set[10];
public:
   void Positioning(MainScreen& mainscreen);

   Coord GetNextValid(Ship& ship, bool* success, char direction);

   bool Collision(void);
   bool Collision(Coord point);

   ShipSet(void);
   ~ShipSet(void);
};

#endif
