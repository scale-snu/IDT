module cctk_FortranWrappers
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_RegisterFortranWrapper (ierr, name, function)
       use iso_c_binding
       implicit none
       integer(c_int)       ierr
       character(c_char)  name(*)
       integer(c_size_t) function
     end subroutine CCTK_RegisterFortranWrapper
     subroutine CCTK_FortranWrapper (function, name)
       use iso_c_binding
       implicit none
       integer(c_size_t) function
       character(c_char)  name(*)
     end subroutine CCTK_FortranWrapper
  end interface
end module cctk_FortranWrappers
