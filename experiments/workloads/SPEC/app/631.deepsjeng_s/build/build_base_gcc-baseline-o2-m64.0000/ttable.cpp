/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2006 Gian-Carlo Pascutto

    File: ttable.c                                       
    Purpose: handling of transposition tables and hashes

*/
#include "sjeng.h"
#include "extvars.h"
#include "limits.h"
#include "ttable.h"
#include "bitboard.h"
#include "utils.h"
#include "pawn.h"

BITBOARD zobrist[14][64];

ttentry_t *TTable;
unsigned int TTAge; 

void clear_tt(void) {
    memset(TTable, 0, sizeof(ttentry_t) * TTSize);    
}

void clear_dp_tt(void) {
    TTAge++;

    if (TTAge > 3) {
        TTAge = 0;
    }

    return;
}

void initialize_zobrist(state_t *s) {
    int p, q;
    
#if defined(SPEC)
    BITBOARD temp;
#endif
    TTAge = 0;

    mysrand(31657);

    for (p = 0; p < 14; p++) {
        for (q = 0; q < 64; q++) {
#if defined(SPEC)
            temp = ((BITBOARD)myrandom()) << 32;
            temp += (BITBOARD)myrandom();
            zobrist[p][q] = temp;
#else
            zobrist[p][q] = (((BITBOARD)myrandom()) << 32) + (BITBOARD)myrandom();
#endif
        }
    }

    /* our magic number */
    s->hash = (CONST64U(0xDEADBEEF) << 32) + CONST64U(0xDEADBEEF);
    s->pawnhash = (CONST64U(0xC0FFEE00) << 32) + CONST64U(0xC0FFEE00);
}

void initialize_hash(state_t *s) {
    int p;

    s->hash = (CONST64U(0xDEADBEEF) << 32) + CONST64U(0xDEADBEEF);
    s->pawnhash = (CONST64U(0xC0FFEE00) << 32) + CONST64U(0xC0FFEE00);

    for (p = 0; p < 64; p++) {
        if (PIECET(s,p) != npiece) {
            s->hash = s->hash ^ zobrist[s->sboard[p]][p];
        }

        if (PIECET(s,p) == pawn) {
            s->pawnhash = s->pawnhash ^ zobrist[s->sboard[p]][p];
        }
    }

    if (s->ep_square) {
        // hash = hash ^ zobrist[0][nobit_to_bit[ep_square]];
    }
}

void StoreTT(state_t*s, 
             int score, int alpha, int beta, unsigned int best, int threat, int singular,
             int nosingular, const int depth) {
    int i, pdepth, bestdepth, besti;
    BITBOARD nhash;
    unsigned int index;
    ttentry_t *temp;
    ttbucket_t *entry;    

    if (!s->white_to_move) {
        nhash = s->hash + 1;
    } else {
        nhash = s->hash;
    }

    index = (unsigned int)nhash;
    temp = &(TTable[index % TTSize]);

    nhash >>= 32;
    
    /*
        get oldest, undeepest entry
    */
    bestdepth = 65535;

    for (i = 0; i < BUCKETS; i++) {
        if (temp->buckets[i].hash == nhash) {
            s->TTStores++;
            besti = i;
            break;
        } else {
            s->TTStores++;
            if (temp->buckets[i].age == TTAge) {             
                s->TTColls++;
            }
        }

        pdepth = temp->buckets[i].depth - (abs((int)(temp->buckets[i].age - TTAge)) * 1024);

        if (pdepth < bestdepth) {
            bestdepth = pdepth;
            besti = i;
        }
    }

    entry = &(temp->buckets[besti]);
        
    if (entry->hash == nhash) {
        if (entry->depth > depth) {      
            return;
        } 
    }
    
    if (score <= alpha) {
        entry->type = UPPER;               
    } else if (score >= beta) {
        entry->type = LOWER;      
    } else {
        entry->type = EXACT;                
    }    
    
    /* 
        normalize mate scores 
    */
    if (score > (+MATE - 500)) {
        score += (s->ply - 1);
    } else if (score < (-MATE + 500)) {
        score -= (s->ply - 1);
    }
    
    if (score > MATE) {
        score = MATE;
    } else if (score < -MATE) {
        score = -MATE;
    }       

    entry->hash = (unsigned int)nhash;
    entry->bestmove = best;

    entry->depth = depth;
    entry->bound = score;
    entry->threat = threat;
    entry->age = TTAge;
    entry->singular = singular;
    entry->nosingular = nosingular;      

    return;
}

int ProbeTT(state_t *s,
            int *score, int alpha, int beta, 
            unsigned int *best, int *threat, int *donull, int *singular,
            int *nosingular, const int depth) {
    int type;
    int i;    
    BITBOARD nhash;
    unsigned int index;
    ttentry_t *temp;
    ttbucket_t *entry;

    *donull = TRUE;

    s->TTProbes++;

    if (!s->white_to_move) {
        nhash = s->hash + 1;
    } else {
        nhash = s->hash;
    }

    index = (unsigned int)nhash;
    temp = &(TTable[index % (TTSize)]);

    nhash >>= 32;

    for (i = 0; i < BUCKETS; i++) {
        if (temp->buckets[i].hash == nhash) {
            s->TTHits++;

            entry = &(temp->buckets[i]);                        
            
            if (entry->age != TTAge) {
                entry->age = TTAge;
            }

            if (entry->type == UPPER 
                && depth - 4 * PLY <= entry->depth 
                && entry->bound < beta) {
                *donull = FALSE;
            }                        

            if (entry->depth >= depth) {
                *score = entry->bound;

                if (*score > (+MATE - 500)) {
                    *score -= (s->ply - 1);
                } else if (*score < (-MATE + 500)) {
                    *score += (s->ply - 1);
                }

                *best = entry->bestmove;
                *threat = entry->threat;
                *singular = entry->singular;
                *nosingular = entry->nosingular;
                type = entry->type;
                                   
                return type;
            } else {
                *best = entry->bestmove;
                *threat = entry->threat;
                *singular = entry->singular;
                *nosingular = entry->nosingular;

                if (entry->type == UPPER) {
                    *score = -INF;
                } else if (entry->type == LOWER) {
                    *score = +INF;
                } else {
                    *score = entry->bound;
                }
                
                entry->depth = depth;
                entry->type = DUMMY;                

                return DUMMY;
            }
        } 
    }      
         
    return HMISS;
}

void alloc_hash(void) {
    TTable = (ttentry_t *)malloc(sizeof(ttentry_t) * (size_t)TTSize);

    if (TTable == NULL) {
        myprintf("Out of memory allocating hashtables.\n");

        exit(EXIT_FAILURE);
    }

    myprintf("Allocated %d hash entries, totalling %llu bytes.\n", TTSize,
             (sizeof(ttentry_t) * (BITBOARD)TTSize));

    return;
}

void free_hash(void) {
    free(TTable);

    TTable = NULL;

    return;
}

