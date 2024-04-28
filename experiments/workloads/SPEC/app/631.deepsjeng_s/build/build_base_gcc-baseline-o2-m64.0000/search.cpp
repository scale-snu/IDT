/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2009 Gian-Carlo Pascutto

    File: search.c
    Purpose: contains functions related to the recursive search

*/

#include "config.h"
#include "sjeng.h"

#include <limits.h>
#include <math.h>
#include <algorithm>

#include "extvars.h"
#include "evpar.h"
#include "utils.h"
#include "bitboard.h"
#include "moves.h"
#include "make.h"
#include "see.h"
#include "ttable.h"
#include "search.h"
#include "board.h"
#include "generate.h"
#include "neval.h"
#include "draw.h"
#include "attacks.h"
#include "ttable.h"
#include "bits.h"
#include "squares.h"
#include "search.h"

static const int rc_index[14] = {
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 2, 2, 0
};

int history_h[MAX_CPU][12][64];
int history_hit[MAX_CPU][12][64];
int history_tot[MAX_CPU][12][64];

static void history_good(state_t *s, const move_s move, const int depth) {
    int i, j;
    int piece, bto;
    
    if (CAPTURED(move) == npiece && !PROMOTED(move)) {
        piece = s->sboard[FROM(move)] - 1;
        bto = TARGET(move);

        history_hit[s->threadid][piece][bto] += (depth + 3) / PLY;
        history_tot[s->threadid][piece][bto] += (depth + 3) / PLY;

        if (history_tot[s->threadid][piece][bto] > 16384) {
            for (i = 0; i < 12; i++) {
                for (j = 0; j < 64; j++) {
                    history_hit[s->threadid][i][j] = (history_hit[s->threadid][i][j] + 1) >> 1;
                    history_tot[s->threadid][i][j] = (history_tot[s->threadid][i][j] + 1) >> 1;
                }
            }
        }
 
        history_h[s->threadid][piece][bto] += (depth + 3) / PLY;

        if (history_h[s->threadid][piece][bto] > INF/2) {
            for (i = 0; i < 12; i++) {
                for (j = 0; j < 64; j++) {
                    history_h[s->threadid][i][j] = (history_h[s->threadid][i][j] + 1) >> 1;                    
                }
            }
        }

        move_s o1 = s->killerstack[s->ply].killer1;
        if (o1 != move) {
            move_s o2 = s->killerstack[s->ply].killer2;
            if (o2 != move) {
                move_s o3 = s->killerstack[s->ply].killer3;
                if (o3 != move) {
                    s->killerstack[s->ply].killer4 = o3;
                }
                s->killerstack[s->ply].killer3 = o2;
            }
            s->killerstack[s->ply].killer2 = o1;
            s->killerstack[s->ply].killer1 = move;
        }        
    }
}

static void history_bad(state_t *s, const move_s move, const int depth) {
    int i, j;
    int piece, bto;

    if (CAPTURED(move) == npiece && !PROMOTED(move)) {
        piece = s->sboard[FROM(move)] - 1;
        bto = TARGET(move);

        history_tot[s->threadid][piece][bto] += (depth + 3) / PLY;

        if (history_tot[s->threadid][piece][bto] > 16384) {
            for (i = 0; i < 12; i++) {
                for (j = 0; j < 64; j++) {
                    history_hit[s->threadid][i][j] = (history_hit[s->threadid][i][j] + 1) >> 1;
                    history_tot[s->threadid][i][j] = (history_tot[s->threadid][i][j] + 1) >> 1;
                }
            }
        }
    }
}

static int history_score(state_t *s, const move_s move) {
    int piece, bto;
    int score;

    piece = s->sboard[TARGET(move)] - 1;
    bto = TARGET(move);

    score = ((history_hit[s->threadid][piece][bto] + 1) * 128)
             / (history_tot[s->threadid][piece][bto] + 1);

    return score;
}

static int history_pre_cut(state_t *s, const move_s move, const int depth) {
    int piece, bto;
    int failure, success;

    piece = s->sboard[FROM(move)] - 1;
    bto = TARGET(move);

    success = history_hit[s->threadid][piece][bto];
    failure = history_tot[s->threadid][piece][bto] - success;

    return (depth * success) < failure;
}

static int make_blunder(state_t *s, move_s themove, int *order, int num_moves) {
    int maxprob, prob;

    if (s->checks[s->ply]) {
        return 0;
    }

    if (elo_to_blunder(uci_elo) >= 100) {
        return 0;
    }

    prob = 100 - elo_to_blunder(uci_elo);

    maxprob = myrandom() % 200;

    if (num_moves < 6) {
        prob -= 80;
    } else if (num_moves < 9) {
        prob -= 60;
    }

    if (gamestate.i_depth < 5) {
        prob += (5 - gamestate.i_depth);
    }

    prob += s->ply * 2;

    prob += s->npieces[wqueen] +
            s->npieces[wrook] +
            s->npieces[wbishop] +
            s->npieces[wknight] +
            s->npieces[bqueen] +
            s->npieces[bbishop] +
            s->npieces[bknight] +
            s->npieces[brook] +
            s->npieces[wpawn] +
            s->npieces[bpawn];

    if (CAPTURED(themove) == s->sboard[FROM(themove)]) {
        prob -= 30;
    }

    if (s->sboard[FROM(themove)] == wpawn || s->sboard[FROM(themove)] == bpawn) {
        prob -= 30;
    }

    if (CAPTURED(themove) != npiece) {
        prob -= 10;
    }

    if (CAPTURED(themove) == wqueen || CAPTURED(themove) == bqueen) {
        prob -= 30;
    }

    if (PROMOTED(themove) != 0
        && PROMOTED(themove) != npiece
        && PROMOTED(themove) != wqueen
        && PROMOTED(themove) != bqueen) {
        prob += 40;
    }

    prob += taxicab_dist(FROM(themove), TARGET(themove)) * 2;

    if (s->sboard[FROM(themove)] == wknight || s->sboard[FROM(themove)] == bknight) {
        prob += 20;
    }

    if (s->white_to_move) {
        if (rank(FROM(themove)) > rank(TARGET(themove))) {
            prob += 4 * (rank(FROM(themove)) - rank(TARGET(themove)));
        }
    } else {
        if (rank(FROM(themove)) < rank(TARGET(themove))) {
            prob += 4 * (rank(FROM(themove)) - rank(TARGET(themove)));
        }
    }

    if (prob >= maxprob) {
        *order = -INF;
        return 1;
    }

    return 0;
}

