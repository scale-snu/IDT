#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_INT,stress_energy_at_RHS,)&&\
CCTK_DECLARE(CCTK_INT,support_old_CalcTmunu_mechanism,)&&\
COMMON /TmunuBaserest/stress_energy_at_RHS, support_old_CalcTmunu_mechanism&&\
CCTK_DECLARE(CCTK_STRING,prolongation_type,)&&\
CCTK_DECLARE(CCTK_INT,stress_energy_storage,)&&\
CCTK_DECLARE(CCTK_INT,timelevels,)&&\
COMMON /TmunuBasepriv/prolongation_type, stress_energy_storage, timelevels&&\

