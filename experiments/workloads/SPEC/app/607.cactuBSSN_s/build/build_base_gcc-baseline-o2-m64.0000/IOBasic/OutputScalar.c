 /*@@
   @file      Output.c
   @date      Mon 21 September
   @author    Gabrielle Allen
   @desc
              Functions to deal with scalar output of grid variables
   @enddesc
 @@*/

/* #define IOBASIC_DEBUG 1 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SPEC 
#include "IOBasic/cctk.h" 
#define THORN_DIR "IOBasic" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOBasic/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOBasic/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "util_String.h"
#if defined(SPEC) && !defined(strdup)
#define strdup Util_Strdup
#endif
#include "iobasicGH.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_IOBasic_OutputScalar_c)


/********************************************************************
 ********************    Static Variables    ************************
 ********************************************************************/


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static int TimeForOutput (const cGH *cctkGH);


/********************************************************************
 ********************    External Routines   ************************
 ********************************************************************/
/*@@
   @routine    IOBasic_ScalarOutputGH
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Loops over all variables and outputs them if necessary
   @enddesc
   @calls      IOBasic_TimeForScalarOutput
               IOBasic_WriteScalar

   @var        GH
   @vdesc      Pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               the number of variables which were output at this iteration
               (or 0 if it wasn't time to output yet)
   @endreturndesc
@@*/
int IOBasic_ScalarOutputGH (const cGH *GH)
{
  int vindex, retval;
  iobasicGH *myGH;


  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  /* check if steerable parameters changed */
  IOBasic_CheckSteerableScalarParameters (myGH);

  if (! TimeForOutput(GH)) return 0;

  retval = 0;

  /* loop over all variables */
  for (vindex = CCTK_NumVars () - 1; vindex >= 0; vindex--)
  {
    if (IOBasic_TimeForScalarOutput (GH, vindex) &&
        IOBasic_WriteScalar (GH, vindex, CCTK_VarName (vindex)) == 0)
    {
      /* register variable as having output this iteration */
      myGH->outScalar_last[vindex] = GH->cctk_iteration;
      retval++;
    }
  }

  return (retval);
}


/*@@
   @routine    IOBasic_OutputVarAs
   @date       Sun 8 Mar 2003
   @author     Thomas Radke
   @desc
               Unconditional output of a variable using the "Scalar" I/O method
   @enddesc
   @calls      IOBasic_WriteScalar

   @var        GH
   @vdesc      pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        fullname
   @vdesc      complete name of variable to output, maybe appended by an
               options string
   @vtype      const char *
   @vio        in
   @endvar
   @var        alias
   @vdesc      variable's alias name used to build the output filename
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine IOBasic_WriteScalar, or<BR>
               -1 if variable cannot be output by "Scalar"
   @endreturndesc
@@*/
int IOBasic_OutputVarAs (const cGH *GH, const char *fullname, const char *alias)
{
  int vindex, old_num_reductions, retval;
  char *varname, *tmp;
  iobasic_parseinfo_t info;
  iobasic_reduction_t *reduction, *old_reductions, *next;
  iobasicGH *myGH;
  DECLARE_CCTK_PARAMETERS


  /* get the variable index from 'fullname' */
  varname = strdup (fullname);
  tmp = strchr (fullname, '[');
  if (tmp)
  {
    varname[tmp - fullname] = 0;
  }

  vindex = CCTK_VarIndex (varname);
  free (varname);
  if (vindex < -1)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOBasic_OutputVarAs: invalid variable name in 'fullname' "
                "argument '%s'", fullname);
    return (-1);
  }

  /* save the old scalar_reductions[vindex] info ... */
  myGH = CCTK_GHExtension (GH, "IOBasic");
  old_num_reductions = myGH->scalar_reductions[vindex].num_reductions;
  old_reductions = myGH->scalar_reductions[vindex].reductions;

  /* ... and create a temporary one from 'fullname' */
  retval = 0;
  info.reduction_list = myGH->scalar_reductions;
  info.reductions_string = outScalar_reductions;
  if (CCTK_TraverseString (fullname, IOBasic_AssignReductionList, &info,
                           CCTK_VAR) < 0)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOBasic_OutputVarAs: failed to parse 'fullname' argument '%s'",
                fullname);
    retval = -1;
  }
  else if (myGH->scalar_reductions[vindex].num_reductions > 0)
  {
    /* do the actual output */
    retval = IOBasic_WriteScalar (GH, vindex, alias);

    /* free temporary scalar_reductions[vindex] info and restore the old one */
    reduction = myGH->scalar_reductions[vindex].reductions;
    while (reduction)
    {
      next = reduction->next;
      free (reduction->name);
      free (reduction);
      reduction = next;
    }
  }
  myGH->scalar_reductions[vindex].num_reductions = old_num_reductions;
  myGH->scalar_reductions[vindex].reductions = old_reductions;

  return (retval);
}


 /*@@
   @routine    IOBasic_TimeForScalarOutput
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Decides if it is time to do Scalar output.
   @enddesc

   @var        GH
   @vdesc      Pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        vindex
   @vdesc      index of variable
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               1 if output should take place at this iteration, or<BR>
               0 if not
   @endreturndesc
@@*/
int IOBasic_TimeForScalarOutput (const cGH *GH, int vindex)
{
  char *fullname;
  iobasicGH *myGH;


  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  /* check if steerable parameters changed */
  IOBasic_CheckSteerableScalarParameters (myGH);

  if (myGH->scalar_reductions[vindex].num_reductions == 0)
  {
    return 0;
  }

  if (! TimeForOutput(GH))
  {
    return 0;
  }

  /* Check if variable wasn't already output this iteration */
  if (myGH->outScalar_last[vindex] == GH->cctk_iteration)
  {
    fullname = CCTK_FullName (vindex);
    CCTK_VWarn (5, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Already done scalar output for '%s' in current iteration "
                "(probably via triggers)", fullname);
    free (fullname);
    return 0;
  }

  return 1;
}


