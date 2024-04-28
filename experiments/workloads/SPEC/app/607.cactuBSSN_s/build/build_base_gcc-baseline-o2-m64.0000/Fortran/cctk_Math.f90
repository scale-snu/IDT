module cctk_Math
  use iso_c_binding
  implicit none
  interface
     function CCTK_copysign (x, y) bind(c,name="cctk_copysign_") 
       use iso_c_binding
       implicit none
       real(c_double) :: CCTK_copysign
       real(c_double), intent(in) :: x, y
     end function CCTK_copysign
     function CCTK_fpclassify (x) bind(c,name="cctk_fpclassify_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: CCTK_fpclassify  
       real(c_double), intent(in) :: x
     end function CCTK_fpclassify
     function CCTK_isfinite (x) bind(c,name="cctk_isfinite_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: CCTK_isfinite
       real(c_double), intent(in) :: x
     end function CCTK_isfinite
     function CCTK_isinf (x) bind(c,name="cctk_isinf_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: CCTK_isinf
       real(c_double), intent(in) :: x
     end function CCTK_isinf
     function CCTK_isnan (x) bind(c,name="cctk_isnan_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: CCTK_isnan
       real(c_double), intent(in) :: x
     end function CCTK_isnan
     function CCTK_isnormal (x) bind(c,name="cctk_isnormal_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: CCTK_isnormal
       real(c_double), intent(in) :: x
     end function CCTK_isnormal
     function CCTK_signbit (x) bind(c,name="cctk_signbit_") 
       use iso_c_binding
       implicit none
       integer(c_int) :: CCTK_signbit
       real(c_double), intent(in) :: x
     end function CCTK_signbit
  end interface
end module cctk_Math
