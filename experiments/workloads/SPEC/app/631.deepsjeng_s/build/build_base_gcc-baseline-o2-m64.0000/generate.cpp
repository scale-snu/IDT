/*
    Sjeng - a chess playing program
    Copyright (C) 2009 Gian-Carlo Pascutto    

    File: generate.c                                        
    Purpose: functions used to generate moves

*/

#include "sjeng.h"
#include "extvars.h"
#include "bitboard.h"
#include "bits.h"
#include "generate.h"
#include "board.h"
#include "moves.h"
#include "state.h"
#include "squares.h"

int gen_quiet(state_t *s, move_s *moves) {
    BITBOARD tsq, temp, friendlymask;
    int to, from;
    move_s *start = moves;
    BITBOARD *BitBoard = s->BitBoard;
    int *sboard = s->sboard;
 
    if (s->white_to_move) {
        friendlymask = ~s->All;
    } else {
        friendlymask = ~s->All;
    }
 
     /* Generate normal moves for white pawns first */
    if (s->white_to_move) {        
        tsq = (WhitePawns >> 8) & ~(s->All);        
        temp = (((WhitePawns & RankMask[Rank2]) >> 16) & ~(s->All));
        temp &= ~(s->All >> 8);
        tsq |= temp;                
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            if (WhitePawns & Mask[to+8]) from = to + 8;
            else                         from = to + 16;
            
            if (Rank(to) == Rank8) {
                moves = add_capture(moves, from, to, sboard[to], wqueen, FALSE);
                moves = add_capture(moves, from, to, sboard[to], wrook, FALSE);
                moves = add_capture(moves, from, to, sboard[to], wbishop, FALSE);
                moves = add_capture(moves, from, to, sboard[to], wknight, FALSE);
            } else {
                moves = add_move(moves, from, to, FALSE);            
            }
        }        
    } else {
        tsq = (BlackPawns << 8) & ~(s->All);        
        temp = (((BlackPawns & RankMask[Rank7]) << 16) & ~(s->All));
        temp &= ~(s->All << 8);
        tsq |= temp;                
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            if (BlackPawns & Mask[to-8]) from = to - 8;
            else                         from = to - 16;
            
            if (Rank(to) == Rank1) {
                moves = add_capture(moves, from, to, sboard[to], bqueen, FALSE);
                moves = add_capture(moves, from, to, sboard[to], brook, FALSE);
                moves = add_capture(moves, from, to, sboard[to], bbishop, FALSE);
                moves = add_capture(moves, from, to, sboard[to], bknight, FALSE);
            } else {
                moves = add_move(moves, from, to, FALSE);  
            }                      
        }
    }
    
    if (s->white_to_move) tsq = WhiteKnights;
    else                  tsq = BlackKnights;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp  = KnightMoves[from];
        temp &= friendlymask;
                
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);                                    
        }        
    }           
    
    if (s->white_to_move) tsq = WhiteBishops | WhiteQueens;
    else                  tsq = BlackBishops | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp  = BishopAttacks(s, from);  
        temp &= friendlymask;      
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);	                          
        }        
    }
    
    if (s->white_to_move) tsq = WhiteRooks | WhiteQueens;
    else                  tsq = BlackRooks | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp  = RookAttacks(s, from);              
        temp &= friendlymask;
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);
        }                        
    }   
    
    if (s->white_to_move) {
        if (s->castleflag & (1 << wck)) {                            
            if (!check_castle_blocker(s, wck)) {
                moves = add_king_castle(moves, s->wking_loc, g1, wck);
            }
        }    
        if (s->castleflag & (1 << wcq)) {                            
            if (!check_castle_blocker(s, wcq)) {
                moves = add_king_castle(moves, s->wking_loc, c1, wcq);
            }
        }
        
        from = s->wking_loc;                
    } else {
        if (s->castleflag & (1 << bck)) {                            
            if (!check_castle_blocker(s, bck)) {
                moves = add_king_castle(moves, s->bking_loc, g8, bck);
            }
        }    
        if (s->castleflag & (1 << bcq)) {                           
            if (!check_castle_blocker(s, bcq)) {
                moves = add_king_castle(moves, s->bking_loc, c8, bcq);
            }
        }
        
        from = s->bking_loc;        
    }
    
    tsq = KingMoves[from] & friendlymask;        
    
    while (tsq) {
        to = FindFirstRemove(&tsq);        
        moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);                      
    }       
 
    return (int)(moves - start);
}

