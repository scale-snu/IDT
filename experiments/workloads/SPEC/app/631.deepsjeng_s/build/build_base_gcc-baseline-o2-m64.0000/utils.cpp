/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2008 Gian-Carlo Pascutto

    File: utils.c                                   
    Purpose: misc. functions used throughout the program

*/
#include "config.h"
#include "sjeng.h"
#include "extvars.h"
#include "bitboard.h"
#include "ttable.h"
#include "board.h"
#include "generate.h"
#include "preproc.h"
#include "bitboard.h"
#include "moves.h"
#include "make.h"
#include "bits.h"
#include "attacks.h"
#include "utils.h"
#include "squares.h"

#include <stdarg.h>
#include <limits.h>

static unsigned int s1, s2, s3;

/* multipv information */
int root_scores[MOVE_BUFF];
char multipv_strings[MOVE_BUFF][STR_BUFF];
int multipv_scores[MOVE_BUFF];


int elo_to_depth(int elo) {
    if (elo >= 2400) {
        return 60;
    } else if (elo < 2400 && elo >= 2200) {    
        return 10;
    } else if (elo < 2200 && elo >= 2000) {    
        return 8;
    } else if (elo < 2000 && elo >= 1800) {
        return 6;
    } else if (elo < 1800 && elo >= 1400) {
        return 4;
    } else if (elo < 1400 && elo >= 1000) {
        return 3;
    } else if (elo < 1000) {
        return 2;
    }
    
    return 60;
}

int elo_to_blunder(int elo) {
    if (elo >= 2400) {
        return 100;
    } else if (elo < 2400 && elo >= 2200) {    
        return 90;
    } else if (elo < 2200 && elo >= 2000) {    
        return 80;
    } else if (elo < 2000 && elo >= 1600) {
        return 70;
    } else if (elo < 1600 && elo >= 1200) {
        return 60;
    } else if (elo < 1200 && elo >= 800) {
        return 50;
    } else if (elo < 800) {
        return 0;
    }
    
    return 100;
}

unsigned short compact_move(move_s move) {
    unsigned int res;
    unsigned int bfrom = FROM(move);
    unsigned int bto   = TARGET(move);
    unsigned int cst   = CASTLED(move);
    unsigned int prom  = PROMOTED(move);

    res = (cst << 12) | (bfrom << 6) | bto;

    if (prom) {
        res |= PIECETYPE(prom) << 12;
    }

    return res;
}

#if !defined(SPEC)
const
#endif
int logL(int num) {
    int b = 0;

    while (num >>= 1) {
        b++;
    }

    return b;
}

void dumpsearchstat(state_t *s) {
    if (!uci_mode) {    
    }
}

int allocate_time(gamestate_t *g, int alloc) {
    /* 
        calculate the ammount of time the program can use in its search, measured
        in centi-seconds (calculate everything in float for more accuracy as
        we go, and return the result as a int) 
    */
    double allocated_time = 0.0, move_speed = 24.0;
    double mttc;

    /* 
        sudden death time allocation: 
    */
    if (!gamestate.moves_to_tc) {
        if (allow_pondering) {
            move_speed -= 3.0;
        }

        if (gamestate.inc) {
            move_speed -= 9.0;
        }

        if (allow_pondering && gamestate.inc) {
            move_speed -= 2.0;
        }

        /* allocate our base time: */
        allocated_time = gamestate.time_left / move_speed;

        /* add our increment if applicable: */
        if (gamestate.inc) {
            if (gamestate.time_left - allocated_time - gamestate.inc > 500) {
                allocated_time += gamestate.inc;
            } else if (gamestate.time_left - allocated_time - (gamestate.inc * 2.0 / 3.0) > 100) {
                allocated_time += gamestate.inc * 2.0 / 3.0;
            }
        }
    }  else {
        /* conventional clock time allocation: */
        mttc = gamestate.moves_to_tc + 1.0;
        if (mttc > move_speed) {
            mttc = move_speed + 1.0;
        }
        if (!uci_mode) {
            allocated_time =
                (
                    ((double)gamestate.time_left - 100.) /
                    (double)(mttc - (((gamestate.move_number / 2) % gamestate.moves_to_tc)))
                );
        } else {
            allocated_time = ((double)gamestate.time_left - 100.) / mttc;
        }

        /* 
            we tend to exit after 1/2.5 of our time use, so
            allocate a bit more 
        */
        /* 
            this should depend on pondering! we can probably 
            use *2.5 with pondering, and *1.5 without
        */
        if (allow_pondering) {
            if (allocated_time * 2.5 < (gamestate.time_left - 100)) {
                allocated_time *= 2.5;
            }
        } else {
            if (allocated_time * 1.5 < (gamestate.time_left - 100)) {
                allocated_time *= 1.5;
            }
        }
    }

    return ((int)allocated_time);
}

