#ifndef UTILS_H
#define UTILS_H

#include "sjeng.h"
#include "state.h"

/*
    data
*/
extern int root_scores[MOVE_BUFF];
extern char multipv_strings[MOVE_BUFF][STR_BUFF];
extern int multipv_scores[MOVE_BUFF];

/* 
    functions
*/
void extract_current_path(state_t *s);
void extract_current_bestline(state_t *s);
move_s extract_ponder_move(state_t *s);
void mysrand(unsigned int seed);
unsigned int myrandom(void);
void post_thinking (state_t *s, int score, move_s pv, char *searching_move, int mc);
void post_fl_thinking (state_t *s, int score, move_s failmove, char *searching_move, int mc);
void post_fh_thinking (state_t *s, int score, move_s failmove, char *searching_move, int mc);
void post_multipv_thinking(state_t *s, int score, int mc, move_s move);
unsigned short compact_move(move_s move);
void init_game(gamestate_t *g, state_t *s);
void reset_piece_square(state_t *s);
void dumpsearchstat(state_t *s);
void display_board(state_t *s, int color);
void myprintf(const char *fmt, ...);
void comp_to_san(state_t *s, move_s move, char *str);
void rinput(char *str, int n, FILE *stream);
int interrupt(void);
int allocate_time(gamestate_t *g, int alloc);
int rtime(void);
int rdifftime(int end, int start);
void comp_to_coord(state_t *s, move_s move, char *str);
void stringize_pv(state_t *s, char *str);
move_s extract_ponder_move(state_t *s);
#if !defined(SPEC)
const int logL(int num);
#else
int logL(int num);
#endif
void start_up(void);
int is_move(char *str);
int verify_coord(state_t *s, char *input, move_s *move);
void make_text_moves(gamestate_t *g, state_t *s, char *movebuff);
void reset_piece_square(state_t *s);
int rdifftime(int end, int start);
int rtime(void);
int elo_to_depth(int elo);
int elo_to_blunder(int elo);

#endif

