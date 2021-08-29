// includes
#include "config.h"
#include "charbuffer.h"
#include "utils/alloc.h"

CharBuffer::CharBuffer(void) : msg(NULL), capacity(0), size(0) {
}

void CharBuffer::append(char c) {
    log_v("CharBuffer::append: size %d capacity %d", size, capacity);
    if (capacity == 0)
        clear();
    if (size + 2 > capacity) {
        log_v("CharBuffer::append realloc: size %d capacity %d", size, capacity);
        char *new_msg = NULL;
        new_msg = (char *)REALLOC( msg, capacity + CHUNK_CAPACITY );
        if (new_msg == NULL) {
            log_e("msg realloc fail");
            while(true);
        }
        msg = new_msg;
        capacity += CHUNK_CAPACITY;
    }
    size++;
    msg[ size - 1 ] = c;
    msg[ size ] = '\0';
}

void CharBuffer::clear(void) {
    if (capacity != INITIAL_CAPACITY) {
        log_d("CharBuffer::clear alloc");
        free(msg);
        msg = (char *)CALLOC( INITIAL_CAPACITY, 1 );
        if ( msg == NULL ) {
            log_e("msg alloc fail");
            while(true);
        }
        capacity = INITIAL_CAPACITY;
    }
    msg[0] = '\0';
    size = 0;
}

void CharBuffer::erase(size_t pos) {
    if ( pos >= size )
        return;
    for ( int i = pos ; i < size ; i++ ) {
        msg[i] = '\0';
    }
    size = pos;
}
