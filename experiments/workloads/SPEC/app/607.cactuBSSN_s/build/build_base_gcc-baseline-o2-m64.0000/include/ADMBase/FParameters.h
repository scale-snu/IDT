#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_STRING,admbase_boundary_condition,)&&\
CCTK_DECLARE(CCTK_STRING,dtlapse_evolution_method,)&&\
CCTK_DECLARE(CCTK_STRING,dtshift_evolution_method,)&&\
CCTK_DECLARE(CCTK_STRING,evolution_method,)&&\
CCTK_DECLARE(CCTK_STRING,initial_data,)&&\
CCTK_DECLARE(CCTK_STRING,initial_dtlapse,)&&\
CCTK_DECLARE(CCTK_STRING,initial_dtshift,)&&\
CCTK_DECLARE(CCTK_STRING,initial_lapse,)&&\
CCTK_DECLARE(CCTK_STRING,initial_shift,)&&\
CCTK_DECLARE(CCTK_STRING,lapse_evolution_method,)&&\
CCTK_DECLARE(CCTK_STRING,lapse_prolongation_type,)&&\
CCTK_DECLARE(CCTK_STRING,metric_prolongation_type,)&&\
CCTK_DECLARE(CCTK_STRING,metric_type,)&&\
CCTK_DECLARE(CCTK_STRING,shift_evolution_method,)&&\
CCTK_DECLARE(CCTK_STRING,shift_prolongation_type,)&&\
CCTK_DECLARE(CCTK_INT,lapse_timelevels,)&&\
CCTK_DECLARE(CCTK_INT,metric_timelevels,)&&\
CCTK_DECLARE(CCTK_INT,shift_timelevels,)&&\
COMMON /ADMBaserest/admbase_boundary_condition, dtlapse_evolution_method, dtshift_evolution_method, evolution_method, initial_data, initial_dtlapse, initial_dtshift, initial_lapse, initial_shift, lapse_evolution_method, lapse_prolongation_type, metric_prolongation_type, metric_type, shift_evolution_method, shift_prolongation_type, lapse_timelevels, metric_timelevels, shift_timelevels&&\

