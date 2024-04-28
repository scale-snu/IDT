 /*@@
   @file      InterpGridArrays.c
   @date      Tue 10 Dec 2002
   @author    Thomas Radke
   @desc
              Implementation of PUGHInterp's global interpolator routine
              which overloads CCTK_InterpGridArrays()
   @enddesc

   @history
   @date      Tue 10 Dec 2002
   @author    Thomas Radke
   @hdesc     source code copied from Operator.c which implements the old
              API for global interpolation
   @endhistory
   @version   $Id: InterpGridArrays.c 106 2012-11-22 03:32:21Z eschnett $
 @@*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util_ErrorCodes.h"
#include "util_Table.h"
#ifdef SPEC 
#include "PUGHInterp/cctk.h" 
#define THORN_DIR "PUGHInterp" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "CactusPUGH/PUGH/src/include/pugh.h"
#include "pughInterpGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusPUGH_PUGHInterp_InterpGridArrays_c)


/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* uncomment this to get some debugging output */
/* #define PUGHINTERP_DEBUG 1 */

/* fudge factor for mapping points onto processors */
#define FUDGE 0.0

/* macro do sort interpolation results from a single communication buffer
   into their appropriate output arrays */
#define SORT_TYPED_ARRAY(cctk_type)                                           \
        {                                                                     \
          int _i;                                                             \
          cctk_type *_src, *_dst;                                             \
                                                                              \
                                                                              \
          _src = (cctk_type *) this->buf;                                     \
          _dst = (cctk_type *) output_arrays[array];                          \
          for (_i = 0; _i < myGH->N_points_from[proc]; _i++)                  \
          {                                                                   \
            _dst[myGH->indices[_i + offset]] = *_src++;                       \
          }                                                                   \
          this->buf = (char *) _src;                                          \
        }



/********************************************************************
 ***********************    Type Definitions   **********************
 ********************************************************************/
#ifdef CCTK_MPI
/* internal structure describing a handle for a single CCTK data type */
typedef struct
{
  int vtypesize;          /* variable type's size in bytes */
  MPI_Datatype mpitype;   /* corresponding MPI datatype */
  int N_arrays;           /* number of in/out arrays */
  void *sendbuf;          /* communication send buffer for this type */
  void *recvbuf;          /* communication receive buffer for this type */
  char *buf;              /* work pointer for sendbuf */
} type_desc_t;
#endif


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static int PrepareParameterTable (const int *bbox,
                                  int param_table_handle,
                                  int N_dims, int N_input_arrays,
                                  CCTK_INT *input_array_time_levels);


/*@@
   @routine    PUGHInterp_InterpGridArrays
   @date       Mon 16 Dec 2002
   @author     Thomas Radke
   @desc
               PUGHInterp's interpolation routine for distributed grid arrays.
               This routine overloads CCTK_InterpGridArrays().
   @enddesc

   @var        GH
   @vdesc      pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        N_dims
   @vdesc      number of dimensions for the interpolation
   @vtype      int
   @vio        in
   @endvar
   @var        local_interp_handle
   @vdesc      handle which specifies the local interpolator to use
   @vtype      int
   @vio        in
   @endvar
   @var        param_table_handle
   @vdesc      parameter table handle for passing optional parameters to the
               interpolator routine
   @vtype      int
   @vio        in
   @endvar
   @var        coord_system_handle
   @vdesc      handle for the underlying coordinate system
   @vtype      int
   @vio        in
   @endvar
   @var        N_points
   @vdesc      number of points to interpolate at
   @vtype      int
   @vio        in
   @endvar
   @var        interp_coords_type
   @vdesc      CCTK datatype of the coordinate arrays as passed via
               <interp_coords> (common datatype for all arrays)
   @vtype      int
   @vio        in
   @endvar
   @var        interp_coords
   @vdesc      list of <N_dims> arrays with coordinate for <N_points>
               points to interpolate at
   @vtype      const void *const []
   @vio        in
   @endvar
   @var        N_input_arrays
   @vdesc      number of input arrays
   @vtype      int
   @vio        in
   @endvar
   @var        input_array_indices
   @vdesc      list of <N_input_arrays> grid variables (given by their indices)
               to interpolate
   @vtype      const CCTK_INT []
   @vio        in
   @endvar
   @var        N_output_arrays
   @vdesc      number of output arrays
   @vtype      int
   @vio        in
   @endvar
   @var        out_array_types
   @vdesc      list of <N_output_arrays> requested CCTK datatypes for the
               output arrays
   @vtype      const CCTK_INT []
   @vio        in
   @endvar
   @var        output_arrays
   @vdesc      list of <N_output_arrays> output arrays (given by their pointers)
               which receive the interpolation results
   @vtype      void *const []
   @vio        out
   @endvar

   @returntype int
   @returndesc
               0  - successful interpolation
               -1 - in case of any errors
   @endreturndesc
@@*/
int PUGHInterp_InterpGridArrays (const cGH *GH,
                                 int N_dims,
                                 int local_interp_handle,
                                 int param_table_handle,
                                 int coord_system_handle,
                                 int N_points,
                                   int interp_coords_type,
                                   const void *const interp_coords[],
                                 int N_input_arrays,
                                   const CCTK_INT input_array_indices[],
                                 int N_output_arrays,
                                   const CCTK_INT output_array_types[],
                                   void *const output_arrays[])
{
  if (CCTK_IsFunctionAliased ("SymmetryInterpolate")) {
    return SymmetryInterpolate
      (GH, N_dims,
       local_interp_handle, param_table_handle, coord_system_handle,
       N_points, interp_coords_type, interp_coords,
       N_input_arrays, input_array_indices,
       N_output_arrays, output_array_types, output_arrays);
  } else {
    return PUGHInterp_DriverInterpolate
      (GH, N_dims,
       local_interp_handle, param_table_handle, coord_system_handle,
       N_points, interp_coords_type, interp_coords,
       N_input_arrays, input_array_indices,
       N_output_arrays, output_array_types, output_arrays);
  }
}




