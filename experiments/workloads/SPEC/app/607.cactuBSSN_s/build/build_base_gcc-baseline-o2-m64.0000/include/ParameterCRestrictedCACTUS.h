#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL cctk_final_time;
  CCTK_REAL cctk_initial_time;
  CCTK_REAL max_runtime;
  const char * terminate;
  CCTK_INT cctk_itlast;
  CCTK_INT terminate_next;
} RESTRICTED_CACTUS_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_CACTUS_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, cctk_final_time, CCTK_PARAMETER__CACTUS__cctk_final_time); \
  CCTK_DECLARE_INIT (CCTK_REAL const, cctk_initial_time, CCTK_PARAMETER__CACTUS__cctk_initial_time); \
  CCTK_DECLARE_INIT (CCTK_REAL const, max_runtime, CCTK_PARAMETER__CACTUS__max_runtime); \
  CCTK_DECLARE_INIT (const char * const, terminate, CCTK_PARAMETER__CACTUS__terminate); \
  CCTK_DECLARE_INIT (CCTK_INT const, cctk_itlast, CCTK_PARAMETER__CACTUS__cctk_itlast); \
  CCTK_DECLARE_INIT (CCTK_INT const, terminate_next, CCTK_PARAMETER__CACTUS__terminate_next); \


#ifndef CCTK_PARAMETER__CACTUS__cctk_final_time
#  define CCTK_PARAMETER__CACTUS__cctk_final_time RESTRICTED_CACTUS_STRUCT.cctk_final_time
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_initial_time
#  define CCTK_PARAMETER__CACTUS__cctk_initial_time RESTRICTED_CACTUS_STRUCT.cctk_initial_time
#endif
#ifndef CCTK_PARAMETER__CACTUS__max_runtime
#  define CCTK_PARAMETER__CACTUS__max_runtime RESTRICTED_CACTUS_STRUCT.max_runtime
#endif
#ifndef CCTK_PARAMETER__CACTUS__terminate
#  define CCTK_PARAMETER__CACTUS__terminate RESTRICTED_CACTUS_STRUCT.terminate
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_itlast
#  define CCTK_PARAMETER__CACTUS__cctk_itlast RESTRICTED_CACTUS_STRUCT.cctk_itlast
#endif
#ifndef CCTK_PARAMETER__CACTUS__terminate_next
#  define CCTK_PARAMETER__CACTUS__terminate_next RESTRICTED_CACTUS_STRUCT.terminate_next
#endif

