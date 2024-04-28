



























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































      MODULE mod_netcdf
!
!svn $Id: mod_netcdf.F 394 2009-09-04 20:26:15Z arango $
!================================================== Hernan G. Arango ===
!  Copyright (c) 2002-2009 The ROMS/TOMS Group                         !
!    Licensed under a MIT/X style license                              !
!    See License_ROMS.txt                                              !
!=======================================================================
!                                                                      !
!  This MODULE contains all NetCDF variables definitions. It also      !
!  contains several variables and routines to facilitate  generic      !
!  manipulations of NetCDF data.  Usually, several NetCDF library      !
!  calls are required to inquire and read a dimension or variable.     !
!  These routines provide a single interface for such operations.      !
!                                                                      !
!=======================================================================
!
      USE mod_kinds
!
      implicit none
!
      INTERFACE netcdf_get_fvar
        MODULE PROCEDURE netcdf_get_fvar_0d
        MODULE PROCEDURE netcdf_get_fvar_1d
        MODULE PROCEDURE netcdf_get_fvar_2d
        MODULE PROCEDURE netcdf_get_fvar_3d
        MODULE PROCEDURE netcdf_get_fvar_4d
      END INTERFACE netcdf_get_fvar

      INTERFACE netcdf_get_ivar
        MODULE PROCEDURE netcdf_get_ivar_0d
        MODULE PROCEDURE netcdf_get_ivar_1d
        MODULE PROCEDURE netcdf_get_ivar_2d
      END INTERFACE netcdf_get_ivar

      INTERFACE netcdf_get_svar
        MODULE PROCEDURE netcdf_get_svar_0d
        MODULE PROCEDURE netcdf_get_svar_1d
      END INTERFACE netcdf_get_svar

      INTERFACE netcdf_put_fvar
        MODULE PROCEDURE netcdf_put_fvar_0d
        MODULE PROCEDURE netcdf_put_fvar_1d
        MODULE PROCEDURE netcdf_put_fvar_2d
        MODULE PROCEDURE netcdf_put_fvar_3d
        MODULE PROCEDURE netcdf_put_fvar_4d
      END INTERFACE netcdf_put_fvar

      INTERFACE netcdf_put_ivar
        MODULE PROCEDURE netcdf_put_ivar_0d
        MODULE PROCEDURE netcdf_put_ivar_1d
        MODULE PROCEDURE netcdf_put_ivar_2d
      END INTERFACE netcdf_put_ivar

      INTERFACE netcdf_put_lvar
        MODULE PROCEDURE netcdf_put_lvar_0d
        MODULE PROCEDURE netcdf_put_lvar_1d
        MODULE PROCEDURE netcdf_put_lvar_2d
      END INTERFACE netcdf_put_lvar

      INTERFACE netcdf_put_svar
        MODULE PROCEDURE netcdf_put_svar_0d
        MODULE PROCEDURE netcdf_put_svar_1d
      END INTERFACE netcdf_put_svar
!
!  Local dimension parameters.
!
      integer, parameter :: Mdims = 50  ! maximun number of dimensions
      integer, parameter :: Mvars = 500 ! maximun number of variables
      integer, parameter :: NvarD = 5   ! number of variable dimensions
      integer, parameter :: NvarA = 50  ! number of variable attributes
!
!  Generic information about current NetCDF for all dimensions and
!  all variables.
!
      integer :: n_dim                  ! number of dimensions
      integer :: n_var                  ! number of variables 
      integer :: n_gatt                 ! number of global attributes
      integer :: ncformat               ! file format number (version)
      integer :: rec_id                 ! unlimited dimension ID
      integer :: rec_size               ! unlimited dimension value
      integer :: dim_id(Mdims)          ! dimensions ID
      integer :: dim_size(Mdims)        ! dimensions value
      integer :: var_id(Mvars)          ! variables ID
      integer :: var_natt(Mvars)        ! variables number of attributes
      integer :: var_flag(Mvars)        ! Variables water points flag
      integer :: var_type(Mvars)        ! variables external data type
      integer :: var_ndim(Mvars)        ! variables number of dimensions
      integer :: var_dim(NvarD,Mvars)   ! variables dimensions ID
