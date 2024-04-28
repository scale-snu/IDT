 /*@@
   @file      GenericRK.c
   @date      Sun May 26 03:47:15 2002
   @author    Ian Hawke
   @desc 
   This routine performs a generic Runge-Kutta type integration
   given the set of coefficients defined in the RKAlphaCoefficients
   and RKBetaCoefficients arrays. See the article by Shu referenced
   in the documentation for more details.
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

#include <stdio.h>

#include "ExternalVariables.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_MoL_GenericRK_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

CCTK_INT AlphaIndex(CCTK_INT Step_Number, CCTK_INT Scratch_Level);

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_GenericRKAdd(CCTK_ARGUMENTS);

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
   @routine    MoL_GenericRKAdd
   @date       Sun May 26 03:50:44 2002
   @author     Ian Hawke
   @desc 
   Performs a single step of a generic Runge-Kutta type time
   integration.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_GenericRKAdd(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
    
  CCTK_INT arraytotalsize, arraydim;

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX Complex_alpha, Complex_beta, Complex_Delta_Time;
  CCTK_COMPLEX * restrict UpdateComplexVar;
  CCTK_COMPLEX const * restrict RHSComplexVar;
  CCTK_COMPLEX * restrict ScratchComplexVar;

#endif

  static CCTK_INT scratchspace_firstindex = -99;
  CCTK_INT index, var, scratchstep, alphaindex, scratchindex;
  CCTK_INT totalsize;
  CCTK_REAL alpha, beta;
  CCTK_REAL * restrict UpdateVar;
  CCTK_REAL const * restrict RHSVar;
  CCTK_REAL * restrict ScratchVar;

  CCTK_INT arrayscratchlocation;

  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

  if (scratchspace_firstindex == -99)
  {
    scratchspace_firstindex = CCTK_FirstVarIndex("MOL::SCRATCHSPACE");
  }

  beta = RKBetaCoefficients[MoL_Intermediate_Steps - 
                           (*MoL_Intermediate_Step)];

  /* FIXME */

#ifdef MOLDOESCOMPLEX
  
  Complex_Delta_Time = CCTK_Cmplx((*Original_Delta_Time) / cctkGH->cctk_timefac, 0);
  Complex_beta = CCTK_Cmplx(beta, 0);

#endif

  /* Real GFs */

  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    
    UpdateVar = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                              EvolvedVariableIndex[var]);
    RHSVar = (CCTK_REAL const *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                 RHSVariableIndex[var]);
/* #define MOLDEBUG 1 */
#ifdef MOLDEBUG
    printf("In generic RK. Variable %d (%s). RHS %d (%s). beta %g.\n",
           EvolvedVariableIndex[var],
           CCTK_VarName(EvolvedVariableIndex[var]),
           RHSVariableIndex[var],
           CCTK_VarName(RHSVariableIndex[var]),
           beta);
