/*  File produced by Kranc */

#define KRANC_C

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef SPEC 
#include "ML_BSSN/cctk.h" 
#define THORN_DIR "ML_BSSN" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ML_BSSN/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "ML_BSSN/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "GenericFD.h"
#include "McLachlan/ML_BSSN/src/Differencing.h"
#include "cctk_Loop.h"
#include "loopcontrol.h"

/* Define macros used in calculations */
#define INITVALUE (42)
#define INV(x) ((CCTK_REAL)1.0 / (x))
#define SQR(x) ((x) * (x))
#define CUB(x) ((x) * SQR(x))
#define QAD(x) (SQR(SQR(x)))

static void ML_BSSN_convertFromADMBase_Body(const cGH* restrict const cctkGH, const int dir, const int face, const CCTK_REAL normal[3], const CCTK_REAL tangentA[3], const CCTK_REAL tangentB[3], const int imin[3], const int imax[3], const int n_subblock_gfs, CCTK_REAL* restrict const subblock_gfs[])
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  /* Include user-supplied include files */
  
  /* Initialise finite differencing variables */
  const ptrdiff_t di CCTK_ATTRIBUTE_UNUSED = 1;
  const ptrdiff_t dj CCTK_ATTRIBUTE_UNUSED = CCTK_GFINDEX3D(cctkGH,0,1,0) - CCTK_GFINDEX3D(cctkGH,0,0,0);
  const ptrdiff_t dk CCTK_ATTRIBUTE_UNUSED = CCTK_GFINDEX3D(cctkGH,0,0,1) - CCTK_GFINDEX3D(cctkGH,0,0,0);
  const ptrdiff_t cdi CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL) * di;
  const ptrdiff_t cdj CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL) * dj;
  const ptrdiff_t cdk CCTK_ATTRIBUTE_UNUSED = sizeof(CCTK_REAL) * dk;
  const CCTK_REAL dx CCTK_ATTRIBUTE_UNUSED = ToReal(CCTK_DELTA_SPACE(0));
  const CCTK_REAL dy CCTK_ATTRIBUTE_UNUSED = ToReal(CCTK_DELTA_SPACE(1));
  const CCTK_REAL dz CCTK_ATTRIBUTE_UNUSED = ToReal(CCTK_DELTA_SPACE(2));
  const CCTK_REAL dt CCTK_ATTRIBUTE_UNUSED = ToReal(CCTK_DELTA_TIME);
  const CCTK_REAL t CCTK_ATTRIBUTE_UNUSED = ToReal(cctk_time);
  const CCTK_REAL dxi CCTK_ATTRIBUTE_UNUSED = INV(dx);
  const CCTK_REAL dyi CCTK_ATTRIBUTE_UNUSED = INV(dy);
  const CCTK_REAL dzi CCTK_ATTRIBUTE_UNUSED = INV(dz);
  const CCTK_REAL khalf CCTK_ATTRIBUTE_UNUSED = 0.5;
  const CCTK_REAL kthird CCTK_ATTRIBUTE_UNUSED = 
    0.333333333333333333333333333333;
  const CCTK_REAL ktwothird CCTK_ATTRIBUTE_UNUSED = 
    0.666666666666666666666666666667;
  const CCTK_REAL kfourthird CCTK_ATTRIBUTE_UNUSED = 
    1.33333333333333333333333333333;
  const CCTK_REAL hdxi CCTK_ATTRIBUTE_UNUSED = 0.5*dxi;
  const CCTK_REAL hdyi CCTK_ATTRIBUTE_UNUSED = 0.5*dyi;
  const CCTK_REAL hdzi CCTK_ATTRIBUTE_UNUSED = 0.5*dzi;
  
  /* Initialize predefined quantities */
  const CCTK_REAL p1o1024dx CCTK_ATTRIBUTE_UNUSED = 0.0009765625*INV(dx);
  const CCTK_REAL p1o1024dy CCTK_ATTRIBUTE_UNUSED = 0.0009765625*INV(dy);
  const CCTK_REAL p1o1024dz CCTK_ATTRIBUTE_UNUSED = 0.0009765625*INV(dz);
  const CCTK_REAL p1o120dx CCTK_ATTRIBUTE_UNUSED = 0.00833333333333333333333333333333*INV(dx);
  const CCTK_REAL p1o120dy CCTK_ATTRIBUTE_UNUSED = 0.00833333333333333333333333333333*INV(dy);
  const CCTK_REAL p1o120dz CCTK_ATTRIBUTE_UNUSED = 0.00833333333333333333333333333333*INV(dz);
  const CCTK_REAL p1o12dx CCTK_ATTRIBUTE_UNUSED = 0.0833333333333333333333333333333*INV(dx);
  const CCTK_REAL p1o12dy CCTK_ATTRIBUTE_UNUSED = 0.0833333333333333333333333333333*INV(dy);
  const CCTK_REAL p1o12dz CCTK_ATTRIBUTE_UNUSED = 0.0833333333333333333333333333333*INV(dz);
  const CCTK_REAL p1o144dxdy CCTK_ATTRIBUTE_UNUSED = 0.00694444444444444444444444444444*INV(dx*dy);
  const CCTK_REAL p1o144dxdz CCTK_ATTRIBUTE_UNUSED = 0.00694444444444444444444444444444*INV(dx*dz);
  const CCTK_REAL p1o144dydz CCTK_ATTRIBUTE_UNUSED = 0.00694444444444444444444444444444*INV(dy*dz);
  const CCTK_REAL p1o1680dx CCTK_ATTRIBUTE_UNUSED = 0.000595238095238095238095238095238*INV(dx);
  const CCTK_REAL p1o1680dy CCTK_ATTRIBUTE_UNUSED = 0.000595238095238095238095238095238*INV(dy);
  const CCTK_REAL p1o1680dz CCTK_ATTRIBUTE_UNUSED = 0.000595238095238095238095238095238*INV(dz);
  const CCTK_REAL p1o180dx2 CCTK_ATTRIBUTE_UNUSED = 0.00555555555555555555555555555556*INV(SQR(dx));
  const CCTK_REAL p1o180dy2 CCTK_ATTRIBUTE_UNUSED = 0.00555555555555555555555555555556*INV(SQR(dy));
  const CCTK_REAL p1o180dz2 CCTK_ATTRIBUTE_UNUSED = 0.00555555555555555555555555555556*INV(SQR(dz));
  const CCTK_REAL p1o24dx CCTK_ATTRIBUTE_UNUSED = 0.0416666666666666666666666666667*INV(dx);
  const CCTK_REAL p1o24dy CCTK_ATTRIBUTE_UNUSED = 0.0416666666666666666666666666667*INV(dy);
  const CCTK_REAL p1o24dz CCTK_ATTRIBUTE_UNUSED = 0.0416666666666666666666666666667*INV(dz);
  const CCTK_REAL p1o2dx CCTK_ATTRIBUTE_UNUSED = 0.5*INV(dx);
  const CCTK_REAL p1o2dy CCTK_ATTRIBUTE_UNUSED = 0.5*INV(dy);
  const CCTK_REAL p1o2dz CCTK_ATTRIBUTE_UNUSED = 0.5*INV(dz);
  const CCTK_REAL p1o3600dxdy CCTK_ATTRIBUTE_UNUSED = 0.000277777777777777777777777777778*INV(dx*dy);
  const CCTK_REAL p1o3600dxdz CCTK_ATTRIBUTE_UNUSED = 0.000277777777777777777777777777778*INV(dx*dz);
  const CCTK_REAL p1o3600dydz CCTK_ATTRIBUTE_UNUSED = 0.000277777777777777777777777777778*INV(dy*dz);
  const CCTK_REAL p1o4dx CCTK_ATTRIBUTE_UNUSED = 0.25*INV(dx);
  const CCTK_REAL p1o4dxdy CCTK_ATTRIBUTE_UNUSED = 0.25*INV(dx*dy);
  const CCTK_REAL p1o4dxdz CCTK_ATTRIBUTE_UNUSED = 0.25*INV(dx*dz);
  const CCTK_REAL p1o4dy CCTK_ATTRIBUTE_UNUSED = 0.25*INV(dy);
  const CCTK_REAL p1o4dydz CCTK_ATTRIBUTE_UNUSED = 0.25*INV(dy*dz);
  const CCTK_REAL p1o4dz CCTK_ATTRIBUTE_UNUSED = 0.25*INV(dz);
  const CCTK_REAL p1o5040dx2 CCTK_ATTRIBUTE_UNUSED = 0.000198412698412698412698412698413*INV(SQR(dx));
  const CCTK_REAL p1o5040dy2 CCTK_ATTRIBUTE_UNUSED = 0.000198412698412698412698412698413*INV(SQR(dy));
  const CCTK_REAL p1o5040dz2 CCTK_ATTRIBUTE_UNUSED = 0.000198412698412698412698412698413*INV(SQR(dz));
  const CCTK_REAL p1o560dx CCTK_ATTRIBUTE_UNUSED = 0.00178571428571428571428571428571*INV(dx);
  const CCTK_REAL p1o560dy CCTK_ATTRIBUTE_UNUSED = 0.00178571428571428571428571428571*INV(dy);
  const CCTK_REAL p1o560dz CCTK_ATTRIBUTE_UNUSED = 0.00178571428571428571428571428571*INV(dz);
  const CCTK_REAL p1o60dx CCTK_ATTRIBUTE_UNUSED = 0.0166666666666666666666666666667*INV(dx);
  const CCTK_REAL p1o60dy CCTK_ATTRIBUTE_UNUSED = 0.0166666666666666666666666666667*INV(dy);
  const CCTK_REAL p1o60dz CCTK_ATTRIBUTE_UNUSED = 0.0166666666666666666666666666667*INV(dz);
  const CCTK_REAL p1o64dx CCTK_ATTRIBUTE_UNUSED = 0.015625*INV(dx);
  const CCTK_REAL p1o64dy CCTK_ATTRIBUTE_UNUSED = 0.015625*INV(dy);
  const CCTK_REAL p1o64dz CCTK_ATTRIBUTE_UNUSED = 0.015625*INV(dz);
  const CCTK_REAL p1o705600dxdy CCTK_ATTRIBUTE_UNUSED = 1.41723356009070294784580498866e-6*INV(dx*dy);
  const CCTK_REAL p1o705600dxdz CCTK_ATTRIBUTE_UNUSED = 1.41723356009070294784580498866e-6*INV(dx*dz);
  const CCTK_REAL p1o705600dydz CCTK_ATTRIBUTE_UNUSED = 1.41723356009070294784580498866e-6*INV(dy*dz);
  const CCTK_REAL p1o840dx CCTK_ATTRIBUTE_UNUSED = 0.00119047619047619047619047619048*INV(dx);
  const CCTK_REAL p1o840dy CCTK_ATTRIBUTE_UNUSED = 0.00119047619047619047619047619048*INV(dy);
  const CCTK_REAL p1o840dz CCTK_ATTRIBUTE_UNUSED = 0.00119047619047619047619047619048*INV(dz);
  const CCTK_REAL p1odx CCTK_ATTRIBUTE_UNUSED = INV(dx);
  const CCTK_REAL p1odx2 CCTK_ATTRIBUTE_UNUSED = INV(SQR(dx));
  const CCTK_REAL p1ody CCTK_ATTRIBUTE_UNUSED = INV(dy);
  const CCTK_REAL p1ody2 CCTK_ATTRIBUTE_UNUSED = INV(SQR(dy));
  const CCTK_REAL p1odz CCTK_ATTRIBUTE_UNUSED = INV(dz);
  const CCTK_REAL p1odz2 CCTK_ATTRIBUTE_UNUSED = INV(SQR(dz));
  const CCTK_REAL pm1o120dx CCTK_ATTRIBUTE_UNUSED = -0.00833333333333333333333333333333*INV(dx);
  const CCTK_REAL pm1o120dy CCTK_ATTRIBUTE_UNUSED = -0.00833333333333333333333333333333*INV(dy);
  const CCTK_REAL pm1o120dz CCTK_ATTRIBUTE_UNUSED = -0.00833333333333333333333333333333*INV(dz);
  const CCTK_REAL pm1o12dx2 CCTK_ATTRIBUTE_UNUSED = -0.0833333333333333333333333333333*INV(SQR(dx));
  const CCTK_REAL pm1o12dy2 CCTK_ATTRIBUTE_UNUSED = -0.0833333333333333333333333333333*INV(SQR(dy));
  const CCTK_REAL pm1o12dz2 CCTK_ATTRIBUTE_UNUSED = -0.0833333333333333333333333333333*INV(SQR(dz));
  const CCTK_REAL pm1o16dx CCTK_ATTRIBUTE_UNUSED = -0.0625*INV(dx);
  const CCTK_REAL pm1o16dy CCTK_ATTRIBUTE_UNUSED = -0.0625*INV(dy);
  const CCTK_REAL pm1o16dz CCTK_ATTRIBUTE_UNUSED = -0.0625*INV(dz);
  const CCTK_REAL pm1o256dx CCTK_ATTRIBUTE_UNUSED = -0.00390625*INV(dx);
  const CCTK_REAL pm1o256dy CCTK_ATTRIBUTE_UNUSED = -0.00390625*INV(dy);
  const CCTK_REAL pm1o256dz CCTK_ATTRIBUTE_UNUSED = -0.00390625*INV(dz);
  const CCTK_REAL pm1o2dx CCTK_ATTRIBUTE_UNUSED = -0.5*INV(dx);
  const CCTK_REAL pm1o2dy CCTK_ATTRIBUTE_UNUSED = -0.5*INV(dy);
  const CCTK_REAL pm1o2dz CCTK_ATTRIBUTE_UNUSED = -0.5*INV(dz);
  const CCTK_REAL pm1o4dx CCTK_ATTRIBUTE_UNUSED = -0.25*INV(dx);
  const CCTK_REAL pm1o4dy CCTK_ATTRIBUTE_UNUSED = -0.25*INV(dy);
  const CCTK_REAL pm1o4dz CCTK_ATTRIBUTE_UNUSED = -0.25*INV(dz);
  const CCTK_REAL pm1o60dx CCTK_ATTRIBUTE_UNUSED = -0.0166666666666666666666666666667*INV(dx);
  const CCTK_REAL pm1o60dy CCTK_ATTRIBUTE_UNUSED = -0.0166666666666666666666666666667*INV(dy);
  const CCTK_REAL pm1o60dz CCTK_ATTRIBUTE_UNUSED = -0.0166666666666666666666666666667*INV(dz);
  const CCTK_REAL pm1o840dx CCTK_ATTRIBUTE_UNUSED = -0.00119047619047619047619047619048*INV(dx);
  const CCTK_REAL pm1o840dy CCTK_ATTRIBUTE_UNUSED = -0.00119047619047619047619047619048*INV(dy);
  const CCTK_REAL pm1o840dz CCTK_ATTRIBUTE_UNUSED = -0.00119047619047619047619047619048*INV(dz);
  
  /* Assign local copies of arrays functions */
  
  
  
  /* Calculate temporaries and arrays functions */
  
  /* Copy local copies back to grid functions */
  
  /* Loop over the grid points */
  const int imin0=imin[0];
  const int imin1=imin[1];
  const int imin2=imin[2];
  const int imax0=imax[0];
  const int imax1=imax[1];
  const int imax2=imax[2];
