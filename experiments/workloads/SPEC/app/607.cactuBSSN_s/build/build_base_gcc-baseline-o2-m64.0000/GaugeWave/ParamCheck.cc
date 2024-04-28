#ifdef SPEC 
#include "GaugeWave/cctk.h" 
#define THORN_DIR "GaugeWave" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "GaugeWave/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "GaugeWave/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

extern "C" void GaugeWave_ParamCheck(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (CCTK_MyProc(cctkGH) == 0)
  {
    
    if (((CCTK_EQUALS(initial_data,"GaugeWave")) || (CCTK_EQUALS(initial_lapse,"GaugeWave")) || (CCTK_EQUALS(initial_shift,"GaugeWave")) || (CCTK_EQUALS(initial_dtlapse,"GaugeWave")) || (CCTK_EQUALS(initial_dtshift,"GaugeWave"))) && ((!CCTK_EQUALS(initial_data,"GaugeWave")) || (!CCTK_EQUALS(initial_lapse,"GaugeWave")) || (!CCTK_EQUALS(initial_shift,"GaugeWave")) || (!CCTK_EQUALS(initial_dtlapse,"GaugeWave")) || (!CCTK_EQUALS(initial_dtshift,"GaugeWave"))))
    {
      CCTK_WARN(CCTK_WARN_ABORT, "If one of the parameters ADMBase::initial_data, ADMBase::initial_lapse, ADMBase::initial_shift, ADMBase::initial_dtlapse, and ADMBase::initial_dtshift are set to \"GaugeWave\", then all must be set to this value");
    }
  }
}
