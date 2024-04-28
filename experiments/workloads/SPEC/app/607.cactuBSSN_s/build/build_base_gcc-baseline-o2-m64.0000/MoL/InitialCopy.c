 /*@@
   @file      InitialCopy.c
   @date      Sun May 26 04:43:06 2002
   @author    Ian Hawke
   @desc 
   Performs the initial copy from the previous timelevel to the
   current. This is required because the driver has rotated the 
   timelevels, but the physics thorns are expecting data in the
   current.
   @enddesc 
   @version   $Header$
 @@*/

#include <stdlib.h>
#include <stdio.h>
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
#include "Operators.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_MoL_InitialCopy_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_InitialCopy(CCTK_ARGUMENTS);

void MoL_InitRHS(CCTK_ARGUMENTS);

void MoL_FillAllLevels(CCTK_ARGUMENTS);

void MoL_ReportNumberVariables(CCTK_ARGUMENTS);

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
   @routine    MoL_InitialCopy
   @date       ???
   @author     Ian Hawke
   @desc 
   Copy the previous time level to the current time level.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_InitialCopy(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  cGroupDynamicData arraydata;
  CCTK_INT groupindex, ierr;
  CCTK_INT arraytotalsize, arraydim;
  CCTK_INT totalarrayscratchsize;

  CCTK_INT var;
  CCTK_INT totalsize;

  CCTK_REAL       * restrict CurrentVar;
  CCTK_REAL const * restrict PreviousVar;
  CCTK_REAL       * restrict ScratchVar;
  CCTK_INT StorageOn;

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX       * restrict CurrentComplexVar;
  CCTK_COMPLEX const * restrict PreviousComplexVar;
  CCTK_COMPLEX       * restrict ScratchComplexVar;

#endif

  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    const int       nsrc = 1;
    const int       srcs[1] = {EvolvedVariableIndex[var]};
    const int       tls[1] = {1};
    const CCTK_REAL facts[1] = {1.0};
    
    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(EvolvedVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 EvolvedVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
#ifdef MOLDEBUG
      printf("Aargh! Vars %d var %d index %d name %s\n",
             MoLNumEvolvedVariables, var, EvolvedVariableIndex[var],  
                 CCTK_VarName(EvolvedVariableIndex[var]));
#endif
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(EvolvedVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }
    
    MoL_LinearCombination(cctkGH, EvolvedVariableIndex[var], 0.0,
                          srcs, tls, facts, nsrc);
  }
    
  /* Set up the array sizes */

  if (MoLNumEvolvedArrayVariables)
  {
    if (!ArrayScratchSizes)
    {
      ArrayScratchSizes = (CCTK_INT *)malloc(MoLNumEvolvedArrayVariables * sizeof(CCTK_INT));
      if (!ArrayScratchSizes)
      {
        CCTK_WARN(0, "Failed to allocate the array scratch sizes array.");
      }
      for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
      {
        ArrayScratchSizes[var] = -1;
      }
    }
  }
  
  totalarrayscratchsize = 0;

  for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
  {
    PreviousVar = (CCTK_REAL const*)CCTK_VarDataPtrI(cctkGH, 1,
                                               EvolvedArrayVariableIndex[var]);
    CurrentVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0,
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

    ArrayScratchSizes[var] = arraytotalsize;
    totalarrayscratchsize += arraytotalsize;

    if (arraytotalsize)
    {  
      if (PreviousVar && CurrentVar)
      {      
        memcpy(CurrentVar, PreviousVar, arraytotalsize * sizeof(CCTK_REAL));
      }
      else
      {
        printf("The pointers are %p (prev) and %p (curr)\n.",
               (void*) PreviousVar, (void*) CurrentVar);
        CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                   CCTK_VarName(EvolvedArrayVariableIndex[var]));
      }
    }
    
  }

  if (totalarrayscratchsize > CurrentArrayScratchSize)
  {
    if (ArrayScratchSpace)
    {
      free(ArrayScratchSpace);
      ArrayScratchSpace = NULL;
    }
    ArrayScratchSpace = 
      (CCTK_REAL*)malloc(totalarrayscratchsize * 
                         MoL_Num_Scratch_Levels * 
                         sizeof(CCTK_REAL));
    for (var = 0; var < totalarrayscratchsize * MoL_Num_Scratch_Levels; var++)
    {
      ArrayScratchSpace[var] = 0.0;
    }
    CurrentArrayScratchSize = totalarrayscratchsize;
  }
  

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
  {
    
    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(EvolvedComplexVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 EvolvedComplexVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(EvolvedComplexVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }
    
    PreviousComplexVar = (CCTK_COMPLEX const*)CCTK_VarDataPtrI(cctkGH, 1, 
                                               EvolvedComplexVariableIndex[var]);
    CurrentComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              EvolvedComplexVariableIndex[var]);
    if (PreviousComplexVar && CurrentComplexVar)
    {      
      memcpy(CurrentComplexVar, PreviousComplexVar, totalsize * sizeof(CCTK_COMPLEX));
    }
    else
    {
      CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                 CCTK_VarName(EvolvedComplexVariableIndex[var]));
    }

  }
  
