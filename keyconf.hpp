#ifndef _KEYCONF_HPP
#define _KEYCONF_HPP

#include "screen.hpp"

#define BTN_DOWN 1
#define BTN_UP 2

#define BTN_LEFT 3
#define BTN_RIGHT 4

#define BTN_ROTATE 5

#define BTN_RETURN 6

#define BTN_KEYCONF 7

using namespace std;

class Keyconf {
private:
   bool loaded;
   Keyconf();
   char data[255];
public:
   char getkey(char meaning);

   static Keyconf KEYCONF;

   bool load(void);
   bool write(void);

   bool config(Screen& ioscreen);

   char operator[](const char idx);
};

#endif
