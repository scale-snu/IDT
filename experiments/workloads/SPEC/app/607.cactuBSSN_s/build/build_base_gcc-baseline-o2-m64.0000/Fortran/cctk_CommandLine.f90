module cctk_CommandLine
  use iso_c_binding
  implicit none
  interface
! This subroutine has been renamed
! because it conflicts with the name of this module
     subroutine CCTK_GetCommandLine (argc, outargv)
       use iso_c_binding
       implicit none
       integer(c_int)   argc
       integer(c_size_t) outargv
     end subroutine CCTK_GetCommandLine
     subroutine CCTK_ParameterFileName (ierr, filename, filenamelen) bind(c,name="cctk_parameterfilename_") 
       use iso_c_binding
       implicit none
       integer(c_int)   ierr
       character(c_char) :: filename(*)
       integer(c_int)    filenamelen
     end subroutine CCTK_ParameterFileName
  end interface
end module cctk_CommandLine
