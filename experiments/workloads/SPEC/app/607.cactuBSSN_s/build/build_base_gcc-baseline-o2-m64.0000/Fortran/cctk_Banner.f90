module cctk_Banner
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_RegisterBanner (ierr, banner) bind(c,name="cctk_registerbanner_") 
       use iso_c_binding
       implicit none
       integer(c_int)     ierr
       character(c_char) :: banner(*)
     end subroutine CCTK_RegisterBanner
  end interface
end module cctk_Banner
