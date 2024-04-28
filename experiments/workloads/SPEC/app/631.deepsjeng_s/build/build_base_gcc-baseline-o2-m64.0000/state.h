#ifndef STATE_H
#define STATE_H

#include "config.h"

#include "sjeng.h"

/*
    defines
*/
struct state_t {
    /* who am i */
    int threadid;    
    /* board state */
    int sboard[64];
    BITBOARD All;         
    BITBOARD BlackPieces, WhitePieces;
    BITBOARD BitBoard[13];

    int Material;
    int psq_score;
    int npieces[13];
    int castleflag;

    int ep_square, white_to_move, wking_loc, bking_loc;
    int ply, fifty;    
    
    BITBOARD hash;
    BITBOARD pawnhash;         
    
    move_x path_x[PV_BUFF];
    move_s path[PV_BUFF];
    int    plyeval[PV_BUFF];    

    /* search state */
    struct {
        move_s killer1;
        move_s killer2;
        move_s killer3;
        move_s killer4;
    } killerstack[PV_BUFF];

    BITBOARD nodes, qnodes;
    int maxply;
    int checks[PV_BUFF];       
    
    /* 
        search statistics must be in state
        so they are CPU-local
    */
    unsigned int TTProbes;
    unsigned int TTHits;
    unsigned int TTStores;
    unsigned int TTColls;   
 
    /* 
        chess960 mode, these are read only
        but handy to have in state_t
    */
    int wking_start;
    int bking_start;
    int wlrook_start;
    int wrrook_start;
    int blrook_start;
    int brrook_start;        
    /*
        this serves a second purpose as padding
    */        
    BITBOARD hash_history[GAME_LENGTH];  
};

struct gamestate_t {    
    int cur_score;    
    int phase;
    int root_to_move;
    int comp_color, result, i_depth;    
    int moves_to_tc, min_per_game, sec_per_game, inc; 
    int time_left, opp_time, time_for_move;
    int fixed_time;
    int maxdepth;
    /* */
    int move_number;
    move_s game_history[GAME_LENGTH];
    move_x game_history_x[GAME_LENGTH];     
    BITBOARD pvnodecount;
    int start_time;    
    /* root search control */          
    unsigned int pv_best;    
    int legals;
    int failed;
    int failedhigh;
    int extendedtime;
    int time_exit, time_failure;
};

struct scoreboard_t {
    int cpus;
    volatile int running;    
    struct {
        int alpha;
        int beta;
        int depth;        
    } searchstart[MAX_CPU];    
    volatile int score[MAX_CPU];
    state_t threadstate[MAX_CPU];    
};

/*
    data
*/
extern state_t      state;
extern gamestate_t  gamestate;
extern scoreboard_t scoreboard;

/* 
    functions
*/

#endif
