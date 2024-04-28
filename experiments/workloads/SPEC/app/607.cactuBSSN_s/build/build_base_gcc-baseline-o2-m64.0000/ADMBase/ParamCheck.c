 /*@@
   @file      ParamCheck.c
   @date      Thu Apr 25 19:02:51 2002
   @author    Tom Goodale
   @desc 
   Parameter checking stuff for ADMBase
   @enddesc
   @version $Header$
 @@*/

#ifdef SPEC 
#include "ADMBase/cctk.h" 
#define THORN_DIR "ADMBase" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 

#ifdef SPEC 
#include "ADMBase/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ADMBase/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusEinstein_ADMBase_ParamCheck_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/


/********************************************************************
 ********************* Other Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 *********************     Local Data   *****************************
 ********************************************************************/

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

 /*@@
   @routine    ADMBase_ParamCheck
   @date       Thu Apr 25 19:04:06 2002
   @author     Tom Goodale
   @desc 
   Scheduled routine to detect invalid parameter settings.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
void ADMBase_ParamCheck(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS

  if(CCTK_EQUALS(initial_data, "Cartesian Minkowski") &&
     ! CCTK_EQUALS(metric_type, "physical"))
  {
    CCTK_PARAMWARN("ADMBase asked to setup Cartesian Minkowski initial data, but metric_type is not \"physical\".  Perhaps you want \"Conformal Minkowski\" data, provided by IDSimple?");
  }
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
