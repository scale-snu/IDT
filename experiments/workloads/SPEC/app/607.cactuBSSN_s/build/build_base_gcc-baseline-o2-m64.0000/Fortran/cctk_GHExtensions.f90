module cctk_GHExtensions
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_RegisterGHExtension (ierr, name)
       implicit none
       integer      ierr
       character(*) name
     end subroutine CCTK_RegisterGHExtension
     subroutine CCTK_UnregisterGHExtension (ierr, name)
       implicit none
       integer      ierr
       character(*) name
     end subroutine CCTK_UnregisterGHExtension
     subroutine CCTK_RegisterGHExtensionSetupGH (ierr, handle, func)
       use iso_c_binding
       implicit none
       integer       ierr
       integer       handle
       integer(c_size_t) func
     end subroutine CCTK_RegisterGHExtensionSetupGH
     subroutine CCTK_RegisterGHExtensionInitGH (ierr, handle, func)
       use iso_c_binding
       implicit none
       integer       ierr
       integer       handle
       integer(c_size_t) func
     end subroutine CCTK_RegisterGHExtensionInitGH
     subroutine CCTK_GHExtensionHandle (ierr, name) bind(c,name="cctk_ghextensionhandle_") 
       use iso_c_binding
       implicit none
       integer(c_int)     ierr
       character(c_char) :: name(*)
     end subroutine CCTK_GHExtensionHandle
     subroutine CCTK_GHExtension (extension, GH, name)
       use iso_c_binding
       implicit none
       integer(c_size_t)          extension
       integer(c_size_t) GH
       character(c_char)          name(*)
     end subroutine CCTK_GHExtension
  end interface
end module cctk_GHExtensions
