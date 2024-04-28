 /*@@
   @file      ReduceGA.c
   @date      Tue Aug 31 13:29:04 2004
   @author    Tom Goodale, Yaakoub Y El Khamra
   @desc
              New grid arrays implementation
   @enddesc
   @version   $Header$
 @@*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "util_Table.h"
#include "pugh_reductions.h"

#define ABS(x)      ((x) < 0 ? -(x) : (x))
#define SMALL_NUMBER 0

static const char *rcsid = "$Id:";

CCTK_FILEVERSION(CactusPUGH_PUGHReduce_ReduceGA_c)

/* local function prototypes */
static int ReduceGridArrays (const cGH *GH,
                          int dest_proc,
                          int local_reduce_handle,
                          int param_table_handle,
                          int N_input_arrays,
                          const CCTK_INT input_array_variable_indices[],
                          int M_output_values,
                          const CCTK_INT output_value_type_codes[],
                          void* const output_values[]);


 /*@@
   @routine    PUGH_ReduceGlobalArrays
   @date       
   @author     Gabrielle Allen, Yaakoub El Khamra
   @desc
               Generic routine for doing a reduction operation on a set of
               Cactus variables.
   @enddesc
   @var     GH
   @vdesc   pointer to the grid hierarchy
   @vtype   cGH *
   @vio     in
   @endvar
   @var     dest_proc
   @vdesc   the number of the processor to which we want to reduce (-1) for all-reduce
   @vtype   int
   @vio     in
   @endvar
   @var     local_reduce_handle
   @vdesc   the handle specifying the reduction operator
   @vtype   int
   @vio     in
   @endvar
   @var     param_reduce_handle
   @vdesc   the parameter table handle
   @vtype   int
   @vio     in
   @endvar
   @var     N_input_arrays
   @vdesc   number of elements in the reduction input
   @vtype   int
   @vio     in
   @endvar
   @var     input_array_variable_indices
   @vdesc   input arrays
   @vtype   const CCTK_INT
   @vio     in
   @endvar
   @var     M_output_values
   @vdesc   number of output values
   @vtype   int
   @vio     in
   @endvar
   @var     output_value_type_codes
   @vdesc   array containing output value type codes
   @vtype   const CCTK_IN
   @vio     in
   @endvar
   @var     output_values
   @vdesc   array of output values
   @vtype   void * const
   @vio     in
   @endvar
@@*/
int PUGH_ReduceGridArrays (const cGH *GH,
                           int dest_proc,
                           int local_reduce_handle,
                           int param_table_handle,
                           int N_input_arrays,
                           const CCTK_INT input_array_variable_indices[],
                           int M_output_values,
                           const CCTK_INT output_value_type_codes[],
                           void* const output_values[])
{
  int retval;
  retval = ReduceGridArrays(GH, dest_proc, local_reduce_handle,
                          param_table_handle, N_input_arrays,
                          input_array_variable_indices,
                          M_output_values, output_value_type_codes, 
                          output_values);
  return retval;
}


