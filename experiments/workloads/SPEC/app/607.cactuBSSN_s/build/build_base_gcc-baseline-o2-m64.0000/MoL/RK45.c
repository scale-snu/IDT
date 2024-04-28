 /*@@
   @file      RK45.c
   @date      Sun May 26 03:47:15 2002
   @author    Ian Hawke
   @desc 
   RK45 following Forsythe, Malcolm and Moler
   (Computer Methods for Mathematical Computations).
   @enddesc 
   @version   $Header$
 @@*/

#include <assert.h>

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

CCTK_FILEVERSION(CactusBase_MoL_RK45_c)

/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 ***************** Scheduled Routine Prototypes *********************
 ********************************************************************/

void MoL_RK45Add(CCTK_ARGUMENTS);

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
   @routine    MoL_RK45Add
   @date       Sun May 26 03:50:44 2002
   @author     Ian Hawke
   @desc 
   Performs a single step of a Runge-Kutta 45 type time
   integration, storing the error estimate.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 

@@*/

void MoL_RK45Add(CCTK_ARGUMENTS)
{

  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
    
  CCTK_REAL       * restrict UpdateVar;
  CCTK_REAL const * restrict RHSVar;
  CCTK_REAL       * restrict ScratchVar;
  CCTK_REAL       * restrict ErrorVar;
  CCTK_REAL const * restrict OldVar;

  CCTK_REAL beta, gamma, gamma_error;

  /* Fehlberg coefficients */
  static const CCTK_REAL beta_array_F[5][5] = {
    { 0.25,    0.0,     0.0, 0.0, 0.0 },
    { 0.09375, 0.28125, 0.0, 0.0, 0.0 },
    { 0.879380974055530268548020027310, -3.27719617660446062812926718252, 3.32089212562585343650432407829, 0.0, 0.0 },
    { 2.03240740740740740740740740741, -8.0, 7.17348927875243664717348927875, -0.205896686159844054580896686160, 0.0 },
    { -0.296296296296296296296296296296, 2.0, -1.38167641325536062378167641326, 0.452972709551656920077972709552, -0.275 }
  };

  static const CCTK_REAL gamma_array_F[6] = 
    { 0.118518518518518518518518518519, 
      0.0, 
      0.518986354775828460038986354776, 
      0.506131490342016657806131490342, 
      -0.18, 
      0.0363636363636363636363636363636
    };

  static const CCTK_REAL gammastar_array_F[6] = 
    { 0.115740740740740740740740740741, 
      0.0, 
      0.548927875243664717348927875244, 
      0.535331384015594541910331384016, 
      -0.2, 
      0.0 
    };

  /* Cash-Karp coefficients */
  static const CCTK_REAL beta_array_CK[5][5] = {
    { 1.0/5.0,        0.0,         0.0,           0.0,              0.0,      },
    { 3.0/40.0,       9.0/40.0,    0.0,           0.0,              0.0,      },
    { 3.0/10.0,       -9.0/10.0,   6.0/5.0,       0.0,              0.0,      },
    { -11.0/54.0,     5.0/2.0,     -70.0/27.0,    35.0/27.0,        0.0,      },
    { 1631.0/55296.0, 175.0/512.0, 575.0/13824.0, 44275.0/110592.0, 253.0/4096.0, },
  };

  static const CCTK_REAL gamma_array_CK[6] = {
    37.0/378.0,
    0.0,
    250.0/621.0,
    125.0/594.0,
    0.0,
    512.0/1771.0,
  };

  static const CCTK_REAL gammastar_array_CK[6] = {
    2825.0/27648.0,
    0.0,
    18575.0/48384.0,
    13525.0/55296.0,
    277.0/14336.0,
    1.0/4.0,
  };

  const CCTK_REAL (* restrict beta_array)[5];
  const CCTK_REAL * restrict gamma_array;
  const CCTK_REAL * restrict gammastar_array;

  if (CCTK_EQUALS(ODE_Method, "RK45"))
  {
    beta_array = beta_array_F;
    gamma_array = gamma_array_F;
    gammastar_array = gammastar_array_F;
  }
  else if (CCTK_EQUALS(ODE_Method, "RK45CK"))
  {
    beta_array = beta_array_CK;
    gamma_array = gamma_array_CK;
    gammastar_array = gammastar_array_CK;
  }
  else
  {
    CCTK_WARN (0, "internal error");
    /* Avoid compiler warnings */
    beta_array = NULL; 
    gammastar_array = NULL;
    gamma_array = NULL;
  }

  int totalsize = 1;
  for (int arraydim = 0; arraydim < cctk_dim; arraydim++)
  {
    totalsize *= cctk_ash[arraydim];
  }  

  /* Real GFs */

  /* First store (dt times) the rhs in the scratch array. */

  for (int var = 0; var < MoLNumEvolvedVariables; var++)
  {
    const CCTK_REAL tmp = (*Original_Delta_Time) / cctkGH->cctk_timefac;

    UpdateVar = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                              EvolvedVariableIndex[var]);
    RHSVar = (CCTK_REAL const *)CCTK_VarDataPtrI(cctkGH, 0, 
						 RHSVariableIndex[var]);
    ScratchVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                              CCTK_FirstVarIndex("MOL::SCRATCHSPACE")
                                              + var 
                                              + MoL_Num_Evolved_Vars * 
                                              (MoL_Intermediate_Steps - 
                                               (*MoL_Intermediate_Step)));
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
    for (int index = 0; index < totalsize; index++)
    {
      ScratchVar[index] = tmp * RHSVar[index];
    }
  }


  for (int var = 0; var < MoLNumEvolvedVariables; var++)
  {
    OldVar = (CCTK_REAL const *)CCTK_VarDataPtrI(cctkGH, 1, 
						 EvolvedVariableIndex[var]);
    UpdateVar = (CCTK_REAL *)CCTK_VarDataPtrI(cctkGH, 0, 
                                              EvolvedVariableIndex[var]);
    RHSVar = (CCTK_REAL const *)CCTK_VarDataPtrI(cctkGH, 0, 
						 CCTK_FirstVarIndex("MOL::SCRATCHSPACE")
						 + var 
						 + MoL_Num_Evolved_Vars * 
						 (MoL_Intermediate_Steps - 
						  (*MoL_Intermediate_Step)));
    ErrorVar =  (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                             CCTK_FirstVarIndex("MOL::ERRORESTIMATE")
                                             + var);

    if (*MoL_Intermediate_Step - 1)
    { 

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (int index = 0; index < totalsize; index++)
      {
        UpdateVar[index] = OldVar[index];
      }

      for (int scratchstep = 0; 
           scratchstep < MoL_Intermediate_Steps - (*MoL_Intermediate_Step) + 1;
           scratchstep++)
      {
        
        ScratchVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                  CCTK_FirstVarIndex("MOL::SCRATCHSPACE")
                                                  + var 
                                                  + MoL_Num_Evolved_Vars * scratchstep);
        
        beta = beta_array[MoL_Intermediate_Steps - (*MoL_Intermediate_Step)][scratchstep];
        
        if ( (beta > MoL_Tiny)||(beta < -MoL_Tiny) )
        {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
          for (int index = 0; index < totalsize; index++)
          {
            UpdateVar[index] += beta * ScratchVar[index];
          }
        }
        
      }
      
    }
    else
    {

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (int index = 0; index < totalsize; index++)
      {
        UpdateVar[index] = OldVar[index];
        ErrorVar[index] = 0;
      }
    
      for (int scratchstep = 0; scratchstep < 6; scratchstep++)
      {
        
        ScratchVar = (CCTK_REAL*)CCTK_VarDataPtrI(cctkGH, 0, 
                                                  CCTK_FirstVarIndex("MOL::SCRATCHSPACE")
                                                  + var 
                                                  + MoL_Num_Evolved_Vars * scratchstep);
        
        gamma = gamma_array[scratchstep];
        gamma_error = gamma - gammastar_array[scratchstep];
        
        if ( (gamma > MoL_Tiny)||(gamma < -MoL_Tiny) )
        {
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
          for (int index = 0; index < totalsize; index++)
          {
            UpdateVar[index] += gamma * ScratchVar[index];
            ErrorVar[index] += gamma_error * ScratchVar[index];
          }
        }
      }
      
    }
    
  }

  /* Real arrays */

  for (int var = 0; var < MoLNumEvolvedArrayVariables; var++)
  {

    CCTK_WARN(0, "Ian has been too lazy to write the RK45 routine "
              "out for array variables. Better send him an email...");

  }
        
  return;
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
