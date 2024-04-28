module cctk_File
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_CreateDirectory (ierr, mode, pathname) bind(c,name="cctk_createdirectory_") 
       use iso_c_binding
       implicit none
       integer(c_int)  ierr
       integer(c_int)   mode
       character(c_char) :: pathname(*)
     end subroutine CCTK_CreateDirectory
  end interface
end module cctk_File