!
      character (len=40) :: dim_name(Mdims)      ! dimensions name
      character (len=40) :: var_name(Mvars)      ! variables name
!
!  Generic information about requested current variable.
!
      integer :: n_vdim                 ! number of variable dimensions
      integer :: n_vatt                 ! number of variable attributes
      integer :: var_kind               ! external data type
      integer :: var_Dids(NvarD)        ! dimensions ID
      integer :: var_Dsize(NvarD)       ! dimensions values
      integer :: var_Aint(NvarA)        ! attribute integer values
      real(r8) :: var_Afloat(NvarA)     ! attribute float values
!
      character (len=40) :: var_Aname(NvarA)     ! Attribute names
      character (len=40) :: var_Dname(NvarD)     ! dimension names
      character (len=80) :: var_Achar(NvarA)     ! Attribute char values
!
!  External data representation for floating-point variables.
!
! Necessary definitions for spec to run benchmark without needing
! netcdf as part of the code. Errors should occurr if any of the
! routines below are actually called.
! Benchmark does NOT read nor produce any netcdf file.
      integer nf90_get_var, nf90_put_var, nf90_def_var, nf90_strerror
      external nf90_get_var, nf90_put_var, nf90_def_var, nf90_strerror
      integer nf90_put_att, nf90_get_dim, nf90_def_dim
      external nf90_put_att, nf90_get_dim, nf90_def_dim
      integer, parameter :: &
    nf90_char   = 2,            &
    nf90_int    = 4,            &
    nf90_float  = 5,            &
    nf90_real   = nf90_float,   &
    nf90_real4  = nf90_float,   &
    nf90_double = 6,            &
    nf90_real8  = nf90_double
  integer, parameter, public :: &
    nf90_noerr     = 0,         &
    nf90_global    = 0,         &
    nf90_unlimited = 0,         &
    nf90_nowrite   = 0,         &
    nf90_write     = 1,         &
    nf90_clobber   = 0,         &
    nf90_noclobber = 4

      integer, parameter :: NF_FOUT = nf90_real
      integer, parameter :: NF_FRST = nf90_double
      integer, parameter :: NF_TYPE = nf90_double
!
!  Netcdf file creation mode flag.
!
      integer :: CMODE = nf90_clobber      ! NetCDF classic format file
!
!  Set NetCDF-4/HDF5 deflate (file compression) parameters.
!
      integer :: shuffle = 1
      integer :: deflate = 1
      integer :: deflate_level = 1
!
!  Set parallel I/O access for processing data: Independent and
!  Collective. Independent I/O access means that processing do not
!  depend on or be affected by other parallel processes (nodes).
!  This is the case in non-tiled I/O variables. Contrarily,
!  Collective I/O access implies that all parallel processess
!  participate during processing. This is the case for tiled
!  variables: each node in the group reads/writes their own tile
!  data when parallel I/O is activated.
!
!  Note: the parallel access flags nf90_independent and nf90_collective
!        are currently missing in module netcdf.mod.
!
      integer, parameter :: IO_independent = 0      ! nf90_independent
      integer, parameter :: IO_collective = 1       ! nf90_collective

      CONTAINS

      SUBROUTINE netcdf_get_dim (ng, model, ncname, DimName, DimSize,   &
     &                           DimID)
!
!=======================================================================
!                                                                      !
!  This routine inquires a NetCDF file dimensions names and values.    !
!  All the dimension information is stored in the module variables     !
!  declared above.  In addition, if a particular dimension name is     !
!  provided, this routine returns the requested information in the     !
!  optional arguments.                                                 !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     DimName      Requested dimension name (string, OPTIONAL)         !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     DimSize      Size of requested dimension (integer, OPTIONAL)     !
!     DimID        ID od requested dimension (integer, OPTIONAL)       !
!                                                                      !
!  Other information stored in this module:                            !
!                                                                      !
!     n_dim        Number of dimensions                                !
!     n_var        Number of variables                                 !
!     n_gatt       Number of global attributes                         !
!     rec_id       Unlimited dimension ID                              !
!     rec_size     Size of unlimited dimension                         !
!     dim_name     Dimensions name (1:n_dim)                           !
!     dim_id       Dimensions ID (1:n_dim)                             !
!     dim_size     Dimensions value (1:n_dim)                          !
!                                                                      !
!     WARNING: This is information is rewritten during each CALL.      !
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
      integer, intent(in) :: ng, model

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in), optional :: DimName

      integer, intent(out), optional :: DimSize
      integer, intent(out), optional :: DimID
      RETURN
      END SUBROUTINE netcdf_get_dim

      SUBROUTINE netcdf_check_dim (ng, model, ncname)
