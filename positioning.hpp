#ifndef _POSITIONING_HPP
#define _POSITIONING_HPP

class Coord {
public:
   Coord(void);
   Coord(int x, int y);
   Coord(const Coord& org);
   int x;
   int y;
};

#endif
