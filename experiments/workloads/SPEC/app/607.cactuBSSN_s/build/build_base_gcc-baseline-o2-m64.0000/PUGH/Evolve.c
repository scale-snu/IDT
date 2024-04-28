 /*@@
   @file      PUGH_Evolve.c
   @date      Thu Oct  8 17:28:46 1998
   @author    Tom Goodale
   @desc
              PUGH evolution stuff.
   @enddesc
   @version   $Id: Evolve.c 461 2005-03-24 15:12:15Z schnetter $
   @hdate Tue Mar 28 22:41:15 2000 @hauthor Ed Evans
   @hdesc Moved into PUGH and added time level rotation
   @hdate Tue Mar 28 22:41:45 2000 @hauthor Tom Goodale
   @hdesc Tidied up
 @@*/

#include <stdio.h>

#ifdef SPEC 
#include "PUGH/cctk.h" 
#define THORN_DIR "PUGH" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "PUGH/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "cctk_Termination.h"

#include "pugh.h"
#include "pughi.h"

#ifdef HAVE_TIME_GETTIMEOFDAY
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <unistd.h>
#endif

static const char *rcsid="$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_Evolve_c)

/* Define some macros for convenience. */

#define ForallConvLevels(iteration, conv_level)  {                     \
                                        int factor = 1;                \
                                        for(conv_level = 0 ;           \
                                            conv_level < config->nGHs; \
                                            conv_level++)              \
                                        {                              \
                                          if(iteration%factor == 0)    \
                                          {

#define EndForallConvLevels                                            \
                                          };                           \
                                          factor *=2;                  \
                                        };                             \
                                     }


/* Local function prototypes. */

static int DoneMainLoop (const cGH *GH, CCTK_REAL cctk_time, int iteration);
static void StepGH(cGH *GH);
static void RotateTimeLevelsGH(const cGH *GH);


 /*@@
   @routine    PUGH_Evolve
   @date       Thu Oct  8 17:30:15 1998
   @author     Tom Goodale
   @desc
   PUGH's evolution routine.
   @enddesc
   @history
   @hdate Tue Mar 28 22:42:47 2000 @hauthor Ed Evans
   @hdesc  Copied from CactusDefaultEvolve and added rotation
   @hdate Tue Mar 28 22:43:18 2000 @hauthor Tom Goodale
   @hdesc Tidied
   @hdate Fri May 12 2000 @hauthor Thomas Radke
   @hdesc  Moved evolution loop termination check into DoneMainLoop()
   @endhistory
@@*/
int PUGH_Evolve(tFleshConfig *config)
{
  unsigned int convergence_level;
  unsigned int iteration=CCTK_MainLoopIndex();

  /*** Call OUTPUT for this GH (this routine    ***/
  /*** checks if output is necessary) and makes ***/
  /*** a Traverse with CCTK_ANALYSIS      ***/
  ForallConvLevels(iteration, convergence_level)
  {
    CCTK_Traverse(config->GH[convergence_level], "CCTK_ANALYSIS");
    CCTK_OutputGH(config->GH[convergence_level]);
  }
  EndForallConvLevels;

  while (! DoneMainLoop (config->GH[0],config->GH[0]->cctk_time, iteration))
  {
    ForallConvLevels(iteration, convergence_level)
    {
      RotateTimeLevelsGH(config->GH[convergence_level]);
    }
    EndForallConvLevels;

    iteration = CCTK_SetMainLoopIndex(++iteration);

    /* Step each convergence level */
    ForallConvLevels(iteration, convergence_level)
    {
      StepGH(config->GH[convergence_level]);
    }
    EndForallConvLevels;

    /* Dump out checkpoint data on all levels */
    ForallConvLevels(iteration, convergence_level)
    {
      CCTK_Traverse(config->GH[convergence_level], "CCTK_CHECKPOINT");
    }
    EndForallConvLevels;

    /* Output perhaps */

    /*** Call OUTPUT for this GH (this routine    ***/
    /*** checks if output is necessary) and makes ***/
    /*** a Traverse with CCTK_ANALYSIS      ***/
    ForallConvLevels(iteration, convergence_level)
    {
      CCTK_Traverse(config->GH[convergence_level], "CCTK_ANALYSIS");
      CCTK_OutputGH(config->GH[convergence_level]);
    }
    EndForallConvLevels;

#if 0
    /* Termination has been raised and broadcasted, exit loop*/
    if (cactus_terminate==TERMINATION_RAISED_BRDCAST) break;
#endif

  } /*** END OF MAIN ITERATION LOOP ***/

  return 0;
}

