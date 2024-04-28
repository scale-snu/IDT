 /*@@
   @file      Reduction.c
   @date      Thu Apr  3 11:54:53 1997
   @author    Thomas Radke, Paul Walker
   @desc
              Various MPI reduction operators.
   @enddesc
   @version   $Id: Reduction.c 100 2012-07-04 01:29:22Z eschnett $
 @@*/

#include <stdlib.h>

#include "pugh_reductions.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGHReduce_Reduction_c)

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

static int ReductionGA (const cGH *GH,
                        int vindex,
                        int proc,
                        CCTK_REAL *outval,
                        reduction_fn_t reduction_fn);

static int ReductionScalar (const cGH *GH, int vindex, int proc,
                            CCTK_REAL *outval,
                            reduction_fn_t reduction_fn);

static int copy_real_to_outtype (int num_elems,
                                 CCTK_REAL inarray [/* num_elems */],
                                 int outtype,
                                 void *outarray     /* [num_elems] */);


/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

 /*@@
   @routine    PUGH_ReductionArrays
   @author     Thomas Radke
   @date       19 Aug 1999
   @desc
               Wrapper to reduce a list of arrays.
               Just calls the appropriate reduction operator and does
               the type conversion of the results.
   @enddesc
   @calls      copy_real_to_outtype

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        proc
   @vdesc      processor that should receive the result of operation
               (negative value means all processors receive the result)
   @vtype      int
   @vio        in
   @endvar
   @var        num_dims
   @vdesc      number of dimensions of input arrays
   @vtype      int
   @vio        in
   @endvar
   @var        dims
   @vdesc      dimensions of input arrays
   @vtype      int *
   @vio        in
   @endvar
   @var        intype
   @vdesc      (common) variable type of input arrays
   @vtype      int
   @vio        in
   @endvar
   @var        num_inarrays
   @vdesc      number of input arrays
   @vtype      int
   @vio        in
   @endvar
   @var        inarrays
   @vdesc      field of input arrays
   @vtype      const void *const *
   @vio        in
   @endvar
   @var        outtype
   @vdesc      (common) variable type of output arrays
   @vtype      int
   @vio        in
   @endvar
   @var        num_outvals
   @vdesc      number of values per output array
   @vtype      int
   @vio        in
   @endvar
   @var        outvals
   @vdesc      pointer to buffer holding the output values
   @vtype      void *
   @vio        in
   @endvar
   @var        reduction_fn
   @vdesc      reduction operator callback
   @vtype      reduction_fn_t
   @vio        in
   @endvar

   @returntype int
   @returndesc
               the return code of the reduction operator, or<BR>
               -1  if array size is zero<BR>
               -2  if <num_outvals> is invalid
   @endreturndesc
@@*/
int PUGH_ReductionArrays (const cGH *GH,
                          int proc,
                          int num_dims,
                          const int dims[/* num_dims */],
                          int intype,
                          int num_inarrays,
                          const void *const inarrays[/* num_inarrays */],
                          int outtype,
                          int num_outvals,
                          void *outvals /* [num_outvals] */,
                          reduction_fn_t reduction_fn)
{
  int i, num_points, retval;
  int from[1], to[1], iterator[1], points_per_dim[1];
  int *intypes;
  CCTK_REAL *buffer;


  points_per_dim[0] = 1;
  from[0] = 0;
  to[0] = dims[0];

  /* get the total number of array elements */
  for (i = 1; i < num_dims; i++)
  {
    to[0] *= dims[i];
  }

  /* check for zero-sized arrays */
  if (to[0] <= 0)
  {
    CCTK_WARN (2, "PUGH_ReductionArrays: Cannot reduce zero-sized arrays");
    return (-1);
  }

  if (num_outvals != 1)
  {
    if (num_outvals != to[0])
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_ReductionArrays: Don't know how to reduce "
                  "a %d-dimensional array with %d elements "
                  "to an output array of %d elements",
                  num_dims, to[0], num_outvals);
      return (-2);
    }
    to[0] = 1;
  }
  num_points = to[0] * CCTK_nProcs (GH);

  /* set the array types to intype */
  /* FIXME: could allow to pass in arrays of different types now !!! */
  intypes = malloc (num_inarrays * sizeof (int));
  for (i = 0; i < num_inarrays; i++)
  {
    intypes[i] = intype;
  }

  buffer = malloc (num_outvals * sizeof (CCTK_REAL));

  /* do the reduction on the input arrays */
  retval = reduction_fn (GH, proc, num_dims, from, to, iterator, points_per_dim,
                         num_points, 1, num_inarrays, intypes, inarrays,
                         num_outvals, buffer);

  if (retval == 0 && (proc < 0 || proc == CCTK_MyProc (GH)))
  {
    /* type-cast the result to the requested datatype */
    retval = copy_real_to_outtype (num_inarrays * num_outvals,
                                   buffer, outtype, outvals);
  }

  free (intypes);
  free (buffer);

  return (retval);
}


 /*@@
   @routine    PUGH_ReductionGVs
   @author     Thomas Radke
   @date       19 Aug 1999
   @desc
               Wrapper to reduce a list of grid variables.
               Just calls the appropriate reduction operator and does
               the type conversion of the results.
   @enddesc
   @calls      CCTK_VarTypeSize
               CCTK_GroupTypeFromVarI
               ReductionGA
               ReductionScalar
               copy_real_to_outtype

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        proc
   @vdesc      processor that should receive the result of operation
               (negative value means all processors receive the result)
   @vtype      int
   @vio        in
   @endvar
   @var        num_invars
   @vdesc      number of input arrays
   @vtype      int
   @vio        in
   @endvar
   @var        invars
   @vdesc      field of input arrays
   @vtype      const int *
   @vio        in
   @endvar
   @var        outtype
   @vdesc      (common) variable type of output arrays
   @vtype      int
   @vio        in
   @endvar
   @var        num_outvals
   @vdesc      number of values per output array
   @vtype      int
   @vio        in
   @endvar
   @var        outvals
   @vdesc      pointer to buffer holding the output values
   @vtype      void *
   @vio        in
   @endvar
   @var        reduction_fn
   @vdesc      reduction operator callback
   @vtype      reduction_fn_t
   @vio        in
   @endvar

   @returntype int
   @returndesc
               the return code of the reduction operator
   @endreturndesc
@@*/
int PUGH_ReductionGVs (const cGH *GH,
                       int proc,
                       int num_invars,
                       const int invars[/* num_invars */],
                       int outtype,
                       int num_outvals,
                       void *outvals /* [num_outvals] */,
                       reduction_fn_t reduction_fn)
{
  int i, myproc, outtypesize, this_retval, retval;
  CCTK_REAL result;


  if (num_outvals != 1)
  {
    CCTK_WARN (1, "PUGH_ReductionGVs: Only one output value per "
                  "input array allowed");
    return (-1);
  }

  if (outvals == NULL)
  {
    CCTK_WARN (1, "PUGH_ReductionGVs: NULL pointer passed for output array");
    return (-1);
  }

  outtypesize = CCTK_VarTypeSize (outtype);
  if (outtypesize <= 0)
  {
    CCTK_WARN (1, "PUGH_ReductionGVs: Invalid output variable type");
    return (-1);
  }

  retval = 0;
  myproc = CCTK_MyProc (GH);

  for (i = 0; i < num_invars; i++)
  {
    switch (CCTK_GroupTypeFromVarI (invars[i]))
    {
      case CCTK_GF:
      case CCTK_ARRAY:
        this_retval = ReductionGA (GH, invars[i], proc, &result,
                                   reduction_fn);
        break;

      case CCTK_SCALAR:
        this_retval = ReductionScalar (GH, invars[i], proc, &result,
                                       reduction_fn);
        break;

      default:
        CCTK_WARN (1, "PUGH_ReductionGVs: Invalid variable index");
        this_retval = -1;
        break;
    }

    if (this_retval == 0 && (proc < 0 || proc == myproc))
    {
      /* type-cast the result to the requested datatype */
      this_retval = copy_real_to_outtype (1, &result, outtype,
                                          (char *) outvals + i*outtypesize);
    }

    retval |= this_retval;
  }

  return (retval);
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/

 /*@@
   @routine    ReductionGA
   @date       Fri Jun 29 15:35:01 2001
   @author     Tom Goodale
   @desc
               Reduction of a grid array variable.
   @enddesc

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        vindex
   @vdesc      The GV index
   @vtype      int
   @vio        in
   @endvar
   @var        proc
   @vdesc      The processor at which we desire the result
   @vtype      int
   @vio        in
   @endvar
   @var        outval
   @vdesc      buffer to store the reduction result in
   @vtype      CCTK_REAL *
   @vio        out
   @endvar
   @var        reduction_fn
   @vdesc      The function which does the actual reduction
   @vtype      reduction_fn_t
   @vio        in
   @endvar

   @returntype int
   @returndesc
               The return code of the reduction operator.
   @endreturndesc
@@*/
static int ReductionGA (const cGH *GH, int vindex, int proc, CCTK_REAL *outval,
                        reduction_fn_t reduction_fn)
{
  int i, num_points, dir_points, retval;
  pGA *GA;
  const void *data;
  int *from, *to, *iterator, *points_per_dim;


  /* get the GA structure for the current timelevel */
  GA = ((pGA ***) PUGH_pGH (GH)->variables)[vindex][0];
  data = GA->data;

  /* allocate temporary vectors */
  from           = malloc (4 * GA->connectivity->dim * sizeof (int));
  to             = from + 1*GA->connectivity->dim;
  iterator       = from + 2*GA->connectivity->dim;
  points_per_dim = from + 3*GA->connectivity->dim;

  /* get the start- and endpoint to iterate over, the total number of points,
     and the points_per_dim[] vector */
  num_points = 1;
  points_per_dim[0] = 1;
  for (i = 0; i < GA->connectivity->dim; i++)
  {
    /* get the start- and endpoint to iterate over in this dimension */
    from[i] = GA->extras->ownership[0][i];
    to[i]   = GA->extras->ownership[1][i];

    /* get the number of points in this dimension (excluding ghostzones) */
    dir_points = GA->extras->nsize[i];
    if (GA->connectivity->perme[i])
    {
      dir_points -= 2*GA->extras->nghostzones[i];
    }

    /* accumulate the total number of points the reduce */
    num_points *= dir_points;

    if (i > 0)
    {
      points_per_dim[i] = points_per_dim[i-1] * GA->extras->lnsize[i-1];
    }
  }

  /* now do the reduction */
  retval = reduction_fn (GH, proc, GA->connectivity->dim, from, to, iterator,
                         points_per_dim, num_points, GA->extras->npoints > 0, 1,
                         &GA->vtype, &data, 1, outval);

  /* free temporary vectors */
  free (from);

  return (retval);
}


 /*@@
   @routine    ReductionScalar
   @date       Fri Jun 29 15:35:01 2001
   @author     Tom Goodale
   @desc
               Reduction of a scalar GV.
               Basically a copy of ReductionGA cut down to the scalar case.
   @enddesc

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        vindex
   @vdesc      The GV index
   @vtype      int
   @vio        in
   @endvar
   @var        proc
   @vdesc      The processor at which we desire the result
   @vtype      int
   @vio        in
   @endvar
   @var        outval
   @vdesc      buffer to store the reduction result in
   @vtype      CCTK_REAL *
   @vio        out
   @endvar
   @var        reduction_fn
   @vdesc      The function which does the actual reduction
   @vtype      reduction_fn_t
   @vio        in
   @endvar

   @returntype int
   @returndesc
               The return code of the reduction operator.
   @endreturndesc
@@*/
static int ReductionScalar (const cGH *GH,
                            int vindex,
                            int proc,
                            CCTK_REAL *outval,
                            reduction_fn_t reduction_fn)
{
  int retval;
  int num_points, from, to, iterator, points_per_dim, type;
  pGA *GA;
  const void *data;


  /* get the GA structure for the current timelevel */
  GA = ((pGA ***) PUGH_pGH (GH)->variables)[vindex][0];
  data = GA->data;

  from           = 0;
  to             = 1;
  iterator       = 1;
  points_per_dim = 1;
  type           = CCTK_VarTypeI (vindex);

  num_points = 1;

  /* now do the reduction */
  retval = reduction_fn (GH, proc, 1, &from, &to, &iterator,
                         &points_per_dim, num_points, 1, 1, &type, &data,
                         1, outval);

  return retval;
}


 /*@@
   @routine    copy_real_to_outtype
   @author     Thomas Radke
   @date       19 Aug 1999
   @desc
               Does the type conversion from CCTK_REAL into the requested
               datatype.
   @enddesc
   @calls      CCTK_VarTypeSize
               CCTK_GroupTypeFromVarI
               ReductionGA
               ReductionScalar
               copy_real_to_outtype

   @var        num_elems
   @vdesc      number of elements to convert
   @vtype      int
   @vio        in
   @endvar
   @var        inarray
   @vdesc      input array with results of reductions
   @vtype      CCTK_REAL *
   @vio        in
   @endvar
   @var        outtype
   @vdesc      requested output datatype
   @vtype      int
   @vio        in
   @endvar
   @var        outarray
   @vdesc      pointer to output buffer to store the converted results
   @vtype      void *
   @vio        out
   @endvar

   @returntype int
   @returndesc
               0  for success, or<BR>
               -1 if conversion into target datatype is not supported.
   @endreturndesc
@@*/
static int copy_real_to_outtype (int num_elems,
                                 CCTK_REAL inarray[/* num_elems */],
                                 int outtype,
                                 void *outarray /* [num_elems] */)
{
  int i, retval;


  retval = 0;

  if (outtype == CCTK_VARIABLE_BYTE)
  {
    CCTK_BYTE *_outarray = (CCTK_BYTE *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_BYTE) inarray[i];
    }
  }
  else if (outtype == CCTK_VARIABLE_INT)
  {
    CCTK_INT *_outarray = (CCTK_INT *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_INT) inarray[i];
    }
  }
