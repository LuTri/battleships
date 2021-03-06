﻿#include <cstdio>
#include "screen.hpp"

#ifndef LINUX
#include "curses.h"
#else
#include <ncurses.h>
#endif

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

void Screen::ShowCursor(void) {
   curs_set(2);
}

void Screen::HideCursor(void) {
   curs_set(0);
}

void Screen::MoveCursor(Coord pos) {
   wmove((WINDOW*)_this_window,pos.y,pos.x);
}

string Screen::ReadString(void) {
   char buff[200];
   echo();
   curs_set(1);
   getstr(buff);
   curs_set(0);
   noecho();
   return string(buff);
}

/* Mainscreen implementation */

MainScreen::MainScreen(void){
   initscr();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   curs_set(0);

   _this_window = (void*)stdscr;
   Refresh();

   mytable = 0;
   enemytable = 0;
}

void MainScreen::InitGameMode(void) {
    mytable = new BattleScreen(0,0);
    enemytable = new BattleScreen(66,0);
    statusscreen = new Screen(2,132,0,45);
}

MainScreen::~MainScreen(void) {
   endwin();
   if (mytable)
      delete mytable;
   if (enemytable)
      delete enemytable;
   MainScreen::INSTANCE = 0;
}

MainScreen* MainScreen::GetInstance(void) {
   if (MainScreen::INSTANCE == 0) {
      MainScreen::INSTANCE = new MainScreen();
   }

   return MainScreen::INSTANCE;
}

Coord MainScreen::GetMaxSize(void) {
   CheckSufficientSize();

   return Coord(_max_x, _max_y);
}

bool MainScreen::CheckSufficientSize(void) {
   getmaxyx((WINDOW*)_this_window, _max_y, _max_x);

   if (_max_x < NEEDED_WIDTH || _max_y < NEEDED_HEIGHT) {
      (*this) << "ERROR: Ihre Konsole ist zu klein.\n" <<\
         "Sie brauchen mindestens eine Groesse von " << NEEDED_WIDTH << " mal " << NEEDED_HEIGHT << ".\n" <<\
         "Bitte vergroessern sie Ihre Konsole und starten sie das Programm neu!.";

      Refresh();
      return false;
   }
   return true;
}

int MainScreen::Centered(string value) {
   int size = value.length();
   int y,x;

   getyx((WINDOW*)_this_window,y,x);

   x = _max_x / 2 - size / 2;

   mvwprintw((WINDOW*)_this_window, y+1, x, value.c_str());

   return x;
}

void MainScreen::Aligned(string value, int align) {
   int y,x;

   getyx((WINDOW*)_this_window,y,x);

   mvwprintw((WINDOW*)_this_window, y, align, value.c_str());
}

/* Battlescreen implementation */

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
            put_game_symbol(TL_CORNER); // top-left corner
         else if (j == 60 && i == 0)
            put_game_symbol(TR_CORNER); // top-right corner
         else if (j == 0 && i == 40)
            put_game_symbol(BL_CORNER); // bottom-left corner
         else if (j == 60 && i == 40)
            put_game_symbol(BR_CORNER); // bottom-right corner
         else if (j % 6 == 0 && i == 0)
            put_game_symbol(D_TTHINGY); // downward T-thingy
         else if (j % 6 == 0 && i == 40)
            put_game_symbol(U_TTHINGY); // upward T-thingy
         else if (j == 0 && i % 4 == 0)
            put_game_symbol(R_TTHINGY); // rightward T-thingy
         else if (j == 60 && i % 4 == 0)
            put_game_symbol(L_TTHINGY); // leftward T-thingy
         else if (j % 6 != 0 && i % 4 == 0)
            put_game_symbol(H_DOUBLEL); // horizontal double line
         else if (j % 6 == 0 && i % 4 != 0)
            put_game_symbol(V_DOUBLEL); // vertical double line
         else if (j % 6 == 0 && i % 4 == 0)
            put_game_symbol(D_CROSS); // cross thingy
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
