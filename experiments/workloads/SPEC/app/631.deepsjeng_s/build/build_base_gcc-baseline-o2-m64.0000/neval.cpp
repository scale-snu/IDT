/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2009 Gian-Carlo Pascutto

    File: neval.c                                        
    Purpose: functions for evaluating positions (standard chess)

*/

#include <math.h>
#include <algorithm>

#include "sjeng.h"
#include "extvars.h"
#include "evpar.h"
#include "bitboard.h"
#include "bits.h"
#include "pawn.h"
#include "see.h"
#include "preproc.h"
#include "endgame.h"
#include "neval.h"
#include "board.h"
#include "state.h"
#include "squares.h"

#undef CBDEBUG
#undef EVDEBUG

extern const int material[14] = { 
    0, 
    MAT_PAWN, -MAT_PAWN, MAT_KNIGHT, -MAT_KNIGHT, 40000, -40000, 
    MAT_ROOK, -MAT_ROOK, MAT_QUEEN, -MAT_QUEEN, MAT_BISHOP, -MAT_BISHOP, 
    0 }; 		       		        

/*
    tables
*/
const static int w_passer[6] = {
    185, 120, 70, 40, 20, 15
};   

const static int w_passer_free[6] = {
    185, 15, 10, 8, 3, 1
};   

const static int w_passer_very_free[6] = {
      0, 80, 30, 15, 10, 10
};

const static int w_passer_king_supported[6] = {
    -25, 25, 7, 5, 5, 4
};

const static int w_passer_blocked[6] = {
    -25, -10, -4, 0, 0, 0
};

const static int w_passer_pawn_supported[6] = {
     65, 25, 8, -3, -5, -5
};

/*
    structure with attacker information
*/
struct attackinfo_t {
    BITBOARD w_pawn_attacks;
    BITBOARD b_pawn_attacks;
    BITBOARD w_bishop_attacks;
    BITBOARD b_bishop_attacks;
    BITBOARD w_knight_attacks;
    BITBOARD b_knight_attacks;
    BITBOARD w_rook_attacks;
    BITBOARD b_rook_attacks;
    BITBOARD w_queen_attacks;
    BITBOARD b_queen_attacks;
};