/*@@
   @routine    PUGHInterp_DriverInterpolate
   @date       2004-05-28
   @author     Erik Schnetter
   @desc
               PUGHInterp's interpolation routine for distributed grid arrays.
               This routine provides the aliased function DriverInterpolate.
   @enddesc

   @var        GH
   @vdesc      pointer to CCTK grid hierarchy
   @vtype      CCTK_POINTER_TO_CONST
   @vio        in
   @endvar
   @var        N_dims
   @vdesc      number of dimensions for the interpolation
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        local_interp_handle
   @vdesc      handle which specifies the local interpolator to use
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        param_table_handle
   @vdesc      parameter table handle for passing optional parameters to the
               interpolator routine
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        coord_system_handle
   @vdesc      handle for the underlying coordinate system
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        N_points
   @vdesc      number of points to interpolate at
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        interp_coords_type
   @vdesc      CCTK datatype of the coordinate arrays as passed via
               <interp_coords> (common datatype for all arrays)
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        interp_coords
   @vdesc      list of <N_dims> arrays with coordinate for <N_points>
               points to interpolate at
   @vtype      const CCTK_POINTER_TO_CONST []
   @vio        in
   @endvar
   @var        N_input_arrays
   @vdesc      number of input arrays
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        input_array_indices
   @vdesc      list of <N_input_arrays> grid variables (given by their indices)
               to interpolate
   @vtype      const CCTK_INT []
   @vio        in
   @endvar
   @var        N_output_arrays
   @vdesc      number of output arrays
   @vtype      CCTK_INT
   @vio        in
   @endvar
   @var        out_array_types
   @vdesc      list of <N_output_arrays> requested CCTK datatypes for the
               output arrays
   @vtype      const CCTK_INT []
   @vio        in
   @endvar
   @var        output_arrays
   @vdesc      list of <N_output_arrays> output arrays (given by their pointers)
               which receive the interpolation results
   @vtype      const CCTK_POINTER []
   @vio        out
   @endvar

   @returntype CCTK_INT
   @returndesc
               0  - successful interpolation
               -1 - in case of any errors
   @endreturndesc
@@*/
CCTK_INT
PUGHInterp_DriverInterpolate (CCTK_POINTER_TO_CONST const GH_,
                              CCTK_INT const N_dims,
                              CCTK_INT const local_interp_handle,
                              CCTK_INT const param_table_handle,
                              CCTK_INT const coord_system_handle,
                              CCTK_INT const N_points,
                              CCTK_INT const interp_coords_type,
                              CCTK_POINTER_TO_CONST const interp_coords [],
                              CCTK_INT const N_input_arrays,
                              CCTK_INT const input_array_indices [],
                              CCTK_INT const N_output_arrays,
                              CCTK_INT const output_array_types [],
                              CCTK_POINTER const output_arrays [])
{
  cGH const * restrict const GH = GH_;
  int i, retval;
#if 0
  int suppress_warnings;
#endif
  CCTK_REAL *origin_local, *delta;
  CCTK_INT *input_array_dims, *input_array_types, *input_array_time_levels;
  const void **input_arrays;
  const char *coord_system_name;
  const pGH *pughGH;
  const pGExtras *extras;
  cGroupDynamicData group_data;
#ifdef CCTK_MPI
  pughInterpGH *myGH;
  CCTK_REAL **interp_coords_local;
  int N_points_local, N_types;
  int j, point, proc, array, offset, type, table_handle;
  CCTK_INT error_point_status;
  CCTK_INT *per_point_status, *proc_status, *overall_status;
  char **output_arrays_local;
  type_desc_t *this, *type_desc;
  CCTK_REAL *range_min, *range_max;
  CCTK_REAL *origin_global;
  CCTK_REAL *interp_coords_proc, *coords, *bbox_local;
  CCTK_REAL **bbox_interp_coords;
#endif


#if 0
  /* check for a "suppress_warnings" key in the parameter table
     to find out whether we should print warnings or not */
  suppress_warnings = Util_TableQueryValueInfo (param_table_handle, NULL,
                                                NULL, "suppress_warnings") == 1;
#endif

  pughGH = CCTK_GHExtension (GH, "PUGH");

  /* check function arguments */
  if (GH == NULL)
  {
    return (UTIL_ERROR_BAD_INPUT);
  }
  if (N_dims <= 0)
  {
    CCTK_WARN (1, "N_dims argument must have a positive value");
    return (UTIL_ERROR_BAD_INPUT);
  }
  if (N_points < 0 || N_input_arrays < 0 || N_output_arrays < 0)
  {
    CCTK_WARN (1, "N_points, N_input_arrays, and N_output_arrays arguments "
               "must have a non-negative value");
    return (UTIL_ERROR_BAD_INPUT);
  }

  /* right now we don't allow query calls only to the local interpolator
     so N_points must be positive and the set of input/output arrays
     must be non-empty */
  if (pughGH->nprocs == 1 && N_points == 0)
  {
    return (0);    /* no error */
  }
  if (N_input_arrays == 0 || N_output_arrays == 0)
  {
    CCTK_WARN (1, "number of input/output arrays must be non-zero");
    return (UTIL_ERROR_BAD_INPUT);
  }
  if ((N_points > 0 && interp_coords == NULL) || input_array_indices == NULL ||
      output_array_types == NULL || output_arrays == NULL)
  {
    CCTK_WARN (1, "input/output array pointer arguments must be non-NULL");
    return (UTIL_ERROR_BAD_INPUT);
  }

  if (interp_coords_type != CCTK_VARIABLE_REAL)
  {
    CCTK_WARN (1, "interpolation coordinates must be of datatype CCTK_REAL");
    return (UTIL_ERROR_BAD_INPUT);
  }

  coord_system_name = CCTK_CoordSystemName (coord_system_handle);
  if (coord_system_name == NULL)
  {
    return (UTIL_ERROR_BAD_HANDLE);
  }
  if (CCTK_CoordSystemDim (coord_system_name) < N_dims)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "coordinate system '%s' has less dimensions than interpolation "
                "coordinates (dim = %d)", coord_system_name, N_dims);
    return (UTIL_ERROR_BAD_INPUT);
  }

  /* get the extras pointer of the first coordinate
     This is used later on to verify the layout of the input arrays as well
     as for mapping points to processors. */
  i = CCTK_CoordIndex (1, NULL, coord_system_name);
  extras = ((const pGA *) pughGH->variables[i][0])->extras;

  /* allocate some temporary arrays */
  origin_local = malloc (2 * N_dims * sizeof (CCTK_REAL));
  delta        = origin_local + N_dims;
  input_arrays = calloc (N_input_arrays, sizeof (void *));
  input_array_dims = calloc (N_dims + 2*N_input_arrays, sizeof (CCTK_INT));
  input_array_types = input_array_dims + N_dims;
  input_array_time_levels = input_array_dims + N_dims + N_input_arrays;

  /* evaluate the options from the user-supplied parameter table */
  retval = PrepareParameterTable (extras->bbox, param_table_handle,
                                  N_dims, N_input_arrays,
                                  input_array_time_levels);
  if (retval < 0)
  {
    free (origin_local);
    free (input_arrays);
    free (input_array_dims);

    return (retval);
  }


  /*************************************************************************/


  /* get the origin and delta of the processor-local grid,
     copy the integer dimension array into an CCTK_INT array */
  for (i = 0; i < N_dims; i++)
  {
    CCTK_CoordLocalRange (GH, &origin_local[i], &delta[i], i + 1, NULL,
                          coord_system_name);
    delta[i] = (delta[i] - origin_local[i]) / extras->lnsize[i];
    input_array_dims[i] = extras->lnsize[i];
  }

  /* check that the input arrays dimensions match the coordinate system
     (but their dimensionality can be less) */
  for (i = retval = 0; i < N_input_arrays; i++)
  {
    /* tolerate negative variable indices which should be treated as no-ops
       for the corresponding input array entries */
    if (input_array_indices[i] < 0)
    {
      continue;
    }

    if (CCTK_GroupDynamicData (GH,
                               CCTK_GroupIndexFromVarI(input_array_indices[i]),
                               &group_data) < 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Invalid input array index %d",
                  input_array_indices[i]);
      retval = UTIL_ERROR_BAD_INPUT;
      continue;
    }

    if (group_data.dim > N_dims)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Input array variable with index %d has more dimensions "
                  "than coordinate system '%s'",
                  input_array_indices[i], coord_system_name);
      retval = UTIL_ERROR_BAD_INPUT;
      continue;
    }

    if (memcmp (group_data.lsh, extras->lnsize, group_data.dim * sizeof (int)))
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Dimensions of input array variable with index %d "
                  "doesn't match with coordinate system '%s'",
                  input_array_indices[i], coord_system_name);
      retval = UTIL_ERROR_BAD_INPUT;
      continue;
    }

    /* get the data pointer to the input array and its datatype */
    input_arrays[i] = CCTK_VarDataPtrI (GH, input_array_time_levels[i],
                                        input_array_indices[i]);
    if (! input_arrays[i])
    {
      char * const fullname = CCTK_FullName (input_array_indices[i]);
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Couldn't get data pointer of input array variable '%s' "
                  "with index %d timelevel %d",
                  fullname ? fullname : "(illegal variable index)",
                  input_array_indices[i], input_array_time_levels[i]);
      free (fullname);
      retval = UTIL_ERROR_BAD_INPUT;
      continue;
    }
    input_array_types[i] = CCTK_VarTypeI (input_array_indices[i]);
  }

  /* single-processor case directly calls the local interpolator */
  if (retval == 0 && pughGH->nprocs == 1)
  {
    retval = CCTK_InterpLocalUniform (N_dims, local_interp_handle,
                                      param_table_handle,
                                      origin_local, delta, N_points,
                                      interp_coords_type, interp_coords,
                                      N_input_arrays, input_array_dims,
                                      input_array_types, input_arrays,
                                      N_output_arrays, output_array_types,
                                      output_arrays);
  }
  if (retval < 0 || pughGH->nprocs == 1)
  {
    free (origin_local);
    free (input_arrays);
    free (input_array_dims);

    /* store this processor's interpolator status in the parameter table */
    if (param_table_handle >= 0)
    {
      Util_TableSetInt(param_table_handle, retval, "local_interpolator_status");
    }
    return (retval);
  }

