#ifndef EPD_H
#define EPD_H

#include "state.h"
/*
    functions
*/

void setup_epd_line(gamestate_t *g, state_t *s, const char *inbuff);
int check_solution(state_t *s, char *inbuff, move_s cmove);
void run_epd_testsuite(gamestate_t *g, state_t *s, const char *filename);
void position_to_fen(state_t *s, char *fen);

/*
    data
*/
        

#endif
