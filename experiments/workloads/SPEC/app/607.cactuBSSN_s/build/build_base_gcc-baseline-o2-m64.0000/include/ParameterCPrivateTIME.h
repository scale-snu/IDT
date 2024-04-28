#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL courant_fac;
  CCTK_REAL dtfac;
  CCTK_REAL timestep;
  CCTK_INT timestep_outevery;
  CCTK_INT verbose;
} PRIVATE_TIME_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_TIME_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, courant_fac, CCTK_PARAMETER__TIME__courant_fac); \
  CCTK_DECLARE_INIT (CCTK_REAL const, dtfac, CCTK_PARAMETER__TIME__dtfac); \
  CCTK_DECLARE_INIT (CCTK_REAL const, timestep, CCTK_PARAMETER__TIME__timestep); \
  CCTK_DECLARE_INIT (CCTK_INT const, timestep_outevery, CCTK_PARAMETER__TIME__timestep_outevery); \
  CCTK_DECLARE_INIT (CCTK_INT const, verbose, CCTK_PARAMETER__TIME__verbose); \


#ifndef CCTK_PARAMETER__TIME__courant_fac
#  define CCTK_PARAMETER__TIME__courant_fac PRIVATE_TIME_STRUCT.courant_fac
#endif
#ifndef CCTK_PARAMETER__TIME__dtfac
#  define CCTK_PARAMETER__TIME__dtfac PRIVATE_TIME_STRUCT.dtfac
#endif
#ifndef CCTK_PARAMETER__TIME__timestep
#  define CCTK_PARAMETER__TIME__timestep PRIVATE_TIME_STRUCT.timestep
#endif
#ifndef CCTK_PARAMETER__TIME__timestep_outevery
#  define CCTK_PARAMETER__TIME__timestep_outevery PRIVATE_TIME_STRUCT.timestep_outevery
#endif
#ifndef CCTK_PARAMETER__TIME__verbose
#  define CCTK_PARAMETER__TIME__verbose PRIVATE_TIME_STRUCT.verbose
#endif

