#ifdef SPEC 
#include "ML_BSSN_Helper/cctk.h" 
#define THORN_DIR "ML_BSSN_Helper" 
#else 
#include <cctk.h>
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ML_BSSN_Helper/cctk_Arguments.h" 
#else 
#include <cctk_Arguments.h>
#endif /* END SPEC */ 

static void
extrap (cGH const * restrict cctkGH,
        CCTK_REAL * restrict var);

void
ML_BSSN_ExtrapolateGammas (CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  
  extrap (cctkGH, Xt1);
  extrap (cctkGH, Xt2);
  extrap (cctkGH, Xt3);
  
  extrap (cctkGH, A);
  
  extrap (cctkGH, B1);
  extrap (cctkGH, B2);
  extrap (cctkGH, B3);
}

static void
extrap (cGH const * restrict const cctkGH,
        CCTK_REAL * restrict const var)
{
  ExtrapolateGammas (cctkGH, var);
}
