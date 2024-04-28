



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































      MODULE exchange_3d_mod
!
!svn $Id: exchange_3d.F 294 2009-01-09 21:37:26Z arango $
!=======================================================================
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                           Hernan G. Arango   !
!========================================== Alexander F. Shchepetkin ===
!                                                                      !
!  This package contains periodic boundary conditions and parallel     !
!  exchage (distributed-memory only) routines for 3D variables.        !
!                                                                      !
!  Routines:                                                           !
!                                                                      !
!    exchange_p3d_tile    periodic conditions/exchange at PSI-points   !
!    exchange_r3d_tile    periodic conditions/exchange at RHO-points   !
!    exchange_u3d_tile    periodic conditions/exchange at U-points     !
!    exchange_v3d_tile    periodic conditions/exchange at V-points     !
!    exchange_w3d_tile    periodic conditions/exchange at W-points     !
!                                                                      !
!=======================================================================
!
      implicit none

      CONTAINS
!
!***********************************************************************
      SUBROUTINE exchange_p3d_tile (ng, tile,                           &
     &                              LBi, UBi, LBj, UBj, LBk, UBk,       &
     &                              A)
!***********************************************************************
!
      USE mod_param
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
      integer, intent(in) :: LBi, UBi, LBj, UBj, LBk, UBk
!
      real(r8), intent(inout) :: A(LBi:,LBj:,LBk:)
!
!  Local variable declarations.
!
      integer :: i, j, k












 

!
!-----------------------------------------------------------------------
!  Set lower and upper tile bounds and staggered variables bounds for
!  this horizontal domain partition.  Notice that if tile=-1, it will
!  set the values for the global grid.
!-----------------------------------------------------------------------
!
      integer :: Istr, IstrR, IstrT, IstrU, Iend, IendR, IendT
      integer :: Jstr, JstrR, JstrT, JstrV, Jend, JendR, JendT
!
      Istr =BOUNDS(ng)%Istr (tile)
      IstrR=BOUNDS(ng)%IstrR(tile)
      IstrT=BOUNDS(ng)%IstrT(tile)
      IstrU=BOUNDS(ng)%IstrU(tile)
      Iend =BOUNDS(ng)%Iend (tile)
      IendR=BOUNDS(ng)%IendR(tile)
      IendT=BOUNDS(ng)%IendT(tile)

      Jstr =BOUNDS(ng)%Jstr (tile)
      JstrR=BOUNDS(ng)%JstrR(tile)
      JstrT=BOUNDS(ng)%JstrT(tile)
      JstrV=BOUNDS(ng)%JstrV(tile)
      Jend =BOUNDS(ng)%Jend (tile)
      JendR=BOUNDS(ng)%JendR(tile)
      JendT=BOUNDS(ng)%JendT(tile)

!
!-----------------------------------------------------------------------
!  East-West periodic boundary conditions.
!-----------------------------------------------------------------------
!
        IF (Istr.eq.1) THEN
          DO k=LBk,UBk
            DO j=Jstr,JendR
              A(Lm(ng)+1,j,k)=A(1,j,k)
              A(Lm(ng)+2,j,k)=A(2,j,k)
            END DO
          END DO
        END IF
        IF (Iend.eq.Lm(ng)) THEN
          DO k=LBk,UBk
            DO j=Jstr,JendR
              A(-2,j,k)=A(Lm(ng)-2,j,k)
              A(-1,j,k)=A(Lm(ng)-1,j,k)
              A( 0,j,k)=A(Lm(ng)  ,j,k)
            END DO
          END DO
        END IF


      RETURN
      END SUBROUTINE exchange_p3d_tile

!
!***********************************************************************
      SUBROUTINE exchange_r3d_tile (ng, tile,                           &
     &                              LBi, UBi, LBj, UBj, LBk, UBk,       &
     &                              A)
!***********************************************************************
!
      USE mod_param
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
      integer, intent(in) :: LBi, UBi, LBj, UBj, LBk, UBk
!
      real(r8), intent(inout) :: A(LBi:,LBj:,LBk:)
!
!  Local variable declarations.
!
      integer :: i, j, k












 

!
!-----------------------------------------------------------------------
!  Set lower and upper tile bounds and staggered variables bounds for
!  this horizontal domain partition.  Notice that if tile=-1, it will
!  set the values for the global grid.
!-----------------------------------------------------------------------
!
      integer :: Istr, IstrR, IstrT, IstrU, Iend, IendR, IendT
      integer :: Jstr, JstrR, JstrT, JstrV, Jend, JendR, JendT
