#include "IOBasic/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int IOBasic_Startup (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void IOBasic_Init (CCTK_ARGUMENTS);
#endif /* CCODE */

