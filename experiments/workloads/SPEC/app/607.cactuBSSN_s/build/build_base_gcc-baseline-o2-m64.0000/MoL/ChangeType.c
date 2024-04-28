 /*@@
   @file      ChangeType.c
   @date      Thu May 30 16:16:40 2002
   @author    Ian Hawke
   @desc 
   The external functions called (via function aliasing) by physics
   thorns to tell MoL that they want these GFs to be treated as a
   different type to the original declaration.
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
#include "MoL/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

#include "ExternalVariables.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_MoL_ChangeType_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

#define MOL_UNKNOWN_VARTYPE 0
#define MOL_EVOLVED_VARTYPE 1
#define MOL_CONSTRAINED_VARTYPE 2
#define MOL_SANDR_VARTYPE 3
#define MOL_EVOLVEDSLOW_VARTYPE 4

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ********************* Other Routine Prototypes *********************
 ********************************************************************/

CCTK_INT MoL_ChangeToEvolved(CCTK_INT EvolvedIndex, CCTK_INT RHSIndex);

CCTK_INT MoL_ChangeToEvolvedSlow(CCTK_INT EvolvedIndex, CCTK_INT RHSIndexSlow);

CCTK_INT MoL_ChangeToConstrained(CCTK_INT ConstrainedIndex);

CCTK_INT MoL_ChangeToSaveAndRestore(CCTK_INT SandRIndex);

CCTK_INT MoL_ChangeToNone(CCTK_INT RemoveIndex);


