



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































      SUBROUTINE set_data (ng, tile)
!
!svn $Id: set_data.F 404 2009-10-06 20:18:53Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This subroutine processes forcing, boundary, climatology, and       !
!  assimilation input data. It time-interpolates between snapshots.    !
!                                                                      !
!=======================================================================
!
      USE mod_param
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
!
!  Local variable declarations.
!








      integer :: IminS, ImaxS, JminS, JmaxS
      integer :: LBi, UBi, LBj, UBj, LBij, UBij
!
!  Set horizontal starting and ending indices for automatic private storage
!  arrays.
!
      IminS=BOUNDS(ng)%Istr(tile)-3
      ImaxS=BOUNDS(ng)%Iend(tile)+3
      JminS=BOUNDS(ng)%Jstr(tile)-3
      JmaxS=BOUNDS(ng)%Jend(tile)+3
!
!  Determine array lower and upper bounds in the I- and J-directions.
!
      LBi=BOUNDS(ng)%LBi(tile)
      UBi=BOUNDS(ng)%UBi(tile)
      LBj=BOUNDS(ng)%LBj(tile)
      UBj=BOUNDS(ng)%UBj(tile)
!
!  Set array lower and upper bounds for MIN(I,J)- and MAX(I,J)-directions.
!
      LBij=BOUNDS(ng)%LBij
      UBij=BOUNDS(ng)%UBij
!
      CALL wclock_on (ng, iNLM, 4)
      CALL set_data_tile (ng, tile,                                     &
     &                    LBi, UBi, LBj, UBj,                           &
     &                    IminS, ImaxS, JminS, JmaxS)
      CALL wclock_off (ng, iNLM, 4)
      RETURN
      END SUBROUTINE set_data
!
!***********************************************************************
      SUBROUTINE set_data_tile (ng, tile,                               &
     &                          LBi, UBi, LBj, UBj,                     &
     &                          IminS, ImaxS, JminS, JmaxS)
!***********************************************************************
!
      USE mod_param
      USE mod_boundary
      USE mod_forces
      USE mod_grid
      USE mod_mixing
      USE mod_ncparam
      USE mod_ocean
      USE mod_stepping
      USE mod_scalars
!
      USE analytical_mod
      USE exchange_2d_mod
      USE set_2dfld_mod
      USE set_3dfld_mod
!
      implicit none
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile
      integer, intent(in) :: LBi, UBi, LBj, UBj
      integer, intent(in) :: IminS, ImaxS, JminS, JmaxS
!
!  Local variable declarations.
!
      logical :: update = .FALSE.

      integer :: i, itrc, j, k, order

      real(r8) :: Zr, cff, cff1, cff2

      real(r8), dimension(IminS:ImaxS,JminS:JmaxS) ::  work1
      real(r8), dimension(IminS:ImaxS,JminS:JmaxS) ::  work2












 

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
!  Set cloud fraction (nondimensional).  Notice that clouds are
!  processed first in case that they are used to adjust shortwave
!  radiation.
!-----------------------------------------------------------------------
!
      CALL ana_cloud (ng, tile, iNLM)

!
!-----------------------------------------------------------------------
!  Set surface air temperature (degC).
!-----------------------------------------------------------------------
!
      CALL ana_tair (ng, tile, iNLM)

!
!-----------------------------------------------------------------------
!  Set surface air relative or specific humidity.
!-----------------------------------------------------------------------
!
      CALL ana_humid (ng, tile, iNLM)

!
!-----------------------------------------------------------------------
!  Set kinematic surface solar shortwave radiation flux (degC m/s).
!-----------------------------------------------------------------------
!
      CALL ana_srflux (ng, tile, iNLM)



!
!-----------------------------------------------------------------------
!  Set surface winds (m/s).
!-----------------------------------------------------------------------
!
      CALL ana_winds (ng, tile, iNLM)

!
!-----------------------------------------------------------------------
!  Set rain fall rate (kg/m2/s).
!-----------------------------------------------------------------------
!
      CALL ana_rain (ng, tile, iNLM)


!
!-----------------------------------------------------------------------
!  Set kinematic bottom net heat flux (degC m/s).
!-----------------------------------------------------------------------
!
      CALL ana_btflux (ng, tile, iNLM, itemp)

!
!-----------------------------------------------------------------------
!  Set kinematic surface freshwater (E-P) flux (m/s).
!-----------------------------------------------------------------------
!
      CALL ana_stflux (ng, tile, iNLM, isalt)

!
!-----------------------------------------------------------------------
!  Set kinematic bottom salt flux (m/s).
!-----------------------------------------------------------------------
!
      CALL ana_btflux (ng, tile, iNLM, isalt)



!
!-----------------------------------------------------------------------
!  Set surface air pressure (mb).
!-----------------------------------------------------------------------
!
      CALL ana_pair (ng, tile, iNLM)
















      RETURN
      END SUBROUTINE set_data_tile
