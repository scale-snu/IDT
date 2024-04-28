ROMS/TOMS version 3.2	 SVN Revision  : 405M

See https://www.myroms.org

Executable reads input file from stdin, 1 more ascii input file read,
no files other than stdout and stderr is created.

Porting:

cppdefs.h	Set benchmark.h in stead of ROMS_HEADER on include
globaldefs.h	Change CRAY, CRAT3TE and CRAYX1 to SPEC_* in ifdef to
		undefine DOUBLE_PRECISION if defined.
		Define _OPENMP if SPEC_OPENMP is defined (globaldefs.h is
		included by almost all via cppdefs.h)
mod_ncparam.F90	Added #ifdef SPEC to initialize strings in stead of using
		defines
get_date.F90	Return date_str as empty string for SPEC from get_date
mod_strings.F90	Set system/compiler specific options to blank strings
timers.F90	Add ifdefs dep on NO_GETTIMEOFDAY to avoid pid, timers etc
		Omitting (mandatory) NO_GETTIMEOFDAY will activate timers
		and can be useful for tuning.
inp_par.F90	Deactivate active threads, tiling and thread/tiling specific
		printouts
		Add a extra header line to log identifying the benchmark
		after this line:
		 Benchmark Test, Idealized Southern Ocean, Medium Grid
		 Version adapted for spec, built without netcdf
mp_routines.F90	Changed ifdefs to use SPEC, SPEC_CRAY, SPEC_AIX for 
		flush and getarg; getpid return 0.
		my_wtime returns 0 if NO_GETTIMEOFDAY set.
mod_kinds.F90	For SPEC define r16 as selected_real_kind(12,300) which
		means deactivating 128-bit precision (use 64).
checkdefs.F90	Deactivate print of 
		_OPENMP             OpenMP parallel shared-memory directives.
		to keep comparable files
close_io.F90	Added gards around printout of analytical header files used.
mod_netcdf.F90	Added SPEC gards to avoid netcdf, this includes adding a few
		netcdf constants plus defining some netcdf functions as
		integer and external. These functions are implemented as
		dummies returning -1 (error) at the end, outside of the
		module.
def_var.F90	Added SPEC version of nf90_def_var function call without
		the use of (optional) varid = Vid in call (this requires
		a proper interface).
stiffness.F90	Code inserted from a later version of ROMS (573) (with gards),
		this solves a data race condition for OpenMP
set_weights.F90	With gards, changed a format to get columns in table with 
		float numbers separated by 1 more space (eye readable).

===========================================================================
Other system or compiler specific defines noted during port but not
changed or adapted in any way:

SGI,CRAY	In 5-6 Fortran sources to use different ANINT calls

#if !((defined G95 && defined I686) || defined UNICOS_SN)
# define ASSUMED_SHAPE
#endif

