#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT stress_energy_at_RHS;
  CCTK_INT support_old_CalcTmunu_mechanism;
} RESTRICTED_TMUNUBASE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_TMUNUBASE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, stress_energy_at_RHS, CCTK_PARAMETER__TMUNUBASE__stress_energy_at_RHS); \
  CCTK_DECLARE_INIT (CCTK_INT const, support_old_CalcTmunu_mechanism, CCTK_PARAMETER__TMUNUBASE__support_old_CalcTmunu_mechanism); \


#ifndef CCTK_PARAMETER__TMUNUBASE__stress_energy_at_RHS
#  define CCTK_PARAMETER__TMUNUBASE__stress_energy_at_RHS RESTRICTED_TMUNUBASE_STRUCT.stress_energy_at_RHS
#endif
#ifndef CCTK_PARAMETER__TMUNUBASE__support_old_CalcTmunu_mechanism
#  define CCTK_PARAMETER__TMUNUBASE__support_old_CalcTmunu_mechanism RESTRICTED_TMUNUBASE_STRUCT.support_old_CalcTmunu_mechanism
#endif

