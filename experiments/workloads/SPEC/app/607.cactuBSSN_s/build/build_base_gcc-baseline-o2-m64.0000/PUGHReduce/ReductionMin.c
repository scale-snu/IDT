 /*@@
   @file      ReductionMin.c
   @date      Thu Apr  3 11:54:53 1997
   @author    Thomas Radke, Paul Walker
   @desc
              Defines the PUGH reduction operator to get the minimum
              for a set of arbitrary arrays.
   @enddesc
   @version   $Id: ReductionMin.c 65 2005-01-29 17:50:28Z schnetter $
 @@*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "pugh_reductions.h"

static const char *rcsid = "$Id: ReductionMin.c 65 2005-01-29 17:50:28Z schnetter $";

CCTK_FILEVERSION(CactusPUGH_PUGHReduce_ReductionMin_c)

/* local function prototypes */
static int ReductionMinVal (const cGH *GH,
                            int proc,
                            int num_dims,
                            const int from[/* dim */],
                            const int to[/* dim */],
                            int iterator[/* dim */],
                            const int points_per_dim[/* dim */],
                            int num_points,
                            int have_local_points,
                            int num_inarrays,
                            const int intypes[/* num_inarrays */],
                            const void *const inarrays[/* num_inarrays */],
                            int num_outvals,
                            CCTK_REAL outvals[/*num_inarrays*num_outvals*/]);


/*@@
  @routine PUGH_ReductionMinValArrays
  @author  Thomas Radke
  @date    19 Aug 1999
  @desc
           Registered PUGH reduction routine for computing the minima
           for a set of arrays.
           The arrays are described by their dimensions and variable type.
           For the number of output values only 1 is accepted.
           Type casting of the result is provided by specifying the
           requested output datatype. The intermediate reduction value
           is always computed as a CCTK_REAL value internally.
  @enddesc
  @history
  @endhistory
  @var     GH
  @vdesc   Pointer to CCTK grid hierarchy
  @vtype   const cGH *
  @vio     in
  @endvar
  @var     proc
  @vdesc   processor that should receive the result of operation
           (negative value means all processors receive the result)
  @vtype   int
  @vio     in
  @endvar
  @var     num_dims
  @vdesc   number of dimensions of input arrays
  @vtype   int
  @vio     in
  @endvar
  @var     dims
  @vdesc   dimensions of input arrays
  @vtype   const int *
  @vio     in
  @endvar
  @var     num_inarrays
  @vdesc   number of input arrays
  @vtype   int
  @vio     in
  @endvar
  @var     inarrays
  @vdesc   field of input arrays
  @vtype   const void *const *
  @vio     in
  @endvar
  @var     intype
  @vdesc   (common) variable type of input arrays
  @vtype   int
  @vio     in
  @endvar
  @var     num_outvals
  @vdesc   number of values per output array
  @vtype   int
  @vio     in
  @endvar
  @var     outvals
  @vdesc   pointer to buffer holding the output values
  @vtype   void *
  @vio     in
  @endvar
  @var     outtype
  @vdesc   (common) variable type of output arrays
  @vtype   int
  @vio     in
  @endvar
@@*/
int PUGH_ReductionMinValArrays (const cGH *GH,
                                int proc,
                                int num_dims,
                                const int dims[/* num_dims */],
                                int num_inarrays,
                                const void *const inarrays[/* num_inarrays */],
                                int intype,
                                int num_outvals,
                                void *outvals /* [num_outvals] */,
                                int outtype)
{
  return (PUGH_ReductionArrays (GH, proc, num_dims, dims,
                                intype, num_inarrays, inarrays,
                                outtype, num_outvals, outvals,
                                ReductionMinVal));
}


/*@@
  @routine PUGH_ReductionMinValGVs
  @author  Thomas Radke
  @date    23 Apr 1999
  @desc
           PUGH reduction routine to be registered for calculating the minimum.
           It just goes through the list of variables and calls the appropriate
           grouptype reduction routine.
           If the number of requested output values equals the size of the
           variable, global reduction is done element-wise.
           Otherwise num_outvals must be 1, and global reduction is done on the
           results of the local reductions.
  @enddesc
  @history
  @endhistory
  @var     GH
  @vdesc   Pointer to CCTK grid hierarchy
  @vtype   const cGH *
  @vio     in
  @endvar
  @var     proc
  @vdesc   processor that should receive the result of operation
           (negative value means all processors receive the result)
  @vtype   int
  @vio     in
  @endvar
  @var     num_outvals
  @vdesc   number of requested output elements
  @vtype   int
  @vio     in
  @endvar
  @var     outtype
  @vdesc   type of return value
  @vtype   int
  @vio     in
  @endvar
  @var     outvals
  @vdesc   array for the result values to be stored
  @vtype   void *
  @vio     in
  @endvar
  @var     num_invars
  @vdesc   number of variables in the list
  @vtype   int
  @vio     in
  @endvar
  @var     invars
  @vdesc   list of variables (given as their global indices)
  @vtype   const int *
  @vio     in
  @endvar
@@*/
int PUGH_ReductionMinValGVs (const cGH *GH,
                             int proc,
                             int num_outvals,
                             int outtype,
                             void *outvals /* [num_outvals] */,
                             int num_invars,
                             const int invars[/* num_invars */])
{
  return (PUGH_ReductionGVs (GH, proc, num_invars, invars,
                             outtype, num_outvals, outvals,
                             ReductionMinVal));
}


