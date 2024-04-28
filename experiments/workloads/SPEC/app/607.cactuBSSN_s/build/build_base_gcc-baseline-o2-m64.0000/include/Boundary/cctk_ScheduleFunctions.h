#include "Boundary/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void Boundary_Check (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void Boundary_RegisterBCs (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void Boundary_ApplyPhysicalBCs (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void Boundary_ClearSelection (CCTK_ARGUMENTS);
#endif /* CCODE */