/*
    passed pawn evaluation
    basic scoring is done in pawnstructure,                        
    these are the extras
*/
static void passer_eval(state_t *s, pawntt_t *pawntt, int phase_scale, t_eval_comps *ec) {
    const int bit_wking = s->wking_loc;
    const int bit_bking = s->bking_loc;
        
    BITBOARD temp, temp2;
    int sq;
    int ra, fi;
    int runsq;
    int run_pawn_dist;
    int run_king_dist;
    int w_score = 0;
    int b_score = 0;
           
    /*
        white passers
    */
    temp = pawntt->w_passed;

    while (temp) {
        sq = FindFirstRemove(&temp);
        ra = Rank(sq);
        fi = File(sq);                     
        
        w_score += w_passer[ra - 1];      
        
        if (s->sboard[sq - 1] == wpawn
            || s->sboard[sq + 9] == wpawn
            || s->sboard[sq + 7] == wpawn) {
            w_score += w_passer_pawn_supported[ra - 1];
        }        

        if (phase_scale < 384) {
            /* 
                check for runners if late endgame
            */        
            runsq = a8 + fi;
            run_pawn_dist = taxicab_dist_bit(runsq, sq);
            run_king_dist = taxicab_dist_bit(runsq, bit_bking);

            if (!s->white_to_move) {
                run_king_dist--;
            }

            if (run_pawn_dist < run_king_dist) {
    #ifdef EVAL_DEBUG
                printf("Runner: %s\n",sq_name[sq]);
    #endif
                w_score += (20 * (1024 - phase_scale)) / 1024;
                if (phase_scale == 0) {
                    w_score += 500;
                }
            }
           
            /*
                forced queening in king-king endgame
            */
            if (phase_scale == 0) {
                if (Rank(bit_wking) <= ra && ra <= Rank7) {
                    if ((fi != FileA && fi != FileH) || File(bit_wking) != fi) {                
                        w_score += 25;
                    }
                } else {
                    w_score -= 25;
                }
            }                
        }
    

        if (Rank(bit_wking) <= ra) {
            /*
                supported passers
            */
            if (abs(File(bit_wking) - fi) == 1) {            
                w_score += w_passer_king_supported[ra - 1];
            }
        }
    
        /*
            free or blocked passers
        */
        temp2 = Mask[sq - 8];

        if (!(temp2 & s->All)) {
            /*
                potentially free
            */
            if (see(s, WHITE, sq, sq - 8, FALSE) >= 0) {                
                w_score += w_passer_free[ra - 1];
                
                if (ra > Rank7 && !(Mask[sq - 16] & s->All)) {
                    if (see(s, WHITE, sq, sq - 16, FALSE) >= 0) {  
                        w_score += w_passer_very_free[ra - 1];
                    }
                }
                
#ifdef EVAL_DEBUG
                printf("Free passer : %s == %d\n",sq_name[sq],scale(7 - ra, PASSER_FREE_MIN, PASSER_FREE_MAX));

#endif
            }
        } else {
            w_score += w_passer_blocked[ra - 1];
        }

        /*
            king tropism
        */
        /*
            bonus for killer being far
        */        
        w_score += (7 - ra) * taxicab_dist_bit(bit_bking, sq - 8);

        /*
            penalty for defender being far
        */        
        w_score += -3 * (7 - ra) * taxicab_dist_bit(bit_wking, sq - 8);

        /*
            rook block
        */        
        if (FileDownMask[sq] & s->BlackRooks) {            
            w_score += -8;
        }
    }

    /*
        black passers
    */
    temp = pawntt->b_passed;

    while (temp) {
        sq = FindFirstRemove(&temp);
        ra = Rank(sq);
        fi = File(sq);
        //nbsq = bit_to_nobit[sq];
      
        b_score -= w_passer[(7 - ra) - 1];   
        
        if (s->sboard[sq + 1] == bpawn
            || s->sboard[sq - 9] == bpawn
            || s->sboard[sq - 7] == bpawn) {
            b_score -= w_passer_pawn_supported[(7 - ra) - 1];
        }                                       

        if (phase_scale < 384) {
            /* 
                check for runners if late endgame
            */       
            runsq = a1 + fi;
            run_pawn_dist = taxicab_dist_bit(runsq,sq);
            run_king_dist = taxicab_dist_bit(runsq,bit_wking);

            if (s->white_to_move) {
                run_king_dist--;
            }

            if (run_pawn_dist < run_king_dist) {
    #ifdef EVAL_DEBUG
                printf("Runner: %s\n",sq_name[sq]);
    #endif
                b_score -= (20 * (1024 - phase_scale)) / 1024;
                if (phase_scale == 0) {
                    b_score -= 500;
                }
            }
            
            /*
                forced queening in king-king endgame
            */
            if (phase_scale == 0) { 
                if (Rank(bit_bking) >= ra && ra >= Rank2) {
                    if ((fi != FileA && fi != FileH) || File(bit_bking) != fi) {                
                        b_score -= 25;
                    }
                } else {
                    b_score += 25;
                }
            }   
        }

        if (Rank(bit_bking) >= ra) {
            /*
                supported passers
            */
            if (abs(File(bit_bking) - fi) == 1) {                
                b_score -= w_passer_king_supported[(7 - ra) - 1];
            }
        }

        /*
            free or blocked passers
        */
        temp2 = Mask[sq + 8];

        if (!(temp2 & s->All)) {

            /*
                potentially free
            */
            if (see(s, BLACK,sq,sq + 8,FALSE) >= 0) {                
                b_score -= w_passer_free[(7 - ra) - 1];
                
                if (ra < Rank2 && !(Mask[sq + 16] & s->All)) {
                    if (see(s, BLACK, sq, sq + 16,FALSE) >= 0) {  
                        b_score -= w_passer_very_free[(7 - ra) - 1];
                    }
                }
#ifdef EVAL_DEBUG
                printf("Free passer : %s == %d\n",sq_name[sq],scale(ra, PASSER_FREE_MIN, PASSER_FREE_MAX));

#endif
            }
        } else {
            b_score -= w_passer_blocked[(7 - ra) - 1];
        }

        /*
            king tropism
        */
        /*
            bonus for killer being far
        */        
        b_score -= ra * taxicab_dist_bit(bit_wking, sq + 8);

        /*
            penalty for defender being far
        */        
        b_score -= -3 * ra * taxicab_dist_bit(bit_bking, sq + 8);

        /*
            rook support or block
        */        
        if (FileUpMask[sq] & s->WhiteRooks) {            
            b_score -= -8;
        }
    }  
    
    ec->w_passers = w_score;
    ec->b_passers = b_score;
    
    return; 
}

