/*
    Sjeng - a chess playing program
    Copyright (C) 2001-2005 Gian-Carlo Pascutto    

    File: see.c                                             
    Purpose: do static exchange evaluation                      
 
*/
#include "sjeng.h"
#include "extvars.h"
#include "bitboard.h"
#include "bits.h"
#include "evpar.h"
#include "attacks.h"
#include "neval.h"
#include "utils.h"
#include "squares.h"

int see(state_t *s, const int color, const int from, const int to, const int promoted) {
    BITBOARD attacks, tmp;
    BITBOARD origAll, origBB, XRayR, XRayB;
    int captured[32];
    int p, np;
    int side;
    int num_captures;
    int score;
    int ptype;
    BITBOARD *BitBoard = s->BitBoard;

    num_captures = 1;
    side = color;

    ptype = s->sboard[from];

    /*
        remove piece from the board temporarily
    */
    origAll = s->All;
    origBB  = BitBoard[ptype];

    s->All          ^= Mask[from];
    BitBoard[ptype] ^= Mask[from];

    /*
        setup all attackers on to square 
    */
    attacks = attacks_to(s, to);

    /*
        perform the initiating capture (or promotion) 
    */
    /* add to capture stack */
    captured[0] = abs(material[s->sboard[to]]);       

    /* set next piece value */
    if (!promoted) {
        p = abs(material[ptype]);        
    } else {
        /* promotion gain is new piece minus pawn value */
        p = abs(material[promoted]) - MAT_PAWN;
    }  
    
    XRayR = (WhiteQueens | BlackQueens | WhiteRooks | BlackRooks);
    XRayB = (WhiteQueens | BlackQueens | WhiteBishops | BlackBishops);

    side ^= 1;

    while (attacks) {
        /* 
           always capture with the least valuable piece first 
        */
        if (side == WHITE) {
            tmp = WhitePawns & attacks;
            if (tmp) {
                np = MAT_PAWN;
            } else {
                tmp = WhiteKnights & attacks;
                if (tmp) {
                    np = MAT_KNIGHT;
                } else {
                    tmp = WhiteBishops & attacks;
                    if (tmp) {
                        np = MAT_BISHOP;
                    } else {
                        tmp = WhiteRooks & attacks;
                        if (tmp) {
                            np = MAT_ROOK;
                        } else {
                            tmp = WhiteQueens & attacks;
                            if (tmp) {
                                np = MAT_QUEEN;
                            } else {
                                tmp = WhiteKing & attacks;
                                if (!tmp) {
                                    break;
                                }
                                np = INF;
                            }
                        }
                    }
                }
            }
        } else {
            tmp = BlackPawns & attacks;
            if (tmp) {
                np = MAT_PAWN;
            } else {
                tmp = BlackKnights & attacks;
                if (tmp) {
                    np = MAT_KNIGHT;
                } else {
                    tmp = BlackBishops & attacks;
                    if (tmp) {
                        np = MAT_BISHOP;
                    } else {
                        tmp = BlackRooks & attacks;
                        if (tmp) {
                            np = MAT_ROOK;
                        } else {
                            tmp = BlackQueens & attacks;
                            if (tmp) {
                                np = MAT_QUEEN;
                            } else {
                                tmp = BlackKing & attacks;
                                if (!tmp) {
                                    break;
                                }
                                np = INF;
                            }
                        }
                    }
                }
            }
        }

        /* update capture stack */
        /* the value here is the material balance at this point */
        captured[num_captures] = -captured[num_captures - 1] + p;
        num_captures++;

        /* set up new piece value */
        p = np;

        /* find attacker square */     
        tmp &= 0-tmp; 

        /* remove from attack board */        
        s->All ^= tmp;               
        
        /* check for new xray attackers */  
        if (XRayR & RookMask[to]) {      
            attacks |= RookAttacks(s, to)   & XRayR;
        }
        if (XRayB & BishopMask[to]) {            
            attacks |= BishopAttacks(s, to) & XRayB;       
        }            
        attacks &= s->All;

        side ^= 1;
    }

    /* we start off the back of the capture sequence */
    num_captures--;
    score = captured[num_captures];

    while (num_captures > 0) {
        /* capturing on is better */
        if (score > -captured[num_captures - 1]) {
            score = -score;
        } else {
            /* the capture sequence is bad from this point on */
            score = captured[num_captures - 1];
        }

        num_captures--;
    }   
    
    /*
        Restore the board
    */
    s->All          = origAll;
    BitBoard[ptype] = origBB;

    if (promoted) {
        return score + abs(material[promoted]) - MAT_PAWN;
    } else {
        return score;
    }
}
