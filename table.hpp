#ifndef _TABLE_HPP
#define _TABLE_HPP

template <typename type>
class List {
private:
   type _vals[10];

public:
   type& operator[](int idx) {
      return _vals[idx];
   }

   List(void) {
      for (int i = 0; i < 10; i++) {
         _vals[i] = 0;
      }
   }

   void Clear(void) {
      for (int i = 0; i < 10; i++) {
         _vals[i] = 0;
      }
   }
};

template <typename type>
class Table {
private:
   List<type> _vals[10];

public:
   Table(void) {};

   List<type>& operator[](int idx) {
      return _vals[idx];
   }

   void Clear(void) {
      for (int i = 0; i < 10; i++) {
         _vals[i].Clear();
      }
   }
};

#endif