static void order_moves(state_t *s,
                        move_s *moves, int *move_ordering, int num_moves,
                        unsigned int best) {
    int promoted, captured;
    int i, from, target, seev;

    for (i = 0; i < num_moves; i++) {
        from = FROM(moves[i]);
        target = TARGET(moves[i]);
        promoted = PROMOTED(moves[i]);
        captured = CAPTURED(moves[i]);

        if (uci_multipv > 1 && s->ply == 1 && gamestate.i_depth > 2) {
            move_ordering[i] = 100000 + root_scores[i];
            continue;
        }

        if (uci_limitstrength && s->ply > 1) {
            if (make_blunder(s, moves[i], &move_ordering[i], num_moves)) {
                move_ordering[i] = -INF;
                continue;
            }
        }

        if (compact_move(moves[i]) == best) {
            move_ordering[i] = INF * 128;
        } else if (captured != npiece || promoted) {
            if (abs(material[captured]) - abs(material[s->sboard[from]]) > 0) {
                seev = abs(material[captured]) - abs(material[s->sboard[from]]);
            } else {
                seev = see(s, TOMOVE(s), from, target, promoted);
            }

            if (seev >= 0) {
                move_ordering[i] = 10000000 + seev;
            } else {
                move_ordering[i] = seev;
            }
        } else {
            if (moves[i] == s->killerstack[s->ply].killer1) {
                move_ordering[i] = 8000000;
            } else if (moves[i] == s->killerstack[s->ply].killer2) {
                move_ordering[i] = 7000000;
            } else if (moves[i] == s->killerstack[s->ply].killer3) {
                move_ordering[i] = 6000000;
            } else if (moves[i] == s->killerstack[s->ply].killer4) {
                move_ordering[i] = 5000000;
            } else {
                int piece = s->sboard[from] - 1;
                move_ordering[i] = history_h[s->threadid][piece][target]
                                 - history_h[s->threadid][piece][from];
            }
        }
    }
}

static void fast_order_moves(state_t *s,
                             move_s *moves, int *move_ordering, int num_moves,
                             unsigned int best) {
    int captured, promoted;
    int i, from, target;

    for (i = 0; i < num_moves; i++) {
        from = FROM(moves[i]);
        target = TARGET(moves[i]);
        captured = CAPTURED(moves[i]);
        promoted = PROMOTED(moves[i]);

        if (compact_move(moves[i]) == best) {
            move_ordering[i] = INF * 128;
        } else if (captured != npiece) {
            move_ordering[i] = 10000000 + (abs(material[captured]) * 4096)
                                        - abs(material[s->sboard[from]]);
        } else if (promoted) {
            move_ordering[i] =  9000000 + (abs(material[promoted]));
        } else {
            /* not a capture, not a hash move */
            if (moves[i] == s->killerstack[s->ply].killer1) {
                move_ordering[i] = 8000000;
            } else if (moves[i] == s->killerstack[s->ply].killer2) {
                move_ordering[i] = 7000000;
            } else if (moves[i] == s->killerstack[s->ply].killer3) {
                move_ordering[i] = 6000000;
            } else if (moves[i] == s->killerstack[s->ply].killer4) {
                move_ordering[i] = 5000000;
            } else {
                int piece = s->sboard[from] - 1;
                move_ordering[i] = history_h[s->threadid][piece][target]
                                   - history_h[s->threadid][piece][from];
            }
        }
    }
}


static int search_time_check(state_t *s) {
    int oldtime;
    gamestate_t *g = &gamestate;

    
    if (!(s->nodes & ((1 << time_check_log) - 1))) {
        if (interrupt()) {
            g->time_exit = TRUE;
            return 1;
        } else if (((rdifftime(rtime(), g->start_time) >= g->time_for_move))
                    && (g->i_depth > MINDEPTH)) {
            if (g->failed == 1
                && (g->extendedtime != 2)
                && !g->fixed_time
                && (g->time_left > std::max(g->time_for_move * 7, 1000))) {
                oldtime = g->time_for_move;
                if (!g->extendedtime) {
                    g->time_for_move += allocate_time(g, TRUE);
                    g->time_for_move += allocate_time(g, TRUE);
                }

                g->time_for_move += allocate_time(g, TRUE);
                g->time_for_move += allocate_time(g, TRUE);
                g->time_for_move += allocate_time(g, TRUE);
                g->time_for_move = std::min(g->time_for_move, g->time_left - 50);

                g->extendedtime = 2;
            } else if (g->failedhigh == 1
                       && !g->extendedtime
                       && !g->fixed_time
                       && (g->time_left > std::max(g->time_for_move * 6, 1000))) {
                g->extendedtime = 1;
                oldtime = g->time_for_move;
                g->time_for_move += allocate_time(g, TRUE);
                g->time_for_move += allocate_time(g, TRUE);
                g->time_for_move = std::min(g->time_for_move, g->time_left - 50);
            } else {
                g->time_exit = TRUE;
                return 1;
            }
        }
    }

    return 0;
}

/*
*  Function to give pick the top move order, one at a time on each call.
*  Will return TRUE while there are still moves left, FALSE after all moves
*  have been used
*/
static int remove_one_fast(int * marker, int * move_ordering,
                           move_s *moves, const int num_moves) {
    move_s tmpmv;
    int i, best = -INF;
    int tmp, mark;

    (*marker)++;
    mark = *marker;

    if (mark < 10) {
        for (i = mark; i < num_moves; i++) {
            if (move_ordering[i] > best) {
                best = move_ordering[i];
                tmp = i;
            }
        }
    } else if (mark < num_moves) {
        best = move_ordering[mark];
        tmp = mark;

        return TRUE;
    }

    if (best > -INF) {
        /*
            swap best move to marker pos
        */
        move_ordering[tmp] = move_ordering[mark];
        move_ordering[mark] = best;

        tmpmv = moves[mark];
        moves[mark] = moves[tmp];
        moves[tmp] = tmpmv;

        return TRUE;
    } else {
        return FALSE;
    }
}