#ifdef CCTK_MPI
  /*** Here follows the multi-processor case:
       All processors locate their points to interpolate at
       and exchange the coordinates so that every processor gets
       those points which it can process locally.
       After interpolation the results have to be sent back to the
       requesting processors.
       For both communications MPI_Alltoallv() is used.

       In order to minimize the total number of MPI_Alltoallv() calls
       (which are quite expensive) we collect the interpolation results
       for all output arrays of the same CCTK data type into a single
       communication buffer. That means, after communication the data
       needs to be resorted from the buffer into the output arrays.
   ***/

  /* first of all, set up a structure with information of the
     CCTK data types we have to deal with */

  /* get the maximum value of the output array CCTK data types
     NOTE: we assume that CCTK data types are defined as consecutive
           positive constants starting from zero */
  for (array = N_types = 0; array < N_output_arrays; array++)
  {
    if (N_types < output_array_types[array])
    {
      N_types = output_array_types[array];
    }
  }

  /* now allocate an array of structures to describe all possible types */
  type_desc = calloc (N_types + 1, sizeof (type_desc_t));

  /* count the number of arrays of same type
     (the N_arrays element was already initialized to zero by calloc() */
  for (array = 0; array < N_output_arrays; array++)
  {
    type_desc[output_array_types[array]].N_arrays++;
  }

  /* fill in the type description information */
  for (type = retval = 0, this = type_desc; type <= N_types; type++, this++)
  {
    if (this->N_arrays > 0)
    {
      /* get the variable type size in bytes */
      this->vtypesize = CCTK_VarTypeSize (type);
      if (this->vtypesize <= 0)
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "Invalid variable type %d passed, arrays of such type will "
                    "be skipped during interpolation", type);
        this->N_arrays = 0;
        continue;
      }

      /* get the MPI data type to use for communicating such a CCTK data type */
      this->mpitype = PUGH_MPIDataType (pughGH, type);
      if (this->mpitype == MPI_DATATYPE_NULL)
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "No MPI data type defined for variable type %d, arrays of "
                    "such type will be skipped during interpolation", type);
        this->N_arrays = 0;
        continue;
      }

      retval++;
    }
  }

  /* check that there's at least one array with a valid CCTK data type */
  if (retval <= 0)
  {
    free (origin_local);
    free (input_arrays);
    free (input_array_dims);
    free (type_desc);

    return (UTIL_ERROR_BAD_INPUT);
  }

  myGH = CCTK_GHExtension (GH, "PUGHInterp");

  /* map the requested points to interpolate at onto the processors
     they belong to and gather the coordinates of all the points local to
     this processor

     the number of processor-local points is returned in N_points_local,
     their coordinates in interp_coords_local */

  /* this holds the proccessor for *each* of N_points points */
  myGH->whichproc = NULL;
  if (N_points > 0)
  {
    myGH->whichproc = malloc (2 * N_points * sizeof (int));
  }
  /* indices[] is used to make the sorting easier
     when receiving the output data */
  myGH->indices = myGH->whichproc + N_points;

  /* initialize whichproc with invalid processor number -1 */
  for (point = 0; point < N_points; point++)
  {
    myGH->whichproc[point] = -1;
  }

  /* initialize N_points_from to 0 for counting it up in the following loop */
  memset (myGH->N_points_from, 0, pughGH->nprocs * sizeof (CCTK_INT));

  /* allocate the ranges for my local coordinates */
  range_min     = malloc (5 * N_dims * sizeof (CCTK_REAL));
  range_max     = range_min + 1*N_dims;
  origin_global = range_min + 2*N_dims;
  bbox_local    = range_min + 3*N_dims;

  /* get the global origin of the coordinate system */
  for (i = 0; i < N_dims; i++)
  {
    CCTK_CoordRange (GH, &origin_global[i], &range_max[i], i+1, NULL,
                     coord_system_name);
  }

  /* locate the points to interpolate at */
  for (proc = 0; proc < pughGH->nprocs; proc++)
  {
    for (i = 0; i < N_dims; i++)
    {
      /* compute the coordinate ranges */
      /* TODO: use bbox instead -- but the bboxes of other processors
         are not known */
      int const has_lower = extras->lb[proc][i] == 0;
      int const has_upper = extras->ub[proc][i] == extras->nsize[i]-1;
      range_min[i] = origin_global[i] + (extras->lb[proc][i] - FUDGE +
                     (!has_lower) * (extras->nghostzones[i]-0.5)) * delta[i];
      range_max[i] = origin_global[i] + (extras->ub[proc][i] + FUDGE -
                     (!has_upper) * (extras->nghostzones[i]-0.5)) * delta[i];

#ifdef PUGHINTERP_DEBUG
      printf("processor %d: range_min[%d]=%g range_max[%d]=%g\n",
             proc, i, (double) range_min[i], i, (double) range_max[i]);
#endif

      /* save this processor's bbox coordinates */
      if (proc == pughGH->myproc)
      {
        bbox_local[2*i + 0] = range_min[i];
        bbox_local[2*i + 1] = range_max[i];
      }
    }

    /* and now which point will be processed by what processor */
    for (point = 0; point < N_points; point++)
    {
      /* skip points which already have been located */
      if (myGH->whichproc[point] >= 0)
      {
        continue;
      }

      /* check if the point belongs to this processor
         (must be within min/max in all dimensions) */
      for (i = j = 0; i < N_dims; i++)
      {
        if (((const CCTK_REAL *) interp_coords[i])[point] >= range_min[i] &&
            ((const CCTK_REAL *) interp_coords[i])[point] <= range_max[i])
        {
          j++;
        }
      }
      if (j == N_dims)
      {
        myGH->whichproc[point] = proc;
        myGH->N_points_from[proc]++;
      }
    }
  }

  /* Check whether the number of ghostzones is sufficient for the local
     interpolator to handle points on the processor's local bounding box.
     This is done as a query, simply by passing all arguments as for the
     real call to the local interpolator, except for the output_arrays[]
     argument which contains NULL pointers.

     To do this check, we want to set up (up to) 2*N_dims test points, one
     in the center of each interprocessor face of this processor's local
     bounding box; for non-interprocessor faces (i.e. the physical outer
     boundaries of the grid) we don't want to make any test (since there's
     no ghost zone there).

     To simplify the bookkeeping, we actually always set up 2*N_dims points.
     We initialize each point to be at the center of the bounding box, then
     move the interprocessor-face points to the centers of their faces (i.e.
     we move each interprocessor-face points to its bounding-box coordinate
     in the direction perpendicular to its face.  The non-interprocessor-face
     points are left at the center of the bounding box, where their interpolator
     stencils will be "safe" and (we can be sure) never fall off the edge of
     the bounding box.
   */
  bbox_interp_coords = malloc (N_dims * sizeof (CCTK_REAL *));
  bbox_interp_coords[0] = malloc (2 * N_dims * N_dims * sizeof (CCTK_REAL));
  for (i = 1; i < N_dims; i++)
  {
    bbox_interp_coords[i] = bbox_interp_coords[i-1] + 2*N_dims;
  }
  for (j = 0; j < 2 * N_dims; j++)    /* j indices faces of the bounding box */
  {
    /* initialize this point to the middle of the bounding box */
    for (i = 0; i < N_dims; i++)
    {
      bbox_interp_coords[i][j] = 0.5 * (bbox_local[2*i+0] + bbox_local[2*i+1]);
    }
    if (! extras->bbox[j])      /* if this is an interprocessor face... */
    {
      /* ... move the point to the center of this face of the bounding box */
      bbox_interp_coords[j/2][j] = bbox_local[j];
    }
  }
  output_arrays_local = N_output_arrays > 0 ?
                        calloc (N_output_arrays, sizeof (void *)) : NULL;

  retval = CCTK_InterpLocalUniform (N_dims, local_interp_handle,
                                    param_table_handle,
                                    origin_local, delta, 2 * N_dims,
                                    CCTK_VARIABLE_REAL,
                                    (const void *const *) bbox_interp_coords,
                                    N_input_arrays, input_array_dims,
                                    input_array_types, input_arrays,
                                    N_output_arrays, output_array_types,
                                    (void *const *) output_arrays_local);

  /* if the query call returned 'point-outside' error
     we turn this into a 'ghost-size-too-small' error here */
  if (retval == CCTK_ERROR_INTERP_POINT_OUTSIDE)
  {
    retval = CCTK_ERROR_INTERP_GHOST_SIZE_TOO_SMALL;
  }

