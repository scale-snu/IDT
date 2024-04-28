#ifndef ATTACKS_H
#define ATTACKS_H

#include "state.h"
/* 
    functions
*/
int is_attacked(state_t *state, const int square, const int color);
BITBOARD attacks_to(state_t *state, const int sq);
int in_check(state_t *state);

#endif

