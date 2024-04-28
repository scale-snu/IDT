module cctk_Timers
  use iso_c_binding
       use iso_c_binding 
  implicit none
  interface
     subroutine CCTK_NumTimers (num_timers) bind(c,name="cctk_numtimers_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: num_timers
     end subroutine CCTK_NumTimers
     subroutine CCTK_NumClocks (num_clocks) bind(c,name="cctk_numclocks_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: num_clocks
     end subroutine CCTK_NumClocks
     subroutine CCTK_TimerName (timer_name, timer_length, timer_handle)
       implicit none
       character timer_name(*) 
       integer      timer_length
       integer      timer_handle
     end subroutine CCTK_TimerName
     subroutine CCTK_ClockName (clock_name, clock_length, clock_handle)
       implicit none
       character clock_name(*) 
       integer      clock_length
       integer      clock_handle
     end subroutine CCTK_ClockName
     subroutine CCTK_ClockHandle (handle, nclock)
       implicit none
       integer      handle
       character nclock(*) 
     end subroutine CCTK_ClockHandle
     subroutine CCTK_TimerCreate (handle, name) bind(c,name="cctk_timercreate_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      handle
       character(c_char) :: name(*) 
     end subroutine CCTK_TimerCreate
     subroutine CCTK_TimerCreateI (handle) bind(c,name="cctk_timercreatei_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: handle
     end subroutine CCTK_TimerCreateI
     subroutine CCTK_TimerDestroy (ierr, name) bind(c,name="cctk_timerdestroy_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      ierr
       character(c_char) :: name(*) 
     end subroutine CCTK_TimerDestroy
     subroutine CCTK_TimerDestroyI (ierr, handle) bind(c,name="cctk_timerdestroyi_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: ierr
       integer(c_int) :: handle
     end subroutine CCTK_TimerDestroyI
     subroutine CCTK_TimerStart (ierr, name) bind(c,name="cctk_timerstart_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      ierr
       character(c_char) :: name(*) 
     end subroutine CCTK_TimerStart
     subroutine CCTK_TimerStartI (ierr, handle) bind(c,name="cctk_timerstarti_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: ierr
       integer(c_int) :: handle
     end subroutine CCTK_TimerStartI
     subroutine CCTK_TimerStop (ierr, name) bind(c,name="cctk_timerstop_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      ierr
       character(c_char) :: name(*) 
     end subroutine CCTK_TimerStop
     subroutine CCTK_TimerStopI (ierr, handle) bind(c,name="cctk_timerstopi_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: ierr
       integer(c_int) :: handle
     end subroutine CCTK_TimerStopI
     subroutine CCTK_TimerReset (ierr, name) bind(c,name="cctk_timerreset_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      ierr
       character(c_char) :: name(*) 
     end subroutine CCTK_TimerReset
     subroutine CCTK_TimerResetI (ierr, handle) bind(c,name="cctk_timerreseti_") 
       use iso_c_binding 
       implicit none
       integer(c_int) :: ierr
       integer(c_int) :: handle
     end subroutine CCTK_TimerResetI
     subroutine CCTK_TimerPrintData (ierr, name, nclock) bind(c,name="cctk_timerprintdata_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      ierr
       character(c_char) :: name(*) 
       character(c_char) :: nclock(*) 
     end subroutine CCTK_TimerPrintData
     subroutine CCTK_TimerPrintDataI (ierr, handle, nclock) bind(c,name="cctk_timerprintdatai_") 
       use iso_c_binding 
       implicit none
       integer(c_int) ::      ierr
       integer(c_int) ::      handle
       character(c_char) :: nclock(*) 
     end subroutine CCTK_TimerPrintDataI
  end interface
end module cctk_Timers