/************************************************************************/

 /*@@
   @routine    DoneMainLoop
   @date       Fri May 12 2000
   @author     Thomas Radke
   @desc
               Check the termination conditions for the evolution loop
   @enddesc
   @calls      CCTK_TerminationReached
   @history
   @hdate      Thu 6 Nov 2002
   @hauthor    Thomas Radke
   @hdesc      Added max_runtime condition test
   @endhistory

   @var        GH
   @vdesc      pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        time
   @vdesc      current physical simulation time
   @vtype      CCTK_REAL
   @vio        in
   @endvar
   @var        iteration
   @vdesc      iteration number
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               true (1) or false (0) for done/not done with evolution loop
   @endreturndesc
@@*/
static int DoneMainLoop (const cGH *GH, CCTK_REAL simulation_time,int iteration)
{
  int max_iteration_reached, max_simulation_time_reached, max_runtime_reached;
  CCTK_INT local, global;
#ifdef HAVE_TIME_GETTIMEOFDAY
  struct timeval runtime;
  static struct timeval starttime = {0, 0};
#endif
  DECLARE_CCTK_PARAMETERS


#ifdef HAVE_TIME_GETTIMEOFDAY
  /* on the first time through, get the start time */
  if (starttime.tv_sec == 0 && starttime.tv_usec == 0)
  {
    gettimeofday (&starttime, NULL);
  }
#endif

  local = terminate_next || CCTK_TerminationReached (GH);
  if (! local && ! CCTK_Equals (terminate, "never"))
  {
    max_iteration_reached = iteration >= cctk_itlast;

    if (GH->cctk_delta_time > 0)
    {
      max_simulation_time_reached = simulation_time >= cctk_final_time - 1.0e-6 * GH->cctk_delta_time;
    }
    else
    {
      max_simulation_time_reached = simulation_time <= cctk_final_time - 1.0e-6 * GH->cctk_delta_time;
    }

    max_runtime_reached = 0;
#ifdef HAVE_TIME_GETTIMEOFDAY
    if (max_runtime > 0)
    {
      /* get the elapsed runtime in minutes and compare with max_runtime */
      gettimeofday (&runtime, NULL);
      runtime.tv_sec -= starttime.tv_sec;
      max_runtime_reached = ((CCTK_REAL) runtime.tv_sec / 60.0) >= max_runtime;
    }
#endif

    if (CCTK_Equals (terminate, "iteration"))
    {
      local = max_iteration_reached;
    }
    else if (CCTK_Equals (terminate, "time"))
    {
      local = max_simulation_time_reached;
    }
    else if (CCTK_Equals (terminate, "runtime"))
    {
      local = max_runtime_reached;
    }
    else if (CCTK_Equals (terminate, "any"))
    {
      local = max_iteration_reached || max_simulation_time_reached ||
               max_runtime_reached;
    }
    else if (CCTK_Equals (terminate, "all"))
    {
      local = max_iteration_reached && max_simulation_time_reached &&
               max_runtime_reached;
    }
    /* the following two conditions are deprecated in BETA14 */
    else if (CCTK_Equals (terminate, "either"))
    {
      local = max_iteration_reached || max_simulation_time_reached;
    }
    else /* if (CCTK_Equals (terminate, "both")) */
    {
      local = max_iteration_reached && max_simulation_time_reached;
    }
  }

#ifdef CCTK_MPI
  /* reduce the local flags from all processors
     into a single global termination flag */
  CACTUS_MPI_ERROR (MPI_Allreduce (&local, &global, 1, PUGH_MPI_INT, MPI_MAX,
                                   PUGH_pGH (GH)->PUGH_COMM_WORLD));
#else
  global = local;
#endif

  return (global);
}


 /*@@
   @routine    StepGH
   @date       Fri Aug 14 12:39:49 1998
   @author     Gerd Lanfermann
   @desc
     The full set of routines used to execute all schedule point
     int the main iteration loop. Makes calls to the individual
     routines for each schedule point.
   @enddesc
   @calls  CCTK_Traverse
 @@*/
static void StepGH(cGH *GH)
{
  /* Advance GH->iteration BEFORE evolving */

  GH->cctk_time = GH->cctk_time + GH->cctk_delta_time;
  GH->cctk_iteration++;

  CCTK_Traverse(GH, "CCTK_PRESTEP");
  CCTK_Traverse(GH, "CCTK_EVOL");
  CCTK_Traverse(GH, "CCTK_POSTSTEP");
}


 /*@@
   @routine    RotateTimeLevelsGH
   @date       Tue Mar 28 22:43:49 2000
   @author     Ed Evans
   @desc
   Rotates the time levels
   @enddesc
@@*/
static void RotateTimeLevelsGH(const cGH *GH)
{
  int i, var, numtimelevels;
  pGA *GA;
  pGH *pughGH;
  DECLARE_CCTK_PARAMETERS


  pughGH = PUGH_pGH(GH);

  for(var = 0; var < pughGH->nvariables; var++)
  {
    /* Only Rotate the number of timelevels actually enabled. */
    numtimelevels = PUGH_NumTimeLevels(pughGH,var);
    if(numtimelevels > 1)
    {
      GA = pughGH->variables[var][numtimelevels-1];

      for(i = numtimelevels-1; i; i--)
      {
        pughGH->variables[var][i] = pughGH->variables[var][i-1];
      }

      pughGH->variables[var][0] = GA;
      PUGH_InitializeMemory (initialize_memory, GA->vtype,
                             GA->extras->npoints * GA->varsize, GA->data);
    }
  }
}
