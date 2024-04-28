 /*@@
   @file      Startup.c
   @date      Friday 18th September 1999
   @author    Gabrielle Allen
   @desc
              Startup routines for IOBasic.
   @enddesc
   @version   $Id: Startup.c 184 2011-01-20 22:04:26Z eschnett $
 @@*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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
#include "util_Table.h"
#if defined(SPEC) && !defined(strdup)
#include "util_String.h"
#define strdup Util_Strdup
#endif
#include "CactusBase/IOUtil/src/ioutil_Utils.h"
#include "iobasicGH.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_IOBasic_Startup_c)


/********************************************************************
 ********************    External Routines   ************************
 ********************************************************************/
int IOBasic_Startup (void);
void IOBasic_Init (CCTK_ARGUMENTS);


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static void *IOBasic_SetupGH (tFleshConfig *config, int conv_level, cGH *GH);


 /*@@
   @routine   IOBasic_Startup
   @date      Friday 18th September 1999
   @author    Gabrielle Allen
   @desc
              The startup registration routine for IOBasic.
              Registers the GH extensions needed for IOBasic
              along with its setup routine.
   @enddesc
   @calls     CCTK_RegisterGHExtensionSetupGH
@@*/
int IOBasic_Startup (void)
{
  if (CCTK_GHExtensionHandle ("IO") < 0)
  {
    CCTK_WARN (1, "Thorn IOUtil was not activated. "
                  "No IOBasic I/O methods will be enabled.");
    return 0;
  }

  CCTK_RegisterGHExtensionSetupGH (CCTK_RegisterGHExtension ("IOBasic"),
                                   IOBasic_SetupGH);

  return 0;
}


 /*@@
   @routine   IOBasic_Init
   @date      Tue 30 Dec 2003
   @author    Erik Schnetter
   @desc
              Initialise the next_*_output_time variables.
   @enddesc
@@*/
void IOBasic_Init (CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  *next_info_output_time = cctk_time;
  *next_scalar_output_time = cctk_time;
}


