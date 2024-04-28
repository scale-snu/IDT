#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * prolongation_type;
  CCTK_INT stress_energy_storage;
  CCTK_INT timelevels;
} PRIVATE_TMUNUBASE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_TMUNUBASE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, prolongation_type, CCTK_PARAMETER__TMUNUBASE__prolongation_type); \
  CCTK_DECLARE_INIT (CCTK_INT const, stress_energy_storage, CCTK_PARAMETER__TMUNUBASE__stress_energy_storage); \
  CCTK_DECLARE_INIT (CCTK_INT const, timelevels, CCTK_PARAMETER__TMUNUBASE__timelevels); \


#ifndef CCTK_PARAMETER__TMUNUBASE__prolongation_type
#  define CCTK_PARAMETER__TMUNUBASE__prolongation_type PRIVATE_TMUNUBASE_STRUCT.prolongation_type
#endif
#ifndef CCTK_PARAMETER__TMUNUBASE__stress_energy_storage
#  define CCTK_PARAMETER__TMUNUBASE__stress_energy_storage PRIVATE_TMUNUBASE_STRUCT.stress_energy_storage
#endif
#ifndef CCTK_PARAMETER__TMUNUBASE__timelevels
#  define CCTK_PARAMETER__TMUNUBASE__timelevels PRIVATE_TMUNUBASE_STRUCT.timelevels
#endif

