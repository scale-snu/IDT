 /*@@
   @file      Reduction.c
   @date      Thu Apr  3 11:54:53 1997
   @author    Thomas Radke, Paul Walker, Yaakoub Y El Khamra
   @desc
              Local Reduction operator
   @enddesc
   @version   $Id: Reduction.c 55 2005-01-26 17:09:38Z yye00 $
 @@*/

#include <stdlib.h>

#include "CactusNumerical/LocalReduce/src/local_reductions.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_LocalReduce_Reduction_c)

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

 /*@@
   @routine    LocalReduce_Reduce
   @author     Thomas Radke, Yaakoub El Khamra
   @date       
   @desc
               Wrapper to reduce a list of arrays.
               Just calls the appropriate reduction operator and does
               the type conversion of the results.
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
int LocalReduce_Reduce (int N_dims, int operator_handle, 
                        int param_table_handle,   int N_input_arrays,
                        const CCTK_INT input_array_dims[], 
                        const CCTK_INT input_array_type_codes[],
                        const void *const input_arrays[],
                        int M_output_numbers,
                        const CCTK_INT output_number_type_codes[],
                        void * const output_numbers[],
                        reduction_fn_t reduction_fn)
{
  int retval;
  /* do the reduction on the input arrays */
  retval = reduction_fn (N_dims, operator_handle, 
                        param_table_handle, N_input_arrays,
                        input_array_dims, input_array_type_codes,
                        input_arrays, M_output_numbers,
                        output_number_type_codes, output_numbers);

  return (retval);
}
