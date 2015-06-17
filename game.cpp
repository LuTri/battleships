#include "game.hpp"

Game Game::INSTANCE;

Game::Game(void) {
   _main_screen = MainScreen::GetInstance();
   _network = 0;

   _ship_set = 0;

   for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
         _my_table[i][j] = 0;
         _enemy_table[i][j] = 0;
      }
   }
}

Game::~Game(void) {
   if (_ship_set) {
      delete _ship_set;
   }

   if (_network) {
      delete _network;
   }

   delete _main_screen;
}

Game& Game::GetGame(void) {
   return INSTANCE;
}

void Game::Run(void) {
   bool connection;
   int choice;
   Coord boundaries;
   if (! _main_screen->CheckSufficientSize()) {
      _main_screen->handle_input();
      return;
   }

   if (!Keyconf::KEYCONF.load()) {
      Keyconf::KEYCONF.config(*(_main_screen));
   }

   boundaries = _main_screen->GetMaxSize();
   do {
      connection = false;
      _main_screen->Clear();
      choice = Menu(boundaries);
      switch (choice) {
         case START_SERVER:
            connection = RunServer();
            break;
         case START_CLIENT:
            connection = RunClient();
            break;
         case KEY_CONF:
            Keyconf::KEYCONF.config(*(_main_screen));
            break;
      }

      if (connection) {
         Play();
         if (_network) {
            if (_is_server) {
               delete (Server*)_network;
               _network = 0;
            } else {
               delete (Client*)_network;
               _network = 0;
            }
         }
      }
   } while (choice != EXIT_GAME);
}

int Game::Menu(Coord boundaries) {
   char input;
   int cursor_pos = 0;
   MainScreen& display  = *_main_screen;
   int alignment;

   for (int i = 0; i < boundaries.x; i++) {
      display << "#";
   }
   display << "\n\n\n";
   display.Centered("Willkommen bei Battleships!");
   display << "\n\n";
   alignment = display.Centered("-----Hauptmenu---------------------------------------------");
   display << "\n";
   display << "\n";
   display.Aligned(" [ ] Tastenbelegung aendern",alignment);
   display << "\n";
   display.Aligned(" [ ] Spiel als Server starten", alignment);
   display << "\n";
   display.Aligned(" [ ] Spiel als Client starten", alignment);
   display << "\n";
   display.Aligned(" [ ] Programm beenden", alignment);
   display << "\n";

   display.Refresh();

   display.MoveCursor(Coord(alignment + 2,cursor_pos + 10),true);

   do {
      input = display.handle_input();
      
      if (input == Keyconf::KEYCONF[BTN_UP]) {
         cursor_pos--;
         if (cursor_pos == -1)
            cursor_pos = 3;
      } else if (input == Keyconf::KEYCONF[BTN_DOWN]) {
         cursor_pos = ++cursor_pos % 4;
      }

      display.MoveCursor(Coord(alignment + 2,cursor_pos + 10),true);
   } while (input != Keyconf::KEYCONF[BTN_RETURN]);

   return cursor_pos;
}

bool Game::RunServer(void) {
   bool connected;
   MainScreen& display = (*_main_screen);
   try {
      _is_server = true;
      _network = Server::GetServer();

      display.Clear();
      display << "Warte auf Client...";

      connected = ((Server*)_network)->AwaitConnection(8000);
   } catch (std::logic_error &e) {
      (*_main_screen) << e.what();
      _main_screen->handle_input();
   }

   if (!connected) {
      delete (Server*)_network;
      display << "Fehler beim Verbinden, Spiel konnte nicht gestartet werden!";
      display.handle_input();
      return false;
   }

   return true;
}

bool Game::RunClient(void) {
#ifdef DEBUG
   return true;
#endif
   int try_counter = 0;
   bool connected = false;
   string address;
   MainScreen& display = (*_main_screen);

   try {
      _is_server = false;
      _network = Client::GetClient();
      display.Clear();
      display << "Bitte die Addresse eingeben:\n";
      address = display.ReadString();
      while (!(connected = ((Client*)_network)->Connect(address,8000)) && try_counter < 10) {
         display << "Versuch " << try_counter + 1 << "...\n";
         try_counter++;
      }
   } catch (std::logic_error &e) {
      (*_main_screen) << e.what();
      _main_screen->handle_input();
   }

   if (!connected) {
      delete (Client*)_network;
      display << "Fehler beim Verbinden, Spiel konnte nicht gestartet werden!";
      display.handle_input();
      return false;
   }

   return true;
}

char Game::QueryHit(Coord pos) {
   return _ship_set->Hit(pos);
}

void Game::InitNew(void) {
   if (_ship_set) {
      delete _ship_set;
   }


   _ship_set = new ShipSet(&_my_table);
   _my_table.Clear();
   _enemy_table.Clear();
}

void Game::Play(void) {
   int hit = 0;
   Coord target(0,0);
   char state;

   InitNew();

   _main_screen->Clear();
   _main_screen->Refresh();
   _main_screen->InitGameMode();

   _ship_set->Positioning((*_main_screen));
   while (hit < 100) {
      target = HitLoop(target);
      if ((state = QueryHit(target)) == STATE_MISS) {
         _enemy_table[target.y][target.x] = STATE_MISS;
      } else {
         _enemy_table[target.y][target.x] = STATE_HIT;
      }
      hit++;
      RenderState();
      _main_screen->GetStatusScreen().Clear();
      _main_screen->GetStatusScreen() << hit;
      _main_screen->GetStatusScreen().Refresh();
   }
}

Coord Game::HitLoop(Coord lastcursor) {
   BattleScreen& display = _main_screen->GetEnemyTable();
   char input;

   display.MoveCursorRel(lastcursor);

   do {
      input = display.handle_input();
      lastcursor.x = (10 + (lastcursor.x +\
         1 * (input == Keyconf::KEYCONF[BTN_RIGHT]) +\
         -1 * (input == Keyconf::KEYCONF[BTN_LEFT])
         )) % 10;

      lastcursor.y = (10 + (lastcursor.y +\
         1 * (input == Keyconf::KEYCONF[BTN_DOWN]) +\
         -1 * (input == Keyconf::KEYCONF[BTN_UP])
         )) % 10;

      display.MoveCursorRel(lastcursor);

   } while (input != Keyconf::KEYCONF[BTN_RETURN] || _enemy_table[lastcursor.y][lastcursor.x] != 0);

   return lastcursor;
}

void Game::RenderState(void) {
   Table<char> tmp_table;

   _ship_set->FillTable(&tmp_table);

   BattleScreen& enemy_display = _main_screen->GetEnemyTable();
   BattleScreen& my_display = _main_screen->GetMyTable();

   for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {

         /* render enemy table */
         if (_enemy_table[i][j] == STATE_MISS)
            enemy_display.PutC(TARGET_MISS,j,i);
         else if (_enemy_table[i][j] == STATE_HIT) {
            enemy_display.PutC(TARGET_HIT,j,i);
         }

         /* render my table */
         if (tmp_table[i][j] == SHIP_SYMBOL) {
            if (_my_table[i][j] == STATE_HIT)
               my_display.PutC(TARGET_HIT,j,i);
            else
               my_display.PutC(SHIP_SYMBOL,j,i);
         } else if (_my_table[i][j] == STATE_MISS) {
            my_display.PutC(TARGET_MISS,j,i);
         }
      }
   }

   enemy_display.Refresh();
   my_display.Refresh();
}