#endif

  /* 
     Now the Save and Restore variables. Shift the data in the 
     current level to the scratch space, then do the copy 
  */

  for (var = 0; var < MoLNumSandRVariables; var++)
  {

    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(SandRVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 SandRVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(SandRVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }

    PreviousVar = (CCTK_REAL const*)CCTK_VarDataPtrI(cctkGH, 1, 
                                               SandRVariableIndex[var]);
    CurrentVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              SandRVariableIndex[var]);
    /*
      ScratchVar = &SandRScratchSpace[var*totalsize];
    */
    ScratchVar = CCTK_VarDataPtrI(cctkGH, 0, 
                                  CCTK_FirstVarIndex("MOL::SANDRSCRATCHSPACE")
                                  + var);
#ifdef MOLDEBUG
    printf("Pointers for the SandR vars are to %ld, %ld and %ld.\n",
           PreviousVar, CurrentVar, ScratchVar);
    
    printf("Init1:Variable %s, current %g, previous %g, scratch %g\n",
           CCTK_VarName(SandRVariableIndex[var]), CurrentVar[0], 
           PreviousVar[0], ScratchVar[0]);
#endif
    if (PreviousVar && CurrentVar && ScratchVar)
    {      
      memcpy(ScratchVar, CurrentVar, totalsize * sizeof(CCTK_REAL));
      memcpy(CurrentVar, PreviousVar, totalsize * sizeof(CCTK_REAL));
    }
    else
    {
      CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                 CCTK_VarName(SandRVariableIndex[var]));
    }
#ifdef MOLDEBUG
    printf("Init2:Variable %s, current %g, previous %g, scratch %g\n",
           CCTK_VarName(SandRVariableIndex[var]), CurrentVar[0], 
           PreviousVar[0], ScratchVar[0]);
#endif
  }  

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  for (var = 0; var < MoLNumSandRComplexVariables; var++)
  {

    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(SandRComplexVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 SandRComplexVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(SandRComplexVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }

    PreviousComplexVar = (CCTK_COMPLEX const*)CCTK_VarDataPtrI(cctkGH, 1, 
                                               SandRComplexVariableIndex[var]);
    CurrentComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              SandRComplexVariableIndex[var]);
    ScratchComplexVar = &ComplexSandRScratchSpace[var*totalsize];
#ifdef MOLDEBUG
    printf("Pointers for the SandR vars are to %ld, %ld and %ld.\n",
           PreviousComplexVar, CurrentComplexVar, ScratchComplexVar);
#endif
    if (PreviousComplexVar && CurrentComplexVar && ScratchComplexVar)
    {      
      memcpy(ScratchComplexVar, CurrentComplexVar, totalsize * 
             sizeof(CCTK_COMPLEX));
      memcpy(CurrentComplexVar, PreviousComplexVar, totalsize * 
             sizeof(CCTK_COMPLEX));
    }
    else
    {
      CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                 CCTK_VarName(SandRComplexVariableIndex[var]));
    }
  }
  
