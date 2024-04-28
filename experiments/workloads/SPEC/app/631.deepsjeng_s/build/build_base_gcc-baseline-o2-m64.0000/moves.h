#ifndef MOVES_H
#define MOVES_H

#include "sjeng.h"

/*
    functions
*/
int check_legal(state_t *s, move_s move);
int check_castle_blocker(state_t *s, const int direction);

#endif
