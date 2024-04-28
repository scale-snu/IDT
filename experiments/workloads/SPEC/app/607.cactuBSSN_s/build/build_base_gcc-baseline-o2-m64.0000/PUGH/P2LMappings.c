 /*@@
   @file      P2LMappings.c
   @date      Sun Oct 30 17:02:58 CST 2005
   @author    Dylan Stark
   @desc
              Provides routines for mapping between physical and
              logical numberings of processors.
   @enddesc
   @version $Header$
 @@*/

#include <stdio.h>
#include <stdlib.h>

#ifdef SPEC 
#include "PUGH/cctk.h" 
#define THORN_DIR "PUGH" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 

#include "pugh_Register.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_P2L_c)


/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

static void *CreateDirectTranslation(int dim, int *nprocs, int np);
static int   DestroyDirectTranslation(void *table);
static int   DirectPhysicalToLogical(void *table, int proc);
static int   DirectLogicalToPhysical(void *table, int ijk);

static void *CreateExampleTranslation(int dim, int *nprocs, int np);
static int   DestroyExampleTranslation(void *table);
static int   ExamplePhysicalToLogical(void *table, int proc);
static int   ExampleLogicalToPhysical(void *table, int ijk);

/********************************************************************
 ********************* Other Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 *********************     Local Data   *****************************
 ********************************************************************/

/********************************************************************
 *********************  Scheduled Routines  *************************
 ********************************************************************/
/* Fortran wrappers appear immediately after C definition of function */

 /*@@
   @routine    PUGH_RegisterDefaultRoutines
   @date       Wed Dec 14 16:51:22 2005
   @author     Tom Goodale
   @desc 
   Registers the default translation and topology generation routines.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/

int PUGH_RegisterPUGHP2LRoutines(void)
{
  PUGH_RegisterP2L(CreateDirectTranslation,
                   DestroyDirectTranslation,
                   DirectPhysicalToLogical,
                   DirectLogicalToPhysical,
                   "direct");

  PUGH_RegisterP2L(CreateExampleTranslation,
                   DestroyExampleTranslation,
                   ExamplePhysicalToLogical,
                   ExampleLogicalToPhysical,
                   "example");

  return 0;
}

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/


/********************************************************************
 *********************  Local Routines  *****************************
 ********************************************************************/

 /*@@
   @routine    CreateDirectTranslation
   @date       Wed Dec 14 19:00:57 2005
   @author     Tom Goodale
   @desc 
   Creates a dummy translation table as one isn't necessary for
   the direct case.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
void *CreateDirectTranslation(int dim, int *nprocs, int np)
{
  static int dummy = 1;
  return (void *)&dummy;
}

 /*@@
   @routine    DestroyDirectTranslation
   @date       Wed Dec 14 19:00:57 2005
   @author     Tom Goodale
   @desc 
   Routine which would destroy the translation table if there was one.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int DestroyDirectTranslation(void *table)
{
  return 0;
}

 /*@@
   @routine    DirectPhysicalToLogical
   @date       Wed Dec 14 19:02:56 2005
   @author     Tom Goodale
   @desc 
   Direct map of the physical processor number to the logical one.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int DirectPhysicalToLogical(void *table, int proc)
{
  return proc;
}

 /*@@
   @routine    DirectLogicalToPhysical
   @date       Wed Dec 14 19:02:56 2005
   @author     Tom Goodale
   @desc 
   Direct map of the logical processor number to the physical one.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int DirectLogicalToPhysical(void *table, int ijk)
{
  return ijk;
}

/*@@
   @routine    CreateExampleTranslation
   @date       Sat Oct  8 22:24:53 CDT 2005
   @author     Dylan Stark
   @desc
               Allocates an int[2][np] array where the first
               row maps id->ijk and the second row maps ijk->id.
   @enddesc
 @@*/
void *CreateExampleTranslation(int dim, int *nprocs, int np)
{
  int i;
  int **translation;
  
  translation    = (int **)malloc(2*sizeof(int *));
  if (translation)
  {
    translation[0] = malloc(np*sizeof(int));
    translation[1] = malloc(np*sizeof(int));

    if (translation[0] && translation[1])
    {
      for (i=0; i<np; i++)
      {
        translation[0][i]=i;
        translation[1][i]=i;
      }
    }
    else
    {
      if (translation[0])
      {
        free(translation[0]);
      }
      if (translation[1])
      {
        free(translation[1]);
      }
      
      free(translation);
      translation = NULL;
    }
  }

  return (void *)translation;
}

/*@@
   @routine    DestroyExampleTranslation
   @date       Sat Oct  8 22:31:52 CDT 2005
   @author     Dylan Stark
   @desc
               Deallocate memory for the translation.
   @enddesc
 @@*/
static int DestroyExampleTranslation(void *table)
{
  int **translation = (int **)table;

  free(translation[0]);
  free(translation[1]);
  free(translation);

  return 0;
}


/*@@
   @routine    ExamplePhysicalToLogical
   @date       Sat Oct  8 21:45:52 CDT 2005
   @author     Dylan Stark
   @desc
     Take an MPI processor number and map it to some logical index.
   @enddesc
 @@*/
static int ExamplePhysicalToLogical(void *table, int proc)
{
  int **translation = (int **)table;
  
  return translation[0][proc];
}

 /*@@
   @routine    LogicalToPhysical
   @date       Sat Oct  8 21:48:52 CDT 2005
   @author     Dylan Stark
   @desc
     Take a logical index and map it to a MPI processor number.
   @enddesc
@@*/
static int ExampleLogicalToPhysical(void *table, int ijk)
{
  int **translation = (int **)table;
  
  return translation[1][ijk];
}