void comp_to_san(state_t *s, move_s move, char str[]) {
    move_s moves[MOVE_BUFF];
    move_s evade_moves[MOVE_BUFF];
    char type_to_char[] = {
        'F', 'P', 'P', 'N', 'N', 'K', 'K', 'R', 'R', 'Q', 'Q', 'B', 'B', 'E'
    };
    int i, num_moves, evasions, ambig, mate;
    int f_rank, t_rank, converter;
    char f_file, t_file;
    int ic;
    int *sboard = s->sboard;
    
    f_rank = rank(FROM(move));
    t_rank = rank(TARGET(move));
    converter = (int)'a';
    f_file = file(FROM(move)) + converter - 1;
    t_file = file(TARGET(move)) + converter - 1;    

    if ((sboard[FROM(move)] == wpawn) || (sboard[FROM(move)] == bpawn)) {
        if (sboard[TARGET(move)] == npiece && !EP(move)) {
            if (!PROMOTED(move)) {
                sprintf(str,"%c%d",t_file,t_rank);
            } else {
                sprintf(str,"%c%d=%c",t_file,t_rank,type_to_char[PROMOTED(move)]);
            }
        } else {
            if (!PROMOTED(move)) {
                sprintf(str,"%cx%c%d",f_file,t_file,t_rank);
            } else {
                sprintf(str,"%cx%c%d=%c",f_file,t_file,t_rank,type_to_char[PROMOTED(move)]);
            }
        }
    } else if (CASTLED(move) != no_castle) {
        if (CASTLED(move) == wck || CASTLED(move) == bck) {
            sprintf(str,"O-O");
        } else {
            sprintf(str,"O-O-O");
        }
    } else {
        ambig = -1;
        num_moves = 0;

        num_moves = gen(s, moves);

        ic = in_check(s);

        /* 
            check whether there is another, identical piece that
	    could also move to this square 
        */
        for (i = 0; i < num_moves; i++) {
            if ((TARGET(moves[i]) == TARGET(move)) 
               && (s->sboard[FROM(moves[i])] == s->sboard[FROM(move)])
               && (FROM(moves[i]) != FROM(move))) {

                /* would it be a legal move ? */
                make(s, moves[i]);
                if (check_legal(s, moves[i])) {
                    unmake(s, moves[i]);
                    ambig = i;
                    break;
                }

                unmake(s, moves[i]);
            }
        }

        if (ambig != -1) {
            if (s->sboard[TARGET(move)] == npiece) {
                if (file(FROM(moves[ambig])) != file(FROM(move))) {
                    sprintf(str,"%c%c%c%d",type_to_char[sboard[FROM(move)]],f_file,t_file,t_rank);
                } else {
                    sprintf(str,"%c%d%c%d",type_to_char[sboard[FROM(move)]],f_rank,t_file,t_rank);
                }
            } else {
                if (file(FROM(moves[ambig])) != file(FROM(move))) {
                    sprintf(str,"%c%cx%c%d",type_to_char[sboard[FROM(move)]],f_file,t_file,t_rank);
                } else {
                    sprintf(str,"%c%dx%c%d",type_to_char[sboard[FROM(move)]],f_rank,t_file,t_rank);
                }
            }
        } else {
            if (s->sboard[TARGET(move)] == npiece) {
                sprintf(str,"%c%c%d",type_to_char[sboard[FROM(move)]],t_file,t_rank);
            } else {
                sprintf(str,"%cx%c%d",type_to_char[sboard[FROM(move)]],t_file,t_rank);
            }
        }
    }
    
    make(s, move);

    if (!check_legal(s, move)) {
        strcpy(str, "illg");
        unmake(s, move);
        return;
    }

    if (in_check(s)) {
        mate = TRUE;
        evasions = gen(s, evade_moves);

        for (i = 0; i < evasions; i++) {
            make(s, evade_moves[i]);
            if (check_legal(s, evade_moves[i])) {
                mate = FALSE;
                unmake(s, evade_moves[i]);
                break;
            }

            unmake(s, evade_moves[i]);
        }

        if (mate == TRUE) {
            strcat(str,"#");
        } else {
            strcat(str,"+");
        }
    }

    unmake(s, move);
}

