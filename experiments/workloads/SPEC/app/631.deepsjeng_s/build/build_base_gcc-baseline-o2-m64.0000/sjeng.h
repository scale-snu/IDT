/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2008 Gian-Carlo Pascutto
   
    File: sjeng.h
    Purpose: global definitions                  
*/

#ifndef SJENG_H
#define SJENG_H

#include "config.h"

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if defined(WIN32) || defined(WIN64) || defined(SPEC_WINDOWS)
#define NOMINMAX  /* prevent defines clash */
#define WIN32_LEAN_AND_MEAN
typedef unsigned __int64 BITBOARD;
#define CONST64U(x) (x##UI64)
#define CONST64(x)  (x##I64)
#define SUFF64     "%I64u"
#define SUFF64_8   "%8I64u"
#if (_MSC_VER >= 1400) /* VC8+ Disable all deprecation warnings */
    #pragma warning(disable : 4996) 
#endif /* VC8+ */
#else
typedef unsigned long long BITBOARD;
#define CONST64U(x) (x##ull)
#define CONST64(x)  (x##ll)
#define SUFF64     "%ull"
#define SUFF64_8   "%8ull"
#endif

#define DIE  (*(int *)(NULL) = 0)

#define FALSE 0
#define TRUE  1

#define WHITE 0
#define BLACK 1

#define TOMOVE(s)       ((s)->white_to_move ? 0 : 1)
#define NOTTOMOVE(s)    ((s)->white_to_move ? 1 : 0)

#define HASH(s,x,y)     ((s)->hash     ^= zobrist[(x)][(y)])
#define PAWNHASH(s,x,y) ((s)->pawnhash ^= zobrist[(x)][(y)])

/* Game phases */
#define OPENING      0
#define MIDDLEGAME   1
#define ENDGAME      2

/* define names for piece constants: */
#define invalid 0
#define wpawn   1
#define bpawn   2
#define wknight 3
#define bknight 4
#define wking   5
#define bking   6
#define wrook   7
#define brook   8
#define wqueen  9
#define bqueen  10
#define wbishop 11
#define bbishop 12
#define npiece  13

#define PIECET(s,x)    ((((s)->sboard[(x)])+1)>>1)
#define PIECESIDE(s,x) ((((s)->sboard[(x)])+1)&1)

#define PIECETYPE(x)   (((x)+1)>>1)
#define PIECECOLOR(x)  (((x)+1)&1)

#define pawn    1
#define knight  2
#define king    3
#define rook    4
#define queen   5
#define bishop  6

/* result flags: */
#define no_result        0
#define stalemate        1
#define white_is_mated   2
#define black_is_mated   3
#define draw_by_fifty    4
#define draw_by_rep      5
#define draw_by_material 6

/* some bitboard stuff */
#define EMPTY          (0)
#define UNIT           ((BITBOARD)1)
#define REMOVE(a,b)    ((a) = ((a)^(UNIT<<(b))))
#define REMOVEFIRST(a) ((a) = ((a) & ((a)-1)))
#define FULLRANK       (255)
#define TWOFULLRANKS   (65535)

#define File(x)        ((x) & 7)
#define Rank(x)        ((x) >> 3)
#define IsSqBlack(x)   ((((x) >> 3) + ((x) & 7)) & 1)

/* castle flags: */
#define no_castle  0
#define wck        1
#define wcq        2
#define bck        3
#define bcq        4

/*
    6 + 6 + 4 + 1 + 4 + 3 = 24 bits
    1  1111  111 1111 111111 111111
    ep  cap  cst prom   from     to
*/
typedef int move_s;

#define TARGET(x)    ( (x)        & 0x3F)
#define FROM(x)      (((x) >>  6) & 0x3F)
#define PROMOTED(x)  (((x) >> 12) & 0x0F)
#define CASTLED(x)   (((x) >> 16) & 0x07)
#define CAPTURED(x)  (((x) >> 19) & 0x0F)
#define EP(x)        (((x) >> 23) & 0x01)

struct move_x {
  int epsq;
  int fifty;
  int castleflag;  
  int psq_score;  
  BITBOARD hash;
  BITBOARD pawnhash;  
};

struct hashkey_t {
  BITBOARD hashkey;
};

#define BIG_BUFF     8192
#define STR_BUFF      512
#define SAN_MAX        10     /* prolly real max is 7? */  
#define MOVE_BUFF     240
#define INF       1000000
#define MATE        32000
#define EVALMAX     10000
#define PV_BUFF        64
#define GAME_LENGTH  1000
#define MINDEPTH        2
#define MAXDEPTH       60
#define PLY             4

#define ADDMATERIAL(s,x)    (s->Material += material[(x)])
#define REMOVEMATERIAL(s,x) (s->Material -= material[(x)])

#define UPPER     1
#define LOWER     2
#define EXACT     3
#define HMISS     4
#define DUMMY     0

#define LOSS      0
#define WIN       1
#define DRAW      2

#define NONE      0
#define SINGLE    1 

#define MAX_CPU   8

#define LOGGING

#endif

