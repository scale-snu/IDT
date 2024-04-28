module cctk_WarnLevel
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_Warn (level, line, file, thorn, message) bind(c,name="cctk_warn_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::  level
       integer(c_int) ::  line
       character(c_char) :: file(*)
       character(c_char) :: thorn(*)
       character(c_char) :: message(*)
     end subroutine CCTK_Warn
     subroutine CCTK_ParamWarn (thorn, message) bind(c,name="cctk_paramwarn_") 
       use iso_c_binding
       implicit none
       character(c_char) :: thorn(*)
       character(c_char) :: message(*)
     end subroutine CCTK_ParamWarn
     subroutine CCTK_Info (thorn, message) bind(c,name="cctk_info_") 
       use iso_c_binding
       implicit none
       character(c_char) :: thorn(*)
       character(c_char) :: message(*)
     end subroutine CCTK_Info
  end interface
  ! Do not declare these, because some compilers insist on a
  ! definition once they have seen an external declaration:
!!$  external CCTK_VWarn
!!$  external CCTK_VParamWarn
!!$  external CCTK_VInfo
end module cctk_WarnLevel
