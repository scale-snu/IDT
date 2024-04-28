#ifndef NEVAL_H
#define NEVAL_H

/*
    defs
*/    

/*
    data
*/
extern const int material[14];

/*
    eval struct
*/
struct t_eval_comps {                      
    int w_positional;
    int b_positional;
            
    int w_pawnstruct;
    int b_pawnstruct;        
            
    int w_passers;
    int b_passers;        
        
    int w_psq_king;    
    int b_psq_king;                      
         
    int w_ks_score;        
    int b_ks_score;     
    
    int accum_score;       
            
};

/* 
    functions
*/
int retrieve_eval(state_t *s);
int eval (state_t *s, const int alpha, const int beta, const int forcefull);
void feval(state_t *s, t_eval_comps *ec);

#endif
