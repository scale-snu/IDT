#ifndef SEARCH_H
#define SEARCH_H

#include "sjeng.h"
#include "state.h"

void reset_search_counters(state_t *s);
void reset_search_data(state_t *s);
int qsearch(state_t *s, int alpha, int beta, int depth, int qply);
int search(state_t *s, int alpha, int beta, int depth, int is_null, int cutnode);
move_s think(gamestate_t *g, state_t *s);
void search_thread_master(int threadid);

#endif