int gen(state_t *s, move_s *moves) {
    BITBOARD tsq, temp, friendlymask;
    int to, from;
    move_s *start = moves;
    BITBOARD *BitBoard = s->BitBoard;
    int *sboard = s->sboard;
 
    if (s->white_to_move) {
        friendlymask = ~s->WhitePieces;
    } else {
        friendlymask = ~s->BlackPieces;
    }
 
     /* Generate normal moves for white pawns first */
    if (s->white_to_move) {        
        tsq = (WhitePawns >> 8) & ~(s->All);        
        temp = (((WhitePawns & RankMask[Rank2]) >> 16) & ~(s->All));
        temp &= ~(s->All >> 8);
        tsq |= temp;                
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            if (WhitePawns & Mask[to+8]) from = to + 8;
            else                         from = to + 16;
            
            if (Rank(to) == Rank8) {
                moves = add_capture(moves, from, to, sboard[to], wqueen, FALSE);
                moves = add_capture(moves, from, to, sboard[to], wrook, FALSE);
                moves = add_capture(moves, from, to, sboard[to], wbishop, FALSE);
                moves = add_capture(moves, from, to, sboard[to], wknight, FALSE);
            } else {
                moves = add_move(moves, from, to, FALSE);            
            }
        }        
    } else {
        tsq = (BlackPawns << 8) & ~(s->All);        
        temp = (((BlackPawns & RankMask[Rank7]) << 16) & ~(s->All));
        temp &= ~(s->All << 8);
        tsq |= temp;                
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            if (BlackPawns & Mask[to-8]) from = to - 8;
            else                         from = to - 16;
            
            if (Rank(to) == Rank1) {
                moves = add_capture(moves, from, to, sboard[to], bqueen, FALSE);
                moves = add_capture(moves, from, to, sboard[to], brook, FALSE);
                moves = add_capture(moves, from, to, sboard[to], bbishop, FALSE);
                moves = add_capture(moves, from, to, sboard[to], bknight, FALSE);
            } else {
                moves = add_move(moves, from, to, FALSE);  
            }                      
        }
    }

    /* captures (including en-passant) */        
    if (s->white_to_move) {                               
        tsq = (((WhitePawns & ~(FileMask[FileH])) >> 7) 
             | ((WhitePawns & ~(FileMask[FileA])) >> 9));
        
        if (s->ep_square > 0) tsq &= (s->BlackPieces | Mask[s->ep_square]);
        else                  tsq &= s->BlackPieces;
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            
            if (File(to) > 0)  {
                from = to + 7;                
                
                if (WhitePawns & Mask[from]) {	
                    /* promotions */
                    if (Rank(to) == Rank8) {
                        moves = add_capture(moves, from, to, sboard[to], wqueen, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], wrook, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], wbishop, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], wknight, FALSE);
                    } else {
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, bpawn, FALSE, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);
                        }
                    }                    
                }
            }
            if (File(to) < 7) {
                from = to + 9;                
                
                if (WhitePawns & Mask[from]) {                    
                    if (Rank(to) == Rank8) {                    
                        moves = add_capture(moves, from, to, sboard[to], wqueen, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], wrook, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], wbishop, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], wknight, FALSE);
                    } else {
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, bpawn, FALSE, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);
                        }
                    }                    
                }
            }                        
        }
    } else {                      
        tsq = (((BlackPawns & ~(FileMask[FileH])) << 9) 
             | ((BlackPawns & ~(FileMask[FileA])) << 7));
        
        if (s->ep_square > 0) tsq &= (s->WhitePieces | Mask[s->ep_square]);
        else                  tsq &=  s->WhitePieces;
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            
            if (File(to) > 0) {
                from = to - 9;                
                
                if (BlackPawns & Mask[from]) {	                    
                    if (Rank(to) == Rank1) {
                        moves = add_capture(moves, from, to, sboard[to], bqueen, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], brook, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], bbishop, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], bknight, FALSE);
                    } else {
                        /* Check for en-passant move */
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, wpawn, FALSE, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);
                        }
                    }                    
                }
            }
            
            if (File(to) < 7) {
                from = to - 7;                
                
                if (BlackPawns & Mask[from]) {	
                    if (Rank(to) == Rank1) {
                        moves = add_capture(moves, from, to, sboard[to], bqueen, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], brook, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], bbishop, FALSE);
                        moves = add_capture(moves, from, to, sboard[to], bknight, FALSE);
                    } else {
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, wpawn, FALSE, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);
                        }                        
                    }                    
                }
            }            
        }
    }         
    
    if (s->white_to_move) tsq = WhiteKnights;
    else                  tsq = BlackKnights;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp  = KnightMoves[from];
        temp &= friendlymask;
                
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);                                    
        }        
    }           
    
    if (s->white_to_move) tsq = WhiteBishops | WhiteQueens;
    else                  tsq = BlackBishops | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp  = BishopAttacks(s, from);  
        temp &= friendlymask;      
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);	                          
        }        
    }
    
    if (s->white_to_move) tsq = WhiteRooks | WhiteQueens;
    else                  tsq = BlackRooks | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp  = RookAttacks(s, from);              
        temp &= friendlymask;
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);
        }                        
    }   
    
    if (s->white_to_move) {
        if (s->castleflag & (1 << wck)) {                            
            if (!check_castle_blocker(s, wck)) {
                moves = add_king_castle(moves, s->wking_loc, g1, wck);
            }
        }    
        if (s->castleflag & (1 << wcq)) {                            
            if (!check_castle_blocker(s, wcq)) {
                moves = add_king_castle(moves, s->wking_loc, c1, wcq);
            }
        }
        
        from = s->wking_loc;                
    } else {
        if (s->castleflag & (1 << bck)) {                            
            if (!check_castle_blocker(s, bck)) {
                moves = add_king_castle(moves, s->bking_loc, g8, bck);
            }
        }    
        if (s->castleflag & (1 << bcq)) {                           
            if (!check_castle_blocker(s, bcq)) {
                moves = add_king_castle(moves, s->bking_loc, c8, bcq);
            }
        }
        
        from = s->bking_loc;        
    }
    
    tsq = KingMoves[from] & friendlymask;        
    
    while (tsq) {
        to = FindFirstRemove(&tsq);        
        moves = add_capture(moves, from, to, sboard[to], FALSE, FALSE);                      
    }       
 
    return (int)(moves - start);
}

