#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL ICN_avg_theta;
  CCTK_REAL MoL_Tiny;
  CCTK_REAL RHS_error_weight;
  CCTK_REAL maximum_absolute_error;
  CCTK_REAL maximum_decrease;
  CCTK_REAL maximum_increase;
  CCTK_REAL maximum_relative_error;
  CCTK_REAL safety_factor;
  const char * AB_Type;
  const char * Generic_Method_Descriptor;
  const char * Generic_Type;
  const char * ODE_Method;
  const char * verbose;
  CCTK_INT AB_initially_reduce_order;
  CCTK_INT ICN_avg_swapped;
  CCTK_INT MoL_Intermediate_Steps;
  CCTK_INT MoL_Memory_Always_On;
  CCTK_INT MoL_NaN_Check;
  CCTK_INT adaptive_stepsize;
  CCTK_INT copy_ID_after_MoL_PostStep;
  CCTK_INT disable_prolongation;
  CCTK_INT init_RHS_zero;
  CCTK_INT initial_data_is_crap;
  CCTK_INT run_MoL_PostStep_in_Post_Recover_Variables;
  CCTK_INT set_ID_boundaries;
  CCTK_INT skip_initial_copy;
} PRIVATE_MOL_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_MOL_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, ICN_avg_theta, CCTK_PARAMETER__MOL__ICN_avg_theta); \
  CCTK_DECLARE_INIT (CCTK_REAL const, MoL_Tiny, CCTK_PARAMETER__MOL__MoL_Tiny); \
  CCTK_DECLARE_INIT (CCTK_REAL const, RHS_error_weight, CCTK_PARAMETER__MOL__RHS_error_weight); \
  CCTK_DECLARE_INIT (CCTK_REAL const, maximum_absolute_error, CCTK_PARAMETER__MOL__maximum_absolute_error); \
  CCTK_DECLARE_INIT (CCTK_REAL const, maximum_decrease, CCTK_PARAMETER__MOL__maximum_decrease); \
  CCTK_DECLARE_INIT (CCTK_REAL const, maximum_increase, CCTK_PARAMETER__MOL__maximum_increase); \
  CCTK_DECLARE_INIT (CCTK_REAL const, maximum_relative_error, CCTK_PARAMETER__MOL__maximum_relative_error); \
  CCTK_DECLARE_INIT (CCTK_REAL const, safety_factor, CCTK_PARAMETER__MOL__safety_factor); \
  CCTK_DECLARE_INIT (const char * const, AB_Type, CCTK_PARAMETER__MOL__AB_Type); \
  CCTK_DECLARE_INIT (const char * const, Generic_Method_Descriptor, CCTK_PARAMETER__MOL__Generic_Method_Descriptor); \
  CCTK_DECLARE_INIT (const char * const, Generic_Type, CCTK_PARAMETER__MOL__Generic_Type); \
  CCTK_DECLARE_INIT (const char * const, ODE_Method, CCTK_PARAMETER__MOL__ODE_Method); \
  CCTK_DECLARE_INIT (const char * const, verbose, CCTK_PARAMETER__MOL__verbose); \
  CCTK_DECLARE_INIT (CCTK_INT const, AB_initially_reduce_order, CCTK_PARAMETER__MOL__AB_initially_reduce_order); \
  CCTK_DECLARE_INIT (CCTK_INT const, ICN_avg_swapped, CCTK_PARAMETER__MOL__ICN_avg_swapped); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Intermediate_Steps, CCTK_PARAMETER__MOL__MoL_Intermediate_Steps); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Memory_Always_On, CCTK_PARAMETER__MOL__MoL_Memory_Always_On); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_NaN_Check, CCTK_PARAMETER__MOL__MoL_NaN_Check); \
  CCTK_DECLARE_INIT (CCTK_INT const, adaptive_stepsize, CCTK_PARAMETER__MOL__adaptive_stepsize); \
  CCTK_DECLARE_INIT (CCTK_INT const, copy_ID_after_MoL_PostStep, CCTK_PARAMETER__MOL__copy_ID_after_MoL_PostStep); \
  CCTK_DECLARE_INIT (CCTK_INT const, disable_prolongation, CCTK_PARAMETER__MOL__disable_prolongation); \
  CCTK_DECLARE_INIT (CCTK_INT const, init_RHS_zero, CCTK_PARAMETER__MOL__init_RHS_zero); \
  CCTK_DECLARE_INIT (CCTK_INT const, initial_data_is_crap, CCTK_PARAMETER__MOL__initial_data_is_crap); \
  CCTK_DECLARE_INIT (CCTK_INT const, run_MoL_PostStep_in_Post_Recover_Variables, CCTK_PARAMETER__MOL__run_MoL_PostStep_in_Post_Recover_Variables); \
  CCTK_DECLARE_INIT (CCTK_INT const, set_ID_boundaries, CCTK_PARAMETER__MOL__set_ID_boundaries); \
  CCTK_DECLARE_INIT (CCTK_INT const, skip_initial_copy, CCTK_PARAMETER__MOL__skip_initial_copy); \


