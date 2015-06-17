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

#define TARGET_HIT 'X'
#define TARGET_MISS '.'

class Game {
private:

   bool _is_server;

   Table<char> _my_table;
   Table<char> _enemy_table;

   MainScreen* _main_screen;
   Network* _network;
   ShipSet* _ship_set;

   static Game INSTANCE;
   Game(void);

   int Menu(Coord boundaries);

   bool RunServer(void);
   bool RunClient(void);

   void Play(void);

   Coord HitLoop(Coord lastcursor);
   void WaitLoop(void);

   char QueryHit(Coord pos);

   void RenderState(void);

   void InitNew(void);

public:
   static Game& GetGame(void);

   void Run(void);

   ~Game();
};

#endif
