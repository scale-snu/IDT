 /*@@
   @file      Startup.c
   @date      Sun Jul 04 1999
   @author    Thomas Radke
   @desc
              Startup routines for PUGHInterp
   @enddesc
   @version   $Id: Startup.c 97 2011-01-21 03:14:51Z eschnett $
 @@*/

#include <stdlib.h>

#ifdef SPEC 
#include "PUGHInterp/cctk.h" 
#define THORN_DIR "PUGHInterp" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "util_Table.h"
#include "cctk_Interp.h"
#include "pughInterpGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusPUGH_PUGHInterp_Startup_c)


/********************************************************************
 ********************    External Routines   ************************
 ********************************************************************/
int PUGHInterp_Startup (void);


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
#ifdef CCTK_MPI
static void *SetupGH (tFleshConfig *config, int convergence_level, cGH *GH);
#endif


 /*@@
   @routine   PUGHInterp_Startup
   @date      Sun Jul 04 1999
   @author    Thomas Radke
   @desc
              The startup registration routine for PUGHInterp.
              Registers the PUGHInterp GH extensions setup routine with the
              flesh and overloads the CCTK_InterpGridArrays() routine.
   @enddesc
   @calls     CCTK_OverloadInterpGridArrays
              CCTK_RegisterGHExtensionSetupGH
@@*/
int PUGHInterp_Startup (void)
{
  CCTK_OverloadInterpGridArrays (PUGHInterp_InterpGridArrays);

#ifdef CCTK_MPI
  CCTK_RegisterGHExtensionSetupGH (CCTK_RegisterGHExtension ("PUGHInterp"),
                                   SetupGH);
#endif

  return 0;
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
#ifdef CCTK_MPI
 /*@@
   @routine   SetupGH
   @date      Sun Jul 04 1999
   @author    Thomas Radke
   @desc
              Allocates the GH extension structure and - for the MPI case -
              the count/displacement buffers used in MPI_Alltoall()
              and MPI_Alltoallv()
   @enddesc
   @calls     CCTK_nProcs

   @returntype void *
   @returndesc
               pointer to the allocated GH extension structure
   @endreturndesc
@@*/
static void *SetupGH (tFleshConfig *config, int convergence_level, cGH *GH)
{
  int nprocs;
  pughInterpGH *myGH;


  /* suppress compiler warnings about unused variables */
  (void) (config + 0);
  (void) (convergence_level + 0);

  myGH = malloc (sizeof (pughInterpGH));

  /* allocate once for all fields of same type */
  nprocs = CCTK_nProcs (GH);
  myGH->sendcnt = malloc (4 * nprocs * sizeof (int));
  myGH->senddispl = myGH->sendcnt + nprocs;
  myGH->recvcnt = myGH->senddispl + nprocs;
  myGH->recvdispl = myGH->recvcnt + nprocs;

  myGH->N_points_from = malloc (2 * nprocs * sizeof (CCTK_INT));
  myGH->N_points_to   = myGH->N_points_from + nprocs;

  return (myGH);
}
#endif /* CCTK_MPI */
