#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * admbase_boundary_condition;
  const char * dtlapse_evolution_method;
  const char * dtshift_evolution_method;
  const char * evolution_method;
  const char * initial_data;
  const char * initial_dtlapse;
  const char * initial_dtshift;
  const char * initial_lapse;
  const char * initial_shift;
  const char * lapse_evolution_method;
  const char * lapse_prolongation_type;
  const char * metric_prolongation_type;
  const char * metric_type;
  const char * shift_evolution_method;
  const char * shift_prolongation_type;
  CCTK_INT lapse_timelevels;
  CCTK_INT metric_timelevels;
  CCTK_INT shift_timelevels;
} RESTRICTED_ADMBASE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_ADMBASE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, admbase_boundary_condition, CCTK_PARAMETER__ADMBASE__admbase_boundary_condition); \
  CCTK_DECLARE_INIT (const char * const, dtlapse_evolution_method, CCTK_PARAMETER__ADMBASE__dtlapse_evolution_method); \
  CCTK_DECLARE_INIT (const char * const, dtshift_evolution_method, CCTK_PARAMETER__ADMBASE__dtshift_evolution_method); \
  CCTK_DECLARE_INIT (const char * const, evolution_method, CCTK_PARAMETER__ADMBASE__evolution_method); \
  CCTK_DECLARE_INIT (const char * const, initial_data, CCTK_PARAMETER__ADMBASE__initial_data); \
  CCTK_DECLARE_INIT (const char * const, initial_dtlapse, CCTK_PARAMETER__ADMBASE__initial_dtlapse); \
  CCTK_DECLARE_INIT (const char * const, initial_dtshift, CCTK_PARAMETER__ADMBASE__initial_dtshift); \
  CCTK_DECLARE_INIT (const char * const, initial_lapse, CCTK_PARAMETER__ADMBASE__initial_lapse); \
  CCTK_DECLARE_INIT (const char * const, initial_shift, CCTK_PARAMETER__ADMBASE__initial_shift); \
  CCTK_DECLARE_INIT (const char * const, lapse_evolution_method, CCTK_PARAMETER__ADMBASE__lapse_evolution_method); \
  CCTK_DECLARE_INIT (const char * const, lapse_prolongation_type, CCTK_PARAMETER__ADMBASE__lapse_prolongation_type); \
  CCTK_DECLARE_INIT (const char * const, metric_prolongation_type, CCTK_PARAMETER__ADMBASE__metric_prolongation_type); \
  CCTK_DECLARE_INIT (const char * const, metric_type, CCTK_PARAMETER__ADMBASE__metric_type); \
  CCTK_DECLARE_INIT (const char * const, shift_evolution_method, CCTK_PARAMETER__ADMBASE__shift_evolution_method); \
  CCTK_DECLARE_INIT (const char * const, shift_prolongation_type, CCTK_PARAMETER__ADMBASE__shift_prolongation_type); \
  CCTK_DECLARE_INIT (CCTK_INT const, lapse_timelevels, CCTK_PARAMETER__ADMBASE__lapse_timelevels); \
  CCTK_DECLARE_INIT (CCTK_INT const, metric_timelevels, CCTK_PARAMETER__ADMBASE__metric_timelevels); \
  CCTK_DECLARE_INIT (CCTK_INT const, shift_timelevels, CCTK_PARAMETER__ADMBASE__shift_timelevels); \


#ifndef CCTK_PARAMETER__ADMBASE__admbase_boundary_condition
#  define CCTK_PARAMETER__ADMBASE__admbase_boundary_condition RESTRICTED_ADMBASE_STRUCT.admbase_boundary_condition
#endif
#ifndef CCTK_PARAMETER__ADMBASE__dtlapse_evolution_method
#  define CCTK_PARAMETER__ADMBASE__dtlapse_evolution_method RESTRICTED_ADMBASE_STRUCT.dtlapse_evolution_method
#endif
#ifndef CCTK_PARAMETER__ADMBASE__dtshift_evolution_method
#  define CCTK_PARAMETER__ADMBASE__dtshift_evolution_method RESTRICTED_ADMBASE_STRUCT.dtshift_evolution_method
#endif
#ifndef CCTK_PARAMETER__ADMBASE__evolution_method
#  define CCTK_PARAMETER__ADMBASE__evolution_method RESTRICTED_ADMBASE_STRUCT.evolution_method
#endif
#ifndef CCTK_PARAMETER__ADMBASE__initial_data
#  define CCTK_PARAMETER__ADMBASE__initial_data RESTRICTED_ADMBASE_STRUCT.initial_data
#endif
#ifndef CCTK_PARAMETER__ADMBASE__initial_dtlapse
#  define CCTK_PARAMETER__ADMBASE__initial_dtlapse RESTRICTED_ADMBASE_STRUCT.initial_dtlapse
#endif
#ifndef CCTK_PARAMETER__ADMBASE__initial_dtshift
#  define CCTK_PARAMETER__ADMBASE__initial_dtshift RESTRICTED_ADMBASE_STRUCT.initial_dtshift
#endif
#ifndef CCTK_PARAMETER__ADMBASE__initial_lapse
#  define CCTK_PARAMETER__ADMBASE__initial_lapse RESTRICTED_ADMBASE_STRUCT.initial_lapse
#endif
#ifndef CCTK_PARAMETER__ADMBASE__initial_shift
#  define CCTK_PARAMETER__ADMBASE__initial_shift RESTRICTED_ADMBASE_STRUCT.initial_shift
#endif
#ifndef CCTK_PARAMETER__ADMBASE__lapse_evolution_method
#  define CCTK_PARAMETER__ADMBASE__lapse_evolution_method RESTRICTED_ADMBASE_STRUCT.lapse_evolution_method
#endif
#ifndef CCTK_PARAMETER__ADMBASE__lapse_prolongation_type
#  define CCTK_PARAMETER__ADMBASE__lapse_prolongation_type RESTRICTED_ADMBASE_STRUCT.lapse_prolongation_type
#endif
#ifndef CCTK_PARAMETER__ADMBASE__metric_prolongation_type
#  define CCTK_PARAMETER__ADMBASE__metric_prolongation_type RESTRICTED_ADMBASE_STRUCT.metric_prolongation_type
#endif
#ifndef CCTK_PARAMETER__ADMBASE__metric_type
#  define CCTK_PARAMETER__ADMBASE__metric_type RESTRICTED_ADMBASE_STRUCT.metric_type
#endif
#ifndef CCTK_PARAMETER__ADMBASE__shift_evolution_method
#  define CCTK_PARAMETER__ADMBASE__shift_evolution_method RESTRICTED_ADMBASE_STRUCT.shift_evolution_method
#endif
#ifndef CCTK_PARAMETER__ADMBASE__shift_prolongation_type
#  define CCTK_PARAMETER__ADMBASE__shift_prolongation_type RESTRICTED_ADMBASE_STRUCT.shift_prolongation_type
#endif
#ifndef CCTK_PARAMETER__ADMBASE__lapse_timelevels
#  define CCTK_PARAMETER__ADMBASE__lapse_timelevels RESTRICTED_ADMBASE_STRUCT.lapse_timelevels
#endif
#ifndef CCTK_PARAMETER__ADMBASE__metric_timelevels
#  define CCTK_PARAMETER__ADMBASE__metric_timelevels RESTRICTED_ADMBASE_STRUCT.metric_timelevels
#endif
#ifndef CCTK_PARAMETER__ADMBASE__shift_timelevels
#  define CCTK_PARAMETER__ADMBASE__shift_timelevels RESTRICTED_ADMBASE_STRUCT.shift_timelevels
#endif

