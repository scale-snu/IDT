/*
    Sjeng - a chess playing program
    Copyright (C) 2001-2009 Gian-Carlo Pascutto

    File: bitboard.c                                             
    Purpose: handle bitboard-related functions                      
 
*/

#include "config.h"
#include "sjeng.h"

#include <algorithm>

#include "extvars.h"
#include "bitboard.h"
#include "bits.h"
#include "moves.h"
#include "board.h"
#include "utils.h"
#include "squares.h"

/*
    Length of the diagonals.
    These are only used as aids in the generation of masks.
*/
static const int DiagonalLength_a1h8[64] = {
  1,2,3,4,5,6,7,8,
  2,3,4,5,6,7,8,7,
  3,4,5,6,7,8,7,6,
  4,5,6,7,8,7,6,5,
  5,6,7,8,7,6,5,4,
  6,7,8,7,6,5,4,3,
  7,8,7,6,5,4,3,2,
  8,7,6,5,4,3,2,1
};

static const int DiagonalLength_a8h1[64] = {
  8,7,6,5,4,3,2,1,
  7,8,7,6,5,4,3,2,
  6,7,8,7,6,5,4,3,
  5,6,7,8,7,6,5,4,
  4,5,6,7,8,7,6,5,
  3,4,5,6,7,8,7,6,
  2,3,4,5,6,7,8,7,
  1,2,3,4,5,6,7,8 
};

BITBOARD KnightMoves[64],
         KingMoves[64],
         PawnAttacksBlack[64],
         PawnAttacksWhite[64],
         PawnMovesBlack[64],
         PawnMovesWhite[64];

BITBOARD fillUpAttacks[64][8], 
         aFileAttacks[64][8];

unsigned char firstRankAttacks[64][8];

/*
    Board masks
*/
BITBOARD Mask[64], 
         InvMask[64];        

BITBOARD DiagMaska1h8[64], 
         DiagMaska8h1[64];

BITBOARD FileMask[8],
         RankMask[8],
         AboveMask[8],
         BelowMask[8],
         LeftMask[8],
         RightMask[8];

BITBOARD RookMask[64],
         BishopMask[64],
         QueenMask[64]; 

BITBOARD CastleMask[4];

BITBOARD FileUpMask[64],
         FileDownMask[64];

BITBOARD WhiteKingSide, 
         WhiteQueenSide, 
         BlackKingSide, 
         BlackQueenSide;
BITBOARD KingSafetyMask[64],
         KingSafetyMask1[64];

BITBOARD WhiteStrongSquareMask,
         BlackStrongSquareMask;

BITBOARD WhiteSqMask,
         BlackSqMask;

BITBOARD KSMask,
         QSMask;    
                 
BITBOARD KingFilesMask[8],
         KingPressureMask[64],
         KingPressureMask1[64],
         CenterMask,
         SpaceMask[2];      

/*
    Debugging aides
*/
void PrintBitboard(const BITBOARD B) {
    BITBOARD b = UNIT;
    int sq;
    
    for (sq = 0; sq < 64; sq++) {
        if (B &( b << sq)) 
            myprintf("X");
        else 
            myprintf(".");
        
        if (sq % 8 == 7) 
            myprintf("\n");
    }
    
    myprintf("\n");
}

void PrintRotBitboards(state_t *s) {
    myprintf("All\n");
    PrintBitboard(s->All);   
}