#if 0
  /* make sure that all points could be mapped onto a processor */
  if (retval == 0)
  {
    for (point = 0; point < N_points; point++)
    {
      if (myGH->whichproc[point] < 0)
      {
        if (! suppress_warnings)
        {
          char *msg = malloc (80 + N_dims*20);
          sprintf (msg, "Unable to locate point %d [%f",
                   point, (double) ((CCTK_REAL *) interp_coords[0])[point]);
          for (i = 1; i < N_dims; i++)
          {
            sprintf (msg, "%s %f",
                     msg, (double) ((CCTK_REAL *) interp_coords[i])[point]);
          }
          sprintf (msg, "%s]", msg);
          CCTK_WARN (1, msg);
          free (msg);
        }
        retval = CCTK_ERROR_INTERP_POINT_OUTSIDE;
      }
    }
  }
#else
  if (! retval)
  {
    /* points which couldn't be mapped onto any processor are now simply
       mapped onto the local one and also passed on to the local interpolator */
    for (point = 0; point < N_points; point++)
    {
      if (myGH->whichproc[point] < 0)
      {
        myGH->whichproc[point] = pughGH->myproc;
        myGH->N_points_from[pughGH->myproc]++;
      }
    }
  }
  else
  {
    /* flag an error condition on this processor's error code in the
       N_points_from[] array which then gets communicated to all processors */
    for (proc = 0; proc < pughGH->nprocs; proc++)
    {
      myGH->N_points_from[proc] = retval;
    }
  }
