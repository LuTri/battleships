#ifndef _GAME_HPP
#define _GAME_HPP

#include "screen.hpp"
#include "network.hpp"
#include "keyconf.hpp"
#include "shipset.hpp"

#define KEY_CONF 0x00
#define START_SERVER 0x01
#define START_CLIENT 0x02
#define EXIT_GAME 0x03

class Game {
private:
   MainScreen* _main_screen;
   Network* _network;
   ShipSet* _ship_set;

   static Game INSTANCE;
   Game(void);

public:
   static Game& GetGame(void);

   int Menu(Coord boundaries);

   bool RunServer(void);
   bool RunClient(void);

   void Run(void);

   void Play(void);
   ~Game();
};

#endif