/****************************************************************************/
/*                           local routines                                 */
/****************************************************************************/
 /*@@
   @routine   IOBasic_SetupGH
   @date      Sat Feb 6 1999
   @author    Gabrielle Allen
   @desc
              Allocates and sets up IOBasic's GH extension structure
   @enddesc

   @calls     CCTK_RegisterIOMethod
              CCTK_RegisterIOMethodOutputGH
              CCTK_RegisterIOMethodOutputVarAs
              CCTK_RegisterIOMethodTimeToOutput
              CCTK_RegisterIOMethodTriggerOutput

   @var       config
   @vdesc     the CCTK configuration as provided by the flesh
   @vtype     tFleshConfig *
   @vio       unused
   @endvar
   @var       conv_level
   @vdesc     the convergence level
   @vtype     int
   @vio       unused
   @endvar
   @var       GH
   @vdesc     Pointer to CCTK grid hierarchy
   @vtype     cGH *
   @vio       in
   @endvar

   @returntype  void *
   @returndesc
                pointer to the allocated GH extension structure
   @endreturndesc
@@*/
static void *IOBasic_SetupGH (tFleshConfig *config, int conv_level, cGH *GH)
{
  int i;
  iobasicGH *myGH;
  const char *my_out_dir;
  DECLARE_CCTK_PARAMETERS


  /* suppress compiler warnings about unused parameters */
  (void) (config + 0);
  (void) (conv_level + 0);
  (void) (GH + 0);

  /* allocate the GH extension and its components */
  myGH = malloc (sizeof (iobasicGH));
  if (! myGH)
  {
    return (NULL);
  }

  /* Register the IOBasic routines as output methods  */
  i = CCTK_RegisterIOMethod ("Scalar");
  CCTK_RegisterIOMethodOutputGH (i, IOBasic_ScalarOutputGH);
  CCTK_RegisterIOMethodOutputVarAs (i, IOBasic_OutputVarAs);
  CCTK_RegisterIOMethodTimeToOutput (i, IOBasic_TimeForScalarOutput);
  CCTK_RegisterIOMethodTriggerOutput (i, IOBasic_TriggerScalarOutput);

  i = CCTK_RegisterIOMethod ("Info");
  CCTK_RegisterIOMethodOutputGH (i, IOBasic_InfoOutputGH);
  CCTK_RegisterIOMethodTimeToOutput (i, IOBasic_TimeForInfoOutput);
  CCTK_RegisterIOMethodTriggerOutput (i, IOBasic_TriggerInfoOutput);

  i = CCTK_NumVars ();

  myGH->info_reductions = calloc (2 * i, sizeof (iobasic_reductionlist_t));
  myGH->scalar_reductions = myGH->info_reductions + i;
  myGH->outInfo_last   = malloc (2 * i * sizeof (int));
  myGH->outScalar_last = myGH->outInfo_last + i;

  /* initialize counters for how often to do output with a value different
     to the current parameter values
     this forces info output about periodic output */
  myGH->outInfo_every = outInfo_every >= 0 ? outInfo_every : out_every;
  myGH->outScalar_every = outScalar_every >= 0 ? outScalar_every : out_every;
  myGH->outInfo_dt = outInfo_dt >= 0 ? outInfo_dt : out_dt;
  myGH->outScalar_dt = outScalar_dt >= 0 ? outScalar_dt : out_dt;

  memset (myGH->outInfo_last, -1, 2 * i * sizeof (int));

  myGH->filenameListScalar = NULL;

  myGH->stop_on_parse_errors = strict_io_parameter_check;
  if (! CCTK_Equals (verbose, "none"))
  {
    CCTK_INFO ("I/O Method 'Scalar' registered: output of scalar quantities "
               "(grid scalars, reductions) to ASCII files");
  }
  IOBasic_CheckSteerableInfoParameters (myGH);
  if (! CCTK_Equals (verbose, "none"))
  {
    CCTK_INFO ("I/O Method 'Info' registered: output of scalar quantities "
               "(grid scalars, reductions) to screen");
  }
  IOBasic_CheckSteerableInfoParameters (myGH);
  myGH->stop_on_parse_errors = 0;

  /* get the name of IOBasic's output directory */
  my_out_dir = out_dir;
  if (*my_out_dir == 0)
  {
    my_out_dir = io_out_dir;
  }

  /* skip the directory pathname if output goes into current directory */
  if (strcmp (my_out_dir, "."))
  {
    i = strlen (my_out_dir);
    myGH->out_dir = malloc (i + 2);
    strcpy (myGH->out_dir, my_out_dir);
    myGH->out_dir[i] = '/';
    myGH->out_dir[i+1] = 0;
  }
  else
  {
    myGH->out_dir = strdup ("");
  }

  /* create the output dir */
  if (*myGH->out_dir && CCTK_MyProc (GH) == 0)
  {
    i = IOUtil_CreateDirectory (GH, myGH->out_dir, 0, 0);
    if (i < 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOBasic_SetupGH: Couldn't create Scalar output directory "
                  "'%s'", myGH->out_dir);
    }
    else if (i >= 0 && CCTK_Equals (verbose, "full"))
    {
      CCTK_VInfo (CCTK_THORNSTRING, "Scalar: Output to directory '%s'",
                  myGH->out_dir);
    }
  }

  return (myGH);
}


 /*@@
   @routine    IOBasic_AssignReductionList
   @date       Tue 31 Jul 2001
   @author     Thomas Radke
   @desc
               Callback routine called by CCTK_TraverseString() to set the
               reduction list for a given variable.
               For CCTK_GF and CCTK_ARRAY variables, it builds a chained list
               of reduction operators according to the settings of 'optstring'
               or the 'IOBasic::out{Info|Scalar}_reductions' parameter.
   @enddesc
   @calls      CCTK_GroupTypeFromVarI
               CCTK_LocalArrayReductionHandle

   @var        vindex
   @vdesc      index of the variable to set info output
   @vtype      int
   @vio        in
   @endvar
   @var        optstring
   @vdesc      option string for this variable
   @vtype      const char *
   @vio        in
   @endvar
   @var        arg
   @vdesc      user-supplied argument to callback routine
   @vtype      void *
   @vio        in
   @endvar
@@*/
void IOBasic_AssignReductionList (int vindex, const char *optstring, void *arg)
{
  int table, iterator, reduction_handle;
  char key[128];
  CCTK_INT type, nelems;
  const char *string_start, *string_end;
  char *fullname, *reduction_op, *reduction_op_list;
  iobasic_reductionlist_t *list;
  iobasic_reduction_t **new_reduction;
  const iobasic_parseinfo_t *info;


  info = (const iobasic_parseinfo_t *) arg;
  list = info->reduction_list + vindex;

  if (CCTK_GroupTypeFromVarI (vindex) == CCTK_SCALAR)
  {
    if (optstring)
    {
      CCTK_VWarn (5, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "option list '%s' for variable '%s' ignored",
                  optstring, CCTK_VarName (vindex));
    }

    list->reductions = malloc (sizeof (iobasic_reduction_t));
    if (strncmp (CCTK_VarTypeName (CCTK_VarTypeI (vindex)),
                 "CCTK_VARIABLE_COMPLEX", 21))
    {
      list->num_reductions = 1;
      list->reductions->name = strdup ("scalar value");
      list->reductions->next = NULL;
    }
    else
    {
      list->num_reductions = 2;
      list->reductions->name = strdup ("real part");
      list->reductions->next = malloc (sizeof (iobasic_reduction_t));
      list->reductions->next->name = strdup ("imag part");
      list->reductions->next->next = NULL;
    }

    return;
  }

  /* initialize to empty list */
  list->num_reductions = 0;
  list->reductions = NULL;

  if (optstring)
  {
    reduction_op_list = NULL;
    fullname = CCTK_FullName (vindex);

    table = Util_TableCreateFromString (optstring);
    if (table >= 0)
    {
      if (Util_TableQueryValueInfo (table, &type, &nelems, "reductions") > 0)
      {
        if (type == CCTK_VARIABLE_CHAR && nelems > 0)
        {
          reduction_op_list = malloc (nelems + 1);
          Util_TableGetString (table, nelems + 1, reduction_op_list,
                               "reductions");
        }
        else
        {
          CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                      "Key 'reductions' in option string '%s' for variable "
                      "'%s' must have a non-empty string value",
                      optstring, fullname);
        }
        Util_TableDeleteKey (table, "reductions");
      }

      /* warn about other options */
      iterator = Util_TableItCreate (table);
      for (iterator = Util_TableItCreate (table);
           Util_TableItQueryIsNonNull (iterator) > 0 &&
           Util_TableItQueryKeyValueInfo (iterator, sizeof(key), key, 0, 0) > 0;
           Util_TableItAdvance (iterator))
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "Option with key '%s' in option string '%s' for variable "
                    "'%s' not recognized", key, optstring, fullname);
      }
      Util_TableItDestroy (iterator);

      Util_TableDestroy (table);
    }
    else
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Option string '%s' for variable '%s' couldn't be parsed",
                  optstring, fullname);
    }
    free (fullname);

    if (! reduction_op_list)
    {
      reduction_op_list = strdup (info->reductions_string);
    }
  }
  else
  {
    reduction_op_list = strdup (info->reductions_string);
  }

  /* now loop over all reduction operators */
  string_start = reduction_op_list;
  reduction_op = malloc (strlen (string_start) + 1);
  while (string_start && *string_start)
  {
    /* skip leading spaces */
    while (isspace ((int) *string_start))
    {
      string_start++;
    }
    if (! *string_start)
    {
      break;
    }

    /* advance to end of the operator string */
    string_end = string_start + 1;
    while (*string_end && ! isspace ((int) *string_end))
    {
      string_end++;
    }

    /* copy the operator string */
    strncpy (reduction_op, string_start, string_end - string_start);
    reduction_op[string_end - string_start] = 0;
    string_start = string_end;

    /* get the reduction handle from the reduction operator */
    reduction_handle = CCTK_LocalArrayReductionHandle (reduction_op);
    if (reduction_handle < 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Unknown reduction operator '%s'.  Maybe you forgot to activate thorn LocalReduce?", reduction_op);
      continue;
    }

    /* add new reduction to end of list */
    new_reduction = &list->reductions;
    while (*new_reduction)
    {
      if (strcmp ((*new_reduction)->name, reduction_op) == 0)
      {
        new_reduction = NULL;
        break;
      }
      new_reduction = &((*new_reduction)->next);
    }
    if (new_reduction == NULL)
    {
      CCTK_VWarn (3, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOBasic_AssignReductionList: Duplicate reduction operator "
                  "'%s' will be ignored", reduction_op);
      continue;
    }

    *new_reduction = malloc (sizeof (**new_reduction));
    (*new_reduction)->handle = reduction_handle;
    (*new_reduction)->name   = strdup (reduction_op);
    (*new_reduction)->next   = NULL;
    list->num_reductions++;
  }

  free (reduction_op_list);
  free (reduction_op);
}
