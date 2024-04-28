#include <assert.h>

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
#ifdef SPEC 
#include "ML_BSSN_Helper/cctk_Parameters.h" 
#else 
#include <cctk_Parameters.h>
#endif /* END SPEC */ 

static void
select_bcs (cGH const * restrict cctkGH, char const * restrict gn);

void
ML_BSSN_SelectBCsADMBase (CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  
  select_bcs (cctkGH, "ADMBase::metric");
  select_bcs (cctkGH, "ADMBase::curv");
  select_bcs (cctkGH, "ADMBase::lapse");
  select_bcs (cctkGH, "ADMBase::shift");
  select_bcs (cctkGH, "ADMBase::dtlapse");
  select_bcs (cctkGH, "ADMBase::dtshift");
}

static void
select_bcs (cGH const * restrict const cctkGH, char const * restrict const gn)
{
  DECLARE_CCTK_PARAMETERS
  
  int const ierr = Boundary_SelectGroupForBC
    (cctkGH, CCTK_ALL_FACES, boundary_width, -1, gn, "none");
  assert (! ierr);
}
