 /*@@
   @file      Startup.c
   @date      Sat Feb 6 1999
   @author    Gabrielle Allen
   @desc
              Startup routines for IOASCII.
   @enddesc
   @version   $Id: Startup.c 206 2009-12-05 02:14:32Z schnetter $
 @@*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SPEC 
#include "IOASCII/cctk.h" 
#define THORN_DIR "IOASCII" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "cctk_IOMethods.h"
#ifdef SPEC 
#include "IOASCII/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "CactusBase/IOUtil/src/ioutil_Utils.h"
#include "ioASCIIGH.h"
#if defined(SPEC) && !defined(strdup)
#include "util_String.h"
#define strdup Util_Strdup
#endif

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Id: Startup.c 206 2009-12-05 02:14:32Z schnetter $";
CCTK_FILEVERSION(CactusBase_IOASCII_Startup_c)


/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
#define CREATE_OUTDIR(dim, dir)                                               \
        {                                                                     \
          const char *_dir = dir;                                             \
                                                                              \
                                                                              \
          /* check whether "dir" was set; if not default to "IO::out_dir" */  \
          if (*_dir == 0)                                                     \
          {                                                                   \
            _dir = out_dir;                                                   \
          }                                                                   \
                                                                              \
          /* omit the directory name if it's the current working dir */       \
          if (strcmp (_dir, ".") == 0)                                        \
          {                                                                   \
            myGH->dir = strdup ("");                                          \
          }                                                                   \
          else                                                                \
          {                                                                   \
            myGH->dir = malloc (strlen (_dir) + 2);                           \
            sprintf (myGH->dir, "%s/", _dir);                                 \
          }                                                                   \
                                                                              \
          /* create the directory */                                          \
          i = IOUtil_CreateDirectory (GH, myGH->dir, 0, 0);                   \
          if (i < 0)                                                          \
          {                                                                   \
            CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,              \
                        "IOASCII_SetupGH: Problem creating directory '%s' "   \
                        "for %dD output", myGH->dir, dim);                    \
          }                                                                   \
          else if (i >= 0 && CCTK_Equals (verbose, "full"))                   \
          {                                                                   \
            CCTK_VInfo (CCTK_THORNSTRING, "%dD output to directory '%s'",     \
                        dim, myGH->dir);                                      \
          }                                                                   \
        }


/********************************************************************
 ********************    External Routines   ************************
 ********************************************************************/
