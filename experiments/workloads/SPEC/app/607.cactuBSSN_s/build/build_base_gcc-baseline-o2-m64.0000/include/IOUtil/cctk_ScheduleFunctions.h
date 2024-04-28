#include "IOUtil/cctk_Arguments.h"

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
int IOUtil_Startup (void);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void IOUtil_RecoverGH (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void IOUtil_RecoverIDFromDatafiles (CCTK_ARGUMENTS);
#endif /* CCODE */

#ifdef CCODE
#ifdef __cplusplus
extern "C"
#endif
void IOUtil_UpdateParFile (CCTK_ARGUMENTS);
#endif /* CCODE */