/*
* perform a quiescence search on the current node
* using alpha-beta with negamax search
*/
int qsearch(state_t *s, int alpha, int beta, int depth, int qply) {
    int num_moves, i;
    int score, standpat;
    int legal_move, no_moves;
    int delta;
    unsigned int best, sbest;
    int originalalpha;
    int bound, xdummy;
    int incheck, afterincheck;
    int pass, multipass;
    int standpatmargin;
    move_s moves[MOVE_BUFF];
    int move_ordering[MOVE_BUFF];

    s->nodes++;
    s->qnodes++;

    if (s->ply > s->maxply) {
        s->maxply = s->ply;
    }

    if (search_time_check(s)) {
        return 0;
    }

    if (is_draw(&gamestate, s) || s->fifty > 99) {
        return (gamestate.comp_color == s->white_to_move ? contempt : -contempt);
    }

    switch (ProbeTT(s, &bound, alpha, beta, &best, &xdummy, &xdummy, &xdummy, &xdummy, 0)) {
        case EXACT:
            return bound;
            break;
        case UPPER:
            if (bound <= alpha) {
                return bound;
            }
            break;
        case LOWER:
            if (bound >= beta) {
                return bound;
            }
            break;
        case DUMMY:
            break;
        case HMISS:
            best = 0xFFFF;
            break;
    }   

    if ((qply > 2*gamestate.i_depth) || s->ply > MAXDEPTH) {
        return eval(s, alpha, beta, FALSE);
    }

    incheck = s->checks[s->ply];

    originalalpha = alpha;

    standpat = retrieve_eval(s);
    standpatmargin = standpat + 50;

    if (!incheck) {
        if (standpat >= beta) {
            StoreTT(s, standpat, originalalpha, beta, best, 0, 0, 0, 0);
            return standpat;
        } else if (standpat > alpha) {
            alpha = standpat;
        } else if (standpatmargin + MAT_QUEEN <= alpha) {
            StoreTT(s, standpatmargin + MAT_QUEEN, originalalpha, beta, best, 0, 0, 0, 0);
            return standpatmargin + MAT_QUEEN;
        } else {
            int *npieces = s->npieces;
            if (s->white_to_move) {
                if (!npieces[bqueen]) {
                    if (!npieces[brook]) {
                        if (!npieces[bbishop] && !npieces[bknight]) {
                            if (standpatmargin + MAT_PAWN <= alpha) {
                                return standpatmargin + MAT_PAWN;
                            }
                        } else {
                            if (standpatmargin + MAT_BISHOP <= alpha) {
                                return standpatmargin + MAT_BISHOP;
                            }
                        }
                    } else {
                        if (standpatmargin + MAT_ROOK <= alpha) {
                            StoreTT(s, standpatmargin + MAT_ROOK, originalalpha, beta, best, 0, 0, 0, 0);
                            return standpatmargin + MAT_ROOK;
                        }
                    }
                }
            } else {
                if (!npieces[wqueen]) {
                    if (!npieces[wrook]) {
                        if (!npieces[wbishop] && !npieces[wknight]) {
                            if (standpatmargin + MAT_PAWN <= alpha) {
                                return standpatmargin + MAT_PAWN;
                            }
                        } else {
                            if (standpatmargin + MAT_BISHOP <= alpha) {
                                return standpatmargin + MAT_BISHOP;
                            }
                        }
                    } else {
                        if (standpatmargin + MAT_ROOK <= alpha) {
                            StoreTT(s, standpatmargin + MAT_ROOK, originalalpha, beta, best, 0, 0, 0, 0);
                            return standpatmargin + MAT_ROOK;
                        }
                    }
                }
            }
        }
    }

    num_moves = 0;    

    if (!incheck) {
        delta = alpha - standpatmargin;
    } else {
        delta = 0;
    }
    
    multipass = FALSE;
    pass = 1;
    score = -MATE;
    no_moves = TRUE;
    sbest = best;

    if (depth > -6) {
        if (incheck) {
            num_moves = gen_evasions(s, moves, incheck);
        } else {
            num_moves = gen_captures(s, moves);
            multipass = TRUE;
        }
    } else {
        if (!incheck) {
            num_moves = gen_captures(s, moves);
        } else {
            num_moves = gen_evasions(s, moves, incheck);
        }
    }

mpass:
    if (pass == 2) {
        num_moves = gen_good_checks(s, moves);
    } else if (pass == 3) {
        num_moves = gen_quiet(s, moves);
    }

    fast_order_moves(s, moves, move_ordering, num_moves, best);

    i = -1;

    /* loop through the moves at the current node: */
    while (remove_one_fast(&i, move_ordering, moves, num_moves)) {
        if (!incheck) {
            if (pass == 1) {
                if (abs(material[CAPTURED(moves[i])]) <= delta && !PROMOTED(moves[i])) {
                    goto endpass;
                }
            }
            if ((pass == 2 || pass == 3) && CAPTURED(moves[i]) != npiece && abs(material[CAPTURED(moves[i])]) > delta) {
                continue;
            }
            if (pass == 3) {
                if (history_pre_cut(s, moves[i], 1)) {
                    continue;
                }
            }
            if (pass == 2 || pass == 3 || abs(material[CAPTURED(moves[i])]) < abs(material[s->sboard[FROM(moves[i])]]) ) {
                if (see(s, TOMOVE(s), FROM(moves[i]), TARGET(moves[i]), FALSE) < -50) {
                    continue;
                }
            }
        }

        legal_move = FALSE;

        make(s, moves[i]);

        if (check_legal(s, moves[i])) {
            s->hash_history[gamestate.move_number + s->ply - 1] = s->hash;
            s->path[s->ply - 1] = moves[i];

            afterincheck = in_check(s);
            s->checks[s->ply] = afterincheck;
            int posteval = -eval(s, -beta, -alpha+60, !!afterincheck);

            if (pass != 3 || posteval > alpha) {
                int newdepth;

                if (pass == 3) {                    
                    newdepth = depth;
                } else if (afterincheck || incheck) {
                    newdepth = depth - 1;
                } else {
                    newdepth = depth - 8;
                }
                score = -qsearch(s, -beta, -alpha, newdepth, qply + 1);
            }

            legal_move = TRUE;
            no_moves = FALSE;
        }

        unmake(s, moves[i]);

        if (gamestate.time_exit) {
            return 0;
        }

        if (score > alpha && legal_move) {
            sbest = compact_move(moves[i]);

            if (score >= beta) {
                StoreTT(s, score, originalalpha, beta, sbest, 0, 0, 0, 0);
                return score;
            }

            alpha = score;
        }        
    }

endpass:
    if (multipass && pass == 1) {
        pass++;
        goto mpass;
    } else if (multipass && pass == 2) {
        if (depth >= 0) {
            if (standpat + 50 > alpha) {
                pass++;
                goto mpass;
            }
        }
    }

    /*
        we don't check for mate / stalemate here, because without generating all
        of the moves leading up to it, we don't know if the position could have
        been avoided by one side or not
    */
    if (no_moves && incheck) {
        alpha = -MATE + s->ply;
    }
    
    StoreTT(s, alpha, originalalpha, beta, sbest, 0, 0, 0, 0);

    return alpha;
}