#endif

  /*
    Now do the constrained variables.
  */

  for (var = 0; var < MoLNumConstrainedVariables; var++)
  {

    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                        CCTK_GroupIndexFromVarI(ConstrainedVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 ConstrainedVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(ConstrainedVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }

    /* 
       Check that there is more than one timelevel.
       If not, copying is unnecessary.
    */

    StorageOn = CCTK_ActiveTimeLevelsVI(cctkGH,
                                        ConstrainedVariableIndex[var]);
    
    if (StorageOn > 1)
    {
      
      PreviousVar = (CCTK_REAL const*)CCTK_VarDataPtrI(cctkGH, 1, 
                                                 ConstrainedVariableIndex[var]);
      CurrentVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                ConstrainedVariableIndex[var]);

      if (PreviousVar && CurrentVar)
      {     
        memcpy(CurrentVar, PreviousVar, totalsize * sizeof(CCTK_REAL));
      }
      else
      {
        CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                   CCTK_VarName(ConstrainedVariableIndex[var]));
      }
      
    }
    
  }

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  for (var = 0; var < MoLNumConstrainedComplexVariables; var++)
  {

    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(ConstrainedComplexVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 ConstrainedComplexVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(ConstrainedComplexVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }

    /* 
       Check that there is more than one timelevel.
       If not, copying is unnecessary.
    */

    StorageOn = CCTK_ActiveTimeLevelsVI(cctkGH,
                                        ConstrainedVariableIndex[var]);
    
    if (StorageOn > 1)
    {

      PreviousComplexVar = (CCTK_COMPLEX const*)CCTK_VarDataPtrI(cctkGH, 1, 
                                                           ConstrainedComplexVariableIndex[var]);
      CurrentComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                          ConstrainedComplexVariableIndex[var]);
      
      if (PreviousComplexVar && CurrentComplexVar)
      {     
        memcpy(CurrentComplexVar, PreviousComplexVar, totalsize * sizeof(CCTK_COMPLEX));
      }
      else
      {
        CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                   CCTK_VarName(ConstrainedComplexVariableIndex[var]));
      }
      
    }
    
  }

#endif
  
  return;
}

 /*@@
   @routine    MoL_InitRHS
   @date       Tue Mar 23 2004
   @author     Erik Schnetter
   @desc 
   Initialise all RHS variables with zero.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_InitRHS(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  cGroupDynamicData arraydata;
  CCTK_INT groupindex, ierr;
  CCTK_INT arraytotalsize, arraydim;

  CCTK_INT var;
  CCTK_INT index;
/*   CCTK_INT i,j,k; */
  CCTK_INT totalsize;

  CCTK_REAL * restrict RHSVar;
  CCTK_INT StorageOn;

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX * restsrict RHSComplexVar;

#endif

  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(RHSVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 RHSVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
#ifdef MOLDEBUG
      printf("Aargh! Vars %d var %d index %d name %s\n",
             MoLNumEvolvedVariables, var, RHSVariableIndex[var],  
                 CCTK_VarName(RHSVariableIndex[var]));
#endif
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(RHSVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }
    
    MoL_LinearCombination(cctkGH, RHSVariableIndex[var], 0.0,
                          NULL, NULL, NULL, 0);
  }
    
  for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
  {
    RHSVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0,
                                          RHSArrayVariableIndex[var]);
    
    groupindex = CCTK_GroupIndexFromVarI(RHSArrayVariableIndex[var]);
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

    if (arraytotalsize)
    {
      if (RHSVar)
      {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
        for (index = 0; index < arraytotalsize; index++)
        {
          RHSVar[index] = 0;
        }
      }
      else
      {
        printf("The pointer is %p (rhs)\n.",
               (void*) RHSVar);
        CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                   CCTK_VarName(RHSArrayVariableIndex[var]));
      }
    }
    
  }

  /* FIXME */

