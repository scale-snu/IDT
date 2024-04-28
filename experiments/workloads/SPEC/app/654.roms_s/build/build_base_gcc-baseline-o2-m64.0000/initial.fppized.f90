



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































      SUBROUTINE initial (ng)
!
!svn $Id: initial.F 376 2009-08-04 22:27:38Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This routine initializes all model variables.                       !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_grid
      USE mod_iounits
      USE mod_ncparam
      USE mod_ocean
      USE mod_scalars
      USE mod_stepping
!
      USE analytical_mod
      USE metrics_mod, ONLY : metrics
      USE set_depth_mod, ONLY : set_depth
      USE omega_mod, ONLY : omega
      USE rho_eos_mod, ONLY : rho_eos
      USE set_massflux_mod, ONLY : set_massflux
      USE stiffness_mod, ONLY : stiffness
!
      implicit none
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng
!
!  Local variable declarations.
!
      logical, save :: First = .TRUE.
      logical :: update = .FALSE.

      integer :: LBi, UBi, LBj, UBj
      integer :: IniRec, Tindex, subs, tile, thread
      integer :: my_numthreads
!
!=======================================================================
!   Initialize model variables.
!=======================================================================
!
      IF (Master) THEN
        WRITE (stdout,20) 'INITIAL: Configuring and initializing ',     &
     &                    'forward nonlinear model ...'
  20    FORMAT (/,1x,a,a,/)
      END IF
!
!-----------------------------------------------------------------------
!  Initialize time stepping indices and counters.
!-----------------------------------------------------------------------
!
      iif(ng)=1
      indx1(ng)=1
      kstp(ng)=1
      krhs(ng)=1
      knew(ng)=1
      PREDICTOR_2D_STEP(ng)=.FALSE.
      synchro_flag(ng)=.TRUE.
      first_time(ng)=0
!
      iic(ng)=0
      nstp(ng)=1
      nrhs(ng)=1
      nnew(ng)=1
      tdays(ng)=dstart
      time(ng)=tdays(ng)*day2sec
      ntstart(ng)=INT((time(ng)-dstart*day2sec)/dt(ng))+1
      ntend(ng)=ntimes(ng)
      ntfirst(ng)=ntstart(ng)
      CALL time_string (time(ng), time_code(ng))

      IniRec=nrrec(ng)
      Tindex=1

      LBi=LBOUND(GRID(ng)%h,DIM=1)
      UBi=UBOUND(GRID(ng)%h,DIM=1)
      LBj=LBOUND(GRID(ng)%h,DIM=2)
      UBj=UBOUND(GRID(ng)%h,DIM=2)

!
!-----------------------------------------------------------------------
!  Start time wall clocks.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread) SHARED(ng,numthreads)
      DO thread=0,numthreads-1
        CALL wclock_on (ng, iNLM, 1)
      END DO
!$OMP END PARALLEL DO

!
!=======================================================================
!  On first pass of ensemble/perturbation/iteration loop, initialize
!  model configuration.
!=======================================================================
!
      IF (Nrun.eq.ERstr) THEN
!
!-----------------------------------------------------------------------
!  Set horizontal grid, bathymetry, and Land/Sea masking (if any).
!  Use analytical functions or read in from a grid NetCDF.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread,subs,tile) SHARED(ng,numthreads)
        DO thread=0,numthreads-1
          subs=NtileX(ng)*NtileE(ng)/numthreads
          DO tile=subs*thread,subs*(thread+1)-1
            CALL ana_grid (ng, tile, iNLM)
          END DO
        END DO
!$OMP END PARALLEL DO

!
!-----------------------------------------------------------------------
!  Set vertical S-coordinate transformation function.
!-----------------------------------------------------------------------
!
        CALL set_scoord (ng)

!
!-----------------------------------------------------------------------
!  Set barotropic time-steps average weighting function.
!-----------------------------------------------------------------------
!
        CALL set_weights (ng)
!
!-----------------------------------------------------------------------
!  Compute various metric term combinations.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread,subs,tile) SHARED(ng,numthreads)
        DO thread=0,numthreads-1
          subs=NtileX(ng)*NtileE(ng)/numthreads
          DO tile=subs*thread,subs*(thread+1)-1
            CALL metrics (ng, tile, iNLM)
          END DO
        END DO
!$OMP END PARALLEL DO


      END IF