!
!=======================================================================
!                                                                      !
!  This routine inquires a NetCDF file dimensions names and values.    !
!  It checks the file dimensions against model dimension parameters    !
!  for consitency.  All the dimensions information is stored in the    !
!  module variables declared above.                                    !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!                                                                      !
!  On output the following information is stored in this module:       !
!                                                                      !
!     n_dim        Number of dimensions                                !
!     n_var        Number of variables                                 !
!     n_gatt       Number of global attributes                         !
!     rec_id       Unlimited dimension ID                              !
!     rec_size     Size of unlimited dimension                         !
!     dim_name     Dimensions name (1:n_dim)                           !
!     dim_id       Dimensions ID (1:n_dim)                             !
!     dim_size     Dimensions value (1:n_dim)                          !
!                                                                      !
!     WARNING: This is information is rewritten during each CALL.      !
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
      integer, intent(in) :: ng, model

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_check_dim

      SUBROUTINE netcdf_check_var (ng, model, ncname)
!
!=======================================================================
!                                                                      !
!  This routine inquires the NetCDF file variables and check if the    !
!  values of few of them are consitent with the parameters provided    !
!  in input scripts. All the variables information is stored in the    !
!  module variables declared above.                                    !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!                                                                      !
!  On output the following information is stored in this module:       !
!                                                                      !
!     n_dim        Number of dimensions                                !
!     n_var        Number of variables                                 !
!     n_gatt       Number of global attributes                         !
!     rec_id       Unlimited dimension ID                              !
!     var_name     Variables name (1:n_var)                            !
!     var_id       Variables ID (1:n_var)                              !
!     var_natt     Variables number of attributes (1:n_var)            !
!     var_flag     Variables flag [1=full field, -1=water points only] !
!     var_type     Variables external data type (1:n_var)              !
!     var_ndim     Variables number of dimensions (1:n_var)            !
!     var_dim      Variables dimensions ID (:,1:n_var)                 !
!                                                                      !
!     WARNING: This is information is rewritten during each CALL.      !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_ncparam
      USE mod_scalars

!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, model

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_check_var

      SUBROUTINE netcdf_inq_var (ng, model, ncname, ncid, myVarName,    &
     &                           SearchVar, VarID, nVarDim, nVarAtt)
