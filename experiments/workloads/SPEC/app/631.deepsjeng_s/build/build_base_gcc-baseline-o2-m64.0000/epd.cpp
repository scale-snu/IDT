/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2005 Gian-Carlo Pascutto   

    File: epd.c                                             
    Purpose: handle and output EPD, run through EPD testsets

*/
#include "sjeng.h"
#include "extvars.h"
#include "bitboard.h"
#include "board.h"
#include "state.h"
#include "utils.h"
#include "ttable.h"
#include "search.h"
#include "squares.h"

/*
    read an EPD string and set up the board position to
    correspond to it
*/
void setup_epd_line(gamestate_t *g, state_t *s, const char* inbuff) {
    static const int rankoffsets[] = { a8, a7, a6, a5, a4, a3, a2, a1 };

    /*
        State of parsing the EPD string
           
        0 : FEN data 
        1 : Active color 
        2 : Castling status
        3 : EP info 
        4 : 50 move 
        5 : movenumber
        6 : EPD data 
    */
    int stage = 0;

    int i = 0;
    int j;
    int curr_rank = 0;
    int fileoffset = 0;
    int rankoffset;
    int castlefile;
    int castlesq;

    int ep_file, ep_rank, norm_file, norm_rank;
    int foundflags = FALSE;

    /* reset to normal game start */
    init_game(g, s);

    /* but clear board */
    memset(s->sboard, invalid, sizeof(s->sboard));

    /* and assume no castling possible at all */
    s->castleflag = 0;

    /* assume normal castling setup */
    s->wlrook_start = a1;
    s->wrrook_start = h1;
    s->blrook_start = a8;
    s->brrook_start = h8;

    rankoffset = rankoffsets[curr_rank];

    /* skip possible starting whitespaces */
    while (inbuff[i] == ' ') {
        i++;

    }

    /* while not end of string or end of line */
    while ((inbuff[i] != '\n') && (inbuff[i] != '\0')) {
        /* 
            input parts are seperated by spaces 
            if we detect one, move to next stage,
            and possibly eat any extra spaces after it
        */
        if (inbuff[i] == ' ') {
            stage++;

            do {
                i++;
            } while (inbuff[i] == ' ');
        }

        /*
            process text depending on parsing stage
        */
        if (stage == 0) {
            /* FEN board part */
            /* digit, make that number of empty squares */
            if (isdigit(inbuff[i])) {
                for (j = 0; j < atoi(&inbuff[i]); j++) {
                    s->sboard[rankoffset + fileoffset] = npiece;
                    fileoffset++;
                }
            } else if (inbuff[i] == '/') {
                /* next rank */
                curr_rank++;
                rankoffset = rankoffsets[curr_rank];
                fileoffset = 0;
            } else if (isalpha(inbuff[i])) {
                switch (inbuff[i]) {
                    case 'p':
                        s->sboard[rankoffset + fileoffset] = bpawn;
                        break;
                    case 'P':
                        s->sboard[rankoffset + fileoffset] = wpawn;
                        break;
                    case 'n':
                        s->sboard[rankoffset + fileoffset] = bknight;
                        break;
                    case 'N':
                        s->sboard[rankoffset + fileoffset] = wknight;
                        break;
                    case 'b':
                        s->sboard[rankoffset + fileoffset] = bbishop;
                        break;
                    case 'B':
                        s->sboard[rankoffset + fileoffset] = wbishop;
                        break;
                    case 'r':
                        s->sboard[rankoffset + fileoffset] = brook;
                        break;
                    case 'R':
                        s->sboard[rankoffset + fileoffset] = wrook;
                        break;
                    case 'q':
                        s->sboard[rankoffset + fileoffset] = bqueen;
                        break;
                    case 'Q':
                        s->sboard[rankoffset + fileoffset] = wqueen;
                        break;
                    case 'k':
                        s->bking_loc = rankoffset + fileoffset;
                        s->sboard[s->bking_loc] = bking;
                        s->bking_start = s->bking_loc;
                        break;
                    case 'K':
                        s->wking_loc = rankoffset + fileoffset;
                        s->sboard[s->wking_loc] = wking;
                        s->wking_start = s->wking_loc;
                        break;
                }

                fileoffset++;
            }
        } else if (stage == 1) {
            /* who-to-move part */
            if (inbuff[i] == 'w') {
                s->white_to_move = 1;
            } else {
                s->white_to_move = 0;
            }
        } else if (stage == 2) {
            /* castling status */
            switch (inbuff[i]) {
                case '-':
                    break;
                case 'K':
                    s->castleflag |= (1 << wck);
                    foundflags = TRUE;
                    break;
                case 'Q':
                    s->castleflag |= (1 << wcq);
                    foundflags = TRUE;
                    break;
                case 'k':
                    s->castleflag |= (1 << bck);
                    foundflags = TRUE;
                    break;
                case 'q':
                    s->castleflag |= (1 << bcq);
                    foundflags = TRUE;
                    break;
                default:    /* something else - chess960 */
                    if (uci_chess960_mode) {
                        if (inbuff[i] >= 'A' && inbuff[i] <= 'H') {
                            /* white castling possibility */
                            foundflags = TRUE;
                            castlefile = (inbuff[i] - 'A') + 1;
                            castlesq = a1 + castlefile - 1;

                            if (castlefile < file(s->wking_loc)) {
                                s->wlrook_start = castlesq;
                                s->castleflag |= 1 << wcq;
                            } else {
                                s->wrrook_start = castlesq;
                                s->castleflag |= 1 << wck;
                            }                            
                        } else if (inbuff[i] >= 'a' && inbuff[i] <= 'h') {
                            /* black castling possibility */
                            foundflags = TRUE;
                            castlefile = (inbuff[i] - 'a') + 1;
                            castlesq = a8 + castlefile - 1;

                            if (castlefile < file(s->bking_loc)) {
                                s->blrook_start = castlesq;                                
                                s->castleflag |= 1 << bcq;
                            } else {
                                s->brrook_start = castlesq;                                
                                s->castleflag |= 1 << bck;
                            }                            
                        }
                    }
                    break;
            }
        } else if (stage == 3) {
            /* ep square */
            if (inbuff[i] == '-') {
                s->ep_square = 0;
            } else {
                ep_file = inbuff[i++];
                ep_rank = inbuff[i];

                norm_file = ep_file - (int)'a';
                norm_rank = ep_rank - (int)'1';

                s->ep_square = ((norm_rank * (a2 - a1)) + a1) + norm_file;
            }
        } else if (stage == 4) {

            /* fifty move info */
            /* looks like a number? */
            if (isdigit(inbuff[i])) {
                s->fifty = atoi(&inbuff[i]);
            }
        } else if (stage == 5) {

            /* move number */
            /* 
               ignore this for now - only of use in GUI really,
               since we don't have previous moves to fill our
               datastructures we shouldn't try to mess with
               move_number or similar
            */
        } else if (stage == 6) {

            /* EPD data */
            /* ignore this for now */
        }

        /* 
            increment if we aren't already at the end 
        */
        if ((inbuff[i] != '\n') && (inbuff[i] != '\0')) {
            i++;
        }
    }

    /*
        - if we got no info on castling data, try to be smart about it
        - should not try guessing when we're being fed from a decent GUI
    */
    if (!foundflags && !uci_mode) {
        if (s->sboard[a8] == brook && s->sboard[e8] == bking) {
            s->castleflag |= 1 << bcq;
        }

        if (s->sboard[h8] == brook && s->sboard[e8] == bking) {
            s->castleflag |= 1 << bck;
        }

        if (s->sboard[a1] == wrook && s->sboard[e1] == wking) {
            s->castleflag |= 1 << wcq;
        }

        if (s->sboard[h1] == wrook && s->sboard[e1] == wking) {
            s->castleflag |= 1 << wck;
        }
    }

    /* recalculate castling masks (chess960) */
    SetupCastleMasks(s);

    reset_piece_square(s);
    initialize_hash(s);

    s->hash_history[g->move_number] = s->hash;
    g->root_to_move = s->white_to_move;
}

