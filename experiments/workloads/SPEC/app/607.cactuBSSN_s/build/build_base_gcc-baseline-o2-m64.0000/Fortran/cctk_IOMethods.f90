module cctk_IOMethods
  use iso_c_binding
  implicit none
  interface
     subroutine CCTKi_RegisterIOMethod (handle, thorn, name) bind(c,name="cctki_registeriomethod_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: handle
       character(c_char) :: thorn(*)
       character(c_char) :: name (*)
     end subroutine CCTKi_RegisterIOMethod
     subroutine CCTK_RegisterIOMethodOutputGH (ierr, handle, OutputGH)
       use iso_c_binding
       implicit none
       integer       ierr
       integer       handle
       integer(c_size_t) OutputGH
     end subroutine CCTK_RegisterIOMethodOutputGH
     subroutine CCTK_IOMethodImplementation (nchars, imp, handle)
       implicit none
       integer      nchars
       character(*) imp
       integer      handle
     end subroutine CCTK_IOMethodImplementation
     subroutine CCTK_IOMethod (nchars, method, handle)
       implicit none
       integer      nchars
       character(*) method
       integer      handle
     end subroutine CCTK_IOMethod
     subroutine CCTK_NumIOMethods (num)
       implicit none
       integer num
     end subroutine CCTK_NumIOMethods
  end interface
end module cctk_IOMethods
