/*
    Sjeng - a chess playing program
    Copyright (C) 2000-2009 Gian-Carlo Pascutto   
                                                          
    File: rcfile.c
    Purpose: Read in config file, allocate hash/caches                  

*/
#include "sjeng.h"
#include "extvars.h"
#include "ttable.h"
#include "config.h"
#include "state.h"
                
void init_parameters( gamestate_t *g ) {        
#ifdef SMALL_MEMORY
    TTSize = 15000000; // ~720M
#elif BIG_MEMORY
    TTSize = 150000000; // ~7.5G
#else
#error Need to specify SMALL_MEMORY or BIG_MEMORY.
#endif

    strcpy(cfg_logfile, "sjeng.log");
    cfg_logging = 0;

    contempt = 0;               
    g->maxdepth = 60;
    
    uci_mode = FALSE;
    uci_chess960_mode = FALSE;  
    uci_showcurrline = FALSE;
    uci_showrefutations = FALSE;
    uci_limitstrength = FALSE;
    uci_elo = 3000;
    uci_multipv = 1;
    
    alloc_hash();
    
    return;
}
