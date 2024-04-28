 /*@@
   @file      IO_Output1D.c
   @date      Tue Jan 9 1999
   @author    Gabrielle Allen
   @desc
              Functions to deal with 1D ASCII output of variables
   @enddesc
   @version   $Id: Output1D.c 207 2009-12-05 03:07:04Z schnetter $
 @@*/

#include <stdlib.h>
#include <string.h>

#ifdef SPEC 
#include "IOASCII/cctk.h" 
#define THORN_DIR "IOASCII" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOASCII/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "util_String.h"
#if defined(SPEC) && !defined(strdup)
#define strdup Util_Strdup
#endif
#include "CactusBase/IOUtil/src/ioutil_Utils.h"
#include "ioASCIIGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Id: Output1D.c 207 2009-12-05 03:07:04Z schnetter $";
CCTK_FILEVERSION(CactusBase_IOASCII_Output1D_c)


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static int CheckOutputVar (int vindex);
static void SetOutputFlag (int vindex, const char *optstring, void *arg);


/*@@
   @routine    IOASCII_Output1DGH
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Loops over all variables and outputs them if necessary
   @enddesc
   @calls      IOASCII_TimeFor1D
               IOASCII_Write1D

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
int IOASCII_Output1DGH (const cGH *GH)
{
  int vindex, retval;
  const asciiioGH *myGH;


  retval = 0;
  myGH = (const asciiioGH *) CCTK_GHExtension (GH, "IOASCII");

  /* loop over all variables */
  for (vindex = CCTK_NumVars () - 1; vindex >= 0; vindex--)
  {
    if (IOASCII_TimeFor1D (GH, vindex) &&
        IOASCII_Write1D (GH, vindex, CCTK_VarName (vindex)) == 0)
    {
      /* register variable as having 1D output this iteration */
      myGH->out1D_last[vindex] = GH->cctk_iteration;
      retval++;
    }
  }

  return (retval);
}


/*@@
   @routine    IOASCII_Output1DVarAs
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Unconditional output of a variable
               using the IOASCII output method
   @enddesc
   @calls      IOASCII_Write1D

   @var        GH
   @vdesc      Pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        fullname
   @vdesc      complete name of variable to output
   @vtype      const char *
   @vio        in
   @endvar
   @var        alias
   @vdesc      alias name of variable to output
               (used to generate output filename)
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine IOASCII_Write1D, or<BR>
               -1 if variable cannot be output by IOASCII_1D
   @endreturndesc
@@*/
int IOASCII_Output1DVarAs (const cGH *GH, const char *fullname, const char *alias)
{
  int vindex, retval;


  retval = -1;
  vindex = CCTK_VarIndex (fullname);
  if (vindex<0)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "There is no such variable %s.  Ignoring IOASCII 1D output.",
                fullname);
  } else if (CheckOutputVar (vindex))
  {
    retval = IOASCII_Write1D (GH, vindex, alias);
  }

  return (retval);
}


 /*@@
   @routine    IOASCII_TimeFor1D
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Decides if it is time to output a variable
               using the IO 1D output method
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
int IOASCII_TimeFor1D (const cGH *GH, int vindex)
{
  int retval;
  char *fullname;
  asciiioGH *myGH;


  myGH = (asciiioGH *) CCTK_GHExtension (GH, "IOASCII");
  IOASCII_CheckSteerableParameters1D (myGH);

  /* check if this variable should be output */
  retval = myGH->out1D_every[vindex] > 0 &&
           GH->cctk_iteration % myGH->out1D_every[vindex] == 0;
  if (retval)
  {
    /* check if this variable wasn't already output this iteration */
    if (myGH->out1D_last[vindex] == GH->cctk_iteration)
    {
      fullname = CCTK_FullName (vindex);
      CCTK_VWarn (5, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Already done IOASCII 1D output for '%s' in current "
                  "iteration (probably via triggers)", fullname);
      free (fullname);
      retval = 0;
    }
  }

  return (retval);
}