/*
*    search the current node using alpha-beta with negamax search
*/
int search(state_t *s, int alpha, int beta, int depth, int is_null, int cutnode) {
    move_s moves[MOVE_BUFF];
    int move_ordering[MOVE_BUFF];
    int num_moves, i, j;
    int score;
    int no_moves, legal_move;
    int bound, threat, donull, best_score, old_ep;
    unsigned int best;
    int incheck, first;
    int extend;
    int originalalpha;
    int mateprune;
    int afterincheck;
    int legalmoves;
    int reduc;
    int remoneflag;
    int mn;
    int singular;
    int huber;
    int nosingular;
    int wpcs, bpcs;
    int fullext;
    move_s searched_moves[MOVE_BUFF];

    if (depth <= 0 || s->ply >= MAXDEPTH) {
        return qsearch(s, alpha, beta, 0, 0);
    }

    s->nodes++;

    if (search_time_check(s)) {
        return 0;
    }

    if (is_draw(&gamestate, s) || s->fifty > 99) {
        return (gamestate.comp_color == s->white_to_move ? contempt : -contempt);
    }

    /*
        mate pruning
        if being mated here would be good enough...
    */
    mateprune = -MATE + s->ply;
    if (mateprune > alpha) {
        alpha = mateprune;
        if (mateprune >= beta) {
            return mateprune;
        }
    }

    /*
        mate pruning
        if mating the opponent isn't good enough..
    */
    mateprune = MATE - s->ply - 1;
    if (mateprune < beta) {
        beta = mateprune;
        if (mateprune <= alpha) {
            return mateprune;
        }
    }

    switch (ProbeTT(s,&bound,alpha,beta,&best,&threat,&donull,&singular,&nosingular,depth)) {
        case EXACT:            
            return bound;
            break;
        case UPPER:
            if (bound <= alpha) {                
                return bound;
            }
            cutnode = FALSE;
            break;
        case LOWER:
            if (bound >= beta) {                
                return bound;
            }
            cutnode = TRUE;
            break;
        case DUMMY:
            if (bound >= beta) {
                cutnode = TRUE;
            }
            break;
        case HMISS:
            best = 0xFFFF;
            threat = FALSE;
            singular = FALSE;
            nosingular = FALSE;
            break;        
    }
    
    originalalpha = alpha;
    incheck = s->checks[s->ply];
    int escore = retrieve_eval(s);

     if (!incheck && beta == alpha + 1) {
        if (depth <= PLY) {
            if (escore - 75 >= beta) {
                StoreTT(s, escore - 75,originalalpha,beta,best,threat,singular,nosingular,depth);
                return escore;
            }
            if (escore < beta) {
                return qsearch(s, alpha, beta, 0, 0);
            }
        } else if (depth <= 2*PLY) {
            if (escore - 125 >= beta) {
                StoreTT(s, escore - 125,originalalpha,beta,best,threat,singular,nosingular,depth);
                return escore;
            }   
        }
    }
    
    best_score = -MATE;
    score     = -MATE;

    wpcs = s->npieces[wqueen] + s->npieces[wrook] + s->npieces[wbishop] + s->npieces[wknight];
    bpcs = s->npieces[bqueen] + s->npieces[brook] + s->npieces[bbishop] + s->npieces[bknight];

    threat = 0;

    if ((is_null == NONE)
        && (s->white_to_move ? wpcs : bpcs)
        && !incheck && donull
        && beta == alpha + 1
        && (threat == FALSE)
        && depth > PLY) {

        /*
        * zugzwang detector
        */
        if (gamestate.phase == ENDGAME) {
            int newdepth = depth - 6*PLY;
            if (newdepth <= 0) {
                score = qsearch(s, beta - 1, beta, 0, 0);
            } else {
                score = search(s, beta - 1, beta, newdepth, SINGLE, cutnode);
            }
            if (gamestate.time_exit) {
                return 0;
            }
        }

        if (gamestate.phase != ENDGAME || score >= beta) {
            old_ep = s->ep_square;
            s->ep_square = 0;
            s->white_to_move ^= 1;
            s->ply++;
            s->fifty++;

            s->path[s->ply - 1] = 0;
            s->checks[s->ply] = 0;
            s->plyeval[s->ply] = s->plyeval[s->ply - 1];

            /*
                R = 3
            */
            reduc = 4 * PLY;
            int newdepth = depth - reduc;

            if (newdepth <= 0) {
                score = -qsearch(s, -beta, -beta + 1, 0, 0);
            } else {
                score = -search(s, -beta, -beta + 1, newdepth, SINGLE, !cutnode);
            }

            s->fifty--;
            s->ply--;
            s->white_to_move ^= 1;
            s->ep_square = old_ep;

            if (gamestate.time_exit) {
                return 0;
            }

            if (score >= beta) {
                StoreTT(s, score, originalalpha, beta, best, threat, FALSE, nosingular, depth);
                return score;
            } else if (score < -MATE + 600) {
                threat = TRUE;
            }
        }
    } else if (beta == alpha + 1 && depth <= 3*PLY
        && escore < beta - 300) {
        int rscore = qsearch(s, alpha, beta, 0, 0);
        if (gamestate.time_exit) {
            return 0;
        }
        // can be < beta, store result score
        if (rscore <= alpha) {
            StoreTT(s,alpha,originalalpha,beta,best,threat,singular,nosingular,depth);
            return alpha;
        }
    }

    extend = 0;

    num_moves = 0;
    no_moves = TRUE;
    legalmoves = 0;

    if (incheck) {
        num_moves = gen_evasions(s, moves, incheck);
    } else {
        num_moves = gen(s, moves);
    }

    if (incheck) {
        if (num_moves) {
            for (i = 0; (i < num_moves) && (legalmoves < 2); i++) {
                make(s, moves[i]);

                if (check_legal(s, moves[i])) {
                    legalmoves++;
                }

                unmake(s, moves[i]);
            }
        }
    } else {
        legalmoves = num_moves;
    }

    order_moves(s, moves, move_ordering, num_moves, best);

    if (depth >= 5 * PLY && (beta != alpha + 1) && (best == 0xFFFF)) {
        int goodmove = FALSE;

        for (i = 0; i < num_moves; i++) {
            if (CAPTURED(moves[i]) != npiece
                && (abs(CAPTURED(moves[i])) > abs(material[s->sboard[FROM(moves[i])]]))) {
                goodmove = TRUE;
            }
        }

        if (!goodmove) {
            int tmp;
            unsigned int mv;

            score = search(s, alpha, beta, depth >> 1, NONE, cutnode);

            if (ProbeTT(s, &tmp, 0, 0, &mv ,&tmp, &tmp, &tmp, &tmp, 0) != HMISS) {
                order_moves(s, moves, move_ordering, num_moves, mv);
            } else {
                order_moves(s, moves, move_ordering, num_moves, best);
            }
        }
    }

    score = -MATE;

    /*
    *   check for multi-cut
    */
    if (!incheck
        && !threat
        && depth >= 4 * PLY
        && legalmoves > 8
        /*&& cutnode*/
        && !s->checks[s->ply - 1]
        && (s->ply < 3 || (!s->checks[s->ply - 2]))
        && (s->ply < 4 || (!s->checks[s->ply - 3]))) {

        int m_s, m_c;

        m_s = 0;
        m_c = 0;
        i = -1;

        remoneflag = remove_one_fast(&i, move_ordering, moves, num_moves);

        while (remoneflag && m_s < 3) {
            m_s++;

            make(s, moves[i]);

            legal_move = FALSE;

            if (check_legal(s, moves[i])) {
                s->hash_history[gamestate.move_number + s->ply - 1] = s->hash;
                s->path[s->ply - 1] = moves[i];

                legal_move = TRUE;

                afterincheck = in_check(s);
                s->checks[s->ply] = afterincheck;

                int newdepth = depth - 4*PLY;

                eval(s, -beta, -alpha+50, newdepth > 0 || afterincheck);

                if (newdepth <= 0) {
                    score = -qsearch(s, -beta,-beta+1, 0, 0);
                } else {
                    score = -search(s, -beta,-beta + 1, newdepth, NONE, !cutnode);
                }
            }

            unmake(s, moves[i]);

            if (!gamestate.time_exit) {
                if (score >= beta && legal_move) {
                    m_c++;

                    if (m_c >= 2) {
                        StoreTT(s, beta, originalalpha, beta, best, threat, FALSE, nosingular, depth);
                        return beta;
                    }
                }
            } else {
                break;
            }

            remoneflag = remove_one_fast(&i,move_ordering,moves,num_moves);
        }
    }

    first = TRUE;

    int s_c;

    /*
    *   check for singularity
    */
    if (!singular
        && !nosingular
        && !threat
        && depth >= 5 * PLY
        && legalmoves > 1
        && (gamestate.phase != ENDGAME)) {

        int prescore = search(s, alpha, beta, depth - 6*PLY, NONE, cutnode);

        if (prescore > alpha) {
            i = -1;

            remoneflag = remove_one_fast(&i,move_ordering,moves,num_moves);

            s_c = 0;

            while (remoneflag && singular <= 1 && s_c < 3) {
                make(s, moves[i]);

                legal_move = FALSE;

                if (check_legal(s, moves[i])) {
                    s->hash_history[gamestate.move_number + s->ply - 1] = s->hash;
                    s->path[s->ply - 1] = moves[i];

                    s_c++;

                    legal_move = TRUE;

                    afterincheck = in_check(s);
                    s->checks[s->ply] = afterincheck;
                    int newdepth = depth - 4 * PLY;
                    eval(s, -beta, -alpha+50, newdepth > 0 || afterincheck);

                    if (first) {
                        score = -search(s, -(alpha + 1),-(alpha), newdepth, NONE, !cutnode);
                        if (score > alpha) {
                            singular = TRUE;
                        } else {
                            singular = FALSE;
                            s_c += 10;
                        }
                    } else {
                        score = -search(s, -(alpha - 49),-(alpha - 50), newdepth, NONE, FALSE);
                        if (score > alpha - 50) {
                            singular = FALSE;
                            s_c += 10;
                        }
                    }

                    first = FALSE;
                }

                unmake(s, moves[i]);
                remoneflag = remove_one_fast(&i,move_ordering,moves,num_moves);
            }
        }
    }

    fullext = (beta != alpha + 1) && !(s->ply > 2*gamestate.i_depth);

    
    first = TRUE;

    mn = 1;
    i = -1;

    remoneflag = remove_one_fast(&i, move_ordering, moves, num_moves);

    /* loop through the moves at the current node: */
    while (remoneflag) {

        extend = 0;

        if (s->ply < MAXDEPTH) {
            if (incheck && legalmoves == 1) {                    
                extend += PLY;                    
            }

            if (PIECET(s, FROM(moves[i])) == pawn
                && (Rank(TARGET(moves[i])) == Rank7
                    || Rank(TARGET(moves[i])) == Rank2
                    || PROMOTED(moves[i]))) {
                if (fullext) {
                    extend += PLY/2;
                } else {
                    extend += 0;
                }
            }               

            if (CAPTURED(moves[i]) != npiece
                && CAPTURED(s->path[s->ply - 1]) != npiece
                && rc_index[CAPTURED(moves[i])] == rc_index[CAPTURED(s->path[s->ply - 1])]
                && TARGET(moves[i]) == TARGET(s->path[s->ply - 1])) {
                int capsee = see(s, TOMOVE(s), FROM(moves[i]), TARGET(moves[i]), PROMOTED(moves[i]));
                if (capsee > 0) {
                    if (fullext) {
                        extend += 3;
                    } else {
                        extend += 1;
                    }
                }
            }

            if (singular == 1 && extend && first) {
                /* wasted work as it gets already extended */
                nosingular = TRUE;
            } else if (!extend && (singular == 1) && (first)) {
                nosingular = FALSE;
                if (fullext) {
                    extend += 3;
                } else {
                    extend += 1;
                }
            }

            if (extend > PLY) {
                extend = PLY;
            }

            /* capture into pawn endgame */
            if (CAPTURED(moves[i]) != npiece
                && CAPTURED(moves[i]) != wpawn
                && CAPTURED(moves[i]) != bpawn) {
                if ((wpcs + bpcs) == 1) {
                    extend += PLY;
                }
            }
        }

        if (CAPTURED(moves[i]) == npiece
            && (mn > 1 + (depth/PLY))
            && history_pre_cut(s, moves[i], 1 + (depth/PLY))
            && depth <= 6 * PLY
            && !extend
            && (beta == alpha + 1)
            && !PROMOTED(moves[i])) {
            remoneflag = remove_one_fast(&i, move_ordering, moves, num_moves);
            no_moves = FALSE;
            continue;
        }
        
        int afutprun = FALSE;
        int pfutprun = FALSE;

        if (depth <= 2 * PLY) {
            if (escore + 75 < alpha) {
                afutprun = TRUE;
            }
            if (escore + 200 < alpha) {
                pfutprun = TRUE;
            }
        } else if (depth <= 3 *PLY) {
            if (escore + 100 < alpha) {
                afutprun = TRUE;
            }
            if (escore + 300 < alpha) {
                pfutprun = TRUE;
            }
        } 

        int capval;
        if (CAPTURED(moves[i]) != npiece) {
            capval = see(s, TOMOVE(s), FROM(moves[i]), TARGET(moves[i]), PROMOTED(moves[i]));
        } else {
            capval = -INF;
        }

        make(s, moves[i]);

        legal_move = FALSE;

        if (check_legal(s, moves[i])) {
            legal_move = TRUE;
            no_moves = FALSE;

            afterincheck = in_check(s);

            if (afterincheck) {
                if (fullext) {
                    extend += PLY;
                } else {
                    extend += PLY/2;
                }
            }

            if (!incheck && !afterincheck && beta == alpha+1) {
                if (pfutprun) {
                    if (capval <= MAT_PAWN && !PROMOTED(moves[i])) {
                        unmake(s, moves[i]);
                        remoneflag = remove_one_fast(&i,move_ordering,moves,num_moves);
                        best_score = alpha;
                        continue;
                    }
                }
                if (afutprun) {
                    if (capval < -50 && !PROMOTED(moves[i])) {
                        unmake(s, moves[i]);
                        remoneflag = remove_one_fast(&i,move_ordering,moves,num_moves);
                        best_score = alpha;
                        continue;
                    }
                }
            }

            int newdepth = depth + extend - PLY;
            eval(s, -beta, -alpha+130, newdepth > 0 || afterincheck);

            s->checks[s->ply] = afterincheck;
            s->hash_history[gamestate.move_number + s->ply - 1] = s->hash;
            s->path[s->ply - 1] = moves[i];

            huber = 0;

            /* reductions */
            if (depth > PLY
                && mn > 3
                && beta == alpha + 1
                && !extend && !afterincheck
                && capval < -50
                && history_score(s, moves[i]) < 80
                && !PROMOTED(moves[i])
                ) {
                extend -= PLY;
                huber += PLY;
                newdepth = depth + extend - PLY;
            }

            if (first == TRUE) {
                if (newdepth <= 0) {
                    score = -qsearch(s,-beta,-alpha,0,0);
                } else {
                    score = -search(s,-beta,-alpha,newdepth,NONE, !cutnode);
                }
            } else {
                if (newdepth <= 0) {
                    score = -qsearch(s,-alpha-1,-alpha,0,0);
                } else {
                    score = -search(s,-alpha-1,-alpha,newdepth,NONE, TRUE);
                }
                if (score > best_score
                    && !gamestate.time_exit) {
                    if (score > alpha) {
                        if (huber) {
                            extend += huber;
                        }
                        if ((score < beta) || huber) {
                            newdepth = depth + extend - PLY;
                            if (newdepth <= 0) {
                                score = -qsearch(s,-beta,-alpha,0,0);
                            } else {
                                score = -search(s,-beta,-alpha,newdepth,NONE, (huber ? TRUE : FALSE));
                            }
                        }
                    }
                }
            }
            
            if (score > best_score) {
                best_score = score;
            }                
        }

        unmake(s, moves[i]);

        if (gamestate.time_exit) {
            return 0;
        }

        if (legal_move) {
            if (score > alpha) {                    
                if (score >= beta) {                        
                    history_good(s, moves[i], depth);                        

                    for (j = 0; j < mn - 1; j++) {                    
                        history_bad(s, searched_moves[j], depth);                            
                    }
                    StoreTT(s,score,originalalpha,beta,compact_move(moves[i]),threat,singular,
                            nosingular,depth);
                    return score;
                }

                alpha = score;
                best = compact_move(moves[i]);
            }

            first = FALSE;
            searched_moves[mn - 1] = moves[i];
            mn++;
        }

        remoneflag = remove_one_fast(&i,move_ordering,moves,num_moves);
    }    

    int validresult = !gamestate.time_exit;

    /* check for mate / stalemate: */
    if (no_moves && validresult) {
        if (in_check(s)) {
            StoreTT(s,-MATE + s->ply,originalalpha,beta,0,threat,singular,nosingular,depth);
            return (-MATE + s->ply);
        } else {
            StoreTT(s,0,originalalpha,beta,0,threat,singular,nosingular,depth);
            return 0;
        }
    } else {
        if (s->fifty >= 99) {
            return 0;
        }
    }    

    if (validresult) {
        StoreTT(s,best_score,originalalpha,beta,best,threat,singular,nosingular,depth);
    }

    return best_score;
}

