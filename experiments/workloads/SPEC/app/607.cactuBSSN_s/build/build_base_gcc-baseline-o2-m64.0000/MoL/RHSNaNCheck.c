 /*@@
   @file      RHSNaNCheck.c
   @date      Mon May 20 09:52:33 2002
   @author    Ian Hawke
   @desc 
   Check the RHS GFs for NaNs.
   @enddesc 
   @version   $Header$
 @@*/

#include <stdlib.h>

#ifdef SPEC 
#include "MoL/cctk.h" 
#define THORN_DIR "MoL" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "MoL/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "MoL/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

#include "ExternalVariables.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_MoL_RHSNaNCheck_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_NaNCheck(CCTK_ARGUMENTS);

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
   @routine    MoL_NaNCheck
   @date       Mon May 20 09:54:39 2002
   @author     Ian Hawke
   @desc 
   Check the RHS GFs for NaNs.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_NaNCheck(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS

  CCTK_INT var;
  CCTK_INT num_nans;

  const char *check_for = "both";
  const char *action_if_found = "just warn";
  
  num_nans = 0;

  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    char * const varname = CCTK_FullName(RHSVariableIndex[var]);
    num_nans += CheckVarsForNaN(cctkGH,
                                -1,
                                varname,
                                check_for,
                                action_if_found);
    free(varname);
  }

  if (num_nans)
  {
    CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,
               "NaNs were found on iteration %d inside MoL",
               (int)(MoL_Intermediate_Steps - *MoL_Intermediate_Step + 1));
  }
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