!
      Istr =BOUNDS(ng)%Istr (tile)
      IstrR=BOUNDS(ng)%IstrR(tile)
      IstrT=BOUNDS(ng)%IstrT(tile)
      IstrU=BOUNDS(ng)%IstrU(tile)
      Iend =BOUNDS(ng)%Iend (tile)
      IendR=BOUNDS(ng)%IendR(tile)
      IendT=BOUNDS(ng)%IendT(tile)

      Jstr =BOUNDS(ng)%Jstr (tile)
      JstrR=BOUNDS(ng)%JstrR(tile)
      JstrT=BOUNDS(ng)%JstrT(tile)
      JstrV=BOUNDS(ng)%JstrV(tile)
      Jend =BOUNDS(ng)%Jend (tile)
      JendR=BOUNDS(ng)%JendR(tile)
      JendT=BOUNDS(ng)%JendT(tile)

!
!-----------------------------------------------------------------------
!  East-West periodic boundary conditions.
!-----------------------------------------------------------------------
!
        IF (Istr.eq.1) THEN
          DO k=LBk,UBk
            DO j=JstrR,JendR
              A(Lm(ng)+1,j,k)=A(1,j,k)
              A(Lm(ng)+2,j,k)=A(2,j,k)
            END DO
          END DO
        END IF
        IF (Iend.eq.Lm(ng)) THEN
          DO k=LBk,UBk
            DO j=JstrR,JendR
              A(-2,j,k)=A(Lm(ng)-2,j,k)
              A(-1,j,k)=A(Lm(ng)-1,j,k)
              A( 0,j,k)=A(Lm(ng)  ,j,k)
            END DO
          END DO
        END IF

      RETURN
      END SUBROUTINE exchange_r3d_tile

!
!***********************************************************************
      SUBROUTINE exchange_u3d_tile (ng, tile,                           &
     &                              LBi, UBi, LBj, UBj, LBk, UBk,       &
     &                              A)
!***********************************************************************
!
      USE mod_param
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
      integer, intent(in) :: LBi, UBi, LBj, UBj, LBk, UBk
!
      real(r8), intent(inout) :: A(LBi:,LBj:,LBk:)
!
!  Local variable declarations.
!
      integer :: i, j, k












 

!
!-----------------------------------------------------------------------
!  Set lower and upper tile bounds and staggered variables bounds for
!  this horizontal domain partition.  Notice that if tile=-1, it will
!  set the values for the global grid.
!-----------------------------------------------------------------------
!
      integer :: Istr, IstrR, IstrT, IstrU, Iend, IendR, IendT
      integer :: Jstr, JstrR, JstrT, JstrV, Jend, JendR, JendT
!
      Istr =BOUNDS(ng)%Istr (tile)
      IstrR=BOUNDS(ng)%IstrR(tile)
      IstrT=BOUNDS(ng)%IstrT(tile)
      IstrU=BOUNDS(ng)%IstrU(tile)
      Iend =BOUNDS(ng)%Iend (tile)
      IendR=BOUNDS(ng)%IendR(tile)
      IendT=BOUNDS(ng)%IendT(tile)

      Jstr =BOUNDS(ng)%Jstr (tile)
      JstrR=BOUNDS(ng)%JstrR(tile)
      JstrT=BOUNDS(ng)%JstrT(tile)
      JstrV=BOUNDS(ng)%JstrV(tile)
      Jend =BOUNDS(ng)%Jend (tile)
      JendR=BOUNDS(ng)%JendR(tile)
      JendT=BOUNDS(ng)%JendT(tile)

!
!-----------------------------------------------------------------------
!  East-West periodic boundary conditions.
!-----------------------------------------------------------------------
!
        IF (Istr.eq.1) THEN
          DO k=LBk,UBk
            DO j=JstrR,JendR
              A(Lm(ng)+1,j,k)=A(1,j,k)
              A(Lm(ng)+2,j,k)=A(2,j,k)
            END DO
          END DO
        END IF
        IF (Iend.eq.Lm(ng)) THEN
          DO k=LBk,UBk
            DO j=JstrR,JendR
              A(-2,j,k)=A(Lm(ng)-2,j,k)
              A(-1,j,k)=A(Lm(ng)-1,j,k)
              A( 0,j,k)=A(Lm(ng)  ,j,k)
            END DO
          END DO
        END IF


      RETURN
      END SUBROUTINE exchange_u3d_tile

!
!***********************************************************************
      SUBROUTINE exchange_v3d_tile (ng, tile,                           &
     &                              LBi, UBi, LBj, UBj, LBk, UBk,       &
     &                              A)
!***********************************************************************
!
      USE mod_param
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
      integer, intent(in) :: LBi, UBi, LBj, UBj, LBk, UBk
!
      real(r8), intent(inout) :: A(LBi:,LBj:,LBk:)
!
!  Local variable declarations.
!
      integer :: i, j, k












 

