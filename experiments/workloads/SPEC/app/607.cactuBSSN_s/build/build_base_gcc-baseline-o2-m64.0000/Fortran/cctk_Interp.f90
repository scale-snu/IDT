module cctk_Interp
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_InterpHandle (handle, name) bind(c,name="cctk_interphandle_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::  handle
       character(c_char) :: name(*)
     end subroutine CCTK_InterpHandle
     subroutine CCTK_InterpOperator (nchars, operator, handle)
       implicit none
       integer      nchars
       character(*) operator
       integer      handle
     end subroutine CCTK_InterpOperator
     subroutine CCTK_NumInterpOperators (num)
       implicit none
       integer num
     end subroutine CCTK_NumInterpOperators
     ! CCTK_InterpGV is declared below
     ! CCTK_InterpLocal is declared below
     subroutine CCTK_InterpLocalUniform &
          (ierr,                        &
           N_dims,                      &
           operator_handle,             &
           param_table_handle,          &
           coord_origin,                &
           coord_delta,                 &
           N_interp_points,             &
             interp_coords_type_code,   &
             interp_coords,             &
           N_input_arrays,              &
             input_array_dims,          &
             input_array_type_codes,    &
             input_arrays,              &
           N_output_arrays,             &
             output_array_type_codes,   &
             output_arrays)
       implicit none
       integer               ierr
       integer               N_dims
       integer               operator_handle
       integer               param_table_handle
       REAL(8)             coord_origin(N_dims)
       REAL(8)             coord_delta(N_dims)
       integer               N_interp_points
       integer                 interp_coords_type_code
       integer(8)   interp_coords(N_dims)
       integer               N_input_arrays
       INTEGER(4)                input_array_dims(N_dims)
       INTEGER(4)                input_array_type_codes(N_input_arrays)
       integer(8)   input_arrays(N_input_arrays)
       integer               N_output_arrays
       INTEGER(4)                output_array_type_codes(N_output_arrays)
       integer(8)            output_arrays(N_output_arrays)
     end subroutine CCTK_InterpLocalUniform
  end interface
end module cctk_Interp
