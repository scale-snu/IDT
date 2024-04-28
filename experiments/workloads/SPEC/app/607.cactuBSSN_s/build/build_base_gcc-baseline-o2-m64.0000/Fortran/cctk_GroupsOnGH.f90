module cctk_GroupsOnGH
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_VarDataPtr (ptr, GH, timelevel, fullvarname) bind(c,name="cctk_vardataptr_") 
       use iso_c_binding
       implicit none
       integer(c_size_t)          ptr
       integer(c_size_t) GH
       integer(c_int)               timelevel
       character(c_char) :: fullvarname(*)
     end subroutine CCTK_VarDataPtr
     subroutine CCTK_VarDataPtrI (ptr, GH, timelevel, varindex) bind(c,name="cctk_vardataptri_") 
       use iso_c_binding
       implicit none
       integer(c_size_t)          ptr
       integer(c_size_t) GH
       integer(c_int)               timelevel
       integer(c_int)               varindex
     end subroutine CCTK_VarDataPtrI
     subroutine CCTK_VarDataPtrB (ptr, GH, timelevel, varindex, fullvarname)
       use iso_c_binding
       implicit none
       integer(c_size_t)          ptr
       integer(c_size_t) GH
       integer(c_int)               timelevel
       integer(c_int)               varindex
       character(c_char) ::  fullvarname(*)
     end subroutine CCTK_VarDataPtrB
     subroutine CCTK_DisableGroupStorageI (ierr, GH, group)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               group
     end subroutine CCTK_DisableGroupStorageI
     subroutine CCTK_DisableGroupCommI (ierr, GH, group)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               group
     end subroutine CCTK_DisableGroupCommI
     subroutine CCTK_EnableGroupStorageI (ierr, GH, group)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               group
     end subroutine CCTK_EnableGroupStorageI
     subroutine CCTK_EnableGroupCommI (ierr, GH, group)
       use iso_c_binding
       implicit none
       integer(c_int)               ierr
       integer(c_size_t) GH
       integer(c_int)               group
     end subroutine CCTK_EnableGroupCommI
     subroutine CCTK_GrouplbndGN (ierr, GH, dim, lbnd, groupname) bind(c,name="cctk_grouplbndgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lbnd(dim)
       character(c_char) ::  groupname(*)
     end subroutine CCTK_GrouplbndGN
     subroutine CCTK_GrouplbndVN (ierr, GH, dim, lbnd, varname) bind(c,name="cctk_grouplbndvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lbnd(dim)
       character(c_char) ::  varname(*)
     end subroutine CCTK_GrouplbndVN
     subroutine CCTK_GrouplbndGI (ierr, GH, dim, lbnd, groupindex) bind(c,name="cctk_grouplbndgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lbnd(dim)
       integer(c_int)                groupindex
     end subroutine CCTK_GrouplbndGI
     subroutine CCTK_GrouplbndVI (ierr, GH, dim, lbnd, varindex) bind(c,name="cctk_grouplbndvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lbnd(dim)
       integer(c_int)                varindex
     end subroutine CCTK_GrouplbndVI
     subroutine CCTK_GroupubndGN (ierr, GH, dim, ubnd, groupname) bind(c,name="cctk_groupubndgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                ubnd(dim)
       character(c_char) ::  groupname(*)
     end subroutine CCTK_GroupubndGN
     subroutine CCTK_GroupubndVN (ierr, GH, dim, ubnd, varname) bind(c,name="cctk_groupubndvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                ubnd(dim)
       character(c_char) ::  varname(*)
     end subroutine CCTK_GroupubndVN
     subroutine CCTK_GroupubndGI (ierr, GH, dim, ubnd, groupindex) bind(c,name="cctk_groupubndgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                ubnd(dim)
       integer(c_int)                groupindex
     end subroutine CCTK_GroupubndGI
     subroutine CCTK_GroupubndVI (ierr, GH, dim, ubnd, varindex) bind(c,name="cctk_groupubndvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                ubnd(dim)
       integer(c_int)                varindex
     end subroutine CCTK_GroupubndVI
     subroutine CCTK_GrouplshGN (ierr, GH, dim, lsh, groupname) bind(c,name="cctk_grouplshgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lsh(dim)
       character(c_char) ::  groupname(*)
     end subroutine CCTK_GrouplshGN
     subroutine CCTK_GrouplshVN (ierr, GH, dim, lsh, varname) bind(c,name="cctk_grouplshvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lsh(dim)
       character(c_char) ::  varname(*)
     end subroutine CCTK_GrouplshVN
     subroutine CCTK_GrouplshGI (ierr, GH, dim, lsh, groupindex) bind(c,name="cctk_grouplshgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lsh(dim)
       integer(c_int)                groupindex
     end subroutine CCTK_GrouplshGI
     subroutine CCTK_GrouplshVI (ierr, GH, dim, lsh, varindex) bind(c,name="cctk_grouplshvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                lsh(dim)
       integer(c_int)                varindex
     end subroutine CCTK_GrouplshVI
     subroutine CCTK_GroupgshGN (ierr, GH, dim, gsh, groupname) bind(c,name="cctk_groupgshgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                gsh(dim)
       character(c_char) ::  groupname(*)
     end subroutine CCTK_GroupgshGN
     subroutine CCTK_GroupgshVN (ierr, GH, dim, gsh, varname) bind(c,name="cctk_groupgshvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                gsh(dim)
       character(c_char) ::  varname(*)
     end subroutine CCTK_GroupgshVN
     subroutine CCTK_GroupgshGI (ierr, GH, dim, gsh, groupindex) bind(c,name="cctk_groupgshgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                gsh(dim)
       integer(c_int)                groupindex
     end subroutine CCTK_GroupgshGI
     subroutine CCTK_GroupgshVI (ierr, GH, dim, gsh, varindex) bind(c,name="cctk_groupgshvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                gsh(dim)
       integer(c_int)                varindex
     end subroutine CCTK_GroupgshVI
     subroutine CCTK_GroupbboxGN (ierr, GH, size, bbox, groupname) bind(c,name="cctk_groupbboxgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                size
       integer(c_int)                bbox(size)
       character(c_char) ::  groupname(*)
     end subroutine CCTK_GroupbboxGN
     subroutine CCTK_GroupbboxVN (ierr, GH, size, bbox, varname) bind(c,name="cctk_groupbboxvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                size
       integer(c_int)                bbox(size)
       character(c_char) ::  varname(*)
     end subroutine CCTK_GroupbboxVN
     subroutine CCTK_GroupbboxGI (ierr, GH, size, bbox, groupindex) bind(c,name="cctk_groupbboxgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                size
       integer(c_int)                bbox(size)
       integer(c_int)                groupindex
     end subroutine CCTK_GroupbboxGI
     subroutine CCTK_GroupbboxVI (ierr, GH, size, bbox, varindex) bind(c,name="cctk_groupbboxvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                size
       integer(c_int)                bbox(size)
       integer(c_int)                varindex
     end subroutine CCTK_GroupbboxVI
     subroutine CCTK_GroupnghostzonesGN (ierr, GH, dim, nghostzones, groupname) bind(c,name="cctk_groupnghostzonesgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                nghostzones(dim)
       character(c_char) ::  groupname(*)
     end subroutine CCTK_GroupnghostzonesGN
     subroutine CCTK_GroupnghostzonesVN (ierr, GH, dim, nghostzones, varname) bind(c,name="cctk_groupnghostzonesvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                nghostzones(dim)
       character(c_char) ::  varname(*)
     end subroutine CCTK_GroupnghostzonesVN
     subroutine CCTK_GroupnghostzonesGI (ierr, GH, dim, nghostzones, groupindex) bind(c,name="cctk_groupnghostzonesgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                nghostzones(dim)
       integer(c_int)                groupindex
     end subroutine CCTK_GroupnghostzonesGI
     subroutine CCTK_GroupnghostzonesVI (ierr, GH, dim, nghostzones, varindex) bind(c,name="cctk_groupnghostzonesvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                dim
       integer(c_int)                nghostzones(dim)
       integer(c_int)                varindex
     end subroutine CCTK_GroupnghostzonesVI
     subroutine CCTK_ActiveTimeLevels (ierr, GH, groupname) bind(c,name="cctk_activetimelevels_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       character(c_char) ::  groupname(*)
     end subroutine CCTK_ActiveTimeLevels
     subroutine CCTK_ActiveTimeLevelsGN (ierr, GH, groupname) bind(c,name="cctk_activetimelevelsgn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       character(c_char) ::   groupname(*)
     end subroutine CCTK_ActiveTimeLevelsGN
     subroutine CCTK_ActiveTimeLevelsGI (ierr, GH, groupindex) bind(c,name="cctk_activetimelevelsgi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                groupindex
     end subroutine CCTK_ActiveTimeLevelsGI
     subroutine CCTK_ActiveTimeLevelsVN (ierr, GH, varname) bind(c,name="cctk_activetimelevelsvn_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       character(c_char) ::  varname(*)
     end subroutine CCTK_ActiveTimeLevelsVN
     subroutine CCTK_ActiveTimeLevelsVI (ierr, GH, varindex) bind(c,name="cctk_activetimelevelsvi_") 
       use iso_c_binding
       implicit none
       integer(c_int)                ierr
       integer(c_size_t) GH
       integer(c_int)                varindex
     end subroutine CCTK_ActiveTimeLevelsVI
  end interface
end module cctk_GroupsOnGH