#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
  #pragma omp parallel // reduction(+: vec_iter_counter, vec_op_counter, vec_mem_counter)
#endif
  CCTK_LOOP3(ML_BSSN_convertFromADMBase,
    i,j,k, imin0,imin1,imin2, imax0,imax1,imax2,
    cctk_ash[0],cctk_ash[1],cctk_ash[2])
  {
    const ptrdiff_t index CCTK_ATTRIBUTE_UNUSED = di*i + dj*j + dk*k;
    // ++vec_iter_counter;
    
    /* Assign local copies of grid functions */
    
    CCTK_REAL alpL CCTK_ATTRIBUTE_UNUSED = alp[index];
    CCTK_REAL betaxL CCTK_ATTRIBUTE_UNUSED = betax[index];
    CCTK_REAL betayL CCTK_ATTRIBUTE_UNUSED = betay[index];
    CCTK_REAL betazL CCTK_ATTRIBUTE_UNUSED = betaz[index];
    CCTK_REAL gxxL CCTK_ATTRIBUTE_UNUSED = gxx[index];
    CCTK_REAL gxyL CCTK_ATTRIBUTE_UNUSED = gxy[index];
    CCTK_REAL gxzL CCTK_ATTRIBUTE_UNUSED = gxz[index];
    CCTK_REAL gyyL CCTK_ATTRIBUTE_UNUSED = gyy[index];
    CCTK_REAL gyzL CCTK_ATTRIBUTE_UNUSED = gyz[index];
    CCTK_REAL gzzL CCTK_ATTRIBUTE_UNUSED = gzz[index];
    CCTK_REAL kxxL CCTK_ATTRIBUTE_UNUSED = kxx[index];
    CCTK_REAL kxyL CCTK_ATTRIBUTE_UNUSED = kxy[index];
    CCTK_REAL kxzL CCTK_ATTRIBUTE_UNUSED = kxz[index];
    CCTK_REAL kyyL CCTK_ATTRIBUTE_UNUSED = kyy[index];
    CCTK_REAL kyzL CCTK_ATTRIBUTE_UNUSED = kyz[index];
    CCTK_REAL kzzL CCTK_ATTRIBUTE_UNUSED = kzz[index];
    CCTK_REAL phiL CCTK_ATTRIBUTE_UNUSED = phi[index];
    CCTK_REAL trKL CCTK_ATTRIBUTE_UNUSED = trK[index];
    
    
    /* Include user supplied include files */
    
    /* Precompute derivatives */
    
    switch (fdOrder)
    {
      case 2:
      {
        break;
      }
      
      case 4:
      {
        break;
      }
      
      case 6:
      {
        break;
      }
      
      case 8:
      {
        break;
      }
      default:
        CCTK_BUILTIN_UNREACHABLE();
    }
    
    /* Calculate temporaries and grid functions */
    CCTK_REAL g11 CCTK_ATTRIBUTE_UNUSED = gxxL;
    
    CCTK_REAL g12 CCTK_ATTRIBUTE_UNUSED = gxyL;
    
    CCTK_REAL g13 CCTK_ATTRIBUTE_UNUSED = gxzL;
    
    CCTK_REAL g22 CCTK_ATTRIBUTE_UNUSED = gyyL;
    
    CCTK_REAL g23 CCTK_ATTRIBUTE_UNUSED = gyzL;
    
    CCTK_REAL g33 CCTK_ATTRIBUTE_UNUSED = gzzL;
    
    CCTK_REAL detg CCTK_ATTRIBUTE_UNUSED = 2*g12*g13*g23 + g33*(g11*g22 - 
      SQR(g12)) - g22*SQR(g13) - g11*SQR(g23);
    
    CCTK_REAL gu11 CCTK_ATTRIBUTE_UNUSED = INV(detg)*(g22*g33 - SQR(g23));
    
    CCTK_REAL gu12 CCTK_ATTRIBUTE_UNUSED = (g13*g23 - g12*g33)*INV(detg);
    
    CCTK_REAL gu13 CCTK_ATTRIBUTE_UNUSED = (-(g13*g22) + 
      g12*g23)*INV(detg);
    
    CCTK_REAL gu22 CCTK_ATTRIBUTE_UNUSED = INV(detg)*(g11*g33 - SQR(g13));
    
    CCTK_REAL gu23 CCTK_ATTRIBUTE_UNUSED = (g12*g13 - g11*g23)*INV(detg);
    
    CCTK_REAL gu33 CCTK_ATTRIBUTE_UNUSED = INV(detg)*(g11*g22 - SQR(g12));
    
    CCTK_REAL em4phi CCTK_ATTRIBUTE_UNUSED;
    
    if (conformalMethod == 1)
    {
      phiL = pow(detg,-0.166666666666666666666666666667);
      
      em4phi = SQR(phiL);
    }
    else
    {
      phiL = 0.0833333333333333333333333333333*log(detg);
      
      em4phi = exp(-4*phiL);
    }
    
    CCTK_REAL gt11L CCTK_ATTRIBUTE_UNUSED = em4phi*g11;
    
    CCTK_REAL gt12L CCTK_ATTRIBUTE_UNUSED = em4phi*g12;
    
    CCTK_REAL gt13L CCTK_ATTRIBUTE_UNUSED = em4phi*g13;
    
    CCTK_REAL gt22L CCTK_ATTRIBUTE_UNUSED = em4phi*g22;
    
    CCTK_REAL gt23L CCTK_ATTRIBUTE_UNUSED = em4phi*g23;
    
    CCTK_REAL gt33L CCTK_ATTRIBUTE_UNUSED = em4phi*g33;
    
    trKL = kxxL*gu11 + kyyL*gu22 + 2*(kxyL*gu12 + kxzL*gu13 + kyzL*gu23) + 
      kzzL*gu33;
    
    CCTK_REAL At11L CCTK_ATTRIBUTE_UNUSED = em4phi*(kxxL - 
      0.333333333333333333333333333333*trKL*g11);
    
    CCTK_REAL At12L CCTK_ATTRIBUTE_UNUSED = em4phi*(kxyL - 
      0.333333333333333333333333333333*trKL*g12);
    
    CCTK_REAL At13L CCTK_ATTRIBUTE_UNUSED = em4phi*(kxzL - 
      0.333333333333333333333333333333*trKL*g13);
    
    CCTK_REAL At22L CCTK_ATTRIBUTE_UNUSED = em4phi*(kyyL - 
      0.333333333333333333333333333333*trKL*g22);
    
    CCTK_REAL At23L CCTK_ATTRIBUTE_UNUSED = em4phi*(kyzL - 
      0.333333333333333333333333333333*trKL*g23);
    
    CCTK_REAL At33L CCTK_ATTRIBUTE_UNUSED = em4phi*(kzzL - 
      0.333333333333333333333333333333*trKL*g33);
    
    CCTK_REAL alphaL CCTK_ATTRIBUTE_UNUSED = alpL;
    
    CCTK_REAL beta1L CCTK_ATTRIBUTE_UNUSED = betaxL;
    
    CCTK_REAL beta2L CCTK_ATTRIBUTE_UNUSED = betayL;
    
    CCTK_REAL beta3L CCTK_ATTRIBUTE_UNUSED = betazL;
    
    /* Copy local copies back to grid functions */
    alpha[index] = alphaL;
    At11[index] = At11L;
    At12[index] = At12L;
    At13[index] = At13L;
    At22[index] = At22L;
    At23[index] = At23L;
    At33[index] = At33L;
    beta1[index] = beta1L;
    beta2[index] = beta2L;
    beta3[index] = beta3L;
    gt11[index] = gt11L;
    gt12[index] = gt12L;
    gt13[index] = gt13L;
    gt22[index] = gt22L;
    gt23[index] = gt23L;
    gt33[index] = gt33L;
    phi[index] = phiL;
    trK[index] = trKL;
  }
  CCTK_ENDLOOP3(ML_BSSN_convertFromADMBase);
}