void PrintAllBitboards(state_t *s) {
    myprintf("WhitePieces\n");
    PrintBitboard(s->WhitePieces);
    myprintf("BlackPieces\n");
    PrintBitboard(s->BlackPieces);
    myprintf("All\n");
    PrintBitboard(s->All);
    
    BITBOARD *BitBoard = s->BitBoard;

    myprintf("Whitepawns\n");
    PrintBitboard(WhitePawns);
    myprintf("Blackpawns\n");
    PrintBitboard(BlackPawns);
    myprintf("WhiteKnights\n");
    PrintBitboard(WhiteKnights);
    myprintf("BlackKnights\n");
    PrintBitboard(BlackKnights);
    myprintf("WhiteBishops\n");
    PrintBitboard(WhiteBishops);
    myprintf("BlackBishops\n");
    PrintBitboard(BlackBishops);
    myprintf("WhiteRooks\n");
    PrintBitboard(WhiteRooks);
    myprintf("BlackRooks\n");
    PrintBitboard(BlackRooks);
    myprintf("WhiteQueens\n");
    PrintBitboard(WhiteQueens);
    myprintf("BlackQueens\n");
    PrintBitboard(BlackQueens);
}

/* 
    Calculate bitboard masks, movegen, ...
*/
void SetupPrecalculatedData(void) {
    BITBOARD b = UNIT;
    BITBOARD mask;
    int i, j, x, y, filenum, diagstart;   
    int comp_j;
    
    for (i = 0; i < 64; i++) {
        Mask[i] = (b << i);
        InvMask[i] = ~Mask[i];
    }
    
    for (i = 0; i < 8; i++) {
        FileMask[i] = 0;
        
        for (j = i; j < 64; j += 8) 
            FileMask[i] |= Mask[j];
        
        RankMask[i] = (((BITBOARD)FULLRANK) << (8*i));
    }     
    
    for (i = 0; i < 64; i++) {
        FileUpMask[i] = 0;
        FileDownMask[i] = 0;
        
        for (j = i - 8; j >= 0; j -= 8) 
            FileUpMask[i] |= Mask[j];
        
        for (j = i + 8; j < 64; j += 8) 
            FileDownMask[i] |= Mask[j];
    }
    
    
    /* Diagonal BitBoard Masks */
    for (i = 0; i < 64; i++) {
        DiagMaska1h8[i] = 0;
        DiagMaska8h1[i] = 0;
        
        /* 
            find starting square, then run over with fixed
            advance (length is known in advance)       
        */
        diagstart = 7*(std::min(File(i), 7-Rank(i))) + i;
        for (j = 0; j < DiagonalLength_a1h8[i]; j++) {
            DiagMaska1h8[i] |= Mask[diagstart - j*7];
        }        
        
        diagstart = i - 9*(std::min(File(i), Rank(i)));
        for (j = 0; j < DiagonalLength_a8h1[i]; j++) {
            DiagMaska8h1[i] |= Mask[diagstart + j*9];
        }        
    }       
    
    /* 
        all squares within 2 squares of king
        -> not including king itself!
    */
    for (i = 0; i < 64; i++) {
        KingSafetyMask[i] = 0;  
        
        for (x = File(i) - 2; x <= File(i) + 2; x++) {
            if (x < 0 || x > 7) continue;
            
            for (y = Rank(i) - 2; y <= Rank(i) + 2; y++) {
                if (y < 0 || y > 7) continue;
                
                j = (y * 8) + x;
                if (i == j) continue;
                
                KingSafetyMask[i] |= Mask[j];
            }
        }    
    }
    
    /* 
        all squares within 1 square of king
        -> not including king itself!
    */
    for (i = 0; i < 64; i++) {
        KingSafetyMask1[i] = 0;  
        
        for (x = File(i) - 1; x <= File(i) + 1; x++) {
            if (x < 0 || x > 7) continue;
            
            for (y = Rank(i) - 1; y <= Rank(i) + 1; y++) {
                if (y < 0 || y > 7) continue;
                
                j = (y * 8) + x;
                if (i == j) continue;
                
                KingSafetyMask1[i] |= Mask[j];
            }
        }    
    }         
    
    /* 
        last set bit in 16 bit number
    */
    for (i = 0; i < 0xFFFF; i++) {                
        for (j = 15; j >= 0; j--) {
            if (i & (1 << j)) {
                last_bit[i] = j; break;
            }
        }
    }
    
    for (i = 0; i < 64; i++) {
        PawnAttacksBlack[i] = 0;
        PawnAttacksWhite[i] = 0;
        PawnMovesBlack[i] = 0;
        PawnMovesWhite[i] = 0;
                
        if (File(i) > 0) {
            if (i < a1) PawnAttacksBlack[i] |= Mask[i+7];
            if (i > h8) PawnAttacksWhite[i] |= Mask[i-9];
        }

        if (File(i) < 7) {
            if (i < a1) PawnAttacksBlack[i] |= Mask[i+9];
            if (i > h8) PawnAttacksWhite[i] |= Mask[i-7];
        }

        if (Rank(i) == Rank1 || Rank(i) == Rank8) 
            continue;

        PawnMovesBlack[i] |= Mask[i+8];
        PawnMovesWhite[i] |= Mask[i-8];
        
        if (Rank(i) == Rank7) 
            PawnMovesBlack[i] |= Mask[i+16];
        if (Rank(i) == Rank2) 
            PawnMovesWhite[i] |= Mask[i-16];
    }
    
    for (i = 0; i < 64; i++) {
        KnightMoves[i] = 0;

        if (Rank(i) > 0) {
            if (Rank(i) > 1) {
                if (File(i) > 0) KnightMoves[i] |= Mask[i-17]; 
                if (File(i) < 7) KnightMoves[i] |= Mask[i-15]; 
            }
            if (File(i) > 1) KnightMoves[i] |= Mask[i-10];
            if (File(i) < 6) KnightMoves[i] |= Mask[i-6];
        }

        if (Rank(i) < 7) {
            if (Rank(i) < 6) {
                if (File(i) > 0) KnightMoves[i] |= Mask[i+15]; 
                if (File(i) < 7) KnightMoves[i] |= Mask[i+17]; 
            }
            if (File(i) > 1) KnightMoves[i] |= Mask[i+6];
            if (File(i) < 6) KnightMoves[i] |= Mask[i+10];
        }
    }
    
    for (i = 0; i < 64; i++) {
        KingMoves[i] = 0;

        if (Rank(i) > 0) {
            if (File(i) > 0) KingMoves[i] |= Mask[i-9];
            if (File(i) < 7) KingMoves[i] |= Mask[i-7];
            KingMoves[i] |= Mask[i-8];
        }

        if (Rank(i) < 7) {
            if (File(i) > 0) KingMoves[i] |= Mask[i+7];
            if (File(i) < 7) KingMoves[i] |= Mask[i+9];
            KingMoves[i] += (b<<(i+8));
        }

        if (File(i) > 0) KingMoves[i] |= Mask[i-1];
        if (File(i) < 7) KingMoves[i] |= Mask[i+1];
    }   
    
    for (filenum = 0; filenum < 8; filenum++) {
        /* 
            rank occupation state loop 
            we don't actually use all 256 values,
            but doing the compression in a lazy way
            is easier
        */
        for (j = 0; j < 256; j++) {
            mask = 0;           

            for (x = filenum - 1; x >= 0; x--) {               
                mask |= Mask[x];
                /* hit, can't go further in this direction */
                if (j & (1 << x)) break;
            }
            for (x = filenum + 1; x < 8; x++) {                
                mask |= Mask[x];
                /* hit, can't go further in this direction */
                if (j & (1 << x)) break;
            }
            comp_j = (j & 126) >> 1;
                           
            firstRankAttacks[comp_j][filenum] = (unsigned char)mask;                           
        }
    }
    
    for (j = 0; j < 8; j++) {    
        for (x = 0; x < 64; x++) {
            fillUpAttacks[x][j]  =   0x0101010101010101ULL * firstRankAttacks[x][j];
            aFileAttacks[x][j^7] = ((0x8040201008040201ULL * firstRankAttacks[x][j]) & 0x8080808080808080ULL) >> 7; 
        }        
    }
        
    /*
        Mask of all possible moves from this position,
        on an empty board        
    */
    for (i = 0; i < 64; i++) {        
        RookMask[i]   = FileMask[File(i)] | RankMask[Rank(i)];
        BishopMask[i] = DiagMaska1h8[i] | DiagMaska8h1[i];
        QueenMask[i]  = RookMask[i] | BishopMask[i];        
    }
    
    for (i = 0; i < 8; i++) {
        AboveMask[i] = EMPTY;
        BelowMask[i] = EMPTY;
        LeftMask[i] = EMPTY;
        RightMask[i] = EMPTY;

        for (j = 0; j < i; j++) {
            AboveMask[i] |= RankMask[j];
            LeftMask[i]  |= FileMask[j];
        }
        for (j = 7; j > i; j--) {
            BelowMask[i] |= RankMask[j];
            RightMask[i] |= FileMask[j];
        }
    }

    /*
        square color masks
    */
    WhiteSqMask = 0;
    BlackSqMask = 0;
    for (i = 0; i < 64; i++) {
        if (IsSqBlack(i)) {        
            BlackSqMask |= Mask[i];            
        } else {
            WhiteSqMask |= Mask[i];                        
        }        
    }
    assert(Mask[a1] & BlackSqMask);
          
    WhiteKingSide  = BelowMask[Rank5] & RightMask[FileD];
    WhiteQueenSide = BelowMask[Rank5] & LeftMask[FileE];
    BlackKingSide  = AboveMask[Rank4] & RightMask[FileD];
    BlackQueenSide = AboveMask[Rank4] & LeftMask[FileE];

    QSMask = FileMask[FileA] | FileMask[FileB] | FileMask[FileC] | FileMask[FileD];
    KSMask = FileMask[FileE] | FileMask[FileF] | FileMask[FileG] | FileMask[FileH];  

    WhiteStrongSquareMask = Mask[d4] | Mask[e4] | Mask[c5] | Mask[d5] | Mask[e5]
                          | Mask[f5] | Mask[c6] | Mask[d6] | Mask[e6] | Mask[f6]
                          | Mask[c7] | Mask[d7] | Mask[e7] | Mask[f7];

    BlackStrongSquareMask = Mask[d5] | Mask[e5] | Mask[c4] | Mask[d4] | Mask[e4]
                          | Mask[f4] | Mask[c3] | Mask[d3] | Mask[e3] | Mask[f3]
                          | Mask[c2] | Mask[d2] | Mask[e2] | Mask[f2];
                          
    CenterMask =   Mask[c3] | Mask[d3] | Mask[e3] | Mask[f3]
                 | Mask[c4] | Mask[d4] | Mask[e4] | Mask[f4]
                 | Mask[c5] | Mask[d5] | Mask[e5] | Mask[f5]
                 | Mask[c6] | Mask[d6] | Mask[e6] | Mask[f6];
                                                            
                          
    /*
        King safety helpers
    */
    KingFilesMask[FileA] = FileMask[FileA] | FileMask[FileB] | FileMask[FileC];
    KingFilesMask[FileB] = FileMask[FileA] | FileMask[FileB] | FileMask[FileC];
    KingFilesMask[FileC] = FileMask[FileA] | FileMask[FileB] | FileMask[FileC];
    KingFilesMask[FileD] = FileMask[FileC] | FileMask[FileD] 
                         | FileMask[FileE] | FileMask[FileF];
    KingFilesMask[FileE] = FileMask[FileC] | FileMask[FileD] 
                         | FileMask[FileE] | FileMask[FileF];
    KingFilesMask[FileF] = FileMask[FileF] | FileMask[FileG] | FileMask[FileH];
    KingFilesMask[FileG] = FileMask[FileF] | FileMask[FileG] | FileMask[FileH];
    KingFilesMask[FileH] = FileMask[FileF] | FileMask[FileG] | FileMask[FileH];
    
    
    /*
        XXX big redundancy!!
        also try real different masks
    */
    for (i = 0; i < 64; i++) {    
        if (File(i) == FileA) {
            KingPressureMask[i] = KingSafetyMask[i + 1];
        } else if (File(i) == FileH) {
            KingPressureMask[i] = KingSafetyMask[i - 1];
        } else {
            KingPressureMask[i] = KingSafetyMask[i];
        }
    }     
    
    for (i = 0; i < 64; i++) {    
        if (File(i) == FileA) {
            KingPressureMask1[i] = KingSafetyMask1[i + 1];
        } else if (File(i) == FileH) {
            KingPressureMask1[i] = KingSafetyMask1[i - 1];
        } else {
            KingPressureMask1[i] = KingSafetyMask1[i];
        }
    }         

    /* space masks */
    SpaceMask[WHITE] = Mask[c2] | Mask[c3] | Mask[c4]
                     | Mask[d2] | Mask[d3] | Mask[d4]
                     | Mask[e2] | Mask[e3] | Mask[e4]
                     | Mask[f2] | Mask[f3] | Mask[f4];
    SpaceMask[BLACK] = Mask[c7] | Mask[c6] | Mask[c5]
                     | Mask[d7] | Mask[d6] | Mask[d5]
                     | Mask[e7] | Mask[e6] | Mask[e5]
                     | Mask[f7] | Mask[f6] | Mask[f5];                          
}

