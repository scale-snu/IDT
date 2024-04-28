



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































      SUBROUTINE checkdefs
!
!svn $Id: checkdefs.F 404 2009-10-06 20:18:53Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This subroutine checks activated C-preprocessing options for        !
!  consistency.                                                        !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_scalars
      USE mod_strings
!
      implicit none
!
!  Local variable declarations.
!
      integer :: FS_east = 0
      integer :: FS_west = 0
      integer :: FS_north = 0
      integer :: FS_south = 0
      integer :: M2_east = 0
      integer :: M2_west = 0
      integer :: M2_north = 0
      integer :: M2_south = 0
      integer :: M3_east = 0
      integer :: M3_west = 0
      integer :: M3_north = 0
      integer :: M3_south = 0
      integer :: K_east = 0
      integer :: K_west = 0
      integer :: K_north = 0
      integer :: K_south = 0
      integer :: T_east = 0
      integer :: T_west = 0
      integer :: T_north = 0
      integer :: T_south = 0
      integer :: ibbl = 0
      integer :: ibiology = 0
      integer :: idriver = 0
      integer :: itrcHadv = 0
      integer :: itrcVadv = 0
      integer :: itrcHadvtl = 0
      integer :: itrcVadvtl = 0
      integer :: ivelHadv = 0
      integer :: ivelVadv = 0
      integer :: ivmix = 0

      integer :: is, lstr, ng
!
!-----------------------------------------------------------------------
!  Report activated C-preprocessing options.
!-----------------------------------------------------------------------
!
      Coptions=' '
      IF (Master) WRITE (stdout,10)
  10  FORMAT (/,' Activated C-preprocessing Options:',/)
  20  FORMAT (1x,a,t22,a)
