 /*@@
   @file      ICN.c
   @date      Sun May 26 04:29:07 2002
   @author    Ian Hawke
   @desc 
   This implements the more efficient Iterative Crank Nicholson integrator.
   This follows the implementation of ICN in all AEI codes and is 
   equivalent to (but hopefully more efficient than) the generic ICN
   integrator also implemented by MoL.
   @enddesc 
   @version   $Header$
 @@*/

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

CCTK_FILEVERSION(CactusBase_MoL_ICN_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_ICNAdd(CCTK_ARGUMENTS);
void MoL_ICNAverage(CCTK_ARGUMENTS);

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
   @routine    MoL_ICNAdd
   @date       Sun May 26 04:17:23 2002
   @author     Ian Hawke
   @desc 
   Performs Iterative Crank Nicholson time integration. The number of
   steps is arbitrary.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_ICNAdd(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  cGroupDynamicData arraydata;
  CCTK_INT groupindex, ierr;
  CCTK_INT arraytotalsize, arraydim;

  CCTK_INT index, var;
  CCTK_INT totalsize;
  CCTK_REAL *OldVar;
  CCTK_REAL *UpdateVar;
  CCTK_REAL *RHSVar;
  
  /* FIXME */

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX *OldComplexVar;
  CCTK_COMPLEX *UpdateComplexVar;
  CCTK_COMPLEX *RHSComplexVar;
  CCTK_COMPLEX Complex_Delta_Time = CCTK_Cmplx(CCTK_DELTA_TIME, 0);

#endif

#ifdef MOLDEBUG
  printf("Inside ICN.\nProcessor %d.\nStep %d.\nRefinement %d.\n"
         "Timestep %g.\nSpacestep %g.\nTime %g\n",
         CCTK_MyProc(cctkGH),
         MoL_Intermediate_Steps - *MoL_Intermediate_Step + 1,
         *cctk_levfac,
         CCTK_DELTA_TIME,
         CCTK_DELTA_SPACE(0),
         cctk_time);
#endif  

  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

#ifdef MOLDEBUG
  printf("MoL: the ICN routine says dt = %f.\n", CCTK_DELTA_TIME);
#endif
  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    OldVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 1,
                                          EvolvedVariableIndex[var]);
    UpdateVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0,
                                             EvolvedVariableIndex[var]);
    RHSVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                          RHSVariableIndex[var]);
    
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < totalsize; index++)
    {
      UpdateVar[index] = OldVar[index] + CCTK_DELTA_TIME * RHSVar[index];
    }
  }
  
  for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
  {
    OldVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 1,
                                             EvolvedArrayVariableIndex[var]);
    UpdateVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0,
                                             EvolvedArrayVariableIndex[var]);
    RHSVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                          RHSArrayVariableIndex[var]);
    
    groupindex = CCTK_GroupIndexFromVarI(EvolvedArrayVariableIndex[var]);
    ierr = CCTK_GroupDynamicData(cctkGH, groupindex,
                                 &arraydata);
    if (ierr)
    {
      CCTK_VWarn(0, __LINE__, __FILE__, CCTK_THORNSTRING, 
                 "The driver does not return group information "
                 "for group '%s'.", 
                 CCTK_GroupName(groupindex));
    }
    arraytotalsize = 1;
    for (arraydim = 0; arraydim < arraydata.dim; arraydim++)
    {
      arraytotalsize *= arraydata.ash[arraydim];
    }

/*     CCTK_VWarn(1, __LINE__, __FILE__, CCTK_THORNSTRING,  */
/*                  "This proc array total size is %d.",  */
/*                  arraytotalsize); */

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < arraytotalsize; index++)
    {
      UpdateVar[index] = OldVar[index] + CCTK_DELTA_TIME * RHSVar[index];
    }
  }

  /* FIXME */

#ifdef MOLDOESCOMPLEX
  
  for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
  {
    OldComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 1,
                                                    EvolvedComplexVariableIndex[var]);
    UpdateComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0,
                                                       EvolvedComplexVariableIndex[var]);
    RHSComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                    RHSComplexVariableIndex[var]);
    
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < totalsize; index++)
    {
      UpdateComplexVar[index] = CCTK_CmplxAdd(OldComplexVar[index],
                                              CCTK_CmplxMul(Complex_Delta_Time,
                                                            RHSComplexVar[index]));
    }
  }

