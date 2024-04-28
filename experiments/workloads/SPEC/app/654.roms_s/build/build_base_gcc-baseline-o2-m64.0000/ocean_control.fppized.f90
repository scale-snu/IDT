



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































!!
!!svn $Id: ocean_control.F 334 2009-03-24 22:38:49Z arango $
!!================================================= Hernan G. Arango ===
!! Copyright (c) 2002-2009 The ROMS/TOMS Group       Andrew M. Moore   !
!!   Licensed under a MIT/X style license                              !
!!   See License_ROMS.txt                                              !
!!====================================================================== 
!!                                                                     !
!!  Regional Ocean Model System (ROMS) Drivers:                        !
!!                                                                     !
!!  ad_ocean.h             Adjoint model driver                        !
!!  adsen_ocean.h          Adjoint sensitivity analysis driver         !
!!  afte_ocean.h           Adjoint finite time eigenmodes driver       !
!!  convolution.h          Error covariance convolution driver         !
!!  correlation.h          Error covariance correlation driver         !
!!  fte_ocean.h            Finite time eigenmodes driver               !
!!  fsv_ocean.h            Forcing singular vectors driver             !
!!  grad_ocean.h           Tangent linear and adjoint models gradient  !
!!                           test driver                               !
!!  is4dvar_ocean.h        Strong constraint, incremental 4DVar data   !
!!                           assimilation driver                       !
!!  nl_ocean.h             Nonlinear model driver (default)            !
!!  op_ocean.h             Optimal perturbations driver                !
!!  optobs_ocean.h         Optimal observations driver                 !
!!  obs_sen_ocean.h        Observations sensitivity driver to the      !
!!                           IS4DVAR data assimilation system          !
!!  obs_sen_w4dpsas.h      Observations sensitivity driver to the      !
!!                           W4DPSAS data assimilation system          !
!!  obs_sen_w4dvar.h       Observations sensitivity driver to the      !
!!                           W4DVAR  data assimilation system          !
!!  rp_ocean.h             Representer tangent linear model driver     !
!!  so_semi_ocean.h        Stochastic optimals, semi-norm driver       !
!!  symmetry.h             Representer matrix, symmetry driver         !
!!  pert_ocean.h           Tangent linear and adjoint models sanity    !
!!                           test driver                               !
!!  picard_ocean.h         Picard test for representers tangent linear !
!!                           model driver                              !
!!  tlcheck_ocean.h        Tangent linear model linearization test     !
!!                           driver                                    !
!!  tl_ocean.h             Tangent linear model driver                 !
!!  tl_w4dpsas_ocean.h     Tangent linear driver to the W4DPSAS        !
!!                           data assimilation system                  !
!!  tl_w4dvar_ocean.h      Tangent linear driver to the W4DVAR         !
!!                           data assimilation system                  !
!!  w4dpsas_ocean.h        Weak constraint 4D-PSAS assimilation driver !
!!  w4dvar_ocean.h         Weak constraint 4DVAR assimilation,         !
!!                           indirect representer method               !
!!                                                                     !
!!======================================================================
!!
      MODULE ocean_control_mod
!
!svn $Id: nl_ocean.h 294 2009-01-09 21:37:26Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  ROMS/TOMS Nonlinear model:                                          !
!                                                                      !
!  This driver executes ROMS/TOMS standard nonlinear model.  It        !
!  controls the initialization, time-stepping, and finalization        !
!  of the nonlinear model execution following ESMF conventions:        !
!                                                                      !
!     ROMS_initialize                                                  !
!     ROMS_run                                                         !
!     ROMS_finalize                                                    !
!                                                                      !
!=======================================================================
!
      implicit none

      PRIVATE
      PUBLIC  :: ROMS_initialize
      PUBLIC  :: ROMS_run
      PUBLIC  :: ROMS_finalize

      CONTAINS

      SUBROUTINE ROMS_initialize (first, MyCOMM)
!
!=======================================================================
!                                                                      !
!  This routine allocates and initializes ROMS/TOMS state variables    !
!  and internal and external parameters.                               !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_scalars
!
!
!  Imported variable declarations.
!
      logical, intent(inout) :: first

      integer, intent(in), optional :: MyCOMM
!
!  Local variable declarations.
!
      logical :: allocate_vars = .TRUE.

      integer :: ng, thread

!
!-----------------------------------------------------------------------
!  On first pass, initialize model parameters a variables for all
!  nested/composed grids.  Notice that the logical switch "first"
!  is used to allow multiple calls to this routine during ensemble
!  configurations.
!-----------------------------------------------------------------------
!
      IF (first) THEN
        first=.FALSE.
