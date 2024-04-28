/*  File produced by Kranc */

#ifdef SPEC 
#include "ML_BSSN/cctk.h" 
#define THORN_DIR "ML_BSSN" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 

extern "C" int ML_BSSN_Startup(void)
{
  const char* banner CCTK_ATTRIBUTE_UNUSED = "ML_BSSN";
  CCTK_RegisterBanner(banner);
  return 0;
}
