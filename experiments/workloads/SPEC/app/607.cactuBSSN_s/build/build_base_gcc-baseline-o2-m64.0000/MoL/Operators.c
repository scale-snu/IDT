#include "Operators.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

/* These are MoL's low-level operators. If they are overloaded as
   aliased functions, these aliased functions are called; otherwise, a
   default implementation is used. */

/* The aliased functions should never be called directly from MoL's
   time integrators, because they may not exist. Instead, the
   operators defined here (with the MoL_ prefix) should be used. */

static
void
error_no_storage(int const var, int const tl)
{
  char *const fullname = CCTK_FullName(var);
  CCTK_VWarn(CCTK_WARN_ABORT, __LINE__, __FILE__, CCTK_THORNSTRING,
             "Variable %s, timelevel %d has no storage", fullname, tl);
  free(fullname);
  return;
}



/* Some common special cases to improve performance */
static
void
op_real_set_0(CCTK_REAL *restrict const varptr,
              ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = 0.0;
  }
}

static
void
op_real_set_1(CCTK_REAL *restrict const varptr,
              CCTK_REAL const *restrict const srcptr0,
              CCTK_REAL const fact0,
              ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = fact0 * srcptr0[i];
  }
}

static
void
op_real_set_2(CCTK_REAL *restrict const varptr,
              CCTK_REAL const *restrict const srcptr0,
              CCTK_REAL const fact0,
              CCTK_REAL const *restrict const srcptr1,
              CCTK_REAL const fact1,
              ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = fact0 * srcptr0[i] + fact1 * srcptr1[i];
  }
}

static
void
op_real_set_3(CCTK_REAL *restrict const varptr,
              CCTK_REAL const *restrict const srcptr0,
              CCTK_REAL const fact0,
              CCTK_REAL const *restrict const srcptr1,
              CCTK_REAL const fact1,
              CCTK_REAL const *restrict const srcptr2,
              CCTK_REAL const fact2,
              ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = fact0 * srcptr0[i] + fact1 * srcptr1[i] + fact2 * srcptr2[i];
  }
}

static
void
op_real_update_0(CCTK_REAL *restrict const varptr,
                 CCTK_REAL const scale,
                 ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = scale * varptr[i];
  }
}

static
void
op_real_update_1(CCTK_REAL *restrict const varptr,
                 CCTK_REAL const scale,
                 CCTK_REAL const *restrict const srcptr0,
                 CCTK_REAL const fact0,
                 ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = scale * varptr[i] + fact0 * srcptr0[i];
  }
}

static
void
op_real_update_2(CCTK_REAL *restrict const varptr,
                 CCTK_REAL const scale,
                 CCTK_REAL const *restrict const srcptr0,
                 CCTK_REAL const fact0,
                 CCTK_REAL const *restrict const srcptr1,
                 CCTK_REAL const fact1,
                 ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] = scale * varptr[i] + fact0 * srcptr0[i] + fact1 * srcptr1[i];
  }
}

static
void
op_real_update_3(CCTK_REAL *restrict const varptr,
                 CCTK_REAL const scale,
                 CCTK_REAL const *restrict const srcptr0,
                 CCTK_REAL const fact0,
                 CCTK_REAL const *restrict const srcptr1,
                 CCTK_REAL const fact1,
                 CCTK_REAL const *restrict const srcptr2,
                 CCTK_REAL const fact2,
                 ptrdiff_t const npoints)
{
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
  for (ptrdiff_t i=0; i<npoints; ++i) {
    varptr[i] =
      scale * varptr[i] +
      fact0 * srcptr0[i] + fact1 * srcptr1[i] + fact2 * srcptr2[i];
  }
}