int gen_captures(state_t *s, move_s *moves) {
    int to, from;
    BITBOARD tsq, temp, targets;
    move_s* start = moves; 
    BITBOARD *BitBoard = s->BitBoard;
    int *sboard = s->sboard;    
        
    if (s->white_to_move) {
        targets = s->BlackPieces;
        /* promotions */
        tsq = (WhitePawns & RankMask[Rank7]) >> 8 & ~(s->All);
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
                        
            moves = add_capture(moves, to+8, to, npiece, wqueen, FALSE);            
        }
        
        /* captures (including en-passant) */
        tsq = (((WhitePawns & ~(FileMask[FileH])) >> 7) 
             | ((WhitePawns & ~(FileMask[FileA])) >> 9));
        
        if (s->ep_square > 0) tsq &= (targets | Mask[s->ep_square]);
        else                  tsq &= targets;
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            
            if (File(to) > 0)  {
                from = to + 7;                
                
                if (WhitePawns & Mask[from]) {	
                    /* promotions */
                    if (Rank(to) == 0) {
                        moves = add_capture(moves, from, to, sboard[to], wqueen, FALSE);
                    } else {
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, bpawn, 0, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);
                        }
                    }                    
                }
            }
            if (File(to) < 7) {
                from = to + 9;                
                
                if (WhitePawns & Mask[from]) {                    
                    if (Rank(to) == 0) {                    
                        moves = add_capture(moves, from, to, sboard[to], wqueen, FALSE);
                    } else {
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, bpawn, 0, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);
                        }
                    }                    
                }
            }                        
        }
    } else  {
        targets = s->WhitePieces;
        
        tsq = (BlackPawns & RankMask[Rank2]) << 8 & ~(s->All);
        
        while (tsq) {
            to = FindFirstRemove(&tsq); 
            
            moves = add_capture(moves, to-8, to, npiece, bqueen, FALSE);                        
        }
        
        /* captures (including ep) */
        tsq = (((BlackPawns & ~(FileMask[FileH])) << 9) 
            | ((BlackPawns & ~(FileMask[FileA])) << 7));
        
        if (s->ep_square > 0) tsq &= (targets | Mask[s->ep_square]);
        else                  tsq &= targets;
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            
            if (File(to) > 0) {
                from = to - 9;                
                
                if (BlackPawns & Mask[from]) {	                    
                    if (Rank(to) == 7) {
                        moves = add_capture(moves, from, to, sboard[to], bqueen, FALSE);
                    } else {
                        /* Check for en-passant move */
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, wpawn, 0, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);
                        }
                    }                    
                }
            }
            
            if (File(to) < 7) {
                from = to - 7;                
                
                if (BlackPawns & Mask[from]) {	
                    if (Rank(to) == 7) {
                        moves = add_capture(moves, from, to, sboard[to], bqueen, FALSE);
                    } else {
                        if (to == s->ep_square) {
                            moves = add_capture(moves, from, to, wpawn, 0, TRUE);
                        } else {
                            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);
                        }                        
                    }                    
                }
            }            
        }
    }  
        
    if (s->white_to_move) {
        from = s->wking_loc;                
    } else {
        from = s->bking_loc;                
    }
    
    tsq = (KingMoves[from] & targets);
    
    while (tsq) {
        to = FindFirstRemove(&tsq);        
        moves = add_capture(moves, from, to, sboard[to], 0, FALSE);                      
    }
    
    if (s->white_to_move) tsq = WhiteKnights;
    else                  tsq = BlackKnights;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        temp = KnightMoves[from] & targets;        
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);                                    
        }        
    }
        
    if (s->white_to_move) tsq = WhiteRooks | WhiteQueens;
    else                  tsq = BlackRooks | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp = RookAttacks(s, from);        
        temp &= targets;        
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);
        }                        
    }
    
    if (s->white_to_move) tsq = WhiteBishops | WhiteQueens;
    else                  tsq = BlackBishops | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        
        temp = BishopAttacks(s, from);        
        temp &= targets;        
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);	                          
        }        
    }           
    
    return (int)(moves - start);
}