!
!=======================================================================
!                                                                      !
!  This routine inquires a NetCDF file dimensions names and values.    !
!  All the dimension information is stored in the module variables     !
!  declared above.  In addition, if a particular dimension name is     !
!  provided, this routine returns the requested information in the     !
!  optional arguments.                                                 !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     myVarName    Requested variable name (string, OPTIONAL)          !
!     SearchVar    Switch used when searching a variable over          !
!                    multiple NetCDF files (logical, OPTIONAL)         !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     VarID        ID of requested variable (integer, OPTIONAL)        !
!     nVarDim      Number of variable dimensions (integer, OPTIONAL)   !
!     nVarAtt      Number of variable attributes (integer, OPTIONAL)   !
!                                                                      !
!  Other information stored in this module:                            !
!                                                                      !
!     n_dim        Number of dimensions                                !
!     n_var        Number of variables                                 !
!     n_gatt       Number of global attributes                         !
!     rec_id       Unlimited dimension ID                              !
!     var_name     Variables name (1:n_var)                            !
!     var_id       Variables ID (1:n_var)                              !
!     var_natt     Variables number of attributes (1:n_var)            !
!     var_flag     Variables flag [1=full field, -1=water points only] !
!     var_type     Variables external data type (1:n_var)              !
!     var_ndim     Variables number of dimensions (1:n_var)            !
!     var_dim      Variables dimensions ID (:,1:n_var)                 !
!                                                                      !
!  If the OPTIONAL argument myVarName is provided, the following       !
!  information for requested variable is also stored:                  !
!                                                                      !
!     n_vatt       Number of variable attributes                       !
!     n_vdim       Number of variable dimensions                       !
!     var_kind     Variable external data type                         !
!     var_Aname    Variable attribute names (1:n_vatt)                 !
!     var_Achar    Variable string attribute values (1:n_vatt)         !
!     var_Afloat   Variable float attribute values (1:n_vatt)          !
!     var_Aint     Variable integer attribute values (1:n_vatt)        !
!     var_Dids     Variable dimensions ID (1:n_vdim)                   !
!     var_Dname    Variable dimensions name (1:n_vdim)                 !
!     var_Dsize    Variable dimensions size (1:n_vdim)                 !
!                                                                      !
!     WARNING: This is information is rewritten during each CALL.      !
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
      integer, intent(in) :: ng, model

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in), optional :: myVarName

      logical, intent(out), optional :: SearchVar

      integer, intent(in), optional :: ncid

      integer, intent(out), optional :: VarID
      integer, intent(out), optional :: nVarDim
      integer, intent(out), optional :: nVarAtt
      RETURN
      END SUBROUTINE netcdf_inq_var

      SUBROUTINE netcdf_inq_varid (ng, model, ncname, myVarName,        &
     &                             ncid, VarID)
!
!=======================================================================
!                                                                      !
!  This routine inquires ID of requested NetCDF variable.              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Requested variable name (string)                    !
!     ncid         NetCDF file ID (integer)                            !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     VarID        ID of requested variable (integer)                  !
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
      integer, intent(in) :: ng, model, ncid

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      integer, intent(out) :: VarID
      RETURN
      END SUBROUTINE netcdf_inq_varid

      SUBROUTINE netcdf_get_fatt (ng, model, ncname, varid, AttName,    &
     &                            AttValue, foundit, ncid)
!
!=======================================================================
!                                                                      !
!  This routine gets requested floating-point variable attribute(s).   !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     varid        Variabke ID of the attribute's variable, or         !
!                    NF90_GLOBAL for a global attribute (integer)      !
!     AttName      Attribute name to read (string array)               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     AttValue     Attribute value (real array)                        !
!     foundit      Switch (T/F) activated when the requested           !
!                    attribute is found (logical array)                !
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
      integer, intent(in) :: ng, model, varid

      integer, intent(in), optional :: ncid

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: AttName(:)

      logical, intent(out) :: foundit(:)

      real(r8), intent(out) :: AttValue(:)
      RETURN
      END SUBROUTINE netcdf_get_fatt

      SUBROUTINE netcdf_get_fvar_0d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total,                &
     &                               min_val, max_val)
!
!=======================================================================
!                                                                      !
!  This routine reads requested floating-point scalar variable from    !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read scalar variable (real)                         !
!     min_val      Read data minimum value (real, OPTIONAL)            !
!     max_val      Read data maximum value (real, OPTIONAL)            !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar)       !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar(1))    !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      real(r8), intent(out), optional :: min_val
      real(r8), intent(out), optional :: max_val

      real(r8), intent(out) :: A
      RETURN
      END SUBROUTINE netcdf_get_fvar_0d

      SUBROUTINE netcdf_get_fvar_1d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total,                &
     &                               min_val, max_val)
!
!=======================================================================
!                                                                      !
!  This routine reads requested floating-point 1D-array variable from  !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 1D-array variable (real)                       !
!     min_val      Read data minimum value (real, OPTIONAL)            !
!     max_val      Read data maximum value (real, OPTIONAL)            !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar)       !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar(0:))   !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar(:,1))  !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      real(r8), intent(out), optional :: min_val
      real(r8), intent(out), optional :: max_val

      real(r8), intent(out) :: A(:)
      RETURN
      END SUBROUTINE netcdf_get_fvar_1d

      SUBROUTINE netcdf_get_fvar_2d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total,                &
     &                               min_val, max_val)
