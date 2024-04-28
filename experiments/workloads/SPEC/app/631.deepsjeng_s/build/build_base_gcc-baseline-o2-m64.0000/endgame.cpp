/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2007 Gian-Carlo Pascutto

    File: endgame.c
    Purpose: contains functions related to endgame draws

*/

#include "sjeng.h"
#include "extvars.h"
#include "board.h"
#include "bitboard.h"
#include "bits.h"
#include "squares.h"

int drawn_endgames(state_t *s, int score, const int wmat, const int bmat) {
    int *npieces       = s->npieces;
    BITBOARD *BitBoard = s->BitBoard;
    int wking_loc      = s->wking_loc;
    int bking_loc      = s->bking_loc;
    int white_to_move  = s->white_to_move;

    /* no more pawns */
    if (!npieces[wpawn] && !npieces[bpawn]) {
        /* no pawns, side with advantage attempts mate */
        if (wmat > bmat) {
            score += 120 - 15 * taxicab_dist(wking_loc, bking_loc);
        } else if (bmat > wmat) {
            score -= 120 - 15 * taxicab_dist(wking_loc, bking_loc);
        }
        /* one minor piece up, no pawns */
        if ((npieces[wrook] + (npieces[wqueen] * 2)) 
             == 
            (npieces[brook] + (npieces[bqueen] * 2))) {             
            if (((npieces[wknight] + npieces[wbishop]) - (npieces[bknight] + npieces[bbishop])) == 1) {            
                /* 2 bishops vs 1 knight is exception */
                if (npieces[wbishop] != 2 || npieces[bknight] != 1) {
                    if (score > 0) {
                        score >>= 3;
                    }
                }
            }
            if (((npieces[bknight] + npieces[bbishop]) - (npieces[wknight] + npieces[wbishop])) == 1) {            
                if (npieces[bbishop] != 2 || npieces[wknight] != 1) {
                    if (score < 0) {
                        score >>= 3;
                    }
                }
            }
        }

        /* no pawns and no heavies */
        if (!npieces[wrook] && !npieces[brook] && !npieces[wqueen] && !npieces[bqueen]) {
            if (!npieces[bbishop] && !npieces[wbishop]) {
                /* only knights */
                /* it pretty safe to say this is a draw */
                if (npieces[wknight] < 3 && npieces[bknight] < 3) {
                    score = 0;
                }
            } else if (!npieces[wknight] && !npieces[bknight]) {
                /* only bishops */
                /* not a draw if one side two other side zero
                else its always a draw                     */
                if (abs(npieces[wbishop] - npieces[bbishop]) < 2) {
                    score = 0;
                }
            } else if ((npieces[wknight] < 3 && !npieces[wbishop]) || (npieces[wbishop] == 1 && !npieces[wknight])) {
                /* we cant win, but can black? */
                if ((npieces[bknight] < 3 && !npieces[bbishop]) || (npieces[bbishop] == 1 && !npieces[bknight])) {
                    /* guess not */
                    score = 0;
                }
            }

            if (!npieces[bbishop] && !npieces[bknight]) {
                if (npieces[wbishop] == 1 && npieces[wknight] == 1) {
                    if (WhiteBishops & BlackSqMask) {
                        if (sum_dist(a1, bking_loc) < sum_dist(h8, bking_loc)) {
                            score += 240 - 30 * sum_dist(a1, bking_loc);
                        } else {
                            score += 240 - 30 * sum_dist(h8, bking_loc);
                        }
                    } else {
                        if (sum_dist(a8,bking_loc) < sum_dist(h1,bking_loc)) {
                            score += 240 - 30 * sum_dist(a8,bking_loc);
                        } else {
                            score += 240 - 30 * sum_dist(h1,bking_loc);
                        }
                    }
                }
            } else if (!npieces[wbishop] && !npieces[wknight]) {
                if (npieces[bbishop] == 1 && npieces[bknight] == 1) {
                    if (BlackBishops & BlackSqMask) {
                        if (sum_dist(a1,wking_loc) < sum_dist(h8,wking_loc)) {
                            score -= 240 - 30 * sum_dist(a1,wking_loc);
                        } else {
                            score -= 240 - 30 * sum_dist(h8,wking_loc);
                        }
                    } else {

                        if (sum_dist(a8,wking_loc) < sum_dist(h1,wking_loc)) {
                            score -= 240 - 30 * sum_dist(a8,wking_loc);
                        } else {
                            score -= 240 - 30 * sum_dist(h1,wking_loc);
                        }
                    }
                }
            } 
        } else if (!npieces[wqueen] && !npieces[bqueen]) {
            /* pawns and rooks */
            if (npieces[wrook] == 1 && !npieces[brook]) {
                /* one rook */
                /* draw if no minors to support AND minors to defend  */
                if ((npieces[wknight] + npieces[wbishop] == 0)
                && (((npieces[bknight] + npieces[bbishop]) == 1) || ((npieces[bknight] + npieces[bbishop]) == 2))) {
                    score >>= 3;

                    /* Keep defender close so as not to get seperated */
                    if (!(KingSafetyMask[bking_loc] & (BlackKnights | BlackBishops))) {
                        score += 20;
                    }
                } else if (((npieces[bbishop] + npieces[bknight]) - (npieces[wbishop] + npieces[wknight])) == 1) {
                    if (score > 0) {
                        score >>= 2;
                    }
                }
            } else if (npieces[brook] == 1 && !npieces[wrook]) {
                /* one rook */
                /* draw if no minors to support AND
                minors to defend  */
                if ((npieces[bknight] + npieces[bbishop] == 0)
                && (((npieces[wknight] + npieces[wbishop]) == 1) || ((npieces[wknight] + npieces[wbishop]) == 2))) {
                    score >>= 3;

                    /* Keep defender close so as not to get seperated */
                    if (!(KingSafetyMask[wking_loc] & (WhiteKnights | WhiteBishops))) {
                        score -= 20;
                    }
                } else if (((npieces[wbishop] + npieces[wknight]) - (npieces[bbishop] + npieces[bknight])) == 1) {
                    if (score < 0) {
                        score >>= 2;
                    }
                }
            } else {
                /* 2 vs 1 rooks */
                if (npieces[wrook] != npieces[brook]) {                    
                    /* 'only' exchange without pawns */
                    if (((npieces[wrook]) - (npieces[brook])) ==
                         ((npieces[bbishop] + npieces[bknight]) - (npieces[wbishop] + npieces[wknight]))) {
                        score >>= 2;
                    }
                }
            }
        } else {
            /* unequal heavies */
            if (((npieces[wqueen] * 2) + npieces[wrook]) != ((npieces[bqueen]* 2) + npieces[brook])) {                
                /* 'only' exchange without pawns */
                if ((((npieces[wqueen] * 2) + npieces[wrook]) - ((npieces[bqueen] * 2) + npieces[brook])) ==
                        ((npieces[bbishop] + npieces[bknight]) - (npieces[wbishop] + npieces[wknight]))) {
                    if (npieces[wpawn] == 0) {
                        if (score > 0) {
                            score >>= 2;
                        }
                    }
                    if (npieces[bpawn] == 0) {
                        if (score < 0) {
                            score >>= 2;
                        }
                    }
                }
            }
        }
    } else if (!npieces[wpawn]) {
        /* can he still win ? */
        if (!npieces[wrook] && !npieces[wqueen]) {
            if (npieces[wbishop] == 1 && !npieces[wknight]) {
                if (score > 0) {
                    score = 0;
                }
            } else if (npieces[wknight] == 1 && !npieces[wbishop]) {
                if (score > 0) {
                    score = 0;
                }
            }
        }

        /* RB vs RP drawn for RB */
        if (wmat == 10 && bmat == 6 && (npieces[bpawn] >= 1)) {
            if (score > 0) {
                score >>= 2;
            }
        }
        
        /* 2 knights is not so good */
        if (wmat == 8 && !npieces[wbishop]) {
            if (score > 0) {
                score >>= 3;
            }
        }

        if (wmat <= 8 && (wmat - bmat <= 4)) {
            if (score > 0) {
                score >>= 3;
            }
        } /* no black pawns, black is 1 minor up */ 
        else if (wmat <= 16 && wmat > bmat && ((wmat - bmat) <= 4)) {
            if (score > 0) {
                score >>= 1;
            }
        }

        /* RP vs R */
        if (npieces[bpawn] == 1 && bmat == 6 && wmat == 6) {
            int square1 = FindLast(BlackPawns);

            if (abs(file(wking_loc) - file(square1)) <= 1 && rank(wking_loc) < rank(square1)) {
                score >>= 1;
            } else if (rank(bking_loc) > rank(square1) || abs(file(bking_loc) - file(square1)) > 1) {
                score >>= 1;
            }
        }
    } else if (!npieces[bpawn]) {
        /* can he still win ? */
        if (!npieces[brook] && !npieces[bqueen]) {
            if (npieces[bbishop] == 1 && !npieces[bknight]) {
                if (score < 0) {
                    score = 0;
                }
            } else if (npieces[bknight] == 1 && !npieces[bbishop]) {
                if (score < 0) {
                    score = 0;
                }
            }
        }

        if (bmat == 10 && wmat == 6 && (npieces[wpawn] >= 1)) {
            if (score < 0) {
                score >>= 2;
            }
        }
        
        /* 2 knights is not so good */
        if (bmat == 8 && !npieces[bbishop]) {
            if (score < 0) {
                score >>= 3;
            }
        }

        if (bmat <= 8 && (bmat - wmat <= 4)) {
            if (score < 0) {
                score >>= 3;
            }
        } /* no black pawns, black is 1 minor up */ 
        else if (bmat <= 16 && bmat > wmat && ((bmat - wmat) <= 4)) {
            if (score < 0) {
                score >>= 1;
            }
        }

        if (npieces[wpawn] == 1 && wmat == 6 && bmat == 6) {
            int square1 = FindFirst(WhitePawns);

            if (abs(file(bking_loc) - file(square1)) <= 1 && rank(bking_loc) > rank(square1)) {
                score >>= 1;
            } else if (rank(wking_loc) < rank(square1) || abs(file(wking_loc) - file(square1)) > 1) {
                score >>= 1;
            }
        }
    }

    /* detect wrong rook pawn */
    if (wmat == 4 && npieces[wbishop] == 1 && npieces[wpawn] == 1) {
        /* pawn is a rook pawn? */
        if (WhitePawns & (FileMask[FileH] | FileMask[FileA])) {
            if ((WhitePawns & FileMask[FileH]) && !(BlackPawns & FileMask[FileG])) {
                /* good bishop? */
                if (!(WhiteBishops & BlackSqMask)) {
                    /* no, who gets there first? */
                    int bkrun = taxicab_dist(h8, bking_loc);

                    /* not adjacent? */
                    if (bkrun > 1) {
                        int wkrun = taxicab_dist(h8, wking_loc);
                        int prun = taxicab_dist(h8, FindFirst(WhitePawns));

                        if (bkrun <= (wkrun - white_to_move) && (bkrun - 1) <= (prun - white_to_move)) {
                            if (score > 0) {
                                score = 0;
                            }
                        }
                    } else {
                        if (score > 0) {
                            score = 0;
                        }
                    }
                }
            } else if ((WhitePawns & FileMask[FileA]) && !(BlackPawns & FileMask[FileB])) {
                /* good bishop? */
                if (!(WhiteBishops & WhiteSqMask)) {
                    /* no, who gets there first? */
                    int bkrun = taxicab_dist(a8, bking_loc);

                    /* not adjedacent? */
                    if (bkrun > 1) {
                        int wkrun = taxicab_dist(a8, wking_loc);
                        int prun = taxicab_dist(a8, FindFirst(WhitePawns));

                        if (bkrun <= (wkrun - white_to_move) && (bkrun - 1) <= (prun - white_to_move)) {
                            if (score > 0) {
                                score = 0;
                            }
                        }
                    } else {
                        if (score > 0) {
                            score = 0;
                        }
                    }
                }
            }
        }
    } else if (bmat == 4 && npieces[bbishop] == 1 && npieces[bpawn] == 1) {
        /* pawn is a rook pawn? */
        if (BlackPawns & (FileMask[FileH] | FileMask[FileA])) {
            if ((BlackPawns & FileMask[FileH]) && !(WhitePawns & FileMask[FileG])) {

                /* good bishop? */
                if (!(BlackBishops & WhiteSqMask)) {

                    /* no, who gets there first? */
                    int wkrun = taxicab_dist(h1, wking_loc);

                    /* not adjacent? */
                    if (wkrun > 1) {
                        int bkrun = taxicab_dist(h1, bking_loc);
                        int prun = taxicab_dist(h1, FindLast(BlackPawns));

                        if (wkrun <= (bkrun + white_to_move) && (wkrun - 1) <= (prun + white_to_move)) {
                            if (score < 0) {
                                score = 0;
                            }
                        }
                    } else {
                        if (score < 0) {
                            score = 0;
                        }
                    }
                }
            } else if ((BlackPawns & FileMask[FileA]) && !(WhitePawns & FileMask[FileB])) {
                /* good bishop? */
                if (!(BlackBishops & BlackSqMask)) {

                    /* no, who gets there first? */
                    int wkrun = taxicab_dist(a1, wking_loc);

                    /* not adjacent? */
                    if (wkrun > 1) {
                        int bkrun = taxicab_dist(a1, bking_loc);
                        int prun = taxicab_dist(a1, FindLast(BlackPawns));

                        if (wkrun <= (bkrun + white_to_move) && (wkrun - 1) <= (prun + white_to_move)) {
                            if (score < 0) {
                                score = 0;
                            }
                        }
                    } else {
                        if (score < 0) {
                            score = 0;
                        }
                    }
                }
            }
        }
    }
    
    /* kpk endgame */
    if (wmat == 0 && bmat == 0 && npieces[wpawn] == 1) {
        /* rook pawns are the hardest */
        if (WhitePawns & FileMask[FileH]) {
            /* black on a key square draws */
            if (BlackKing & (Mask[h8] | Mask[h7] | Mask[g7] | Mask[g8] | Mask[f8] | Mask[h6])) {
                score >>= 2;
            }
        } else if (WhitePawns & FileMask[FileA]) {
            if (BlackKing & (Mask[a8] | Mask[a7] | Mask[b7] | Mask[b8] | Mask[c8] | Mask[a6])) {
                score >>= 2;
            }
        }    
        score >>= 1;    
    }
    if (wmat == 0 && bmat == 0 && npieces[bpawn] == 1) {
        /* rook pawns are the hardest */
        if (BlackPawns & FileMask[FileH]) {
            /* black on a key square draws */
            if (WhiteKing & (Mask[h1] | Mask[h2] | Mask[g1] | Mask[g2] | Mask[f1] | Mask[h3])) {
                score >>= 2;
            }
        } else if (BlackPawns & FileMask[FileA]) {
            if (WhiteKing & (Mask[a1] | Mask[a2] | Mask[b1] | Mask[b2] | Mask[c1] | Mask[a3])) {
                score >>= 2;        
            }
        }         
        score >>= 1;
    }    
    
    return score;
}