!
!-----------------------------------------------------------------------
!  Set lower and upper tile bounds and staggered variables bounds for
!  this horizontal domain partition.  Notice that if tile=-1, it will
!  set the values for the global grid.
!-----------------------------------------------------------------------
!
      integer :: Istr, IstrR, IstrT, IstrU, Iend, IendR, IendT
      integer :: Jstr, JstrR, JstrT, JstrV, Jend, JendR, JendT
!
      Istr =BOUNDS(ng)%Istr (tile)
      IstrR=BOUNDS(ng)%IstrR(tile)
      IstrT=BOUNDS(ng)%IstrT(tile)
      IstrU=BOUNDS(ng)%IstrU(tile)
      Iend =BOUNDS(ng)%Iend (tile)
      IendR=BOUNDS(ng)%IendR(tile)
      IendT=BOUNDS(ng)%IendT(tile)

      Jstr =BOUNDS(ng)%Jstr (tile)
      JstrR=BOUNDS(ng)%JstrR(tile)
      JstrT=BOUNDS(ng)%JstrT(tile)
      JstrV=BOUNDS(ng)%JstrV(tile)
      Jend =BOUNDS(ng)%Jend (tile)
      JendR=BOUNDS(ng)%JendR(tile)
      JendT=BOUNDS(ng)%JendT(tile)

!
!-----------------------------------------------------------------------
!  East-West periodic boundary conditions.
!-----------------------------------------------------------------------
!
        IF (Istr.eq.1) THEN
          DO k=LBk,UBk
            DO j=Jstr,JendR
              A(Lm(ng)+1,j,k)=A(1,j,k)
              A(Lm(ng)+2,j,k)=A(2,j,k)
            END DO
          END DO
        END IF
        IF (Iend.eq.Lm(ng)) THEN
          DO k=LBk,UBk
            DO j=Jstr,JendR
              A(-2,j,k)=A(Lm(ng)-2,j,k)
              A(-1,j,k)=A(Lm(ng)-1,j,k)
              A( 0,j,k)=A(Lm(ng)  ,j,k)
            END DO
          END DO
        END IF


      RETURN
      END SUBROUTINE exchange_v3d_tile

!
!***********************************************************************
      SUBROUTINE exchange_w3d_tile (ng, tile,                           &
     &                              LBi, UBi, LBj, UBj, LBk, UBk,       &
     &                              A)
!***********************************************************************
!
      USE mod_param
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
      integer, intent(in) :: LBi, UBi, LBj, UBj, LBk, UBk
!
      real(r8), intent(inout) :: A(LBi:,LBj:,LBk:)
!
!  Local variable declarations.
!
      integer :: i, j, k












 

!
!-----------------------------------------------------------------------
!  Set lower and upper tile bounds and staggered variables bounds for
!  this horizontal domain partition.  Notice that if tile=-1, it will
!  set the values for the global grid.
!-----------------------------------------------------------------------
!
      integer :: Istr, IstrR, IstrT, IstrU, Iend, IendR, IendT
      integer :: Jstr, JstrR, JstrT, JstrV, Jend, JendR, JendT
!
      Istr =BOUNDS(ng)%Istr (tile)
      IstrR=BOUNDS(ng)%IstrR(tile)
      IstrT=BOUNDS(ng)%IstrT(tile)
      IstrU=BOUNDS(ng)%IstrU(tile)
      Iend =BOUNDS(ng)%Iend (tile)
      IendR=BOUNDS(ng)%IendR(tile)
      IendT=BOUNDS(ng)%IendT(tile)

      Jstr =BOUNDS(ng)%Jstr (tile)
      JstrR=BOUNDS(ng)%JstrR(tile)
      JstrT=BOUNDS(ng)%JstrT(tile)
      JstrV=BOUNDS(ng)%JstrV(tile)
      Jend =BOUNDS(ng)%Jend (tile)
      JendR=BOUNDS(ng)%JendR(tile)
      JendT=BOUNDS(ng)%JendT(tile)

!
!-----------------------------------------------------------------------
!  East-West periodic boundary conditions.
!-----------------------------------------------------------------------
!
        IF (Istr.eq.1) THEN
          DO k=LBk,UBk
            DO j=JstrR,JendR
              A(Lm(ng)+1,j,k)=A(1,j,k)
              A(Lm(ng)+2,j,k)=A(2,j,k)
            END DO
          END DO
        END IF
        IF (Iend.eq.Lm(ng)) THEN
          DO k=LBk,UBk
            DO j=JstrR,JendR
              A(-2,j,k)=A(Lm(ng)-2,j,k)
              A(-1,j,k)=A(Lm(ng)-1,j,k)
              A( 0,j,k)=A(Lm(ng)  ,j,k)
            END DO
          END DO
        END IF

      RETURN
      END SUBROUTINE exchange_w3d_tile

      END MODULE exchange_3d_mod
