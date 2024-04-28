#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_INT,MoL_Max_Evolved_Array_Size,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Max_Evolved_ComplexArray_Size,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ArrayConstrained_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ArrayEvolved_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ArraySaveAndRestore_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ComplexArrayConstrained_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ComplexArrayEvolved_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ComplexArraySaveAndRestore_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ComplexConstrained_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ComplexEvolved_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_ComplexSaveAndRestore_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_Constrained_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_Evolved_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_Evolved_Vars_Slow,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_SaveAndRestore_Vars,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Num_Scratch_Levels,)&&\
COMMON /MethodOfLinesrest/MoL_Max_Evolved_Array_Size, MoL_Max_Evolved_ComplexArray_Size, MoL_Num_ArrayConstrained_Vars, MoL_Num_ArrayEvolved_Vars, MoL_Num_ArraySaveAndRestore_Vars, MoL_Num_ComplexArrayConstrained_Vars, MoL_Num_ComplexArrayEvolved_Vars, MoL_Num_ComplexArraySaveAndRestore_Vars, MoL_Num_ComplexConstrained_Vars, MoL_Num_ComplexEvolved_Vars, MoL_Num_ComplexSaveAndRestore_Vars, MoL_Num_Constrained_Vars, MoL_Num_Evolved_Vars, MoL_Num_Evolved_Vars_Slow, MoL_Num_SaveAndRestore_Vars, MoL_Num_Scratch_Levels&&\
CCTK_DECLARE(CCTK_REAL,ICN_avg_theta,)&&\
CCTK_DECLARE(CCTK_REAL,MoL_Tiny,)&&\
CCTK_DECLARE(CCTK_REAL,RHS_error_weight,)&&\
CCTK_DECLARE(CCTK_REAL,maximum_absolute_error,)&&\
CCTK_DECLARE(CCTK_REAL,maximum_decrease,)&&\
CCTK_DECLARE(CCTK_REAL,maximum_increase,)&&\
CCTK_DECLARE(CCTK_REAL,maximum_relative_error,)&&\
CCTK_DECLARE(CCTK_REAL,safety_factor,)&&\
CCTK_DECLARE(CCTK_STRING,AB_Type,)&&\
CCTK_DECLARE(CCTK_STRING,Generic_Method_Descriptor,)&&\
CCTK_DECLARE(CCTK_STRING,Generic_Type,)&&\
CCTK_DECLARE(CCTK_STRING,ODE_Method,)&&\
CCTK_DECLARE(CCTK_STRING,verbose,)&&\
CCTK_DECLARE(CCTK_INT,AB_initially_reduce_order,)&&\
CCTK_DECLARE(CCTK_INT,ICN_avg_swapped,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Intermediate_Steps,)&&\
CCTK_DECLARE(CCTK_INT,MoL_Memory_Always_On,)&&\
CCTK_DECLARE(CCTK_INT,MoL_NaN_Check,)&&\
CCTK_DECLARE(CCTK_INT,adaptive_stepsize,)&&\
CCTK_DECLARE(CCTK_INT,copy_ID_after_MoL_PostStep,)&&\
CCTK_DECLARE(CCTK_INT,disable_prolongation,)&&\
CCTK_DECLARE(CCTK_INT,init_RHS_zero,)&&\
CCTK_DECLARE(CCTK_INT,initial_data_is_crap,)&&\
CCTK_DECLARE(CCTK_INT,run_MoL_PostStep_in_Post_Recover_Variables,)&&\
CCTK_DECLARE(CCTK_INT,set_ID_boundaries,)&&\
CCTK_DECLARE(CCTK_INT,skip_initial_copy,)&&\
COMMON /MoLpriv/ICN_avg_theta, MoL_Tiny, RHS_error_weight, maximum_absolute_error, maximum_decrease, maximum_increase, maximum_relative_error, safety_factor, AB_Type, Generic_Method_Descriptor, Generic_Type, ODE_Method, verbose, AB_initially_reduce_order, ICN_avg_swapped, MoL_Intermediate_Steps, MoL_Memory_Always_On, MoL_NaN_Check, adaptive_stepsize, copy_ID_after_MoL_PostStep, disable_prolongation, init_RHS_zero, initial_data_is_crap, run_MoL_PostStep_in_Post_Recover_Variables, set_ID_boundaries, skip_initial_copy&&\
CCTK_DECLARE(CCTK_REAL,CCTKH0,)&&\
CCTK_DECLARE(CCTK_REAL,cctk_initial_time,)&&\
CCTK_DECLARE(CCTK_REAL,CCTKH2,)&&\
CCTK_DECLARE(CCTK_STRING,CCTKH3,)&&\
CCTK_DECLARE(CCTK_INT,CCTKH1,)&&\
CCTK_DECLARE(CCTK_INT,CCTKH4,)&&\
COMMON /CACTUSrest/CCTKH0, cctk_initial_time, CCTKH2, CCTKH3, CCTKH1, CCTKH4&&\

