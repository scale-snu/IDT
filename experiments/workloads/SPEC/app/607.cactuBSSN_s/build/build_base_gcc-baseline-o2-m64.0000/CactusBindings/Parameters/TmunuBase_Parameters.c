/*@@
   @file    TmunuBase_Parameters.c
   @author  Automatically generated by CreateParameterBindings.pl
   @desc
            Creates/extends parameters for this thorn
   @enddesc
 @@*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cctk_Config.h"
#include "cctk_Constants.h"
#include "ParameterBindings.h"
#include "CParameterStructNames.h"

/* structure containing all private parameters of thorn TmunuBase */
struct
{
  const char * prolongation_type;
  CCTK_INT stress_energy_storage;
  CCTK_INT timelevels;
} PRIVATE_TMUNUBASE_STRUCT;


/* structure containing all restricted parameters of thorn TmunuBase */
struct
{
  CCTK_INT stress_energy_at_RHS;
  CCTK_INT support_old_CalcTmunu_mechanism;
} RESTRICTED_TMUNUBASE_STRUCT;


int CCTKi_BindingsCreateTmunuBaseParameters(void);
int CCTKi_BindingsCreateTmunuBaseParameters(void)
{
  CCTKi_ParameterCreate("stress_energy_at_RHS",
                        "TmunuBase",
                        "BOOLEAN",
                        "RESTRICTED",
                        CCTK_STEERABLE_NEVER,
                        "Should the stress-energy tensor be calculated for the RHS evaluation?",
                        "no",
                        &(RESTRICTED_TMUNUBASE_STRUCT.stress_energy_at_RHS),
                        0,
                        NULL,
                        0);

  CCTKi_ParameterCreate("support_old_CalcTmunu_mechanism",
                        "TmunuBase",
                        "BOOLEAN",
                        "RESTRICTED",
                        CCTK_STEERABLE_NEVER,
                        "Should the old CalcTmunu.inc mechanism be supported? This is deprecated.",
                        "no",
                        &(RESTRICTED_TMUNUBASE_STRUCT.support_old_CalcTmunu_mechanism),
                        0,
                        NULL,
                        0);

  CCTKi_ParameterCreate("prolongation_type",
                        "TmunuBase",
                        "STRING",
                        "PRIVATE",
                        CCTK_STEERABLE_NEVER,
                        "The kind of boundary prolongation for the stress-energy tensor",
                        "Lagrange",
                        &(PRIVATE_TMUNUBASE_STRUCT.prolongation_type),
                        0,
                        NULL,
                        3,
                        "^Lagrange$", "standard prolongation (requires several time levels)",
                        "^none$", "no prolongation (use this if you do not have enough time levels active)",
                        "", "any other supported prolongation type");

  CCTKi_ParameterCreate("stress_energy_storage",
                        "TmunuBase",
                        "BOOLEAN",
                        "PRIVATE",
                        CCTK_STEERABLE_NEVER,
                        "Should the stress-energy tensor have storage?",
                        "no",
                        &(PRIVATE_TMUNUBASE_STRUCT.stress_energy_storage),
                        0,
                        NULL,
                        0);

  CCTKi_ParameterCreate("timelevels",
                        "TmunuBase",
                        "INT",
                        "PRIVATE",
                        CCTK_STEERABLE_RECOVER,
                        "Number of time levels",
                        "1",
                        &(PRIVATE_TMUNUBASE_STRUCT.timelevels),
                        0,
                        NULL,
                        1,
                        "0:3", "");

  return 0;
}

int CCTKi_BindingsTmunuBaseParameterExtensions(void);
int CCTKi_BindingsTmunuBaseParameterExtensions(void)
{
  return 0;
}