#ifndef CCTK_PARAMETER__MOL__ICN_avg_theta
#  define CCTK_PARAMETER__MOL__ICN_avg_theta PRIVATE_MOL_STRUCT.ICN_avg_theta
#endif
#ifndef CCTK_PARAMETER__MOL__MoL_Tiny
#  define CCTK_PARAMETER__MOL__MoL_Tiny PRIVATE_MOL_STRUCT.MoL_Tiny
#endif
#ifndef CCTK_PARAMETER__MOL__RHS_error_weight
#  define CCTK_PARAMETER__MOL__RHS_error_weight PRIVATE_MOL_STRUCT.RHS_error_weight
#endif
#ifndef CCTK_PARAMETER__MOL__maximum_absolute_error
#  define CCTK_PARAMETER__MOL__maximum_absolute_error PRIVATE_MOL_STRUCT.maximum_absolute_error
#endif
#ifndef CCTK_PARAMETER__MOL__maximum_decrease
#  define CCTK_PARAMETER__MOL__maximum_decrease PRIVATE_MOL_STRUCT.maximum_decrease
#endif
#ifndef CCTK_PARAMETER__MOL__maximum_increase
#  define CCTK_PARAMETER__MOL__maximum_increase PRIVATE_MOL_STRUCT.maximum_increase
#endif
#ifndef CCTK_PARAMETER__MOL__maximum_relative_error
#  define CCTK_PARAMETER__MOL__maximum_relative_error PRIVATE_MOL_STRUCT.maximum_relative_error
#endif
#ifndef CCTK_PARAMETER__MOL__safety_factor
#  define CCTK_PARAMETER__MOL__safety_factor PRIVATE_MOL_STRUCT.safety_factor
#endif
#ifndef CCTK_PARAMETER__MOL__AB_Type
#  define CCTK_PARAMETER__MOL__AB_Type PRIVATE_MOL_STRUCT.AB_Type
#endif
#ifndef CCTK_PARAMETER__MOL__Generic_Method_Descriptor
#  define CCTK_PARAMETER__MOL__Generic_Method_Descriptor PRIVATE_MOL_STRUCT.Generic_Method_Descriptor
#endif
#ifndef CCTK_PARAMETER__MOL__Generic_Type
#  define CCTK_PARAMETER__MOL__Generic_Type PRIVATE_MOL_STRUCT.Generic_Type
#endif
#ifndef CCTK_PARAMETER__MOL__ODE_Method
#  define CCTK_PARAMETER__MOL__ODE_Method PRIVATE_MOL_STRUCT.ODE_Method
#endif
#ifndef CCTK_PARAMETER__MOL__verbose
#  define CCTK_PARAMETER__MOL__verbose PRIVATE_MOL_STRUCT.verbose
#endif
#ifndef CCTK_PARAMETER__MOL__AB_initially_reduce_order
#  define CCTK_PARAMETER__MOL__AB_initially_reduce_order PRIVATE_MOL_STRUCT.AB_initially_reduce_order
#endif
#ifndef CCTK_PARAMETER__MOL__ICN_avg_swapped
#  define CCTK_PARAMETER__MOL__ICN_avg_swapped PRIVATE_MOL_STRUCT.ICN_avg_swapped
#endif
#ifndef CCTK_PARAMETER__MOL__MoL_Intermediate_Steps
#  define CCTK_PARAMETER__MOL__MoL_Intermediate_Steps PRIVATE_MOL_STRUCT.MoL_Intermediate_Steps
#endif
#ifndef CCTK_PARAMETER__MOL__MoL_Memory_Always_On
#  define CCTK_PARAMETER__MOL__MoL_Memory_Always_On PRIVATE_MOL_STRUCT.MoL_Memory_Always_On
#endif
#ifndef CCTK_PARAMETER__MOL__MoL_NaN_Check
#  define CCTK_PARAMETER__MOL__MoL_NaN_Check PRIVATE_MOL_STRUCT.MoL_NaN_Check
#endif
#ifndef CCTK_PARAMETER__MOL__adaptive_stepsize
#  define CCTK_PARAMETER__MOL__adaptive_stepsize PRIVATE_MOL_STRUCT.adaptive_stepsize
#endif
#ifndef CCTK_PARAMETER__MOL__copy_ID_after_MoL_PostStep
#  define CCTK_PARAMETER__MOL__copy_ID_after_MoL_PostStep PRIVATE_MOL_STRUCT.copy_ID_after_MoL_PostStep
#endif
#ifndef CCTK_PARAMETER__MOL__disable_prolongation
#  define CCTK_PARAMETER__MOL__disable_prolongation PRIVATE_MOL_STRUCT.disable_prolongation
#endif
#ifndef CCTK_PARAMETER__MOL__init_RHS_zero
#  define CCTK_PARAMETER__MOL__init_RHS_zero PRIVATE_MOL_STRUCT.init_RHS_zero
#endif
#ifndef CCTK_PARAMETER__MOL__initial_data_is_crap
#  define CCTK_PARAMETER__MOL__initial_data_is_crap PRIVATE_MOL_STRUCT.initial_data_is_crap
#endif
#ifndef CCTK_PARAMETER__MOL__run_MoL_PostStep_in_Post_Recover_Variables
#  define CCTK_PARAMETER__MOL__run_MoL_PostStep_in_Post_Recover_Variables PRIVATE_MOL_STRUCT.run_MoL_PostStep_in_Post_Recover_Variables
#endif
#ifndef CCTK_PARAMETER__MOL__set_ID_boundaries
#  define CCTK_PARAMETER__MOL__set_ID_boundaries PRIVATE_MOL_STRUCT.set_ID_boundaries
#endif
#ifndef CCTK_PARAMETER__MOL__skip_initial_copy
#  define CCTK_PARAMETER__MOL__skip_initial_copy PRIVATE_MOL_STRUCT.skip_initial_copy
#endif

