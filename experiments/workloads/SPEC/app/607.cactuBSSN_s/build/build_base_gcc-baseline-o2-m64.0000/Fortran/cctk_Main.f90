module cctk_Main
  use iso_c_binding
  implicit none
  interface
     subroutine CCTK_Initialise (ierr, config)
       use iso_c_binding
       implicit none
       integer      ierr
       integer(c_size_t) config
     end subroutine CCTK_Initialise
     subroutine CCTK_Evolve (ierr, config)
       use iso_c_binding
       implicit none
       integer      ierr
       integer(c_size_t) config
     end subroutine CCTK_Evolve
     subroutine CCTK_Shutdown (ierr, config)
       use iso_c_binding
       implicit none
       integer      ierr
       integer(c_size_t) config
     end subroutine CCTK_Shutdown
     subroutine CCTK_MainLoopIndex (main_loop_index)
       implicit none
       integer main_loop_index
     end subroutine CCTK_MainLoopIndex
     subroutine CCTK_SetMainLoopIndex (ierr, main_loop_index)
       implicit none
       integer ierr
       integer main_loop_index
     end subroutine CCTK_SetMainLoopIndex
  end interface
end module cctk_Main
