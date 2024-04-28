 /*@@
   @file      Given.c
   @date      September 4 1999
   @author    Gabrielle Allen
   @desc 
   Standard specification of timestep
   @enddesc 
 @@*/

#include <stdlib.h>

#ifdef SPEC 
#include "Time/cctk.h" 
#define THORN_DIR "Time" 
#else 
#include <cctk.h>
#endif /* END SPEC */ 
#ifdef SPEC 
#include "Time/cctk_Arguments.h" 
#else 
#include <cctk_Arguments.h>
#endif /* END SPEC */ 
#ifdef SPEC 
#include "Time/cctk_Parameters.h" 
#else 
#include <cctk_Parameters.h>
#endif /* END SPEC */ 


void Time_Given(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_PARAMETERS
  DECLARE_CCTK_ARGUMENTS

  cctkGH->cctk_delta_time = timestep;  
}