int check_solution(state_t *s, char *inbuff, move_s cmove) {
    char san[SAN_MAX];
    char *mvstart;
    int len;

    comp_to_san(s, cmove, san);
    len = (int)strlen(san);

    mvstart = strstr(inbuff,"bm ");

    if (mvstart != NULL) {

        /* skip "bm " */
        mvstart += 3;

        /* skip extra spaces */
        while (*mvstart == ' ') {
            mvstart++;
        }

        if (strncmp(mvstart, san, len) == 0) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    mvstart = strstr(inbuff,"am ");

    if (mvstart != NULL) {

        /* skip "am " */
        mvstart += 3;

        /* skip extra spaces */
        while (*mvstart == ' ') {
            mvstart++;
        }

        if (strncmp(mvstart, san, len) == 0) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    myprintf("No best-move or avoid-move found!");

    return FALSE;
}

void run_epd_testsuite(gamestate_t *g, state_t *s, const char *testname) {
    FILE *testsuite;
    char readbuff[STR_BUFF];    
    int thinkdepth;
    move_s comp_move;
    int tested = 0;

    testsuite = fopen(testname, "r");

    if (testsuite == NULL) {
        myprintf("Workload not found\n");
        return;
    }

    while (fgets(readbuff, STR_BUFF, testsuite) != NULL) {
        tested++;

        clear_tt();
        setup_epd_line(g, s, readbuff);

        fgets(readbuff, STR_BUFF, testsuite);
        sscanf(readbuff, "%d", &thinkdepth);

        myprintf("Analyzing %d plies...\n", thinkdepth);
        display_board(s, 1);

        g->fixed_time = 99999999;
        g->maxdepth = thinkdepth;

        comp_move = think(g, s);

        myprintf("\nNodes: %llu (%0.2f%% qnodes)\n",
                 s->nodes,
                 (float)((float)s->qnodes / (float)s->nodes * 100.0));

        dumpsearchstat(s);

        myprintf("\n");        
    };

    fclose(testsuite);    
}

/*
    translates current position to fen
    buffer pointed to by fen should be large enough
*/
void position_to_fen(state_t *s, char *fen) {
    static const char xlate[] = { 'F', 'P', 'p', 'N', 'n', 'K', 'k', 'R', 'r', 'Q', 'q', 'B', 'b', 'E' };
    static const char str_empty[9] = { ' ', '1', '2', '3', '4', '5', '6', '7', '8' };
    static const int rankoffsets[] = { a1, a2, a3, a4, a5, a6, a7, a8 };
    int xrank, xfile, nempty, thissq;
    int castflag;
    char *c;

    int *sboard = s->sboard;

    /* pointer to current position in fen */
    c = fen;

    for (xrank = 8; xrank >= 1; xrank--) {

        /* number of empty squares in this rank */
        nempty = 0;

        for (xfile = FileA; xfile <= FileH; xfile++) {
            thissq = sboard[(rankoffsets[xrank - 1]) + xfile];

            /* if the square is not empty */
            if (thissq != npiece) {

                /* dump out possible empty squares before */
                if (nempty) {
                    sprintf(c,"%c",str_empty[nempty]);
                    c++;
                    nempty = 0;
                }

                /* print whatever is on here */
                sprintf(c,"%c",xlate[thissq]);
                c++;
            } else {

                /* count more empty squares */
                nempty++;
            }
        }

        /* leftovers at end of rank */
        if (nempty) {
            sprintf(c,"%c",str_empty[nempty]);
            c++;
        }

        if (xrank != 1) {
            sprintf(c,"/");
            c++;
        }
    }

    sprintf(c," %c ",(s->white_to_move) ? 'w' : 'b');
    c += 3;

    castflag = FALSE;

    if (s->castleflag & (1 << wck)) {
        sprintf(c,"K");
        c++;
        castflag = TRUE;
    }

    if (s->castleflag & (1 << wcq)) {
        sprintf(c,"Q");
        c++;
        castflag = TRUE;
    }

    if (s->castleflag & (1 << bck)) {
        sprintf(c,"k");
        c++;
        castflag = TRUE;
    }

    if (s->castleflag & (1 << bcq)) {
        sprintf(c,"q");
        c++;
        castflag = TRUE;
    }

    if (!castflag) {
        sprintf(c,"-");
        c++;
    }

    if (s->ep_square) {
        sprintf(c," %c%c",'a' + (file(s->ep_square) - 1),'1' + (rank(s->ep_square) - 1));
        c += 3;
    } else {
        sprintf(c," -");
        c += 2;
    }
}
