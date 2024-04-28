/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2008 Gian-Carlo Pascutto

    File: pawn.c                                             
    Purpose: evaluate pawnstructure
*/

#include "sjeng.h"
#include "extvars.h"
#include "evpar.h"
#include "bitboard.h"
#include "bits.h"
#include "pawn.h"
#include "squares.h"

#define PAWN_HASH_LOG   (14)
#undef PAWN_EVAL_DEBUG

/*
    table
*/
static const int w_candidate[6] = {
    0, 44, 12, 10, 3, 3 
};

/*
    pawn evaluation hashtable 
*/
static pawntt_t PawnTT[MAX_CPU][1 << PAWN_HASH_LOG];

/*
    reset pawn evaluation hash table
*/
void clear_pawn_tt(void) {
    memset(PawnTT, 0, sizeof(PawnTT));
}

/*
    power scaler 
*/
#if !defined(SPEC)
const
#endif
int scale(const int rank, const int min, const int max) {
    const int scf[8] = { 0, 1, 2, 5, 10, 20, 32, 0 };

    assert(rank >= 0 && rank <= 7);
    return min + (((max - min) * scf[rank] + 16) / 32);
}

static void full_pawn_eval(state_t *s, pawntt_t *const pawntt) {
    int i;
    BITBOARD temp;
    BITBOARD this_file;
    BITBOARD side_mask;
    BITBOARD urank_mask;
    BITBOARD urank2_mask;
    BITBOARD urank3_mask;
    int supporters, blockers;
    int doubled;
    int isolated;
    int openfile;
    int backward;
    int passed;
    int candidate;
    int trailer;
    int it;
    int newsq, newfile;
    int attks;
    BITBOARD *BitBoard = s->BitBoard;

    /*
        initialize
    */
    memset(pawntt, 0, sizeof(pawntt_t));
   
    /*
        File openness loop
    */
    for (i = 0; i < 8; i++) {
        this_file = FileMask[i];

        if (!(this_file & WhitePawns)) {
            pawntt->w_half_open_files |= this_file;
        }

        if (!(this_file & BlackPawns)) {
            pawntt->b_half_open_files |= this_file;
        }

        if (!(this_file & (WhitePawns | BlackPawns))) {
            pawntt->open_files |= this_file;
        }
    }

    /*
        White loop
    */
    temp = WhitePawns;
    while (temp) {

        /*
            whereami
        */
        it = FindFirstRemove(&temp);
        i = File(it);

        /*
            init status
        */
        doubled = FALSE;
        isolated = FALSE;
        openfile = FALSE;
        backward = FALSE;
        passed = FALSE;
        candidate = FALSE;
        trailer = FALSE;

        /*
            mask this file
        */
        this_file = FileMask[i] & WhitePawns;

        /*
            determine mask for side files
        */
        if (i == FileA) {
            side_mask = FileMask[FileB];
        } else if (i == FileH) {
            side_mask = FileMask[FileG];
        } else {
            side_mask = FileMask[i - 1] | FileMask[i + 1];
        }

        /*
            open file
        */
        if (!(FileUpMask[it] & BlackPawns)) {
            openfile = TRUE;
        }

        /* 
            doubled 
        */
        if (PopCount(this_file) > 1) {
            doubled = TRUE;

            if (this_file & AboveMask[Rank(it)]) {
                trailer = TRUE;
            }
        }

        /*
            isolated
        */
        if (!(side_mask & WhitePawns)) {
            isolated = TRUE;
        } else if (!(WhitePawns & side_mask & BelowMask[Rank(it) - 1])) {

            /* 
                backward?                        
                no supporters behind
                assume the worst
            */
            backward = TRUE;

            /* 
                precalculate some masks
            */
            urank_mask = RankMask[Rank(it) - 1];
            if (Rank(it) > Rank7) {
                urank2_mask = urank_mask | RankMask[Rank(it) - 2];
            }

            if (Rank(it) > Rank6) {
                urank3_mask = urank2_mask | RankMask[Rank(it) - 3];
            }

            /*                    
                can we join supporter forward?
            */
            if (WhitePawns & side_mask & urank_mask) {
                /*
                    are we in a vulnerable position?
                */
                if (FileMask[i] & urank_mask & (BlackPawns | WhitePawns)) {
                    /* 
                        someone above us, so no 
                    */
                    backward = FALSE;
                }

                if (Rank(it) > Rank7) {
                    if (!(side_mask & urank2_mask & BlackPawns)) {
                        /*
                            no attackers, so no
                        */
                        backward = FALSE;
                    }
                } else {
                    /*
                        too far to attack
                    */
                    backward = FALSE;
                }
            } else if ((Rank(it) == Rank2) && (WhitePawns & side_mask & urank2_mask)) {
                /*
                    we can move up 2 if needed
                */
                if (FileMask[i] & urank2_mask & (BlackPawns | WhitePawns)) {
                    backward = FALSE;
                }

                if (!(side_mask & urank3_mask & BlackPawns)) {
                    backward = FALSE;
                }
            }
        }

        if (openfile) {
            /*
                passed
            */
            if (!(AboveMask[Rank(it)] & side_mask & BlackPawns)) {
                passed = TRUE;
                pawntt->w_passed |= Mask[it];
            } else {

                /*
                    candidate passed                        
                */
                supporters = PopCount(WhitePawns & side_mask & BelowMask[Rank(it) - 1]);
                blockers = PopCount(BlackPawns & side_mask & AboveMask[Rank(it)]);

                /*
                    remove supporters that can't really support us
                    because they are blocked themselves
                */
                supporters -= PopCount(BlackPawns & side_mask & BelowMask[Rank(it) - 1]);

                /*
                    as much supporters as blockers?
                */
                attks = supporters - blockers;

                if (attks >= 0) {

                    /*
                        defended now?
                    */
                    attks = PopCount(WhitePawns & side_mask & RankMask[Rank(it) + 1]);
                    attks -= PopCount(BlackPawns & side_mask & RankMask[Rank(it) - 1]);

                    if (attks >= 0) {
                        candidate = TRUE;
                    }
                }
            }
        }

        /*
            we doublecount here, so /2
        */
        if (doubled) {                        
            if (openfile) {
                pawntt->w_score += -10;
            } else {
                pawntt->w_score +=  -5;
            }
        }

        if (isolated) {
            if (openfile) {
                pawntt->w_score += -20;
            } else {
                pawntt->w_score +=  -8;
            }
        } else if (backward) {
            if (openfile) {
                pawntt->w_score += -16;
            } else {
                pawntt->w_score +=  -6;
            }
        }

        if (candidate) {
            if (!trailer) {                
                pawntt->w_score += w_candidate[Rank(it) - 1] * 2;
            } else {                
                pawntt->w_score += w_candidate[Rank(it) - 1];
            }
        }

#ifdef PAWN_EVAL_DEBUG
        myprintf("Pawn %s: ",sq_name[it]);
        if (doubled) {
            myprintf("doubled ");
        }

        if (trailer) {
            myprintf("trailer ");
        }

        if (isolated) {
            myprintf("isolated ");
        }

        if (backward) {
            myprintf("backward ");
        }

        if (candidate) {
            myprintf("candidate (%d) ",scale(7 - Rank(it), PAWN_CANDIDATE_MIN, PAWN_CANDIDATE_MAX));
        }

        if (openfile) {
            myprintf("on open file ");
        }

        myprintf("\n");
#endif

        if (Rank(it) >= Rank6) {
            /*
                check for strong squares
                square is supported by this
                pawn and there are no attacking pawns
            */
            if (i > FileA) {

                /* 
                    left up
                */
                newsq = it - 9;
                newfile = File(newsq);

                if (!(Mask[newsq] & (WhitePawns | BlackPawns))) {
                    /* 
                        can't be attacked? 
                    */
                    if (newfile == FileA) {
                        side_mask = FileMask[FileB];
                    } else if (File(newfile) == FileH) {
                        side_mask = FileMask[FileG];
                    } else {
                        side_mask = FileMask[newfile - 1] | FileMask[newfile + 1];
                    }

                    /*
                        no enemy pawns in side files above us
                    */
                    if (!(side_mask & AboveMask[Rank(newsq)] & BlackPawns))
                    {
#ifdef PAWN_EVAL_DEBUG
                        myprintf("WStrong square %s\n",sq_name[newsq]);
#endif
                        if (pawntt->w_strong_square & Mask[newsq])
                        {
#ifdef PAWN_EVAL_DEBUG
                            myprintf("WSuperStrong square %s\n",sq_name[newsq]);
#endif
                            pawntt->w_super_strong_square |= Mask[newsq];
                        } else {
                            pawntt->w_strong_square |= Mask[newsq];
                        }
                    }
                }
            }

            if (i < FileH) {
                /* 
                    left up
                */
                newsq = it - 7;
                newfile = File(newsq);

                if (!(Mask[newsq] & (WhitePawns | BlackPawns))) {
                    /* 
                        can't be attacked? 
                    */
                    if (newfile == FileA) {
                        side_mask = FileMask[FileB];
                    } else if (File(newfile) == FileH) {
                        side_mask = FileMask[FileG];
                    } else {
                        side_mask = FileMask[newfile - 1] | FileMask[newfile + 1];
                    }

                    /*
                        no enemy pawns in side files above us
                    */
                    if (!(side_mask & AboveMask[Rank(newsq)] & BlackPawns))
                    {
#ifdef PAWN_EVAL_DEBUG
                        myprintf("WStrong square %s\n",sq_name[newsq]);
#endif
                        if (pawntt->w_strong_square & Mask[newsq])
                        {
#ifdef PAWN_EVAL_DEBUG
                            myprintf("WSuperStrong square %s\n",sq_name[newsq]);
#endif
                            pawntt->w_super_strong_square |= Mask[newsq];
                        } else {
                            pawntt->w_strong_square |= Mask[newsq];
                        }
                    }
                }
            }
        }
    }

    /*
        Black loop
    */
    temp = BlackPawns;
    while (temp) {

        /*
            whereami
        */
        it = FindFirstRemove(&temp);
        i = File(it);

        /*
            init status
        */
        doubled = FALSE;
        isolated = FALSE;
        openfile = FALSE;
        backward = FALSE;
        passed = FALSE;
        candidate = FALSE;
        trailer = FALSE;

        /*
            mask this file
        */
        this_file = FileMask[i] & BlackPawns;

        /*
            determine mask for side files
        */
        if (i == FileA) {
            side_mask = FileMask[FileB];
        } else if (i == FileH) {
            side_mask = FileMask[FileG];
        } else {
            side_mask = FileMask[i - 1] | FileMask[i + 1];
        }

        /*
            open file
        */
        if (!(FileDownMask[it] & WhitePawns)) {
            openfile = TRUE;
        }       

        /* 
            doubled 
        */
        if (PopCount(this_file) > 1) {
            doubled = TRUE;

            if (this_file & BelowMask[Rank(it)]) {
                trailer = TRUE;
            }
        }

        /*
            isolated
        */
        if (!(side_mask & BlackPawns)) {
            isolated = TRUE;
        } else if (!(BlackPawns & side_mask & AboveMask[Rank(it) + 1])) {

            /* 
                backward?                        
                no supporters behind
                assume the worst
            */
            backward = TRUE;

            /* 
                precalculate some masks
            */
            urank_mask = RankMask[Rank(it) + 1];
            if (Rank(it) < Rank2) {
                urank2_mask = urank_mask | RankMask[Rank(it) + 2];
            }

            if (Rank(it) < Rank3) {
                urank3_mask = urank2_mask | RankMask[Rank(it) + 3];
            }

            /*                    
                can we join supporter forward?
            */
            if (BlackPawns & side_mask & urank_mask) {
                /*
                    are we in a vulnerable position?
                */
                if (FileMask[i] & urank_mask & (BlackPawns | WhitePawns)) {
                    /* 
                        someone above us, so no 
                    */
                    backward = FALSE;
                }

                if (Rank(it) < Rank2) {
                    if (!(side_mask & urank2_mask & WhitePawns)) {
                        /*
                            no attackers, so no
                        */
                        backward = FALSE;
                    }
                } else {
                    /*
                        too far to attack
                    */
                    backward = FALSE;
                }
            } else if ((Rank(it) == Rank7) && (BlackPawns & side_mask & urank2_mask)) {
                /*
                    we can move up 2 if needed
                */
                if (FileMask[i] & urank2_mask & (BlackPawns | WhitePawns)) {
                    backward = FALSE;
                }

                if (!(side_mask & urank3_mask & BlackPawns)) {

                    backward = FALSE;
                }
            }
        }

        if (openfile) {
            /*
                passed
            */
            if (!(BelowMask[Rank(it)] & side_mask & WhitePawns)) {
                passed = TRUE;
                pawntt->b_passed |= Mask[it];
            } else {
                /*
                    candidate passed                        
                */
                supporters = PopCount(BlackPawns & side_mask & AboveMask[Rank(it) + 1]);
                blockers = PopCount(WhitePawns & side_mask & BelowMask[Rank(it)]);

                /*
                    remove supporters that can't really support us
                    because they are blocked themselves
                */
                supporters -= PopCount(WhitePawns & side_mask & AboveMask[Rank(it) + 1]);

                /*
                    as much supporters as blockers?
                */
                attks = supporters - blockers;

                if (attks >= 0) {

                    /*
                        defended now?
                    */
                    attks = PopCount(BlackPawns & side_mask & RankMask[Rank(it) - 1]);
                    attks -= PopCount(WhitePawns & side_mask & RankMask[Rank(it) + 1]);

                    if (attks >= 0) {
                        candidate = TRUE;
                    }
                }
            }
        }

        /*
            we doublecount here, so /2
        */
        if (doubled) {            
            if (openfile) {
                pawntt->b_score -= -10;
            } else {
                pawntt->b_score -=  -5;
            }
        }

        if (isolated) {
            if (openfile) {
                pawntt->b_score -= -20;
            } else {
                pawntt->b_score -=  -8;
            }
        } else if (backward) {
            if (openfile) {
                pawntt->b_score -= -16;
            } else {
                pawntt->b_score -=  -6;
            }
        }

        if (candidate) {
            if (!trailer) {
                pawntt->b_score -= w_candidate[(7 - Rank(it)) - 1] * 2;
            } else {
                pawntt->b_score -= w_candidate[(7 - Rank(it)) - 1];
            }
        }

#ifdef PAWN_EVAL_DEBUG
        myprintf("Pawn %s: ",sq_name[it]);
        if (doubled) {
            myprintf("doubled ");
        }

        if (trailer) {
            myprintf("trailer ");
        }

        if (isolated) {
            myprintf("isolated ");
        }

        if (backward) {
            myprintf("backward ");
        }

        if (candidate) {
            myprintf("candidate (%d) ",scale(Rank(it), PAWN_CANDIDATE_MIN, PAWN_CANDIDATE_MAX));
        }

        if (openfile) {
            myprintf("on open file ");
        }

        myprintf("\n");
#endif

        if (Rank(it) <= Rank3) {

            /*
                check for strong squares
                square is supported by this
                pawn and there are no attacking pawns
            */
            if (i > FileA) {

                /* 
                    left up
                */
                newsq = it + 7;
                newfile = File(newsq);

                if (!(Mask[newsq] & (WhitePawns | BlackPawns))) {

                    /* 
                        can't be attacked? 
                    */
                    if (newfile == FileA) {
                        side_mask = FileMask[FileB];
                    } else if (File(newfile) == FileH) {
                        side_mask = FileMask[FileG];
                    } else {
                        side_mask = FileMask[newfile - 1] | FileMask[newfile + 1];
                    }

                    /*
                        no enemy pawns in side files above us
                    */
                    if (!(side_mask & BelowMask[Rank(newsq)] & WhitePawns))
                    {
#ifdef PAWN_EVAL_DEBUG
                        myprintf("BStrong square %s\n",sq_name[newsq]);
#endif
                        if (pawntt->b_strong_square & Mask[newsq])
                        {
#ifdef PAWN_EVAL_DEBUG
                            myprintf("BSuperStrong square %s\n",sq_name[newsq]);
#endif
                            pawntt->b_super_strong_square |= Mask[newsq];
                        } else {
                            pawntt->b_strong_square |= Mask[newsq];
                        }
                    }
                }
            }

            if (i < FileH) {

                /* 
                    left up
                */
                newsq = it + 9;
                newfile = File(newsq);

                if (!(Mask[newsq] & (WhitePawns | BlackPawns))) {

                    /* 
                        can't be attacked? 
                    */
                    if (newfile == FileA) {
                        side_mask = FileMask[FileB];
                    } else if (File(newfile) == FileH) {
                        side_mask = FileMask[FileG];
                    } else {
                        side_mask = FileMask[newfile - 1] | FileMask[newfile + 1];
                    }

                    /*
                        no enemy pawns in side files above us
                    */
                    if (!(side_mask & BelowMask[Rank(newsq)] & WhitePawns))
                    {
#ifdef PAWN_EVAL_DEBUG
                        myprintf("BStrong square %s\n",sq_name[newsq]);
#endif
                        if (pawntt->b_strong_square & Mask[newsq])
                        {
#ifdef PAWN_EVAL_DEBUG
                            myprintf("BSuperStrong square %s\n",sq_name[newsq]);
#endif
                            pawntt->b_super_strong_square |= Mask[newsq];
                        } else {
                            pawntt->b_strong_square |= Mask[newsq];
                        }
                    }
                }
            }
        }
    }

    /*
        only interesting squares are really strong squares
    */
    pawntt->w_strong_square &= WhiteStrongSquareMask;
    pawntt->b_strong_square &= BlackStrongSquareMask;
    pawntt->w_super_strong_square &= WhiteStrongSquareMask;
    pawntt->b_super_strong_square &= BlackStrongSquareMask;

    /*
        don't double count strong squares
    */
    pawntt->w_strong_square ^= pawntt->w_super_strong_square;
    pawntt->b_strong_square ^= pawntt->b_super_strong_square;    
}

pawntt_t *static_pawn_eval(state_t *s) {
    pawntt_t *pawntt;
    int index;        

    index = ((unsigned int)s->pawnhash) & ((1 << PAWN_HASH_LOG) - 1);
    pawntt = &PawnTT[s->threadid][index];
    
    if (pawntt->pawnhash != s->pawnhash) {            
        full_pawn_eval(s, pawntt);
        pawntt->pawnhash = s->pawnhash;
    }        

    return pawntt;
}
