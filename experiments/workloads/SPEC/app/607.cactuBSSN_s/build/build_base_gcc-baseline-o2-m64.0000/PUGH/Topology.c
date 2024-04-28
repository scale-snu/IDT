 /*@@
   @file      Topology.c
   @date      Wed Sep 13 20:10:24 2000
   @author    Tom Goodale
   @desc 
   Topology routines
   @enddesc
   @version  $Header$
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

CCTK_FILEVERSION(CactusPUGH_PUGH_Topology_c)

/********************************************************************
 *********************  Macro Definitions  **************************
 ********************************************************************/

/********************************************************************
 *********************  Local Data Types  ***************************
 ********************************************************************/

/********************************************************************
 *********************  Aliased Routine Prototypes  *****************
 ********************************************************************/

/********************************************************************
 *********************  Scheduled Routine Prototypes  ***************
 ********************************************************************/

/********************************************************************
 *********************  Fortran Wrapper Prototypes  *****************
 ********************************************************************/

/********************************************************************
 *********************  Local Routine Prototypes  *******************
 ********************************************************************/

static int FranksTopology(int dim,
                          int total_procs, 
                          const int *nsize,
                          const int *nghostzones, 
                          int *nprocs);

static int TraditionalTopology(int dim, 
                               int total_procs, 
                               const int *nsize,
                               const int *nghostzones, 
                               int *nprocs);

static int IntegerRoot(int number, int invpower);

static int IntSort (const void *a, const void *b);

/********************************************************************
 *********************  Local Data  *********************************
 ********************************************************************/

/********************************************************************
 *********************  Aliased Routines  ***************************
 ********************************************************************/

/********************************************************************
 *********************  Scheduled Routines  *************************
 ********************************************************************/
/* Fortran wrappers appear immediately after C definition of function */

int PUGH_RegisterPUGHTopologyRoutines(void);
int PUGH_RegisterPUGHTopologyRoutines(void)
{
  PUGH_RegisterGenerateTopology(TraditionalTopology,
                                "manual");
  PUGH_RegisterGenerateTopology(FranksTopology, 
                                "automatic");
  PUGH_RegisterGenerateTopology(TraditionalTopology, 
                                "automatic_old");
  return 0;
}

/********************************************************************
 *********************  Other External Routines  ********************
 ********************************************************************/

