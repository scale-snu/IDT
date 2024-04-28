 /*@@
   @file      ReductionNormInf.c
   @date      
   @author    Tom Goodale, Yaakoub Y El Khamra
   @desc
              Defines the reduction operator to get the infinite norm
              of an arbitrary array.
   @enddesc
   @version   $Id: ReductionNormInf.c 103 2013-03-03 22:25:23Z eschnett $
 @@*/

#include <stdlib.h>
#include <string.h>

#include "CactusNumerical/LocalReduce/src/local_reductions.h"
#include "CactusNumerical/LocalReduce/src/NormInf_Functions.h"

static const char *rcsid = "$Id: ReductionNormInf.c 103 2013-03-03 22:25:23Z eschnett $";

CCTK_FILEVERSION(CactusBase_LocalReduce_ReductionNormInf_c)


/* Define the reduction operations */



/* local function prototypes */
static int ReductionLInf (int N_dims, int operator_handle, 
                        int param_table_handle,   int N_input_arrays,
                        const CCTK_INT input_array_dims[], 
                        const CCTK_INT input_array_type_codes[],
                        const void *const input_arrays[],
                        int M_output_numbers,
                        const CCTK_INT output_number_type_codes[],
                        void * const output_numbers[]);

/*@@
  @routine LocalReduce_LInf
  @author  Tom Goodale, Yaakoub Y El Khamra
  @date    
  @desc
  @enddesc
  @history
  @endhistory
  @var     N_dims
  @vdesc   number of dimensions in the *reduction*
  @vtype   int
  @vio     in
  @endvar
  @var     operator_handle
  @vdesc   operator handle specificies the type of reduction we will perform
  @vtype   int
  @vio     in
  @endvar
  @var     param_table_handle
  @vdesc   handle to "parameter table", a key-value table
  @vtype   int
  @vio     in
  @endvar
  @var     N_input_arrays
  @vdesc   number of input arrays
  @vtype   int 
  @vio     in
  @endvar
  @var     input_array_dims
  @vdesc   array of input array dimensions (common to all input arrays)
  @vtype   const CCTK_INT
  @vio     in
  @endvar
  @var     input_array_type_codes
  @vdesc   array of CCTK_VARIABLE_* codes giving data types of input arrays
  @vtype   const CCTK_INT
  @vio     in
  @endvar
  @var     input_arrays
  @vdesc   array of pointers to input arrays
  @vtype   const void *const
  @vio     in
  @endvar
  @var     M_output_numbers
  @vdesc   
  @vtype   int
  @vio     in
  @endvar
  @var     output_number_type_codes
  @vdesc   array of CCTK_VARIABLE_* codes giving data types of output numbers
  @vtype   const CCTK_INT
  @vio     in
  @endvar
  @var     output_numbers
  @vdesc   array[M_output_numbers] of pointers to output numbers[M_reduce_numbers]
  @vtype   void *const
  @vio     in
  @endvar
@@*/
int LocalReduce_LInf (int N_dims, int operator_handle, 
                      int param_table_handle,   int N_input_arrays,
                      const CCTK_INT input_array_dims[], 
                      const CCTK_INT input_array_type_codes[],
                      const void *const input_arrays[],
                      int M_output_numbers,
                      const CCTK_INT output_number_type_codes[],
                      void * const output_numbers[])
{
  return (LocalReduce_Reduce (N_dims, operator_handle, 
                          param_table_handle, N_input_arrays,
                          input_array_dims, input_array_type_codes,
                          input_arrays, M_output_numbers,
                          output_number_type_codes, output_numbers,
                          ReductionLInf));
}



