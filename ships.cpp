#include "ships.hpp"
#include "keyconf.hpp"

int Battleship::_count = 0;
int Cruiser::_count = 0;
int Destroyer::_count = 0;
int Submarine::_count = 0;

Ship::Ship(void) { _azimut = 1; }

Battleship::Battleship(void):Ship() { _set_size(); }

void Battleship::_set_size(void) { _size = 5; }

Cruiser::Cruiser(void) { _set_size(); }

void Cruiser::_set_size(void) { _size = 4; }

Destroyer::Destroyer(void) { _set_size(); }

void Destroyer::_set_size(void) { _size = 3; }

Submarine::Submarine(void) { _set_size(); }

void Submarine::_set_size(void) { _size = 2; }

const Coord& Ship::GetPosition(void) {
   return position;
}

void Ship::Move(int x, int y) {
   position = Coord(x,y);
}

void Ship::Move(Coord pos) {
   position = pos;
}

void Ship::Move(char dir) {
   int x = position.x;
   int y = position.y;
   /* size-less positioning */
   x = (10 + (x + 1 * (dir == Keyconf::KEYCONF[BTN_RIGHT]) + -1 * (dir == Keyconf::KEYCONF[BTN_LEFT]))) % 10;
   y = (10 + (y + 1 * (dir == Keyconf::KEYCONF[BTN_DOWN]) + -1 * (dir == Keyconf::KEYCONF[BTN_UP]))) % 10;

   /* considering size */
   if (_azimut == 0 && x > 10 - _size)
      x = 0 + (10 * (dir == Keyconf::KEYCONF[BTN_LEFT]) - _size * (dir == Keyconf::KEYCONF[BTN_LEFT]));
   if (_azimut == 1 && y > 10 - _size)
      y = 0 + (10 * (dir == Keyconf::KEYCONF[BTN_UP]) - _size * (dir == Keyconf::KEYCONF[BTN_UP]));

   position.x = x;
   position.y = y;
}

void Ship::SetAzimut(int azi) {
   _azimut = azi;
}

Coord* Ship::GetPoints(int* size) {
   Coord* points = new Coord[_size];
   *size = _size;

   for (int idx = 0; idx < _size; idx++) {
      points[idx].x = position.x + idx * (_azimut == 0);
      points[idx].y = position.y + idx * (_azimut == 1); 
   }
   
   return points;
}

void Ship::Draw(BattleScreen& screen, char symbol) {
   Coord* positions;
   int size;

   positions = GetPoints(&size);

   for (int i = 0; i < size; i++) {
      screen.PutC(symbol,
         positions[i].x,
         positions[i].y);
   }

   delete[] positions;
   screen.Refresh();
}

void Ship::Erase(BattleScreen& screen) {
   Draw(screen, ' ');
}

void Ship::Rotate(void) {
   _azimut = ++_azimut % 2;
   if (_azimut == 0 && position.x > 10 - _size)
      position.x = _size;
   if (_azimut == 1 && position.y > 10 - _size)
      position.y = _size;
}
