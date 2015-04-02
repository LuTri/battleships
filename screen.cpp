#include <cstdio>
#include "screen.hpp"
#include "curses.h"

MainScreen* MainScreen::INSTANCE = 0;

const int BattleScreen::offset_x = 6;
const int BattleScreen::offset_y = 4;

Screen::Screen(void) {}

Screen::~Screen(void) {}

Screen::Screen(int height, int width, int x, int y) {
   _this_window = newwin(height,width,y,x);
   keypad((WINDOW*)_this_window, TRUE);
   Refresh();
}

void Screen::Refresh(void) {
   wrefresh((WINDOW*)_this_window);
}

void Screen::Clear(void) {
   wclear((WINDOW*)_this_window);
}

void Screen::printw(string val, bool refresh) { 
   wprintw((WINDOW*)_this_window,val.c_str());
   if (refresh)
      Refresh();
}

Screen& Screen::operator<<(string val) {
   printw(val, true);
   return *(this);
};

Screen& Screen::operator<<(int val) {
   stringstream ss;
   ss << val;
   printw(ss.str(),false);
   return *this;
}

char Screen::handle_input(void) {
   return wgetch((WINDOW*)_this_window);
}

MainScreen::MainScreen(void){
   initscr();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   curs_set(0);

   _this_window = (void*)stdscr;
   Refresh();

    mytable = new BattleScreen(0,0);
    enemytable = new BattleScreen(66,0);
    statusscreen = new Screen(2,132,0,45);
}

MainScreen::~MainScreen(void) {
   endwin();
   delete mytable;
   delete enemytable;
   MainScreen::INSTANCE = 0;
}

MainScreen& MainScreen::GetInstance(void) {
   if (MainScreen::INSTANCE == 0) {
      MainScreen::INSTANCE = new MainScreen();
   }

   return *(MainScreen::INSTANCE);
}

BattleScreen& MainScreen::GetMyTable(void) {
   return *mytable;
}

BattleScreen& MainScreen::GetEnemyTable(void) {
   return *enemytable;
}

Screen& MainScreen::GetStatusScreen(void) {
   return *statusscreen;
}

/* BATTLE SCREEN */
BattleScreen::BattleScreen(int x, int y) {
   _this_window = (void*) newwin(44,66,y,x);
   keypad((WINDOW*)_this_window, TRUE);
   Init();
}

void BattleScreen::put_game_symbol(char val) {
   char buff[3];
   sprintf(buff,"%c",(char)val);
   wprintw((WINDOW*)_this_window,buff);
}

void BattleScreen::DrawTable(void) {
   for (int i = 0; i < 41; i++) {
      wmove((WINDOW*)_this_window, offset_y - 1 + i, offset_x - 1);
      for (int j = 0; j < 61; j++) {
         if (j == 0 && i == 0)
            put_game_symbol(201); // top-left corner
         else if (j == 60 && i == 0)
            put_game_symbol(187); // top-right corner
         else if (j == 0 && i == 40)
            put_game_symbol(200); // bottom-left corner
         else if (j == 60 && i == 40)
            put_game_symbol(188); // bottom-right corner
         else if (j % 6 == 0 && i == 0)
            put_game_symbol(203); // downward T-thingy
         else if (j % 6 == 0 && i == 40)
            put_game_symbol(202); // upward T-thingy
         else if (j == 0 && i % 4 == 0)
            put_game_symbol(204); // rightward T-thingy
         else if (j == 60 && i % 4 == 0)
            put_game_symbol(185); // leftward T-thingy
         else if (j % 6 != 0 && i % 4 == 0)
            put_game_symbol(205); // horizontal double line
         else if (j % 6 == 0 && i % 4 != 0)
            put_game_symbol(186); // vertical double line
         else if (j % 6 == 0 && i % 4 == 0)
            put_game_symbol(206); // cross thingy
         else
            put_game_symbol(' ');
         }
   }
   Refresh();
}

void BattleScreen::PutC(char val, int x, int y) {
   int res_x = offset_x + offset_x / 2 + x * offset_x - 1;
   int res_y = offset_y + offset_y / 2 + y * offset_y - 1;
   mvwaddch((WINDOW*)_this_window,res_y,res_x,val);
}

void BattleScreen::Init(void) {
   DrawTable();
   
   for (int i = 0; i < 10; i++) {
      PutC('0' + i, -1, i);
      PutC('A' + i, i, -1);
   }
   Refresh();
}