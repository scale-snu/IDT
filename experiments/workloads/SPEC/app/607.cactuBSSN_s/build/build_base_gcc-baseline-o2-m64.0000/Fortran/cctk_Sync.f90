module cctk_Sync
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_SyncGroupI (ierr, cctkGH, group) bind(c,name="cctk_syncgroupi_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::              ierr
       integer(c_size_t) :: cctkGH
       integer(c_int) ::              group
     end subroutine CCTK_SyncGroupI
     subroutine CCTK_SyncGroupWithVar (ierr, cctkGH, varname) bind(c,name="cctk_syncgroupwithvar_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::              ierr
       integer(c_size_t) :: cctkGH
       character(c_char) ::  varname(*)
     end subroutine CCTK_SyncGroupWithVar
     subroutine CCTK_SyncGroupWithVarI (ierr, cctkGH, var) bind(c,name="cctk_syncgroupwithvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::              ierr
       integer(c_size_t) :: cctkGH
       integer(c_int) ::              var
     end subroutine CCTK_SyncGroupWithVarI
     subroutine CCTK_SyncGroups (ierr, cctkGH, n_groups, groups)
       use iso_c_binding
       implicit none
       integer(c_int) ::              ierr
       integer(c_size_t) :: cctkGH
       integer(c_int) ::              n_groups
       integer(c_int) ::              groups(n_groups)
     end subroutine CCTK_SyncGroups
  end interface
end module cctk_Sync