/*****************************************************************************/
/*                             local functions                               */
/*****************************************************************************/
/*@@
   @routine    ReductionMinVal
   @date       Aug 19 1999
   @author     Thomas Radke
   @desc       Returns the minimum for a set of distributed array with
               'num_points' elements each.
               Global reduction is done element-wise (num_outvals == 1)
               or on the results of the local reductions.
   @enddesc
@@*/
static int ReductionMinVal (const cGH *GH,
                            int proc,
                            int num_dims,
                            const int from[/* dim */],
                            const int to[/* dim */],
                            int iterator[/* dim */],
                            const int points_per_dim[/* dim */],
                            int num_points,
                            int have_local_points,
                            int num_inarrays,
                            const int intypes[/* num_inarrays */],
                            const void *const inarrays[/* num_inarrays */],
                            int num_outvals,
                            CCTK_REAL outvals[/*num_inarrays*num_outvals*/])
{
  int i, total_outvals;
  const char *vtypename;
#ifdef CCTK_MPI
  const pGH *pughGH;
  CCTK_REAL *local_outvals;
#endif


  /* prevent compiler warnings about unused parameters */
  (void) (GH + 0);
  (void) (proc + 0);
  (void) (num_points + 0);

/* macros to complete the ITERATE_ARRAY macro */
#define INITIAL_REDUCTION_VALUE(array)     ((array)[0])
#define REDUCTION_OPERATION(min, scalar)   if (min > scalar) min = scalar

  for (i = total_outvals = 0; i < num_inarrays; i++)
  {
    switch (intypes[i])
    {
      case CCTK_VARIABLE_BYTE:
        ITERATE_ARRAY (CCTK_BYTE, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_BYTE, outvals, num_outvals, total_outvals);
        break;

      case CCTK_VARIABLE_INT:
        ITERATE_ARRAY (CCTK_INT, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_INT, outvals, num_outvals, total_outvals);
        break;

#ifdef CCTK_INT1
      case CCTK_VARIABLE_INT1:
        ITERATE_ARRAY (CCTK_INT1, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_INT1, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_INT2
      case CCTK_VARIABLE_INT2:
        ITERATE_ARRAY (CCTK_INT2, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_INT2, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_INT4
      case CCTK_VARIABLE_INT4:
        ITERATE_ARRAY (CCTK_INT4, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_INT4, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_INT8
      case CCTK_VARIABLE_INT8:
        ITERATE_ARRAY (CCTK_INT8, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_INT8, outvals, num_outvals, total_outvals);
        break;
#endif

      case CCTK_VARIABLE_REAL:
        ITERATE_ARRAY (CCTK_REAL, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL, outvals, num_outvals, total_outvals);
        break;

#ifdef CCTK_REAL4
      case CCTK_VARIABLE_REAL4:
        ITERATE_ARRAY (CCTK_REAL4, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL4, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_REAL8
      case CCTK_VARIABLE_REAL8:
        ITERATE_ARRAY (CCTK_REAL8, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL8, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_REAL16
      case CCTK_VARIABLE_REAL16:
        ITERATE_ARRAY (CCTK_REAL16, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL16, outvals, num_outvals, total_outvals);
        break;
#endif

      default:
        vtypename = CCTK_VarTypeName (intypes[i]);
        if (vtypename && strncmp (vtypename, "CCTK_VARIABLE_COMPLEX", 21) == 0)
        {
          CCTK_WARN (1, "PUGH_ReductionMinVal: Don't know how to compute "
                        "the minimum of complex variables !!!");
        }
        else
        {
          CCTK_WARN (1, "PUGH_ReductionMinVal: Unknown variable type");
        }
        return (-1);
    }
  }

#ifdef CCTK_MPI
  pughGH = PUGH_pGH (GH);

  /* do the global reduction from the processors' local reduction results */
  if (pughGH->nprocs > 1)
  {
    local_outvals = malloc (total_outvals * sizeof (CCTK_REAL));

    if (have_local_points) 
    { 
      /* outvals[] contains now the local minimum */
      memcpy (local_outvals, outvals, total_outvals * sizeof (CCTK_REAL));
    }
    else
    {
      /* initialize local values to be largest possible fp number */
      for (i = 0; i < total_outvals; i++)
      {
        local_outvals[i] = DBL_MAX;
      }
    }
    if (proc < 0)
    {
      CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, outvals, total_outvals,
                        PUGH_MPI_REAL, MPI_MIN, pughGH->PUGH_COMM_WORLD));
    }
    else
    {
      CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, outvals, total_outvals,
                        PUGH_MPI_REAL, MPI_MIN, proc, pughGH->PUGH_COMM_WORLD));
    }
    free (local_outvals);
  }
#endif

  return (0);
}
