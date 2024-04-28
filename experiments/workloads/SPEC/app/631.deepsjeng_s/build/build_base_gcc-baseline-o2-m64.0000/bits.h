#ifndef BITS_H
#define BITS_H
/*
    data
*/
#include "config.h"
#include "sjeng.h"

extern unsigned char last_bit[65536];

/*
    functions
*/
#if !defined(SPEC)
const int PopCount (BITBOARD b);
const int FindFirst (const BITBOARD b);
int FindFirstRemove(BITBOARD * const b);
const int FindLast(BITBOARD B);
const int ThickPopCount(const BITBOARD b);
#else
int PopCount (BITBOARD b);
int FindFirst (const BITBOARD b);
int FindFirstRemove(BITBOARD * const b);
int FindLast(BITBOARD B);
int ThickPopCount(const BITBOARD b);
#endif

#endif

