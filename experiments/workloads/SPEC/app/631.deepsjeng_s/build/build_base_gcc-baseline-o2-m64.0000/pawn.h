#ifndef PAWN_H
#define PAWN_H

/*
    defs
*/
struct pawntt_t {
    BITBOARD pawnhash;
    BITBOARD open_files;
    BITBOARD w_half_open_files;
    BITBOARD b_half_open_files;
    BITBOARD w_passed;
    BITBOARD b_passed;      
    BITBOARD w_strong_square;
    BITBOARD b_strong_square;
    BITBOARD w_super_strong_square;
    BITBOARD b_super_strong_square;
            
    int w_score;                              
    int b_score;        
};

/*
    data
*/
extern int PawnProbes;
extern int PawnHits;

/* 
    functions
*/
void clear_pawn_tt(void);
pawntt_t *static_pawn_eval(state_t *s);
#if !defined(SPEC)
const
#endif
int scale(const int rank, const int min, const int max);

#endif

