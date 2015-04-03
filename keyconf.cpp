#include "keyconf.hpp"
#include <fstream>

#define FILENAME "battleship.keys"

Keyconf Keyconf::KEYCONF;

Keyconf::Keyconf() {
   loaded = false;
}

bool Keyconf::load(void) {
   if (!loaded) {
      ifstream file(FILENAME, ios::in | ios::binary);
      if (!file) {
         return false;
      }
      file.read(data,255);
      if (!file) {
         return false;
      }
      file.close();

      loaded = true;
   }
   return true;
}

char Keyconf::operator[](const char idx) {
   return data[idx];
}

bool Keyconf::config(Screen& ioscreen) {
   char input;
   ioscreen.Clear();

   ioscreen << "Bitte druecken fuer: LEFT";
   input = ioscreen.handle_input();
   data[BTN_LEFT] = input;
   ioscreen.Clear();

   ioscreen << "Bitte druecken fuer: RIGHT";
   input = ioscreen.handle_input();
   data[BTN_RIGHT] = input;
   ioscreen.Clear();

   ioscreen << "Bitte druecken fuer: UP";
   input = ioscreen.handle_input();
   data[BTN_UP] = input;
   ioscreen.Clear();

   ioscreen << "Bitte druecken fuer: DOWN";
   input = ioscreen.handle_input();
   data[BTN_DOWN] = input;
   ioscreen.Clear();

   ioscreen << "Bitte druecken fuer: RETURN";
   input = ioscreen.handle_input();
   data[BTN_RETURN] = input;
   ioscreen.Clear();

   ioscreen << "Bitte druecken fuer: ROTATE";
   input = ioscreen.handle_input();
   data[BTN_ROTATE] = input;
   ioscreen.Clear();

   data[BTN_KEYCONF] = 'k';

   return (loaded = true && write());
}

bool Keyconf::write(void) {
   ofstream file(FILENAME, ios::out | ios::binary);
   if (!file) {
      return false;
   }
   file.write(data,255);
   if (!file) {
      return false;
   }
   file.close();

   return true;
}