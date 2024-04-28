/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2009 Gian-Carlo Pascutto

    File: attacks.c                 
    Purpose: calculate attack information
*/

#include "sjeng.h"
#include "extvars.h"
#include "bitboard.h"
#include "state.h"

/* 
   check if square is attacked by !color
   return piece type if attacked 
*/
int is_attacked(state_t *s, const int sq, const int color) {    
    BITBOARD rq, bq;    
    int x, y;
    BITBOARD *BitBoard = s->BitBoard;
    BITBOARD All = s->All;
    
    if (color == WHITE) {
        if (BlackPawns & PawnAttacksWhite[sq]) 
            return bpawn;
      
        if (BlackKnights & KnightMoves[sq]) 
            return bknight;
      
        if (BlackKing & KingMoves[sq]) 
            return bking;
      
        y = Rank(sq);      
        rq = (BlackRooks | BlackQueens);       
      
        if (RankMask[y] & rq) {         
            if (rq & RankAttacks(All, sq)) {
                return brook;
            }
        }
      
        x = File(sq);      
      
        if (FileMask[x] & rq) {        
            if (rq & FileAttacks(All, sq)) {
                return brook;
            }
        }   
      
        bq = (BlackBishops | BlackQueens); 
      
        if (DiagMaska1h8[sq] & bq) {          
            if (bq & DiagonalAttacks(All, sq)) { 
                return bbishop;		
            }				 
        }
      
        if (DiagMaska8h1[sq] & bq) {         
            if (bq & AntiDiagAttacks(All, sq)) 
                return bbishop;						 
        }
    } else {      

        if (WhitePawns & PawnAttacksBlack[sq]) 
            return wpawn;

        if (WhiteKnights & KnightMoves[sq]) 
            return wknight;

        if (WhiteKing & KingMoves[sq]) 
            return wking;

        rq = (WhiteRooks | WhiteQueens);       
        y = Rank(sq);      

        if (RankMask[y] & rq) {         
            if (rq & RankAttacks(All, sq)) {
                return wrook;
            }
        }

        x = File(sq);      

        if (FileMask[x] & rq) {        
            if (rq & FileAttacks(All, sq)) {
                return wrook;
            }
        }   

        bq = (WhiteBishops | WhiteQueens); 

        if (DiagMaska1h8[sq] & bq) {          
            if (bq & DiagonalAttacks(All, sq)) { 
                return wbishop;		
            }				 
        }
      
        if (DiagMaska8h1[sq] & bq) {         
            if (bq & AntiDiagAttacks(All, sq)) {
                return wbishop;			
            }			 
        }
    }
    
    return FALSE;
}

/*
    is the king of the side to move in check?
*/
int in_check (state_t *s) {
    if (s->white_to_move) {
        return (is_attacked(s, s->wking_loc, !s->white_to_move)); 
    } else {
        return (is_attacked(s, s->bking_loc, !s->white_to_move)); 
    }
}

/* 
    generate a mask of all pieces attacking square,
    including friendly pieces    
*/ 
BITBOARD attacks_to(state_t *s, const int sq) {
    BITBOARD temp, rq, bq;    
    const int x = File(sq), y = Rank(sq);
    BITBOARD *BitBoard = s->BitBoard;
    BITBOARD All = s->All;
    
    temp  = (BlackPawns & PawnAttacksWhite[sq]);
    temp |= (WhitePawns & PawnAttacksBlack[sq]);
    temp |= ((BlackKnights | WhiteKnights) & KnightMoves[sq]);
    temp |= (BlackKing | WhiteKing) & KingMoves[sq];
    
    rq = (BlackRooks | BlackQueens | WhiteRooks | WhiteQueens); 
    bq = (BlackBishops | BlackQueens | WhiteBishops | WhiteQueens); 
    
    if (RankMask[y] & rq) {     
        temp |= (rq & RankAttacks(All, sq));
    }
    
    if (FileMask[x] & rq) {       
        temp |= (rq & FileAttacks(All, sq));
    }   
    
    if (DiagMaska1h8[sq] & bq) {       
        temp |= (bq & DiagonalAttacks(All, sq));						 
    }
    
    if (DiagMaska8h1[sq] & bq) {        
        temp |= (bq & AntiDiagAttacks(All, sq));						 
    }
    
    return temp;
}
