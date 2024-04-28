#ifndef BOARD_H
#define BOARD_H

#include "state.h"
/*
    data
*/                 

/*
    functions
*/
#if !defined(SPEC)
const int sum_dist(const int x, const int y);
const int taxicab_dist(const int x, const int y);
const int taxicab_dist_bit(const int x, const int y);
void check_phase(gamestate_t *g, state_t *s);
const int rank(const int sq);
const int file(const int sq);
#else
int sum_dist(const int x, const int y);
int taxicab_dist(const int x, const int y);
int taxicab_dist_bit(const int x, const int y);
void check_phase(gamestate_t *g, state_t *s);
int rank(const int sq);
int file(const int sq);
#endif

#endif