!
      IF (Master) THEN
        WRITE (stdout,20) TRIM(ADJUSTL(MyAppCPP)), TRIM(ADJUSTL(title))
      END IF
      is=LEN_TRIM(Coptions)+1
      lstr=LEN_TRIM(MyAppCPP)
      Coptions(is:is+lstr)=TRIM(ADJUSTL(MyAppCPP))
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is)=','
!
      IF (Master) WRITE (stdout,20) 'ALBEDO',                           &
     &   'Shortwave radiation from albedo equation.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+8)=' ALBEDO,'
      IF (Master) WRITE (stdout,20) 'ANA_BSFLUX',                       &
     &   'Analytical kinematic bottom salinity flux.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' ANA_BSFLUX,'
      IF (Master) WRITE (stdout,20) 'ANA_BTFLUX',                       &
     &   'Analytical kinematic bottom temperature flux.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' ANA_BTFLUX,'
      IF (Master) WRITE (stdout,20) 'ANA_CLOUD',                        &
     &   'Analytical cloud fraction.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+11)=' ANA_CLOUD,'
      IF (Master) WRITE (stdout,20) 'ANA_GRID',                         &
     &   'Analytical grid set-up.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' ANA_GRID,'
      IF (Master) WRITE (stdout,20) 'ANA_HUMIDITY',                     &
     &   'Analytical surface air humidity.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+14)=' ANA_HUMIDITY,'
      IF (Master) WRITE (stdout,20) 'ANA_INITIAL',                      &
     &   'Analytical initial conditions.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+13)=' ANA_INITIAL,'
      IF (Master) WRITE (stdout,20) 'ANA_PAIR',                         &
     &   'Analytical surface air pressure.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' ANA_PAIR,'
      IF (Master) WRITE (stdout,20) 'ANA_RAIN',                         &
     &   'Analytical rain fall rate.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' ANA_RAIN,'
      IF (Master) WRITE (stdout,20) 'ANA_SRFLUX',                       &
     &   'Analytical kinematic shortwave radiation flux.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' ANA_SRFLUX,'
      IF (Master) WRITE (stdout,20) 'ANA_SSFLUX',                       &
     &   'Analytical kinematic surface salinity flux.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' ANA_SSFLUX,'
      IF (Master) WRITE (stdout,20) 'ANA_WINDS',                        &
     &   'Analytical surface wind components.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+11)=' ANA_WINDS,'
      IF (Master) WRITE (stdout,20) 'ASSUMED_SHAPE',                    &
     &   'Using assumed-shape arrays.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+15)=' ASSUMED_SHAPE,'
      IF (Master) WRITE (stdout,20) 'BULK_FLUXES',                      &
     &   'Surface bulk fluxes parametererization.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+13)=' BULK_FLUXES,'
      IF (Master) WRITE (stdout,20) 'CURVGRID',                         &
     &   'Orthogonal curvilinear grid.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' CURVGRID,'
      IF (Master) WRITE (stdout,20) 'DJ_GRADPS',                        &
     &   'Parabolic Splines density Jacobian (Shchepetkin, 2002).'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+11)=' DJ_GRADPS,'
      IF (Master) WRITE (stdout,20) 'DOUBLE_PRECISION',                 &
     &   'Double precision arithmetic.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+18)=' DOUBLE_PRECISION,'
      IF (Master) WRITE (stdout,20) 'EW_PERIODIC',                      &
     &   'East-West periodic boundaries.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+13)=' EW_PERIODIC,'
      FS_east=FS_east+1
      FS_west=FS_west+1
      M2_east=M2_east+1
      M2_west=M2_west+1
      M3_east=M3_east+1
      M3_west=M3_west+1
      K_east=K_east+1
      K_west=K_west+1
      T_east=T_east+1
      T_west=T_west+1
      IF (Master) WRITE (stdout,20) 'LMD_CONVEC',                       &
     &   'LMD convective mixing due to shear instability.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' LMD_CONVEC,'
      IF (Master) WRITE (stdout,20) 'LMD_MIXING',                       &
     &   'Large/McWilliams/Doney interior mixing.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' LMD_MIXING,'
      ivmix=ivmix+1
      IF (Master) WRITE (stdout,20) 'LMD_NONLOCAL',                     &
     &   'LMD convective nonlocal transport.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+14)=' LMD_NONLOCAL,'
      IF (Master) WRITE (stdout,20) 'LMD_RIMIX',                        &
     &   'LMD diffusivity due to shear instability.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+11)=' LMD_RIMIX,'
      IF (Master) WRITE (stdout,20) 'LMD_SKPP',                         &
     &   'KPP surface boundary layer mixing.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' LMD_SKPP,'
      IF (Master) WRITE (stdout,20) 'LONGWAVE',                         &
     &   'Compute net longwave radiation internally.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' LONGWAVE,'
      IF (Master) WRITE (stdout,20) 'MIX_GEO_TS',                       &
     &   'Mixing of tracers along geopotential surfaces.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' MIX_GEO_TS,'
      IF (Master) WRITE (stdout,20) 'MIX_S_UV',                         &
     &   'Mixing of momentum along constant S-surfaces.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' MIX_S_UV,'
      IF (Master) WRITE (stdout,20) 'NONLINEAR',                        &
     &   'Nonlinear Model.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' NONLINEAR,'
      IF (Master) WRITE (stdout,20) 'NONLIN_EOS',                       &
     &   'Nonlinear Equation of State for seawater.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' NONLIN_EOS,'
      IF (Master) WRITE (stdout,20) 'NORTHERN_WALL',                    &
     &   'Wall boundary at Northern edge.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+15)=' NORTHERN_WALL,'
      IF (Master) WRITE (stdout,20) 'POWER_LAW',                        &
     &   'Power-law shape time-averaging barotropic filter.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+11)=' POWER_LAW,'
      IF (Master) WRITE (stdout,20) 'PROFILE',                          &
     &   'Time profiling activated .'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+9)=' PROFILE,'
      IF (Master) WRITE (stdout,20) '!RST_SINGLE',                      &
     &   'Double precision fields in restart NetCDF file.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+13)=' !RST_SINGLE,'
      IF (Master) WRITE (stdout,20) 'SALINITY',                         &
     &   'Using salinity.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' SALINITY,'
      IF (Master) WRITE (stdout,20) 'SOLAR_SOURCE',                     &
     &   'Solar Radiation Source Term.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+14)=' SOLAR_SOURCE,'
      IF (Master) WRITE (stdout,20) 'SOLVE3D',                          &
     &   'Solving 3D Primitive Equations.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+9)=' SOLVE3D,'
      IF (Master) WRITE (stdout,20) 'SOUTHERN_WALL',                    &
     &   'Wall boundary at Southern edge.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+15)=' SOUTHERN_WALL,'
      IF (Master) WRITE (stdout,20) 'SPLINES',                          &
     &   'Conservative parabolic spline reconstruction.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+9)=' SPLINES,'
      IF (Master) WRITE (stdout,20) 'SPHERICAL',                        &
     &   'Spherical grid configuration.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+11)=' SPHERICAL,'
      IF (Master) WRITE (stdout,20) 'TS_U3HADVECTION',                  &
     &   'Third-order upstream horizontal advection of tracers.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+17)=' TS_U3HADVECTION,'
      itrcHadv=itrcHadv+1
      IF (Master) WRITE (stdout,20) 'TS_C4VADVECTION',                  &
     &   'Fourth-order centered vertical advection of tracers.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+17)=' TS_C4VADVECTION,'
      itrcVadv=itrcVadv+1
      IF (Master) WRITE (stdout,20) 'TS_DIF2',                          &
     &   'Harmonic mixing of tracers.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+9)=' TS_DIF2,'
      IF (Master) WRITE (stdout,20) 'UV_ADV',                           &
     &   'Advection of momentum.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+8)=' UV_ADV,'
      IF (Master) WRITE (stdout,20) 'UV_COR',                           &
     &   'Coriolis term.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+8)=' UV_COR,'
      IF (Master) WRITE (stdout,20) 'UV_U3HADVECTION',                  &
     &   'Third-order upstream horizontal advection of 3D momentum.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+17)=' UV_U3HADVECTION,'
      ivelHadv=ivelHadv+1
      IF (Master) WRITE (stdout,20) 'UV_C4VADVECTION',                  &
     &   'Fourth-order centered vertical advection of momentum.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+16)=' UV_C4VADVECTION,'
      ivelVadv=ivelVadv+1
      IF (Master) WRITE (stdout,20) 'UV_QDRAG',                         &
     &   'Quadratic bottom stress.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+10)=' UV_QDRAG,'
      ibbl=ibbl+1
      IF (Master) WRITE (stdout,20) 'UV_VIS2',                          &
     &   'Harmonic mixing of momentum.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+9)=' UV_VIS2,'
      IF (Master) WRITE (stdout,20) 'VAR_RHO_2D',                       &
     &   'Variable density barotropic mode.'
      is=LEN_TRIM(Coptions)+1
      Coptions(is:is+12)=' VAR_RHO_2D,'
