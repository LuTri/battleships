#ifndef _SCREEN_HPP
#define _SCREEN_HPP

#include <sstream>
#include <string>
#include <iostream>
#include "positioning.hpp"

#define NEEDED_WIDTH 132
#define NEEDED_HEIGHT 60

#ifndef LINUX
// Windows Borders
#define TL_CORNER (char)201
#define TR_CORNER (char)187
#define BL_CORNER (char)200
#define BR_CORNER (char)188
#define D_TTHINGY (char)203
#define U_TTHINGY (char)202
#define R_TTHINGY (char)204
#define L_TTHINGY (char)185
#define H_DOUBLEL (char)205
#define V_DOUBLEL (char)186
#define D_CROSS   (char)206
#else
// Linux Borders
#define TL_CORNER '+'
#define TR_CORNER '+'
#define BL_CORNER '+'
#define BR_CORNER '+'
#define D_TTHINGY '+'
#define U_TTHINGY '+'
#define R_TTHINGY '+'
#define L_TTHINGY '+'
#define H_DOUBLEL '-'
#define V_DOUBLEL '|'
#define D_CROSS   '+'
#endif

using namespace std;

class GameSymbol {
private:
   int val;

public:
   GameSymbol(float val);
   operator char();
};

class Screen {
protected:
   void* _this_window;
   Screen(void);
   ~Screen(void);
   void printw(string val, bool refresh);

public:
   Screen(int height, int width, int x, int y);

   char handle_input(void);

   Screen& operator<<(string val);
   Screen& operator<<(int val);

   void ShowCursor(void);
   void HideCursor(void);

   void MoveCursor(Coord pos);

   void Refresh(void);
   void Clear(void);

   string ReadString(void);
};


class BattleScreen: public Screen {
private:
   static const int offset_x;
   static const int offset_y;
   void DrawTable(void);
   void put_game_symbol(char val);
public:
   BattleScreen(int x, int y);
   void Init(void);
   void PutC(char val, int x, int y);
};

class MainScreen: public Screen {
private:
   BattleScreen *mytable;
   BattleScreen *enemytable;
   Screen *statusscreen;

   int _max_x;
   int _max_y;

   static MainScreen* INSTANCE;
   MainScreen();
public:

   BattleScreen& GetMyTable(void);
   BattleScreen& GetEnemyTable(void);
   Screen& GetStatusScreen(void);

   static MainScreen* GetInstance(void);

   void InitGameMode(void);

   Coord GetMaxSize(void);
   bool CheckSufficientSize(void);

   int Centered(string value);
   void Aligned(string value, int align);

   ~MainScreen();
};


#endif

#define _CRT_SECURE_NO_WARNINGS
