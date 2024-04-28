/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2005 Gian-Carlo Pascutto    

    File: make.c                                        
    Purpose: making and unmaking moves

*/
#include <assert.h>
#include "sjeng.h"
#include "bitboard.h"
#include "extvars.h"
#include "squares.h"
#include "preproc.h"
#include "state.h"
#include "ttable.h"
#include "utils.h"

void make(state_t * const s, const move_s move) {
    int ep, from, target, captured, promoted, castled;    
    int ep_target;
    int mvpiece;
    int rook_start, rook_end;    
    BITBOARD M, MIM;
    
    /* 
        processing aides 
    */          
    
    from = FROM(move);
    target = TARGET(move);
    promoted = PROMOTED(move);
    captured = CAPTURED(move);
    ep = EP(move);
    castled = CASTLED(move);
    
    /* DEBUG */
    assert(s->sboard[from] != npiece);   
    assert((captured == npiece) || s->sboard[target] == captured || ep);
    assert(captured != 0);
    assert(Mask[from] & s->BitBoard[s->sboard[from]]);
    assert(ep || (captured == npiece) || (s->BitBoard[captured] & Mask[target]));

    /* 
        clear the en-passant rights: 
    */
    s->path_x[s->ply].epsq = s->ep_square;
    if (s->ep_square) {
        //  HASH(0, nobit_to_bit[ep_square]);
        s->ep_square = 0;
    }

    /* 
        update the 50 move info: 
    */
    s->path_x[s->ply].fifty = s->fifty;
    
    /*
        save castling info
    */        
    s->path_x[s->ply].castleflag = s->castleflag;
    
    s->path_x[s->ply].psq_score = s->psq_score;

    if (PIECET(s,from) == pawn || (captured != npiece)) {
        s->fifty = 0;
    } else {
        s->fifty++;
    }

    /* 
        store bitboards and hash 
    */
    s->path_x[s->ply].hash = s->hash;
    s->path_x[s->ply].pawnhash = s->pawnhash;  

    /*
        helper masks
    */
    M = Mask[target];
    MIM = Mask[from] | M;

    /* 
        Remove captured piece from
        count, hash, bitboards        
    */
    if (!ep) {
        /* normal capture? */
        if (captured != npiece) {
            s->BitBoard[captured] ^= M;

            if (PIECECOLOR(captured) == WHITE) {
                s->WhitePieces ^= M;
            } else {
                s->BlackPieces ^= M;
            }

            REMOVEMATERIAL(s, captured);
            s->npieces[captured]--;
            
            s->psq_score -= psq_table[captured - 1][target];    

            HASH(s, captured, target);
            if (PIECETYPE(captured) == pawn) {
                PAWNHASH(s, captured, target);
            }            
        }
    } else {
        /* ep capture
           need to clean ep-target square too,
           since unlike normal capture its empty now
        */
        if (s->sboard[from] == bpawn) {
            ep_target = target - 8;            

            s->WhitePieces &= InvMask[ep_target];
            s->WhitePawns  &= InvMask[ep_target];

            HASH(s, wpawn, ep_target);
            PAWNHASH(s, wpawn, ep_target);

            REMOVEMATERIAL(s, wpawn);
            s->npieces[wpawn]--;
        } else {
            ep_target = target + 8;            

            s->BlackPieces &= InvMask[ep_target];
            s->BlackPawns  &= InvMask[ep_target];

            HASH(s, bpawn, ep_target);
            PAWNHASH(s, bpawn, ep_target);

            REMOVEMATERIAL(s, bpawn);
            s->npieces[bpawn]--;
        }
        
        s->psq_score -= psq_table[s->sboard[ep_target] - 1][ep_target];
        s->sboard[ep_target] = npiece;                        
    }

    /*
        leave from square
    */    
    mvpiece = s->sboard[from];
    s->sboard[from] = npiece;                
    s->sboard[target] = mvpiece;
    
    assert(s->BitBoard[mvpiece] & Mask[from]);
    
    s->psq_score -= psq_table[mvpiece - 1][from];    
    s->psq_score += psq_table[mvpiece - 1][target];
                
    if (PIECETYPE(mvpiece) == pawn) {
        PAWNHASH(s, mvpiece, from);
    }
    HASH(s, mvpiece, from);

    /*
        update rotated boards and piecemap
    */
    if (!castled) {
        s->BitBoard[mvpiece] ^= MIM;

        if (PIECECOLOR(mvpiece) == WHITE) {
            s->WhitePieces ^= MIM;
        } else {
            s->BlackPieces ^= MIM;
        }
    }
   
    /*
        side to move and ply adjust
    */
    s->white_to_move ^= 1;
    s->ply++;

    /* 
        check for special pawn moves
        or castling
    */
    if (mvpiece == wpawn) {
        if (promoted) {
            s->sboard[target] = promoted;
            s->psq_score -= psq_table[wpawn - 1][target];
            s->psq_score += psq_table[promoted - 1][target];
            
            HASH(s, promoted, target);

            s->npieces[wpawn]--;
            s->npieces[promoted]++;

            REMOVEMATERIAL(s, wpawn);
            ADDMATERIAL(s, promoted);

            /* fixup bitboards */
            s->BitBoard[promoted] |= M;
            s->WhitePawns ^= M;
        } else {

            /* moved a pawn up 2 squares: */
            if (target == from - 16) {
                s->ep_square = from - 8;
                // HASH(0, nobit_to_bit[ep_square]);
            }

            HASH(s, wpawn, target);
            PAWNHASH(s, wpawn, target);
        }
    } else if (mvpiece == bpawn) {
        if (promoted) {
            s->sboard[target] = promoted;
            s->psq_score -= psq_table[bpawn - 1][target];
            s->psq_score += psq_table[promoted - 1][target];
            
            HASH(s, promoted, target);

            s->npieces[promoted]++;
            s->npieces[bpawn]--;

            REMOVEMATERIAL(s, bpawn);
            ADDMATERIAL(s, promoted);

            s->BitBoard[promoted] |= M;
            s->BlackPawns ^= M;
        } else {
            if (target == from + 16) {
                s->ep_square = from + 8;

                //      HASH(0, nobit_to_bit[ep_square]);
            }

            HASH(s, bpawn, target);
            PAWNHASH(s, bpawn, target);
        }
    } else if (mvpiece != wking && mvpiece != bking) {
        /* 
            normal move 
        */
        HASH(s, mvpiece, target);
    } else if (mvpiece == wking) {
        HASH(s, wking, target);
        s->wking_loc = target;
        s->castleflag &= ~((1 << wck) | (1 << wcq));

        /* check for castling: */
        /* check for white kingside castling: */
        if (castled) {
            if (castled == wck) {
                rook_start = s->wrrook_start;
                rook_end = f1;
            } else {
                rook_start = s->wlrook_start;
                rook_end = d1;
            }
            
            s->sboard[rook_end] = wrook;            

            if (rook_start != rook_end) {
                if (rook_start != target) {
                    s->sboard[rook_start] = npiece;                   
                }                               
            }
            
            s->psq_score -= psq_table[wrook - 1][rook_start];
            s->psq_score += psq_table[wrook - 1][rook_end];

            HASH(s, wrook, rook_start);
            HASH(s, wrook, rook_end);

            s->WhiteRooks ^= Mask[rook_start];
            s->WhiteRooks ^= Mask[rook_end];

            s->WhiteKing = Mask[s->wking_loc];

            s->WhitePieces ^= CastleMask[castled - 1];
        }
    } else {
        /* if (mvpiece == bking) */
        
        HASH(s, bking, target);
        s->bking_loc = target;
        s->castleflag &= ~((1 << bck) | (1 << bcq));

        /* check for castling: */
        if (castled) {
            /* check for black kingside castling: */
            if (castled == bck) {
                rook_start = s->brrook_start;
                rook_end = f8;
            } else {
                rook_start = s->blrook_start;
                rook_end = d8;
            }
            
            s->sboard[rook_end] = brook;

            if (rook_start != rook_end) {
                if (rook_start != target) {
                    s->sboard[rook_start] = npiece;                   
                }                
            }
            
            s->psq_score -= psq_table[brook - 1][rook_start];
            s->psq_score += psq_table[brook - 1][rook_end];

            HASH(s, brook, rook_start);
            HASH(s, brook, rook_end);

            s->BlackRooks ^= Mask[rook_start];
            s->BlackRooks ^= Mask[rook_end];

            s->BlackKing = Mask[s->bking_loc];

            s->BlackPieces ^= CastleMask[castled - 1];
        }
    }        
    
    if (from == s->brrook_start || target == s->brrook_start) {
        s->castleflag &= ~(1 << bck);
    } 
    if (from == s->blrook_start || target == s->blrook_start) {
        s->castleflag &= ~(1 << bcq);
    }
    if (from == s->wrrook_start || target == s->wrrook_start) {
        s->castleflag &= ~(1 << wck);
    } 
    if (from == s->wlrook_start || target == s->wlrook_start) {
        s->castleflag &= ~(1 << wcq);
    }    

    s->All = s->WhitePieces | s->BlackPieces;

    assert(s->sboard[from] == npiece);    
    assert(!(s->All & Mask[from]));
    assert(s->sboard[target] != npiece);    
    assert(s->All & Mask[target]);   
    assert(!(s->BlackPawns & s->WhitePieces));
    assert(!(s->WhitePawns & s->BlackPieces));
        
    return;
}