CCTK_INT
MoL_LinearCombination(cGH const *const cctkGH,
                      CCTK_INT   const var,
                      CCTK_REAL  const scale,
                      CCTK_INT   const srcs[],
                      CCTK_INT   const tls[],
                      CCTK_REAL  const facts[],
                      CCTK_INT   const nsrcs)
{
  // Forward call to aliased function, if it is defined
  static int is_aliased = -1;
  if (is_aliased < 0) {
    is_aliased = CCTK_IsFunctionAliased("LinearCombination");
  }
  if (is_aliased) {
    return LinearCombination(cctkGH, var, scale, srcs, tls, facts, nsrcs);
  }
  
  // Determine grid variable size
  int const dim = CCTK_GroupDimFromVarI(var);
  int ash[dim];
  int const ierr = CCTK_GroupashVI(cctkGH, dim, ash, var);
  assert(!ierr);
  // TODO: check that all src variables have the same ash
  ptrdiff_t npoints = 1;
  for (int d=0; d<dim; ++d) {
    npoints *= ash[d];
  }
  
  switch (CCTK_VarTypeI(var)) {
    
  case CCTK_VARIABLE_REAL: {
    // Obtain pointer to variable data
    // TODO: check that all variable types are CCTK_REAL
    CCTK_REAL *restrict const varptr = CCTK_VarDataPtrI(cctkGH, 0, var);
    if (!varptr) error_no_storage(var, 0);
    CCTK_REAL const *restrict srcptrs[nsrcs];
    for (int n=0; n<nsrcs; ++n) {
      srcptrs[n] = CCTK_VarDataPtrI(cctkGH, tls[n], srcs[n]);
      if (!srcptrs[n]) error_no_storage(srcs[n], tls[n]);
    }
    
    if (scale == 0.0) {
      // Set (overwrite) target variable
      
      // Introduce special cases for some common cases to improve
      // performance
      switch (nsrcs) {
      case 0:
        op_real_set_0(varptr, npoints);
        break;
      case 1:
        op_real_set_1(varptr, srcptrs[0], facts[0], npoints);
        break;
      case 2:
        op_real_set_2(varptr,
                      srcptrs[0], facts[0], srcptrs[1], facts[1], npoints);
        break;
      case 3:
        op_real_set_3(varptr,
                      srcptrs[0], facts[0], srcptrs[1], facts[1],
                      srcptrs[2], facts[2], npoints);
        break;
      default:
        // Loop over all grid points
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
        for (ptrdiff_t i=0; i<npoints; ++i) {
          CCTK_REAL tmp = 0.0;
          for (int n=0; n<nsrcs; ++n) {
            tmp += facts[n] * srcptrs[n][i];
          }
          varptr[i] = tmp;
        }
        break;
      }
      
    } else {
      // Update (add to) target variable
      
      // Introduce special cases for some common cases to improve
      // performance
      switch (nsrcs) {
      case 0:
        op_real_update_0(varptr, scale, npoints);
        break;
      case 1:
        op_real_update_1(varptr, scale, srcptrs[0], facts[0], npoints);
        break;
      case 2:
        op_real_update_2(varptr, scale,
                         srcptrs[0], facts[0], srcptrs[1], facts[1], npoints);
        break;
      case 3:
        op_real_update_3(varptr, scale,
                         srcptrs[0], facts[0], srcptrs[1], facts[1],
                         srcptrs[2], facts[2], npoints);
        break;
      default:
        // Loop over all grid points
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
        for (ptrdiff_t i=0; i<npoints; ++i) {
          CCTK_REAL tmp = scale * varptr[i];
          for (int n=0; n<nsrcs; ++n) {
            tmp += facts[n] * srcptrs[n][i];
          }
          varptr[i] = tmp;
        }
        break;
      }
      
    }
    break;
  }
    
  case CCTK_VARIABLE_COMPLEX: {
    // Obtain pointer to variable data
    // TODO: check that all variable types are CCTK_COMPLEX
    CCTK_COMPLEX *restrict const varptr = CCTK_VarDataPtrI(cctkGH, 0, var);
    if (!varptr) error_no_storage(var, 0);
    CCTK_COMPLEX const *restrict srcptrs[nsrcs];
    for (int n=0; n<nsrcs; ++n) {
      srcptrs[n] = CCTK_VarDataPtrI(cctkGH, tls[n], srcs[n]);
      if (!srcptrs[n]) error_no_storage(srcs[n], tls[n]);
    }
    
    if (scale == 0.0) {
      // Set (overwrite) target variable
      // Loop over all grid points
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (ptrdiff_t i=0; i<npoints; ++i) {
        CCTK_COMPLEX tmp = CCTK_Cmplx(0.0, 0.0);
        for (int n=0; n<nsrcs; ++n) {
          tmp = CCTK_CmplxAdd(tmp, CCTK_CmplxMul(CCTK_Cmplx(facts[n], 0.0),
                                                 srcptrs[n][i]));
        }
        varptr[i] = tmp;
      }
    } else {
      // Update (add to) target variable
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#pragma omp parallel for
#endif
      for (ptrdiff_t i=0; i<npoints; ++i) {
        CCTK_COMPLEX tmp = CCTK_CmplxMul(CCTK_Cmplx(scale, 0.0), varptr[i]);
        for (int n=0; n<nsrcs; ++n) {
          tmp = CCTK_CmplxAdd(tmp, CCTK_CmplxMul(CCTK_Cmplx(facts[n], 0.0),
                                                 srcptrs[n][i]));
        }
        varptr[i] = tmp;
      }
    }
    break;
  }
    
  default:
    // Other types (e.g. CCTK_REAL4) could be supported as well
    CCTK_WARN(CCTK_WARN_ABORT, "Unsupported variable type");
  }
  
  if (CCTK_IsFunctionAliased("Accelerator_NotifyDataModified")) {
    CCTK_INT const tl = 0;
    Accelerator_NotifyDataModified(cctkGH, &var, &tl, 1, 0);
  }

  // Done
  return 0;
}
