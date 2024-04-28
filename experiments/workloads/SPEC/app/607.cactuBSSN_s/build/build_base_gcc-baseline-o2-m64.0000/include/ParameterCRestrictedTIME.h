#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * timestep_method;
  CCTK_INT timestep_outonly;
} RESTRICTED_TIME_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_TIME_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, timestep_method, CCTK_PARAMETER__TIME__timestep_method); \
  CCTK_DECLARE_INIT (CCTK_INT const, timestep_outonly, CCTK_PARAMETER__TIME__timestep_outonly); \


#ifndef CCTK_PARAMETER__TIME__timestep_method
#  define CCTK_PARAMETER__TIME__timestep_method RESTRICTED_TIME_STRUCT.timestep_method
#endif
#ifndef CCTK_PARAMETER__TIME__timestep_outonly
#  define CCTK_PARAMETER__TIME__timestep_outonly RESTRICTED_TIME_STRUCT.timestep_outonly
#endif

