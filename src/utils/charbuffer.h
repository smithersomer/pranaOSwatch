#ifndef _CHAR_BUFFER_H
    #define _CHAR_BUFFER_H

    #include "config.h"

    #define INITIAL_CAPACITY 64
    #define CHUNK_CAPACITY 16

    class CharBuffer {
      public:

      CharBuffer();

      void append(char c);

      void clear();

      void erase(size_t pos =  0);

      inline const char *c_str() { return msg; }

      inline size_t length() { return size; }
      private:

      char* msg;

      size_t capacity;

      size_t size;
    };

#endif
