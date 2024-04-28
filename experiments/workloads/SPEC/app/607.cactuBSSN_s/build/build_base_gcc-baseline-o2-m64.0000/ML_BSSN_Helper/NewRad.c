#include <math.h>

#ifdef SPEC 
#include "ML_BSSN_Helper/cctk.h" 
#define THORN_DIR "ML_BSSN_Helper" 
#else 
#include <cctk.h>
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ML_BSSN_Helper/cctk_Arguments.h" 
#else 
#include <cctk_Arguments.h>
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ML_BSSN_Helper/cctk_Parameters.h" 
#else 
#include <cctk_Parameters.h>
#endif /* END SPEC */ 

#define THORN_ML_BSSN           // "ML_BSSN" will be replaced
#ifdef THORN_ML_CCZ4
#  define HAVE_THETA
#endif

static void
newrad (cGH const * restrict cctkGH,
        CCTK_REAL const * restrict var,
        CCTK_REAL * restrict rhs,
        CCTK_REAL var0,
        CCTK_REAL v0);

void
ML_BSSN_NewRad (CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  CCTK_REAL const v0 = sqrt (harmonicF);
  
  newrad (cctkGH, phi  , phirhs  , conformalMethod ? 1 : 0, v0 );
  
  newrad (cctkGH, gt11 , gt11rhs , 1.0, 1.0);
  newrad (cctkGH, gt12 , gt12rhs , 0.0, 1.0);
  newrad (cctkGH, gt13 , gt13rhs , 0.0, 1.0);
  newrad (cctkGH, gt22 , gt22rhs , 1.0, 1.0);
  newrad (cctkGH, gt23 , gt23rhs , 0.0, 1.0);
  newrad (cctkGH, gt33 , gt33rhs , 1.0, 1.0);
  
  newrad (cctkGH, Xt1  , Xt1rhs  , 0.0, 1.0);
  newrad (cctkGH, Xt2  , Xt2rhs  , 0.0, 1.0);
  newrad (cctkGH, Xt3  , Xt3rhs  , 0.0, 1.0);
  
#ifdef HAVE_THETA
  newrad (cctkGH, Theta, Thetarhs, 0.0, v0 );
#endif
  
  newrad (cctkGH, trK  , trKrhs  , 0.0, v0 );
  
  newrad (cctkGH, At11 , At11rhs , 0.0, 1.0);
  newrad (cctkGH, At12 , At12rhs , 0.0, 1.0);
  newrad (cctkGH, At13 , At13rhs , 0.0, 1.0);
  newrad (cctkGH, At22 , At22rhs , 0.0, 1.0);
  newrad (cctkGH, At23 , At23rhs , 0.0, 1.0);
  newrad (cctkGH, At33 , At33rhs , 0.0, 1.0);
  
  newrad (cctkGH, alpha, alpharhs, 1.0, v0 );
  
  newrad (cctkGH, A    , Arhs    , 0.0, v0 );
  
  newrad (cctkGH, beta1, beta1rhs, 0.0, 1.0);
  newrad (cctkGH, beta2, beta2rhs, 0.0, 1.0);
  newrad (cctkGH, beta3, beta3rhs, 0.0, 1.0);
  
  newrad (cctkGH, B1   , B1rhs   , 0.0, 1.0);
  newrad (cctkGH, B2   , B2rhs   , 0.0, 1.0);
  newrad (cctkGH, B3   , B3rhs   , 0.0, 1.0);
}

static void
newrad (cGH const * restrict const cctkGH,
        CCTK_REAL const * restrict const var,
        CCTK_REAL * restrict const rhs,
        CCTK_REAL const var0,
        CCTK_REAL const v0)
{
  DECLARE_CCTK_PARAMETERS
  
  NewRad_Apply (cctkGH, var, rhs, var0, v0, radpower);
}
