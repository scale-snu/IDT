#ifndef _CCTK_LOOP_H_
#define _CCTK_LOOP_H_

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
/* It looks like there's no standards-compliant way to keep _Pragma from
 * doing anything, so we've got two complete versions of this file.  The
 * version covered by this conditional is the original and contains
 * _Pragma("omp ...") all over the place.
 */
# include <cctk_Loop_omp.h>

#else

/* The version of cctk_Loop.h covered by this conditional is the version with
 * all _Pragma and "!$omp" removed.  It is otherwise mostly identical to the
 * original copy found in cctk_Loop_omp.h.
 */
# include <cctk_Loop_noomp.h>

#endif /* !OpenMP */

#endif /* #ifndef _CCTK_LOOP_H_ */