/*
    extra positional bonuses
*/
static void positional_eval(state_t *s, pawntt_t *pawntt, t_eval_comps *ec) {          
    int w_score = 0;
    int b_score = 0;

    /* 
        strong pieces (Knights)
        
    */    
    w_score += 15 * PopCount(pawntt->w_strong_square & s->WhiteKnights);
    w_score += 25 * PopCount(pawntt->w_super_strong_square & s->WhiteKnights);

    b_score -= 15 * PopCount(pawntt->b_strong_square & s->BlackKnights);
    b_score -= 25 * PopCount(pawntt->b_super_strong_square & s->BlackKnights);             
    
    /* 
        seventh rank extras
    */
    if ((RankMask[Rank8] & s->BlackKing) || (RankMask[Rank7] & s->BlackPawns)) {
        if (RankMask[Rank7] & s->WhiteRooks) {            
            if (PopCount(RankMask[Rank7] & s->WhiteRooks) > 1) {
                w_score += 40;
            } else {
                w_score +=  3;
            }
        } 
    }

    if ((RankMask[Rank1] & s->WhiteKing) || (RankMask[Rank2] & s->WhitePawns)) {
        if (RankMask[Rank2] & s->BlackRooks) {            
            if (PopCount(RankMask[Rank2] & s->BlackRooks) > 1) {
                b_score -= 40;
            } else {
                b_score -=  3;
            }
        } 
    }

    /* 
        bad bishops
        one bishop, no knight
    */
    if (s->npieces[wbishop] == 1 && s->npieces[wknight] == 0) {
        if (s->WhiteBishops & WhiteSqMask) {
            w_score += -3 * PopCount(s->WhitePawns & WhiteSqMask);
        } else {
            w_score += -3 * PopCount(s->WhitePawns & BlackSqMask);
        }        
    }
    if (s->npieces[bbishop] == 1 && s->npieces[bknight] == 0) {
        if (s->BlackBishops & WhiteSqMask) {
            b_score -= -3 * PopCount(s->BlackPawns & WhiteSqMask);
        } else {
            b_score -= -3 * PopCount(s->BlackPawns & BlackSqMask);
        }        
    }
            
    if (s->npieces[wbishop] == 2) {
        w_score += 30;
    } 

    if (s->npieces[bbishop] == 2) {
        b_score -= 30;
    }  
    
    ec->w_positional = w_score;
    ec->b_positional = b_score;
    
    return;             
}

/*
    trapped pieces
*/
static int trapped_eval(state_t *s, t_eval_comps *ec) {    
    int score = 0;
    int *sboard = s->sboard;
    
    if ((sboard[a2] == bbishop || sboard[b1] == bbishop) && sboard[b3] == wpawn && sboard[c2] == wpawn) {
        score -= -90;
    }

    if ((sboard[h2] == bbishop || sboard[g1] == bbishop) && sboard[g3] == wpawn && (sboard[f2] == wpawn || sboard[f2] == wking)) {
        score -= -90;
    }

    if ((sboard[a7] == wbishop || sboard[b8] == wbishop) && sboard[b6] == bpawn && sboard[c7] == bpawn) {
        score += -90;
    }

    if ((sboard[h7] == wbishop || sboard[g8] == wbishop) && sboard[g6] == bpawn && (sboard[f7] == bpawn || sboard[f7] == bking)) {
        score += -90;
    }

    /* 
        score 'rabbits' 
    */
    if ((sboard[h7] == wknight) && (sboard[f6] == bpawn) && (sboard[g7] == bpawn)) {
        score += -35;
    }

    if ((sboard[h2] == bknight) && (sboard[f3] == wpawn) && (sboard[g2] == wpawn)) {
        score -= -35;
    }

    /* 
        fake castling traps
    */
    if ((sboard[c8] == bking || sboard[b8] == bking) && (sboard[a8] == brook || sboard[a7] == brook || sboard[b8] == brook)) {
        score -= -25;
    }

    if ((sboard[f8] == bking || sboard[g8] == bking) && (sboard[h8] == brook || sboard[h7] == brook || sboard[g8] == brook)) {
        score -= -25;
    }

    if ((sboard[c1] == wking || sboard[b1] == wking) && (sboard[a1] == wrook || sboard[a2] == wrook || sboard[b1] == wrook)) {
        score += -25;
    }

    if ((sboard[f1] == wking || sboard[g1] == wking) && (sboard[h1] == wrook || sboard[h2] == wrook || sboard[g1] == wrook)) {
        score += -25;
    }    
    
    return score;
}

