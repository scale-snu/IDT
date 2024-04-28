module cctk_IO
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_OutputGH (ierr, GH) bind(c,name="cctk_outputgh_") 
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
     end subroutine CCTK_OutputGH
     subroutine CCTK_OutputVarAs (ierr, GH, var, alias) bind(c,name="cctk_outputvaras_") 
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       character(c_char) ::  var(*)
       character(c_char) ::  alias(*)
     end subroutine CCTK_OutputVarAs
     subroutine CCTK_OutputVar (ierr, GH, var) bind(c,name="cctk_outputvar_") 
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       character(c_char) ::  var(*)
     end subroutine CCTK_OutputVar
     subroutine CCTK_OutputVarByMethod (ierr, GH, var, method) bind(c,name="cctk_outputvarbymethod_") 
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       character(c_char) ::  var(*)
       character(c_char) ::  method(*)
     end subroutine CCTK_OutputVarByMethod
  end interface
end module cctk_IO
