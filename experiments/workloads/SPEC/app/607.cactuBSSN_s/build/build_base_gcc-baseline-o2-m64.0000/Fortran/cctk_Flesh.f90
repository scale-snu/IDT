module cctk_Flesh
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_Traverse (ierr, GH, where)
       use iso_c_binding
       implicit none
       integer(c_int)  ierr
       integer(c_size_t) GH
       character(c_char) where(*)
     end subroutine CCTK_Traverse
  end interface
end module cctk_Flesh