/*****************************************************************************/
/*                             local functions                               */
/*****************************************************************************/
/*@@
   @routine    ReduceGridArrays
   @date       
   @author     Tom Goodale, Yaakoub Y El Khamra
   @desc       Returns the maximum of a distributed array with
               'num_points' elements. Global reduction is done element-wise
               (num_outvals == 1) or on the results of the local reductions.
   @enddesc
@@*/
static int ReduceGridArrays (const cGH *GH,
                          int dest_proc,
                          int local_reduce_handle,
                          int param_table_handle,
                          int N_input_arrays,
                          const CCTK_INT input_array_variable_indices[],
                          int M_output_values,
                          const CCTK_INT output_value_type_codes[],
                          void* const output_values[])
{
  /* Utility variables */
  int i, ierr = 0;
  int dim = 0;
  int proc;
  int num_points=0;
  int total_num_points = 1;
  int perform_division = 1;
  int perform_2_root   = 1;
  int perform_3_root   = 1;
  int perform_4_root   = 1;      
  int perform_all_reduce = 1;
  pGA *GA;
  int created_local_par_table = 0;

  /* weight variables */
  int  weight_variable_index;
  int weight_on = 0;
  CCTK_REAL weight_sum = 0.0;
  CCTK_REAL total_weight_sum = 1.0;
  const void * weight;

  CCTK_INT * lower_array_bounds;
  CCTK_INT * min_array_subscript;
  CCTK_INT * array_lsh;
  CCTK_INT * input_array_dims;
  CCTK_POINTER * input_arrays;
  CCTK_INT * input_array_type_codes;
  CCTK_INT * input_array_gz;
  
#ifdef CCTK_MPI
  int nprocs = 0, global_operation = 0;
  const pGH *pughGH = NULL;
  void *local_outvals = NULL;
  nprocs = CCTK_nProcs(GH);
  if (dest_proc >=0 && dest_proc <=nprocs)
  {
    perform_all_reduce = 0;
  }
  else if (dest_proc == -1)
  {
    perform_all_reduce = 1;
  }
  else
  {
    CCTK_WARN (0, "PUGHReduce Destination processor for global reduction\n \
                   is out of bounds");
  }
#endif


  /* prevent compiler warnings about unused parameters */
  (void) (GH + 0);
  
  /* allocate memory for type codes and input array pointer */
  input_array_type_codes = (CCTK_INT *)malloc (N_input_arrays *sizeof(CCTK_INT));
  input_arrays           = (CCTK_POINTER) malloc (N_input_arrays *sizeof(CCTK_POINTER));

  /* set the number of dimensions */
  dim = CCTK_GroupDimFromVarI(input_array_variable_indices[0]);
 
  /* allocate memory for the array of the dimensions of the input arrays */
  lower_array_bounds  = (CCTK_INT *)malloc (dim *sizeof(CCTK_INT));
  min_array_subscript = (CCTK_INT *)malloc (dim *sizeof(CCTK_INT));
  array_lsh           = (CCTK_INT *)malloc (dim *sizeof(CCTK_INT));
  input_array_dims    = (CCTK_INT *)malloc (dim *sizeof(CCTK_INT));
  input_array_gz      = (CCTK_INT *)malloc (dim *sizeof(CCTK_INT));
  
  /* find out the types of the input arrays and put that */
  /* in an array ipughGH->GFExtras[dim]->lnsize[i]*/
  for ( i = 0; i < N_input_arrays; i++)
  {
    input_array_type_codes[i] = CCTK_VarTypeI( input_array_variable_indices[i] );
    input_arrays[i]           = CCTK_VarDataPtrI(GH, 0, input_array_variable_indices[i] );
  }

  /* fill out the lsh of the input arrays, the lower array bounds and ghostzone numbers*/
  ierr = CCTK_GrouplshVI(GH, dim, array_lsh, input_array_variable_indices[0] );
  ierr = CCTK_GrouplbndVI(GH, dim, lower_array_bounds, input_array_variable_indices[0] );
  i    = CCTK_GroupIndexFromVarI(input_array_variable_indices[0]);
  ierr = CCTK_GroupnghostzonesGI(GH, dim, input_array_gz,i );

  /* get the GA structure for the current timelevel */
  GA   = ((pGA ***) PUGH_pGH (GH)->variables)[input_array_variable_indices[0]][0];
      
  /* calculate index ranges */
  for ( i=0; i < dim; i++)
  {
    /* get the start- and endpoint to iterate over in this dimension */
    min_array_subscript[i] = GA->extras->ownership[0][i];
    array_lsh[i]           = GA->extras->ownership[1][i];
    input_array_dims[i]    = GA->extras->lnsize[i];
  }

  /* Create the parameter table if it is not there and add the bounds to it */
  if ( Util_TableQueryNKeys(param_table_handle) < 0)
  {
    created_local_par_table = 1;
    param_table_handle = Util_TableCreate (UTIL_TABLE_FLAGS_DEFAULT);
    ierr = Util_TableSetGenericArray (param_table_handle, CCTK_VARIABLE_INT, dim,min_array_subscript, "input_array_min_subscripts"); 
    ierr = Util_TableSetGenericArray (param_table_handle, CCTK_VARIABLE_INT, dim,array_lsh, "input_array_max_subscripts"); 
  }
  else
  {
    if (!Util_TableQueryValueInfo(param_table_handle, NULL, NULL, "input_array_min_subscripts"))
    {
      ierr = Util_TableSetGenericArray (param_table_handle, CCTK_VARIABLE_INT, dim,min_array_subscript, "input_array_min_subscripts"); 
    }
    if (!Util_TableQueryValueInfo(param_table_handle, NULL, NULL, "input_array_max_subscripts"))
    {
      ierr = Util_TableSetGenericArray (param_table_handle, CCTK_VARIABLE_INT, dim,array_lsh, "input_array_max_subscripts"); 
    }
  }

  /* Set flag to tell local reduction not to divide by num_points */
  ierr = Util_TableSetInt(param_table_handle, 1, "global_calling");

  /* Set the weight variable from the weight variable index */
  ierr   = Util_TableGetInt(param_table_handle, &weight_variable_index, "weight_variable_index");
  if (ierr == 1)
  {
    weight = CCTK_VarDataPtrI(GH, 0, weight_variable_index);
    ierr   = Util_TableSetPointerToConst(param_table_handle, weight, "weight");
  }
  else
  {
    weight = NULL;
    ierr = Util_TableSetInt(param_table_handle, 0, "weight_on");
  }

  ierr = CCTK_ReduceLocalArrays(dim, local_reduce_handle, param_table_handle,
                                N_input_arrays, input_array_dims, input_array_type_codes,
                                (void *)input_arrays, M_output_values, output_value_type_codes, output_values);

  /* Get flag to perform division by number of points or not, number_of_points and weight_sum */
  ierr = Util_TableGetInt(param_table_handle, &perform_division, "perform_division");
  ierr = Util_TableGetInt(param_table_handle, &num_points, "num_points");
  ierr = Util_TableGetReal(param_table_handle, &weight_sum, "weight_sum");
  ierr = Util_TableGetInt(param_table_handle, &weight_on, "weight_on");

  /* Get flags to perform root operations                   */
  ierr = Util_TableGetInt(param_table_handle, &perform_2_root, "perform_2_root");
  ierr = Util_TableGetInt(param_table_handle, &perform_3_root, "perform_3_root");
  ierr = Util_TableGetInt(param_table_handle, &perform_4_root, "perform_4_root");
  
#ifdef CCTK_MPI
  pughGH = PUGH_pGH (GH);
  ierr = Util_TableGetInt(param_table_handle, &proc, "proc");
  ierr = Util_TableGetInt(param_table_handle, &global_operation, "global_operation");

  if (perform_all_reduce)
  {
    CACTUS_MPI_ERROR (MPI_Allreduce (&num_points, &total_num_points, 1,
                    PUGH_MPI_INT, MPI_SUM, pughGH->PUGH_COMM_WORLD));
    CACTUS_MPI_ERROR (MPI_Allreduce (&weight_sum, &total_weight_sum, 1,
                    PUGH_MPI_REAL, MPI_SUM, pughGH->PUGH_COMM_WORLD));
  }
  else
  {
    CACTUS_MPI_ERROR (MPI_Reduce (&num_points, &total_num_points, 1,
                    PUGH_MPI_INT, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
    CACTUS_MPI_ERROR (MPI_Reduce (&weight_sum, &total_weight_sum, 1,
                    PUGH_MPI_REAL, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
  }
  for (i = 0; i< M_output_values; i++)
  {
    switch (output_value_type_codes[i])
    {
     /* out values type switches*/
      case CCTK_VARIABLE_BYTE:
        local_outvals = malloc (1 * sizeof (CCTK_BYTE));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_BYTE *) output_values[i], 1 * sizeof (CCTK_BYTE));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_BYTE *) output_values[i], 1,
                          PUGH_MPI_BYTE, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_BYTE *) output_values[i], 1,
                          PUGH_MPI_BYTE, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_BYTE *) output_values[i], 1,
                          PUGH_MPI_BYTE, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_BYTE *) output_values[i], 1,
                          PUGH_MPI_BYTE, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_BYTE *) output_values[i], 1,
                          PUGH_MPI_BYTE, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_BYTE *) output_values[i], 1,
                          PUGH_MPI_BYTE, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      case CCTK_VARIABLE_INT:
        local_outvals = malloc (1 * sizeof (CCTK_INT));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_INT *) output_values[i], 1 * sizeof (CCTK_INT));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT *) output_values[i], 1,
                          PUGH_MPI_INT, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT *) output_values[i], 1,
                          PUGH_MPI_INT, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT *) output_values[i], 1,
                          PUGH_MPI_INT, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT *) output_values[i], 1,
                          PUGH_MPI_INT, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT *) output_values[i], 1,
                          PUGH_MPI_INT, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT *) output_values[i], 1,
                          PUGH_MPI_INT, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #ifdef HAVE_CCTK_INT1
      case CCTK_VARIABLE_INT1:
        local_outvals = malloc (1 * sizeof (CCTK_INT1));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_INT1 *) output_values[i], 1 * sizeof (CCTK_INT1));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT1 *) output_values[i], 1,
                          PUGH_MPI_INT1, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT1 *) output_values[i], 1,
                          PUGH_MPI_INT1, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT1 *) output_values[i], 1,
                          PUGH_MPI_INT1, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT1 *) output_values[i], 1,
                          PUGH_MPI_INT1, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT1 *) output_values[i], 1,
                          PUGH_MPI_INT1, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT1 *) output_values[i], 1,
                          PUGH_MPI_INT1, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif                                                              
      #ifdef HAVE_CCTK_INT2
      case CCTK_VARIABLE_INT2:
        local_outvals = malloc (1 * sizeof (CCTK_INT2));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_INT2 *) output_values[i], 1 * sizeof (CCTK_INT2));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT2 *) output_values[i], 1,
                          PUGH_MPI_INT2, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT2 *) output_values[i], 1,
                          PUGH_MPI_INT2, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT2 *) output_values[i], 1,
                          PUGH_MPI_INT2, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT2 *) output_values[i], 1,
                          PUGH_MPI_INT2, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT2 *) output_values[i], 1,
                          PUGH_MPI_INT2, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT2 *) output_values[i], 1,
                          PUGH_MPI_INT2, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      #ifdef HAVE_CCTK_INT4
      case CCTK_VARIABLE_INT4:
        local_outvals = malloc (1 * sizeof (CCTK_INT4));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_INT4 *) output_values[i], 1 * sizeof (CCTK_INT4));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT4 *) output_values[i], 1,
                          PUGH_MPI_INT4, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT4 *) output_values[i], 1,
                          PUGH_MPI_INT4, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT4 *) output_values[i], 1,
                          PUGH_MPI_INT4, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT4 *) output_values[i], 1,
                          PUGH_MPI_INT4, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT4 *) output_values[i], 1,
                          PUGH_MPI_INT4, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT4 *) output_values[i], 1,
                          PUGH_MPI_INT4, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      #ifdef HAVE_CCTK_INT8
      case CCTK_VARIABLE_INT8:
        local_outvals = malloc (1 * sizeof (CCTK_INT8));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_INT8 *) output_values[i], 1 * sizeof (CCTK_INT8));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT8 *) output_values[i], 1,
                          PUGH_MPI_INT8, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT8 *) output_values[i], 1,
                          PUGH_MPI_INT8, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_INT8 *) output_values[i], 1,
                          PUGH_MPI_INT8, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT8 *) output_values[i], 1,
                          PUGH_MPI_INT8, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT8 *) output_values[i], 1,
                          PUGH_MPI_INT8, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_INT8 *) output_values[i], 1,
                          PUGH_MPI_INT8, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }

      break;                                                                
      #endif
      case CCTK_VARIABLE_REAL:
        local_outvals = malloc (1 * sizeof (CCTK_REAL));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_REAL *) output_values[i], 1 * sizeof (CCTK_REAL));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL *) output_values[i], 1,
                          PUGH_MPI_REAL, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL *) output_values[i], 1,
                          PUGH_MPI_REAL, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL *) output_values[i], 1,
                          PUGH_MPI_REAL, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL *) output_values[i], 1,
                          PUGH_MPI_REAL, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL *) output_values[i], 1,
                          PUGH_MPI_REAL, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL *) output_values[i], 1,
                          PUGH_MPI_REAL, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #ifdef HAVE_CCTK_REAL4
      case CCTK_VARIABLE_REAL4:
        local_outvals = malloc (1 * sizeof (CCTK_REAL4));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_REAL4 *) output_values[i], 1 * sizeof (CCTK_REAL4));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL4 *) output_values[i], 1,
                          PUGH_MPI_REAL4, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL4 *) output_values[i], 1,
                          PUGH_MPI_REAL4, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL4 *) output_values[i], 1,
                          PUGH_MPI_REAL4, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL4 *) output_values[i], 1,
                          PUGH_MPI_REAL4, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL4 *) output_values[i], 1,
                          PUGH_MPI_REAL4, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL4 *) output_values[i], 1,
                          PUGH_MPI_REAL4, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      #ifdef HAVE_CCTK_REAL8
      case CCTK_VARIABLE_REAL8:
        local_outvals = malloc (1 * sizeof (CCTK_REAL8));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_REAL8 *) output_values[i], 1 * sizeof (CCTK_REAL8));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL8 *) output_values[i], 1,
                          PUGH_MPI_REAL8, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL8 *) output_values[i], 1,
                          PUGH_MPI_REAL8, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL8 *) output_values[i], 1,
                          PUGH_MPI_REAL8, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL8 *) output_values[i], 1,
                          PUGH_MPI_REAL8, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL8 *) output_values[i], 1,
                          PUGH_MPI_REAL8, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL8 *) output_values[i], 1,
                          PUGH_MPI_REAL8, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      #ifdef HAVE_CCTK_REAL16
      case CCTK_VARIABLE_REAL16:
        local_outvals = malloc (1 * sizeof (CCTK_REAL16));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_REAL16 *) output_values[i], 1 * sizeof (CCTK_REAL16));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL16 *) output_values[i], 1,
                          PUGH_MPI_REAL16, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL16 *) output_values[i], 1,
                          PUGH_MPI_REAL16, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_REAL16 *) output_values[i], 1,
                          PUGH_MPI_REAL16, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL16 *) output_values[i], 1,
                          PUGH_MPI_REAL16, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL16 *) output_values[i], 1,
                          PUGH_MPI_REAL16, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_REAL16 *) output_values[i], 1,
                          PUGH_MPI_REAL16, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      case CCTK_VARIABLE_COMPLEX:
        local_outvals = malloc (1 * sizeof (CCTK_COMPLEX));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_COMPLEX *) output_values[i], 1 * sizeof (CCTK_COMPLEX));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #ifdef HAVE_CCTK_COMPLEX8
      case CCTK_VARIABLE_COMPLEX8:
        local_outvals = malloc (1 * sizeof (CCTK_COMPLEX8));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1 * sizeof (CCTK_COMPLEX8));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex8, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex8, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex8, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex8, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex8, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX8 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex8, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      #ifdef HAVE_CCTK_COMPLEX16
      case CCTK_VARIABLE_COMPLEX16:
        local_outvals = malloc (1 * sizeof (CCTK_COMPLEX16));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1 * sizeof (CCTK_COMPLEX16));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex16, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex16, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex16, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex16, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex16, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX16 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex16, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
      #ifdef HAVE_CCTK_COMPLEX32
      case CCTK_VARIABLE_COMPLEX32:
        local_outvals = malloc (1 * sizeof (CCTK_COMPLEX32));

        /* outvals[] contains now the local reduction values */
        memcpy ( local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1 * sizeof (CCTK_COMPLEX32));

        if (perform_all_reduce)
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex32, MPI_MAX, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex32, MPI_MIN, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Allreduce (local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex32, MPI_SUM, pughGH->PUGH_COMM_WORLD));
        }
        else
        {
          if (global_operation == 1)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex32, MPI_MAX, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 2)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex32, MPI_MIN, dest_proc, pughGH->PUGH_COMM_WORLD));         
          else if (global_operation == 3)
            CACTUS_MPI_ERROR (MPI_Reduce (local_outvals, (CCTK_COMPLEX32 *) output_values[i], 1,
                          pughGH->PUGH_mpi_complex32, MPI_SUM, dest_proc, pughGH->PUGH_COMM_WORLD));
        }
      break;
      #endif
    }
    free (local_outvals);
  }
  num_points = total_num_points;
  weight_sum = total_weight_sum;