extern "C" void ML_BSSN_convertFromADMBase(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Entering ML_BSSN_convertFromADMBase_Body");
  }
  
  if (cctk_iteration % ML_BSSN_convertFromADMBase_calc_every != ML_BSSN_convertFromADMBase_calc_offset)
  {
    return;
  }
  
  const char* const groups[] = {
    "ADMBase::curv",
    "ADMBase::lapse",
    "ADMBase::metric",
    "ADMBase::shift",
    "ML_BSSN::ML_curv",
    "ML_BSSN::ML_lapse",
    "ML_BSSN::ML_log_confac",
    "ML_BSSN::ML_metric",
    "ML_BSSN::ML_shift",
    "ML_BSSN::ML_trace_curv"};
  GenericFD_AssertGroupStorage(cctkGH, "ML_BSSN_convertFromADMBase", 10, groups);
  
  switch (fdOrder)
  {
    case 2:
    {
      break;
    }
    
    case 4:
    {
      break;
    }
    
    case 6:
    {
      break;
    }
    
    case 8:
    {
      break;
    }
    default:
      CCTK_BUILTIN_UNREACHABLE();
  }
  
  GenericFD_LoopOverEverything(cctkGH, ML_BSSN_convertFromADMBase_Body);
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Leaving ML_BSSN_convertFromADMBase_Body");
  }
}
