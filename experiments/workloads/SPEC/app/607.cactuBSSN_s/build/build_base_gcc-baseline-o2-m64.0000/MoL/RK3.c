 /*@@
   @file      RK3.c
   @date      Tue Jul 22 00:38:47 2003
   @author    Ian Hawke
   @desc 
   A specialized third order Runge-Kutta time integrator. This is
   the integrator that Shu refers to as the optimal TVD third 
   order method (see reference in documentation).
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
#include "Operators.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_MoL_RK3_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_RK3Add(CCTK_ARGUMENTS);

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
   @routine    MoL_RK3Add
   @date       Tue Jul 22 00:39:55 2003
   @author     Ian Hawke
   @desc 
   Performs third order Runge-Kutta time integration.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

 @@*/

void MoL_RK3Add(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS

  CCTK_INT arraydim;
  
  CCTK_INT var;
  CCTK_INT totalsize;
  
  /* FIXME */

#ifdef MOLDOESCOMPLEX

  CCTK_COMPLEX const * restrict OldComplexVar;
  CCTK_COMPLEX       * restrict UpdateComplexVar;
  CCTK_COMPLEX const * restrict RHSComplexVar;
  CCTK_COMPLEX Complex_Delta_Time = CCTK_Cmplx(CCTK_DELTA_TIME, 0);
  CCTK_COMPLEX Complex_Half = CCTK_Cmplx(0.5, 0);
  CCTK_COMPLEX Complex_Third = CCTK_Cmplx(1.0/3.0, 0);
  CCTK_COMPLEX Complex_TwoThird = CCTK_Cmplx(2.0/3.0, 0);
  CCTK_COMPLEX Complex_Quarter = CCTK_Cmplx(0.25, 0);
  CCTK_COMPLEX Complex_ThreeQuarter = CCTK_Cmplx(0.75, 0);

#endif

#ifdef MOLDEBUG
  printf("Inside RK3.\nStep %d.\nRefinement %d.\nTimestep %g.\n"
         "Spacestep %g.\nTime %g\n",
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

  switch (*MoL_Intermediate_Step)
  {

    case 3:
      {
        for (var = 0; var < MoLNumEvolvedVariables; var++)
        {
          int const nsrcs = 1;
          CCTK_INT const srcs[] = {RHSVariableIndex[var]};
          CCTK_INT const tls[] = {0};
          CCTK_REAL const facts[] = {CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedVariableIndex[var], 1.0,
                                srcs, tls, facts, nsrcs);
        }

        for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
        {
          int const nsrcs = 1;
          CCTK_INT const srcs[] = {RHSArrayVariableIndex[var]};
          CCTK_INT const tls[] = {0};
          CCTK_REAL const facts[] = {CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedArrayVariableIndex[var], 1.0,
                                srcs, tls, facts, nsrcs);
        }

  /* FIXME */

#ifdef MOLDOESCOMPLEX

        for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
        {
          int const nsrcs = 1;
          CCTK_INT const srcs[] = {RHSComplexVariableIndex[var]};
          CCTK_INT const tls[] = {0};
          CCTK_REAL const facts[] = {CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedComplexVariableIndex[var], 1.0,
                                srcs, tls, facts, nsrcs);
        }

#endif

        break;
      }
  
    case 2:
      {
        for (var = 0; var < MoLNumEvolvedVariables; var++)
        {
          int const nsrcs = 2;
          CCTK_INT const srcs[] =
            {EvolvedVariableIndex[var], RHSVariableIndex[var]};
          CCTK_INT const tls[] = {1, 0};
          CCTK_REAL const facts[] = {0.75, CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedVariableIndex[var], 0.25,
                                srcs, tls, facts, nsrcs);
        }

        for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
        {
          int const nsrcs = 2;
          CCTK_INT const srcs[] =
            {EvolvedArrayVariableIndex[var], RHSArrayVariableIndex[var]};
          CCTK_INT const tls[] = {1, 0};
          CCTK_REAL const facts[] = {0.75, CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedArrayVariableIndex[var], 0.25,
                                srcs, tls, facts, nsrcs);
        }

  /* FIXME */

#ifdef MOLDOESCOMPLEX

        for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
        {
          int const nsrcs = 2;
          CCTK_INT const srcs[] =
            {EvolvedComplexVariableIndex[var], RHSComplexVariableIndex[var]};
          CCTK_INT const tls[] = {1, 0};
          CCTK_REAL const facts[] = {0.75, CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedComplexVariableIndex[var], 0.25,
                                srcs, tls, facts, nsrcs);
        }

#endif

        break;
      }
    case 1:
      {
        for (var = 0; var < MoLNumEvolvedVariables; var++)
        {
          int const nsrcs = 2;
          CCTK_INT const srcs[] =
            {EvolvedVariableIndex[var], RHSVariableIndex[var]};
          CCTK_INT const tls[] = {1, 0};
          CCTK_REAL const facts[] = {1.0/3.0, CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedVariableIndex[var], 2.0/3.0,
                                srcs, tls, facts, nsrcs);
        }

        for (var = 0; var < MoLNumEvolvedArrayVariables; var++)
        {
          int const nsrcs = 2;
          CCTK_INT const srcs[] =
            {EvolvedArrayVariableIndex[var], RHSArrayVariableIndex[var]};
          CCTK_INT const tls[] = {1, 0};
          CCTK_REAL const facts[] = {1.0/3.0, CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedArrayVariableIndex[var], 2.0/3.0,
                                srcs, tls, facts, nsrcs);
        }

  /* FIXME */

#ifdef MOLDOESCOMPLEX

        for (var = 0; var < MoLNumEvolvedComplexVariables; var++)
        {
          int const nsrcs = 2;
          CCTK_INT const srcs[] =
            {EvolvedComplexVariableIndex[var], RHSComplexVariableIndex[var]};
          CCTK_INT const tls[] = {1, 0};
          CCTK_REAL const facts[] = {1.0/3.0, CCTK_DELTA_TIME};
          MoL_LinearCombination(cctkGH,
                                EvolvedComplexVariableIndex[var], 2.0/3.0,
                                srcs, tls, facts, nsrcs);
        }

#endif

        break;
      }
    default:
      {
        CCTK_WARN(0, "RK3 expects MoL_Intermediate_Step to be "
                  "in [1,3]. This should be caught at ParamCheck - bug Ian!");
        break;
      }
      
  }

  return;

}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
