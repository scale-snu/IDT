 /*@@
   @file      ReductionCount.c
   @date      Thu Apr  3 11:54:53 1997
   @author    Thomas Radke
   @desc
              Defines the PUGH reduction operator "count" to get the number of
              grid points of an arbitrary array.
   @enddesc
   @version   $Id: ReductionCount.c 42 2003-07-09 16:04:43Z tradke $
 @@*/

#include <stdlib.h>
#include <string.h>

#include "pugh_reductions.h"

static const char *rcsid = "$Id: ReductionCount.c 42 2003-07-09 16:04:43Z tradke $";

CCTK_FILEVERSION(CactusPUGH_PUGHReduce_ReductionCount_c)

/* local function prototypes */
static int ReductionCount (const cGH *GH,
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
  @routine PUGH_ReductionCountArrays
  @author  Thomas Radke
  @date    19 Aug 1999
  @desc
           Registered PUGH reduction routine for computing the counts
           of a set of arrays.
           The arrays are described by their dimensions and variable type.
           For the number of output values only 1 is accepted.
           Type casting of the result is provided by specifying the
           requested output datatype. The intermediate reduction value
           is always computed as a CCTK_REAL value internally.
  @enddesc
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
  @var     nDims
  @vdesc   number of dimensions of input arrays
  @vtype   int
  @vio     in
  @endvar
  @var     dims
  @vdesc   dimensions of input arrays
  @vtype   const int *
  @vio     in
  @endvar
  @var     nArrays
  @vdesc   number of input arrays
  @vtype   int
  @vio     in
  @endvar
  @var     inarrays
  @vdesc   field of input arrays
  @vtype   const void *const
  @vio     in
  @endvar
  @var     inType
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
int PUGH_ReductionCountArrays (const cGH *GH,
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
                                ReductionCount));
}


/*@@
  @routine PUGH_ReductionCountGVs
  @author  Thomas Radke
  @date    23 Apr 1999
  @desc
           PUGH reduction routine to be registered for calculating the count.
           It just goes through the list of variables and calls the appropriate
           grouptype reduction routine.
           If the number of requested output values equals the size of the
           variable, global reduction is done element-wise.
           Otherwise num_outvals must be 1, and global reduction is done on the
           results of the local reductions.
  @enddesc
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
int PUGH_ReductionCountGVs (const cGH *GH,
                          int proc,
                          int num_outvals,
                          int outtype,
                          void *outvals /* [num_outvals] */,
                          int num_invars,
                          const int invars[/* num_invars */])
{
  return (PUGH_ReductionGVs (GH, proc, num_invars, invars,
                             outtype, num_outvals, outvals,
                             ReductionCount));
}


/*****************************************************************************/
/*                             local functions                               */
/*****************************************************************************/
/*@@
   @routine    ReductionCount
   @date       Aug 19 1999
   @author     Thomas Radke
   @desc
               Returns the number of grid points of a distributed array
   @enddesc
@@*/
static int ReductionCount (const cGH *GH,
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
  int i;


  /* avoid compiler warnings about unused parameters */
  (void) (GH + 0);
  (void) (proc + 0);
  (void) (num_dims + 0);
  (void) (from + 0);
  (void) (to + 0);
  (void) (iterator + 0);
  (void) (points_per_dim + 0);
  (void) (have_local_points + 0);
  (void) (intypes + 0);
  (void) (inarrays + 0);
  (void) (num_outvals + 0);

  /* assign the return value */
  if (proc < 0 || proc == CCTK_MyProc (GH))
  {
    for (i = 0; i < num_inarrays; i++)
    {
      outvals[i] = num_points;
    }
  }

  return (0);
}