//XXX: sboard[to] is currently always npiece by definition
int gen_good_checks(state_t *s, move_s *moves) {
    BITBOARD BCheckMask, RCheckMask, KMask, tsq, temp, reverse, freesqmask;
    int to, from, tking_loc;
    move_s* start = moves;     
    BITBOARD *BitBoard = s->BitBoard;
    int *sboard = s->sboard;   
    
    if (s->white_to_move) {
        tking_loc = s->bking_loc;                        
        freesqmask = ~s->WhitePieces; 
    } else {
        tking_loc = s->wking_loc;           
        freesqmask = ~s->BlackPieces; 
    }           

    BCheckMask  = BishopMask[tking_loc];
    RCheckMask  = RookMask[tking_loc];          
    BCheckMask &= freesqmask;        
    RCheckMask &= freesqmask;    
    KMask       = Mask[tking_loc];
    
    if (s->white_to_move) tsq = WhiteKnights;
    else                  tsq = BlackKnights;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);
        
        temp = KnightMoves[from];
        temp &= KnightMoves[tking_loc];          
        temp &= freesqmask;
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            moves = add_capture(moves, from, to, sboard[to], 0, FALSE);
        }
    }
    
    /* pawns (no promotions, no captures) */
    if (s->white_to_move) {
        tsq = (WhitePawns >> 8 & ~(s->All));
        tsq &= PawnAttacksBlack[tking_loc];  
    } else {
        tsq = (BlackPawns << 8 & ~(s->All));
        tsq &= PawnAttacksWhite[tking_loc];      
    }        
  
    while (tsq) {
        to = FindFirstRemove(&tsq);
        
        if (s->white_to_move) {
            from = to+8;
        } else {
            from = to-8;
        }
        
        moves = add_move(moves, from, to, 0);                
    }
    
    if (s->white_to_move) tsq = WhiteRooks | WhiteQueens;
    else                  tsq = BlackRooks | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);
        
        temp = RookAttacks(s, from);      
        temp &= RCheckMask;
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            reverse = RookAttacks(s, to);
            if (reverse & KMask) {                          
                moves = add_capture(moves, from, to, sboard[to], 0, FALSE);                       
            }     
        }
    }
    
    if (s->white_to_move) tsq = WhiteBishops | WhiteQueens;
    else                  tsq = BlackBishops | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);
        
        temp = BishopAttacks(s, from);    
        temp &= BCheckMask;
        
        while (temp) {
            to = FindFirstRemove(&temp);            
            reverse = BishopAttacks(s, to);
            if (reverse & KMask) {                              
                moves = add_capture(moves, from, to, sboard[to], 0, FALSE);                        
            } 
        }
    }            
  
    return (int)(moves - start);
}

