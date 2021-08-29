#include "config.h"
#include "alloc.h"
#include "msg_chain.h"

msg_chain_t * msg_chain_add_msg(msg_chain_t *msg_chain, const char *msg) {
    
    if ( msg_chain == NULL ) {
        msg_chain = (msg_chain_t *)CALLOC( sizeof( msg_chain_t ), 1 );
        if ( msg_chain == NULL ) {
            log_e("msg_chain_t alloc failed");
            while( true );
        }

        msg_chain->entrys = 0;
        msg_chain->current_entry = 0;
        msg_chain->current_msg_chain_entry = NULL;
        msg_chain->first_msg_chain_entry = NULL;
    }
 
    msg_chain_entry_t *msg_chain_entry = (msg_chain_entry_t *)CALLOC( sizeof( msg_chain_entry_t ), 1 );
    if ( msg_chain_entry == NULL ) {
        log_e("msg_chain_entry_t alloc failed");
        while( true );
    }
    else {
        msg_chain_entry->msg = (const char *)CALLOC( strlen( msg ) + 1, 1 );
        if ( msg_chain_entry->msg == NULL ) {
            log_e("msg calloc failed");
            while( true );
        }
        msg_chain_entry->prev_msg = NULL;
        msg_chain_entry->next_msg = NULL;
        time( &msg_chain_entry->timestamp );
        strcpy( (char*)msg_chain_entry->msg, msg );
    }
    
    if ( msg_chain->first_msg_chain_entry == NULL ) {
        msg_chain->first_msg_chain_entry = msg_chain_entry;
        msg_chain->entrys++;
    }
    else {
        msg_chain_entry_t *current_msg_chain_entry = msg_chain->first_msg_chain_entry;

        while( current_msg_chain_entry->next_msg != NULL ) {
            current_msg_chain_entry = current_msg_chain_entry->next_msg;
        }

        current_msg_chain_entry->next_msg = msg_chain_entry;
        msg_chain_entry->prev_msg = current_msg_chain_entry;
        msg_chain->entrys++;
    }
    return( msg_chain );
}

bool msg_chain_delete_msg_entry(msg_chain_t *msg_chain, int32_t entry) {
    int32_t msg_counter = 0;
    bool retval = false;

    if ( msg_chain == NULL ) {
        return( retval );
    }

    if (msg_chain->first_msg_chain_entry == NULL) {
        return( retval );
    }

    if ( entry > msg_chain->entrys ) {
        return( retval );
    }

    msg_chain_entry_t *msg_chain_entry = msg_chain->first_msg_chain_entry;
    msg_chain_entry_t *prev_msg_chain_entry = NULL;
    msg_chain_entry_t *next_msg_chain_entry = NULL;

    do {
        if (entry == msg_counter) {

            if ( msg_chain_entry->prev_msg != NULL ) {
                prev_msg_chain_entry = msg_chain_entry->prev_msg;
            }
            if ( msg_chain_entry->next_msg != NULL ) {
                next_msg_chain_entry = msg_chain_entry->next_msg;
            }

            free( (void *)msg_chain_entry->msg );

            if ( prev_msg_chain_entry && next_msg_chain_entry ) {
                prev_msg_chain_entry->next_msg = next_msg_chain_entry;
                next_msg_chain_entry->prev_msg = prev_msg_chain_entry;
            }
            else if( !prev_msg_chain_entry && next_msg_chain_entry ) {
                next_msg_chain_entry->prev_msg = NULL;
                msg_chain->first_msg_chain_entry = next_msg_chain_entry;
            }
            else if( prev_msg_chain_entry && !next_msg_chain_entry ) {
                prev_msg_chain_entry->next_msg = NULL;
            }
            else {
                msg_chain->first_msg_chain_entry = NULL;
            }

            free( msg_chain_entry );
            msg_chain->entrys--;
            retval = true;
            break;
        }

        if (msg_chain_entry->next_msg != NULL) {
            msg_counter++;
            msg_chain_entry = msg_chain_entry->next_msg;
        }
        else {
            retval = false;
            break;
        }

    } while (true);

    return(retval);
}

time_t* msg_chain_get_msg_timestamp_entry(msg_chain_t *msg_chain, int32_t entry) {
    time_t* retval = NULL;
    int32_t msg_counter = 0;

    if ( msg_chain == NULL ) {
        return( retval );
    }

    if (msg_chain->first_msg_chain_entry == NULL) {
        return( retval );
    }

    if (entry > msg_chain->entrys) {
        return( retval );
    }

    msg_chain_entry_t *msg_chain_entry = msg_chain->first_msg_chain_entry;

    do {
        if (entry == msg_counter) {
            retval = &msg_chain_entry->timestamp;
            break;
        }
        if (msg_chain_entry->next_msg != NULL) {
            msg_counter++;
            msg_chain_entry = msg_chain_entry->next_msg;
        }
        else {
            break;
        }
    } while ( true );

    return( retval );
}

