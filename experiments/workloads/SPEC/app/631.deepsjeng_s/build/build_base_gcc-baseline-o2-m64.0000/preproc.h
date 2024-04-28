#ifndef PREPROC_H
#define PREPROC_H

#include "neval.h"

/*
    data
*/
extern const int flip[64];
extern signed char psq_table[12][64];

/* 
    functions
*/
void psq_eval(state_t *s, t_eval_comps *ec);
void preprocess_prepare(void);
void init_psq_score(state_t *s);

#endif
