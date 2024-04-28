#ifndef TTABLE_H
#define TTABLE_H

#include "state.h"
/*
    defs
*/
#define BUCKETS 4

struct ttbucket_t {
    unsigned int   hash; 
    short          bound;       
    unsigned short bestmove;   
    unsigned char  depth;             
    unsigned char  threat     : 1,
                   type       : 2,
                   singular   : 1,
                   nosingular : 1,                   
                   age        : 2;                      
};

struct ttentry_t {
    ttbucket_t buckets[BUCKETS];
};

/*
    data
*/
extern BITBOARD zobrist[14][64]; 
    
/*
    functions
*/
void StoreTT(state_t *s, 
             int score, int alpha, int beta, unsigned int best, int threat, 
             int singular, int nosingular, const int depth);
int ProbeTT(state_t *s, 
            int *score, int alpha, int beta, unsigned int *best, int *threat, int *donull, 
            int *singular, int *nosingular, const int depth);                       
void initialize_hash(state_t *s);
void clear_tt(void);
void alloc_hash(void);
void clear_dp_tt(void);
void initialize_zobrist(state_t *s);
void free_hash(void);

#endif

