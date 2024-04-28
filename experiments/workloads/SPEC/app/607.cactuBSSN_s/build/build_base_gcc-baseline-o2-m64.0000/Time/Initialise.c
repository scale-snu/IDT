 /*@@
   @file      Initialise.c
   @date      May 12 2001
   @author    Gabrielle Allen
   @desc 
   Initialise grid variables
   @enddesc 
 @@*/

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


void Time_Initialise(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS

  *courant_wave_speed = 0;  
  *courant_min_time = 0;
  *courant_dt = 0;
}