/*
    adjust opposite bishop endgames
*/
static int opposite_bishops(state_t *s, int score, const int wmat, const int bmat) {
    /*         
        no more than rook and one piece
    */
    if (wmat <= 10 && bmat <= 10) {
        /* 
            opposite bishops, both 1 bishop, different color
        */
        if ((s->npieces[wbishop] == s->npieces[bbishop]) && (s->npieces[wbishop] == 1)
            && (PopCount((s->WhiteBishops | s->BlackBishops) & WhiteSqMask) == 1)) {

            /* 
                material difference of less than or equal to
                    2 pawns
            */
            if (abs(s->Material) <= 2 * MAT_PAWN) {
                /*
                    more than just bishops?
                */
                if (wmat > 4 || bmat > 4) {

                    /* reduce 1/4 */
                    score = (score * 3) >> 2;
                } else {

                    /* reduce 1/2 */
                    score = score >> 1;
                }
            }
        }
    }

    return score;
}

/* 
    mobility evaluation
    also include rooks on files etc
*/
static int mobility_eval(state_t *s, t_eval_comps *ec, attackinfo_t *att) {
    int i;
    BITBOARD temp, pcatt, negatt;
    BITBOARD WhitePcMask, BlackPcMask;    
    BITBOARD WhitePAttkMask, BlackPAttkMask;  
    int tmp;
    int score = 0;        
    int w_counter = 0;
    int b_counter = 0;
    BITBOARD *BitBoard = s->BitBoard;

    WhitePcMask = ~s->WhitePieces;
    BlackPcMask = ~s->BlackPieces;
    
    /*
        pawn attacks
    */
    pcatt = (((WhitePawns & ~(FileMask[FileH])) >> 7) 
           | ((WhitePawns & ~(FileMask[FileA])) >> 9));    
    att->w_pawn_attacks = pcatt;
    WhitePAttkMask = ~pcatt;

    pcatt = (((BlackPawns & ~(FileMask[FileH])) << 9) 
           | ((BlackPawns & ~(FileMask[FileA])) << 7));
    att->b_pawn_attacks = pcatt;  
    BlackPAttkMask = ~pcatt;
    
    /*
        piece attacks
    */        
    temp = WhiteBishops;
    while (temp) {
        i = FindFirstRemove(&temp);                
        
        /*
            X-Ray through Queen and Knight
        */
        s->All ^= WhiteQueens | WhiteKnights;
        pcatt = BishopAttacks(s, i);  
        negatt = pcatt & WhitePawns;                         
        att->w_bishop_attacks = pcatt;        
        pcatt &= WhitePcMask;      
        pcatt &= BlackPAttkMask; 
        s->All ^= WhiteQueens | WhiteKnights;
        
        int moves = ThickPopCount(pcatt);
        
        tmp = moves - PopCount(negatt);
        w_counter += tmp; 
    }

    temp = BlackBishops;
    while (temp) {
        i = FindFirstRemove(&temp);
        
        /*
            X-Ray through Queen
        */
        s->All ^= BlackQueens | BlackKnights;
        pcatt = BishopAttacks(s, i);                   
        negatt = pcatt & BlackPawns;   
        att->b_bishop_attacks = pcatt;   
        pcatt &= BlackPcMask;     
        pcatt &= WhitePAttkMask;
        s->All ^= BlackQueens | BlackKnights; 
        
        int moves = ThickPopCount(pcatt);
        
        tmp = moves - PopCount(negatt);
        b_counter -= tmp;      
    }

    temp = WhiteRooks;
    while (temp) {
        i = FindFirstRemove(&temp);                
     
        /*
            X-Ray through Queen
        */
        s->All ^= WhiteQueens | WhiteKnights;
        pcatt = RookAttacks(s, i);                   
        negatt = pcatt & WhitePawns;
        att->w_rook_attacks = pcatt;   
        pcatt &= WhitePcMask;     
        pcatt &= BlackPAttkMask;
        s->All ^= WhiteQueens | WhiteKnights;   
        
        tmp = ThickPopCount(pcatt) - PopCount(negatt);
        w_counter += tmp;              
        
        /* 
            halfopen file 
        */
        if (!(FileMask[File(i)] & WhitePawns)) {
            /*
                connected?
            */
            int connected = PopCount(FileMask[File(i)] & WhiteRooks);

            /* 
                open file
            */
            if (!(FileMask[File(i)] & BlackPawns)) {
                if (connected >= 2) {
                    score += s->npieces[wpawn] + 12;                    
                } else {
                    score += s->npieces[wpawn] +  8;                    
                }
            } else {
                if (connected >= 2) {
                    score += (s->npieces[wpawn] >> 1) + 8;  
                } else {
                    score += (s->npieces[wpawn] >> 1) + 4;                    
                }
            }
        }
    }

    temp = BlackRooks;
    while (temp) {
        i = FindFirstRemove(&temp);       
        
        /*
            X-Ray through Queen
        */
        s->All ^= BlackQueens | BlackKnights;
        pcatt = RookAttacks(s, i);     
        negatt = pcatt & BlackPawns;              
        att->b_rook_attacks = pcatt;        
        pcatt &= BlackPcMask;
        pcatt &= WhitePAttkMask;
        s->All ^= BlackQueens | BlackKnights; 
        
        tmp = ThickPopCount(pcatt) - PopCount(negatt);
        b_counter -= tmp;      
        
        /* 
            halfopen file 
        */
        if (!(FileMask[File(i)] & BlackPawns)) {
            /*
                connected?
            */
            int connected = PopCount(FileMask[File(i)] & BlackRooks);
            
            /* 
                open file
            */
            if (!(FileMask[File(i)] & WhitePawns)) {
                if (connected >= 2) {
                    score -= s->npieces[bpawn] + 12;
                } else {
                    score -= s->npieces[bpawn] +  8;
                }
            } else {
                if (connected >= 2) {
                    score -= (s->npieces[bpawn] >> 1) + 8;
                } else {
                    score -= (s->npieces[bpawn] >> 1) + 4;
                }
            }
        }          
    }

    temp = WhiteQueens;
    while (temp) {
        i = FindFirstRemove(&temp);

        pcatt = RookAttacks(s, i);
        pcatt |= BishopAttacks(s, i);        
        
        att->w_queen_attacks = pcatt;                    

	score += 14 - sum_dist(i, s->bking_loc);
    }

    temp = BlackQueens;
    while (temp) {
        i = FindFirstRemove(&temp);

        pcatt = RookAttacks(s, i);
        pcatt |= BishopAttacks(s, i);       
        
        att->b_queen_attacks = pcatt,

	score -= 14 - sum_dist(i, s->wking_loc);    
    }

    temp = WhiteKnights;
    while (temp) {
        i = FindFirstRemove(&temp);

        pcatt = KnightMoves[i];                
        att->w_knight_attacks = pcatt;  
        pcatt &= WhitePcMask;
        pcatt &= BlackPAttkMask;
        
        int moves = PopCount(pcatt);;
        
        w_counter += 2 * moves;
    }

    temp = BlackKnights;
    while (temp) {
        i = FindFirstRemove(&temp);

        pcatt = KnightMoves[i];                
        att->b_knight_attacks = pcatt;
        pcatt &= BlackPcMask;
        pcatt &= WhitePAttkMask;  
        
        int moves = PopCount(pcatt);
        
        b_counter -= 2 * moves;      
    }                      
    
    score += (w_counter + b_counter) * 3;

    return score;       
}