/*@@
   @routine    IOASCII_TriggerOutput1D
   @date       Sat March 6 1999
   @author     Gabrielle Allen
   @desc
               Triggers the output a variable
               using the IOASCII output method method
   @enddesc
   @calls      IOASCII_Write1D

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
               return code of @seeroutine IOASCII_Write1D
   @endreturndesc
@@*/
int IOASCII_TriggerOutput1D (const cGH *GH, int vindex)
{
  int retval;
  asciiioGH *myGH;


  retval = IOASCII_Write1D (GH, vindex, CCTK_VarName (vindex));
  if (retval == 0)
  {
    /* register variables as having 1D output this iteration */
    myGH = (asciiioGH *) CCTK_GHExtension (GH, "IOASCII");
    myGH->out1D_last[vindex] = GH->cctk_iteration;
  }

  return (retval);
}


/*@@
   @routine    IOASCII_CheckSteerableParameters1D
   @date       Tue 8 June 2004
   @author     Thomas Radke
   @desc
               Check if steerable IOASCII 1D parameters have changed
   @enddesc
   @calls      CCTK_TraverseString

   @var        myGH
   @vdesc      Pointer to IOASCII GH
   @vtype      asciiioGH *
   @vio        inout
   @endvar
@@*/
void IOASCII_CheckSteerableParameters1D (asciiioGH *myGH)
{
  int i, num_vars;
  char *fullname, *msg;
  DECLARE_CCTK_PARAMETERS


  /* how often to output */
  i = myGH->out1D_every_default;
  myGH->out1D_every_default = out1D_every >= 0 ? out1D_every : out_every;

  /* report if frequency changed */
  if (myGH->out1D_every_default != i && ! CCTK_Equals (verbose, "none"))
  {
    if (myGH->out1D_every_default > 0)
    {
      CCTK_VInfo (CCTK_THORNSTRING, "Periodic 1D output every %d "
                  "iterations", myGH->out1D_every_default);
    }
    else
    {
      CCTK_INFO ("Periodic 1D output turned off");
    }
  }

  /* re-parse the 'out1D_vars' parameter if it was changed */
  if (strcmp (out1D_vars, myGH->out1D_vars) || myGH->out1D_every_default != i)
  {
    num_vars = CCTK_NumVars ();
    memset (myGH->out1D_every, 0, num_vars * sizeof (CCTK_INT));
    if (CCTK_TraverseString (out1D_vars, SetOutputFlag, myGH,
                             CCTK_GROUP_OR_VAR) < 0)
    {
      CCTK_WARN (myGH->stop_on_parse_errors ? 0 : 1,
                 "error while parsing parameter 'IOASCII::out1D_vars'");
    }

    if (myGH->out1D_every_default == i || ! CCTK_Equals (verbose, "none"))
    {
      msg = NULL;
      for (i = 0; i < num_vars; i++)
      {
        if (myGH->out1D_every[i] > 0)
        {
          fullname = CCTK_FullName (i);
          if (! msg)
          {
            Util_asprintf (&msg, "Periodic 1D output requested for "
                           "'%s'", fullname);
          }
          else
          {
            char *tmp = msg;
            Util_asprintf (&msg, "%s, '%s'", msg, fullname);
            free (tmp);
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

    /* save the last setting of 'out1D_vars' parameter */
    free (myGH->out1D_vars);
    myGH->out1D_vars = strdup (out1D_vars);
  }
}


/**************************** local functions ******************************/
/* check if this variable can be output (static conditions) */
static int CheckOutputVar (int vindex)
{
  int retval;
  int grouptype;
  char *fullname;


  /* check the variable type */
  grouptype = CCTK_GroupTypeFromVarI (vindex);
  retval = grouptype == CCTK_GF || grouptype == CCTK_ARRAY;
  if (! retval)
  {
    fullname = CCTK_FullName (vindex);
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "No IOASCII 1D output for '%s': not a grid function or an "
                "array", fullname);
    free (fullname);
  }

  return (retval);
}


/* callback for CCTK_TraverseString() to set the output flag
   for the given variable */
static void SetOutputFlag (int vindex, const char *optstring, void *arg)
{
  const asciiioGH *myGH = (const asciiioGH *) arg;


  if (CheckOutputVar (vindex))
  {
    myGH->out1D_every[vindex] = myGH->out1D_every_default;

    if (optstring)
    {
      IOUtil_ParseOutputFrequency ("1D IOASCII", "IOASCII::out1D_vars",
                                   myGH->stop_on_parse_errors,
                                   vindex, optstring,
                                   &myGH->out1D_every[vindex], NULL);
    }
  }
}