/********************************************************************
 *********************  Local Routines  *****************************
 ********************************************************************/

 /*@@
   @routine    TraditionalTopology
   @date       Fri Nov  5 11:31:21 1999
   @author     Tom Goodale
   @desc
   Generate the appropriate processor topology for this processor
   decomposition.
   @enddesc
   @history
   @hdate Tue Jan 30 17:04:50 2001 @hauthor Tom Goodale
   @hdesc Added call to integer root function and qsort to
          avoid problems with real to integer conversions and
          demonstrable failure of the algorithm when dealing
          with large prime factors.
   @endhistory
@@*/
static int TraditionalTopology(int dim, 
                               int total_procs, 
                               const int *nsize,
                               const int *nghostzones, 
                               int *nprocs)
{
  int i;
  int used_procs;
  int free_procs;
  int retval;
  int free_dims;

  used_procs = 0;
  free_procs = total_procs;

  retval = 0;

  free_dims = dim;

  for(i=0; i < dim; i++)
  {
    if((nprocs[i])>0)
    {
      free_dims--;
      if(used_procs)
      {
        used_procs *= nprocs[i];
      }
      else
      {
        used_procs = nprocs[i];
      }
      if (total_procs%used_procs)
      {
        CCTK_WARN(0, "Inconsistent PUGH topology");
        retval = 1;
      }
      else
      {
        free_procs = total_procs/used_procs;
      }
    }
  }

  /* if the user specified the number of processors for each dimension
     check that they sum up to the total number of processors available */
  if (dim > 1 && retval == 0 && free_dims == 0 && used_procs != total_procs)
  {
    CCTK_WARN(0, "Inconsistent PUGH topology");
  }

  /* Ok calculate topology if necessary */
  if(free_dims && ! retval)
  {
    /* This algorithm gives the most number of processors
     * in the highest dimension.
     */

    int *working;
    int root;
    int place;

    root = free_dims;
    working = calloc(free_dims,sizeof(int));
#ifdef DEBUG_PUGH
    printf("Processor topology for dimension %d\n",dim);
#endif


    for(i = 0; i < free_dims  ; i++)
    {
      working[i] = IntegerRoot(free_procs, root);

      while(free_procs % working[i]) working[i]--;

#ifdef DEBUG_PUGH
      printf(" working[%d] = %d\n",i,working[i]);
#endif
      free_procs /= working[i];
      root--;
    }


    /* The above doesn't necessarily sort them properly
     * e.g. if one of the factors is a prime then the
     * above will sort the 1 before the prime.
     */
    qsort(working,free_dims,sizeof(int),IntSort);

    for(i = 0,place=0; i < dim ; i++)
    {
      if(nprocs[i] <= 0)
      {
        nprocs[i] = working[place];
        place++;
      }

#ifdef DEBUG_PUGH
      printf(" nprocs[%d] = %d\n",i,nprocs[i]);
#endif
    }

    free(working);
  }

  return retval;
}



 /*@@
   @routine    FranksTopology
   @date       Thu Feb 02 17:39:21 2005
   @author     Frank Loeffler
   @desc
   Generate the appropriate processor topology for this processor
   decomposition.
   This routine tries to decompose using the information about the actual
   grid size. It can fail in certain situations in which it falls back
   gracefully to the traditional decomposition giving a warning.
   It also does not support manually set topologies and falls back in this
   case.
   @enddesc
   @history
   @endhistory
@@*/
static int FranksTopology(int dim,
                          int total_procs, 
                          const int *nsize,
                          const int *nghostzones, 
                          int *nprocs)
{
  int i;
  int max_dir, max_length;
  int free_procs = total_procs-1;
  int used_procs = 1;
  int *my_nprocs;
  
  /* Nothing to decompose here */
  if (dim == 0)
  {
    return TraditionalTopology(dim, total_procs, nsize, 
                                        nghostzones, nprocs);
  }
  my_nprocs = (int*)malloc(dim*sizeof(int));
  if (!my_nprocs)
  {
    CCTK_WARN(0, "Out of memory, malloc returned NULL");
  }

  /* start with a single domain */
  for (i = 0; i < dim; i++)
  {
    my_nprocs[i] = 1;
  }
  /* divide as long as there are processors left */
  while (free_procs)
  {
    used_procs = total_procs - free_procs;
    /* find the direction with the longest length, which is allowed */
    max_dir = -1;
    max_length = 0;
    for (i = 0; i < dim; i++)
    {
      /* is one part larger than the max? (then it might be a new max) */
      if ((nsize[i] / my_nprocs[i] > max_length) &&
          /* would there be at least one real point if we divide? */
          (nsize[i] > 2*nghostzones[i]+my_nprocs[i]) &&
          /* do we have enough processors left to divide in this direction? */
          (used_procs/my_nprocs[i] <= free_procs))
      {
        max_length = nsize[i] / my_nprocs[i];
        max_dir = i;
      }
    }
    /* if no such direction is found: fall back giving a warning */
    if (max_dir == -1)
    {
      free(my_nprocs);
      CCTK_WARN(CCTK_WARN_DEBUG,
                "Falling back to the old PUGH topology method");
      return TraditionalTopology(dim, total_procs, nsize, 
                                 nghostzones, nprocs);
    }
    /* count the new direction and go on */
    /* note: this is garanteed to decrement at least by one, since the number
       of used processors is naturally always >= the number of processors used
       for one dimension */
    free_procs -= used_procs/my_nprocs[max_dir];
    my_nprocs[max_dir]++;
  }
  /* If there are numbers already set up which are different from what
     we arrive at or we have funny grid sizes, fall back */
  for (i = 0; i < dim; i++)
  {
    if ((nprocs[i] && (nprocs[i]!=my_nprocs[i])) || (nsize[i]<1))
    {
      free(my_nprocs);
      CCTK_WARN(CCTK_WARN_DEBUG,
                "Falling back to the old PUGH topology method");
      return TraditionalTopology(dim, total_procs, nsize, 
                                          nghostzones, nprocs);
    }
  }
  for (i = 0; i < dim; i++)
  {
    nprocs[i] = my_nprocs[i];
  }
  free(my_nprocs);
  /* success */
  return 0;
}

 /*@@
   @routine    IntegerRoot
   @date       Tue Jan 30 17:06:21 2001
   @author     Tom Goodale
   @desc
   Generate the highest integer below a given integer root of an integer.
   @enddesc

   @var     number
   @vdesc   The number to take the root of
   @vtype   int
   @vio     in
   @endvar
   @var     invpower
   @vdesc   The root to take
   @vtype   int
   @vio     in
   @endvar

   @returntype int
   @returndesc
   The highest integer below the desired root.
   @endreturndesc
@@*/
static int IntegerRoot(int number, int invpower)
{
  int i;
  int tmp;
  int root;

  for(root = 1; root <= number; root++)
  {
    for(i=1, tmp=root; i < invpower; i++, tmp*=root);

    if(tmp > number)
    {
      root--;
      break;
    }
  }

  return root;
}

 /*@@
   @routine    IntSort
   @date       Tue Jan 30 17:08:47 2001
   @author     Tom Goodale
   @desc
               Sorts two integers for the qsort routine.
   @enddesc

   @var        a
   @vdesc      Pointer to first integer to compare
   @vtype      const void *
   @vio        in
   @endvar
   @var        b
   @vdesc      Pointer to second integer to compare
   @vtype      const void *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               -ve if b is greater than a.<BR>
               +ve if a is greater than b.<BR>
               0   if a is equal to b.
   @endreturndesc
@@*/
static int IntSort (const void *a, const void *b)
{
  return (*(const int *) a - *(const int *) b);
}