#endif
  free (lower_array_bounds);
  free (min_array_subscript);
  free (array_lsh);
  free (input_array_dims);
  free (input_array_gz);
    
  if (perform_division == 0)
  {
    if (weight_on == 1)
    {
      if (ABS(weight_sum) > SMALL_NUMBER)
      {
        for (i = 0; i< M_output_values; i++)
        {
          switch (output_value_type_codes[i])
          {
           /* out values type switches*/
            case CCTK_VARIABLE_BYTE:
              *( (CCTK_BYTE *) output_values[i]) = *( (CCTK_BYTE *) output_values[i]) / weight_sum; 
            break;
            case CCTK_VARIABLE_INT:
              *( (CCTK_INT *) output_values[i]) = *( (CCTK_INT *) output_values[i]) / weight_sum; 
            break;
            #ifdef HAVE_CCTK_INT1
            case CCTK_VARIABLE_INT1:
              *( (CCTK_INT1 *) output_values[i]) = *( (CCTK_INT1 *) output_values[i]) / weight_sum; 
            break;
            #endif                                                              
            #ifdef HAVE_CCTK_INT2
            case CCTK_VARIABLE_INT2:
              *( (CCTK_INT2 *) output_values[i]) = *( (CCTK_INT2 *) output_values[i]) / weight_sum; 
            break;
            #endif
            #ifdef HAVE_CCTK_INT4
            case CCTK_VARIABLE_INT4:
              *( (CCTK_INT4 *) output_values[i]) = *( (CCTK_INT4 *) output_values[i]) / weight_sum; 
            break;
            #endif
            #ifdef HAVE_CCTK_INT8
            case CCTK_VARIABLE_INT8:
              *( (CCTK_INT8 *) output_values[i]) = *( (CCTK_INT8 *) output_values[i]) / weight_sum; 
            break;                                                                
            #endif
            case CCTK_VARIABLE_REAL:
              *( (CCTK_REAL *) output_values[i]) = *( (CCTK_REAL *) output_values[i]) / weight_sum; 
            break;
            #ifdef HAVE_CCTK_REAL4
            case CCTK_VARIABLE_REAL4:
              *( (CCTK_REAL4 *) output_values[i]) = *( (CCTK_REAL4 *) output_values[i]) / weight_sum; 
            break;
            #endif
            #ifdef HAVE_CCTK_REAL8
            case CCTK_VARIABLE_REAL8:
              *( (CCTK_REAL8 *) output_values[i]) = *( (CCTK_REAL8 *) output_values[i]) / weight_sum; 
            break;
            #endif
            #ifdef HAVE_CCTK_REAL16
            case CCTK_VARIABLE_REAL16:
              *( (CCTK_REAL16 *) output_values[i]) = *( (CCTK_REAL16 *) output_values[i]) / weight_sum; 
            break;
            #endif
            case CCTK_VARIABLE_COMPLEX:
              *( (CCTK_COMPLEX *) output_values[i]) = *( (CCTK_COMPLEX *) output_values[i]) / weight_sum; 
            break;
            #ifdef HAVE_CCTK_COMPLEX8
            case CCTK_VARIABLE_COMPLEX8:
              *( (CCTK_COMPLEX8 *) output_values[i]) = *( (CCTK_COMPLEX8 *) output_values[i]) / weight_sum; 
            break;
            #endif
            #ifdef HAVE_CCTK_COMPLEX16
            case CCTK_VARIABLE_COMPLEX16:
            *( (CCTK_COMPLEX16 *) output_values[i]) = *( (CCTK_COMPLEX16 *) output_values[i]) / weight_sum; 
            break;
            #endif
            #ifdef HAVE_CCTK_COMPLEX32
            case CCTK_VARIABLE_COMPLEX32:
              *( (CCTK_COMPLEX32 *) output_values[i]) = *( (CCTK_COMPLEX32 *) output_values[i]) / weight_sum; 
            break;
            #endif
          }
        }
      }
      else
      {
        CCTK_WARN (1, "The sum of weights in average reduction is zero");
        return (-1);
      }
    }
    else
    {
      for (i = 0; i< M_output_values; i++)
      {
        switch (output_value_type_codes[i])
        {
         /* out values type switches*/
          case CCTK_VARIABLE_BYTE:
            *( (CCTK_BYTE *) output_values[i]) = *( (CCTK_BYTE *) output_values[i]) / num_points; 
          break;
          case CCTK_VARIABLE_INT:
            *( (CCTK_INT *) output_values[i]) = *( (CCTK_INT *) output_values[i]) / num_points; 
          break;
          #ifdef HAVE_CCTK_INT1
          case CCTK_VARIABLE_INT1:
            *( (CCTK_INT1 *) output_values[i]) = *( (CCTK_INT1 *) output_values[i]) / num_points; 
          break;
          #endif                                                              
          #ifdef HAVE_CCTK_INT2
          case CCTK_VARIABLE_INT2:
            *( (CCTK_INT2 *) output_values[i]) = *( (CCTK_INT2 *) output_values[i]) / num_points; 
          break;
          #endif
          #ifdef HAVE_CCTK_INT4
          case CCTK_VARIABLE_INT4:
            *( (CCTK_INT4 *) output_values[i]) = *( (CCTK_INT4 *) output_values[i]) / num_points; 
          break;
          #endif
          #ifdef HAVE_CCTK_INT8
          case CCTK_VARIABLE_INT8:
            *( (CCTK_INT8 *) output_values[i]) = *( (CCTK_INT8 *) output_values[i]) / num_points; 
          break;                                                                
          #endif
          case CCTK_VARIABLE_REAL:
            *( (CCTK_REAL *) output_values[i]) = *( (CCTK_REAL *) output_values[i]) / num_points; 
          break;
          #ifdef HAVE_CCTK_REAL4
          case CCTK_VARIABLE_REAL4:
            *( (CCTK_REAL4 *) output_values[i]) = *( (CCTK_REAL4 *) output_values[i]) / num_points; 
          break;
          #endif
          #ifdef HAVE_CCTK_REAL8
          case CCTK_VARIABLE_REAL8:
            *( (CCTK_REAL8 *) output_values[i]) = *( (CCTK_REAL8 *) output_values[i]) / num_points; 
          break;
          #endif
          #ifdef HAVE_CCTK_REAL16
          case CCTK_VARIABLE_REAL16:
            *( (CCTK_REAL16 *) output_values[i]) = *( (CCTK_REAL16 *) output_values[i]) / num_points; 
          break;
          #endif
          case CCTK_VARIABLE_COMPLEX:
            *( (CCTK_COMPLEX *) output_values[i]) = *( (CCTK_COMPLEX *) output_values[i]) / num_points; 
          break;
          #ifdef HAVE_CCTK_COMPLEX8
          case CCTK_VARIABLE_COMPLEX8:
            *( (CCTK_COMPLEX8 *) output_values[i]) = *( (CCTK_COMPLEX8 *) output_values[i]) / num_points; 
          break;
          #endif
          #ifdef HAVE_CCTK_COMPLEX16
          case CCTK_VARIABLE_COMPLEX16:
          *( (CCTK_COMPLEX16 *) output_values[i]) = *( (CCTK_COMPLEX16 *) output_values[i]) / num_points; 
          break;
          #endif
          #ifdef HAVE_CCTK_COMPLEX32
          case CCTK_VARIABLE_COMPLEX32:
            *( (CCTK_COMPLEX32 *) output_values[i]) = *( (CCTK_COMPLEX32 *) output_values[i]) / num_points; 
          break;
          #endif
        }
      }
    }
  }

  if (perform_2_root == 0)
  {
    for (i = 0; i< M_output_values; i++)
    {
      switch (output_value_type_codes[i])
      {
       /* out values type switches*/
        case CCTK_VARIABLE_BYTE:
          *( (CCTK_BYTE *) output_values[i]) = sqrt(*( (CCTK_BYTE *) output_values[i])); 
        break;
        case CCTK_VARIABLE_INT:
          *( (CCTK_INT *) output_values[i]) = sqrt( *( (CCTK_INT *) output_values[i])); 
        break;
        #ifdef HAVE_CCTK_INT1
        case CCTK_VARIABLE_INT1:
          *( (CCTK_INT1 *) output_values[i]) = sqrt(*( (CCTK_INT1 *) output_values[i])); 
        break;
        #endif                                                              
        #ifdef HAVE_CCTK_INT2
        case CCTK_VARIABLE_INT2:
          *( (CCTK_INT2 *) output_values[i]) = sqrt(*( (CCTK_INT2 *) output_values[i])); 
        break;
        #endif
        #ifdef HAVE_CCTK_INT4
        case CCTK_VARIABLE_INT4:
          *( (CCTK_INT4 *) output_values[i]) = sqrt( *( (CCTK_INT4 *) output_values[i])); 
        break;
        #endif
        #ifdef HAVE_CCTK_INT8
        case CCTK_VARIABLE_INT8:
          *( (CCTK_INT8 *) output_values[i]) = sqrt(*( (CCTK_INT8 *) output_values[i])); 
        break;                                                                
        #endif
        case CCTK_VARIABLE_REAL:
          *( (CCTK_REAL *) output_values[i]) = sqrt(*( (CCTK_REAL *) output_values[i])); 
        break;
        #ifdef HAVE_CCTK_REAL4
        case CCTK_VARIABLE_REAL4:
          *( (CCTK_REAL4 *) output_values[i]) = sqrt(*( (CCTK_REAL4 *) output_values[i])); 
        break;
        #endif
        #ifdef HAVE_CCTK_REAL8
        case CCTK_VARIABLE_REAL8:
          *( (CCTK_REAL8 *) output_values[i]) = sqrt(*( (CCTK_REAL8 *) output_values[i])); 
        break;
        #endif
        #ifdef HAVE_CCTK_REAL16
        case CCTK_VARIABLE_REAL16:
          *( (CCTK_REAL16 *) output_values[i]) = sqrt(*( (CCTK_REAL16 *) output_values[i])); 
        break;
        #endif
        case CCTK_VARIABLE_COMPLEX:
          *( (CCTK_COMPLEX *) output_values[i]) = CCTK_CmplxSqrt(*( (CCTK_COMPLEX *) output_values[i]));
        break;
        #ifdef HAVE_CCTK_COMPLEX8
        case CCTK_VARIABLE_COMPLEX8:
          *( (CCTK_COMPLEX8 *) output_values[i]) = CCTK_Cmplx8Sqrt(*( (CCTK_COMPLEX8 *) output_values[i]));
        break;
        #endif
        #ifdef HAVE_CCTK_COMPLEX16
        case CCTK_VARIABLE_COMPLEX16:
          *( (CCTK_COMPLEX16 *) output_values[i]) = CCTK_Cmplx16Sqrt(*( (CCTK_COMPLEX16 *) output_values[i]));
        break;
        #endif
        #ifdef HAVE_CCTK_COMPLEX32
        case CCTK_VARIABLE_COMPLEX32:
          *( (CCTK_COMPLEX32 *) output_values[i]) = CCTK_Cmplx32Sqrt(*( (CCTK_COMPLEX32 *) output_values[i]));
        break;
        #endif
      }
    }
  }

  if (perform_3_root == 0)
  {
    for (i = 0; i< M_output_values; i++)
    {
      switch (output_value_type_codes[i])
      {
       /* out values type switches*/
        case CCTK_VARIABLE_BYTE:
          *( (CCTK_BYTE *) output_values[i]) = pow(*( (CCTK_BYTE *) output_values[i]), 1.0/3.0); 
        break;
        case CCTK_VARIABLE_INT:
          *( (CCTK_INT *) output_values[i]) = pow(*( (CCTK_INT *) output_values[i]), 1.0/3.0); 
        break;
        #ifdef HAVE_CCTK_INT1
        case CCTK_VARIABLE_INT1:
          *( (CCTK_INT1 *) output_values[i]) = pow(*( (CCTK_INT1 *) output_values[i]), 1.0/3.0); 
        break;
        #endif                                                              
        #ifdef HAVE_CCTK_INT2
        case CCTK_VARIABLE_INT2:
          *( (CCTK_INT2 *) output_values[i]) = pow(*( (CCTK_INT2 *) output_values[i]), 1.0/3.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_INT4
        case CCTK_VARIABLE_INT4:
          *( (CCTK_INT4 *) output_values[i]) = pow( *( (CCTK_INT4 *) output_values[i]), 1.0/3.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_INT8
        case CCTK_VARIABLE_INT8:
          *( (CCTK_INT8 *) output_values[i]) = pow(*( (CCTK_INT8 *) output_values[i]), 1.0/3.0); 
        break;                                                                
        #endif
        case CCTK_VARIABLE_REAL:
          *( (CCTK_REAL *) output_values[i]) = pow(*( (CCTK_REAL *) output_values[i]), 1.0/3.0); 
        break;
        #ifdef HAVE_CCTK_REAL4
        case CCTK_VARIABLE_REAL4:
          *( (CCTK_REAL4 *) output_values[i]) = pow(*( (CCTK_REAL4 *) output_values[i]), 1.0/3.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_REAL8
        case CCTK_VARIABLE_REAL8:
          *( (CCTK_REAL8 *) output_values[i]) = pow(*( (CCTK_REAL8 *) output_values[i]), 1.0/3.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_REAL16
        case CCTK_VARIABLE_REAL16:
          *( (CCTK_REAL16 *) output_values[i]) = pow(*( (CCTK_REAL16 *) output_values[i]), 1.0/3.0); 
        break;
        #endif
        case CCTK_VARIABLE_COMPLEX:
          *( (CCTK_COMPLEX *) output_values[i]) = CCTK_CmplxPow(*( (CCTK_COMPLEX *) output_values[i]), 1.0/3.0);
        break;
        #ifdef HAVE_CCTK_COMPLEX8
        case CCTK_VARIABLE_COMPLEX8:
          *( (CCTK_COMPLEX8 *) output_values[i]) = CCTK_Cmplx8Pow(*( (CCTK_COMPLEX8 *) output_values[i]), 1.0/3.0);
        break;
        #endif
        #ifdef HAVE_CCTK_COMPLEX16
        case CCTK_VARIABLE_COMPLEX16:
          *( (CCTK_COMPLEX16 *) output_values[i]) = CCTK_Cmplx16Pow(*( (CCTK_COMPLEX16 *) output_values[i]), 1.0/3.0);
        break;
        #endif
        #ifdef HAVE_CCTK_COMPLEX32
        case CCTK_VARIABLE_COMPLEX32:
          *( (CCTK_COMPLEX32 *) output_values[i]) = CCTK_Cmplx32Pow(*( (CCTK_COMPLEX32 *) output_values[i]), 1.0/3.0);
        break;
        #endif
      }
    }
  }

  if (perform_4_root == 0)
  {
    for (i = 0; i< M_output_values; i++)
    {
      switch (output_value_type_codes[i])
      {
       /* out values type switches*/
        case CCTK_VARIABLE_BYTE:
          *( (CCTK_BYTE *) output_values[i]) = pow(*( (CCTK_BYTE *) output_values[i]), 1.0/4.0); 
        break;
        case CCTK_VARIABLE_INT:
          *( (CCTK_INT *) output_values[i]) = pow(*( (CCTK_INT *) output_values[i]), 1.0/4.0); 
        break;
        #ifdef HAVE_CCTK_INT1
        case CCTK_VARIABLE_INT1:
          *( (CCTK_INT1 *) output_values[i]) = pow(*( (CCTK_INT1 *) output_values[i]), 1.0/4.0); 
        break;
        #endif                                                              
        #ifdef HAVE_CCTK_INT2
        case CCTK_VARIABLE_INT2:
          *( (CCTK_INT2 *) output_values[i]) = pow(*( (CCTK_INT2 *) output_values[i]), 1.0/4.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_INT4
        case CCTK_VARIABLE_INT4:
          *( (CCTK_INT4 *) output_values[i]) = pow( *( (CCTK_INT4 *) output_values[i]), 1.0/4.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_INT8
        case CCTK_VARIABLE_INT8:
          *( (CCTK_INT8 *) output_values[i]) = pow(*( (CCTK_INT8 *) output_values[i]), 1.0/4.0); 
        break;                                                                
        #endif
        case CCTK_VARIABLE_REAL:
          *( (CCTK_REAL *) output_values[i]) = pow(*( (CCTK_REAL *) output_values[i]), 1.0/4.0); 
        break;
        #ifdef HAVE_CCTK_REAL4
        case CCTK_VARIABLE_REAL4:
          *( (CCTK_REAL4 *) output_values[i]) = pow(*( (CCTK_REAL4 *) output_values[i]), 1.0/4.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_REAL8
        case CCTK_VARIABLE_REAL8:
          *( (CCTK_REAL8 *) output_values[i]) = pow(*( (CCTK_REAL8 *) output_values[i]), 1.0/4.0); 
        break;
        #endif
        #ifdef HAVE_CCTK_REAL16
        case CCTK_VARIABLE_REAL16:
          *( (CCTK_REAL16 *) output_values[i]) = pow(*( (CCTK_REAL16 *) output_values[i]), 1.0/4.0); 
        break;
        #endif
        case CCTK_VARIABLE_COMPLEX:
          *( (CCTK_COMPLEX *) output_values[i]) = CCTK_CmplxPow(*( (CCTK_COMPLEX *) output_values[i]), 1.0/4.0);
        break;
        #ifdef HAVE_CCTK_COMPLEX8
        case CCTK_VARIABLE_COMPLEX8:
          *( (CCTK_COMPLEX8 *) output_values[i]) = CCTK_Cmplx8Pow(*( (CCTK_COMPLEX8 *) output_values[i]), 1.0/4.0);
        break;
        #endif
        #ifdef HAVE_CCTK_COMPLEX16
        case CCTK_VARIABLE_COMPLEX16:
          *( (CCTK_COMPLEX16 *) output_values[i]) = CCTK_Cmplx16Pow(*( (CCTK_COMPLEX16 *) output_values[i]), 1.0/4.0);
        break;
        #endif
        #ifdef HAVE_CCTK_COMPLEX32
        case CCTK_VARIABLE_COMPLEX32:
          *( (CCTK_COMPLEX32 *) output_values[i]) = CCTK_Cmplx32Pow(*( (CCTK_COMPLEX32 *) output_values[i]), 1.0/4.0);
        break;
        #endif
      }
    }
  }

  if(created_local_par_table)
  {
    ierr = Util_TableDestroy(param_table_handle);
  }
  
  return ierr;
}
