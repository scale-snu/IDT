/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2008 Gian-Carlo Pascutto

    File: extvars.h                                        
    Purpose: global data definitions

*/
#include "sjeng.h"

extern char buffered_command[20][BIG_BUFF];
extern int buffered_count;
extern int is_pondering, allow_pondering, is_analyzing;
extern unsigned int TTSize;
extern int uci_mode;
extern int uci_chess960_mode;
extern int uci_showcurrline;
extern int uci_showrefutations;
extern int uci_limitstrength;
extern int uci_elo;
extern int uci_multipv;
extern int cfg_logging;
extern char cfg_logfile[STR_BUFF];
extern int contempt;
extern int time_check_log;
extern int global_id;
extern int EGTBHits;
extern int EGTBProbes;