/* this is a "proxy" to search which checks the root moves list */
int rootmovesearch(state_t *s, int alpha, int beta, int depth,
		   int is_null, int cutnode) {
    int res;
    res = search(s, alpha, beta, depth, is_null, cutnode);
    if (gamestate.time_exit) return 0;    
    return res;
}

move_s search_root(state_t *s, int originalalpha, int originalbeta, int depth) {
    move_s moves[MOVE_BUFF], best_move;
    int num_moves, i;
    int root_score = -MATE, move_ordering[MOVE_BUFF];
    int no_moves, legal_move, first;
    int alpha, beta;
    int dummy;
    unsigned int dummy2;
    int incheck;
    int mc;
    BITBOARD oldnodecount;
    int extend, huber;
    static int changes;
    static move_s bmove;
    char searching_move[STR_BUFF];

    alpha = originalalpha;
    beta = originalbeta;

    best_move = bmove;
    
    s->ply = 1;
    num_moves = 0;
    no_moves = TRUE;
    gamestate.cur_score = -MATE;

    incheck = s->checks[s->ply];
    if (incheck) {
        depth += PLY;
    }

    ProbeTT(s, &dummy, alpha, beta, &dummy2, &dummy, &dummy, &dummy, &dummy, depth);

    /*
        generate and order moves:
    */
    if (in_check(s)) {
        num_moves = gen_evasions(s, moves, in_check(s));
    } else {
        num_moves = gen(s, moves);
    }

    int movetotal = gamestate.legals;

    order_moves(s, moves, move_ordering, num_moves, gamestate.pv_best);

    for (i = 0; i < MOVE_BUFF; i++) {
        multipv_scores[i] = -MATE;
        root_scores[i] = -MATE;
    }

    eval(s, alpha, beta, TRUE);

    first = TRUE;
    i = -1;
    mc = 0;

    /* loop through the moves at the current node: */
    while (remove_one_fast(&i, move_ordering, moves, num_moves)) {
        make(s, moves[i]);

        legal_move = FALSE;

        s->hash_history[gamestate.move_number + s->ply - 1] = s->hash;
        s->path[s->ply - 1] = moves[i];

        oldnodecount = s->nodes;

        /* go deeper if it's a legal move: */
        if (check_legal(s, moves[i])) {

            unmake(s, moves[i]);
            mc++;
            int moveleft = movetotal - mc;

            if (!uci_mode) {
                comp_to_san(s, moves[i], searching_move);
            } else {
                comp_to_coord(s, moves[i], searching_move);
            }
            
            make(s, moves[i]);

            huber = 0;
            extend = 0;

            s->checks[s->ply] = in_check(s);

            eval(s, -beta, -alpha, TRUE);

            if (s->checks[s->ply]) {
                extend += 4;
            } else if ((((s->sboard[TARGET(moves[i])] == wpawn) && (rank(TARGET(moves[i])) >= 7))
                          || ((s->sboard[TARGET(moves[i])] == bpawn) && (rank(TARGET(moves[i])) <= 2))
                          || (PROMOTED(moves[i])))) {
                extend += 3;
            }

            if (!extend
                && !incheck
                && mc > 3
                && history_score(s, moves[i]) < 80
                && !(PROMOTED(moves[i]) || CAPTURED(moves[i]) != npiece)) {
                huber = 1*PLY;
            }

            if (((first == TRUE) || gamestate.i_depth < 2 || uci_multipv > 1)) {
                if (uci_multipv == 1) {
                    root_score = -rootmovesearch(s, -beta, -alpha, depth + extend - PLY, NONE, FALSE);
                } else {
                    root_score = -rootmovesearch(s, -beta, INF, depth + extend - PLY, NONE, FALSE);
                }
                multipv_scores[mc] = root_score;
                root_scores[i] = root_score;                

                if (!gamestate.time_exit) {
                    if (root_score >= beta) {
                        post_fh_thinking(s, root_score, moves[i], searching_move, movetotal - moveleft);
                    } else if (root_score <= alpha && first) {
                        gamestate.failed = 1;

                        post_fl_thinking(s, root_score, moves[i], searching_move, movetotal - moveleft);

                        root_score = -rootmovesearch(s, -beta, INF, depth + extend - PLY, NONE, FALSE);

                        if (!gamestate.time_exit) {
                            if (uci_multipv == 1) {
                                post_thinking(s, root_score, moves[i], searching_move, movetotal - moveleft);
                            }
                        }
                    } else if (root_score > alpha && root_score < beta) {
                        if (uci_multipv == 1) {
                            post_thinking(s, root_score, moves[i], searching_move, movetotal - moveleft);
                        }
                    }

                    if (uci_multipv > 1) {
                        post_multipv_thinking(s, root_score, mc, moves[i]);
                    }

                    if (root_score > gamestate.cur_score && !gamestate.time_exit) {
                        gamestate.cur_score = root_score;
                    }
                }
            } else {
                root_score = -rootmovesearch(s,-alpha - 1, -alpha, depth + extend - PLY - huber, FALSE, TRUE);

                if ((root_score > alpha) && ((root_score < beta) || huber)
                    && !gamestate.time_exit) {
                    /* was -root_score,-alpha */
                    gamestate.failedhigh = 1;

                    root_score = -rootmovesearch(s,-beta,-alpha,depth + extend - PLY, FALSE, FALSE);

                    gamestate.failedhigh = 0;

                    if (root_score > alpha && !gamestate.time_exit) {
                        gamestate.cur_score = root_score;
                        best_move = moves[i];
                        gamestate.pv_best = compact_move(best_move);

                        if (root_score < beta) {
                            post_thinking(s, root_score, moves[i], searching_move, movetotal - moveleft);
                        }
                    }
                }

                if (root_score >= beta && !gamestate.time_exit) {
                    post_fh_thinking(s, root_score, moves[i], searching_move, movetotal - moveleft);
                }
            }

            if (root_score > gamestate.cur_score && !gamestate.time_exit) {
                gamestate.cur_score = root_score;
            }
            
            if ((gamestate.time_exit) && (gamestate.cur_score == -MATE)) {
                if (no_moves) {
                    gamestate.time_failure = TRUE;
                }
            }

            no_moves = FALSE;
            legal_move = TRUE;
        }

        if (uci_mode && gamestate.i_depth > 4 && (rdifftime(rtime(), gamestate.start_time) >= 150)
            && uci_showrefutations) {
            myprintf("info refutation %s ", searching_move);
            extract_current_bestline(s);
        }

        unmake(s, moves[i]);

        /* if we've run out of time, return the best we have so far: */
        if (gamestate.time_exit) {
            return best_move;
        }

        if (legal_move) {
            /* check our current score vs. alpha: */
            if (root_score > alpha) {
                history_good(s, moves[i], depth);

                alpha = root_score;
                best_move = moves[i];
                gamestate.cur_score = alpha;
                gamestate.pv_best = compact_move(best_move);

                if (gamestate.cur_score >= beta) {
                    StoreTT(s, gamestate.cur_score, originalalpha, originalbeta, gamestate.pv_best,
                            FALSE, FALSE, FALSE, depth);
                    return best_move;
                }
            }

             if (first) {
                gamestate.pvnodecount = s->nodes - oldnodecount;
                first = FALSE;
            }
        }
    }

    if (gamestate.i_depth == 4) {
        changes = 0;
    } else if (gamestate.i_depth > 4) {
        if (best_move != bmove) {
            changes++;
        }
    }
    if ((((BITBOARD) (gamestate.pvnodecount * 100) / s->nodes) >= 75)
        && (gamestate.i_depth > 6)
        && !(gamestate.time_for_move == 99999999)
        && !gamestate.failed) {
        gamestate.time_for_move = gamestate.time_for_move / 2;
    }
    if (changes > 3) {
        gamestate.failedhigh = 1;
        changes--;
    } else {
        /* one extra ply without changes, cancel time extension */
        if (gamestate.failedhigh) {
            gamestate.failedhigh = 0;
        }
    }
    
    bmove = best_move;
    /* check to see if we are mated / stalemated: */
    if (no_moves && !is_pondering && !gamestate.time_failure) {
        if (in_check(s)) {
            if (s->white_to_move == 1) {
                gamestate.result = white_is_mated;
            } else {
                gamestate.result = black_is_mated;
            }
        } else {
            gamestate.result = stalemate;
        }
    } else {
        /* check for draw by the 50 move rule: */
        if (s->fifty >= 99 && !is_pondering && !uci_mode) {
            gamestate.result = draw_by_fifty;
            gamestate.cur_score = 0;
        }
    }

    StoreTT(s, gamestate.cur_score, originalalpha, originalbeta,
            gamestate.pv_best, FALSE, FALSE, FALSE, depth);
    return best_move;
}

