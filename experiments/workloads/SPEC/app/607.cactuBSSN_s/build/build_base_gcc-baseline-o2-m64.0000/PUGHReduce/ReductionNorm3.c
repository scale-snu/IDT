/*@@
   @file      ReductionNorm3.c
   @date      Tue Apr 15 2003
   @author    Thomas Radke
   @desc
              Defines the PUGH reduction operator to get the norm
              of an arbitrary array which is defined as:
                  norm3 = $\sqrt[3]{\Sigma |a_i|^3 / np}$
   @enddesc
   @version   $Id: ReductionNorm3.c 65 2005-01-29 17:50:28Z schnetter $
 @@*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pugh_reductions.h"

static const char *rcsid = "$Id: ReductionNorm3.c 65 2005-01-29 17:50:28Z schnetter $";

CCTK_FILEVERSION(CactusPUGH_PUGHReduce_ReductionNorm3_c)

/* local function prototypes */
static int ReductionNorm3 (const cGH *GH,
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
  @routine PUGH_ReductionNorm3Arrays
  @author  Thomas Radke
  @date    Tue Apr 15 2003
  @desc
           Registered PUGH reduction routine for computing the "norm3"
           of a set of arrays.
           The arrays are described by their dimensions and variable type.
           If the number of requested output values equals the number of
           elements in an input array, global reduction is done element-wise.
           Otherwise num_outvals must be 1, and global reduction is done on the
           results of the local reductions.
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
  @var     num_arrays
  @vdesc   number of input arrays
  @vtype   int
  @vio     in
  @endvar
  @var     inarrays
  @vdesc   field of input arrays
  @vtype   const void *const
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
int PUGH_ReductionNorm3Arrays (const cGH *GH,
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
                                ReductionNorm3));
}


/*@@
  @routine PUGH_ReductionNorm3GVs
  @author  Thomas Radke
  @date    Tue Apr 15 2003
  @desc
           PUGH reduction routine to be registered for calculating the norm3.
           It just goes through the list of variables and calls the appropriate
           grouptype reduction routine.
           Global reduction is always done on the results of the local
           reductions, so num_outvals must be 1.
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
  @var     nOutVars
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
int PUGH_ReductionNorm3GVs (const cGH *GH,
                            int proc,
                            int num_outvals,
                            int outtype,
                            void *outvals /* [num_outvals] */,
                            int num_invars,
                            const int invars[/* num_invars */])
{
  return (PUGH_ReductionGVs (GH, proc, num_invars, invars,
                             outtype, num_outvals, outvals,
                             ReductionNorm3));
}


/*****************************************************************************/
/*                             local functions                               */
/*****************************************************************************/
/*@@
   @routine    ReductionNorm3
   @date       Tue Apr 15 2003
   @author     Thomas Radke
   @desc       Returns the "norm3" of a distributed array with
               'num_points' elements. Global reduction is done element-wise
               (num_outvals == 1) or on the results of the local reductions.

               "norm3" is defined as $\sqrt{\Sigma |a_i|^3 / np}$.
   @enddesc
@@*/
static int ReductionNorm3 (const cGH *GH,
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
  const pGH *pughGH;
#ifdef CCTK_MPI
  CCTK_REAL *local_outvals;
#endif


/* macros to complete the ITERATE_ARRAY macro */
#define INITIAL_REDUCTION_VALUE(array)       0


  for (i = total_outvals = 0; i < num_inarrays; i++)
  {
    switch (intypes[i])
    {
      case CCTK_VARIABLE_BYTE:
#define CUBE_ABS(x)      ((x) * (x) * (x))
#define REDUCTION_OPERATION(norm3, scalar)   norm3 += CUBE_ABS (scalar)
        ITERATE_ARRAY (CCTK_BYTE, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_BYTE, outvals, num_outvals, total_outvals);
        break;

      case CCTK_VARIABLE_INT:
#undef  CUBE_ABS
#define CUBE_ABS(x)      ((x) < 0 ? -((x) * (x) * (x)) : (x) * (x) * (x))
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

      case CCTK_VARIABLE_COMPLEX:
#define CUBE(x)      ((x) * (x) * (x))
#undef  REDUCTION_OPERATION
#define REDUCTION_OPERATION(norm1, scalar) \
        norm1 += CUBE (CCTK_CmplxAbs (scalar))
        ITERATE_ARRAY (CCTK_COMPLEX, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL, outvals, num_outvals, total_outvals);
        break;

#ifdef CCTK_REAL4
#undef  REDUCTION_OPERATION
#define REDUCTION_OPERATION(norm1, scalar) \
        norm1 += CUBE (CCTK_Cmplx8Abs (scalar))
        ITERATE_ARRAY (CCTK_COMPLEX8, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL4, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_REAL8
#undef  REDUCTION_OPERATION
#define REDUCTION_OPERATION(norm1, scalar) \
        norm1 += CUBE (CCTK_Cmplx16Abs (scalar))
        ITERATE_ARRAY (CCTK_COMPLEX16, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL8, outvals, num_outvals, total_outvals);
        break;
#endif

#ifdef CCTK_REAL16
#undef  REDUCTION_OPERATION
#define REDUCTION_OPERATION(norm1, scalar) \
        norm1 += CUBE (CCTK_Cmplx32Abs (scalar))
        ITERATE_ARRAY (CCTK_COMPLEX32, num_dims, inarrays[i],
                       from, to, iterator, points_per_dim,
                       CCTK_REAL16, outvals, num_outvals, total_outvals);
        break;
#endif

      default:
        CCTK_WARN (1, "PUGH_ReductionNorm3: Unknown variable type");
        return (-1);
    }
  }

  pughGH = PUGH_pGH (GH);

#ifdef CCTK_MPI
  /* do the global reduction from the processors' local reduction results */
  if (pughGH->nprocs > 1)
  {
    local_outvals = malloc (total_outvals * sizeof (CCTK_REAL));

    if (have_local_points)
    {
      /* outvals[] contains now the local sum */
      memcpy (local_outvals, outvals, total_outvals * sizeof (CCTK_REAL));
    }
    else
    {
      /* initialize local values to zero */
      memset (local_outvals, 0, total_outvals * sizeof (CCTK_REAL));
    }
    if (proc < 0)
    {
      CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, outvals, total_outvals,
                        PUGH_MPI_REAL, MPI_SUM, pughGH->PUGH_COMM_WORLD));
    }
    else
    {
      CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, outvals, total_outvals,
                        PUGH_MPI_REAL, MPI_SUM, proc, pughGH->PUGH_COMM_WORLD));
    }
    free (local_outvals);
  }

#endif

  /* finally assign the return value */
  if (proc < 0 || proc == pughGH->myproc)
  {
    for (i = 0; i < total_outvals; i++)
    {
      outvals[i] = pow (outvals[i] / num_points, 1.0 / 3.0);
    }
  }

  return (0);
}