/*
    kingsafety eval
*/
static void kingsafety_eval(state_t *s, pawntt_t *pawntt, t_eval_comps *ec) {                         
    BITBOARD filemask;                   
    const int bit_wking = s->wking_loc;
    const int bit_bking = s->bking_loc;     
    int kra;
    int kfi;
    int nbsq;
    int w_score = 0;      
    int b_score = 0;
                    
    /*
        white king safety
    */
    if (s->npieces[bqueen]) {
        kra = Rank(bit_wking);
        kfi = File(bit_wking);
        
        filemask = KingFilesMask[kfi];

        /*
            castle loss
        */            
        if (kfi > FileC && kfi < FileG) {            

            if (!(s->castleflag & (1 << wcq))) {            
                w_score += -16;
            } 
            if (!(s->castleflag & (1 << wck))) {
                w_score += -12;
            }
        } 

        /*
            close up defects
        */
        if (kra > Rank6) {  
            if (PopCount(s->WhitePawns & filemask & RankMask[Rank2]) == 3) {
                w_score += 6;
            }
	    w_score -= 2 * ThickPopCount(pawntt->w_half_open_files & filemask);
            
            nbsq = s->wking_loc;
            
            if (File(s->wking_loc) == FileA) {
                nbsq++;
            } else if (File(s->wking_loc) == FileH) {
                nbsq--;
            }
            
            w_score += 12 * (s->sboard[nbsq - 9] == wpawn);
            w_score += 24 * (s->sboard[nbsq - 8] == wpawn);
            w_score += 12 * (s->sboard[nbsq - 7] == wpawn);
            
            w_score +=  4 * (s->sboard[nbsq - 17] == wpawn);            
            w_score += 10 * (s->sboard[nbsq - 16] == wpawn);            
            w_score +=  4 * (s->sboard[nbsq - 15] == wpawn);
                        
            w_score +=  4 * (s->sboard[nbsq - 24] == wpawn);                                 
        } else {
            w_score += -30;
        }        
                            
        /*
            pawnstorm
        */
        if (kra > Rank6) {
            w_score += -4 * PopCount(s->BlackPawns & filemask & (RankMask[kra - 1] | RankMask[kra - 2] | RankMask[kra - 3]));
        }  
        
         /*
            attacking rooks/queens
        */                                                 
        w_score += -13 * PopCount(filemask & (s->BlackRooks | s->BlackQueens) & pawntt->b_half_open_files);                                                    
    }   

    /*
        black king safety
    */
    if (s->npieces[wqueen]) {
        kra = Rank(bit_bking);
        kfi = File(bit_bking);       

        filemask = KingFilesMask[kfi];
      
        /*
            castle loss
        */
        if (kfi > FileC && kfi < FileG) {            

            if (!(s->castleflag & (1 << bcq))) {            
                b_score -= -16;           
            } 
            if (!(s->castleflag & (1 << bck))) {
                b_score -= -12;
            }
        } 

        /*
            close up defects
        */
        if (kra < Rank3) {      
            if (PopCount(s->BlackPawns & filemask & RankMask[Rank7]) == 3) {
                b_score -= 6;
            }
	    b_score += 2 * ThickPopCount(pawntt->b_half_open_files & filemask);
            
            nbsq = s->bking_loc;
            
            if (File(s->bking_loc) == FileA) {
                nbsq++;
            } else if (File(s->bking_loc) == FileH) {
                nbsq--;
            }
            
            b_score -= 12 * (s->sboard[nbsq + 9] == bpawn);
            b_score -= 24 * (s->sboard[nbsq + 8] == bpawn);
            b_score -= 12 * (s->sboard[nbsq + 7] == bpawn);
            
            b_score -=  4 * (s->sboard[nbsq + 15] == bpawn);            
            b_score -= 10 * (s->sboard[nbsq + 16] == bpawn);            
            b_score -=  4 * (s->sboard[nbsq + 17] == bpawn);
                                    
            b_score -=  4 * (s->sboard[nbsq + 24] == bpawn);            
        } else {
            b_score -= -30;
        }                                 
        
        /*
            pawnstorm
        */
        if (kra < Rank3) {
            b_score -= -4 * PopCount(s->WhitePawns & filemask & (RankMask[kra + 1] | RankMask[kra + 2] | RankMask[kra + 3]));
            
        }                                                 
                                
        b_score -= -13 * PopCount(filemask & (s->WhiteRooks | s->WhiteQueens) & pawntt->w_half_open_files);                                                    
    }     
    
    ec->w_ks_score += w_score;
    ec->b_ks_score += b_score;  

    return;    
}