void comp_to_coord(state_t *s, move_s move, char *str) {
    /* convert a move_s internal format move to coordinate notation: */
    int prom, from, castled, target, f_rank, t_rank, converter;
    char f_file, t_file;

    prom = PROMOTED(move);
    from = FROM(move);
    target = TARGET(move);
    castled = CASTLED(move);

    if (from == 0 && target == 0) {
        sprintf(str, "0000");
        return;
    }

    f_rank = rank(from);
    t_rank = rank(target);
    converter = ((int)'a') - 1;
    f_file = file(from) + converter;
    t_file = file(target) + converter;

    if (castled && uci_chess960_mode) {
        if (castled == wck) {
            target = s->wrrook_start;
        } else if (castled == wcq) {
            target = s->wlrook_start;
        } else if (castled == bck) {
            target = s->brrook_start;
        } else if (castled == bcq) {
            target = s->blrook_start;
        }

        t_file = file(target) + converter;
        t_rank = rank(target);
        sprintf(str, "%c%d%c%d", f_file,f_rank,t_file,t_rank);
    } else if (!prom) {
        /* "normal" move: */
        sprintf(str, "%c%d%c%d", f_file,f_rank,t_file,t_rank);
    } else {

        /* promotion move: */
        if (PIECETYPE(prom) == knight) {
            sprintf(str, "%c%d%c%dn", f_file,f_rank,t_file,t_rank);
        } else if (PIECETYPE(prom) == rook) {
            sprintf(str, "%c%d%c%dr", f_file,f_rank,t_file,t_rank);
        } else if (PIECETYPE(prom) == bishop) {
            sprintf(str, "%c%d%c%db", f_file,f_rank,t_file,t_rank);
        } else {
            sprintf(str, "%c%d%c%dq", f_file,f_rank,t_file,t_rank);
        }
    }
}

void display_board(state_t *s, int color) {
    /* prints a text-based representation of the board: */
    const char *line_sep = "+----+----+----+----+----+----+----+----+";
    const char *piece_rep[14] = {
        "!!", " P", "*P", " N", "*N", " K", "*K", " R", "*R", " Q",
        "*Q", " B", "*B", "  "
    };
    int a, b, c;
    int *sboard = s->sboard;

    if (color % 2) {
        myprintf("  %s\n",line_sep);
        for (a = 0; a < 8; a++) {
            myprintf("%d |", 8 - a);
            for (b = 0; b < 8; b++) {
                c = a * 8 + b;                
                myprintf(" %s |", piece_rep[sboard[c]]);                
            }

            myprintf("\n  %s\n",line_sep);
        }

        myprintf("\n     a    b    c    d    e    f    g    h\n\n");
    } else {
        myprintf("  %s\n",line_sep);
        for (a = 7; a >= 0; a--) {
            myprintf("%d |", a+1);
            for (b = 7; b >= 0; b--) {
                c = a * 8 + b;                
                myprintf(" %s |", piece_rep[sboard[c]]);                
            }

            myprintf("\n  %s\n",line_sep);
        }

        myprintf("\n     h    g    f    e    d    c    b    a\n\n");
    }
}

