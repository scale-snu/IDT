module cctk_Groups
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_DecomposeName (ierr, fullname, implementation, implementation_nchars, name, name_nchars)
       implicit none
       integer      ierr
       character(*) fullname
       character(*) implementation
       integer      implementation_nchars
       character(*) name
       integer      name_nchars
     end subroutine CCTK_DecomposeName
     subroutine CCTK_FirstVarIndex (index, group) bind(c,name="cctk_firstvarindex_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      index
       character(c_char) :: group(*)
     end subroutine CCTK_FirstVarIndex
     subroutine CCTK_FirstVarIndexI (index, group) bind(c,name="cctk_firstvarindexi_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: index
       integer(c_int) :: group
     end subroutine CCTK_FirstVarIndexI
     subroutine CCTK_FullName (nchars, var, fullname) bind(c,name="cctk_fullname_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      nchars
       integer(c_int) ::      var
       character(c_char) :: fullname(*)
     end subroutine CCTK_FullName
     ! CCTK_GroupData fills a structure and has no Fortran wrapper
     subroutine CCTK_GroupDimI (dim, group) bind(c,name="cctk_groupdimi_") 
       use iso_c_binding
       implicit none
       integer(c_int) dim
       integer(c_int) :: group
     end subroutine CCTK_GroupDimI
     subroutine CCTK_GroupDimFromVarI (dim, var) bind(c,name="cctk_groupdimfromvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: dim
       integer(c_int) :: var
     end subroutine CCTK_GroupDimFromVarI
     subroutine CCTK_GroupDistribNumber (number, distrib) bind(c,name="cctk_groupdistribnumber_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      number
       character(c_char) :: distrib(*)
     end subroutine CCTK_GroupDistribNumber
     ! CCTK_GroupGhostsizesI is a strange function and has no Fortran wrapper
     subroutine CCTK_ImplementationI (nchars, group, implementation)
       implicit none
       integer      nchars
       integer      group
       character(*) implementation
     end subroutine CCTK_ImplementationI
     subroutine CCTK_GroupIndex (index, group) bind(c,name="cctk_groupindex_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::    index
       character(c_char) :: group(*)
     end subroutine CCTK_GroupIndex
     subroutine CCTK_GroupIndexFromVar (index, var) bind(c,name="cctk_groupindexfromvar_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::     index
       character(c_char) :: var(*)
     end subroutine CCTK_GroupIndexFromVar
     subroutine CCTK_GroupIndexFromVarI (index, var) bind(c,name="cctk_groupindexfromvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: index
       integer(c_int) :: var
     end subroutine CCTK_GroupIndexFromVarI
     subroutine CCTK_GroupName (nchars, group, groupname) bind(c,name="cctk_groupname_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      nchars
       integer(c_int) ::      group
       character(c_char) :: groupname(*)
     end subroutine CCTK_GroupName
     subroutine CCTK_GroupNameFromVarI (nchars, var, groupname) bind(c,name="cctk_groupnamefromvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      nchars
       integer(c_int) ::      var
       character(c_char) :: groupname(*)
     end subroutine CCTK_GroupNameFromVarI
     subroutine CCTK_GroupScopeNumber (number, scope) bind(c,name="cctk_groupscopenumber_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      number
       character(c_char) :: scope(*)
     end subroutine CCTK_GroupScopeNumber
     ! CCTK_GroupSizesI is a strange function and has no Fortran wrapper
     subroutine CCTK_GroupTypeFromVarI (type, var) bind(c,name="cctk_grouptypefromvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: type
       integer(c_int) :: var
     end subroutine CCTK_GroupTypeFromVarI
     subroutine CCTK_GroupTypeNumber (number, type) bind(c,name="cctk_grouptypenumber_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      number
       character(c_char) :: type(*)
     end subroutine CCTK_GroupTypeNumber
     subroutine CCTK_GroupTypeI (type, group) bind(c,name="cctk_grouptypei_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: type
       integer(c_int) :: group
     end subroutine CCTK_GroupTypeI
     subroutine CCTK_ImpFromVarI (nchars, var, imp) bind(c,name="cctk_impfromvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      nchars
       integer (c_int) ::    var
       character(c_char) :: imp(*)
     end subroutine CCTK_ImpFromVarI
     subroutine CCTK_MaxDim (maxdim) bind(c,name="cctk_maxdim_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: maxdim
     end subroutine CCTK_MaxDim
     subroutine CCTK_NumGroups (numgroups) bind(c,name="cctk_numgroups_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: numgroups
     end subroutine CCTK_NumGroups
     subroutine CCTK_NumTimeLevelsFromVar (numtimelevels, var) bind(c,name="cctk_numtimelevelsfromvar_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      numtimelevels
       character(c_char) :: var(*)
     end subroutine CCTK_NumTimeLevelsFromVar
     subroutine CCTK_NumTimeLevelsFromVarI (numtimelevels, var) bind(c,name="cctk_numtimelevelsfromvari_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: numtimelevels
       integer(c_int) :: var
     end subroutine CCTK_NumTimeLevelsFromVarI
     subroutine CCTK_NumTimeLevels (numtimelevels, var) bind(c,name="cctk_numtimelevels_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      numtimelevels
       character(c_char) :: var(*)
     end subroutine CCTK_NumTimeLevels
     subroutine CCTK_NumTimeLevelsI (numtimelevels, var) bind(c,name="cctk_numtimelevelsi_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: numtimelevels
       integer(c_int) :: var
     end subroutine CCTK_NumTimeLevelsI
     subroutine CCTK_MaxTimeLevels (maxtimelevels, group) bind(c,name="cctk_maxtimelevels_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      maxtimelevels
       character(c_char) :: group(*)
     end subroutine CCTK_MaxTimeLevels
     subroutine CCTK_MaxTimeLevelsVN (maxtimelevels, var) bind(c,name="cctk_maxtimelevelsvn_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      maxtimelevels
       character(c_char) :: var(*)
     end subroutine CCTK_MaxTimeLevelsVN
     subroutine CCTK_MaxTimeLevelsVI (maxtimelevels, var) bind(c,name="cctk_maxtimelevelsvi_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: maxtimelevels
       integer(c_int) :: var
     end subroutine CCTK_MaxTimeLevelsVI
     subroutine CCTK_MaxTimeLevelsGN (maxtimelevels, group) bind(c,name="cctk_maxtimelevelsgn_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      maxtimelevels
       character(c_char) :: group(*)
     end subroutine CCTK_MaxTimeLevelsGN
     subroutine CCTK_MaxTimeLevelsGI (maxtimelevels, group) bind(c,name="cctk_maxtimelevelsgi_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: maxtimelevels
       integer(c_int) :: group
     end subroutine CCTK_MaxTimeLevelsGI
     subroutine CCTK_NumVars (numvars) bind(c,name="cctk_numvars_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: numvars
     end subroutine CCTK_NumVars
     subroutine CCTK_NumVarsInGroup (numvars, group) bind(c,name="cctk_numvarsingroup_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      numvars
       character(c_char) :: group(*)
     end subroutine CCTK_NumVarsInGroup
     subroutine CCTK_NumVarsInGroupI (numvars, group) bind(c,name="cctk_numvarsingroupi_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: numvars
       integer(c_int) :: group
     end subroutine CCTK_NumVarsInGroupI
     subroutine CCTK_VarIndex (index, var) bind(c,name="cctk_varindex_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      index
       character(c_char) :: var(*)
     end subroutine CCTK_VarIndex
     subroutine CCTK_VarName (nchars, var, varname) bind(c,name="cctk_varname_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      nchars
       integer(c_int) ::      var
       character(c_char) :: varname(*)
     end subroutine CCTK_VarName
     subroutine CCTK_VarTypeI (type, var) bind(c,name="cctk_vartypei_") 
       use iso_c_binding
       implicit none
       integer(c_int) type
       integer(c_int) :: var
     end subroutine CCTK_VarTypeI
     subroutine CCTK_VarTypeNumber (number, type) bind(c,name="cctk_vartypenumber_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      number
       character(c_char) :: type(*)
     end subroutine CCTK_VarTypeNumber
     subroutine CCTK_VarTypeName (nchars, type, typename) bind(c,name="cctk_vartypename_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      nchars
       integer(c_int) ::      type
       character(c_char) :: typename(*)
     end subroutine CCTK_VarTypeName
     subroutine CCTK_VarTypeSize (size, type) bind(c,name="cctk_vartypesize_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: size
       integer(c_int) :: type
     end subroutine CCTK_VarTypeSize
     ! CCTKi_GroupLengthAsPointer is a strange function and has no
     ! Fortran wrapper
     ! CCTK_TraverseString has no Fortran wrapper
     subroutine CCTK_GroupTagsTable (table, group) bind(c,name="cctk_grouptagstable_") 
       use iso_c_binding
       implicit none
       integer(c_int) ::      table
       character(c_char) :: group(*)
     end subroutine CCTK_GroupTagsTable
     subroutine CCTK_GroupTagsTableI (table, group) bind(c,name="cctk_grouptagstablei_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: table
       integer(c_int) :: group
     end subroutine CCTK_GroupTagsTableI
  end interface
end module cctk_Groups