!
!-----------------------------------------------------------------------
!  Stop if unsupported C-preprocessing options or report issues with
!  particular options.
!-----------------------------------------------------------------------
!
      CALL checkadj
!
!-----------------------------------------------------------------------
!  Check C-preprocessing options.
!-----------------------------------------------------------------------
!
!  Stop if more than one vertical closure scheme is selected.
!
      IF (Master.and.(ivmix.gt.1)) THEN
        WRITE (stdout,30)
  30    FORMAT (/,' CHECKDEFS - only one vertical closure scheme',      &
     &            ' is allowed.')
        exit_flag=5
      END IF
!
!  Stop if more that one bottom stress formulation is selected.
!
      IF (Master.and.(ibbl.gt.1)) THEN
        WRITE (stdout,40)
  40    FORMAT (/,' CHECKDEFS - only one bottom stress formulation is', &
     &            ' allowed.')
        exit_flag=5
      END IF
!
!  Stop if no bottom stress formulation is selected.
!
      IF (Master.and.(ibbl.eq.0)) THEN
        WRITE (stdout,50)
  50    FORMAT (/,' CHECKDEFS - no bottom stress formulation is',       &
     &            ' selected.')
        exit_flag=5
      END IF
!
!  Stop if more than one biological module is selected.
!
      IF (Master.and.(ibiology.gt.1)) THEN
        WRITE (stdout,60)
  60    FORMAT (/,' CHECKDEFS - only one biology MODULE is allowed.')
        exit_flag=5
      END IF
!
!  Stop if more that one model driver is selected.
!
      IF (Master.and.(idriver.gt.1)) THEN
        WRITE (stdout,70)
  70    FORMAT (/,' CHECKDEFS - only one model example is allowed.')
        exit_flag=5
      END IF