void init_game(gamestate_t *g, state_t *s) {
    /* set up a new game: */
    static const int init_board[64] = {        
        brook, bknight, bbishop, bqueen, bking, bbishop, bknight, brook,         
        bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, bpawn, 
        npiece, npiece, npiece, npiece, npiece, npiece, npiece, npiece, 
        npiece, npiece, npiece, npiece, npiece, npiece, npiece, npiece, 
        npiece, npiece, npiece, npiece, npiece, npiece, npiece, npiece, 
        npiece, npiece, npiece, npiece, npiece, npiece, npiece, npiece, 
        wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, wpawn, 
        wrook, wknight, wbishop, wqueen, wking, wbishop, wknight, wrook,                         
    };

    memcpy(s->sboard, init_board, sizeof(init_board));

    s->white_to_move = 1;
    s->ep_square = 0;
    s->castleflag = 0x1E;
    s->wking_loc = e1;
    s->bking_loc = e8;
    s->wking_start = s->wking_loc;
    s->bking_start = s->bking_loc;
    s->wlrook_start = a1;
    s->wrrook_start = h1;
    s->blrook_start = a8;
    s->brrook_start = h8;
    SetupCastleMasks(s);

    gamestate.result = no_result;    

    s->Material = 0;

    reset_piece_square(s);
        
    s->fifty = 0;
    s->ply = 0;
    s->threadid = 0;

    gamestate.phase = OPENING;    
}