#endif

  /* don't need this anymore */
  if (output_arrays_local)
  {
    free (output_arrays_local);
  }
  free (bbox_interp_coords[0]);
  free (bbox_interp_coords);
  free (range_min);

  /* Now we want to resolve the N_points_from[]. Currently this is
     the form of ( in 2 proc mode )
     P1:  Num from P1  NFP2
     P2:  NFP1         NFP2

     and this needs to become
     P1:  Num to P1    NTP2
     P2:  NTP1         NTP1

     Since NTP1 = NFP2 (and this works in more proc mode too)
     this is an all-to-all communication.
     */
  CACTUS_MPI_ERROR (MPI_Alltoall (myGH->N_points_from, 1, PUGH_MPI_INT,
                                  myGH->N_points_to, 1, PUGH_MPI_INT,
                                  pughGH->PUGH_COMM_WORLD));

#ifdef PUGHINTERP_DEBUG
  for (proc = 0; proc < pughGH->nprocs; proc++)
  {
    printf ("processor %d <-> %d  From: %d  To: %d\n",
            CCTK_MyProc (GH), proc, myGH->N_points_from[proc],
            myGH->N_points_to[proc]);
  }
#endif

  /* check if there was an error condition on a remote processor
     different processors may flag different error codes - the first one wins */
  for (proc = 0; proc < pughGH->nprocs; proc++)
  {
    if (myGH->N_points_to[proc] < 0)
    {
      retval = myGH->N_points_to[proc];
      break;
    }
  }
  /* collectively return in case of an error */
  if (retval)
  {
    if (myGH->whichproc)
    {
      free (myGH->whichproc);
      myGH->whichproc = NULL;
    }
    free (origin_local);
    free (input_arrays);
    free (input_array_dims);
    free (type_desc);

    /* store this processor's interpolator status in the parameter table */
    if (param_table_handle >= 0)
    {
      Util_TableSetInt (param_table_handle, myGH->N_points_to[pughGH->myproc],
                        "local_interpolator_status");
    }
    return (retval);
  }

  /* Great. Now we know how many to expect from each processor,
     and how many to send to each processor. So first we have
     to send the locations to the processors which hold our data.
     This means I send interp_coords[i][point] to whichproc[point].
     I have N_points_from[proc] to send to each processor.
     */

  /* This is backwards in the broadcast location; the number of points
     we are getting is how many everyone else is sending to us,
     eg, N_points_to, not how many we get back from everyone else,
     eg, N_points_from. The number we are sending, of course, is
     all of our locations, eg, N_points */
  for (proc = N_points_local = 0; proc < pughGH->nprocs; proc++)
  {
    N_points_local += myGH->N_points_to[proc];
  }

#ifdef PUGHINTERP_DEBUG
  printf ("processor %d gets %d points in total\n",
           CCTK_MyProc (GH), N_points_local);
