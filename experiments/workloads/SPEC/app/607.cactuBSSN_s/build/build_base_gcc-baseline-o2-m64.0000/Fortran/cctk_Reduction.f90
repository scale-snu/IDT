module cctk_Reduction
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_ReductionHandle (ierr, reduction) bind(c,name="cctk_reductionhandle_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::  ierr
       character(c_char) :: reduction
     end subroutine CCTK_ReductionHandle
     subroutine CCTK_ReductionArrayHandle (ierr, reduction) bind(c,name="cctk_reductionarrayhandle_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::   ierr
       character(c_char) :: reduction
     end subroutine CCTK_ReductionArrayHandle
     ! Name is too long
!!$     subroutine CCTK_ReduceOperatorImplementation &
!!$          (implementation, implementation_length, handle)
!!$       implicit none
!!$       character(*) implementation
!!$       integer      implementation_length
!!$       integer      handle
!!$     end subroutine CCTK_ReduceOperatorImplementation
     subroutine CCTK_ReduceOperator (operator, operator_length, handle)
       implicit none
       character(*) operator
       integer      operator_length
       integer      handle
     end subroutine CCTK_ReduceOperator
     subroutine CCTK_NumReduceOperators (num_operators)
       implicit none
       integer num_operators
     end subroutine CCTK_NumReduceOperators
     subroutine CCTK_ReduceLocalArrays (ierr, &  
          N_dims, &
          operator_handle, &
          param_table_handle, &
          N_input_arrays, &
          input_array_dims, &
          input_array_type_codes, &
          input_arrays, &
          M_output_numbers, &
          output_number_type_codes, &
          output_numbers) bind(c,name="cctk_reducelocalarrays_")
       use iso_c_binding
       implicit none
       integer(c_int) ::               ierr
       integer(c_int) ::               N_dims
       integer(c_int) ::               operator_handle
       integer(c_int) ::               param_table_handle
       integer(c_int) ::               N_input_arrays
       INTEGER(c_int) ::              input_array_dims(N_input_arrays)
       INTEGER(c_int) ::              input_array_type_codes(N_input_arrays)
       integer(c_size_t) :: input_arrays(N_input_arrays)
       integer(c_int) ::               M_output_numbers
       INTEGER(c_int) ::              output_number_type_codes(M_output_numbers)
       integer(c_size_t) ::         output_numbers(M_output_numbers)
     end subroutine CCTK_ReduceLocalArrays
     subroutine CCTK_LocalArrayReductionHandle (ierr, reduction) bind(c,name="cctk_localarrayreductionhandle_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::  ierr
       character(c_char) :: reduction(*)
     end subroutine CCTK_LocalArrayReductionHandle
     ! Name is too long
!!$     subroutine CCTK_LocalArrayReduceOperatorImplementation &
!!$          (implementation, implementation_length, handle)
!!$       implicit none
!!$       character(*) implementation
!!$       integer      implementation_length
!!$       integer      handle
!!$     end subroutine CCTK_LocalArrayReduceOperatorImplementation
     subroutine CCTK_LocalArrayReduceOperator &
          (operator, operator_length, handle)
       implicit none
       character(*) operator
       integer      operator_length
       integer      handle
     end subroutine CCTK_LocalArrayReduceOperator
     ! Name is too long
!!$     subroutine CCTK_NumLocalArrayReduceOperators (num_operators) bind(c,name="cctk_numlocalarrayreduceoperators_") 
!!$       implicit none
!!$       integer num_operators
!!$     end subroutine CCTK_NumLocalArrayReduceOperators
     subroutine CCTK_ReduceGridArrays (ierr, cctkGH, &  
          dest_proc, &
          local_reduce_handle, &
          param_table_handle, &
          N_input_arrays, &
          input_array_variable_indices, &
          M_output_values, &
          output_value_type_codes, &
          output_values) bind(c,name="cctk_reducegridarrays_")
       use iso_c_binding
       implicit none
       integer(c_int) ::              ierr
       integer(c_size_t) :: cctkGH
       integer(c_int) ::               dest_proc
       integer(c_int) ::               local_reduce_handle
       integer(c_int) ::               param_table_handle
       integer(c_int) ::               N_input_arrays
       INTEGER(c_int) ::              input_array_variable_indices(N_input_arrays)
       integer(c_int) ::               M_output_values
       INTEGER(c_int) ::              output_value_type_codes(M_output_values)
       integer(c_size_t) ::          output_values(M_output_values)
     end subroutine CCTK_ReduceGridArrays
     subroutine CCTK_GridArrayReductionOperator (operator, operator_length)
       implicit none
       character(*) operator
       integer      operator_length
     end subroutine CCTK_GridArrayReductionOperator
     ! Name is too long
!!$     subroutine CCTK_NumGridArrayReductionOperators (num_operators)
!!$       implicit none
!!$       integer num_operators
!!$     end subroutine CCTK_NumGridArrayReductionOperators
!!$     subroutine CCTK_ReduceLocalScalar (ierr, &  
!!$          cctkGH,  proc, operation_handle, &
!!$          inScalar, outScalar, dataType)
!!$       implicit none
!!$       integer               ierr
!!$       integer*8 cctkGH
!!$       integer               proc
!!$       integer               operation_handle
!!$       integer*8 inScalar
!!$       integer*8          outScalar
!!$       integer               dataType
!!$     end subroutine CCTK_ReduceLocalScalar
!!$     subroutine CCTK_ReduceLocalArray1D (ierr, & 
!!$          cctkGH,  proc, operation_handle, &
!!$          in_array1d, out_array1d, num_in_array1d, data_type)
!!$       implicit none
!!$       integer               ierr
!!$       integer*8 cctkGH
!!$       integer               proc
!!$       integer               operation_handle
!!$       integer*8 in_array1d
!!$       integer*8          out_array1d
!!$       integer               num_in_array1d
!!$       integer               data_type
!!$     end subroutine CCTK_ReduceLocalArray1D
!!$     subroutine CCTK_ReduceLocScalar (ierr, & 
!!$          cctkGH, proc, operation_handle, &
!!$          in_scalar, out_scalar, data_type)
!!$       implicit none
!!$       integer               ierr
!!$       integer*8 cctkGH
!!$       integer               proc
!!$       integer               operation_handle
!!$       integer*8 in_scalar
!!$       integer*8          out_scalar
!!$       integer               data_type
!!$     end subroutine CCTK_ReduceLocScalar
!!$     subroutine CCTK_ReduceLocArrayToArray1D (ierr, &  
!!$          cctkGH, proc, operation_handle, &
!!$          in_array1d, out_array1d, num_in_array1d, data_type)
!!$       implicit none
!!$       integer               ierr
!!$       integer*8 cctkGH
!!$       integer               proc
!!$       integer               operation_handle
!!$       integer*8 in_array1d
!!$       integer*8          out_array1d
!!$       integer               num_in_array1d
!!$       integer               data_type
!!$     end subroutine CCTK_ReduceLocArrayToArray1D
!!$     subroutine CCTK_ReduceLocArrayToArray2D (ierr, &  
!!$          cctkGH, proc, operation_handle, &
!!$          in_array2d, out_array2d, xsize, ysize, data_type)
!!$       implicit none
!!$       integer               ierr
!!$       integer*8 cctkGH
!!$       integer               proc
!!$       integer               operation_handle
!!$       integer*8 in_array2d
!!$       integer*8          out_array2d
!!$       integer               xsize
!!$       integer               ysize
!!$       integer               data_type
!!$     end subroutine CCTK_ReduceLocArrayToArray2D
!!$     subroutine CCTK_ReduceLocArrayToArray3D (ierr, & 
!!$          cctkGH, proc, operation_handle, &
!!$          in_array3d, out_array3d, xsize, ysize, zsize, data_type)
!!$       implicit none
!!$       integer               ierr
!!$       integer*8 cctkGH
!!$       integer               proc
!!$       integer               operation_handle
!!$       integer*8 in_array3d
!!$       integer*8          out_array3d
!!$       integer               xsize
!!$       integer               ysize
!!$       integer               zsize
!!$       integer               data_type
!!$     end subroutine CCTK_ReduceLocArrayToArray3D
  end interface
  external CCTK_Reduce
!  external CCTK_Reduce1
  external CCTK_ReduceLocalScalar
  external CCTK_ReduceLocalArray1D
  external CCTK_ReduceLocScalar
  external CCTK_ReduceLocArrayToArray1D
  external CCTK_ReduceLocArrayToArray2D
  external CCTK_ReduceLocArrayToArray3D
  external CCTK_ReduceArray
end module cctk_Reduction
