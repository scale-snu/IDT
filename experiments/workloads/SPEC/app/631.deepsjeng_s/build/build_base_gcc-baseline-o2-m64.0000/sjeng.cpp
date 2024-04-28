/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2008 Gian-Carlo Pascutto

    File: sjeng.c
    Purpose: main program, xboard/user interface
*/

#include "config.h"
#include "sjeng.h"

#include "extvars.h"
#include "bitboard.h"
#include "make.h"
#include "ttable.h"
#include "epd.h"
#include "utils.h"
#include "preproc.h"
#include "state.h"
#include "initp.h"
#include "pawn.h"
#include "search.h"
#include "draw.h"

char buffered_command[20][BIG_BUFF];
int buffered_count;
int is_pondering, allow_pondering, is_analyzing;
unsigned int TTSize;
int uci_mode;
int uci_chess960_mode;
int uci_showcurrline;
int uci_showrefutations;
int uci_limitstrength;
int uci_elo;
int uci_multipv;
int cfg_logging;
char cfg_logfile[STR_BUFF];
int contempt;
int time_check_log;
int global_id;
int EGTBHits;
int EGTBProbes;

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    
    start_up();

    init_parameters(&gamestate);

    initialize_zobrist(&state);
    SetupPrecalculatedData();
    preprocess_prepare();

    /*
        randomize again
    */
    mysrand(12345);

    init_game(&gamestate, &state);

    initialize_hash(&state);

    clear_tt();
    clear_pawn_tt();

    state.ply = 0;

    gamestate.moves_to_tc =  40;
    gamestate.min_per_game = 15;
    gamestate.sec_per_game =  0;
    gamestate.time_left = 15 * 60 * 100;

    contempt = 0;

    gamestate.comp_color = 0;
    is_pondering = FALSE;
    allow_pondering = TRUE;
    is_analyzing = FALSE;
    gamestate.fixed_time = FALSE;
    gamestate.phase = OPENING;
    gamestate.root_to_move = WHITE;    
    time_check_log = 14;    // 16384

    gamestate.move_number = 0;
    memset(gamestate.game_history, 0, sizeof(gamestate.game_history));
    memset(gamestate.game_history_x, 0, sizeof(gamestate.game_history_x));

    state.hash_history[gamestate.move_number] = state.hash;

    memset(&buffered_command[0], 0, sizeof(buffered_command));
    buffered_count = 0;
    
    /* SPEC version: take EPD testset from commandline */
    if (argc == 2) {
        run_epd_testsuite(&gamestate, &state, argv[1]);    
    } else {
        myprintf("Please specify the workfile.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