!
!=======================================================================
!  Initialize model state variables and forcing.  This part is
!  executed for each ensemble/perturbation/iteration run.
!=======================================================================


!
!-----------------------------------------------------------------------
!  If analytical initial conditions, compute initial time-evolving
!  depths with zero free-surface.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread,subs,tile)                             &
!$OMP&            SHARED(ng,numthreads)
      DO thread=0,numthreads-1
        subs=NtileX(ng)*NtileE(ng)/numthreads
        DO tile=subs*thread,subs*(thread+1)-1
          CALL set_depth (ng, tile)
        END DO
      END DO
!$OMP END PARALLEL DO
!
!-----------------------------------------------------------------------
!  Set primitive variables initial conditions.
!-----------------------------------------------------------------------

!
!  Analytical initial conditions for momentum and active tracers.
!
      IF (nrrec(ng).eq.0) THEN
!$OMP PARALLEL DO PRIVATE(thread,subs,tile) SHARED(ng,numthreads)
        DO thread=0,numthreads-1
          subs=NtileX(ng)*NtileE(ng)/numthreads
          DO tile=subs*thread,subs*(thread+1)-1
            CALL ana_initial (ng, tile, iNLM)
          END DO
        END DO
!$OMP END PARALLEL DO
      END IF




!
!  If restart, read in initial conditions restart NetCDF file.
!
      IF (nrrec(ng).ne.0) THEN
        CALL get_state (ng, 0, 1, INIname(ng), IniRec, Tindex)
        IF (exit_flag.ne.NoError) RETURN
      END IF



!
!-----------------------------------------------------------------------
!  Compute initial time-evolving depths.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread,subs,tile)                             &
!$OMP&            SHARED(ng,numthreads)
      DO thread=0,numthreads-1
        subs=NtileX(ng)*NtileE(ng)/numthreads
        DO tile=subs*thread,subs*(thread+1)-1
          CALL set_depth (ng, tile)
        END DO
      END DO
!$OMP END PARALLEL DO
!
!-----------------------------------------------------------------------
!  Compute initial horizontal mass fluxes, Hz*u/n and Hz*v/m.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread,subs,tile) SHARED(ng,numthreads)
      DO thread=0,numthreads-1
        subs=NtileX(ng)*NtileE(ng)/numthreads
        DO tile=subs*thread,subs*(thread+1)-1
          CALL set_massflux (ng, tile)
        END DO
      END DO
!$OMP END PARALLEL DO
!
!-----------------------------------------------------------------------
!  Compute initial S-coordinates vertical velocity. Compute initial
!  density anomaly from potential temperature and salinity via equation
!  of state for seawater.  Also compute other equation of state related
!  quatities.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread,subs,tile) SHARED(ng,numthreads)
      DO thread=0,numthreads-1
        subs=NtileX(ng)*NtileE(ng)/numthreads
        DO tile=subs*thread,subs*(thread+1)-1
          CALL omega (ng, tile)
          CALL rho_eos (ng, tile)
        END DO
      END DO
!$OMP END PARALLEL DO

!
!-----------------------------------------------------------------------
!  Read in initial forcing, climatology and assimilation data from
!  input NetCDF files.  It loads the first relevant data record for
!  the time-interpolation between snapshots.
!-----------------------------------------------------------------------

!
!  If applicable, read in input data.
!
      CALL get_data (ng)
      IF (exit_flag.ne.NoError) RETURN

!
!-----------------------------------------------------------------------
!  Compute grid stiffness.
!-----------------------------------------------------------------------
!
      IF (Lstiffness) THEN
        Lstiffness=.FALSE.
!$OMP PARALLEL DO PRIVATE(thread,subs,tile) SHARED(ng,numthreads)
        DO thread=0,numthreads-1
          subs=NtileX(ng)*NtileE(ng)/numthreads
          DO tile=subs*thread,subs*(thread+1)-1
            CALL stiffness (ng, tile, iNLM)
          END DO
        END DO
!$OMP END PARALLEL DO
      END IF



!
!-----------------------------------------------------------------------
!  Turn off initiialization time wall clock.
!-----------------------------------------------------------------------
!
!$OMP PARALLEL DO PRIVATE(thread) SHARED(ng,numthreads)
      DO thread=0,numthreads-1
        CALL wclock_off (ng, iNLM, 1)
      END DO
!$OMP END PARALLEL DO
      RETURN
      END SUBROUTINE initial
