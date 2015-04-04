#include "game.hpp"

Game Game::INSTANCE;

Game::Game(void) {
   _main_screen = MainScreen::GetInstance();
   _network = 0;

   _ship_set = 0;
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

   display.ShowCursor();

   display.MoveCursor(Coord(alignment + 2,cursor_pos + 10));

   do {
      input = display.handle_input();
      
      if (input == Keyconf::KEYCONF[BTN_UP]) {
         cursor_pos--;
         if (cursor_pos == -1)
            cursor_pos = 3;
      } else if (input == Keyconf::KEYCONF[BTN_DOWN]) {
         cursor_pos = ++cursor_pos % 4;
      }

      display.MoveCursor(Coord(alignment + 2,cursor_pos + 10));
   } while (input != Keyconf::KEYCONF[BTN_RETURN]);

   display.HideCursor();
   return cursor_pos;
}

bool Game::RunServer(void) {
   bool connected;
   MainScreen& display = (*_main_screen);
   try {
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
   int try_counter = 0;
   bool connected = false;
   string address;
   MainScreen& display = (*_main_screen);

   try {
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

void Game::Play(void) {
   _ship_set = new ShipSet();
   _main_screen->InitGameMode();

   _ship_set->Positioning((*_main_screen));
}