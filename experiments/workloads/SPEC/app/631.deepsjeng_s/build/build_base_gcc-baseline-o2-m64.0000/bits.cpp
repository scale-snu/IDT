/*
    Sjeng - a chess playing program
    Copyright (C) 2001-2009 Gian-Carlo Pascutto

    File: bits.c                                             
    Purpose: bit-twiddling functions
 
*/

#include "sjeng.h"

#if !defined(SPEC)
const
#endif
int PopCount (BITBOARD b) {
    int c = 0;

    while (b) {
        b &= b - 1;
        c++;
    }

    return c;
}

/*
    DeBruijn magic data
*/
static const BITBOARD magic = CONST64U(0x03f08c5392f756cd);
static const int magictable[64] = {
     0,  1, 12,  2, 13, 22, 17,  3,
    14, 33, 23, 36, 18, 58, 28,  4,
    62, 15, 34, 26, 24, 48, 50, 37,
    19, 55, 59, 52, 29, 44, 39,  5,
    63, 11, 21, 16, 32, 35, 57, 27,
    61, 25, 47, 49, 54, 51, 43, 38,
    10, 20, 31, 56, 60, 46, 53, 42,
     9, 30, 45, 41,  8, 40,  7,  6,
};

#if !defined(SPEC)
const
#endif
int FindFirst (const BITBOARD b) {
    const BITBOARD lsb = b & -b;    
    return magictable[(lsb * magic) >> 58];
}

int FindFirstRemove(BITBOARD * const b) {    
    const BITBOARD lsb = (*b) & -(*b);
    *b ^= lsb;
    return magictable[(lsb * magic) >> 58];  
}

unsigned char last_bit[65536];
#if !defined(SPEC)
const
#endif
int FindLast(BITBOARD b)
{
    int result = 0;
    if (b > 0xFFFFFFFF) {
        b >>= 32;
        result = 32;
    }
    if (b > 0xFFFF) {
        b >>= 16;
        result += 16;
    }
    return result + last_bit[b];
} 

#if !defined(SPEC)
const
#endif
int ThickPopCount(const BITBOARD bb){
    BITBOARD b = bb;
    b -= ((b>>1) & 0x5555555555555555ULL);
    b = ((b>>2) & 0x3333333333333333ULL) + (b & 0x3333333333333333ULL);
    b = ((b>>4) + b) & 0x0F0F0F0F0F0F0F0FULL;
    b *= 0x0101010101010101ULL;
    return (int)(b >> 56);    
} 

