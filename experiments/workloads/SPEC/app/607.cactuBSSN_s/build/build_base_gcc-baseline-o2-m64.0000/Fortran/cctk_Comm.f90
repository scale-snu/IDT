module cctk_Comm
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_SyncGroup (ierr, GH, group) bind(c,name="cctk_syncgroup_") 
       use iso_c_binding
       implicit none
       integer(c_int)   ierr
       integer(c_size_t) GH
       character(c_char) ::  group(*)
     end subroutine CCTK_SyncGroup
     subroutine CCTK_Barrier (ierr, GH) bind(c,name="cctk_barrier_") 
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
     end subroutine CCTK_Barrier
! This routine has been made a function instead of a subroutine
! and is declared in cctk.h
!     subroutine CCTK_MyProc (ierr, GH) bind(c,name="cctk_myproc_") 
!       implicit none
!       integer               ierr
!       integer*8 GH
!     end subroutine CCTK_MyProc
! This routine has been made a function instead of a subroutine
! and is declared in cctk.h
!     subroutine CCTK_nProcs (ierr, GH) bind(c,name="cctk_nprocs_") 
!       implicit none
!       integer               ierr
!       integer*8 GH
!     end subroutine CCTK_nProcs
     subroutine CCTK_ParallelInit (ierr, GH)
       use iso_c_binding
       implicit none
       integer(c_int)         ierr
       integer(c_size_t) GH
     end subroutine CCTK_ParallelInit
     subroutine CCTK_Exit (ierr, GH, retval) bind(c,name="cctk_exit_") 
       use iso_c_binding
       implicit none
       integer(c_int)   ierr
       integer(c_size_t) GH
       integer(c_int)   retval
     end subroutine CCTK_Exit
     subroutine CCTK_Abort (ierr, GH, retval) bind(c,name="cctk_abort_") 
       use iso_c_binding
       implicit none
       integer(c_int)      ierr
       integer(c_size_t) GH
       integer(c_int)      retval
     end subroutine CCTK_Abort
     subroutine CCTK_SetupGH (GH, config, convergence_level)
       use iso_c_binding
       implicit none
       integer(c_size_t) GH
       integer(c_size_t) config
       integer(c_int)    convergence_level
     end subroutine CCTK_SetupGH
! CCTK_ArrayGroupSizeB is not available from Fortran
     subroutine CCTK_QueryGroupStorageB (ierr, GH, group, groupname)
       use iso_c_binding
       implicit none
       integer(c_int)         ierr
       integer(c_size_t) GH
       integer(c_int)    group
       character(*)          groupname
     end subroutine CCTK_QueryGroupStorageB
     subroutine CCTK_GroupDynamicData (ierr, GH, group, data)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)            group
       integer(c_size_t)          data
     end subroutine CCTK_GroupDynamicData
     subroutine CCTK_GroupStorageIncrease &
          (ierr, GH, n_groups, groups, timelevels, status)
       use iso_c_binding
       implicit none
       integer(c_int)            ierr
       integer(c_size_t) GH
       integer(c_int)               n_groups
       integer(c_int)               groups(n_groups)
       integer(c_int)               timelevels(n_groups)
       integer(c_int)               status(n_groups)
     end subroutine CCTK_GroupStorageIncrease
     subroutine CCTK_GroupStorageDecrease &
          (ierr, GH, n_groups, groups, timelevels, status)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               n_groups
       integer(c_int)               groups(n_groups)
       integer(c_int)               timelevels(n_groups)
       integer(c_int)               status(n_groups)
     end subroutine CCTK_GroupStorageDecrease
     subroutine CCTK_InterpGridArrays           &
          (ierr,                                &
           GH,                                  &
           N_dims,                              &
           local_interp_handle,                 &
           param_table_handle,                  &
           coord_system_handle,                 &
           N_interp_points,                     &
             interp_coords_type,                &
             interp_coords,                     &
           N_input_arrays,                      &
             input_array_indices,               &
           N_output_arrays,                     &
             output_array_types,                &
             output_arrays)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               N_dims
       integer(c_int)               local_interp_handle
       integer(c_int)               param_table_handle
       integer(c_int)               coord_system_handle
       integer(c_int)               N_interp_points
       integer(c_int)                 interp_coords_type
       integer(c_size_t)   interp_coords(N_dims)
       integer(c_int)               N_input_arrays
       INTEGER(c_int)                input_array_indices(N_input_arrays)
       integer(c_int)               N_output_arrays
       INTEGER(c_int)                output_array_types(N_output_arrays)
       integer(c_size_t)            output_arrays(N_output_arrays)
     end subroutine CCTK_InterpGridArrays
     subroutine CCTK_QueryGroupStorage (ierr, GH, groupname) bind(c,name="cctk_querygroupstorage_") 
       use iso_c_binding
       implicit none
       integer(c_int)             ierr
       integer(c_size_t) GH
       character(c_char) :: groupname(*)
     end subroutine CCTK_QueryGroupStorage
     subroutine CCTK_QueryGroupStorageI (ierr, GH, group)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               group
     end subroutine CCTK_QueryGroupStorageI
! CCTK_ArrayGroupSize is not available from Fortran
! CCTK_ArrayGroupSizeI is not available from Fortran
  end interface
end module cctk_Comm
