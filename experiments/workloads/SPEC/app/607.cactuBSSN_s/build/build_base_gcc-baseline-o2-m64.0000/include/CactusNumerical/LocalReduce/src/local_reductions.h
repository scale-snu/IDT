 /*@@
   @header    local_reductions.h
   @date      
   @author    Tom Goodale, Yaakoub Y El Khamra
   @desc
              Prototypes for local reduction operators
   @enddesc
   @version   $Header$
 @@*/

#ifndef _LOCAL_REDUCTIONS_H_
#define _LOCAL_REDUCTIONS_H_


#include "LocalReduce/cctk.h"
#include "util_Table.h"
#include "cctk_Reduction.h"

#define ABS(x)      ((x) < 0 ? -(x) : (x))
#define SMALL_NUMBER 0

#ifdef __cplusplus
extern "C" {
#endif


#define ITERATE_ON_ARRAY(i,cctk_type, in_data, out_type, out_num, weight_on, weight, input_array_offset, indices, sum_indices, max_iter, iter, flag, actual_indices,input_array_strides, input_array_min_subscripts,input_array_dims,product)           \
{                                                                                 \
  const cctk_type * typed_vdata = (const cctk_type *)(in_data);                   \
  CCTK_ATTRIBUTE_UNUSED out_type inval;                                           \
  out_type * outval = (out_type *) out_num;                                       \
  iter = 0;                                                                       \
  sum_indices = 0;                                                                \
  num_points = 1;                                                                 \
  REDUCTION_INITIAL(inval)                                                    \
  REDUCTION_INITIAL( * outval)                                                    \
                                                                                  \
  if ( N_dims == 0 )                                                              \
  {                                                                               \
    REDUCTION_PREOP_CAST(inval, typed_vdata,sum_indices, out_type);               \
    REDUCTION_OPERATION(*outval,inval);                                           \
  }                                                                               \
  else                                                                            \
  {                                                                               \
  if ( weight_on == 1)                                                            \
  {                                                                               \
    if ( input_array_offset == 0)                                                 \
    {                                                                             \
      while (iter < max_iter)                                                     \
      {                                                                           \
        sum_indices = actual_indices[0];                                          \
        for (k=N_dims-1;k>0;k--)                                                  \
        {                                                                         \
          product = 1;                                                            \
          for (j=k-1;j>=0;j--)                                                    \
          {                                                                       \
            product *= input_array_dims[j];                                       \
          }                                                                       \
          sum_indices += actual_indices[k]*product;                               \
        }                                                                         \
        weight_value = ((const CCTK_REAL *) weight)[sum_indices];                       \
                                                                                  \
        REDUCTION_PREOP_CAST(inval, typed_vdata,sum_indices, out_type);           \
        WEIGHTED_REDUCTION_OPERATION(*outval,inval,weight_value);                 \
        num_points++;                                                             \
        weight_sum += weight_value;                                               \
        iter++;                                                                   \
        flag = 0;                                                                 \
        for (k=0;k<N_dims;k++)                                                    \
        {                                                                         \
          if( indices[k] < iters_per_dim[k]-1)                                    \
          {                                                                       \
            if (flag==1)                                                          \
            {                                                                     \
              actual_indices[k] += input_array_strides[k];                        \
              indices[k]++;                                                       \
              flag = 0;                                                           \
              break;                                                              \
            }                                                                     \
            indices[k]++;                                                         \
            actual_indices[k] += input_array_strides[k];                          \
            break;                                                                \
          }                                                                       \
          else if (indices[k] == iters_per_dim[k]-1)                              \
          {                                                                       \
            indices[k] = 0;                                                       \
            actual_indices[k] = input_array_min_subscripts[k];                    \
            flag = 1;                                                             \
            continue;                                                             \
          }                                                                       \
          else                                                                    \
          {                                                                       \
            CCTK_WARN(1,"indices out of bounds, this should not happen");         \
            return -1;                                                            \
          }                                                                       \
        }                                                                         \
      }                                                                           \
    }                                                                             \
    else                                                                          \
    {                                                                             \
      while (iter < max_iter)                                                     \
      {                                                                           \
        sum_indices = actual_indices[0];                                          \
        for (k=N_dims-1;k>0;k--)                                                  \
        {                                                                         \
          product = 1;                                                            \
          for (j=k-1;j>=0;j--)                                                    \
          {                                                                       \
            product *= actual_iters_per_dim[j];                                   \
          }                                                                       \
          sum_indices += actual_indices[k]*product;                               \
        }                                                                         \
        /* prevent offset from giving segfaults */                                \
        if (sum_indices >= max_iter)                                              \
        {                                                                         \
          CCTK_WARN(1,"offsets and strides access unallocated memory");           \
          return -1;                                                              \
        }                                                                         \
        REDUCTION_PREOP_CAST(inval, typed_vdata,sum_indices, out_type);           \
        WEIGHTED_REDUCTION_OPERATION(*outval,inval,weight_value);                 \
        num_points++;                                                             \
        iter++;                                                                   \
        flag = 0;                                                                 \
        for (k=0;k<N_dims;k++)                                                    \
        {                                                                         \
          if( indices[k] < iters_per_dim[k]-1)                                    \
          {                                                                       \
            if (flag==1)                                                          \
            {                                                                     \
              actual_indices[k] += input_array_strides[k-1];                      \
              indices[k]++;                                                       \
              flag = 0;                                                           \
              break;                                                              \
            }                                                                     \
            indices[k]++;                                                         \
            actual_indices[k] += input_array_strides[k];                          \
            break;                                                                \
          }                                                                       \
          else if (indices[k] == iters_per_dim[k]-1)                              \
          {                                                                       \
            indices[k] = 0;                                                       \
            actual_indices[k] = input_array_min_subscripts[k];                    \
            flag = 1;                                                             \
            continue;                                                             \
          }                                                                       \
          else                                                                    \
          {                                                                       \
            CCTK_WARN(1,"indices out of bounds, this should not happen");         \
            return -1;                                                            \
          }                                                                       \
        }                                                                         \
      }                                                                           \
    }                                                                             \
  }                                                                               \
  else if (weight_on ==0)                                                         \
  {                                                                               \
    if ( input_array_offset == 0)                                                 \
    {                                                                             \
      while (iter < max_iter)                                                     \
      {                                                                           \
        sum_indices = actual_indices[0];                                          \
        for (k=N_dims-1;k>0;k--)                                                  \
        {                                                                         \
          product = 1;                                                            \
          for (j=k-1;j>=0;j--)                                                    \
          {                                                                       \
            product *= input_array_dims[j];                                       \
          }                                                                       \
          sum_indices += actual_indices[k]*product;                               \
        }                                                                         \
        REDUCTION_PREOP_CAST(inval, typed_vdata,sum_indices, out_type);           \
        REDUCTION_OPERATION(*outval,inval);                                       \
        num_points++;                                                             \
        weight_sum += weight_value;                                               \
        iter++;                                                                   \
        flag = 0;                                                                 \
        for (k=0;k<N_dims;k++)                                                    \
        {                                                                         \
          if( indices[k] < iters_per_dim[k]-1)                                    \
          {                                                                       \
            if (flag==1)                                                          \
            {                                                                     \
              actual_indices[k] += input_array_strides[k];                        \
              indices[k]++;                                                       \
              flag = 0;                                                           \
              break;                                                              \
            }                                                                     \
            indices[k]++;                                                         \
            actual_indices[k] += input_array_strides[k];                          \
            break;                                                                \
          }                                                                       \
          else if (indices[k] == iters_per_dim[k]-1)                              \
          {                                                                       \
            indices[k] = 0;                                                       \
            actual_indices[k] = input_array_min_subscripts[k];                    \
            flag = 1;                                                             \
            continue;                                                             \
          }                                                                       \
          else                                                                    \
          {                                                                       \
            CCTK_WARN(1,"indices out of bounds, this should not happen");         \
            return -1;                                                            \
          }                                                                       \
        }                                                                         \
      }                                                                           \
    }                                                                             \
    else                                                                          \
    {                                                                             \
      while (iter < max_iter)                                                     \
      {                                                                           \
        sum_indices = actual_indices[0];                                          \
        for (k=N_dims-1;k>0;k--)                                                  \
        {                                                                         \
          product = 1;                                                            \
          for (j=k-1;j>=0;j--)                                                    \
          {                                                                       \
            product *= actual_iters_per_dim[j];                                   \
          }                                                                       \
          sum_indices += actual_indices[k]*product;                               \
        }                                                                         \
        /* prevent offset from giving segfaults */                                \
        if (sum_indices >= max_iter)                                              \
        {                                                                         \
          CCTK_WARN(1,"offsets and strides access unallocated memory");           \
          return -1;                                                              \
        }                                                                         \
        REDUCTION_PREOP_CAST(inval, typed_vdata,sum_indices, out_type);           \
        REDUCTION_OPERATION(*outval,inval);                                       \
        num_points++;                                                             \
        iter++;                                                                   \
        flag = 0;                                                                 \
        for (k=0;k<N_dims;k++)                                                    \
        {                                                                         \
          if( indices[k] < iters_per_dim[k]-1)                                    \
          {                                                                       \
            if (flag==1)                                                          \
            {                                                                     \
              actual_indices[k] += input_array_strides[k-1];                      \
              indices[k]++;                                                       \
              flag = 0;                                                           \
              break;                                                              \
            }                                                                     \
            indices[k]++;                                                         \
            actual_indices[k] += input_array_strides[k];                          \
            break;                                                                \
          }                                                                       \
          else if (indices[k] == iters_per_dim[k]-1)                              \
          {                                                                       \
            indices[k] = 0;                                                       \
            actual_indices[k] = input_array_min_subscripts[k];                    \
            flag = 1;                                                             \
            continue;                                                             \
          }                                                                       \
          else                                                                    \
          {                                                                       \
            CCTK_WARN(1,"indices out of bounds, this should not happen");         \
            return -1;                                                            \
          }                                                                       \
        }                                                                         \
      }                                                                           \
    }                                                                             \
  }                                                                               \
  else                                                                            \
  {                                                                               \
    CCTK_WARN(1, "weight_on is not set to a valid value");                        \
  }                                                                               \
  }                                                                               \
  EXTRA_STEP(*outval, (out_type)num_points)                                       \
}


int LocalReduce_Mean     (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_Max  (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_Min  (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_Count   (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_Sum     (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);

int LocalReduce_L1   (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_L2   (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_L3   (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_L4   (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);
int LocalReduce_LInf (REDUCTION_LOCAL_ARRAY_OPERATOR_REGISTER_ARGLIST);


typedef int (*reduction_fn_t) (int N_dims, int operator_handle, 
                        int param_table_handle,   int N_input_arrays,
                        const CCTK_INT input_array_dims[], 
                        const CCTK_INT input_array_type_codes[],
                        const void *const input_arrays[],
                        int M_output_numbers,
                        const CCTK_INT output_number_type_codes[],
                        void * const output_numbers[]);

int LocalReduce_Reduce (int N_dims, int operator_handle, 
                          int param_table_handle,   int N_input_arrays,
                          const CCTK_INT input_array_dims[], 
                          const CCTK_INT input_array_type_codes[],
                          const void *const input_arrays[],
                          int M_output_numbers,
                          const CCTK_INT output_number_type_codes[],
                          void * const output_numbers[],reduction_fn_t reduction_fn);



#ifdef __cplusplus
}
#endif




#endif
