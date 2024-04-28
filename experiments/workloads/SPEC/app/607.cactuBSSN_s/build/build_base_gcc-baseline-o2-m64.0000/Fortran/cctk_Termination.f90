module cctk_Termination
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_TerminationReached (ires, cctkGH)
       use iso_c_binding
       implicit none
       integer               ires
       integer(c_size_t) cctkGH
     end subroutine CCTK_TerminationReached
     subroutine CCTK_TerminateNext (cctkGH) bind(c,name="cctk_terminatenext_") 
       use iso_c_binding
       implicit none
       integer(c_size_t) cctkGH
     end subroutine CCTK_TerminateNext
  end interface
end module cctk_Termination
