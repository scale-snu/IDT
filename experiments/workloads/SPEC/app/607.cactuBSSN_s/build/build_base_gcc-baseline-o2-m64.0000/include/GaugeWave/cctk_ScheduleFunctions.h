#include "GaugeWave/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int GaugeWave_Startup (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_RegisterSymmetries (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_initial (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_always (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_SelectBoundConds (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_CheckBoundaries (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_RegisterVars (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void GaugeWave_ParamCheck (CCTK_ARGUMENTS);
#endif /* CCODE */