int IOASCII_Startup (void);


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static void *IOASCII_SetupGH (tFleshConfig *config, int conv_level, cGH *GH);


 /*@@
   @routine   IOASCII_Startup
   @date      Sat Feb 6 1999
   @author    Gabrielle Allen
   @desc
              The startup registration routine for IOASCII.
              Registers the GH extensions needed for IOASCII
              along with its setup routine.
   @enddesc
   @calls     CCTK_RegisterGHExtension
              CCTK_RegisterGHExtensionSetupGH
@@*/
int IOASCII_Startup (void)
{
  CCTK_RegisterGHExtensionSetupGH (CCTK_RegisterGHExtension ("IOASCII"),
                                   IOASCII_SetupGH);
  return 0;
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
 /*@@
   @routine   IOASCII_SetupGH
   @date      Sat Feb 6 1999
   @author    Gabrielle Allen
   @desc
              Allocates and sets up IOASCII's GH extension structure
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

   @returntype void *
   @returndesc
               pointer to the allocated GH extension structure
   @endreturndesc
@@*/
static void *IOASCII_SetupGH (tFleshConfig *config, int conv_level, cGH *GH)
{
  int i, maxdim, numvars;
  asciiioGH *myGH;
  DECLARE_CCTK_PARAMETERS


  /* suppress compiler warnings about unused variables */
  (void) (config + 0);
  (void) (conv_level + 0);
  (void) (GH + 0);

  /* allocate the GH extension and its components */
  myGH = malloc (sizeof (asciiioGH));
  if (! myGH)
  {
    CCTK_WARN (0, "IOASCII_SetupGH: Unable to allocate memory for GH");
  }

  /* register the IOASCII routines as output methods  */
  i = CCTK_RegisterIOMethod ("IOASCII_1D");
  CCTK_RegisterIOMethodOutputGH (i, IOASCII_Output1DGH);
  CCTK_RegisterIOMethodOutputVarAs (i, IOASCII_Output1DVarAs);
  CCTK_RegisterIOMethodTimeToOutput (i, IOASCII_TimeFor1D);
  CCTK_RegisterIOMethodTriggerOutput (i, IOASCII_TriggerOutput1D);

  /* only register N-D IOASCII I/O methods
     if at least N-dimensional grid variables are defined by thorns */
  maxdim = CCTK_MaxDim ();
  if (maxdim >= 2)
  {
    i = CCTK_RegisterIOMethod ("IOASCII_2D");
    CCTK_RegisterIOMethodOutputGH (i, IOASCII_Output2DGH);
    CCTK_RegisterIOMethodOutputVarAs (i, IOASCII_Output2DVarAs);
    CCTK_RegisterIOMethodTimeToOutput (i, IOASCII_TimeFor2D);
    CCTK_RegisterIOMethodTriggerOutput (i, IOASCII_TriggerOutput2D);
  }

  if (maxdim >= 3)
  {
    i = CCTK_RegisterIOMethod ("IOASCII_3D");
    CCTK_RegisterIOMethodOutputGH (i, IOASCII_Output3DGH);
    CCTK_RegisterIOMethodOutputVarAs (i, IOASCII_Output3DVarAs);
    CCTK_RegisterIOMethodTimeToOutput (i, IOASCII_TimeFor3D);
    CCTK_RegisterIOMethodTriggerOutput (i, IOASCII_TriggerOutput3D);
  }

  numvars = CCTK_NumVars ();
  myGH->out1D_every = malloc (numvars * sizeof (CCTK_INT));
  myGH->out2D_every = malloc (numvars * sizeof (CCTK_INT));
  myGH->out3D_every = malloc (numvars * sizeof (CCTK_INT));
  myGH->out1D_last = malloc (numvars * sizeof (int));
  myGH->out2D_last = malloc (numvars * sizeof (int));
  myGH->out3D_last = malloc (numvars * sizeof (int));

  for (i = 0; i < numvars; i++)
  {
    myGH->out1D_last[i] = -1;
    myGH->out2D_last[i] = -1;
    myGH->out3D_last[i] = -1;
  }

  myGH->out1D_vars = strdup ("");
  myGH->out2D_vars = strdup ("");
  myGH->out3D_vars = strdup ("");
  myGH->out1D_every_default = out1D_every - 1;
  myGH->out2D_every_default = out2D_every - 1;
  myGH->out3D_every_default = out3D_every - 1;

  myGH->filenameList1D = NULL;
  myGH->fileList_2D = NULL;
  myGH->fileList_3D = NULL;

  myGH->stop_on_parse_errors = strict_io_parameter_check;
  if (! CCTK_Equals (verbose, "none"))
  {
    CCTK_INFO ("I/O Method 'IOASCII_1D' registered: output of 1D lines of grid "
               "functions/arrays to ASCII files");
  }
  IOASCII_CheckSteerableParameters1D (myGH);
  if (maxdim >= 2)
  {
    if (! CCTK_Equals (verbose, "none"))
    {
      CCTK_INFO ("I/O Method 'IOASCII_2D' registered: output of 2D planes of "
                 "grid functions/arrays to ASCII files");
    }
    IOASCII_CheckSteerableParameters2D (myGH);
  }
  if (maxdim >= 3)
  {
    if (! CCTK_Equals (verbose, "none"))
    {
      CCTK_INFO ("I/O Method 'IOASCII_3D' registered: output of 3D grid "
                 "functions/arrays to ASCII files");
    }
    IOASCII_CheckSteerableParameters3D (myGH);
  }
  myGH->stop_on_parse_errors = 0;

  /* make sure all output directories exist */
  CREATE_OUTDIR (1, out1D_dir);
  CREATE_OUTDIR (2, out2D_dir);
  CREATE_OUTDIR (3, out3D_dir);

  return (myGH);
}