!
!=======================================================================
!                                                                      !
!  This routine reads requested floating-point 2D-array variable from  !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 2D-array variable (real)                       !
!     min_val      Read data minimum value (real, OPTIONAL)            !
!     max_val      Read data maximum value (real, OPTIONAL)            !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar)       !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar(0:,:)) !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar(0:,0:))!
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar(:,:,1))!
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      real(r8), intent(out), optional :: min_val
      real(r8), intent(out), optional :: max_val

      real(r8), intent(out) :: A(:,:)
      RETURN
      END SUBROUTINE netcdf_get_fvar_2d

      SUBROUTINE netcdf_get_fvar_3d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total,                &
     &                               min_val, max_val)
!
!=======================================================================
!                                                                      !
!  This routine reads requested floating-point 3D-array variable from  !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 3D-array variable (real)                       !
!     min_val      Read data minimum value (real, OPTIONAL)            !
!     max_val      Read data maximum value (real, OPTIONAL)            !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar)       !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      real(r8), intent(out), optional :: min_val
      real(r8), intent(out), optional :: max_val

      real(r8), intent(out) :: A(:,:,:)
      RETURN
      END SUBROUTINE netcdf_get_fvar_3d

      SUBROUTINE netcdf_get_fvar_4d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total,                &
     &                               min_val, max_val)
!
!=======================================================================
!                                                                      !
!  This routine reads requested floating-point 4D-array variable from  !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 4D-array variable (real)                       !
!     min_val      Read data minimum value (real, OPTIONAL)            !
!     max_val      Read data maximum value (real, OPTIONAL)            !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_fvar (ng, iNLM, 'file.nc', 'VarName', fvar)       !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      real(r8), intent(out), optional :: min_val
      real(r8), intent(out), optional :: max_val

      real(r8), intent(out) :: A(:,:,:,:)
      RETURN
      END SUBROUTINE netcdf_get_fvar_4d

      SUBROUTINE netcdf_get_ivar_0d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total)
!
!=======================================================================
!                                                                      !
!  This routine reads requested integer scalar variable from specified !
!  NetCDF file.                                                        !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read scalar variable (integer)                      !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar)       !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar(1))    !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      integer, intent(out) :: A
      RETURN
      END SUBROUTINE netcdf_get_ivar_0d

      SUBROUTINE netcdf_get_ivar_1d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total)
!
!=======================================================================
!                                                                      !
!  This routine reads requested integer 1D-array variable from         !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 1D-array variable (integer)                    !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar)       !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar(0:))   !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar(:,1))  !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      integer, intent(out) :: A(:)
      RETURN
      END SUBROUTINE netcdf_get_ivar_1d

      SUBROUTINE netcdf_get_ivar_2d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total)
!
!=======================================================================
!                                                                      !
!  This routine reads requested integer 2D-array variable from         !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 2D-array variable (integer)                    !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar)       !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar(0:,:)) !
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar(0:,0:))!
!    CALL netcdf_get_ivar (ng, iNLM, 'file.nc', 'VarName', ivar(:,:,1))!
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      integer, intent(out) :: A(:,:)
      RETURN
      END SUBROUTINE netcdf_get_ivar_2d

      SUBROUTINE netcdf_get_svar_0d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total)
!
!=======================================================================
!                                                                      !
!  This routine reads requested string scalar variable from specified  !
!  NetCDF file.                                                        !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read scalar variable (string)                       !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_svar (ng, iNLM, 'file.nc', 'VarName', ivar)       !
!    CALL netcdf_get_svar (ng, iNLM, 'file.nc', 'VarName', ivar(1))    !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      character (len=*), intent(out) :: A
      RETURN
      END SUBROUTINE netcdf_get_svar_0d

      SUBROUTINE netcdf_get_svar_1d (ng, model, ncname, myVarName, A,   &
     &                               ncid, start, total)
