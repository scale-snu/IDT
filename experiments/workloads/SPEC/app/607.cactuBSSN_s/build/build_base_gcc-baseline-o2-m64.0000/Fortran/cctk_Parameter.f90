module cctk_Parameter
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_ParameterLevel (level)
       implicit none
       integer level
     end subroutine CCTK_ParameterLevel
     subroutine CCTK_ParameterSet (ierr, name, thorn, value) bind(c,name="cctk_parameterset_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: ierr
       character(c_char) :: name(*)
       character(c_char) :: thorn(*)
       character(c_char) :: value(*)
     end subroutine CCTK_ParameterSet
     subroutine CCTK_ParameterGet (param, name, thorn, type)
       implicit none
       integer param
       character name(*)
       character thorn(*)
       integer      type
     end subroutine CCTK_ParameterGet
     subroutine CCTK_ParameterValString (nchars, param, thorn, value) bind(c,name="cctk_parametervalstring_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::    nchars
       character(c_char) :: param(*)
       character(c_char) :: thorn(*)
       character(c_char) :: value(*)
     end subroutine CCTK_ParameterValString
     subroutine CCTK_ParameterWalk (ierr, first, origin, pfullname, pdata)
       implicit none
       integer      ierr
       integer      first
       character origin(*)
       integer pfullname
       integer pdata
     end subroutine CCTK_ParameterWalk
     subroutine CCTK_ParamtereDate (data, name, thorn)
       implicit none
       integer data
       character          name(*)
       character          thorn(*)
     end subroutine CCTK_ParamtereDate
     subroutine CCTK_ParameterQueryTimesSet (ierr, name, thorn)
       implicit none
       integer ierr
       character(*) name
       character(*) thorn
     end subroutine CCTK_ParameterQueryTimesSet
  end interface
end module cctk_Parameter