/********************************************************************
 *********************     Local Data   *****************************
 ********************************************************************/

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

 /*@@
   @routine    MoL_ChangeToEvolved
   @date       Thu May 30 16:45:30 2002
   @author     Ian Hawke
   @desc 
   Changes a variable to evolved type. Checks to see which type it was
   before and does the bookkeeping on the index arrays.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

CCTK_INT MoL_ChangeToEvolved(CCTK_INT EvolvedIndex, CCTK_INT RHSIndex)
{

  DECLARE_CCTK_PARAMETERS

  CCTK_INT index, usedindex;
  CCTK_INT vartype; /* See the defines at the top of file */
  CCTK_INT timelevs;
  
  vartype = 0;
  usedindex = -1;
  
  for (index = 0; (index < MoLNumEvolvedVariables)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVED_VARTYPE * 
      (EvolvedVariableIndex[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumEvolvedVariablesSlow)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVEDSLOW_VARTYPE * 
      (EvolvedVariableIndexSlow[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumConstrainedVariables)&&(!vartype); index++)
  {
    vartype = MOL_CONSTRAINED_VARTYPE * 
      (ConstrainedVariableIndex[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumSandRVariables)&&(!vartype); index++)
  {
    vartype = MOL_SANDR_VARTYPE * 
      (SandRVariableIndex[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }

  switch (vartype)
  {
    
    case MOL_UNKNOWN_VARTYPE:

      {
        timelevs = CCTK_MaxTimeLevelsVI(EvolvedIndex);
        if (timelevs < 1)
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to evolved.", EvolvedIndex);
          CCTK_WARN(0, "The index passed does not correspond to a GF.");
        }
        else if (timelevs == 1) 
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to evolved.", EvolvedIndex);
          CCTK_WARN(0, "The index passed only has a single timelevel.");
        }
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to evolved (RHS GF).", RHSIndex);
          CCTK_WARN(0, "The index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariables < MoL_Num_Evolved_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        EvolvedVariableIndex[MoLNumEvolvedVariables] = EvolvedIndex;
        RHSVariableIndex[MoLNumEvolvedVariables] = RHSIndex;
        MoLNumEvolvedVariables++;
#ifdef MOLDEBUG
        printf("Changing (unknown): vars %d var %d (%s).\n",
               MoLNumEvolvedVariables, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    case MOL_EVOLVED_VARTYPE:
      
      {
        RHSVariableIndex[usedindex] = RHSIndex;
        break;
      }

    case MOL_EVOLVEDSLOW_VARTYPE:
      
      {
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to evolved type from constrained.", RHSIndex);
          CCTK_WARN(0, "The RHS index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariables < MoL_Num_Evolved_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        for (index = usedindex; index < MoLNumEvolvedVariablesSlow - 1; 
             index++)
        {
          EvolvedVariableIndexSlow[index] = EvolvedVariableIndexSlow[index+1];
        }
        MoLNumEvolvedVariablesSlow--;
        EvolvedVariableIndex[MoLNumEvolvedVariables] = EvolvedIndex;
        RHSVariableIndex[MoLNumEvolvedVariables] = RHSIndex;
        MoLNumEvolvedVariables++;
#ifdef MOLDEBUG
        printf("Changing (constrained): vars %d var %d (%s).\n",
               MoLNumEvolvedVariables, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    case MOL_CONSTRAINED_VARTYPE:
      
      {
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to evolved type from constrained.", RHSIndex);
          CCTK_WARN(0, "The RHS index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariables < MoL_Num_Evolved_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        for (index = usedindex; index < MoLNumConstrainedVariables - 1; 
             index++)
        {
          ConstrainedVariableIndex[index] = ConstrainedVariableIndex[index+1];
        }
        MoLNumConstrainedVariables--;
        EvolvedVariableIndex[MoLNumEvolvedVariables] = EvolvedIndex;
        RHSVariableIndex[MoLNumEvolvedVariables] = RHSIndex;
        MoLNumEvolvedVariables++;
#ifdef MOLDEBUG
        printf("Changing (constrained): vars %d var %d (%s).\n",
               MoLNumEvolvedVariables, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    case MOL_SANDR_VARTYPE:
      
      {
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to evolved type from save and "
                     "restore.", RHSIndex);
          CCTK_WARN(0, "The RHS index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariables < MoL_Num_Evolved_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        for (index = usedindex; index < MoLNumSandRVariables - 1; index++)
        {
          SandRVariableIndex[index] = SandRVariableIndex[index+1];
        }
        MoLNumSandRVariables--;
        EvolvedVariableIndex[MoLNumEvolvedVariables] = EvolvedIndex;
        RHSVariableIndex[MoLNumEvolvedVariables] = RHSIndex;
        MoLNumEvolvedVariables++;
#ifdef MOLDEBUG
        printf("Changing (SandR): vars %d var %d (%s).\n",
               MoLNumEvolvedVariables, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    default:
      
      {
        CCTK_WARN(0, "Something is seriously wrong in ChangeType.c! "
                  "Case out of range in switch statement.");
      }
      
  }
      
  return 0;
  
}

 /*@@
   @routine    MoL_ChangeToEvolvedSlow
   @date       Thu May 30 16:45:30 2002
   @author     Ian Hawke, Roland Haas
   @desc 
   Changes a variable to evolved slow type. Checks to see which type it was
   before and does the bookkeeping on the index arrays.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

CCTK_INT MoL_ChangeToEvolvedSlow(CCTK_INT EvolvedIndex, CCTK_INT RHSIndex)
{

  DECLARE_CCTK_PARAMETERS

  CCTK_INT index, usedindex;
  CCTK_INT vartype; /* See the defines at the top of file */
  CCTK_INT timelevs;
  
  vartype = 0;
  usedindex = -1;
  
  for (index = 0; (index < MoLNumEvolvedVariables)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVED_VARTYPE * 
      (EvolvedVariableIndex[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumEvolvedVariablesSlow)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVEDSLOW_VARTYPE * 
      (EvolvedVariableIndexSlow[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumConstrainedVariables)&&(!vartype); index++)
  {
    vartype = MOL_CONSTRAINED_VARTYPE * 
      (ConstrainedVariableIndex[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumSandRVariables)&&(!vartype); index++)
  {
    vartype = MOL_SANDR_VARTYPE * 
      (SandRVariableIndex[index] == EvolvedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }

  switch (vartype)
  {
    
    case MOL_UNKNOWN_VARTYPE:

      {
        timelevs = CCTK_MaxTimeLevelsVI(EvolvedIndex);
        if (timelevs < 1)
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to slow evolved.", EvolvedIndex);
          CCTK_WARN(0, "The index passed does not correspond to a GF.");
        }
        else if (timelevs == 1) 
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to slow evolved.", EvolvedIndex);
          CCTK_WARN(0, "The index passed only has a single timelevel.");
        }
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to slow evolved (RHS GF).", RHSIndex);
          CCTK_WARN(0, "The index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariablesSlow < MoL_Num_Evolved_Vars_Slow))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to slow evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars_Slow allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        EvolvedVariableIndexSlow[MoLNumEvolvedVariablesSlow] = EvolvedIndex;
        RHSVariableIndexSlow[MoLNumEvolvedVariablesSlow] = RHSIndex;
        MoLNumEvolvedVariablesSlow++;
#ifdef MOLDEBUG
        printf("Changing (unknown): vars %d var %d (%s).\n",
               MoLNumEvolvedVariablesSlow, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    case MOL_EVOLVED_VARTYPE:
      
      {
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to slow evolved type from constrained.", RHSIndex);
          CCTK_WARN(0, "The RHS index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariablesSlow < MoL_Num_Evolved_Vars_Slow))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to slow evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars_Slow allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        for (index = usedindex; index < MoLNumEvolvedVariables - 1; 
             index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
        }
        MoLNumEvolvedVariables--;
        EvolvedVariableIndexSlow[MoLNumEvolvedVariablesSlow] = EvolvedIndex;
        RHSVariableIndexSlow[MoLNumEvolvedVariablesSlow] = RHSIndex;
        MoLNumEvolvedVariablesSlow++;
#ifdef MOLDEBUG
        printf("Changing (constrained): vars %d var %d (%s).\n",
               MoLNumEvolvedVariablesSlow, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    case MOL_EVOLVEDSLOW_VARTYPE:
      
      {
        RHSVariableIndex[usedindex] = RHSIndex;
        break;
      }

    case MOL_CONSTRAINED_VARTYPE:
      
      {
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to slow evolved type from constrained.", RHSIndex);
          CCTK_WARN(0, "The RHS index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariablesSlow < MoL_Num_Evolved_Vars_Slow))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to slow evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars_Slow allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        for (index = usedindex; index < MoLNumConstrainedVariables - 1; 
             index++)
        {
          ConstrainedVariableIndex[index] = ConstrainedVariableIndex[index+1];
        }
        MoLNumConstrainedVariables--;
        EvolvedVariableIndexSlow[MoLNumEvolvedVariablesSlow] = EvolvedIndex;
        RHSVariableIndexSlow[MoLNumEvolvedVariablesSlow] = RHSIndex;
        MoLNumEvolvedVariablesSlow++;
#ifdef MOLDEBUG
        printf("Changing (constrained): vars %d var %d (%s).\n",
               MoLNumEvolvedVariables, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    case MOL_SANDR_VARTYPE:
      
      {
        timelevs = CCTK_MaxTimeLevelsVI(RHSIndex);
        if (timelevs < 1) {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i to slow evolved type from save and "
                     "restore.", RHSIndex);
          CCTK_WARN(0, "The RHS index passed does not correspond to a GF.");
        }

        if (!(MoLNumEvolvedVariablesSlow < MoL_Num_Evolved_Vars_Slow))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to slow evolved.", 
                     EvolvedIndex, CCTK_VarName(EvolvedIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_Evolved_Vars_Slow allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }

        for (index = usedindex; index < MoLNumSandRVariables - 1; index++)
        {
          SandRVariableIndex[index] = SandRVariableIndex[index+1];
        }
        MoLNumSandRVariables--;
        EvolvedVariableIndexSlow[MoLNumEvolvedVariablesSlow] = EvolvedIndex;
        RHSVariableIndexSlow[MoLNumEvolvedVariablesSlow] = RHSIndex;
        MoLNumEvolvedVariablesSlow++;
#ifdef MOLDEBUG
        printf("Changing (SandR): vars %d var %d (%s).\n",
               MoLNumEvolvedVariablesSlow, EvolvedIndex,
               CCTK_VarName(EvolvedIndex));
#endif
        break;
      }
      
    default:
      
      {
        CCTK_WARN(0, "Something is seriously wrong in ChangeType.c! "
                  "Case out of range in switch statement.");
      }
      
  }
      
  return 0;
  
}

 /*@@
   @routine    MoL_ChangeToConstrained
   @date       Thu May 30 16:47:08 2002
   @author     Ian Hawke
   @desc 
   Changes a variable to be constrained.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

CCTK_INT MoL_ChangeToConstrained(CCTK_INT ConstrainedIndex)
{

  DECLARE_CCTK_PARAMETERS

  CCTK_INT index, usedindex;
  CCTK_INT vartype; /* See the defines at the top of file */
  CCTK_INT timelevs;
  
  vartype = 0;
  usedindex = -1;

  timelevs = CCTK_MaxTimeLevelsVI(ConstrainedIndex);
  if (timelevs < 1)
  {
    CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
               "Warning whilst trying to change variable "
               "index %i to constrained.", ConstrainedIndex);
    CCTK_WARN(0, "The index passed does not correspond to a GF.");
  }
  else if (timelevs == 1) 
  {
    CCTK_VInfo(CCTK_THORNSTRING,
               "MoL will not treat variable %s as a constrained "
               "variable as it has only one timelevel. This should "
               "not cause problems with the evolution.", 
               CCTK_VarName(ConstrainedIndex));
  }
  
  for (index = 0; (index < MoLNumEvolvedVariables)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVED_VARTYPE * 
      (EvolvedVariableIndex[index] == ConstrainedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumEvolvedVariablesSlow)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVEDSLOW_VARTYPE * 
      (EvolvedVariableIndexSlow[index] == ConstrainedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumConstrainedVariables)&&(!vartype); index++)
  {
    vartype = MOL_CONSTRAINED_VARTYPE * 
      (ConstrainedVariableIndex[index] == ConstrainedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumSandRVariables)&&(!vartype); index++)
  {
    vartype = MOL_SANDR_VARTYPE * 
      (SandRVariableIndex[index] == ConstrainedIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }

  switch (vartype)
  {
    
    case MOL_UNKNOWN_VARTYPE:

      {
        if (timelevs > 1)
        {

          if ( !(MoLNumConstrainedVariables < MoL_Num_Constrained_Vars))
          {
            CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                       "Warning whilst trying to change variable "
                       "index %i (%s) to constrained.", 
                       ConstrainedIndex, CCTK_VarName(ConstrainedIndex));
            CCTK_WARN(0, "When changing type there are more variables "
                      "than the accumulator parameter "
                      "MoL_Num_Constrained_Vars allows. Check that "
                      "you are accumulating onto this parameter correctly");
          }

          ConstrainedVariableIndex[MoLNumConstrainedVariables] = 
            ConstrainedIndex;
          MoLNumConstrainedVariables++;
        }
        break;
      }
      
    case MOL_EVOLVED_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumEvolvedVariables - 1; index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
          RHSVariableIndex[index] = RHSVariableIndex[index+1];
        }
        MoLNumEvolvedVariables--;
        if (timelevs > 1)
        {
          if ( !(MoLNumConstrainedVariables < MoL_Num_Constrained_Vars))
          {
            CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                       "Warning whilst trying to change variable "
                       "index %i (%s) to constrained.", 
                       ConstrainedIndex, CCTK_VarName(ConstrainedIndex));
            CCTK_WARN(0, "When changing type there are more variables "
                      "than the accumulator parameter "
                      "MoL_Num_Constrained_Vars allows. Check that "
                      "you are accumulating onto this parameter correctly");
          }
          ConstrainedVariableIndex[MoLNumConstrainedVariables] = ConstrainedIndex;
          MoLNumConstrainedVariables++;
        }
        break;
      }

    case MOL_EVOLVEDSLOW_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumEvolvedVariablesSlow - 1; index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
          RHSVariableIndex[index] = RHSVariableIndex[index+1];
        }
        MoLNumEvolvedVariablesSlow--;
        if (timelevs > 1)
        {
          if ( !(MoLNumConstrainedVariables < MoL_Num_Constrained_Vars))
          {
            CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                       "Warning whilst trying to change variable "
                       "index %i (%s) to constrained.", 
                       ConstrainedIndex, CCTK_VarName(ConstrainedIndex));
            CCTK_WARN(0, "When changing type there are more variables "
                      "than the accumulator parameter "
                      "MoL_Num_Constrained_Vars allows. Check that "
                      "you are accumulating onto this parameter correctly");
          }
          ConstrainedVariableIndex[MoLNumConstrainedVariables] = ConstrainedIndex;
          MoLNumConstrainedVariables++;
        }
        break;
      }

    case MOL_CONSTRAINED_VARTYPE:
      
      {
        break;
      }
      
    case MOL_SANDR_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumSandRVariables - 1; index++)
        {
          SandRVariableIndex[index] = SandRVariableIndex[index+1];
        }
        MoLNumSandRVariables--;
        if (timelevs > 1)
        {
          if ( !(MoLNumConstrainedVariables < MoL_Num_Constrained_Vars))
          {
            CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                       "Warning whilst trying to change variable "
                       "index %i (%s) to constrained.", 
                       ConstrainedIndex, CCTK_VarName(ConstrainedIndex));
            CCTK_WARN(0, "When changing type there are more variables "
                      "than the accumulator parameter "
                      "MoL_Num_Constrained_Vars allows. Check that "
                      "you are accumulating onto this parameter correctly");
          }
          ConstrainedVariableIndex[MoLNumConstrainedVariables] = ConstrainedIndex;
          MoLNumConstrainedVariables++;
        }
        break;
      }
      
    default:
      
      {
        CCTK_WARN(0, "Something is seriously wrong in ChangeType.c! "
                  "Case out of range in switch statement.");
      }
      
  }
      
  return 0;
  
}

 /*@@
   @routine    MoL_ChangeToSaveAndRestore
   @date       Thu May 30 16:50:36 2002
   @author     Ian Hawke
   @desc 
   Changes the variable type to save and restore.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

CCTK_INT MoL_ChangeToSaveAndRestore(CCTK_INT SandRIndex)
{

  DECLARE_CCTK_PARAMETERS

  CCTK_INT index, usedindex;
  CCTK_INT vartype; /* See the defines at the top of file */
  CCTK_INT timelevs;
  
  vartype = 0;
  usedindex = -1;

  timelevs = CCTK_MaxTimeLevelsVI(SandRIndex);
  if (timelevs < 1)
  {
    CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
               "Warning whilst trying to change variable "
               "index %i to save and restore.", SandRIndex);
    CCTK_WARN(0, "The index passed does not correspond to a GF.");
  }
  
  for (index = 0; (index < MoLNumEvolvedVariables)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVED_VARTYPE * 
      (EvolvedVariableIndex[index] == SandRIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumEvolvedVariablesSlow)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVEDSLOW_VARTYPE * 
      (EvolvedVariableIndexSlow[index] == SandRIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumConstrainedVariables)&&(!vartype); index++)
  {
    vartype = MOL_CONSTRAINED_VARTYPE * 
      (ConstrainedVariableIndex[index] == SandRIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumSandRVariables)&&(!vartype); index++)
  {
    vartype = MOL_SANDR_VARTYPE * 
      (SandRVariableIndex[index] == SandRIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }

  switch (vartype)
  {
    
    case MOL_UNKNOWN_VARTYPE:

      {

        if (!(MoLNumSandRVariables < MoL_Num_SaveAndRestore_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to save and restore.", 
                     SandRIndex, CCTK_VarName(SandRIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_SaveAndRestore_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }
        SandRVariableIndex[MoLNumSandRVariables] = SandRIndex;
        MoLNumSandRVariables++;
        break;
      }
      
    case MOL_EVOLVED_VARTYPE:
      
      {

        if (!(MoLNumSandRVariables < MoL_Num_SaveAndRestore_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to save and restore.", 
                     SandRIndex, CCTK_VarName(SandRIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_SaveAndRestore_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }
        for (index = usedindex; index < MoLNumEvolvedVariables - 1; index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
          RHSVariableIndex[index] = RHSVariableIndex[index+1];
        }
        MoLNumEvolvedVariables--;
        SandRVariableIndex[MoLNumSandRVariables] = SandRIndex;
        MoLNumSandRVariables++;
        break;
      }

    case MOL_EVOLVEDSLOW_VARTYPE:
      
      {

        if (!(MoLNumSandRVariables < MoL_Num_SaveAndRestore_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to save and restore.", 
                     SandRIndex, CCTK_VarName(SandRIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_SaveAndRestore_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }
        for (index = usedindex; index < MoLNumEvolvedVariablesSlow - 1; index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
          RHSVariableIndex[index] = RHSVariableIndex[index+1];
        }
        MoLNumEvolvedVariablesSlow--;
        SandRVariableIndex[MoLNumSandRVariables] = SandRIndex;
        MoLNumSandRVariables++;
        break;
      }

    case MOL_CONSTRAINED_VARTYPE:
      
      {

        if (!(MoLNumSandRVariables < MoL_Num_SaveAndRestore_Vars))
        {
          CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
                     "Warning whilst trying to change variable "
                     "index %i (%s) to save and restore.", 
                     SandRIndex, CCTK_VarName(SandRIndex));
          CCTK_WARN(0, "When changing type there are more variables "
                    "than the accumulator parameter "
                    "MoL_Num_SaveAndRestore_Vars allows. Check that "
                    "you are accumulating onto this parameter correctly");
        }
        for (index = usedindex; index < MoLNumConstrainedVariables -
               1; 
             index++)
        {
          ConstrainedVariableIndex[index] = ConstrainedVariableIndex[index+1];
        }
        MoLNumConstrainedVariables--;
        SandRVariableIndex[MoLNumSandRVariables] = SandRIndex;
        MoLNumSandRVariables++;
        break;
      }
      
    case MOL_SANDR_VARTYPE:
      
      {
        break;
      }
      
    default:
      
      {
        CCTK_WARN(0, "Something is seriously wrong in ChangeType.c! "
                  "Case out of range in switch statement.");
      }
      
  }
      
  return 0;
  
}

 /*@@
   @routine    MoL_ChangeToNone
   @date       Thu May 30 16:53:46 2002
   @author     Ian Hawke
   @desc 
   Changes a variable type to none (i.e., MoL no longer considers it).
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

CCTK_INT MoL_ChangeToNone(CCTK_INT RemoveIndex)
{

  CCTK_INT index, usedindex;
  CCTK_INT vartype; /* See the defines at the top of file */
/*    CCTK_INT timelevs; */
  
  vartype = 0;
  usedindex = -1;
  
  for (index = 0; (index < MoLNumEvolvedVariables)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVED_VARTYPE * 
      (EvolvedVariableIndex[index] == RemoveIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumEvolvedVariablesSlow)&&(!vartype); index++)
  {
    vartype = MOL_EVOLVEDSLOW_VARTYPE * 
      (EvolvedVariableIndexSlow[index] == RemoveIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumConstrainedVariables)&&(!vartype); index++)
  {
    vartype = MOL_CONSTRAINED_VARTYPE * 
      (ConstrainedVariableIndex[index] == RemoveIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }
  
  for (index = 0; (index < MoLNumSandRVariables)&&(!vartype); index++)
  {
    vartype = MOL_SANDR_VARTYPE * 
      (SandRVariableIndex[index] == RemoveIndex);
    if (vartype)
    {
      usedindex = index;
    }
  }

  switch (vartype)
  {
    
    case MOL_UNKNOWN_VARTYPE:

      {
        break;
      }
      
    case MOL_EVOLVED_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumEvolvedVariables - 1; index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
          RHSVariableIndex[index] = RHSVariableIndex[index+1];
        }
        MoLNumEvolvedVariables--;
        break;
      }

    case MOL_EVOLVEDSLOW_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumEvolvedVariablesSlow - 1; index++)
        {
          EvolvedVariableIndex[index] = EvolvedVariableIndex[index+1];
          RHSVariableIndex[index] = RHSVariableIndex[index+1];
        }
        MoLNumEvolvedVariablesSlow--;
        break;
      }

    case MOL_CONSTRAINED_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumConstrainedVariables - 1; 
             index++)
        {
          ConstrainedVariableIndex[index] = ConstrainedVariableIndex[index+1];
        }
        MoLNumConstrainedVariables--;
        break;
      }
      
    case MOL_SANDR_VARTYPE:
      
      {
        for (index = usedindex; index < MoLNumSandRVariables - 1; index++)
        {
          SandRVariableIndex[index] = SandRVariableIndex[index+1];
        }
        MoLNumSandRVariables--;
        break;
      }
      
    default:
      
      {
        CCTK_WARN(0, "Something is seriously wrong in ChangeType.c! "
                  "Case out of range in switch statement.");
      }
      
  }
      
  return 0;
  
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
