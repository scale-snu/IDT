#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT ML_BSSN_MaxNumConstrainedVars;
} PRIVATE_ML_BSSN_HELPER_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_ML_BSSN_HELPER_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, ML_BSSN_MaxNumConstrainedVars, CCTK_PARAMETER__ML_BSSN_HELPER__ML_BSSN_MaxNumConstrainedVars); \


#ifndef CCTK_PARAMETER__ML_BSSN_HELPER__ML_BSSN_MaxNumConstrainedVars
#  define CCTK_PARAMETER__ML_BSSN_HELPER__ML_BSSN_MaxNumConstrainedVars PRIVATE_ML_BSSN_HELPER_STRUCT.ML_BSSN_MaxNumConstrainedVars
#endif