#endif

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < totalsize; index++)
    {
      UpdateVar[index] = (*Original_Delta_Time) / cctkGH->cctk_timefac * beta * RHSVar[index];
#ifdef MOLDEBUG
      if (CCTK_EQUALS(verbose,"extreme"))
      {
        printf("Variable: %d. Index: %d. dt: %f. beta %f. RHS: %f. q: %f.\n",
               var, index, (*Original_Delta_Time) / cctkGH->cctk_timefac, beta, RHSVar[index], 
               UpdateVar[index]);
      }
#endif
    }
    
    for (scratchstep = 0; 
         scratchstep < MoL_Intermediate_Steps - (*MoL_Intermediate_Step) + 1;
         scratchstep++)
    {

      alphaindex = AlphaIndex(*MoL_Intermediate_Step, scratchstep);
      scratchindex = scratchstep - 1;

      alpha = RKAlphaCoefficients[alphaindex];
#ifdef MOLDEBUG
      printf("In generic RK. Variable %d (%s). RHS %d (%s). step %d. alpha %g.\n",
             EvolvedVariableIndex[var],
             CCTK_VarName(EvolvedVariableIndex[var]),
             RHSVariableIndex[var],
             CCTK_VarName(RHSVariableIndex[var]),
             scratchstep,
             alpha);
#endif

      if (scratchstep) 
      {
        /*
          The following would work if all drivers considered
          a vector group to have contiguous storage.
        */
        /*
          ScratchVar = &ScratchSpace[(var * MoL_Num_Scratch_Levels + 
                                   scratchindex) * totalsize];
        */
        ScratchVar = CCTK_VarDataPtrI(cctkGH, 0, 
                                      scratchspace_firstindex
                                      + var 
                                      + MoL_Num_Evolved_Vars * scratchindex);
#ifdef MOLDEBUG
        if (CCTK_EQUALS(verbose,"extreme"))
        {
          printf("Reading from scratch space, initial address %ld index %d\n", 
                 ScratchVar, (var * MoL_Num_Scratch_Levels + 
                              scratchindex) * totalsize);
        }
#endif
      }
      else
      {
        ScratchVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 1, 
                                                  EvolvedVariableIndex[var]);
      }
      
      if ( (alpha > MoL_Tiny)||(alpha < -MoL_Tiny) )
      {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
        for (index = 0; index < totalsize; index++)
        {
          UpdateVar[index] += alpha * ScratchVar[index];
#ifdef MOLDEBUG
          if (CCTK_EQUALS(verbose,"extreme"))
          {
            printf("Variable: %d. Index: %d. step: %d. "
                   "alpha: %f. Scratch: %f. q: %f.\n",
                   var, index, (*MoL_Intermediate_Step), alpha, 
                   ScratchVar[index], UpdateVar[index]);
          }
#endif
        }
      }
      
    }
    
  }

  if (*MoL_Intermediate_Step > 1)
  {
    for (var = 0; var < MoLNumEvolvedVariables; var++)
    {
      UpdateVar = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                EvolvedVariableIndex[var]);
      /*
        The following would work if all drivers considered
        a vector group to have contiguous storage.
      */
      /*
        ScratchVar = &ScratchSpace[(var * MoL_Num_Scratch_Levels + 
                                 MoL_Intermediate_Steps -
                                 (*MoL_Intermediate_Step)) * totalsize];
      */
      ScratchVar = CCTK_VarDataPtrI(cctkGH, 0, 
                                    scratchspace_firstindex
                                    + var 
                                    + MoL_Num_Evolved_Vars * (MoL_Intermediate_Steps - (*MoL_Intermediate_Step)));
#ifdef MOLDEBUG
      printf("Writing to scratch space, initial address %ld, "
             "index %d, totalsize %d \n", 
             ScratchVar, (var * MoL_Num_Scratch_Levels + 
                          MoL_Intermediate_Steps -
                          (*MoL_Intermediate_Step)) * totalsize, totalsize);
#endif
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (index = 0; index < totalsize; index++)
      {
        ScratchVar[index] = UpdateVar[index];
#ifdef MOLDEBUG
        if (CCTK_EQUALS(verbose,"extreme"))
        {
          printf("Variable: %d. Index: %d. step: %d. Scratch: %f.\n",
                 var, index, (*MoL_Intermediate_Step), ScratchVar[index]);
          fflush(stdout);
        }
#endif        
      }
    }
  }
  

  /* Complex GFs */

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
  {
    
    UpdateComplexVar = (CCTK_COMPLEX *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                        EvolvedComplexVariableIndex[var]);
    RHSComplexVar = (CCTK_COMPLEX const *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                           RHSVariableIndex[var]);

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < totalsize; index++)
    {
      UpdateComplexVar[index] = CCTK_CmplxMul(Complex_Delta_Time,
                                              CCTK_CmplxMul(Complex_beta, 
                                                            RHSComplexVar[index])); 
    }
    
    for (scratchstep = 0; 
         scratchstep < MoL_Intermediate_Steps - (*MoL_Intermediate_Step) + 1;
         scratchstep++)
    {

      alphaindex = AlphaIndex(*MoL_Intermediate_Step, scratchstep);
      scratchindex = scratchstep - 1;

      alpha = RKAlphaCoefficients[alphaindex];
      Complex_alpha = CCTK_Cmplx(alpha, 0);

      if (scratchstep) 
      {
        ScratchComplexVar = &ComplexScratchSpace[(var *
                                                  MoL_Num_Scratch_Levels +
                                                  scratchindex) * totalsize];
      }
      else
      {
        ScratchComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 1, 
                                                            EvolvedComplexVariableIndex[var]);
      }
      
      if ( (alpha > MoL_Tiny)||(alpha < -MoL_Tiny) )
      {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
        for (index = 0; index < totalsize; index++)
        {
          UpdateComplexVar[index] =
            CCTK_CmplxAdd(UpdateComplexVar[index],
                          CCTK_CmplxMul(Complex_alpha,
                                        ScratchComplexVar[index]));
        }
      }
      
    }
    
  }

  if (*MoL_Intermediate_Step > 1)
  {
    for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
    {
      UpdateComplexVar = (CCTK_COMPLEX *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                          EvolvedComplexVariableIndex[var]);
      ScratchComplexVar = &ComplexScratchSpace[(var * MoL_Num_Scratch_Levels + 
                                                MoL_Intermediate_Steps -
                                                (*MoL_Intermediate_Step)) * 
                                              totalsize];
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (index = 0; index < totalsize; index++)
      {
        ScratchComplexVar[index] = UpdateComplexVar[index];
      }
    }
  }

  /* FIXME */

