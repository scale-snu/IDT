 /*@@
   @file      OutputInfo.c
   @date      June 31 1999
   @author    Gabrielle Allen
   @desc
              Functions to deal with info output of variables
   @enddesc
   @version   $Id: OutputInfo.c 158 2004-08-12 22:11:25Z yye00 $
 @@*/


#include <math.h>
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
#include "iobasicGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusBase_IOBasic_OutputInfo_c)

/********************************************************************
 ********************    Macro Definitions    ***********************
 ********************************************************************/
/* uncomment the following to get some debugging output */
/* #define IOBASIC_DEBUG 1 */

/* the number at which to switch from decimal to exponential notation */
#define DECIMAL_SMALLEST        1.0e-8
#define DECIMAL_TOOBIG          1.0e+3
#define USE_DECIMAL_NOTATION(x) ((x) == 0.0 ||                                \
                                 (fabs ((double) x) >= DECIMAL_SMALLEST &&    \
                                  fabs ((double) x) < DECIMAL_TOOBIG))
#define PRINT_FORMATTED_REDUCTION_VALUE(reduction)                            \
          if (reduction->is_valid)                                            \
          {                                                                   \
            if (USE_DECIMAL_NOTATION (reduction->value))                      \
            {                                                                 \
              printf ("% 13.8f |", reduction->value);                         \
            }                                                                 \
            else                                                              \
            {                                                                 \
              printf ("% 11.6e |", reduction->value);                         \
            }                                                                 \
          }                                                                   \
          else                                                                \
          {                                                                   \
            printf (" ------------ |");                                       \
          }


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static void PrintHeader (iobasicGH *myGH, int num_vars);
static int TimeForOutput (const cGH *cctkGH);