void reset_search_counters(state_t *s) {
    s->nodes = 0;
    s->qnodes = 0;
    s->ply = 0;
    s->maxply = 0;

    /* a separate copy means we can keep using our time check mechanism */
    for (int i = 0; i < MAX_CPU; i++) {
        scoreboard.threadstate[i].nodes = 0;
        scoreboard.threadstate[i].qnodes = 0;
    }

    s->TTProbes = 0;
    s->TTHits = 0;
    s->TTStores = 0;
    s->TTColls = 0;

    gamestate.extendedtime = 0;
}

void reset_search_data(state_t *s) {
    /* clear the history heuristic: */
    memset(history_h, 0, sizeof(history_h));
    memset(history_hit, 0, sizeof(history_hit));
    memset(history_tot, 0, sizeof(history_tot));

    /* clear the killer moves: */
    for (int i = 0; i < PV_BUFF; i++) {
        s->killerstack[i].killer1 = 0;
        s->killerstack[i].killer2 = 0;
        s->killerstack[i].killer3 = 0;
        s->killerstack[i].killer4 = 0;
    }

    gamestate.pvnodecount = 0;
    gamestate.pv_best = 0;

    if (uci_limitstrength) {
        gamestate.maxdepth = std::min(gamestate.maxdepth, elo_to_depth(uci_elo));
    }
}

