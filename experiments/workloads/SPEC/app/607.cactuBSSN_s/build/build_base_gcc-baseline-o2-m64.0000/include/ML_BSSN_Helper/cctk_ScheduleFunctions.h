#include "ML_BSSN_Helper/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int ML_BSSN_RegisterSlicing (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int ML_BSSN_SetGroupTags (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_RegisterConstrained (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_ExtrapolateGammas (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_NewRad (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void ML_BSSN_SelectBCsADMBase (CCTK_ARGUMENTS);
#endif /* CCODE */