#endif
  
  return;

}

 /*@@
   @routine    MoL_ICNAverage
   @date       Fri Jul 18 14:02:00 2003
   @author     Ian Hawke, Erik Schnetter
   @desc 
   Averages between the current and the previous time level.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_ICNAverage(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  cGroupDynamicData arraydata;
  CCTK_INT groupindex, ierr;
  CCTK_INT arraytotalsize, arraydim;

  CCTK_INT index, var;
  CCTK_INT totalsize;
  CCTK_REAL *OldVar;
  CCTK_REAL *UpdateVar;
  
  /* FIXME */

  CCTK_REAL theta;

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX *OldComplexVar;
  CCTK_COMPLEX *UpdateComplexVar;
  CCTK_COMPLEX *RHSComplexVar;
  CCTK_COMPLEX Complex_Half = CCTK_Cmplx(0.5, 0);

#endif

   
  theta = ICN_avg_theta;
  if (ICN_avg_swapped && (*MoL_Intermediate_Step%2))
  {
    theta = 1.0 - theta;
  }

#ifdef MOLDEBUG
  printf("Inside ICN.\nProcessor %d.\nStep %d.\nRefinement %d.\n"
         "Timestep %g.\nSpacestep %g.\nTime %g Theta %g\n",
         CCTK_MyProc(cctkGH),
         MoL_Intermediate_Steps - *MoL_Intermediate_Step + 1,
         *cctk_levfac,
         CCTK_DELTA_TIME,
         CCTK_DELTA_SPACE(0),
         cctk_time,
         theta);
#endif  

  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

#ifdef MOLDEBUG
  printf("MoL: the ICN routine says dt = %f.\n", CCTK_DELTA_TIME);
#endif
  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    OldVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 1,
                                          EvolvedVariableIndex[var]);
    UpdateVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0,
                                             EvolvedVariableIndex[var]);
    
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < totalsize; index++)
    {
/*       UpdateVar[index] = 0.5 * (UpdateVar[index] + OldVar[index]); */
      UpdateVar[index] = (1.0 - theta) * UpdateVar[index] + 
                                theta  * OldVar[index];
    }
  }
  
  for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
  {
    OldVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 1,
                                             EvolvedArrayVariableIndex[var]);
    UpdateVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0,
                                             EvolvedArrayVariableIndex[var]);
    
    groupindex = CCTK_GroupIndexFromVarI(EvolvedArrayVariableIndex[var]);
    ierr = CCTK_GroupDynamicData(cctkGH, groupindex,
                                 &arraydata);
    if (ierr)
    {
      CCTK_VWarn(0, __LINE__, __FILE__, CCTK_THORNSTRING, 
                 "The driver does not return group information "
                 "for group '%s'.", 
                 CCTK_GroupName(groupindex));
    }
    arraytotalsize = 1;
    for (arraydim = 0; arraydim < arraydata.dim; arraydim++)
    {
      arraytotalsize *= arraydata.ash[arraydim];
    }

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < arraytotalsize; index++)
    {
/*       UpdateVar[index] = 0.5 * (UpdateVar[index] + OldVar[index]); */
      UpdateVar[index] = (1.0 - theta) * UpdateVar[index] + 
                                theta  * OldVar[index];
    }
  }

  /* FIXME */

#ifdef MOLDOESCOMPLEX
  
  for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
  {
    OldComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 1,
                                                    EvolvedComplexVariableIndex[var]);
    UpdateComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0,
                                                       EvolvedComplexVariableIndex[var]);
    RHSComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                    RHSComplexVariableIndex[var]);
    
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < totalsize; index++)
    {
      UpdateComplexVar[index] = CCTK_CmplxMul(Complex_Half, CCTK_CmplxAdd(UpdateComplexVar[index], OldComplexVar[index]));
    }
  }

#endif
  
  return;

}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