!
!  Initialize parallel parameters.
!
        CALL initialize_parallel
!
!  Initialize wall clocks.
!
        IF (Master) THEN
          WRITE (stdout,10)
        END IF
        DO ng=1,Ngrids
!$OMP PARALLEL DO PRIVATE(thread) SHARED(ng,numthreads)
          DO thread=0,numthreads-1
            CALL wclock_on (ng, iNLM, 0)
          END DO
!$OMP END PARALLEL DO
        END DO
!
!  Read in model tunable parameters from standard input. Initialize
!  "mod_param", "mod_ncparam" and "mod_scalar" modules.
!
        CALL inp_par (iNLM)
        IF (exit_flag.ne.NoError) RETURN
!
!  Allocate and initialize modules variables.
!
        CALL mod_arrays (allocate_vars)

      END IF
!
!-----------------------------------------------------------------------
!  Initialize model state variables for all nested/composed grids.
!-----------------------------------------------------------------------
!
      DO ng=1,Ngrids
        CALL initial (ng)
        IF (exit_flag.ne.NoError) RETURN
      END DO
!
!  Initialize run or ensemble counter.
!
      Nrun=1


!
!  Substract a time-step to model time after initialization because the
!  main time-stepping driver always add a single time-step.
!
      DO ng=1,Ngrids
        IF (Master) THEN
          WRITE (stdout,20) ng, ntstart(ng), ntend(ng)
        END IF
        time(ng)=time(ng)-dt(ng)
      END DO

 10   FORMAT (' Process Information:',/)
 20   FORMAT ('NL ROMS/TOMS: started time-stepping:',                   &
     &        ' (Grid: ',i2.2,' TimeSteps: ',i8.8,' - ',i8.8,')')

      RETURN
      END SUBROUTINE ROMS_initialize

      SUBROUTINE ROMS_run (Tstr, Tend)
!
!=======================================================================
!                                                                      !
!  This routine runs ROMS/TOMS nonlinear model from specified starting !
!  (Tstr) to ending (Tend) time-steps.                                 !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_scalars
!
!  Imported variable declarations.
!
      integer, dimension(Ngrids) :: Tstr   ! starting time-step
      integer, dimension(Ngrids) :: Tend   ! ending   time-step
!
!  Local variable declarations.
!
      integer :: ng, my_iic
!
!-----------------------------------------------------------------------
!  Run model for all nested grids, if any.
!-----------------------------------------------------------------------
!
      NEST_LOOP : DO ng=1,Ngrids

        NL_LOOP : DO my_iic=Tstr(ng),Tend(ng)

          iic(ng)=my_iic
          CALL main3d (ng)
          IF (exit_flag.ne.NoError) RETURN

        END DO NL_LOOP

      END DO NEST_LOOP

      RETURN
      END SUBROUTINE ROMS_run

      SUBROUTINE ROMS_finalize
!
!=======================================================================
!                                                                      !
!  This routine terminates ROMS/TOMS nonlinear model execution.        !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_ncparam
      USE mod_scalars
!
!  Local variable declarations.
!
      integer :: ng, thread

!
!-----------------------------------------------------------------------
!  If blowing-up, save latest model state into RESTART NetCDF file.
!-----------------------------------------------------------------------
!
!  If cycling restart records, write solution into the next record.
!
      DO ng=1,Ngrids
        IF (LwrtRST(ng).and.(exit_flag.eq.1)) THEN
          IF (Master) WRITE (stdout,10)
 10       FORMAT (/,' Blowing-up: Saving latest model state into ',     & 
     &              ' RESTART file',/)
          IF (LcycleRST(ng).and.(NrecRST(ng).ge.2)) THEN
            tRSTindx(ng)=2
            LcycleRST(ng)=.FALSE.
          END IF
          blowup=exit_flag
          exit_flag=NoError
          CALL wrt_rst (ng)
        END IF
      END DO
!
!-----------------------------------------------------------------------
!  Stop model and time profiling clocks.  Close output NetCDF files.
!-----------------------------------------------------------------------
!
!  Stop time clocks.
!
      IF (Master) THEN
        WRITE (stdout,20)
 20     FORMAT (/,' Elapsed CPU time (seconds):',/)
      END IF

      DO ng=1,Ngrids
!$OMP PARALLEL DO PRIVATE(thread) SHARED(ng,numthreads)
        DO thread=0,numthreads-1
          CALL wclock_off (ng, iNLM, 0)
        END DO
!$OMP END PARALLEL DO
      END DO
!
!  Close IO files.
!
      CALL close_io

      RETURN
      END SUBROUTINE ROMS_finalize

      END MODULE ocean_control_mod
