/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2008 Gian-Carlo Pascutto    

    File: moves.c                                        
    Purpose: functions used to check move legality

*/

#include "sjeng.h"
#include "extvars.h"
#include "state.h"
#include "squares.h"
#include "moves.h"
#include "board.h"
#include "attacks.h"

/* 
   determines if a move made was legal.  Checks to see if the player who
   just moved castled through check, or is in check.  If the move made
   was illegal, returns FALSE, otherwise, returns TRUE. 
*/
int check_legal(state_t *s, move_s move) {    
    int castled = CASTLED(move);
    int i, king_dest, king_start, color;

    if (castled) {        
        if (castled == wck) {     
            king_start = s->wking_start;
            king_dest = g1;
            color = 0;            
        } else if (castled == wcq) {
            king_start = s->wking_start;
            king_dest = c1;
            color = 0;
        } else if (castled == bck) {
            king_start = s->bking_start;
            king_dest = g8;
            color = 1;            
        } else if (castled == bcq) {
            king_start = s->bking_start;
            king_dest = c8;
            color = 1;
        }
        /* now figure out direction of movement */
        if (king_dest > king_start) {
            for (i = king_start; i <= king_dest; i++) {
                if (is_attacked(s, i, color)) {                    
                    return FALSE;
                }
            }
        } else {
            for (i = king_start; i >= king_dest; i--) {
                if (is_attacked(s, i, color)) {                    
                    return FALSE;
                }
            }
        }
        return TRUE;
    } else if (s->white_to_move) { /* otherwise, just check on the kings: */ 
        if (is_attacked(s, s->bking_loc, 1)) {
            return FALSE;
        } else {
            return TRUE;
        }
    } else { 
        if (is_attacked(s, s->wking_loc, 0)) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

/*
    checks if castling is possible because
    there is no piece inbetween
    chess960 makes this complicated
*/
int check_castle_blocker(state_t *s, const int direction) {    
    int trace;

    if (direction == wck) {                
        /* trace king path */
        if (s->wking_start > g1) {
            for (trace = s->wking_start - 1; trace >= g1; trace--) {
                /* involved rook doesn't count */
                if (trace == s->wrrook_start) continue;
                if (s->sboard[trace] != npiece) {                    
                    return TRUE;
                }
            }
        } else if (s->wking_start < g1) {
            for (trace = s->wking_start + 1; trace <= g1; trace++) {
                /* involved rook doesn't count */
                if (trace == s->wrrook_start) continue;
                if (s->sboard[trace] != npiece) {                    
                    return TRUE;
                }
            }
        }
        /* trace rook path */
        if (s->wrrook_start > f1) {
            for (trace =s-> wrrook_start - 1; trace >= f1; trace--) {
                /* involved king doesn't count */
                if (trace == s->wking_start) continue;
                if (s->sboard[trace] != npiece) {                  
                    return TRUE;
                }
            }
        } else if (s->wrrook_start < f1) {
            for (trace = s->wrrook_start + 1; trace <= f1; trace++) {
                /* involved king doesn't count */
                if (trace == s->wking_start) continue;
                if (s->sboard[trace] != npiece) {                  
                    return TRUE;
                }
            }
        }        
    } else if (direction == wcq) {
        /* trace king path */
        if (s->wking_start > c1) {
            for (trace = s->wking_start - 1; trace >= c1; trace--) {
                /* involved rook doesn't count */
                if (trace == s->wlrook_start) continue;
                if (s->sboard[trace] != npiece) {                  
                    return TRUE;
                }
            }
        } else if (s->wking_start < c1) {
            for (trace = s->wking_start + 1; trace <= c1; trace++) {
                /* involved rook doesn't count */
                if (trace == s->wlrook_start) continue;
                if (s->sboard[trace] != npiece) {                
                    return TRUE;
                }
            }
        }
        /* trace rook path */
        if (s->wlrook_start > d1) {
            for (trace = s->wlrook_start - 1; trace >= d1; trace--) {
                /* involved king doesn't count */
                if (trace == s->wking_start) continue;
                if (s->sboard[trace] != npiece) {                  
                    return TRUE;
                }
            }
        } else if (s->wlrook_start < d1) {
            for (trace = s->wlrook_start + 1; trace <= d1; trace++) {
                /* involved king doesn't count */
                if (trace == s->wking_start) continue;
                if (s->sboard[trace] != npiece) {                 
                    return TRUE;
                }
            }
        }
    } else if (direction == bck) {
        /* trace king path */
        if (s->bking_start > g8) {
            for (trace = s->bking_start - 1; trace >= g8; trace--) {
                /* involved rook doesn't count */
                if (trace == s->brrook_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        } else if (s->bking_start < g8) {
            for (trace = s->bking_start + 1; trace <= g8; trace++) {
                /* involved rook doesn't count */
                if (trace == s->brrook_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        }
        /* trace rook path */
        if (s->brrook_start > f8) {
            for (trace = s->brrook_start - 1; trace >= f8; trace--) {
                /* involved king doesn't count */
                if (trace == s->bking_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        } else if (s->brrook_start < f8) {
            for (trace = s->brrook_start + 1; trace <= f8; trace++) {
                /* involved king doesn't count */
                if (trace == s->bking_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        }
    } else if (direction == bcq) {
        /* trace king path */
        if (s->bking_start > c8) {
            for (trace = s->bking_start - 1; trace >= c8; trace--) {
                /* involved rook doesn't count */
                if (trace == s->blrook_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        } else if (s->bking_start < c8) {
            for (trace = s->bking_start + 1; trace <= c8; trace++) {
                /* involved rook doesn't count */
                if (trace == s->blrook_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        }
        /* trace rook path */
        if (s->blrook_start > d8) {
            for (trace = s->blrook_start - 1; trace >= d8; trace--) {
                /* involved king doesn't count */
                if (trace == s->bking_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        } else if (s->blrook_start < d8) {
            for (trace = s->blrook_start + 1; trace <= d8; trace++) {
                /* involved king doesn't count */
                if (trace == s->bking_start) continue;
                if (s->sboard[trace] != npiece)
                    return TRUE;
            }
        }
    }

    return FALSE;
}