static int TimeForOutput (const cGH *cctkGH)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  iobasicGH *myGH;

  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (cctkGH, "IOBasic");

  /* check if steerable parameters changed */
  IOBasic_CheckSteerableInfoParameters (myGH);

  /* how to decide when to output? */
  /* (return if no output is required) */
  if (CCTK_EQUALS(outInfo_criterion, "never"))
  {
    return 0;
  }
  else if (CCTK_EQUALS(outInfo_criterion, "iteration"))
  {
    if (myGH->outInfo_every <= 0 || cctk_iteration % myGH->outInfo_every)
    {
      return 0;
    }
  }
  else if (CCTK_EQUALS(outInfo_criterion, "time"))
  {
    if (myGH->outInfo_dt < 0)
    {
      return 0;
    }
    if (myGH->outInfo_dt > 0)
    {
      static int output_iteration = -1;
      static int output_this_iteration;
      if(! (cctk_iteration >= output_iteration))
        CCTK_VWarn ( 0, __LINE__, __FILE__, "IOBasic", "cctk_iteration out of bounds");
      if (cctk_iteration > output_iteration)
      {
        output_iteration = cctk_iteration;
        output_this_iteration
          = cctk_time >= *next_info_output_time - 1.0e-12 * cctk_delta_time;
        if (output_this_iteration)
        {
          *next_info_output_time += myGH->outInfo_dt;
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
      CCTK_VWarn ( 0, __LINE__, __FILE__, "IOBasic", "outInfo_criterion out of bounds");
  }

  return 1;
}


 /*@@
   @routine    IOBasic_InfoOutputGH
   @date       June 31 1999
   @author     Gabrielle Allen
   @desc
               Loops over all variables and prints output if requested
   @enddesc
   @calls      IOBasic_CheckSteerableInfoParameters
               PrintHeader
               IOBasic_WriteInfo

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
int IOBasic_InfoOutputGH (const cGH *GH)
{
  int vindex, num_vars, retval;
  iobasic_reduction_t *reduction;
  iobasicGH  *myGH;


  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  /* check if steerable parameters changed */
  IOBasic_CheckSteerableInfoParameters (myGH);

  if (! TimeForOutput(GH)) return 0;

  retval = 0;

  /* print header if neccessary */
  num_vars = CCTK_NumVars ();
  if (myGH->info_reductions_changed)
  {
    PrintHeader (myGH, num_vars);
  }

  /* print the iteration/timestep information for all variables */
  if (USE_DECIMAL_NOTATION (GH->cctk_time))
  {
    printf ("%6d |%9.3f |", GH->cctk_iteration, GH->cctk_time);
  }
  else
  {
    printf ("%6d |%7.3e |", GH->cctk_iteration, GH->cctk_time);
  }

  /* loop over all variables */
  for (vindex = 0; vindex < num_vars; vindex++)
  {
    /* check this variable should be output */
    if (myGH->info_reductions[vindex].num_reductions == 0)
    {
      continue;
    }

    /* check variable not already output this iteration */
    if (myGH->outInfo_last[vindex] != GH->cctk_iteration)
    {
#ifdef IOBASIC_DEBUG
      printf("\nIn IO OutputInfoGH\n----------------\n");
      printf("  Index = %d\n", vindex);
      printf("  Variable = -%s-\n", CCTK_VarName (vindex);
      printf("  Last output iteration was = %d\n", myGH->outInfo_last[vindex]);
#endif

      /* get the data to output */
      if (IOBasic_WriteInfo (GH, vindex) == 0)
      {
        /* register variable as having info output this iteration */
        myGH->outInfo_last[vindex] = GH->cctk_iteration;
        retval++;
      }
    }

    /* finally print the stuff to screen */
    reduction = myGH->info_reductions[vindex].reductions;
    while (reduction)
    {
      PRINT_FORMATTED_REDUCTION_VALUE (reduction);
      reduction = reduction->next;
    }
  } /* end of loop over all variables */

  /* add the new line */
  putchar ('\n');
  fflush (stdout);

  return (retval);
}


 /*@@
   @routine    IOBasic_TimeForInfoOutput
   @date       June 31 1999
   @author     Gabrielle Allen
   @desc
               Decides if it is time to output a variable using info output
   @enddesc
   @calls      IOBasic_CheckSteerableInfoParameters

   @var        GH
   @vdesc      Pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        vindex
   @vdesc      index of variable to check for output
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               true/false (1 or 0) if this variable should be output or not
   @endreturndesc
@@*/
int IOBasic_TimeForInfoOutput (const cGH *GH, int vindex)
{
  char *fullname;
  iobasicGH *myGH;


  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  /* check if steerable parameters changed */
  IOBasic_CheckSteerableInfoParameters (myGH);

  if (myGH->info_reductions[vindex].num_reductions == 0)
  {
    return 0;
  }

  if (! TimeForOutput(GH)) return 0;

  /* check if variable was not already output this iteration */
  if (myGH->outInfo_last[vindex] == GH->cctk_iteration)
  {
    fullname = CCTK_FullName (vindex);
    CCTK_VWarn (5, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Already done Info output for '%s' in current iteration "
                "(probably via triggers)", fullname);
    free (fullname);
    return 0;
  }

  return 1;
}


 /*@@
   @routine    IOBasic_TriggerInfoOutput
   @date       June 31 1999
   @author     Gabrielle Allen
   @desc
               Triggers the output of a variable using IOBasic's info output
   @enddesc
   @calls      IOBasic_WriteInfo

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
               return code of @seeroutine IOBasic_WriteInfo
   @endreturndesc
@@*/
int IOBasic_TriggerInfoOutput (const cGH *GH, int vindex)
{
  int retval;
  iobasicGH *myGH;


  /* get the GH extension for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

#ifdef IOBASIC_DEBUG
  printf ("\nIn IO TriggerOutputInfo\n---------------------\n");
  printf ("  Index = %d\n", vindex);
  printf ("  Variable = -%s-\n", CCTK_VarName (vindex);
#endif

  /* get the data values to output */
  retval = IOBasic_WriteInfo (GH, vindex);

  if (retval == 0)
  {
    /* register variable as having Info output at this iteration */
    myGH->outInfo_last[vindex] = GH->cctk_iteration;
  }

  return (retval);
}


 /*@@
   @routine    IOBasic_CheckSteerableInfoParameters
   @date       Tue 31 Jul 2001
   @author     Thomas Radke
   @desc
               Re-evaluates 'IOBasic::outInfo_every' and/or 'IO::out_every'
               resp. to set myGH->outInfo_every to the frequency of info output.
               Re-evaluates 'IOBasic::outInfo_vars' and
               'IOBasic::outInfo_reductions' and sets
               myGH->info_reductions_changed to true if one of them was changed.
   @enddesc
   @calls      CCTK_ParameterQueryTimesSet

   @var        myGH
   @vdesc      Pointer to IOBasic's GH extension
   @vtype      iobasicGH *
   @vio        in
   @endvar
@@*/
void IOBasic_CheckSteerableInfoParameters (iobasicGH *myGH)
{
  int vindex, out_old, times_set, update_reductions_list;
  CCTK_REAL outdt_old;
  iobasic_reduction_t *reduction, *next;
  static int outInfo_vars_lastset       = -1;
  static int outInfo_reductions_lastset = -1;
  iobasic_parseinfo_t info;
  DECLARE_CCTK_PARAMETERS


  if (CCTK_EQUALS(outInfo_criterion, "never"))
  {
    return ;
  }
  else if (CCTK_EQUALS(outInfo_criterion, "iteration"))
  {
    /* how often to output */
    out_old = myGH->outInfo_every;
    myGH->outInfo_every = outInfo_every >= 0 ? outInfo_every : out_every;

    /* report if frequency changed */
    if (myGH->outInfo_every != out_old && ! CCTK_Equals (verbose, "none"))
    {
      if (myGH->outInfo_every > 0)
      {
        CCTK_VInfo (CCTK_THORNSTRING, "Periodic info output every %d iterations",
                    myGH->outInfo_every);
      }
      else
      {
        CCTK_INFO ("Periodic info output turned off");
      }
    }

    /* return if there's nothing to do */
    if (myGH->outInfo_every <= 0)
    {
      return;
    }
  }
  else if (CCTK_EQUALS(outInfo_criterion, "time"))
  {
    /* how often to output */
    outdt_old = myGH->outInfo_dt;
    myGH->outInfo_dt = outInfo_dt >= 0 ? outInfo_dt : out_dt;

    /* report if frequency changed */
    if (myGH->outInfo_dt != outdt_old && ! CCTK_Equals (verbose, "none"))
    {
      if (myGH->outInfo_dt >= 0)
      {
        CCTK_VInfo (CCTK_THORNSTRING, "Periodic info output dt %g",
                    (double)myGH->outInfo_dt);
      }
      else
      {
        CCTK_INFO ("Periodic info output turned off");
      }
    }

    /* return if there's nothing to do */
    if (myGH->outInfo_dt < 0)
    {
      return;
    }
  }
  else
  {
    if(! (0))
      CCTK_VWarn ( 0, __LINE__, __FILE__, "IOBasic", "outInfo_criterion out of bounds");
  }

  /* check if the 'outInfo_reductions' parameter if it was changed */
  times_set = CCTK_ParameterQueryTimesSet ("outInfo_reductions",
                                           CCTK_THORNSTRING);
  update_reductions_list = times_set != outInfo_reductions_lastset;
  outInfo_reductions_lastset = times_set;

  /* check if the 'outInfo_vars' parameter if it was changed */
  times_set = CCTK_ParameterQueryTimesSet ("outInfo_vars", CCTK_THORNSTRING);
  update_reductions_list |= times_set != outInfo_vars_lastset;
  outInfo_vars_lastset = times_set;

  if (update_reductions_list)
  {
    /* free old info output lists ... */
    for (vindex = CCTK_NumVars ()-1; vindex >= 0; vindex--)
    {
      if (myGH->info_reductions[vindex].num_reductions > 0)
      {
        myGH->info_reductions[vindex].num_reductions = 0;
        reduction = myGH->info_reductions[vindex].reductions;
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
    info.reduction_list = myGH->info_reductions;
    info.reductions_string = outInfo_reductions;
    if (CCTK_TraverseString (outInfo_vars, IOBasic_AssignReductionList, &info,
                             CCTK_GROUP_OR_VAR) < 0)
    {
      CCTK_WARN (myGH->stop_on_parse_errors ? 0 : 1,
                 "error while parsing parameter 'IOBasic::outInfo_vars'");
    }

    myGH->info_reductions_changed = 1;
  }
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
 /*@@
   @routine    PrintHeader
   @date       Tue 31 Jul 2001
   @author     Thomas Radke
   @desc
               It re-evaluates the info reduction lists of all CCTK variables
               and prints the header for the info output table.
   @enddesc
   @calls      CCTK_TraverseString

   @var        myGH
   @vdesc      Pointer to IOBasic's GH extension
   @vtype      iobasicGH *
   @vio        in
   @endvar
   @var        num_vars
   @vdesc      total number of CCTK variables
   @vtype      int
   @vio        in
   @endvar
@@*/
static void PrintHeader (iobasicGH *myGH, int num_vars)
{
  DECLARE_CCTK_PARAMETERS
  int i, num_columns, vindex;
  char *fullname, *msg, *oldmsg;
  iobasic_reduction_t *reduction;


  /* count number of info values to output */
  num_columns = 0;
  msg = NULL;
  for (vindex = 0; vindex < num_vars; vindex++)
  {
    num_columns += myGH->info_reductions[vindex].num_reductions;

    if (myGH->info_reductions[vindex].num_reductions > 0 &&
        ! CCTK_Equals (verbose, "none"))
    {
      fullname = CCTK_FullName (vindex);
      if (! msg)
      {
        Util_asprintf (&msg, "Periodic info output requested for '%s'",
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

  /* draw a horizontal delimiter line */
  printf ("-------------------");
  for (i = 0; i < num_columns; i++)
  {
    printf ("---------------");
  }
  putchar ('\n');

  /* the first header line displays the iteration number (first column)
     and the full names of the variables to output (third column ff.) */
  printf ("   it  |          |");
  if (num_columns > 0)
  {
    for (vindex = 0; vindex < num_vars; vindex++)
    {
      i = myGH->info_reductions[vindex].num_reductions;
      if (i > 0)
      {
        fullname = CCTK_FullName (vindex);
        if (strlen (fullname) > (unsigned int) 15*i - 3)
        {
          printf (" *%s |", fullname + strlen (fullname) - (15*i - 4));
        }
        else
        {
          printf (" %-*s |", 15*i - 3, fullname);
        }
        free (fullname);
      }
    }
  }
  putchar ('\n');

  /* the second header line displays the physical time (second column)
     and the names of the reduction operators to apply (third column ff.) */
  printf ("       |    t     ");
  for (vindex = 0; vindex < num_vars; vindex++)
  {
    if (myGH->info_reductions[vindex].num_reductions > 0)
    {
      reduction = myGH->info_reductions[vindex].reductions;
      while (reduction)
      {
        printf ("| %-12s ", reduction->name);
        reduction = reduction->next;
      }
    }
  }
  printf ("|\n");

  /* finally draw another horizontal delimiter line */
  printf ("-------------------");
  for (i = 0; i < num_columns; i++)
  {
    printf ("---------------");
  }
  putchar ('\n');
  fflush (stdout);

  myGH->info_reductions_changed = 0;
}