#ifdef CCTK_INT1
  else if (outtype == CCTK_VARIABLE_INT1)
  {
    CCTK_INT1 *_outarray = (CCTK_INT1 *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_INT1) inarray[i];
    }
  }
#endif
#ifdef CCTK_INT2
  else if (outtype == CCTK_VARIABLE_INT2)
  {
    CCTK_INT2 *_outarray = (CCTK_INT2 *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_INT2) inarray[i];
    }
  }
#endif
#ifdef CCTK_INT4
  else if (outtype == CCTK_VARIABLE_INT4)
  {
    CCTK_INT4 *_outarray = (CCTK_INT4 *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_INT4) inarray[i];
    }
  }
#endif
#ifdef CCTK_INT8
  else if (outtype == CCTK_VARIABLE_INT8)
  {
    CCTK_INT8 *_outarray = (CCTK_INT8 *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_INT8) inarray[i];
    }
  }
#endif
  else if (outtype == CCTK_VARIABLE_REAL)
  {
    CCTK_REAL *_outarray = (CCTK_REAL *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_REAL) inarray[i];
    }
  }
#ifdef CCTK_REAL4
  else if (outtype == CCTK_VARIABLE_REAL4)
  {
    CCTK_REAL4 *_outarray = (CCTK_REAL4 *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_REAL4) inarray[i];
    }
  }
#endif
#ifdef CCTK_REAL8
  else if (outtype == CCTK_VARIABLE_REAL8)
  {
        CCTK_REAL8 *_outarray = (CCTK_REAL8 *) outarray;


        for (i = 0; i < num_elems; i++)
        {
          _outarray[i] = (CCTK_REAL8) inarray[i];
        }
  }
#endif
#ifdef CCTK_REAL16
  else if (outtype == CCTK_VARIABLE_REAL16)
  {
    CCTK_REAL16 *_outarray = (CCTK_REAL16 *) outarray;


    for (i = 0; i < num_elems; i++)
    {
      _outarray[i] = (CCTK_REAL16) inarray[i];
    }
  }
#endif
  else
  {
    CCTK_WARN (1, "copy_real_to_outtype: Unsupported output type");
    retval = -1;
  }

  return (retval);
}