/*****************************************************************************/
/*                             local functions                               */
/*****************************************************************************/
/*@@
   @routine    ReductionLInf
   @date       
   @author     Tom Goodale, Yaakoub Y El Khamra
   @desc       Returns the infinite norm of a distributed array with
               'num_points' elements. Global reduction is done element-wise
               (num_outvals == 1) or on the results of the local reductions.
   @enddesc
@@*/
static int ReductionLInf (int N_dims, int operator_handle, 
                        int param_table_handle,   int N_input_arrays,
                        const CCTK_INT input_array_dims[], 
                        const CCTK_INT input_array_type_codes[],
                        const void *const input_arrays[],
                        int M_output_numbers,
                        const CCTK_INT output_number_type_codes[],
                        void * const output_numbers[])
{
  /* utility variables */
  int i, j, num_points;
  int * iters_per_dim;
  int global_calling = 0;

  /* indices to hold the temp indices of size N_dims  and iteration indices*/
  int * indices;
  int * actual_indices;
  int * actual_iters_per_dim;
  int max_iter = 0;
  int max_index = 1;

  /* data pointer offset and strides declared here */
  CCTK_INT * input_array_offsets;
  CCTK_INT * input_array_strides;
  CCTK_INT * input_array_min_subscripts;
  CCTK_INT * input_array_max_subscripts;

  /* weight variables declared here */
  int weight_on = 0;          /* weight is by default off=0 */
  void const * weight;        /* pointer to the weight variable */  
  CCTK_REAL weight_sum;

  /* prevent warnings for unused vars */
  (void)(operator_handle + 0);

  /* set the number of points */
  num_points = 0;

  /* allocate memory for iters_per_dim */
  iters_per_dim = (int *)malloc(N_dims * sizeof(int));

  /* allocate then initialize the values of the strides and subscripts */
  indices = (int *)malloc (N_dims * sizeof(int));
  actual_indices = (int *)malloc (N_dims * sizeof(int));
  actual_iters_per_dim = (int *)malloc (N_dims * sizeof(int));

  /* allocate then initialize the values of the strides and subscripts */
  input_array_offsets = (CCTK_INT *)malloc (N_input_arrays * sizeof(CCTK_INT));
  input_array_strides = (CCTK_INT *) malloc (N_dims * sizeof(CCTK_INT));
  input_array_min_subscripts = (CCTK_INT *) malloc (N_dims * sizeof(CCTK_INT));
  input_array_max_subscripts = (CCTK_INT *) malloc (N_dims * sizeof(CCTK_INT));

  for (i = 0; i<N_input_arrays; i++)
  {
    input_array_offsets[i] = 0;
  }

  for (i = 0; i<N_dims; i++)
  {
    input_array_strides[i]        = 1;
    input_array_min_subscripts[i] = 0;
    input_array_max_subscripts[i] = input_array_dims[i];
    max_index *= input_array_max_subscripts[i];
  }

  /* for strides and subscripts get values from param table (it they exist) */
  if ( Util_TableQueryNKeys(param_table_handle) != 0)
  {
    Util_TableGetGenericArray(param_table_handle, CCTK_VARIABLE_INT, 
             N_input_arrays, input_array_offsets, "input_array_offsets");  
    Util_TableGetGenericArray(param_table_handle, CCTK_VARIABLE_INT, 
             N_dims, input_array_strides, "input_array_strides");  
    Util_TableGetGenericArray(param_table_handle, CCTK_VARIABLE_INT, 
             N_dims, input_array_min_subscripts, "input_array_min_subscripts");  
    Util_TableGetGenericArray(param_table_handle, CCTK_VARIABLE_INT, 
             N_dims, input_array_max_subscripts, "input_array_max_subscripts");  
  }

  /* for weights get values from param table (it they exist) */
  if ( Util_TableQueryNKeys(param_table_handle) != 0)
  {
    /* Get the weight_on parameter and the weight_var_index parameter */
    Util_TableGetInt(param_table_handle, &weight_on, "weight_on");
    Util_TableGetPointerToConst(param_table_handle, &weight, "weight");
    /* Need to add more checking here for size, storage */
  }

  /* reduction maps an array to a single value of the same type */
  if (M_output_numbers != N_input_arrays)
  {
    CCTK_WARN (1, "Infinite norm reduction returns a single value\n                 \
                   for each input array\n");
    return (-1);
  }

  /* set the indices to their minimum values */
  max_iter = 1;
  for (j = 0; j <N_dims; j++)
  {
    indices       [j]        = 0;
    actual_indices[j]        = input_array_min_subscripts[j];
    actual_iters_per_dim [j] = (int) (input_array_max_subscripts[j] - input_array_min_subscripts[j]);
    iters_per_dim [j]        = (int) ((input_array_max_subscripts[j] - input_array_min_subscripts[j])/input_array_strides[j]);
    max_iter *= iters_per_dim [j];
  }

#undef REDUCTION_OPERATION
#undef REDUCTION_INITIAL
#undef EXTRA_STEP

#ifdef ABS
#undef ABS
#endif
#define ABS(x)      ((x) < 0 ? -(x) : (x))
#ifdef MAX
#undef MAX
#endif
#define MAX(x,y)    ((x) >= (y) ? (x) : (y))

#define REDUCTION_OPERATION(norm_inf, scalar)   norm_inf = MAX(norm_inf, ABS(scalar));
#define REDUCTION_INITIAL(num) num = 0;
#define EXTRA_STEP(a, b) 

  for (i = 0; i < N_input_arrays; i++)
  { 
    /* Do the type matching */
    switch (input_array_type_codes[i])
    {
    /* in values type switches*/
      case CCTK_VARIABLE_BYTE:
        LocalReduce_NormInf_BYTE(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      case CCTK_VARIABLE_INT:
        LocalReduce_NormInf_INT(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #ifdef HAVE_CCTK_INT1
      case CCTK_VARIABLE_INT1:
        LocalReduce_NormInf_INT1(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #endif                                                              
      #ifdef HAVE_CCTK_INT2
      case CCTK_VARIABLE_INT2:
        LocalReduce_NormInf_INT2(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #endif
      #ifdef HAVE_CCTK_INT4
      case CCTK_VARIABLE_INT4:
        LocalReduce_NormInf_INT4(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #endif
      #ifdef HAVE_CCTK_INT8
      case CCTK_VARIABLE_INT8:
        LocalReduce_NormInf_INT8(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;                                                                
      #endif
      case CCTK_VARIABLE_REAL:
        LocalReduce_NormInf_REAL(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #ifdef HAVE_CCTK_REAL4
      case CCTK_VARIABLE_REAL4:
        LocalReduce_NormInf_REAL4(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #endif
      #ifdef HAVE_CCTK_REAL8
      case CCTK_VARIABLE_REAL8:
        LocalReduce_NormInf_REAL8(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #endif
      #ifdef HAVE_CCTK_REAL16
      case CCTK_VARIABLE_REAL16:
        LocalReduce_NormInf_REAL16(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle);
      break;
      #endif  
      /* case CCTK_VARIABLE_COMPLEX: */
      /*   LocalReduce_NormInf_COMPLEX(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle); */
      /* break; */
      /* #ifdef HAVE_CCTK_COMPLEX8 */
      /* case CCTK_VARIABLE_COMPLEX8: */
      /*   LocalReduce_NormInf_COMPLEX8(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle); */
      /* break; */
      /* #endif */
      /* #ifdef HAVE_CCTK_COMPLEX16 */
      /* case CCTK_VARIABLE_COMPLEX16: */
      /*   LocalReduce_NormInf_COMPLEX16(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle); */
      /* break; */
      /* #endif */
      /* #ifdef HAVE_CCTK_COMPLEX32 */
      /* case CCTK_VARIABLE_COMPLEX32: */
      /*   LocalReduce_NormInf_COMPLEX32(i, weight_on, weight, input_array_offsets, indices, max_iter, actual_indices, input_array_strides, input_array_min_subscripts, input_array_dims, num_points, actual_iters_per_dim, iters_per_dim,   N_dims, input_arrays, output_number_type_codes, output_numbers, param_table_handle); */
      /* break; */
      /* #endif */
    }
  }

  /* Get the values of num_points and weight_sum */
  Util_TableGetInt(param_table_handle, &num_points, "num_points");
  Util_TableGetReal(param_table_handle, &weight_sum, "weight_sum");
  num_points--;

  /* store the number of points in the paramater table  and perform division */
  Util_TableGetInt(param_table_handle, &global_calling, "global_calling");
  if ( global_calling != 0)
  {
    Util_TableSetInt(param_table_handle, num_points, "num_points");
    Util_TableSetInt(param_table_handle, 1,"global_operation");
  }
  
  /* free memory */
  free (iters_per_dim);
  free (indices);
  free (actual_indices);
  free (actual_iters_per_dim);
  free (input_array_offsets);
  free (input_array_strides);
  free (input_array_min_subscripts);
  free (input_array_max_subscripts);
                                                                                
  return (0);
}
