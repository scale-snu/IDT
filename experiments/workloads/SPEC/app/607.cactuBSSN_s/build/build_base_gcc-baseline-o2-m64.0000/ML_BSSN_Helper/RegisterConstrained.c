#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
register_constrained (char const * restrict const gn);

void
ML_BSSN_RegisterConstrained (CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  
  register_constrained ("ADMBase::metric");
  register_constrained ("ADMBase::curv");
  register_constrained ("ADMBase::lapse");
  register_constrained ("ADMBase::shift");
  register_constrained ("ADMBase::dtlapse");
  register_constrained ("ADMBase::dtshift");
}

static void
register_constrained (char const * restrict const gn)
{
  assert (gn);
  
  int const gi = CCTK_GroupIndex (gn);
  int const ierr = MoLRegisterConstrainedGroup (gi);
  assert (! ierr);
}
