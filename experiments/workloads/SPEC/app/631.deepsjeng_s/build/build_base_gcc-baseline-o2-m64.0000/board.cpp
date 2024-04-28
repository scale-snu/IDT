/*
    Sjeng - a chess playing program
    Copyright (C) 2001-2007 Gian-Carlo Pascutto

    File: board.c                                             
    Purpose: board info helpers
 
*/

#include "sjeng.h"
#include "extvars.h"
#include "bitboard.h"
#include "preproc.h"
#include "bits.h"
#include "neval.h"
#include "squares.h"

#if !defined(SPEC)
const
#endif
int rank(const int sq) {
    return (8 - Rank(sq));
}

#if !defined(SPEC)
const
#endif
int file(const int sq) {
    return File(sq) + 1;
}

/*
    returns sum of rank and file distance 
*/
#if !defined(SPEC)
const
#endif
int sum_dist(const int x, const int y) {
    const int filedist = abs(file(x) - file(y));
    const int rankdist = abs(rank(x) - rank(y));

    return filedist + rankdist;
}

/*
    returns "taxicab" distance
*/
#if !defined(SPEC)
const
#endif
int taxicab_dist(const int x, const int y) {
    const int filedist = abs(file(x) - file(y));
    const int rankdist = abs(rank(x) - rank(y));
    
    if (filedist > rankdist) {
        return filedist;
    } else {
        return rankdist;
    }
}

#if !defined(SPEC)
const
#endif
int taxicab_dist_bit(const int x, const int y) {
    return taxicab_dist(x, y);
}

void check_phase(gamestate_t *g, state_t *s) {
    int i;

    int total_material_w = 0;
    int total_material_b = 0;

    int num_pieces = 0;    
         
    BITBOARD pcs = s->All;
    int *sboard = s->sboard;
    
    while (pcs) {
        i = FindFirstRemove(&pcs); 

        if (sboard[i] != wpawn && sboard[i] != bpawn && sboard[i] != npiece) {
            if (sboard[i] != wking && sboard[i] != bking) {
                if (sboard[i] % 2 == 1) {
                    total_material_w += abs(material[sboard[i]]);
                } else {
                    total_material_b += abs(material[sboard[i]]);
                }
            }

            num_pieces++;
        }
    }

    if ((num_pieces >= 12)                             /* not both have castled */
         && (s->wking_loc == e1 || s->bking_loc == e8) /* no both lost castling privileges */
         && (s->castleflag > 0)) {
        g->phase = OPENING;
    } else if (total_material_w <= 1700 && total_material_b <= 1700) {
        g->phase = ENDGAME;
    } else {
        g->phase = MIDDLEGAME;
    }
}