/*
    king pressure eval
*/
static void kingpressure_eval(state_t *s, attackinfo_t* attk, t_eval_comps *ec) {                           
    const int bit_wking = s->wking_loc;
    const int bit_bking = s->bking_loc;    
    int weight;
    int counter;    
    BITBOARD temp;    
    BITBOARD defenders;
    BITBOARD attackers;          
    int w_score = 0;    
    int b_score = 0;
        
    /*
        no KS with no Qs
    */
    if (!s->npieces[wqueen] && !s->npieces[bqueen]) {
        return;
    }
    
    /*
        some precalculation
    */    
    defenders = attk->w_pawn_attacks |
                attk->w_bishop_attacks |
                attk->w_knight_attacks |
                attk->w_rook_attacks |
                attk->w_queen_attacks;
        
    attackers = attk->b_pawn_attacks |
                attk->b_bishop_attacks |
                attk->b_knight_attacks |
                attk->b_rook_attacks |
                attk->b_queen_attacks;    
        
    /*
        white king safety
    */
    if (s->npieces[bqueen]) {              
        /*
            attack analysis
        */        
        counter = 0;

        temp = KingPressureMask[bit_wking];
        
        w_score += -22 * PopCount(temp & (s->BlackQueens | s->BlackRooks 
                                          | s->BlackBishops | s->BlackKnights));        
        
        if (temp & attk->b_pawn_attacks) {
            counter++;            
        }
        
        if (temp & attk->b_bishop_attacks) {
            counter++;            
        }
        
        if (temp & attk->b_knight_attacks) {
            counter++;            
        }
        
        if (temp & attk->b_rook_attacks) {
            counter++;            
        }
        
        if (temp & attk->b_queen_attacks) {
            counter++;            
        }                          
                     
        w_score += -3 * counter;
        w_score += -3 * ThickPopCount(temp & attackers);
                                                                    
        weight = 0;        

        temp = KingPressureMask1[bit_wking];          
        
        if (temp & attk->b_pawn_attacks) {            
            weight += 1;
        }
        
        if (temp & attk->b_bishop_attacks) {            
            weight += 2;
        }
        
        if (temp & attk->b_knight_attacks) {            
            weight += 2;
        }
        
        if (temp & attk->b_rook_attacks) {            
            weight += 4;
        }
        
        if (temp & attk->b_queen_attacks) {            
            weight += 8;
        }     
            
        w_score += -2 * weight;            
    }
    
    /*
        flip
    */
    temp = defenders;
    defenders = attackers;
    attackers = temp;

    /*
        black king safety
    */
    if (s->npieces[wqueen]) {       
        /*
            attack analysis
        */        
        counter = 0;

        temp = KingPressureMask[bit_bking];
        
        b_score -= -22 * PopCount(temp & (s->WhiteQueens | s->WhiteRooks 
                                          | s->WhiteBishops | s->WhiteKnights));
        
        if (temp & attk->w_pawn_attacks) {
            counter++;          
        }
        
        if (temp & attk->w_bishop_attacks) {
            counter++;            
        }
        
        if (temp & attk->w_knight_attacks) {
            counter++;            
        }
        
        if (temp & attk->w_rook_attacks) {
            counter++;            
        }
        
        if (temp & attk->w_queen_attacks) {
            counter++;            
        }                           
               
        b_score -= -3 * counter;    
        b_score -= -3 * ThickPopCount(temp & attackers);
        
        weight = 0;        

        temp = KingPressureMask1[bit_bking];           
        
        if (temp & attk->w_pawn_attacks) {            
            weight += 1;
        }
        
        if (temp & attk->w_bishop_attacks) {            
            weight += 2;
        }
        
        if (temp & attk->w_knight_attacks) {            
            weight += 2;
        }
        
        if (temp & attk->w_rook_attacks) {            
            weight += 4;
        }
        
        if (temp & attk->w_queen_attacks) {            
            weight += 8;
        }     
             
        b_score -= -2 * weight;           
    }     
    
    ec->w_ks_score += w_score;
    ec->b_ks_score += b_score;     

    return;    
}  