#endif

  /* allocate the local coordinates array (sorted in processor order) */
  interp_coords_proc = NULL;
  if (N_points > 0)
  {
    interp_coords_proc = malloc (N_dims * N_points * sizeof (CCTK_REAL));
  }

  /* now sort my own coordinates as tupels of [N_dims] */
  for (proc = j = 0; proc < pughGH->nprocs; proc++)
  {
    for (point = 0; point < N_points; point++)
    {
      if (myGH->whichproc[point] == proc)
      {
        for (i = 0; i < N_dims; i++)
        {
          interp_coords_proc[N_dims*j + i] =
            ((const CCTK_REAL *) interp_coords[i])[point];
        }
        myGH->indices[j++] = point;
      }
    }
  }

  /* So load up the send and recv stuff */
  /* Send N_dims elements per data point */
  myGH->sendcnt[0] = N_dims * myGH->N_points_from[0];
  myGH->recvcnt[0] = N_dims * myGH->N_points_to[0];
  myGH->senddispl[0] = myGH->recvdispl[0] = 0;
  for (proc = 1; proc < pughGH->nprocs; proc++)
  {
    myGH->sendcnt[proc] = N_dims * myGH->N_points_from[proc];
    myGH->recvcnt[proc] = N_dims * myGH->N_points_to[proc];
    myGH->senddispl[proc] = myGH->senddispl[proc-1] + myGH->sendcnt[proc-1];
    myGH->recvdispl[proc] = myGH->recvdispl[proc-1] + myGH->recvcnt[proc-1];
  }

  /* Great, and now exchange the coordinates and collect the ones
     that I have to process in *interp_coords_local[] */
  coords = NULL;
  if (N_points_local > 0)
  {
    coords = malloc (N_dims * N_points_local * sizeof (CCTK_REAL));
  }
  CACTUS_MPI_ERROR (MPI_Alltoallv (interp_coords_proc, myGH->sendcnt,
                                   myGH->senddispl, PUGH_MPI_REAL,
                                   coords, myGH->recvcnt,
                                   myGH->recvdispl, PUGH_MPI_REAL,
                                   pughGH->PUGH_COMM_WORLD));

  /* don't need this anymore */
  if (interp_coords_proc)
  {
    free (interp_coords_proc);
  }

  /* finally, sort the local coordinates array (which is flat one-dimensional)
     into the interp_coords[N_dim][N_points_local] array */
  interp_coords_local = malloc (N_dims * sizeof (CCTK_REAL *));
  for (i = 0; i < N_dims; i++)
  {
    interp_coords_local[i] = NULL;
    if (N_points_local > 0)
    {
      interp_coords_local[i] = malloc (N_points_local * sizeof (CCTK_REAL));
      for (point = 0; point < N_points_local; point++)
      {
        interp_coords_local[i][point] = coords[point*N_dims + i];
      }
    }
  }

  if (coords)
  {
    free (coords);
  }

  /* Prepare the proc_status[_p_] status array which in the first [nprocs]
     elements will contain the minimum over all per-point status codes as
     returned by CCTK_InterpLocalUniform() for all points requested by
     processor _p_.
     Since the per_point_status is an optional return argument which may not
     be supported by the local interpolator, we allocate the proc_status[]
     array to be all zeros as a default status.
     The last element of the proc_status[] array will keep the return code
     of CCTK_InterpLocalUniform() on processor _p_. */
  proc_status = malloc ((2*(pughGH->nprocs + 1) + N_points_local)
                        * sizeof (CCTK_INT));
  memset (proc_status, 0, (pughGH->nprocs + 1) * sizeof (CCTK_INT));
  overall_status   = proc_status + 1*(pughGH->nprocs + 1);
  per_point_status = proc_status + 2*(pughGH->nprocs + 1);

  /* allocate intermediate output arrays for local interpolation */
  output_arrays_local = calloc (N_output_arrays, sizeof (void *));
  if (N_points_local > 0)
  {
    for (array = 0; array < N_output_arrays; array++)
    {
      this = type_desc + output_array_types[array];
      output_arrays_local[array] = malloc (N_points_local * this->vtypesize);
    }
  }
  else
  {
    /* this indicates a query of the local interpolator whether it will
       return per-point status information */
    per_point_status = NULL;
  }

  /* add the per_point_status[] array the user-supplied table
     (create a new empty table if the user didn't supply one) */
  table_handle = param_table_handle;
  if (table_handle < 0)
  {
    table_handle = Util_TableCreate (UTIL_TABLE_FLAGS_DEFAULT);
  }
  Util_TableSetPointer (table_handle, per_point_status, "per_point_status");

  /* now call the local interpolator for all local points and store
     the results in the intermediate local output arrays */
  proc_status[pughGH->nprocs] =
    CCTK_InterpLocalUniform (N_dims, local_interp_handle, table_handle,
                             origin_local, delta, N_points_local,
                             interp_coords_type,
                             (const void **) interp_coords_local,
                             N_input_arrays, input_array_dims,
                             input_array_types, input_arrays,
                             N_output_arrays, output_array_types,
                             (void **) output_arrays_local);

  /* Now check whether the local interpolator returned per-point
     status information.
     If so, and there were per-point errors returned by the local
     interpolator, then reduce the status values of all points from
     a requesting processor into a single value (ie. reduce
     per_point_status[N_points_local] into proc_status[nprocs]).
     Per definition the minimum overall status value will be taken. */
  i = Util_TableGetInt (table_handle, &error_point_status,"error_point_status");
  if (i == 1 && error_point_status)
  {
    for (proc = 0; proc < pughGH->nprocs; proc++)
    {
      for (point = 0; point < myGH->N_points_to[proc]; point++)
      {
        if (proc_status[proc] > *per_point_status)
        {
          proc_status[proc] = *per_point_status;
        }
        per_point_status++;
      }
    }
  }

  /* remove the internal options from the user-supplied table */
  if (table_handle == param_table_handle)
  {
    Util_TableDeleteKey (table_handle, "error_point_status");
    Util_TableDeleteKey (table_handle, "per_point_status");
  }
  else
  {
    Util_TableDestroy (table_handle);
  }

  /* Now do the global reduction of all per-processor status values.
     This will then be taken as the return code to the caller. */
  CACTUS_MPI_ERROR (MPI_Allreduce (proc_status, overall_status,
                                   pughGH->nprocs + 1, PUGH_MPI_INT, MPI_MIN,
                                   pughGH->PUGH_COMM_WORLD));

  /* if the local interpolator returned per-point status information then
     store this processor's local interpolator status in the parameter table */
  if (i == 1 && param_table_handle >= 0)
  {
    Util_TableSetInt (param_table_handle, overall_status[pughGH->myproc],
                      "local_interpolator_status");
  }

  /* set the return code to the overall processors' local interpolator status */
  retval = overall_status[pughGH->nprocs];

  /* clean up some intermediate arrays */
  if (N_points_local > 0)
  {
    for (i = 0; i < N_dims; i++)
    {
      free (interp_coords_local[i]);
    }
  }
  free (proc_status);
  free (interp_coords_local);
  free (origin_local);
  free (input_arrays);
  free (input_array_dims);

  /* now send the interpolation results to the processors which requested them,
     and also receive my own results that were computed remotely.
     Before we can do the communication in one go (for each datatype, of course)
     we have to sort the results from the intermediate output arrays, which the
     local interpolator wanted, into a single contiguous communication buffer.*/
  for (type = 0, this = type_desc; type <= N_types; type++, this++)
  {
    /* skip unused types */
    if (this->N_arrays <= 0)
    {
      continue;
    }

    /* set up the communication (this is type-independent) */
    myGH->sendcnt[0] = this->N_arrays * myGH->N_points_to[0];
    myGH->recvcnt[0] = this->N_arrays * myGH->N_points_from[0];
    myGH->senddispl[0] = myGH->recvdispl[0] = 0;
    for (proc = 1; proc < pughGH->nprocs; proc++)
    {
      myGH->sendcnt[proc] = this->N_arrays * myGH->N_points_to[proc];
      myGH->recvcnt[proc] = this->N_arrays * myGH->N_points_from[proc];
      myGH->senddispl[proc] = myGH->senddispl[proc-1] + myGH->sendcnt[proc-1];
      myGH->recvdispl[proc] = myGH->recvdispl[proc-1] + myGH->recvcnt[proc-1];
    }

    /* Allocate contiguous communication buffer for each datatype into which
       the local interpolation results from all input arrays of that datatype
       will be written to.
       If there are no points to send/receive by this processor
       set the buffer pointer to an invalid but non-NULL value
       otherwise we might get trouble with NULL pointers in MPI_Alltoallv () */

    this->sendbuf = this->recvbuf = NULL;

    /* here goes the allocation for the send buffer, along with copying the
       results from the intermediate local output arrays */
    if (N_points_local > 0)
    {
      this->buf = malloc (this->N_arrays * N_points_local * this->vtypesize);
      this->sendbuf = this->buf;

      for (proc = offset = 0; proc < pughGH->nprocs; proc++)
      {
        for (array = 0; array < N_output_arrays; array++)
        {
          if (output_array_types[array] != type)
          {
            continue;
          }

          memcpy (this->buf, output_arrays_local[array] + offset,
                  myGH->N_points_to[proc] * this->vtypesize);
          this->buf += myGH->N_points_to[proc] * this->vtypesize;
        }
        offset += myGH->N_points_to[proc] * this->vtypesize;
      }
    }

    /* receive buffer is easy */
    if (N_points > 0)
    {
      this->recvbuf = malloc (this->N_arrays * N_points*this->vtypesize);
    }

    /* now do the global exchange for this datatype */
    CACTUS_MPI_ERROR (MPI_Alltoallv (this->sendbuf, myGH->sendcnt,
                                     myGH->senddispl, this->mpitype,
                                     this->recvbuf, myGH->recvcnt,
                                     myGH->recvdispl, this->mpitype,
                                     pughGH->PUGH_COMM_WORLD));

    /* now that the data is sent we don't need the send buffer anymore */
    if (N_points_local > 0)
    {
      free (this->sendbuf);
    }

    /* no sort neccessary if there are no points */
    if (N_points <= 0)
    {
      continue;
    }

    /* go back from processor-sorted data to input-ordered data.
       The creation of the indices array above makes this not so bad. */
    this->buf = this->recvbuf;
    for (proc = offset = 0; proc < pughGH->nprocs; proc++)
    {
      if (myGH->N_points_from[proc] <= 0)
      {
        continue;
      }

      for (array = 0; array < N_output_arrays; array++)
      {
        if (output_array_types[array] != type || ! output_arrays[array])
        {
          continue;
        }

        /* now do the sorting according to the CCTK data type */
        switch (output_array_types[array])
        {
          case CCTK_VARIABLE_BYTE:      SORT_TYPED_ARRAY (CCTK_BYTE); break;
          case CCTK_VARIABLE_INT:       SORT_TYPED_ARRAY (CCTK_INT); break;
          case CCTK_VARIABLE_REAL:      SORT_TYPED_ARRAY (CCTK_REAL); break;
          case CCTK_VARIABLE_COMPLEX:   SORT_TYPED_ARRAY (CCTK_COMPLEX); break;
#ifdef CCTK_REAL4
          case CCTK_VARIABLE_REAL4:     SORT_TYPED_ARRAY (CCTK_REAL4); break;
          case CCTK_VARIABLE_COMPLEX8:  SORT_TYPED_ARRAY (CCTK_COMPLEX8); break;
#endif
#ifdef CCTK_REAL8
          case CCTK_VARIABLE_REAL8:     SORT_TYPED_ARRAY (CCTK_REAL8); break;
          case CCTK_VARIABLE_COMPLEX16: SORT_TYPED_ARRAY (CCTK_COMPLEX16);break;
#endif
#ifdef CCTK_REAL16
          case CCTK_VARIABLE_REAL16:    SORT_TYPED_ARRAY (CCTK_REAL16); break;
          case CCTK_VARIABLE_COMPLEX32: SORT_TYPED_ARRAY (CCTK_COMPLEX32);break;
#endif
          default: CCTK_WARN (0, "Implementation error");
        }

      } /* end of loop over all output arrays */

      /* advance the offset into the communication receive buffer */
      offset += myGH->N_points_from[proc];

    } /* end of loop over all processors */

    /* this communication receive buffer isn't needed anymore */
    free (this->recvbuf);

  } /* end of loop over all types */

  /* free intermediate output arrays */
  for (array = 0; array < N_output_arrays; array++)
  {
    if (output_arrays_local[array])
    {
      free (output_arrays_local[array]);
    }
  }
  free (output_arrays_local);

  /* free remaining resources allocated within this run */
  if (myGH->whichproc)
  {
    free (myGH->whichproc);
    myGH->whichproc = NULL;
  }
  free (type_desc);
