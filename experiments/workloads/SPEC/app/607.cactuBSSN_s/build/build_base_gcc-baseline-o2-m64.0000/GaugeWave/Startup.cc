/*  File produced by Kranc */

#ifdef SPEC 
#include "GaugeWave/cctk.h" 
#define THORN_DIR "GaugeWave" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 

extern "C" int GaugeWave_Startup(void)
{
  const char* banner CCTK_ATTRIBUTE_UNUSED = "GaugeWave";
  CCTK_RegisterBanner(banner);
  return 0;
}
