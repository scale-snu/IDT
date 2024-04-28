/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2005 Gian-Carlo Pascutto

    File: preproc.c                                             
    Purpose: piece-square table generation

*/

#include "sjeng.h"
#include "extvars.h"
#include "evpar.h"
#include "bitboard.h"
#include "neval.h"
#include "bits.h"
#include "squares.h"

#undef PSQ_EVAL_DEBUG

signed char psq_table[12][64];

/*
    horizontal flipped board
*/
extern const int flip[64] = {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

/* 
    piece square tables for king
*/
static const int psq_king_nopawn[64] = {
    -40,-30,-22,-20,-20,-22,-30,-40,
    -30,-15,-10, -5, -5,-10,-15,-30,
    -22,-10,  5, 10, 10,  5,-10,-22,
    -20, -5, 10, 20, 20, 10, -5,-20,
    -20, -5, 10, 20, 20, 10, -5,-20,
    -22,-10,  5, 10, 10,  5,-10,-22,
    -30,-15,-10, -5, -5,-10,-15,-30,
    -40,-30,-22,-20,-20,-22,-30,-40
};
    
static const int psq_king_kingside[64] = {
    -75,-55,-40,-30,-20,-20,-30,-40,
    -70,-50,-30,-15, -5,  0,  0,-25,
    -70,-45,-25,-10, 20, 15, 10,-20,
    -60,-35,-15, -5, 30, 30, 15,-15,
    -60,-35,-25,-10, 25, 20, 10,-15,
    -75,-45,-25,-15, 15, 15,  5,-25,
    -75,-50,-30,-20,-10,-10,-10,-30,
    -80,-60,-50,-40,-30,-15,-20,-50,                           
};
 
static const int psq_king_queenside[64] = {
    -40,-30,-20,-20,-30,-40,-55,-75,
    -25,  0,  0, -5,-15,-30,-50,-70,
    -20, 10, 15, 20,-10,-25,-45,-70,
    -15, 15, 30, 30, -5,-15,-35,-60,
    -15, 10, 20, 25,-10,-25,-35,-60,
    -25,  5, 15, 15,-15,-25,-45,-75,
    -30,-10,-10,-10,-20,-30,-50,-75,
    -50,-20,-15,-30,-40,-50,-60,-80                              
};

static const int wknight_psq_end[64] = {
   -25,   -5,    0,    8,    8,    0,   -5,  -25,
   -16,    4,   10,   16,   16,   10,    4,    7,
    -1,   15,   20,   22,   22,   20,   15,   -7,
    -5,   10,   16,   16,   16,   16,   10,    6,
    -6,    5,   14,   13,   13,   14,    5,   -2,
   -14,   -3,    4,    7,    7,    4,   -3,  -14,
   -20,  -12,   -4,   -5,   -5,   -4,  -12,  -20,
   -25,  -24,  -16,  -14,  -14,  -16,  -24,  -25
};

static const int wking_psq_end[64] = {
     0,   30,   35,   20,   20,   35,   30,    0,
    -4,   25,   20,   24,   24,   20,   25,   -4,
    -4,   14,   16,    6,    6,   16,   14,   -4,
   -20,    0,    5,   -6,   -6,    5,    0,  -20,
   -16,   -4,    0,    2,    2,    0,   -4,  -16,
   -22,   -8,   -4,    0,    0,   -4,   -8,  -22,
   -26,  -15,  -10,   -8,   -8,  -10,  -15,  -26,
   -30,  -25,  -25,  -25,  -25,  -25,  -25,  -30
};  

static const int wrook_psq_end[64] = {
     5,    5,    7,   10,   10,    7,    5,    5,
     8,   10,   14,   14,   14,   14,   10,    8,
     1,    7,    7,    7,    7,    7,    7,    1,
    -1,    4,    3,    0,    0,    3,    4,   -1,
    -6,   -1,   -3,   -2,   -2,   -3,   -1,   -6,
   -10,   -4,   -8,   -8,   -8,   -8,   -4,  -10,
   -15,  -12,   -9,   -8,   -8,   -9,  -12,  -15,
   -15,  -10,   -8,   -8,   -8,   -8,  -10,  -15
};

static const int wbishop_psq_end[64] = {
    -8,  -10,   -6,   -1,   -1,   -6,  -10,   -8,
    -8,   -1,   -1,    0,    0,   -1,   -1,   -8,
    -1,    5,    7,    8,    8,    7,    5,   -1,
    -1,    4,    5,   10,   10,    5,    4,   -1,
     2,    2,    3,    9,    9,    7,    3,   -5,
    -2,    0,    6,    4,    4,    6,    0,   -2,
    -5,    3,    1,    2,    2,    1,    3,   -5,
   -10,   -6,   -8,   -8,   -8,   -8,   -6,  -10
};    

static const int wqueen_psq_end[64] = {
     5,   12,   16,   16,   16,   16,   12,    5,
   -10,   12,   20,   26,   26,   20,   12,  -10,
    -5,   10,   15,   18,   18,   15,   10,   -5,
   -15,    1,   10,   14,   14,   10,    1,  -15,
    -7,   -4,    6,    9,    9,    6,   -4,   -7,
   -12,   -8,   -2,   -4,   -4,   -2,   -8,  -12,
   -12,  -12,  -13,  -10,  -10,  -13,  -12,  -12,
   -20,  -25,  -25,  -10,  -10,  -25,  -25,  -20
};

static const int wpawn_psq[64] = {
     0,    0,    0,    0,    0,    0,    0,    0,
     0,  -10,    0,    0,    0,    0,  -10,    0,
     0,   -5,    8,   12,   12,    8,   -5,    0,
     0,    0,    6,   10,   10,    6,    0,    0,
     0,    0,    4,    8,    8,    4,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0
};

/*
    evaluate piece-square tables
*/
void psq_eval(state_t *s, t_eval_comps *ec) {                
    const int *king_psq;
    BITBOARD pawns;    
    int score;    
    
    score = s->psq_score;
  
    /*
        get right king endgame psq table
    */    
    pawns = s->WhitePawns | s->BlackPawns;

    if ((pawns & KSMask) && (pawns & QSMask)) {
        king_psq = wking_psq_end;
    } else if (pawns & KSMask) {
        king_psq = psq_king_kingside;
    } else if (pawns & QSMask) {
        king_psq = psq_king_queenside;
    } else {
        king_psq = psq_king_nopawn;
    }            
        
    ec->w_psq_king += king_psq[s->wking_loc];    
    ec->b_psq_king -= king_psq[flip[s->bking_loc]];
    
    return;
}

/*
    initialize piece-square tables score
*/
void init_psq_score(state_t *s) {    
    int b;                
    int score = 0;    
    
    BITBOARD pcs = s->All;

    while (pcs) {
        b = FindFirstRemove(&pcs);
        score += psq_table[s->sboard[b] - 1][b];
    }
    
    s->psq_score = score;
      
    return;
}

void preprocess_prepare(void) {
    int p, s, score;
    
    memset(psq_table, 0, sizeof(psq_table));
    
    for (p = 1; p <= 12; p++) {
        for (s = 0; s < 64; s++) {
            score = 0;
            if (p == wpawn) {
                score = wpawn_psq[s];
            } else if (p == bpawn) {
                score = -wpawn_psq[flip[s]];
            } else if (p == wknight) {
                score = wknight_psq_end[s];
            } else if (p == bknight) {
                score = -wknight_psq_end[flip[s]];
            } else if (p == wbishop) {
                score = wbishop_psq_end[s];
            } else if (p == bbishop) {
                score = -wbishop_psq_end[flip[s]];
            } else if (p == wrook) {
                score = wrook_psq_end[s];
            } else if (p == brook) {
                score = -wrook_psq_end[flip[s]];
            } else if (p == wqueen) {
                score = wqueen_psq_end[s] >> 1;
            } else if (p == bqueen) {
                score = -(wqueen_psq_end[flip[s]] >> 1);
            }            
            psq_table[p - 1][s] = score;
        }        
    }
    return;
}
