        MODULE ran_state
!
!svn $Id: ran_state.F 294 2009-01-09 21:37:26Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This module supports the random number generation routines. It      !
!  provides each generator with five vectors integers, for use as      !
!  internal random state space.  The  first three integers (iran,      !
!  jran, kran) are maintained as nonnegative  values,  while  the      !
!  last two (mran, nran) have 32-bit nonzero values.  This module      !
!  also includes support for initializing  or  reinitializing the      !
!  state to a desired sequence number, hashing the initial values      !
!  to randon values, and allocating and deallocateing of internal      !
!  workspace.                                                          !
!                                                                      !
!  Adapted from Numerical Recepies.                                    !
!                                                                      !
!  Press, W.H., S.A. Teukolsky, W.T. Vetterling, and B.P. Flannery,    !
!     1996:  Numerical Recipes in Fortran 90,  The Art of Parallel     !
!     Scientific Computing, 2nd Edition, Cambridge Univ. Press.        !
!                                                                      !
!=======================================================================
!
      USE mod_kinds

      CONTAINS
      SUBROUTINE ran_init (length)
      integer(i4b), intent(in) :: length
      print 4, "ran_init"
   4  FORMAT(a, " should not be called by this  benchmark")
      stop
      END SUBROUTINE ran_init
      SUBROUTINE ran_seed (sequence, size, put, get)
      integer, optional, intent(in) :: sequence
      integer, optional, intent(out) :: size
      integer, optional, intent(in) :: put(:)
      integer, optional, intent(out) :: get(:)
      print 4, "ran_seed" 
   4  FORMAT(a, " should not be called by this  benchmark")
      stop
      END SUBROUTINE ran_seed
      END MODULE ran_state