void feval(state_t *s, int phase_scale, t_eval_comps *ec) {
    pawntt_t *pawntt;
    attackinfo_t attack_info;    
    int score = 0;            
      
    memset(&attack_info, 0, sizeof(attackinfo_t));  

    /*
        mobility eval
    */
    score += mobility_eval(s, ec, &attack_info); 

    /*
        pawn evaluation 
    */
    pawntt = static_pawn_eval(s);

    ec->w_pawnstruct     = pawntt->w_score;
    ec->b_pawnstruct     = pawntt->b_score;       

    /*
        kingsafety eval
    */   
    kingsafety_eval  (s, pawntt,       ec);
    kingpressure_eval(s, &attack_info, ec);     
    /*
        passed pawn evaluation
    */
    passer_eval(s, pawntt, phase_scale, ec);
    
    /*
        piece-square eval
    */
    psq_eval(s, ec);

    /*
        extra position bonuses
    */
    positional_eval(s, pawntt, ec);

    /*
        traps
    */
    score += trapped_eval(s, ec);                           

    ec->accum_score = score;                                                              
}

int eval(state_t *s, const int alpha, const int beta, const int forcefull) {
    t_eval_comps ec;
    int eval_fast, lazy_margin, lazy_score;
    int score;
    int wmat, bmat, total_mat;
    int phase_scale;

    eval_fast  = s->Material;
    eval_fast += s->psq_score;
        
    /* 
        check for a lazy evaluation exit
        if there are no pawns there are too many exceptions
    */
    if (!forcefull && s->npieces[wpawn] && s->npieces[bpawn]) {
       
        lazy_margin = 50;

        /*
            estimate score with help of last ply
        */
        if (s->white_to_move) {
            lazy_score = s->plyeval[s->ply - 1];
            lazy_score += eval_fast;
        } else {
            lazy_score = -s->plyeval[s->ply - 1];
            lazy_score += -eval_fast;
        }                

        /*
            forward store positional eval
        */
        s->plyeval[s->ply] = s->plyeval[s->ply - 1];                

        if (lazy_score + lazy_margin <= alpha) {                 
            return lazy_score + lazy_margin;
        } else if (lazy_score - lazy_margin >= beta) {                        
            return lazy_score - lazy_margin;
        }
    }

    /*
        if it's not lazy, it's a full one
    */        
    wmat = s->npieces[wbishop] * 4 + s->npieces[wknight] * 4 
           + s->npieces[wrook] * 6 + s->npieces[wqueen] * 12;
    bmat = s->npieces[bbishop] * 4 + s->npieces[bknight] * 4 
           + s->npieces[brook] * 6 + s->npieces[bqueen] * 12;
            
    wmat = std::min(40, wmat);
    bmat = std::min(40, bmat);
    total_mat = wmat + bmat;

    phase_scale = (total_mat * 1024) / 80;         

    score = 0;
    
    memset(&ec, 0, sizeof(t_eval_comps));
    feval(s, phase_scale, &ec);                
    
    score += !!s->white_to_move *  10;
    score -=  !s->white_to_move *  10;
    
    score += eval_fast;          
    
    score += ec.w_positional + ec.b_positional;                                   
    score += ec.accum_score;                
    score += ec.w_pawnstruct + ec.b_pawnstruct;
    score += ((ec.w_psq_king + ec.b_psq_king) * (1024 - phase_scale)) / 1280;        
    score += ((ec.w_passers + ec.b_passers) * (2048 - phase_scale)) / 1536;
    
    int wks_score, bks_score;    

     /* quadratic scaling y = -0,0039x2 + 0,9954x + 13,2572 */
    wks_score = (-( ec.w_ks_score *  ec.w_ks_score) / 256) +  ec.w_ks_score;
    bks_score = ( ( ec.b_ks_score *  ec.b_ks_score) / 256) +  ec.b_ks_score;

    score += ((wks_score + bks_score) * (1024 + phase_scale)) / 2048;    
    
    /* opposite bishops */
    score = opposite_bishops(s, score, wmat, bmat);    
    score = drawn_endgames(s, score, wmat, bmat);    
        
    /*
        keep track of positional score
    */
    s->plyeval[s->ply] = score - eval_fast;           
    
    /* 
        adjust for color: 
    */
    if (s->white_to_move) {
        return score;
    } else {
        return -score;
    }
}

int retrieve_eval(state_t *s) {
    int score;

    score = s->plyeval[s->ply] + s->Material + s->psq_score;

    if (s->white_to_move) {
        return score;
    } else {
        return -score;
    }
}