#endif

  /* Real arrays */

/* #define MOLDEBUGARRAYS 1 */

  arrayscratchlocation = 0;

#ifdef MOLDEBUGARRAYS
  CCTK_VWarn(1, __LINE__, __FILE__, CCTK_THORNSTRING, 
             "Array sizes are %d %d %d\n", MoL_Max_Evolved_Array_Size, arraytotalsize, singlearraysize);
#endif  

  for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
  {
    
    UpdateVar = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                              EvolvedArrayVariableIndex[var]);
    RHSVar = (CCTK_REAL const *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                 RHSArrayVariableIndex[var]);

    arraytotalsize = ArrayScratchSizes[var];

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (index = 0; index < arraytotalsize; index++)
    {
      UpdateVar[index] = (*Original_Delta_Time) / cctkGH->cctk_timefac * beta * RHSVar[index];
    }
    
    for (scratchstep = 0; 
         scratchstep < MoL_Intermediate_Steps - (*MoL_Intermediate_Step) + 1;
         scratchstep++)
    {

      alphaindex = AlphaIndex(*MoL_Intermediate_Step, scratchstep);
      scratchindex = scratchstep - 1;

      alpha = RKAlphaCoefficients[alphaindex];

      if (scratchstep) 
      {
        ScratchVar = &ArrayScratchSpace[scratchindex*CurrentArrayScratchSize + 
                                        arrayscratchlocation];
      }
      else
      {
        ScratchVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 1, 
                                                  EvolvedArrayVariableIndex[var]);
      }
      
      if ( (alpha > MoL_Tiny)||(alpha < -MoL_Tiny) )
      {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
        for (index = 0; index < arraytotalsize; index++)
        {
          UpdateVar[index] += alpha * ScratchVar[index];
#ifdef MOLDEBUGARRAYS
          if (CCTK_EQUALS(verbose,"extreme"))
          {
            CCTK_VWarn(1, __LINE__, __FILE__, CCTK_THORNSTRING, 
                       "Variable: %d. Index: %d. step: %d. "
                       "alpha: %f. Scratch: %f. q: %f.\n",
                       var, index, (*MoL_Intermediate_Step), 
                       alpha, ScratchVar[index], UpdateVar[index]);
          }
#endif
        }
      }
      
    }
   
    arrayscratchlocation += arraytotalsize;
 
  }

  arrayscratchlocation = 0;

  if (*MoL_Intermediate_Step > 1)
  {
    for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
    {
      UpdateVar = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                                EvolvedArrayVariableIndex[var]);
      ScratchVar = &ArrayScratchSpace[(MoL_Intermediate_Steps -
                                       (*MoL_Intermediate_Step)) * 
                                      CurrentArrayScratchSize +
/*                                      singlearraysize + */
/*                                      (MoL_Max_Evolved_Array_Size+1) +  */
                                     arrayscratchlocation];

      arraytotalsize = ArrayScratchSizes[var];


#ifdef MOLDEBUGARRAYS
      CCTK_VWarn(1, __LINE__, __FILE__, CCTK_THORNSTRING, 
                 "Writing to scratch space, initial address %ld, index %d \n", 
                 ScratchVar, (MoL_Intermediate_Steps -
                              (*MoL_Intermediate_Step)) * 
                 CurrentArrayScratchSize +
                 arrayscratchlocation);
#endif
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (index = 0; index < arraytotalsize; index++)
      {
        ScratchVar[index] = UpdateVar[index];
#ifdef MOLDEBUGARRAYS
        if (CCTK_EQUALS(verbose,"extreme"))
        {
          CCTK_VWarn(1, __LINE__, __FILE__, CCTK_THORNSTRING, 
                     "Variable: %d. Index: %d. step: %d. Scratch: %f.\n",
                     var, index, (*MoL_Intermediate_Step), ScratchVar[index]);
        }
#endif        
      }
      arrayscratchlocation += arraytotalsize;
    }
  }
        
  return;
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/

CCTK_INT AlphaIndex(CCTK_INT Step_Number, CCTK_INT Scratch_Level)
{
  DECLARE_CCTK_PARAMETERS
  
  return (MoL_Intermediate_Steps - Step_Number) * MoL_Intermediate_Steps + 
    Scratch_Level;
}
