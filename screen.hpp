#ifndef _SCREEN_HPP
#define _SCREEN_HPP

#include <sstream>
#include <string>
#include <iostream>


#define BTN_DOWN 2
#define BTN_UP 3

#define BTN_LEFT 4
#define BTN_RIGHT 5

#define _CRT_SECURE_NO_WARNINGS

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

   void Refresh(void);
   void Clear(void);
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

   static MainScreen* INSTANCE;
   MainScreen();
public:

   BattleScreen& GetMyTable(void);
   BattleScreen& GetEnemyTable(void);
   Screen& GetStatusScreen(void);

   static MainScreen& GetInstance(void);

   ~MainScreen();
};


#endif