#endif /* MPI */

  return (retval);
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
 /*@@
   @routine    PrepareParameterTable
   @date       Fri 20 Dec 2002
   @author     Thomas Radke
   @desc
               Parses the 'input_array_time_levels' option from the
               user-supplied parameter table and sets up the bounding box
               option arrays for the local interpolator
   @enddesc

   @var        bbox
   @vdesc      bounding box of the underlying coordinate system
   @vtype      const int *
   @vio        in
   @endvar
   @var        param_table_handle
   @vdesc      handle to the user-supplied parameter table
   @vtype      int
   @vio        in
   @endvar
   @var        N_dims
   @vdesc      number of dimensions in which to interpolate
   @vtype      int
   @vio        in
   @endvar
   @var        N_input_arrays
   @vdesc      total number of input arrays
   @vtype      int
   @vio        in
   @endvar
   @var        input_array_time_levels
   @vdesc      pointer to array of timelevels for input arrays
   @vtype      CCTK_INT *
   @vio        out
   @endvar

   @returntype int
   @returndesc
               0 for success, or one of the UTIL_ERROR_TABLE_* error codes
   @endreturndesc
@@*/
static int PrepareParameterTable (const int *bbox,
                                  int param_table_handle,
                                  int N_dims, int N_input_arrays,
                                  CCTK_INT *input_array_time_levels)
{
  int i, retval;
  CCTK_INT *N_boundary_points_to_omit;
  CCTK_REAL *boundary_off_centering_tolerance,*boundary_extrapolation_tolerance;


  retval = 0;

  if (param_table_handle < 0)
  {
    CCTK_WARN (1, "Invalid parameter table handle passed");
    return (UTIL_ERROR_BAD_HANDLE);
  }

  /* allocate table option arrays and initialize them to their defaults */
  N_boundary_points_to_omit = malloc (2 * N_dims * sizeof (CCTK_INT));
  boundary_off_centering_tolerance = malloc (2*2 * N_dims * sizeof (CCTK_REAL));
  boundary_extrapolation_tolerance = boundary_off_centering_tolerance +2*N_dims;
  for (i = 0; i < 2 * N_dims; i++)
  {
    N_boundary_points_to_omit[i] = 0;
    boundary_off_centering_tolerance[i] = 999.0;
    boundary_extrapolation_tolerance[i] = 1e-10;
  }

  /* read the 'N_boundary_points_to_omit' options array from the table
     and verify its datatype and element count */
  i = Util_TableGetIntArray (param_table_handle, 2 * N_dims,
                             N_boundary_points_to_omit,
                             "N_boundary_points_to_omit");
  if (i < 0 && i != UTIL_ERROR_TABLE_NO_SUCH_KEY)
  {
    CCTK_WARN (1, "Options array with key 'N_boundary_points_to_omit' "
                  "must be of CCTK_INT datatype");
    retval = UTIL_ERROR_BAD_INPUT;
  }
  else if (i >= 0 && i < 2 * N_dims)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Options array with key 'N_boundary_points_to_omit' "
                "must have exactly 2 * N_dims (= %d) elements", N_dims);
    retval = UTIL_ERROR_BAD_INPUT;
  }

  /* read the 'boundary_off_centering_tolerance' options array from the table
     and verify its datatype and element count */
  i = Util_TableGetRealArray (param_table_handle, 2 * N_dims,
                              boundary_off_centering_tolerance,
                              "boundary_off_centering_tolerance");
  if (i < 0 && i != UTIL_ERROR_TABLE_NO_SUCH_KEY)
  {
    CCTK_WARN (1, "Options array with key 'boundary_off_centering_tolerance' "
                  "must be of CCTK_REAL datatype");
    retval = UTIL_ERROR_BAD_INPUT;
  }
  else if (i >= 0 && i < 2 * N_dims)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Options array with key 'boundary_off_centering_tolerance' "
                "must have exactly 2 * N_dims (= %d) elements", N_dims);
    retval = UTIL_ERROR_BAD_INPUT;
  }

  /* read the 'boundary_extrapolation_tolerance' options array from the table
     and verify its datatype and element count */
  i = Util_TableGetRealArray (param_table_handle, 2 * N_dims,
                              boundary_extrapolation_tolerance,
                              "boundary_extrapolation_tolerance");
  if (i < 0 && i != UTIL_ERROR_TABLE_NO_SUCH_KEY)
  {
    CCTK_WARN (1, "Options array with key 'boundary_extrapolation_tolerance' "
                  "must be of CCTK_REAL datatype");
    retval = UTIL_ERROR_BAD_INPUT;
  }
  else if (i >= 0 && i != 2 * N_dims)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Options array with key 'boundary_extrapolation_tolerance' "
                "must have exactly 2 * N_dims (= %d) elements", N_dims);
    retval = UTIL_ERROR_BAD_INPUT;
  }

  /* read the 'input_array_time_levels' options array from the table
     and verify its datatype and element count */
  i = Util_TableGetIntArray (param_table_handle, N_input_arrays,
                             input_array_time_levels,
                             "input_array_time_levels");
  if (i == UTIL_ERROR_TABLE_NO_SUCH_KEY)
  {
    /* if no such option was specified, take the current timelevel as default */
    memset (input_array_time_levels, 0, N_input_arrays * sizeof (CCTK_INT));
  }
  else if (i < 0)
  {
    CCTK_WARN (1, "Options array with key 'input_array_time_levels' must be of "
                  "CCTK_INT datatype");
    retval = UTIL_ERROR_BAD_INPUT;
  }
  else if (i != N_input_arrays)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Options array with key 'input_array_time_levels' must have "
                "exactly N_input_arrays (= %d) elements", N_input_arrays);
    retval = UTIL_ERROR_BAD_INPUT;
  }

  /* complete the bounding box arrays and set them in the parameter table */
  for (i = 0; i < 2 * N_dims; i++)
  {
    if (! bbox[i])
    {
      N_boundary_points_to_omit[i] = 0;
      boundary_off_centering_tolerance[i] = 0.0;
      boundary_extrapolation_tolerance[i] = 0.0;
    }
  }
  if (! retval)
  {
    Util_TableSetIntArray (param_table_handle, 2 * N_dims,
                           N_boundary_points_to_omit,
                           "N_boundary_points_to_omit");
    Util_TableSetRealArray (param_table_handle, 2 * N_dims,
                            boundary_off_centering_tolerance,
                            "boundary_off_centering_tolerance");
    Util_TableSetRealArray (param_table_handle, 2 * N_dims,
                            boundary_extrapolation_tolerance,
                            "boundary_extrapolation_tolerance");
  }

  /* free allocated resources */
  free (N_boundary_points_to_omit);
  free (boundary_off_centering_tolerance);

  return (retval);
}