!
!=======================================================================
!                                                                      !
!  This routine reads requested string 1D-array variable from          !
!  specified NetCDF file.                                              !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     start        Starting index where the first of the data values   !
!                    will be read along each dimension (integer,       !
!                    OPTIONAL)                                         !
!     total        Number of data values to be read along each         !
!                    dimension (integer, OPTIONAL)                     !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     A            Read 1D-array variable (string)                     !
!                                                                      !
!  Examples:                                                           !
!                                                                      !
!    CALL netcdf_get_svar (ng, iNLM, 'file.nc', 'VarName', ivar)       !
!    CALL netcdf_get_svar (ng, iNLM, 'file.nc', 'VarName', ivar(0:))   !
!    CALL netcdf_get_svar (ng, iNLM, 'file.nc', 'VarName', ivar(:,1))  !
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
      integer, intent(in) :: ng, model

      integer, intent(in), optional :: ncid
      integer, intent(in), optional :: start(:)
      integer, intent(in), optional :: total(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName

      character (len=*), intent(out) :: A(:)
      RETURN
      END SUBROUTINE netcdf_get_svar_1d

      SUBROUTINE netcdf_put_fvar_0d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a floating-point scalar variable into a NetCDF  !
!  file.  If the NetCDF ID is not provided, it opens the file, writes  !
!  data, and then closes the file.                                     !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (real)                  !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      real(r8), intent(in) :: A

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_fvar_0d

      SUBROUTINE netcdf_put_fvar_1d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a floating-point 1D-array variable into a file. !
!  If the NetCDF ID is not provided,  it opens the file,  writes data, !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (real)                  !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      real(r8), intent(in) :: A(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_fvar_1d

      SUBROUTINE netcdf_put_fvar_2d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a floating-point 2D-array variable into a file. !
!  If the NetCDF ID is not provided,  it opens the file,  writes data, !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (real)                  !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      real(r8), intent(in) :: A(:,:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_fvar_2d

      SUBROUTINE netcdf_put_fvar_3d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a floating-point 3D-array variable into a file. !
!  If the NetCDF ID is not provided,  it opens the file,  writes data, !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (real)                  !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      real(r8), intent(in) :: A(:,:,:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_fvar_3d

      SUBROUTINE netcdf_put_fvar_4d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a floating-point 4D-array variable into a file. !
!  If the NetCDF ID is not provided,  it opens the file,  writes data, !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (real)                  !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      real(r8), intent(in) :: A(:,:,:,:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_fvar_4d

      SUBROUTINE netcdf_put_ivar_0d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a integer scalar variable into a NetCDF file.   !
!  If the NetCDF ID is not provided, it opens the file, writes data,   !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (integer)               !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      integer, intent(in) :: A

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_ivar_0d

      SUBROUTINE netcdf_put_ivar_1d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a integer 1D-array variable into a file. If     !
!  the NetCDF ID is not provided, it opens the file,  writes data,     !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (integer)               !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      integer, intent(in) :: A(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_ivar_1d

      SUBROUTINE netcdf_put_ivar_2d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a integer 2D-array variable into a file. If     !
!  the NetCDF ID is not provided, it opens the file,  writes data,     !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (integer)               !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      integer, intent(in) :: A(:,:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_ivar_2d

      SUBROUTINE netcdf_put_lvar_0d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a logical scalar variable into a NetCDF file.   !
!  If the NetCDF ID is not provided, it opens the file, writes data,   !
!  and then closes the file.                                           !
!                                                                      !
!  The input logical data is converted to integer such that .FALSE.    !
!  is interpreted as zero, and .TRUE. is interpreted as one.           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (logical)               !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      logical, intent(in) :: A

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_lvar_0d

      SUBROUTINE netcdf_put_lvar_1d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a logical 1D-array variable into a file. If     !
!  the NetCDF ID is not provided, it opens the file,  writes data,     !
!  and then closes the file.                                           !
!                                                                      !
!  The input logical data is converted to integer such that .FALSE.    !
!  is interpreted as zero, and .TRUE. is interpreted as one.           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (logical)               !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      logical, intent(in) :: A(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_lvar_1d

      SUBROUTINE netcdf_put_lvar_2d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a logical 2D-array variable into a file. If     !
!  the NetCDF ID is not provided, it opens the file,  writes data,     !
!  and then closes the file.                                           !
!                                                                      !
!  The input logical data is converted to integer such that .FALSE.    !
!  is interpreted as zero, and .TRUE. is interpreted as one.           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (logical)               !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      logical, intent(in) :: A(:,:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_lvar_2d

      SUBROUTINE netcdf_put_svar_0d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a  string scalar variable into a NetCDF file.   !
!  If the NetCDF ID is not provided, it opens the file, writes data,   !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (string)                !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model

      integer, intent(in) :: start(:), total(:)    ! dummy variables

      integer, intent(in), optional :: ncid, varid

      character (len=*), intent(in) :: A
      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_svar_0d

      SUBROUTINE netcdf_put_svar_1d (ng, model, ncname, myVarName, A,   &
     &                               start, total, ncid, varid)
!
!=======================================================================
!                                                                      !
!  This routine writes a string 1D-array variable into a file.  If     !
!  the NetCDF ID is not provided, it opens the file,  writes data,     !
!  and then closes the file.                                           !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       NetCDF file name (string)                           !
!     myVarName    Variable name (string)                              !
!     A            Data value(s) to be written (1D string array)       !
!     start        Starting index where the first of the data values   !
!                    will be written along each dimension (integer)    !
!     total        Number of data values to be written along each      !
!                    dimension (integer)                               !
!     ncid         NetCDF file ID (integer, OPTIONAL)                  !
!     varid        NetCDF variable ID (integer, OPTIONAL)              !
!                                                                      !
!  On Ouput:                                                           !
!                                                                      !
!     exit_flag    Error flag (integer) stored in MOD_SCALARS          !
!     ioerror      NetCDF return code (integer) stored in MOD_IOUNITS  !
!                                                                      !
!  Notice: This routine must be used to write only nontiled variables. !
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
      integer, intent(in) :: ng, model
 
      integer, intent(in) :: start(:), total(:)

      integer, intent(in), optional :: ncid, varid

      character (len=*), intent(in) :: A(:)

      character (len=*), intent(in) :: ncname
      character (len=*), intent(in) :: myVarName
      RETURN
      END SUBROUTINE netcdf_put_svar_1d

      SUBROUTINE netcdf_close (ng, model, ncid, ncname)
!
!=======================================================================
!                                                                      !
!  This routine closes requested NetCDF file. If appropriate, it       !
!  also performs additional processing, like updating the global       !
!  attributes, before closing the file.                                !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncid         NetCDF file ID (integer)                            !
!     ncname       NetCDF file name (string, OPTIONAL)                 !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_parallel
      USE mod_iounits
      USE mod_ncparam
      USE mod_scalars

!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, model

      integer, intent(inout) :: ncid

      character (len=*), intent(in), optional :: ncname
      RETURN
      END SUBROUTINE netcdf_close

      SUBROUTINE netcdf_create (ng, model, ncname, ncid)
!
!=======================================================================
!                                                                      !
!  This routine creates a new NetCDF file. Currently, it only creates  !
!  file for serial or parallel I/O access.                             !
!                                                                      !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       Name of the new NetCDF file (string)                !
!                                                                      !
!  On Output:                                                          !
!                                                                      !
!     ncid         NetCDF file ID (integer)                            !
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
      integer, intent(in) :: ng, model

      integer, intent(out) :: ncid

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_create

      SUBROUTINE netcdf_enddef (ng, model, ncname, ncid)
!
!=======================================================================
!                                                                      !
!  This routine ends definition in an  open NetCDF  dataset.  The      !
!  changes made in define mode  are checked and committed to disk      !
!  if no errors occurred. The dataset is then placed in data mode,     !
!  so variable data can be read or written.                            !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       Name of the new NetCDF file (string)                !
!     ncid         NetCDF file ID (integer)                            !
!                                                                      !
!  Note:   There is not need to call this function when processing     !
!          NetCDF-4 format type file.                                  !
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
      integer, intent(in) :: ng, model, ncid

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_enddef

      SUBROUTINE netcdf_open (ng, model, ncname, omode, ncid)
!
!=======================================================================
!                                                                      !
!  This routine opens an existing NetCDF file for access. Currently,   !
!  it only opens file for serial or parallel I/O access.               !
!                                                                      !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       Name of the new NetCDF file (string)                !
!     omode        Open mode flag:                                     !
!                    omode = 0,  read-only access (NF90_NOWRITE)       !
!                    omode = 1,  read and write access (NF90_WRITE)    !
!                                                                      !
!  On Output:                                                          !
!                                                                      !
!     ncid         NetCDF file ID (integer)                            !
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
      integer, intent(in) :: ng, model, omode

      integer, intent(out) :: ncid

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_open

      SUBROUTINE netcdf_openid (nfiles, fname, fid, ncname, ncid)
!
!=======================================================================
!                                                                      !
!  This routine determines association between pathnames and NetCDF    !
!  file descriptors. It is used to get the  ID  of an opened NetCDF    !
!  from file name.  This is done to avoid  opening  too  many files    !
!  simultaneously for the same dataset.                                !
!                                                                      !
!  Input:                                                              !
!                                                                      !
!    nfiles    Number of input NetCDF files.                           !
!    fname     NetCDF file name(s).                                    !
!    fid       NetCDF ID(s) associated with fname.                     !
!    ncname    NetCDF file name to check in association table.         !
!    ncid      NetCDF ID to check in association table.                !
!                                                                      !
!  Output:                                                             !
!                                                                      !
!    fid       If updated, it stores the ID associated with fname.     !
!    ncid      If updated, indicates that ncname has been opened       !
!                previously and this is its associated ID.             !
!                                                                      !
!=======================================================================
!
      implicit none
!
!  Imported variable declarations.
!
      integer, intent(in) :: nfiles

      character (len=*), intent(in) :: fname(nfiles)
      character (len=*), intent(in) :: ncname

      integer, intent(inout) :: fid(nfiles)
      integer, intent(inout) :: ncid
      RETURN
      END SUBROUTINE netcdf_openid

      SUBROUTINE netcdf_redef (ng, model, ncname, ncid)
!
!=======================================================================
!                                                                      !
!  This routine puts an open NetCDF dataset into define mode, so       !
!  dimensions, variables, and attributes can be added or renamed       !
!  an attributes can be deleted.                                       !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       Name of the new NetCDF file (string)                !
!     ncid         NetCDF file ID (integer)                            !
!                                                                      !
!  Note:   There is not need to call this function when processing     !
!          NetCDF-4 format type files.                                 !
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
      integer, intent(in) :: ng, model, ncid

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_redef

      SUBROUTINE netcdf_sync (ng, model, ncname, ncid)
!
!=======================================================================
!                                                                      !
!  This routine synchronize to disk requested NetCDF file with         !
!  in-memory buffer to make data available to other  processes         !
!  immediately after it is written.                                    !
!                                                                      !
!  On Input:                                                           !
!                                                                      !
!     ng           Nested grid number (integer)                        !
!     model        Calling model identifier (integer)                  !
!     ncname       Name of the new NetCDF file (string)                !
!     ncid         NetCDF file ID (integer)                            !
!                                                                      !
!  NOTE:                                                               !
!                                                                      !
!  Nowadays, it is recommended to have the writer and readers open     !
!  the NetCDF file with the NF90_SHARE flag to improve performance.    !
!  See NetCDF user manual for more details.                            !
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
      integer, intent(in) :: ng, model, ncid

      character (len=*), intent(in) :: ncname
      RETURN
      END SUBROUTINE netcdf_sync

      END MODULE mod_netcdf
       integer function nf90_get_var()
         nf90_get_var=-1
       end function 
       integer function nf90_put_var()
         nf90_put_var=-1
       end function
       integer function nf90_def_var()
         nf90_def_var=-1
       end function
       integer function nf90_put_att()
         nf90_put_att=-1
       end function
       integer function nf90_get_dim()
         nf90_get_dim=-1
       end function
       integer function nf90_def_dim()
         nf90_def_dim=-1
       end function
       integer function nf90_strerror()
         nf90_strerror=-1
       end function
