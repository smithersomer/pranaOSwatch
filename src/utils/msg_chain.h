#ifndef _MSG_CHAIN_H
#define _MSG_CHAIN_H

#include <stdint.h>
#include <sys/time.h>

struct msg_chain_entry_t {
    msg_chain_entry_t *prev_msg;
    time_t timestamp;
    const char *msg;
    msg_chain_entry_t *next_msg;
}

struct msg_chain_t {
    int32_t entrys;
    msg_chain_entry_t *first_msg_chain_entry;
    int32_t current_entry;
    msg_chain_entry_t *current_msg_chain_entry;
}

msg_chain_t* msg_chain_add_msg(msg_chain_t *msg_chain, const char *msg);

int32_t msg_chain_get_entrys(msg_chain_t *msg_chain);

msg_chain_t * msg_chain_delete(msg_chain_t *msg_chain);

void msg_chain_printf_msg_chain(msg_chain_t *msg_chain);

#endif