void unmake(state_t * const s, const move_s move) {
    int ep, from, target, captured, promoted, castled;
    int ep_target;    
    int tpiece;
    int rook_start, rook_end;
    BITBOARD M, MIM;
    
    from = FROM(move);
    target = TARGET(move);
    promoted = PROMOTED(move);
    captured = CAPTURED(move);
    ep = EP(move);
    castled = CASTLED(move);
    
    assert(s->sboard[target] != npiece);
    assert(s->All & Mask[target]);
    assert(from >= a8 && from <= h1);
    assert(target >= a8 && target <= h1);
   
    M = Mask[target];
    MIM = (Mask[from] | M);

    s->white_to_move ^= 1;
    s->ply--;

    s->ep_square = s->path_x[s->ply].epsq;

    s->fifty = s->path_x[s->ply].fifty;
    s->castleflag = s->path_x[s->ply].castleflag;
    s->psq_score = s->path_x[s->ply].psq_score;
    
    s->hash = s->path_x[s->ply].hash;
    s->pawnhash = s->path_x[s->ply].pawnhash;    

    tpiece = s->sboard[target];
    s->sboard[from] = tpiece;

    if (!ep) {
        if (from != target) {
            s->sboard[target] = captured;
        }

        if (captured != npiece) {
            ADDMATERIAL(s, captured);

            s->BitBoard[captured] ^= M;

            /* captured piece color = NOT target piece color */
            if (PIECECOLOR(tpiece) == WHITE) {
                s->BlackPieces ^= M;
            } else {
                s->WhitePieces ^= M;

            }

            s->npieces[captured]++;            
        }
    } else {
        s->sboard[target] = npiece;

        if (tpiece == wpawn) {
            ep_target = target + 8;            

            s->BlackPieces |= Mask[ep_target];
            s->BlackPawns  |= Mask[ep_target];

            ADDMATERIAL(s, bpawn);
            s->npieces[bpawn]++;

            s->sboard[ep_target] = bpawn;
        } else {
            ep_target = target - 8;            

            s->WhitePieces |= Mask[ep_target];
            s->WhitePawns  |= Mask[ep_target];

            ADDMATERIAL(s, wpawn);
            s->npieces[wpawn]++;

            s->sboard[ep_target] = wpawn;
        }          
    }

    if (!castled) {
        s->BitBoard[tpiece] ^= MIM;

        if (PIECECOLOR(tpiece) == WHITE) {
            s->WhitePieces ^= MIM;
        } else {
            s->BlackPieces ^= MIM;
        }
    }

    if (promoted) {
        /* white promotions: */
        if (PIECECOLOR(tpiece) == WHITE) {
            s->sboard[from] = wpawn;

            s->npieces[promoted]--;
            s->npieces[wpawn]++;

            REMOVEMATERIAL(s, promoted);
            ADDMATERIAL(s, wpawn);

            s->WhitePawns |= Mask[from];
            s->BitBoard[promoted] ^= Mask[from];
        } else {
            s->sboard[from] = bpawn;

            s->npieces[promoted]--;
            s->npieces[bpawn]++;

            REMOVEMATERIAL(s, promoted);
            ADDMATERIAL(s, bpawn);

            s->BlackPawns |= Mask[from];
            s->BitBoard[promoted] ^= Mask[from];
        }
    } else if (tpiece == wking) {

        s->wking_loc = from;

        s->WhiteKing = Mask[s->wking_loc];

        /* check for castling: */
        if (castled) {
            /* check for white kingside castling: */
            if (castled == wck) {
                rook_start = f1;
                rook_end = s->wrrook_start;
            } else {
                rook_start = d1;
                rook_end = s->wlrook_start;
            }

            s->sboard[rook_end] = wrook;

            if (rook_start != rook_end) {
                if (rook_start != from) {                    
                    s->sboard[rook_start] = npiece;                    
                }            
            }

            s->WhiteRooks ^= Mask[rook_start];
            s->WhiteRooks ^= Mask[rook_end];

            s->WhiteKing = Mask[s->wking_loc];

            s->WhitePieces ^= CastleMask[castled - 1];
        }
    } else if (tpiece == bking) {
        s->bking_loc = from;

        s->BlackKing = Mask[s->bking_loc];

        if (castled) {
            if (castled == bck) {
                rook_start = f8;
                rook_end = s->brrook_start;
            } else {
                rook_start = d8;
                rook_end = s->blrook_start;
            }
            
            s->sboard[rook_end] = brook;

            if (rook_start != rook_end) {
                if (rook_start != from) {                    
                    s->sboard[rook_start] = npiece;                    
                }                
            }

            s->BlackRooks ^= Mask[rook_start];
            s->BlackRooks ^= Mask[rook_end];

            s->BlackKing = Mask[s->bking_loc];

            s->BlackPieces ^= CastleMask[castled - 1];
        }
    }

    s->All = s->WhitePieces | s->BlackPieces;

    assert(s->sboard[from] != npiece);
    assert(s->All & Mask[from]);
    assert(s->sboard[target] == captured || ep);
    
    return;
}
