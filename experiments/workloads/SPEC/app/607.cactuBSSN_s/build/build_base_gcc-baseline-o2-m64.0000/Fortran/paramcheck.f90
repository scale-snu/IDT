subroutine CheckFortranParameters (cctk_dim,cctk_gsh,cctk_lsh,cctk_lbnd,cctk_ubnd,cctk_ash,cctk_from,cctk_to,cctk_bbox,cctk_delta_t& 
  &ime,cctk_time,cctk_delta_space,cctk_origin_space,cctk_levfac,cctk_levoff,cctk_levoffdenom,cctk_timefac,cctk_convlevel,cctk_convf&
  &ac,cctk_nghostzones,cctk_iteration,cctkGH,cctk_ash1,cctk_ash2,cctk_ash3) &
  &bind(c,name="checkfortranparameters_")
  use iso_c_binding
  implicit none
  INTEGER(c_int) cctk_dim
  integer(c_int), parameter :: cctki_use_cctk_dim = kind(cctk_dim)
  INTEGER(c_int) cctk_gsh (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_gsh = kind(cctk_gsh)
  INTEGER(c_int) cctk_lsh (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_lsh = kind(cctk_lsh)
  INTEGER(c_int) cctk_lbnd (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_lbnd = kind(cctk_lbnd)
  INTEGER(c_int) cctk_ubnd (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_ubnd = kind(cctk_ubnd)
  INTEGER(c_int) cctk_ash (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_ash = kind(cctk_ash)
  INTEGER(c_int) cctk_from (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_from = kind(cctk_from)
  INTEGER(c_int) cctk_to (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_to = kind(cctk_to)
  INTEGER(c_int) cctk_bbox (2*cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_bbox = kind(cctk_bbox)
  REAL(c_double) cctk_delta_time
  integer(c_int), parameter :: cctki_use_cctk_delta_time = kind(cctk_delta_time)
  REAL(c_double) cctk_time
  integer(c_int), parameter :: cctki_use_cctk_time = kind(cctk_time)
  REAL(c_double) cctk_delta_space (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_delta_space = kind(cctk_delta_space)
  REAL(c_double) cctk_origin_space (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_origin_space = kind(cctk_origin_space)
  INTEGER(c_int) cctk_levfac (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_levfac = kind(cctk_levfac)
  INTEGER(c_int) cctk_levoff (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_levoff = kind(cctk_levoff)
  INTEGER(c_int) cctk_levoffdenom (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_levoffdenom = kind(cctk_levoffdenom)
  INTEGER(c_int) cctk_timefac
  integer(c_int), parameter :: cctki_use_cctk_timefac = kind(cctk_timefac)
  INTEGER(c_int) cctk_convlevel
  integer(c_int), parameter :: cctki_use_cctk_convlevel = kind(cctk_convlevel)
  INTEGER(c_int) cctk_convfac
  integer(c_int), parameter :: cctki_use_cctk_convfac = kind(cctk_convfac)
  INTEGER(c_int) cctk_nghostzones (cctk_dim)
  integer(c_int), parameter :: cctki_use_cctk_nghostzones = kind(cctk_nghostzones)
  INTEGER(c_int) cctk_iteration
  integer(c_int), parameter :: cctki_use_cctk_iteration = kind(cctk_iteration)
  integer(c_size_t) cctkGH
  integer(c_int), parameter :: cctki_use_cctkGH = kind(cctkGH)
  INTEGER(c_int) cctk_ash1
  integer(c_int), parameter :: cctki_use_cctk_ash1 = kind(cctk_ash1)
  INTEGER(c_int) cctk_ash2
  integer(c_int), parameter :: cctki_use_cctk_ash2 = kind(cctk_ash2)
  INTEGER(c_int) cctk_ash3
  integer(c_int), parameter :: cctki_use_cctk_ash3 = kind(cctk_ash3)
  
  INTEGER(c_int) one
  integer(c_int), parameter :: cctki_use_one = kind(one)
  COMMON /Fortranpriv/one
  
  external     CCTK_PointerTo
  integer(c_size_t) CCTK_PointerTo
  interface
  function CCTK_Equals (arg1, arg2) bind(c,name="cctk_equals_") 
  use iso_c_binding
  implicit none
  integer(c_int) :: CCTK_Equals  
  integer(c_size_t) arg1
  character(c_char) :: arg2(*)
  end function CCTK_Equals
  function CCTK_MyProc (cctkGH) bind(c,name="cctk_myproc_") 
  use iso_c_binding
  implicit none
  integer(c_int) CCTK_MyProc
  integer(c_size_t) cctkGH
  end function CCTK_MyProc
  function CCTK_nProcs (cctkGH) bind(c,name="cctk_nprocs_") 
  use iso_c_binding
  implicit none
  integer(c_int) :: CCTK_nProcs
  integer(c_size_t) cctkGH
  end function CCTK_nProcs
  function CCTK_IsThornActive (name) bind(c,name="cctk_isthornactive_") 
  use iso_c_binding
  implicit none
  integer(c_int) :: CCTK_IsThornActive
  character(c_char) :: name(*)
  end function CCTK_IsThornActive
  function CCTK_NullPointer () bind(c,name="cctk_nullpointer_") 
  use iso_c_binding
  implicit none
  integer(c_size_t) :: CCTK_NullPointer
  end function CCTK_NullPointer
  end interface
  
  if (one /= 1) then
     call CCTK_ParamWarn("Fortran",("Fortran parameters do not work -- check the C/Fortran language interface"))
  end if
end subroutine CheckFortranParameters
