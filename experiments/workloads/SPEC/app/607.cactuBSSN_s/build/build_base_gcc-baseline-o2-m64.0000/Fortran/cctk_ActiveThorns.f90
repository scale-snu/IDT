module cctk_ActiveThorns
  use iso_c_binding
  implicit none
  interface
! This routine has been made a function instead of a subroutine
! and is declared in cctk.h
!     subroutine CCTK_IsThornActive (ierr, name) bind(c,name="cctk_isthornactive_") 
!       implicit none
!       integer      ierr
!       character(*) name
!     end subroutine CCTK_IsThornActive
     subroutine CCTK_ThornHandle (handle, name)
       use iso_c_binding
       implicit none
       integer(c_size_t) handle
       character(c_char) name(*)
     end subroutine CCTK_ThornHandle
     subroutine CCTK_IsThornActiveH (ierr, handle)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) handle
     end subroutine CCTK_IsThornActiveH
     subroutine CCTK_IsThornCompiled (ierr, name) bind(c,name="cctk_isthorncompiled_") 
       use iso_c_binding
       implicit none
       integer(c_int)      ierr
       character(c_char) :: name(*)
     end subroutine CCTK_IsThornCompiled
     subroutine CCTK_IsImplementationActive (ierr, name) bind(c,name="cctk_isimplementationactive_") 
       use iso_c_binding
       implicit none
       integer(c_int)      ierr
       character(c_char) :: name(*)
     end subroutine CCTK_IsImplementationActive
     subroutine CCTK_IsImplementationCompiled (ierr, name) bind(c,name="cctk_isimplementationcompiled_") 
       use iso_c_binding
       implicit none
       integer(c_int)      ierr
       character(c_char) :: name(*)
     end subroutine CCTK_IsImplementationCompiled
     subroutine CCTK_ActivatingThorn (thorn, imp)
       implicit none
       character(*) thorn
       character(*) imp
     end subroutine CCTK_ActivatingThorn
     subroutine CCTK_ImpThornList (thornlist, imp)
       use iso_c_binding
       implicit none
       integer(c_size_t) thornlist
       character(c_char) imp(*)
     end subroutine CCTK_ImpThornList
     subroutine CCTK_ThornImplementation (imp, thorn)
       implicit none
       character(*) imp
       character(*) thorn
     end subroutine CCTK_ThornImplementation
     subroutine CCTK_ImplementationThorn (thorn, imp)
       implicit none
       character(*) thorn
       character(*) imp
     end subroutine CCTK_ImplementationThorn
     subroutine CCTK_ImplementationRequires (implist, imp)
       use iso_c_binding
       implicit none
       integer(c_size_t) implist
       character(c_char) imp(*)
     end subroutine CCTK_ImplementationRequires
     subroutine CCTK_NumCompiledThorns (num)
       implicit none
       integer num
     end subroutine CCTK_NumCompiledThorns
     subroutine CCTK_NumCompiledImplementations (num)
       implicit none
       integer num
     end subroutine CCTK_NumCompiledImplementations
     subroutine CCTK_CompiledThorn (thorn_name, thorn_index)
       implicit none
       character(*) thorn_name
       integer      thorn_index
     end subroutine CCTK_CompiledThorn
     subroutine CCTK_CompiledImplementation (imp_name, imp_index)
       implicit none
       character(*) imp_name
       integer      imp_index
     end subroutine CCTK_CompiledImplementation
  end interface
end module cctk_ActiveThorns
