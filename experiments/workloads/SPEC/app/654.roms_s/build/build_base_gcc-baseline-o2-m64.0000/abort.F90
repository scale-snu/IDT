#include "cppdefs.h"
#ifndef SPEC
      SUBROUTINE abort (status)
#else
! silence a warning about conflict with library routine
      SUBROUTINE my_abort (status)
#endif
!
!svn $Id: abort.F 294 2009-01-09 21:37:26Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
! This subroutine terminates execution after flushing all buffers and  !
! closing IO files.                                                    !
!                                                                      !
!=======================================================================
!
#ifdef ESMF_LIB
      USE ESMF_Mod
#endif
      USE ocean_control_mod, ONLY : ROMS_finalize
!
      implicit none
!
!  Imported variable declarations.
!
      integer, intent(in) :: status
!
!-----------------------------------------------------------------------
!  Terminate execution due to fatal error.
!-----------------------------------------------------------------------
!
!  Finalize ROMS component.
!
      CALL ROMS_finalize

#ifdef ESMF_LIB
!
!  Finalize ESMF and flush buffers.
!
      CALL ESMF_Finalize (terminationflag=ESMF_ABORT)
#endif
!
!  Stop execution.
!
      STOP

#ifndef SPEC
      END SUBROUTINE abort   
#else
      END SUBROUTINE my_abort   
#endif
