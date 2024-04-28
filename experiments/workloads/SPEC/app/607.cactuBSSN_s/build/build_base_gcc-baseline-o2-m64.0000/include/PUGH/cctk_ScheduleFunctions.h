#include "PUGH/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int PUGH_Startup (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int PUGH_RegisterPUGHP2LRoutines (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int PUGH_RegisterPUGHTopologyRoutines (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void PUGH_Report (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void PUGH_PrintTimingInfo (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void PUGH_PrintFinalStorageReport (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void PUGH_PrintStorageReport (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void PUGH_Terminate (CCTK_ARGUMENTS);
#endif /* CCODE */