/*@@
   @routine    IOBasic_TriggerScalarOutput
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Triggers the output of a variable using IOBasic's Scalar
               output method
   @enddesc

   @var        GH
   @vdesc      Pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        vindex
   @vdesc      index of variable to output
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine IOBasic_WriteScalar
   @endreturndesc
@@*/
int IOBasic_TriggerScalarOutput (const cGH *GH, int vindex)
{
  int retval;
  const char *name;
  iobasicGH *myGH;


  /* Get the GH extension for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  name = CCTK_VarName (vindex);

#ifdef IOBASIC_DEBUG
  printf ("\nIn IOBasic_TriggerScalarOutput\n---------------------\n");
  printf ("  Index = %d\n", vindex);
  printf ("  Variable = -%s-\n", name);
#endif

  /* do the Scalar output */
  retval = IOBasic_WriteScalar (GH, vindex, name);
  if (retval == 0)
  {
    /* register variable as having Scalar output this iteration */
    myGH->outScalar_last[vindex] = GH->cctk_iteration;
  }

  return (retval);
}


 /*@@
   @routine    IOBasic_CheckSteerableScalarParameters
   @date       Tue 31 Jul 2001
   @author     Thomas Radke
   @desc
               Re-evaluates 'IOBasic::outScalar_every' and/or 'IO::out_every'
               resp. to set myGH->outScalar_every to the frequency of scalar
               output. Re-evaluates 'IOBasic::outScalar_vars' and
               'IOBasic::outScalar_reductions'.
   @enddesc
   @calls      CCTK_ParameterQueryTimesSet

   @var        myGH
   @vdesc      Pointer to IOBasic's GH extension
   @vtype      iobasicGH *
   @vio        in
   @endvar
@@*/
void IOBasic_CheckSteerableScalarParameters (iobasicGH *myGH)
{
  int vindex, out_old, times_set, update_reductions_list, num_vars;
  CCTK_REAL outdt_old;
  iobasic_reduction_t *reduction, *next;
  static int outScalar_vars_lastset       = -1;
  static int outScalar_reductions_lastset = -1;
  iobasic_parseinfo_t info;
  char *msg, *oldmsg, *fullname;
  DECLARE_CCTK_PARAMETERS


  if (CCTK_EQUALS(outScalar_criterion, "never"))
  {
    return ;
  }
  else if (CCTK_EQUALS(outScalar_criterion, "iteration"))
  {
    /* how often to output */
    out_old = myGH->outScalar_every;
    myGH->outScalar_every = outScalar_every >= 0 ? outScalar_every : out_every;

    /* report if frequency changed */
    if (myGH->outScalar_every != out_old && ! CCTK_Equals (verbose, "none"))
    {
      if (myGH->outScalar_every > 0)
      {
        CCTK_VInfo (CCTK_THORNSTRING, "Periodic scalar output every %d iterations",
                    myGH->outScalar_every);
      }
      else
      {
        CCTK_INFO ("Periodic scalar output turned off");
      }
    }

    /* return if there's nothing to do */
    if (myGH->outScalar_every <= 0)
    {
      return;
    }
  }
  else if (CCTK_EQUALS(outScalar_criterion, "time"))
  {
    /* how often to output */
    outdt_old = myGH->outScalar_dt;
    myGH->outScalar_dt = outScalar_dt >= 0 ? outScalar_dt : out_dt;

    /* report if frequency changed */
    if (myGH->outScalar_dt != outdt_old && ! CCTK_Equals (verbose, "none"))
    {
      if (myGH->outScalar_dt >= 0)
      {
        CCTK_VInfo (CCTK_THORNSTRING, "Periodic scalar output dt %g",
                    (double)myGH->outScalar_dt);
      }
      else
      {
        CCTK_INFO ("Periodic scalar output turned off");
      }
    }

    /* return if there's nothing to do */
    if (myGH->outScalar_dt < 0)
    {
      return;
    }
  }
  else
  {
    if(! (0))
      CCTK_VWarn ( 0, __LINE__, __FILE__, "IOBasic", "outScalar_criterion out of bounds");
  }

  /* check if the 'outScalar_reductions' parameter if it was changed */
  times_set = CCTK_ParameterQueryTimesSet ("outScalar_reductions",
                                           CCTK_THORNSTRING);
  update_reductions_list = times_set != outScalar_reductions_lastset;
  outScalar_reductions_lastset = times_set;

  /* check if the 'outScalar_vars' parameter if it was changed */
  times_set = CCTK_ParameterQueryTimesSet ("outScalar_vars", CCTK_THORNSTRING);
  update_reductions_list |= times_set != outScalar_vars_lastset;
  outScalar_vars_lastset = times_set;

  if (update_reductions_list)
  {
    /* free old reduction lists ... */
    num_vars = CCTK_NumVars ();
    for (vindex = 0; vindex < num_vars; vindex++)
    {
      if (myGH->scalar_reductions[vindex].num_reductions > 0)
      {
        myGH->scalar_reductions[vindex].num_reductions = 0;
        reduction = myGH->scalar_reductions[vindex].reductions;
        while (reduction)
        {
          next = reduction->next;
          free (reduction->name);
          free (reduction);
          reduction = next;
        }
      }
    }

    /* ... and create new ones */
    info.reduction_list = myGH->scalar_reductions;
    info.reductions_string = outScalar_reductions;
    if (CCTK_TraverseString (outScalar_vars, IOBasic_AssignReductionList, &info,
                             CCTK_GROUP_OR_VAR) < 0)
    {
      CCTK_WARN (myGH->stop_on_parse_errors ? 0 : 1,
                 "error while parsing parameter 'IOBasic::outScalar_vars'");
    }
    else if (! CCTK_Equals (verbose, "none"))
    {
      msg = NULL;
      for (vindex = 0; vindex < num_vars; vindex++)
      {
        if (myGH->scalar_reductions[vindex].num_reductions)
        {
          fullname = CCTK_FullName (vindex);
          if (! msg)
          {
            Util_asprintf (&msg, "Periodic scalar output requested for '%s'",
                           fullname);
          }
          else
          {
            oldmsg = msg;
            Util_asprintf (&msg, "%s, '%s'", oldmsg, fullname);
            free (oldmsg);
          }
          free (fullname);
        }
      }
      if (msg)
      {
        CCTK_INFO (msg);
        free (msg);
      }
    }
  }
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static int TimeForOutput (const cGH *cctkGH)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  iobasicGH *myGH;


  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (cctkGH, "IOBasic");

  /* check if steerable parameters changed */
  IOBasic_CheckSteerableScalarParameters (myGH);

  /* how to decide when to output? */
  /* (return if no output is required) */
  if (CCTK_EQUALS(outScalar_criterion, "never"))
  {
    return 0;
  }
  else if (CCTK_EQUALS(outScalar_criterion, "iteration"))
  {
    if (myGH->outScalar_every <= 0 || cctk_iteration % myGH->outScalar_every)
    {
      return 0;
    }
  }
  else if (CCTK_EQUALS(outScalar_criterion, "time"))
  {
    if (myGH->outScalar_dt < 0)
    {
      return 0;
    }
    if (myGH->outScalar_dt > 0)
    {
      static int output_iteration = -1;
      static int output_this_iteration;
      if(! (cctk_iteration >= output_iteration))
        CCTK_VWarn ( 0, __LINE__, __FILE__, "IOBasic", "cctk_iteration out of bounds");
      if (cctk_iteration > output_iteration)
      {
        output_iteration = cctk_iteration;
        output_this_iteration
          = cctk_time >= *next_scalar_output_time - 1.0e-12 * cctk_delta_time;
        if (output_this_iteration)
        {
          *next_scalar_output_time += myGH->outScalar_dt;
        }
      }
      if (! output_this_iteration)
      {
        return 0;
      }
    }
  }
  else
  {
    if(! (0))
      CCTK_VWarn ( 0, __LINE__, __FILE__, "IOBasic", "outScalar_criterion out of bounds");
  }

  return 1;
}
