 /*@@
   @file      SandR.c
   @date      Sun May 26 03:35:58 2002
   @author    Ian Hawke
   @desc 
   Restores the Save and Restore variables to their original positions.
   @enddesc 
   @version   $Header$
 @@*/

#include <string.h>

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

CCTK_FILEVERSION(CactusBase_MoL_SandR_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_RestoreSandR(CCTK_ARGUMENTS);

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
   @routine    MoL_RestoreSandR
   @date       Sun May 26 03:39:02 2002
   @author     Ian Hawke
   @desc 
   Save and Restore variables are those that the physics thorn may 
   need to know to calculate the RHS, but which may be evolved by
   something other than MoL. In order to get the timelevels correct,
   the previous timelevel is copied to the current before the MoL step.
   As we do not know whether the variable will be evolved before or
   after MoL we must save the data that was in the current timelevel, 
   and then restore it at the end of the MoL timestep. This routine
   restores the variables.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_RestoreSandR(CCTK_ARGUMENTS) 
{
  
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  CCTK_INT /*  index, */ var;
  CCTK_INT totalsize, arraydim;
  CCTK_REAL *SandRDataArray;
  CCTK_REAL *ScratchVar;

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX *SandRComplexDataArray;
  CCTK_COMPLEX *ComplexScratchVar;

#endif
  
  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

  for (var = 0; var < MoLNumSandRVariables; var++) 
  {
    
    SandRDataArray = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                   SandRVariableIndex[var]);

    ScratchVar = CCTK_VarDataPtrI(cctkGH, 0, 
                                  CCTK_FirstVarIndex("MOL::SANDRSCRATCHSPACE")
                                  + var);

#ifdef MOLDEBUG    
    printf("Restore:Variable %s, first entry %g, scratch %g\n",
           CCTK_VarName(SandRVariableIndex[var]), SandRDataArray[0], 
           ScratchVar[0]);
#endif

    memcpy(SandRDataArray, ScratchVar, 
           totalsize * sizeof(CCTK_REAL));
  }

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  for (var = 0; var < MoLNumSandRComplexVariables; var++) 
  {
    
    SandRComplexDataArray = (CCTK_COMPLEX *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                             SandRComplexVariableIndex[var]);
    
    ComplexScratchVar = CCTK_VarDataPtrI(cctkGH, 0, 
                                         CCTK_FirstVarIndex("MOL::COMPLEXSANDRSCRATCHSPACE")
                                         + var);

    memcpy(SandRComplexDataArray, ComplexScratchVar, 
           totalsize * sizeof(CCTK_COMPLEX));
  }

#endif
  
  return;
  
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