void SetupCastleMasks(state_t *s) {    
    /* 
        only 4 castle masks, but 5 castle conditions (no_castle is there too) 
        so we must correct array indexes
    */
    CastleMask[wck-1] = Mask[s->wking_start] ^ Mask[g1] ^ Mask[s->wrrook_start] ^ Mask[f1];
    CastleMask[wcq-1] = Mask[s->wking_start] ^ Mask[c1] ^ Mask[s->wlrook_start] ^ Mask[d1];
    CastleMask[bck-1] = Mask[s->bking_start] ^ Mask[g8] ^ Mask[s->brrook_start] ^ Mask[f8];
    CastleMask[bcq-1] = Mask[s->bking_start] ^ Mask[c8] ^ Mask[s->blrook_start] ^ Mask[d8];  
}

#if !defined(SPEC)
const
#endif
BITBOARD RankAttacks(const BITBOARD occ, const unsigned int sq) {
   unsigned int f = sq &  7;
   unsigned int r = sq & ~7; 
   unsigned int o = (unsigned int)(occ >> (r+1)) & 63;
   return (BITBOARD) firstRankAttacks[o][f] << r;
}

#if !defined(SPEC)
const
#endif
BITBOARD FileAttacks(BITBOARD occ, const unsigned int sq) {
   unsigned int o;
   unsigned int f = sq & 7;
   occ   =   0x0101010101010101ULL & (occ   >> f); 
   o     = ( 0x0080402010080400ULL *  occ ) >> 58;
   return  ( aFileAttacks[o][sq>>3]    ) << f;   
}

#if !defined(SPEC)
const
#endif
BITBOARD DiagonalAttacks(BITBOARD occ, const unsigned int sq) {
   unsigned int o;
   unsigned int f = sq & 7;
   occ   = ( DiagMaska1h8[sq]   &  occ );
   o     = ( 0x0202020202020202ULL *  occ ) >> 58;
   return  ( DiagMaska1h8[sq]   &  fillUpAttacks[o][f] );
}

#if !defined(SPEC)
const
#endif
BITBOARD AntiDiagAttacks(BITBOARD occ, const unsigned int sq) {
   unsigned int f = sq & 7;
   unsigned int o;
   occ   = ( DiagMaska8h1[sq]   &  occ );
   o     = ( 0x0202020202020202ULL *  occ ) >> 58;
   return  ( DiagMaska8h1[sq]   &  fillUpAttacks[o][f] );
} 

BITBOARD BishopAttacks(state_t *s, const int sq) {
    return DiagonalAttacks(s->All, sq) | AntiDiagAttacks(s->All, sq);
}

BITBOARD RookAttacks(state_t *s, const int sq) {
    return RankAttacks(s->All, sq) | FileAttacks(s->All, sq);
}
