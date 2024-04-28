module cctk_Coord
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_CoordDir (dir, name, systemname) bind(c,name="cctk_coorddir_") 
       use iso_c_binding
       implicit none
       integer(c_int)      dir
       character(c_char) :: name(*)
       character(c_char) :: systemname(*)
     end subroutine CCTK_CoordDir
     subroutine CCTK_CoordIndex (ierr, dir, name, systemname) bind(c,name="cctk_coordindex_") 
       use iso_c_binding
       implicit none
       integer(c_int)      ierr
       integer(c_int)      dir
       character(c_char) :: name(*)
       character(c_char) :: systemname(*)
     end subroutine CCTK_CoordIndex
     subroutine CCTK_CoordName (name, namelen, dir, systemname)
       implicit none
       character(*) name
       integer      namelen
       integer      dir
       character(*) systemname
     end subroutine CCTK_CoordName
     subroutine CCTK_CoordRange &
          (ierr, GH, coord_lower, coord_upper, &
          coord_dir, coord_name, system_name)
       use iso_c_binding
       implicit none
       integer               ierr
       integer(c_size_t) GH
       REAL(8)             coord_lower
       REAL(8)             coord_upper
       integer               coord_dir
       character(*)          coord_name
       character(*)          system_name
     end subroutine CCTK_CoordRange
     subroutine CCTK_CoordRangePhysIndex &
          (ierr, GH, lower, upper, coorddir, coordname, systemname)
       use iso_c_binding
       implicit none
       integer               ierr
       integer(c_size_t) GH
       integer               lower
       integer               upper
       integer               coorddir
       character(*)          coordname
       character(*)          systemname
     end subroutine CCTK_CoordRangePhysIndex
     subroutine CCTK_CoordRegisterData (ierr, dir, gv, name, systemname) bind(c,name="cctk_coordregisterdata_") 
       use iso_c_binding
       implicit none
       integer(c_int)      ierr
       integer(c_int)      dir
       character(c_char) :: gv(*)
       character(c_char) :: name(*)
       character(c_char) :: systemname(*)
     end subroutine CCTK_CoordRegisterData
     subroutine CCTK_CoordSystemDim (dim, systemname) bind(c,name="cctk_coordsystemdim_") 
       use iso_c_binding
       implicit none
       integer(c_int)      dim
       character(c_char) :: systemname(*)
     end subroutine CCTK_CoordSystemDim
     subroutine CCTK_CoordSystemHandle (handle, systemname) bind(c,name="cctk_coordsystemhandle_") 
       use iso_c_binding
       implicit none
       integer(c_int)      handle
       character(c_char) :: systemname(*)
     end subroutine CCTK_CoordSystemHandle
     subroutine CCTK_CoordSystemName (name, namelen, handle)
       implicit none
       character(*) name
       integer      namelen
       integer      handle
     end subroutine CCTK_CoordSystemName
     subroutine CCTK_CoordLocalRange &
          (GH, lower, upper, coord_dir, coord_name, system_name)
       use iso_c_binding
       implicit none
       integer               ierr
       integer(c_size_t) GH
       REAL(8)             lower
       REAL(8)             upper
       integer               coord_dir
       character(*)          coord_name
       character(*)          system_name
     end subroutine CCTK_CoordLocalRange
     subroutine CCTK_CoordRegisterRange &
          (GH, coord_min, coord_max, coord_dir, coord_name, system_name)
       use iso_c_binding
       implicit none
       integer               ierr
       integer(c_size_t) GH
       REAL(8)             coord_min
       REAL(8)             coord_max
       integer               coord_dir
       character(*)          coord_name
       character(*)          system_name
     end subroutine CCTK_CoordRegisterRange
! This routine is commented out because its name is too long.
! Therefore it cannot be called from Fortran anyway.
!     subroutine CCTK_CoordRegisterRangePhysIndex &
!          (ierr, GH, coord_min, coord_max, coord_dir, coord_name, system_name)
!       implicit none
!       integer               ierr
!       integer*8 GH
!       integer               coord_min
!       integer               coord_max
!       integer               coord_dir
!       character(*)          coord_name
!       character(*)          system_name
!     end subroutine CCTK_CoordRegisterRangePhysIndex
     subroutine CCTK_NumCoordSystems (num)
       implicit none
       integer num
     end subroutine CCTK_NumCoordSystems
     subroutine CCTK_CoordSystemImplementation (imp, implen, handle)
       implicit none
       character(*) imp
       integer      implen
       integer      handle
     end subroutine CCTK_CoordSystemImplementation
  end interface
end module cctk_Coord
