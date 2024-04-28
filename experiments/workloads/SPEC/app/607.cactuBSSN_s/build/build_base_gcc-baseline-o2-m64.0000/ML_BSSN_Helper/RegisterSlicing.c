#ifdef SPEC 
#include "ML_BSSN_Helper/cctk.h" 
#define THORN_DIR "ML_BSSN_Helper" 
#else 
#include <cctk.h>
#endif /* END SPEC */ 
#include <Slicing.h>

int
ML_BSSN_RegisterSlicing (void)
{
  Einstein_RegisterSlicing ("ML_BSSN");
  return 0;
}
