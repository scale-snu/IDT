/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2005 Gian-Carlo Pascutto    

    File: draw.c                                       
    Purpose: Detect draws

*/

#include "config.h"
#include "sjeng.h"
#include "extvars.h"
#include "state.h"

int is_draw (gamestate_t *g, state_t *s) {
    int i, repeats, end, start;
    
    /* 2 rep draw in search */
    if (s->fifty >= 4 && s->ply > 1) {
        
        end = g->move_number + s->ply - 1 - s->fifty;
        
        for (i = (g->move_number + s->ply - 3); i >= 0 && i >= end; i -= 2) {
            if (s->hash == s->hash_history[i]) {
                return TRUE;
            }
        }
    }

    /* 3 rep draw on root */    
    if (s->fifty >= 6 && s->ply <= 1) {
        start = g->move_number + s->ply;
        end = g->move_number + s->ply - s->fifty;      
        
        repeats = 0;
        
        for (i = start; i >= 0 && i >= end; i-=2) {           
            if (s->hash == s->hash_history[i]) {                
                repeats++;                                
                if (repeats >= 2) {
                    return TRUE;
                }
            }
        }
    }
    
    return FALSE;    
}

int insufficient_material(state_t *s) {    
    int *npieces = s->npieces;

    /* only B and N */
    if (!npieces[wpawn] && !npieces[bpawn] 
        && !npieces[wrook] && !npieces[brook]
        && !npieces[wqueen] 
        && !npieces[bqueen] ) {
        /* only 2x1 bishops */
        if (((npieces[bbishop] <= 1) 
            && (npieces[wbishop] <= 1)) 
            && !npieces[bknight] 
            && !npieces[wknight]) {
            return TRUE;
        }
        /* not more than 2 knights per side */
        if (!npieces[wbishop] 
            && !npieces[bbishop] 
            && (npieces[wknight] <= 2 
            && npieces[bknight] <= 2)) {
            return TRUE;
        }
    }
    return FALSE;
}