move_s think(gamestate_t *g, state_t *s) {
    move_s comp_move, temp_move;
    int elapsed, temp_score;
    char output[STR_BUFF], output2[STR_BUFF];
    int alpha, beta;
    int rs;
    move_s moves[MOVE_BUFF];
    int l, lastlegal, ic;
    int num_moves;
    static int lastsearchscore;
    int true_i_depth;
    move_s pondermove = 0;
    
    clear_dp_tt();
    reset_search_data(s);
    reset_search_counters(s);

    rs = 0;
    true_i_depth = 0;

    gamestate.start_time = rtime();

    int legals = 0;

    s->ply = 1;
    /*
        set up the search stack
    */
    s->hash_history[gamestate.move_number] = s->hash;

    ic = in_check(s);
    s->checks[s->ply] = ic;    

    if (ic) {
        num_moves = gen_evasions(s, moves, ic);
    } else {
        num_moves = gen(s, moves);
    }

    lastlegal = 0;
    for (l = 0; l < num_moves; l++) {
        make(s, moves[l]);

        if (check_legal(s, moves[l])) {
            legals++;
            lastlegal = l;
        }

        unmake(s, moves[l]);
    }

    if (!is_pondering) {
        if (legals == 1) {
            if (uci_mode) {
                comp_to_coord(s, moves[lastlegal], output);
                myprintf("info depth 1 time 0 nodes 1 score cp %d\n", lastsearchscore);
                myprintf("bestmove %s\n",output);
            }
            return moves[lastlegal];
        }
    }

    check_phase(&gamestate, s);

    if (!uci_mode) {
        switch (gamestate.phase) {
            case OPENING:
                myprintf("Opening phase.\n");
                break;
            case MIDDLEGAME:
                myprintf("Middlegame phase.\n");
                break;
            case ENDGAME:
                myprintf("Endgame phase.\n");
                break;
        }
    }

    /* allocate our time for this move: */
    if (!is_pondering && !is_analyzing) {
        if (!gamestate.fixed_time) {
            gamestate.time_for_move = allocate_time(&gamestate, TRUE);
        } else {
            gamestate.time_for_move = gamestate.fixed_time;
        }
    } else {
        gamestate.time_for_move = 99999999;
    }

#if !defined COPYPROTECTION
    if (!uci_mode) {
        myprintf("Time for move : %d\n", gamestate.time_for_move);
    } else {
        if (gamestate.time_for_move != 99999999) {
            myprintf("info string Time for move: %ds, early break: %ds\n",
                     gamestate.time_for_move / 100,
                     (int)((gamestate.time_for_move * 1.0 / 2.5) / 100));
        }
    }
#endif
 
    gamestate.time_exit = FALSE;
    gamestate.time_failure = FALSE;

    gamestate.failed = 0;
    gamestate.failedhigh = 0;

    comp_move = 0;
    temp_score = 0;
    gamestate.cur_score = 0;

    gamestate.i_depth = 1;

    for (;
         gamestate.i_depth <= std::min(40, gamestate.maxdepth);
         gamestate.i_depth++) {

        if (uci_mode) {
            s->TTStores >>= 1;
            s->TTColls >>= 1;
        }

        if (gamestate.i_depth > s->maxply) {
            s->maxply = gamestate.i_depth;
        }

        /*
            don't bother going deeper if we've already used 1/2 of our time, and we
            haven't finished our mindepth search, since we likely won't finish
        */
        elapsed = rdifftime(rtime(), gamestate.start_time);

        if ((!gamestate.failed && !gamestate.failedhigh)
             && ((gamestate.extendedtime)
              || (!gamestate.fixed_time && (elapsed > gamestate.time_for_move * 1.0 / 2.5)))
            && (gamestate.i_depth > MINDEPTH)) {
            break;
        }

        rs = 1;

        alpha = temp_score - 30;
        beta  = temp_score + 30;
        //alpha = -INF;
        //beta = INF;

        temp_move = search_root(s, alpha, beta, (gamestate.i_depth * PLY));

        if (gamestate.result) {
            break;
        }

        if (gamestate.cur_score <= alpha && !gamestate.time_exit) {
            gamestate.failed = 1;
        } else {
            gamestate.failed = 0;
        }

        if (gamestate.cur_score <= alpha && !gamestate.time_exit) {
            /* fail low */
            alpha = -INF;

            rs++;

            temp_move = search_root(s, alpha, beta, (gamestate.i_depth * PLY));

            if (!gamestate.time_exit) {
                gamestate.failed = 0;
            }

            if (gamestate.cur_score >= beta && !gamestate.time_exit) {
                temp_move = search_root(s, -INF, +INF, (gamestate.i_depth * PLY));

                if (!gamestate.time_exit) {
                    gamestate.failed = 0;
                }
            }
        } else if (gamestate.cur_score >= beta && !gamestate.time_exit) {
            /* fail high */
            comp_move = temp_move;
            temp_score = gamestate.cur_score;

            beta = +INF;

            rs++;

            temp_move = search_root(s, alpha, beta, (gamestate.i_depth * PLY));

            if (gamestate.cur_score <= alpha && !gamestate.time_exit) {
                gamestate.failed = 1;
                temp_move = search_root(s, -INF, +INF, (gamestate.i_depth * PLY));
                if (!gamestate.time_exit) {
                    gamestate.failed = 0;
                }
            };
        };

        if (!uci_mode) {
            if (interrupt()) {
                if (is_pondering) {
                    break;
                }
            }
        }

        if (!gamestate.time_failure
            && !gamestate.failed
            && temp_move == 0) {
            myprintf("info string Nonsense in temp_move, time_failure %d failed %d time_exit %d result %d\n",
                      gamestate.time_failure, gamestate.failed, gamestate.time_exit, gamestate.result);
            myprintf("bestmove 0000\n");
            return 0;
        }

        /*
            if we haven't aborted our search on time,
            set the computer's move
            and post our thinking:
        */
        if (!gamestate.time_failure) {
            /* accidentally pondering if mated */
            if (!uci_mode && gamestate.cur_score == -MATE) {
                return 0;
            }

            comp_move = temp_move;
            temp_score = gamestate.cur_score;

            if (!gamestate.time_exit) {
                true_i_depth = gamestate.i_depth;
            }

            if (!gamestate.time_exit && !uci_mode && uci_multipv == 1) {
                post_thinking(s, gamestate.cur_score, comp_move, 0, 0);
            }

            if ((gamestate.i_depth > MINDEPTH)
                 && temp_score > MATE - 500
                 && ((int)(MATE - gamestate.cur_score) < gamestate.i_depth)
                 && (!is_pondering || gamestate.time_for_move != 99999999)
                 ) {
                break;
            }
        }

        if (gamestate.time_exit) {
            break;
        }
    }

    if (comp_move == 0) {
        myprintf("bestmove 0000\n");
        return 0;
    }

    if (gamestate.i_depth >= 32 && is_pondering && uci_mode && !buffered_count) {
        while ((gamestate.time_for_move == 99999999) && !interrupt());
    }

    if (uci_mode) {
        // get ponder move
        make(s, comp_move);
        pondermove = extract_ponder_move(s);
        unmake(s, comp_move);

        comp_to_coord(s, comp_move, output);

        if (pondermove != 0) {
            comp_to_coord(s, pondermove, output2);
            myprintf("bestmove %s ponder %s\n", output, output2);
        } else {
            myprintf("bestmove %s\n", output);
        }
    }

    if (temp_score == MATE - 2 && !is_pondering) {
        if (s->white_to_move) {
            gamestate.result = black_is_mated;
            pondermove = 0;
        } else {
            gamestate.result = white_is_mated;
            pondermove = 0;
        }
    }

    if (elapsed > 20) {
        time_check_log = logL((int)((((float)s->nodes / (float)(elapsed / 100.0f))) / 20.0f));
        if (time_check_log < 8) {
            time_check_log = 8;
        }
        if (time_check_log > 17) {
            time_check_log = 17;
        }
    }

    if (!in_check(s)
        && !is_pondering
        && (gamestate.result != white_is_mated)
        && (gamestate.result != black_is_mated)
        && (gamestate.result != stalemate)
        && (gamestate.result != draw_by_fifty)
        && (gamestate.result != draw_by_rep)
        && (true_i_depth >= 5)) {
        lastsearchscore = temp_score;
    }

    return comp_move;
}