int gen_check_blocks(state_t *s, move_s *moves, const int checker) {
    int from, to, ty, target;
    BITBOARD tsq, temp, TargetMask, PieceTargetMask;       
    move_s* start = moves;
    BITBOARD *BitBoard = s->BitBoard;
    
    if (s->white_to_move){
        target = s->wking_loc;
    } else {
        target = s->bking_loc;
    }
    
    ty = rank(target);
    
    /* if we know the exact attacker, we can be more specific here */
    if (checker == wbishop || checker == bbishop) {
        TargetMask = BishopMask[target];
    } else if (checker == wrook || checker == brook) {
        TargetMask = RookMask[target];
    } else {
        TargetMask = ~((BITBOARD)EMPTY);
    }
    
    /* exclude captures */
    PieceTargetMask = TargetMask & ~(s->All);
    
    /* Generate moves for white pawns first */
    if (s->white_to_move) {        
        tsq = (WhitePawns >> 8) & ~(s->All);
        tsq &= ~RankMask[Rank8];
        temp = (((WhitePawns & RankMask[Rank2]) >> 16) & ~(s->All));
        temp &= ~(s->All >> 8);
        tsq |= temp;
        
        tsq &= TargetMask;
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            if (WhitePawns & Mask[to+8]) from = to + 8;
            else                         from = to + 16;

            moves = add_move(moves, from, to, 0);            
        }        
    } else {
        tsq = (BlackPawns << 8) & ~(s->All);
        tsq &= ~RankMask[Rank1];
        temp = (((BlackPawns & RankMask[Rank7]) << 16) & ~(s->All));
        temp &= ~(s->All << 8);
        tsq |= temp;
        
        tsq &= TargetMask;
        
        while (tsq) {
            to = FindFirstRemove(&tsq);
            if (BlackPawns & Mask[to-8]) from = to - 8;
            else                         from = to - 16;

            moves = add_move(moves, from, to, 0);            
        }
    }
    
    /* Now Generate Knight Moves */
    if (s->white_to_move) tsq = WhiteKnights;
    else                  tsq = BlackKnights;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        if (s->white_to_move) temp = KnightMoves[from] & PieceTargetMask;
        else                  temp = KnightMoves[from] & PieceTargetMask;
        
        while (temp) {
            to = FindFirstRemove(&temp);
            moves = add_move(moves, from, to, 0);            
        }
    }
    
    /* Now Generate Rook Moves */
    if (s->white_to_move) tsq = WhiteRooks | WhiteQueens;
    else                  tsq = BlackRooks | BlackQueens;

    while (tsq) {
        from = FindFirstRemove(&tsq);
        
        temp = RookAttacks(s, from);        
        temp &= PieceTargetMask;
        
        while (temp) {
            to = FindFirstRemove(&temp);
            moves = add_move(moves, from, to, 0);            
        }
    }
    
    /* Now Generate Bishop Moves */
    if (s->white_to_move) tsq = WhiteBishops | WhiteQueens;
    else                  tsq = BlackBishops | BlackQueens;
    
    while (tsq) {
        from = FindFirstRemove(&tsq);        
        temp = BishopAttacks(s, from);
        temp &= PieceTargetMask;                
        
        while (temp) {
            to = FindFirstRemove(&temp);
            moves = add_move(moves, from, to, 0);            
        }
    }      

    return (int)(moves - start);
}

int gen_evasions(state_t *s, move_s *moves, const int checker) {
    int to, from;
    BITBOARD tsq;    
    move_s* start = moves;        
    
    /* Now Generate King Moves */  
    if (s->white_to_move) {
        tsq = KingMoves[s->wking_loc] & ~(s->All);
        from = s->wking_loc;
    } else {
        tsq = KingMoves[s->bking_loc] & ~(s->All);
        from = s->bking_loc;
    }
    
    while (tsq) {
        to = FindFirstRemove(&tsq);
        moves = add_move(moves, from, to, 0);                
    }        
      
    moves += gen_captures(s, moves);
      
    /* break if check cannot be blocked */
    if (checker == wknight || checker == wpawn || checker == wking
        || checker == bknight || checker == bpawn || checker == bking) 
        return (int)(moves - start);
    
    moves += gen_check_blocks(s, moves, checker);                
    
    return (int)(moves - start);
}

/*
    6 + 6 + 4 + 1 + 4 + 3 = 24 bits
    1  1111  111 1111 111111 111111
    ep  cap  cst prom   from     to
*/
move_s* add_move(move_s *move,
                 int Pfrom,
                 int Ptarget,
	         int Ppromoted) {
    *move = (npiece << 19) | (no_castle << 16) 
          | (Ppromoted << 12) | (Pfrom << 6) | (Ptarget);
    return ++move;	
}

move_s* add_capture(move_s *move,
                    int Pfrom,
                    int Ptarget,
		    int Pcaptured,
		    int Ppromoted,
		    int Pep) {
    *move = (Pep << 23) | (Pcaptured << 19) | (no_castle << 16) 
          | (Ppromoted << 12) | (Pfrom << 6) | (Ptarget);    
    return ++move;
}

move_s* add_king_castle (move_s *move,
                         int Pfrom, int Ptarget, int Pcastle_type) {
    *move = (npiece << 19) | (Pcastle_type << 16) | (Pfrom << 6) | (Ptarget);    
    return ++move;
}