!
!  Stop if more than one boundary conditions option for free-surface
!  is selected at each boundary edge.
!
      IF (Master.and.(FS_east.gt.1)) THEN
        WRITE (stdout,80) 'free-surface','eastern edge'
        exit_flag=5
      END IF
      IF (Master.and.(FS_west.gt.1)) THEN
        WRITE (stdout,80) 'free-surface','western edge'
        exit_flag=5
      END IF
      IF (Master.and.(FS_north.gt.1)) THEN
        WRITE (stdout,80) 'free-surface','northern edge'
        exit_flag=5
      END IF
      IF (Master.and.(FS_south.gt.1)) THEN
        WRITE (stdout,80) 'free-surface','southern edge'
        exit_flag=5
      END IF
  80  FORMAT (/,' CHECKDEFS - only one boundary condition option for',  &
     &        1x,a,/,13x,'is allowed at the',1x,a,'.')
!
!  Stop if more than one boundary conditions option for 2D momentum
!  is selected at each boundary edge.
!
      IF (Master.and.(M2_east.gt.1)) THEN
        WRITE (stdout,80) '2D momentum','eastern edge'
        exit_flag=5
      END IF
      IF (Master.and.(M2_west.gt.1)) THEN
        WRITE (stdout,80) '2D momentum','western edge'
        exit_flag=5
      END IF
      IF (Master.and.(M2_north.gt.1)) THEN
        WRITE (stdout,80) '2D momentum','northern edge'
        exit_flag=5
      END IF
      IF (Master.and.(M2_south.gt.1)) THEN
        WRITE (stdout,80) '2D momentum','southern edge'
        exit_flag=5
      END IF
!
!  Stop if more than one boundary conditions option for 3D momentum
!  is selected at each boundary edge.
!
      IF (Master.and.(M3_east.gt.1)) THEN
        WRITE (stdout,80) '3D momentum','eastern edge'
        exit_flag=5
      END IF
      IF (Master.and.(M3_west.gt.1)) THEN
        WRITE (stdout,80) '3D momentum','western edge'
        exit_flag=5
      END IF
      IF (Master.and.(M3_north.gt.1)) THEN
        WRITE (stdout,80) '3D momentum','northern edge'
        exit_flag=5
      END IF
      IF (Master.and.(M3_south.gt.1)) THEN
        WRITE (stdout,80) '3D momentum','southern edge'
        exit_flag=5
      END IF
!
!  Stop if more than one boundary conditions option for tracers is
!  selected at each boundary edge.
!
      IF (Master.and.(T_east.gt.1)) THEN
        WRITE (stdout,80) 'tracers','eastern edge'
        exit_flag=5
      END IF
      IF (Master.and.(T_west.gt.1)) THEN
        WRITE (stdout,80) 'tracers','western edge'
        exit_flag=5
      END IF
      IF (Master.and.(T_north.gt.1)) THEN
        WRITE (stdout,80) 'tracers','northern edge'
        exit_flag=5
      END IF
      IF (Master.and.(T_south.gt.1)) THEN
        WRITE (stdout,80) 'tracers','southern edge'
        exit_flag=5
      END IF
!
!  Stop if more than one advection scheme has been activated.
!
      IF (Master.and.(ivelHadv.gt.1)) THEN
        WRITE (stdout,150) 'horizontal','momentum','ivelHadv =',ivelHadv
        exit_flag=5
      END IF
      IF (Master.and.(ivelVadv.gt.1)) THEN
        WRITE (stdout,150) 'vertical','momentum','ivelVadv =',ivelVadv
        exit_flag=5
      END IF
      IF (Master.and.(itrcHadv.gt.1)) THEN
        WRITE (stdout,150) 'horizontal','tracers','itrcHadv =',itrcHadv
        exit_flag=5
      END IF
      IF (Master.and.(itrcVadv.gt.1)) THEN
        WRITE (stdout,150) 'vertical','tracers','itrcVadv =',itrcVadv
        exit_flag=5
      END IF
 150  FORMAT (/,' CHECKDEFS - only one ',a,' advection scheme',         &
     &        /,13x,'is allowed for ',a,', ',a,1x,i1)

      RETURN
      END SUBROUTINE checkdefs
