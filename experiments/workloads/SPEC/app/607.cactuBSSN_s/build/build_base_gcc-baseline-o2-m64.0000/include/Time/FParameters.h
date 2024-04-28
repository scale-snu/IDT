#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_STRING,timestep_method,)&&\
CCTK_DECLARE(CCTK_INT,timestep_outonly,)&&\
COMMON /timerest/timestep_method, timestep_outonly&&\
CCTK_DECLARE(CCTK_REAL,courant_fac,)&&\
CCTK_DECLARE(CCTK_REAL,dtfac,)&&\
CCTK_DECLARE(CCTK_REAL,timestep,)&&\
CCTK_DECLARE(CCTK_INT,timestep_outevery,)&&\
CCTK_DECLARE(CCTK_INT,verbose,)&&\
COMMON /Timepriv/courant_fac, dtfac, timestep, timestep_outevery, verbose&&\
CCTK_DECLARE(CCTK_REAL,cctk_final_time,)&&\
CCTK_DECLARE(CCTK_REAL,CCTKH0,)&&\
CCTK_DECLARE(CCTK_REAL,CCTKH2,)&&\
CCTK_DECLARE(CCTK_STRING,terminate,)&&\
CCTK_DECLARE(CCTK_INT,CCTKH1,)&&\
CCTK_DECLARE(CCTK_INT,CCTKH3,)&&\
COMMON /CACTUSrest/cctk_final_time, CCTKH0, CCTKH2, terminate, CCTKH1, CCTKH3&&\