#ifdef MOLDOESCOMPLEX

  for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
  {
    
    StorageOn = CCTK_QueryGroupStorageI(cctkGH,
                                       CCTK_GroupIndexFromVarI(RHSComplexVariableIndex[var]));
    
    if (StorageOn < 0)
    {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for index %i", 
                 RHSComplexVariableIndex[var]);
      CCTK_WARN(0, "The index passed does not correspond to a GF.");
    }
    else if (StorageOn == 0) {
      CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,"Warning for GF %s", 
                 CCTK_VarName(RHSComplexVariableIndex[var]));
      CCTK_WARN(0, "The grid function does not have storage assigned.");
    }
    
    RHSComplexVar = (CCTK_COMPLEX*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                    RHSComplexVariableIndex[var]);
    if (RHSVar)
    {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (index = 0; index < totalsize; index++)
      {
        RHSVar[index] = 0;
      }
    }
    else
    {
      CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,"Null pointer for variable %s", 
                 CCTK_VarName(RHSComplexVariableIndex[var]));
    }

  }
  
#endif
  
  return;
}

 /*@@
   @routine    MoL_FillAllLevels
   @date       Fri Apr 25 16:11:18 2003
   @author     Ian Hawke
   @desc 
   This routine is a bit of a hack, and I'm still not convinced 
   it is really necessary. It fills the previous timelevels by 
   copying the data from the current timelevels, which should have
   been set up during the CCTK_INITIAL timebin.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_FillAllLevels(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS

  CCTK_INT var, level;
  CCTK_INT totalsize, arraydim;

  CCTK_REAL const * restrict CurrentVar;
  CCTK_REAL       * restrict PreviousVar;

  totalsize = 1;
  for (arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }

  for (var = 0; var < MoLNumEvolvedVariables; var++)
  {
    CurrentVar = (CCTK_REAL const*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              EvolvedVariableIndex[var]);
    for (level = 1; 
         level < CCTK_ActiveTimeLevelsVI(cctkGH,
                                         EvolvedVariableIndex[var]); 
         level++)
    {
      PreviousVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, level, 
                                                 EvolvedVariableIndex[var]);
      if (PreviousVar)
      {   
        memcpy(PreviousVar, CurrentVar, totalsize * sizeof(CCTK_REAL));
      }
    }
  }
  

  for (var = 0; var < MoLNumConstrainedVariables; var++)
  {
    CurrentVar = (CCTK_REAL const*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              ConstrainedVariableIndex[var]);
    for (level = 1; level < CCTK_ActiveTimeLevelsVI(cctkGH,
                                                    ConstrainedVariableIndex[var]); level++)
    {
      PreviousVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, level, 
                                                 ConstrainedVariableIndex[var]);
      if (PreviousVar)
      {   
        memcpy(PreviousVar, CurrentVar, totalsize * sizeof(CCTK_REAL));
      }
    }
  }
  

  for (var = 0; var < MoLNumSandRVariables; var++)
  {
    CurrentVar = (CCTK_REAL const*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              SandRVariableIndex[var]);
    for (level = 1; level < CCTK_ActiveTimeLevelsVI(cctkGH,
                                                    SandRVariableIndex[var]); 
         level++)
    {
      PreviousVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, level, 
                                                 SandRVariableIndex[var]);
      if (PreviousVar)
      {   
        memcpy(PreviousVar, CurrentVar, totalsize * sizeof(CCTK_REAL));
      }
    }
  }

  return;
}

 /*@@
   @routine    MoL_ReportNumberVariables
   @date       Thu Jul 17 18:35:54 2003
   @author     Ian Hawke
   @desc 
   Prints some useful information about the number of
   registered functions.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_ReportNumberVariables(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS

  CCTK_INT var;
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of evolved variables is %d. "
             "%d are registered.",
             MoL_Num_Evolved_Vars,MoLNumEvolvedVariables);

  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of constrained variables is %d. "
             "%d are registered.",
             MoL_Num_Constrained_Vars,MoLNumConstrainedVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of SandR variables is %d. "
             "%d are registered.",
             MoL_Num_SaveAndRestore_Vars,MoLNumSandRVariables);
  

  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of evolved complex variables is %d. "
             "%d are registered.",
             MoL_Num_ComplexEvolved_Vars,MoLNumEvolvedComplexVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of constrained complex variables is %d. "
             "%d are registered.",
             MoL_Num_ComplexConstrained_Vars,MoLNumConstrainedComplexVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of SandR complex variables is %d. "
             "%d are registered.",
             MoL_Num_ComplexSaveAndRestore_Vars,MoLNumSandRComplexVariables);
  

  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of evolved array variables is %d. "
             "%d are registered.",
             MoL_Num_ArrayEvolved_Vars,MoLNumEvolvedArrayVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of constrained array variables is %d. "
             "%d are registered.",
             MoL_Num_ArrayConstrained_Vars,MoLNumConstrainedArrayVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of SandR array variables is %d. "
             "%d are registered.",
             MoL_Num_ArraySaveAndRestore_Vars,MoLNumSandRArrayVariables);
  

  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of evolved complex array variables is %d. "
             "%d are registered.",
             MoL_Num_ComplexArrayEvolved_Vars,
             MoLNumEvolvedComplexArrayVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of constrained complex array "
             "variables is %d. %d are registered.",
             MoL_Num_ComplexArrayConstrained_Vars,
             MoLNumConstrainedComplexArrayVariables);
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum number of SandR complex array "
             "variables is %d. %d are registered.",
             MoL_Num_ComplexArraySaveAndRestore_Vars,
             MoLNumSandRComplexArrayVariables);  
  
  CCTK_VInfo(CCTK_THORNSTRING,
             "The maximum size of any array variables is %d.",
             MoL_Max_Evolved_Array_Size);

  if (CCTK_Equals(verbose, "register"))
  {

    if (MoLNumEvolvedVariables)
    {
      CCTK_INFO("The evolved variables are:");
      for (var = 0; var < MoLNumEvolvedVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(EvolvedVariableIndex[var]));
      }
    }

    if (MoLNumConstrainedVariables)
    {    
      CCTK_INFO("The constrained variables are:");
      for (var = 0; var < MoLNumConstrainedVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(ConstrainedVariableIndex[var]));
      }
    }
    
    if (MoLNumSandRVariables)
    {
      CCTK_INFO("The save and restore variables are:");
      for (var = 0; var < MoLNumSandRVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(SandRVariableIndex[var]));
      }
    }
    
    if (MoLNumEvolvedComplexVariables)
    {
      CCTK_INFO("The evolved complex variables are:");
      for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(EvolvedComplexVariableIndex[var]));
      }
    }
    
    if (MoLNumConstrainedComplexVariables)
    {
      CCTK_INFO("The constrained complex variables are:");
      for (var = 0; var < MoLNumConstrainedComplexVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(ConstrainedComplexVariableIndex[var]));
      }
    }
    
    if (MoLNumSandRComplexVariables)
    {
      CCTK_INFO("The constrained complex variables are:");
      for (var = 0; var < MoLNumSandRComplexVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(SandRComplexVariableIndex[var]));
      }
    }
    
    if (MoLNumEvolvedArrayVariables)
    {
      CCTK_INFO("The evolved array variables are:");
      for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(EvolvedArrayVariableIndex[var]));
      }
    }

    if (MoLNumConstrainedArrayVariables)
    {    
      CCTK_INFO("The constrained array variables are:");
      for (var = 0; var < MoLNumConstrainedArrayVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(ConstrainedArrayVariableIndex[var]));
      }
    }
    
    if (MoLNumSandRArrayVariables)
    {
      CCTK_INFO("The save and restore array variables are:");
      for (var = 0; var < MoLNumSandRArrayVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(SandRArrayVariableIndex[var]));
      }
    }
    
    if (MoLNumEvolvedComplexArrayVariables)
    {
      CCTK_INFO("The evolved complex array variables are:");
      for (var = 0; var < MoLNumEvolvedComplexArrayVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(EvolvedComplexArrayVariableIndex[var]));
      }
    }
    
    if (MoLNumConstrainedComplexArrayVariables)
    {
      CCTK_INFO("The constrained complex array variables are:");
      for (var = 0; var < MoLNumConstrainedComplexArrayVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(ConstrainedComplexArrayVariableIndex[var]));
      }
    }
    
    if (MoLNumSandRComplexArrayVariables)
    {
      CCTK_INFO("The constrained complex array variables are:");
      for (var = 0; var < MoLNumSandRComplexArrayVariables; var++)
      {
        CCTK_VInfo(CCTK_THORNSTRING,"   %d   :   %s",
                   var, CCTK_VarName(SandRComplexArrayVariableIndex[var]));
      }
    }
    
  }

  return;
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
