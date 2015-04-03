#ifndef _SHIPS_HPP
#define _SHIPS_HPP
#include "screen.hpp"
#include "positioning.hpp"

using namespace std;

#define MAX_BATTLE 1
#define MAX_CRUISER 2
#define MAX_DESTROYER 3
#define MAX_SUBMARINE 4

#define SHIP_SYMBOL '@'

class Ship {
protected:
   int _size;
   int _azimut;
   Coord position;

   virtual void _set_size() = 0;

public:
   void Draw(BattleScreen& screen, char symbol = SHIP_SYMBOL);
   void Erase(BattleScreen& screen);

   void Move(int x, int y);
   void Move(Coord pos);
   void Move(char dir);

   void SetAzimut(int azi);
   Coord* GetPoints(int* size);

   void Rotate(void);
   const Coord& GetPosition(void);

   Ship(void);
};

class Battleship: public Ship {
private:
   static int _count;

protected:
   void _set_size();

public:
   Battleship();
};

class Cruiser: public Ship {
private:
   static int _count;

protected:
   void _set_size();

public:
   Cruiser();
};

class Destroyer: public Ship {
private:
   static int _count;

protected:
   void _set_size();

public:
   Destroyer();
};

class Submarine: public Ship {
private:
   static int _count;

protected:
   void _set_size();

public:
   Submarine();
};

#endif
