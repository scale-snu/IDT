#include "ML_BSSN/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int ML_BSSN_Startup (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_RegisterSymmetries (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_Minkowski (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_convertFromADMBase (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_InitGamma (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_convertFromADMBaseGamma (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_RHS (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_Dissipation (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_Advect (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_InitRHS (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_RHSStaticBoundary (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_enforce (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_boundary (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_convertToADMBase (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_convertToADMBaseDtLapseShift (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_convertToADMBaseDtLapseShiftBoundary (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_convertToADMBaseFakeDtLapseShift (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_constraints (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_constraints_SelectBCs (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_SelectBoundConds (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_CheckBoundaries (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_RegisterVars (CCTK_ARGUMENTS);
#endif /* CCODE */

