#include "positioning.hpp"

Coord::Coord(void) {
   x = 0; 
   y = 0;
}

Coord::Coord(int x, int y) {
   this->x = x;
   this->y = y;
}

Coord::Coord(const Coord& org) {
   this->x = org.x;
   this->y = org.y;
}