int is_move(char *str) {
    /* check to see if the input string is a move or not.  Returns true if it
     is in a move format supported by Sjeng. */
    if (isalpha(str[0]) && isdigit(str[1]) && isalpha(str[2]) && isdigit(str[3])) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void extract_current_path(state_t *s) {
    char output[SAN_MAX];
    char str[SAN_MAX * PV_BUFF];
    int i;
    
    str[0] = '\0';
    
    for (i = 1; i < s->ply; i++) {
        if (FROM(s->path[i]) == 0 && TARGET(s->path[i]) == 0) {
            strcat(str, "0000");
        } else {
            comp_to_coord(s, s->path[i], output);
            strcat(str, output);
        }        
        strcat(str, " ");
    }
    strcat(str, "\n");

    myprintf(str);
}

static void hash_extract_pv(state_t *s, int level, char *str) {
    int xdummy, bm, j;
    unsigned int mv;
    move_s moves[MOVE_BUFF];
    int num_moves;
    char output[STR_BUFF];
    static BITBOARD levelstack[PV_BUFF + 1];
    int i;

    levelstack[level] = s->hash;

    for (i = level + 1; i <= PV_BUFF; i++) {
        if (levelstack[i] == levelstack[level]) {
            return;
        }
    }

    level--;
    if (!level) {
        return;
    }

    if (ProbeTT(s, &xdummy,0,0,&mv,&xdummy,&xdummy,&xdummy,&xdummy,0) != HMISS) {
        if (in_check(s)) {
            num_moves = gen_evasions(s, moves, in_check(s));
        } else {
            num_moves = gen(s, moves);
        }

        bm = -1;
        for (j = 0; j < num_moves; j++) {
            if (compact_move(moves[j]) == mv) {
                bm = j;
            }
        }

        if ((bm >= 0) && (bm < num_moves)) {
            if (!uci_mode) {
                comp_to_san(s, moves[bm], output);
            } else {
                comp_to_coord(s, moves[bm], output);
            }

            make(s, moves[bm]);
            if (check_legal(s, moves[bm])) {                                
                strcat(str,output);                
                strcat(str," ");                

                hash_extract_pv(s, level,str);
            }

            unmake(s, moves[bm]);
        }        
    }
}

move_s extract_ponder_move(state_t *s) {
    unsigned int mv;
    int xdummy, num_moves, bm, j, result;
    move_s moves[MOVE_BUFF];
    
    result = 0;
    
    if (ProbeTT(s, &xdummy, 0, 0, &mv, &xdummy, &xdummy, &xdummy, &xdummy, 0) != HMISS) {
        if (in_check(s)) {
            num_moves = gen_evasions(s, moves, in_check(s));
        } else {
            num_moves = gen(s, moves);
        }

        bm = -1;
        for (j = 0; j < num_moves; j++) {
            if (compact_move(moves[j]) == mv) {
                bm = j;
            }
        }
       
        if (bm > -1) {
            make(s, moves[bm]);
            if (check_legal(s, moves[bm])) {                                
                result = moves[bm];
            }
            unmake(s, moves[bm]);        
        }
    }
    
    return result;
}

void extract_current_bestline(state_t *s) {
    char output[SAN_MAX * PV_BUFF];    

    output[0] = '\0';
    
    hash_extract_pv(s, 60, output);
    strcat(output, "\n");
    
    myprintf(output);
}

void stringize_pv(state_t *s, char *str) {
    char output[STR_BUFF];

    str[0] = '\0';
    output[0] = '\0';

    hash_extract_pv(s, 60, str);
}

void post_thinking(state_t *s, int score, move_s pv, char *searching_move, int mc) {    
    int remake = 0;
    int elapsed;
    char hashpv[STR_BUFF];
    int sign = 1;

    if (gamestate.i_depth <= MINDEPTH && gamestate.maxdepth > MINDEPTH) {
        return;
    }
    
    elapsed = rdifftime(rtime(), gamestate.start_time);
    
    if (!uci_mode) {
        myprintf("%2d %7d %5d %8llu  ",gamestate.i_depth,score,elapsed,s->nodes);
    } else {
        myprintf("info currmove %s currmovenumber %d\n",searching_move,mc);
        
        if (abs(score) < 10000) {
            myprintf("info depth %d seldepth %d time %d nodes %llu tbhits %d score cp %d multipv 1 pv ",
                 gamestate.i_depth,s->maxply,elapsed * 10,s->nodes, EGTBHits, score);
        } else {
            if (score < 0) {
                score = -score;
                sign = -sign;
            }
            myprintf("info depth %d seldepth %d time %d nodes %llu tbhits %d score mate %d multipv 1 pv ",
                 gamestate.i_depth,s->maxply,elapsed * 10,s->nodes, EGTBHits, sign * (int)((MATE - score) / 2));
        }
    }

    /* if root move is already/still played, back it up */
    /* 25-06-2000 our en passant info is unrecoverable here
       so we cannot gen.... */
    if (s->ply > 1) {
        unmake(s, pv);
        remake = 1;
    } 

    memset(hashpv, 0, sizeof(hashpv));
    char movestr[STR_BUFF];
    if (!uci_mode) {
        comp_to_san(s, pv, movestr);
    } else {
        comp_to_coord(s, pv, movestr);
    }
    strcpy(hashpv, movestr);
    strcat(hashpv, " ");

    make(s, pv);
    
    hash_extract_pv(s, 60, hashpv);
    myprintf("%s", hashpv);  

    unmake(s, pv);

    if (remake) {
        make(s, pv);
    }

    myprintf("\n");
}

void post_fh_thinking(state_t *s, int score, move_s failmove, char *searching_move, int mc) {

    /* post our thinking output: */
    int elapsed;
    char output[STR_BUFF];
    int sign = 1;

    if (gamestate.i_depth <= MINDEPTH && gamestate.maxdepth > MINDEPTH) {
        return;
    }    

    elapsed = rdifftime(rtime(),gamestate.start_time);
    if (!uci_mode) {
        myprintf("%2d %7d %5d %8llu  ",gamestate.i_depth,score,elapsed,s->nodes);
    } else {
        if (abs(score) < 10000) {
            myprintf("info depth %d seldepth %d time %d nodes %llu tbhits %d score cp %d lowerbound multipv 1 pv ",
                 gamestate.i_depth,s->maxply,elapsed * 10,s->nodes,EGTBHits,score);
        } else {
            if (score < 0) {
                score = -score;
                sign = -sign;
            }
            myprintf("info depth %d seldepth %d time %d nodes %llu tbhits %d score cp %d lowerbound multipv 1 pv ",
                 gamestate.i_depth,s->maxply,elapsed * 10,s->nodes,EGTBHits,sign * (int)((MATE - score) / 2));
        }
    }

    unmake(s, failmove);
    if (!uci_mode) {
        comp_to_san(s, failmove, output);
    } else {
        comp_to_coord(s, failmove, output);
    }

    make(s, failmove);
    if (!uci_mode) {
        myprintf("%s !!",output);
    } else {
        myprintf("%s", output);
        myprintf("\n");
        myprintf("info currmove %s currmovenumber %d",output,mc);
    }

    myprintf("\n");
}

void post_fl_thinking(state_t *s, int score, move_s failmove, char *searching_move, int mc) {

    /* post our thinking output: */
    int elapsed;
    char output[STR_BUFF];
    int sign = 1;

    if (gamestate.i_depth <= MINDEPTH && gamestate.maxdepth > MINDEPTH) {
        return;
    }    

    /* in xboard mode, follow xboard conventions for thinking output, otherwise
     output the iterative depth, human readable score, and the pv */
    elapsed = rdifftime(rtime(), gamestate.start_time);
if (!uci_mode)
    {
        myprintf("%2d %7d %5d %8llu  ",gamestate.i_depth,score,elapsed,s->nodes);
    } else {
        myprintf("info currmove %s currmovenumber %d\n",searching_move,mc);
        if (abs(score) < 10000)
        {
            myprintf("info depth %d seldepth %d time %d nodes %llu tbhits %d score cp %d upperbound multipv 1 pv ",
                 gamestate.i_depth,s->maxply,elapsed * 10,s->nodes,EGTBHits,score);
        } else {
            if (score < 0) {
                score = -score;
                sign = -sign;
            }
            myprintf("info depth %d seldepth %d time %d nodes %llu tbhits %d score cp %d upperbound multipv 1 pv ",
                 gamestate.i_depth,s->maxply,elapsed * 10,s->nodes,EGTBHits,sign * (int)((MATE - score) / 2));

        }
    }

    unmake(s, failmove);
    if (!uci_mode) {
        comp_to_san(s, failmove, output);
    } else {
        comp_to_coord(s, failmove, output);
    }

    make(s, failmove);
    
    if (!uci_mode) {
        myprintf("%s ??", output);
    } else {
        myprintf("%s", output);
    }
    
    myprintf("\n");
}


static void resort_multipv(void) {
    char temp[STR_BUFF];
    int i, j, temps;

    for (i = 1; i < MOVE_BUFF; i++) {
        for (j = i; j < MOVE_BUFF; j++) {
            if (multipv_scores[j] > multipv_scores[i]) {
                strcpy(temp, multipv_strings[j]);
                strcpy(multipv_strings[j], multipv_strings[i]);
                strcpy(multipv_strings[i], temp);

                temps = multipv_scores[j];
                multipv_scores[j] = multipv_scores[i];
                multipv_scores[i] = temps;
            }
        }
    }
}

void post_multipv_thinking(state_t *s, int score, int mc, move_s move) {
    int i;
    int elapsed;
    char output[SAN_MAX];
    char hashpv[STR_BUFF];
    int sign = 1;

    if (gamestate.i_depth <= MINDEPTH && gamestate.maxdepth > MINDEPTH) {
        return;
    }    
    
    memset(multipv_strings[mc], 0, STR_BUFF);

    multipv_scores[mc] = score;

    elapsed = rdifftime(rtime(), gamestate.start_time);

    if (abs(score) < 10000) {
        sprintf(multipv_strings[mc],
                "depth %d seldepth %d time %d nodes %llu tbhits %d score cp %d pv ", gamestate.i_depth,
                s->maxply, elapsed * 10, s->nodes, EGTBHits, score);
    } else {
        if (score < 0) {
            score = -score;
            sign = -sign;
        }
        sprintf(multipv_strings[mc],
                "depth %d seldepth %d time %d nodes %llu tbhits %d score mate %d pv ",gamestate.i_depth,
                s->maxply,elapsed * 10,s->nodes,EGTBHits, sign * (int)((MATE - score) / 2));
    }

    comp_to_coord(s, move, output);

    strcat(multipv_strings[mc], output);
    strcat(multipv_strings[mc], " ");

    memset(hashpv, 0, sizeof(hashpv));

    hash_extract_pv(s, 60, hashpv);

    strcat(multipv_strings[mc], hashpv);
    strcat(multipv_strings[mc], "\n");

    if ((multipv_scores[mc] >= multipv_scores[uci_multipv]) 
         || (mc <= uci_multipv)) {
        resort_multipv();
        for (i = 1; i <= uci_multipv; i++) {
            sprintf(hashpv, "info multipv %d ",i);
            strcat(hashpv, multipv_strings[i]);
            myprintf(hashpv);
        }
    }

}

int rdifftime(int end, int start) {    
    return 0;
}

void reset_piece_square(state_t *s) {    
    int i;

    s->Material = 0;    
    memset(s->npieces, 0, sizeof(s->npieces));

    for (i = 0; i < 64; i++) {
        if (s->sboard[i] && (s->sboard[i] < npiece)) {
            if (s->sboard[i] == wking) {
                s->wking_loc = i;
            } else if (s->sboard[i] == bking) {
                s->bking_loc = i;
            }

            ADDMATERIAL(s, s->sboard[i]);            
            s->npieces[s->sboard[i]]++;        
        }
    }

    /* bitboard stuff */
    s->WhitePieces = s->BlackPieces = 0;
    s->WhitePawns = s->BlackPawns = 0;
    s->WhiteRooks = s->BlackRooks = 0;
    s->WhiteKnights = s->BlackKnights = 0;
    s->WhiteBishops = s->BlackBishops = 0;
    s->WhiteQueens = s->BlackQueens = 0;
    s->WhiteKing = s->BlackKing = 0;

    for (i = 0; i < 64; i++) {
        assert(s->sboard[i] != invalid);
        if (s->sboard[i] != npiece) {
            s->BitBoard[s->sboard[i]] |= Mask[i];

            if (PIECESIDE(s, i) == WHITE) {
                s->WhitePieces |= Mask[i];
            } else {
                s->BlackPieces |= Mask[i];
            }
        }
    }        

    s->All = s->WhitePieces | s->BlackPieces;
    
    init_psq_score(s);
}

void myprintf(const char *fmt, ...) {
    va_list ap;
    FILE *log;        
    
    va_start(ap,fmt);
    
    vprintf(fmt,ap);

    if (cfg_logging) {
        log = fopen(cfg_logfile,"a");

        if (log) {
            vfprintf(log,fmt,ap);
            fclose(log);
        }
    }           

    va_end(ap);
}

void rinput(char str[], int n, FILE *stream) {
    FILE *log;

    /* 
        My input function - reads in up to n-1 characters from stream, or
        until we encounter a \n or an EOF.  Appends a null character at the end of        
        the string, and stores the string in str[] 
    */
    int ch, i = 0, j;

    if (buffered_count && !is_pondering) {
        strncpy(str, buffered_command[0], n);

        j = buffered_count - 1;

        while (j) {
            strncpy(buffered_command[i], buffered_command[i + 1], BIG_BUFF);
            j--;
            i++;
        }

        buffered_count--;
        memset(buffered_command[buffered_count], 0, sizeof(buffered_command[buffered_count]));

        return;
    }

    i = 0;

    while ((ch = getc(stream)) != (int)'\n' && ch != EOF) {
        if (i < n - 1) {
            str[i++] = ch;
        }
    }

    str[i] = '\0';

    if (cfg_logging) {
        log = fopen(cfg_logfile,"a");

        if (log) {
            fprintf(log,"< %s\n",str);
            fclose(log);
        }
    }
    
    if (ch == EOF) {
        myprintf("Nothing at other end - exiting\n");
        free_hash();
        exit(EXIT_FAILURE);
    }
}

int rtime( void )
{
    return 0;
}

/* 
    things to do on start up of the program 
*/
void start_up(void) {    
    myprintf(PACKAGE " version " VERSION ", Copyright (C) 2000-2009 Gian-Carlo Pascutto\n");
}

int verify_coord(state_t *s, char *input, move_s *move) {
    /* checks to see if the move the user entered was legal or not, returns
     true if the move was legal, and stores the legal move inside move */
    move_s tmoves[MOVE_BUFF];
    int num_moves, xi;
    char comp_move[6];
    int legal = FALSE;

    num_moves = gen(s, tmoves);

    /* compare user input to the generated moves: */
    for (xi = 0; (xi < num_moves) && !legal; xi++) {
        comp_to_coord(s, tmoves[xi], comp_move);

        if (!strcmp(input, comp_move)) {
            make(s, tmoves[xi]);

            if (check_legal(s, tmoves[xi])) {
                legal = TRUE;
                *move = tmoves[xi];
            }
            unmake(s, tmoves[xi]);
        }
    }

    return (legal);
}

int input_causes_stop(void) {
    int c;
    int elapsed;
    int newtime;
    
    c = getc(stdin);

    if (c == 'p') {
        /* ponderhit */
        elapsed = rdifftime(rtime(), gamestate.start_time);
        gamestate.time_for_move = allocate_time(&gamestate, FALSE);
        ungetc(c,stdin);

        /* we need this out of our buffer ! */
        rinput(&buffered_command[buffered_count][0], BIG_BUFF, stdin);
        buffered_count++;

#if !defined COPYPROTECTION
        if ((int)(((gamestate.time_for_move * 1.0 / 2.5) - elapsed) / 100) >= 0) {
            myprintf("info string Time for move: %ds, elapsed: %ds, left: %ds, early break: %ds\n",
                 gamestate.time_for_move / 100,elapsed / 100,
                 (gamestate.time_for_move - elapsed) / 100,
                 (int)(((gamestate.time_for_move * 1.0 / 2.5) - elapsed) / 100));
        } else {
            myprintf("info string Time for move: %ds, elapsed: %ds, left: %ds\n",
                 gamestate.time_for_move / 100,elapsed / 100,(gamestate.time_for_move - elapsed) / 100);

        }
#endif        
            
        return 0;
    } else {
        ungetc(c,stdin);

        if (uci_mode) {
            /* attempt to read out */
            rinput(&buffered_command[buffered_count][0],BIG_BUFF,stdin);

            if (buffered_command[buffered_count][0] == '\0') {
                return 0;
            }

            if (!strncmp(buffered_command[buffered_count],"time",4)) {
                sscanf(buffered_command[buffered_count] + 5,"%d",&newtime);
                gamestate.time_left = newtime;
                return 0;
            } else if (!strncmp(buffered_command[buffered_count],"otim",4)) {
                buffered_count++;
                return 0;
            } else if (!strncmp(buffered_command[buffered_count],
                       "setoption name MultiPV value", 22)) {
                sscanf(buffered_command[buffered_count] + 23, "value %d", &uci_multipv);
                return 0;
            }

            buffered_count++;
        }
    }       
    
    return 1;             
}

int interrupt(void) {
    return 0;
}

void reset_board(state_t *s) {
    // empty game        
    memset(s->sboard, npiece, sizeof(s->sboard));

    s->ep_square = 0;    
    s->Material = 0;    
    s->fifty = 0;
    s->psq_score = 0;
    s->castleflag = 0x1E;   // 11110
    s->threadid = 0;

    reset_piece_square(s);
}

void make_text_moves(gamestate_t *g, state_t *s, char *movebuff) {
    char movestr[STR_BUFF];
    int index = 0;
    move_s xmove;
    int movesmade = 0;    

    while (*(movebuff + index) != '\0' && (index < (int)strlen(movebuff))) {
        sscanf(movebuff + index,"%s",movestr);
        index += (int)(strlen(movestr) + 1);
        if (!verify_coord(s, movestr, &xmove)) {
            myprintf("WARNING: no move match: -%s-\n",movestr);            
            return;
        }

        s->hash_history[g->move_number] = s->hash;
        g->game_history[g->move_number] = xmove;        

        make(s, xmove);
        g->game_history_x[g->move_number++] = s->path_x[0];
        s->ply = 0;
        movesmade++;        
        g->root_to_move ^= 1;        
    }
}

unsigned int myrandom(void) {      
    const unsigned int mask = 0xffffffff;
    unsigned int b;
    b  = (((s1 << 13) & mask) ^ s1) >> 19;
    s1 = (((s1 & 0xFFFFFFFEU) << 12) & mask) ^ b;
    b  = (((s2 << 2) & mask) ^ s2) >> 25;
    s2 = (((s2 & 0xFFFFFFF8U) <<  4) & mask) ^ b;
    b  = (((s3 << 3) & mask) ^ s3) >> 11;
    s3 = (((s3 & 0xFFFFFFF0U) << 17) & mask) ^ b;
    return (s1 ^ s2 ^ s3);        
}

void mysrand(unsigned int s) {    
    if (s == 0) {
        s = 1;
    }        
    
    s1 = (741103597 * s);
    s2 = (741103597 * s1);
    s3 = (741103597 * s2);
    
    s1 |= 2;
    s2 |= 8;
    s3 |= 16;
} 

