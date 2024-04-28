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

extern "C" void ML_BSSN_Advect_SelectBCs(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  if (cctk_iteration % ML_BSSN_Advect_calc_every != ML_BSSN_Advect_calc_offset)
    return;
  CCTK_INT ierr CCTK_ATTRIBUTE_UNUSED = 0;
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_curvrhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_curvrhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_dtlapserhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_dtlapserhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_dtshiftrhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_dtshiftrhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_Gammarhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_Gammarhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_lapserhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_lapserhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_log_confacrhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_log_confacrhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_metricrhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_metricrhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_shiftrhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_shiftrhs.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_trace_curvrhs","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_trace_curvrhs.");
  return;
}

static void ML_BSSN_Advect_Body(const cGH* restrict const cctkGH, const int dir, const int face, const CCTK_REAL normal[3], const CCTK_REAL tangentA[3], const CCTK_REAL tangentB[3], const int imin[3], const int imax[3], const int n_subblock_gfs, CCTK_REAL* restrict const subblock_gfs[])
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
  CCTK_LOOP3(ML_BSSN_Advect,
    i,j,k, imin0,imin1,imin2, imax0,imax1,imax2,
    cctk_ash[0],cctk_ash[1],cctk_ash[2])
  {
    const ptrdiff_t index CCTK_ATTRIBUTE_UNUSED = di*i + dj*j + dk*k;
    // ++vec_iter_counter;
    
    /* Assign local copies of grid functions */
    
    CCTK_REAL AL CCTK_ATTRIBUTE_UNUSED = A[index];
    CCTK_REAL alphaL CCTK_ATTRIBUTE_UNUSED = alpha[index];
    CCTK_REAL alpharhsL CCTK_ATTRIBUTE_UNUSED = alpharhs[index];
    CCTK_REAL ArhsL CCTK_ATTRIBUTE_UNUSED = Arhs[index];
    CCTK_REAL At11L CCTK_ATTRIBUTE_UNUSED = At11[index];
    CCTK_REAL At11rhsL CCTK_ATTRIBUTE_UNUSED = At11rhs[index];
    CCTK_REAL At12L CCTK_ATTRIBUTE_UNUSED = At12[index];
    CCTK_REAL At12rhsL CCTK_ATTRIBUTE_UNUSED = At12rhs[index];
    CCTK_REAL At13L CCTK_ATTRIBUTE_UNUSED = At13[index];
    CCTK_REAL At13rhsL CCTK_ATTRIBUTE_UNUSED = At13rhs[index];
    CCTK_REAL At22L CCTK_ATTRIBUTE_UNUSED = At22[index];
    CCTK_REAL At22rhsL CCTK_ATTRIBUTE_UNUSED = At22rhs[index];
    CCTK_REAL At23L CCTK_ATTRIBUTE_UNUSED = At23[index];
    CCTK_REAL At23rhsL CCTK_ATTRIBUTE_UNUSED = At23rhs[index];
    CCTK_REAL At33L CCTK_ATTRIBUTE_UNUSED = At33[index];
    CCTK_REAL At33rhsL CCTK_ATTRIBUTE_UNUSED = At33rhs[index];
    CCTK_REAL B1L CCTK_ATTRIBUTE_UNUSED = B1[index];
    CCTK_REAL B1rhsL CCTK_ATTRIBUTE_UNUSED = B1rhs[index];
    CCTK_REAL B2L CCTK_ATTRIBUTE_UNUSED = B2[index];
    CCTK_REAL B2rhsL CCTK_ATTRIBUTE_UNUSED = B2rhs[index];
    CCTK_REAL B3L CCTK_ATTRIBUTE_UNUSED = B3[index];
    CCTK_REAL B3rhsL CCTK_ATTRIBUTE_UNUSED = B3rhs[index];
    CCTK_REAL beta1L CCTK_ATTRIBUTE_UNUSED = beta1[index];
    CCTK_REAL beta1rhsL CCTK_ATTRIBUTE_UNUSED = beta1rhs[index];
    CCTK_REAL beta2L CCTK_ATTRIBUTE_UNUSED = beta2[index];
    CCTK_REAL beta2rhsL CCTK_ATTRIBUTE_UNUSED = beta2rhs[index];
    CCTK_REAL beta3L CCTK_ATTRIBUTE_UNUSED = beta3[index];
    CCTK_REAL beta3rhsL CCTK_ATTRIBUTE_UNUSED = beta3rhs[index];
    CCTK_REAL gt11L CCTK_ATTRIBUTE_UNUSED = gt11[index];
    CCTK_REAL gt11rhsL CCTK_ATTRIBUTE_UNUSED = gt11rhs[index];
    CCTK_REAL gt12L CCTK_ATTRIBUTE_UNUSED = gt12[index];
    CCTK_REAL gt12rhsL CCTK_ATTRIBUTE_UNUSED = gt12rhs[index];
    CCTK_REAL gt13L CCTK_ATTRIBUTE_UNUSED = gt13[index];
    CCTK_REAL gt13rhsL CCTK_ATTRIBUTE_UNUSED = gt13rhs[index];
    CCTK_REAL gt22L CCTK_ATTRIBUTE_UNUSED = gt22[index];
    CCTK_REAL gt22rhsL CCTK_ATTRIBUTE_UNUSED = gt22rhs[index];
    CCTK_REAL gt23L CCTK_ATTRIBUTE_UNUSED = gt23[index];
    CCTK_REAL gt23rhsL CCTK_ATTRIBUTE_UNUSED = gt23rhs[index];
    CCTK_REAL gt33L CCTK_ATTRIBUTE_UNUSED = gt33[index];
    CCTK_REAL gt33rhsL CCTK_ATTRIBUTE_UNUSED = gt33rhs[index];
    CCTK_REAL phiL CCTK_ATTRIBUTE_UNUSED = phi[index];
    CCTK_REAL phirhsL CCTK_ATTRIBUTE_UNUSED = phirhs[index];
    CCTK_REAL trKL CCTK_ATTRIBUTE_UNUSED = trK[index];
    CCTK_REAL trKrhsL CCTK_ATTRIBUTE_UNUSED = trKrhs[index];
    CCTK_REAL Xt1L CCTK_ATTRIBUTE_UNUSED = Xt1[index];
    CCTK_REAL Xt1rhsL CCTK_ATTRIBUTE_UNUSED = Xt1rhs[index];
    CCTK_REAL Xt2L CCTK_ATTRIBUTE_UNUSED = Xt2[index];
    CCTK_REAL Xt2rhsL CCTK_ATTRIBUTE_UNUSED = Xt2rhs[index];
    CCTK_REAL Xt3L CCTK_ATTRIBUTE_UNUSED = Xt3[index];
    CCTK_REAL Xt3rhsL CCTK_ATTRIBUTE_UNUSED = Xt3rhs[index];
    
    
    /* Include user supplied include files */
    
    /* Precompute derivatives */
    CCTK_REAL PDupwindNthAnti1A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti1Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm1Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti2Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm2Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthAnti3Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDupwindNthSymm3Xt3 CCTK_ATTRIBUTE_UNUSED;
    
    switch (fdOrder)
    {
      case 2:
      {
        PDupwindNthAnti1A = PDupwindNthAntifdOrder21(&A[index]);
        PDupwindNthSymm1A = PDupwindNthSymmfdOrder21(&A[index]);
        PDupwindNthAnti2A = PDupwindNthAntifdOrder22(&A[index]);
        PDupwindNthSymm2A = PDupwindNthSymmfdOrder22(&A[index]);
        PDupwindNthAnti3A = PDupwindNthAntifdOrder23(&A[index]);
        PDupwindNthSymm3A = PDupwindNthSymmfdOrder23(&A[index]);
        PDupwindNthAnti1alpha = PDupwindNthAntifdOrder21(&alpha[index]);
        PDupwindNthSymm1alpha = PDupwindNthSymmfdOrder21(&alpha[index]);
        PDupwindNthAnti2alpha = PDupwindNthAntifdOrder22(&alpha[index]);
        PDupwindNthSymm2alpha = PDupwindNthSymmfdOrder22(&alpha[index]);
        PDupwindNthAnti3alpha = PDupwindNthAntifdOrder23(&alpha[index]);
        PDupwindNthSymm3alpha = PDupwindNthSymmfdOrder23(&alpha[index]);
        PDupwindNthAnti1At11 = PDupwindNthAntifdOrder21(&At11[index]);
        PDupwindNthSymm1At11 = PDupwindNthSymmfdOrder21(&At11[index]);
        PDupwindNthAnti2At11 = PDupwindNthAntifdOrder22(&At11[index]);
        PDupwindNthSymm2At11 = PDupwindNthSymmfdOrder22(&At11[index]);
        PDupwindNthAnti3At11 = PDupwindNthAntifdOrder23(&At11[index]);
        PDupwindNthSymm3At11 = PDupwindNthSymmfdOrder23(&At11[index]);
        PDupwindNthAnti1At12 = PDupwindNthAntifdOrder21(&At12[index]);
        PDupwindNthSymm1At12 = PDupwindNthSymmfdOrder21(&At12[index]);
        PDupwindNthAnti2At12 = PDupwindNthAntifdOrder22(&At12[index]);
        PDupwindNthSymm2At12 = PDupwindNthSymmfdOrder22(&At12[index]);
        PDupwindNthAnti3At12 = PDupwindNthAntifdOrder23(&At12[index]);
        PDupwindNthSymm3At12 = PDupwindNthSymmfdOrder23(&At12[index]);
        PDupwindNthAnti1At13 = PDupwindNthAntifdOrder21(&At13[index]);
        PDupwindNthSymm1At13 = PDupwindNthSymmfdOrder21(&At13[index]);
        PDupwindNthAnti2At13 = PDupwindNthAntifdOrder22(&At13[index]);
        PDupwindNthSymm2At13 = PDupwindNthSymmfdOrder22(&At13[index]);
        PDupwindNthAnti3At13 = PDupwindNthAntifdOrder23(&At13[index]);
        PDupwindNthSymm3At13 = PDupwindNthSymmfdOrder23(&At13[index]);
        PDupwindNthAnti1At22 = PDupwindNthAntifdOrder21(&At22[index]);
        PDupwindNthSymm1At22 = PDupwindNthSymmfdOrder21(&At22[index]);
        PDupwindNthAnti2At22 = PDupwindNthAntifdOrder22(&At22[index]);
        PDupwindNthSymm2At22 = PDupwindNthSymmfdOrder22(&At22[index]);
        PDupwindNthAnti3At22 = PDupwindNthAntifdOrder23(&At22[index]);
        PDupwindNthSymm3At22 = PDupwindNthSymmfdOrder23(&At22[index]);
        PDupwindNthAnti1At23 = PDupwindNthAntifdOrder21(&At23[index]);
        PDupwindNthSymm1At23 = PDupwindNthSymmfdOrder21(&At23[index]);
        PDupwindNthAnti2At23 = PDupwindNthAntifdOrder22(&At23[index]);
        PDupwindNthSymm2At23 = PDupwindNthSymmfdOrder22(&At23[index]);
        PDupwindNthAnti3At23 = PDupwindNthAntifdOrder23(&At23[index]);
        PDupwindNthSymm3At23 = PDupwindNthSymmfdOrder23(&At23[index]);
        PDupwindNthAnti1At33 = PDupwindNthAntifdOrder21(&At33[index]);
        PDupwindNthSymm1At33 = PDupwindNthSymmfdOrder21(&At33[index]);
        PDupwindNthAnti2At33 = PDupwindNthAntifdOrder22(&At33[index]);
        PDupwindNthSymm2At33 = PDupwindNthSymmfdOrder22(&At33[index]);
        PDupwindNthAnti3At33 = PDupwindNthAntifdOrder23(&At33[index]);
        PDupwindNthSymm3At33 = PDupwindNthSymmfdOrder23(&At33[index]);
        PDupwindNthAnti1B1 = PDupwindNthAntifdOrder21(&B1[index]);
        PDupwindNthSymm1B1 = PDupwindNthSymmfdOrder21(&B1[index]);
        PDupwindNthAnti2B1 = PDupwindNthAntifdOrder22(&B1[index]);
        PDupwindNthSymm2B1 = PDupwindNthSymmfdOrder22(&B1[index]);
        PDupwindNthAnti3B1 = PDupwindNthAntifdOrder23(&B1[index]);
        PDupwindNthSymm3B1 = PDupwindNthSymmfdOrder23(&B1[index]);
        PDupwindNthAnti1B2 = PDupwindNthAntifdOrder21(&B2[index]);
        PDupwindNthSymm1B2 = PDupwindNthSymmfdOrder21(&B2[index]);
        PDupwindNthAnti2B2 = PDupwindNthAntifdOrder22(&B2[index]);
        PDupwindNthSymm2B2 = PDupwindNthSymmfdOrder22(&B2[index]);
        PDupwindNthAnti3B2 = PDupwindNthAntifdOrder23(&B2[index]);
        PDupwindNthSymm3B2 = PDupwindNthSymmfdOrder23(&B2[index]);
        PDupwindNthAnti1B3 = PDupwindNthAntifdOrder21(&B3[index]);
        PDupwindNthSymm1B3 = PDupwindNthSymmfdOrder21(&B3[index]);
        PDupwindNthAnti2B3 = PDupwindNthAntifdOrder22(&B3[index]);
        PDupwindNthSymm2B3 = PDupwindNthSymmfdOrder22(&B3[index]);
        PDupwindNthAnti3B3 = PDupwindNthAntifdOrder23(&B3[index]);
        PDupwindNthSymm3B3 = PDupwindNthSymmfdOrder23(&B3[index]);
        PDupwindNthAnti1beta1 = PDupwindNthAntifdOrder21(&beta1[index]);
        PDupwindNthSymm1beta1 = PDupwindNthSymmfdOrder21(&beta1[index]);
        PDupwindNthAnti2beta1 = PDupwindNthAntifdOrder22(&beta1[index]);
        PDupwindNthSymm2beta1 = PDupwindNthSymmfdOrder22(&beta1[index]);
        PDupwindNthAnti3beta1 = PDupwindNthAntifdOrder23(&beta1[index]);
        PDupwindNthSymm3beta1 = PDupwindNthSymmfdOrder23(&beta1[index]);
        PDupwindNthAnti1beta2 = PDupwindNthAntifdOrder21(&beta2[index]);
        PDupwindNthSymm1beta2 = PDupwindNthSymmfdOrder21(&beta2[index]);
        PDupwindNthAnti2beta2 = PDupwindNthAntifdOrder22(&beta2[index]);
        PDupwindNthSymm2beta2 = PDupwindNthSymmfdOrder22(&beta2[index]);
        PDupwindNthAnti3beta2 = PDupwindNthAntifdOrder23(&beta2[index]);
        PDupwindNthSymm3beta2 = PDupwindNthSymmfdOrder23(&beta2[index]);
        PDupwindNthAnti1beta3 = PDupwindNthAntifdOrder21(&beta3[index]);
        PDupwindNthSymm1beta3 = PDupwindNthSymmfdOrder21(&beta3[index]);
        PDupwindNthAnti2beta3 = PDupwindNthAntifdOrder22(&beta3[index]);
        PDupwindNthSymm2beta3 = PDupwindNthSymmfdOrder22(&beta3[index]);
        PDupwindNthAnti3beta3 = PDupwindNthAntifdOrder23(&beta3[index]);
        PDupwindNthSymm3beta3 = PDupwindNthSymmfdOrder23(&beta3[index]);
        PDupwindNthAnti1gt11 = PDupwindNthAntifdOrder21(&gt11[index]);
        PDupwindNthSymm1gt11 = PDupwindNthSymmfdOrder21(&gt11[index]);
        PDupwindNthAnti2gt11 = PDupwindNthAntifdOrder22(&gt11[index]);
        PDupwindNthSymm2gt11 = PDupwindNthSymmfdOrder22(&gt11[index]);
        PDupwindNthAnti3gt11 = PDupwindNthAntifdOrder23(&gt11[index]);
        PDupwindNthSymm3gt11 = PDupwindNthSymmfdOrder23(&gt11[index]);
        PDupwindNthAnti1gt12 = PDupwindNthAntifdOrder21(&gt12[index]);
        PDupwindNthSymm1gt12 = PDupwindNthSymmfdOrder21(&gt12[index]);
        PDupwindNthAnti2gt12 = PDupwindNthAntifdOrder22(&gt12[index]);
        PDupwindNthSymm2gt12 = PDupwindNthSymmfdOrder22(&gt12[index]);
        PDupwindNthAnti3gt12 = PDupwindNthAntifdOrder23(&gt12[index]);
        PDupwindNthSymm3gt12 = PDupwindNthSymmfdOrder23(&gt12[index]);
        PDupwindNthAnti1gt13 = PDupwindNthAntifdOrder21(&gt13[index]);
        PDupwindNthSymm1gt13 = PDupwindNthSymmfdOrder21(&gt13[index]);
        PDupwindNthAnti2gt13 = PDupwindNthAntifdOrder22(&gt13[index]);
        PDupwindNthSymm2gt13 = PDupwindNthSymmfdOrder22(&gt13[index]);
        PDupwindNthAnti3gt13 = PDupwindNthAntifdOrder23(&gt13[index]);
        PDupwindNthSymm3gt13 = PDupwindNthSymmfdOrder23(&gt13[index]);
        PDupwindNthAnti1gt22 = PDupwindNthAntifdOrder21(&gt22[index]);
        PDupwindNthSymm1gt22 = PDupwindNthSymmfdOrder21(&gt22[index]);
        PDupwindNthAnti2gt22 = PDupwindNthAntifdOrder22(&gt22[index]);
        PDupwindNthSymm2gt22 = PDupwindNthSymmfdOrder22(&gt22[index]);
        PDupwindNthAnti3gt22 = PDupwindNthAntifdOrder23(&gt22[index]);
        PDupwindNthSymm3gt22 = PDupwindNthSymmfdOrder23(&gt22[index]);
        PDupwindNthAnti1gt23 = PDupwindNthAntifdOrder21(&gt23[index]);
        PDupwindNthSymm1gt23 = PDupwindNthSymmfdOrder21(&gt23[index]);
        PDupwindNthAnti2gt23 = PDupwindNthAntifdOrder22(&gt23[index]);
        PDupwindNthSymm2gt23 = PDupwindNthSymmfdOrder22(&gt23[index]);
        PDupwindNthAnti3gt23 = PDupwindNthAntifdOrder23(&gt23[index]);
        PDupwindNthSymm3gt23 = PDupwindNthSymmfdOrder23(&gt23[index]);
        PDupwindNthAnti1gt33 = PDupwindNthAntifdOrder21(&gt33[index]);
        PDupwindNthSymm1gt33 = PDupwindNthSymmfdOrder21(&gt33[index]);
        PDupwindNthAnti2gt33 = PDupwindNthAntifdOrder22(&gt33[index]);
        PDupwindNthSymm2gt33 = PDupwindNthSymmfdOrder22(&gt33[index]);
        PDupwindNthAnti3gt33 = PDupwindNthAntifdOrder23(&gt33[index]);
        PDupwindNthSymm3gt33 = PDupwindNthSymmfdOrder23(&gt33[index]);
        PDupwindNthAnti1phi = PDupwindNthAntifdOrder21(&phi[index]);
        PDupwindNthSymm1phi = PDupwindNthSymmfdOrder21(&phi[index]);
        PDupwindNthAnti2phi = PDupwindNthAntifdOrder22(&phi[index]);
        PDupwindNthSymm2phi = PDupwindNthSymmfdOrder22(&phi[index]);
        PDupwindNthAnti3phi = PDupwindNthAntifdOrder23(&phi[index]);
        PDupwindNthSymm3phi = PDupwindNthSymmfdOrder23(&phi[index]);
        PDupwindNthAnti1trK = PDupwindNthAntifdOrder21(&trK[index]);
        PDupwindNthSymm1trK = PDupwindNthSymmfdOrder21(&trK[index]);
        PDupwindNthAnti2trK = PDupwindNthAntifdOrder22(&trK[index]);
        PDupwindNthSymm2trK = PDupwindNthSymmfdOrder22(&trK[index]);
        PDupwindNthAnti3trK = PDupwindNthAntifdOrder23(&trK[index]);
        PDupwindNthSymm3trK = PDupwindNthSymmfdOrder23(&trK[index]);
        PDupwindNthAnti1Xt1 = PDupwindNthAntifdOrder21(&Xt1[index]);
        PDupwindNthSymm1Xt1 = PDupwindNthSymmfdOrder21(&Xt1[index]);
        PDupwindNthAnti2Xt1 = PDupwindNthAntifdOrder22(&Xt1[index]);
        PDupwindNthSymm2Xt1 = PDupwindNthSymmfdOrder22(&Xt1[index]);
        PDupwindNthAnti3Xt1 = PDupwindNthAntifdOrder23(&Xt1[index]);
        PDupwindNthSymm3Xt1 = PDupwindNthSymmfdOrder23(&Xt1[index]);
        PDupwindNthAnti1Xt2 = PDupwindNthAntifdOrder21(&Xt2[index]);
        PDupwindNthSymm1Xt2 = PDupwindNthSymmfdOrder21(&Xt2[index]);
        PDupwindNthAnti2Xt2 = PDupwindNthAntifdOrder22(&Xt2[index]);
        PDupwindNthSymm2Xt2 = PDupwindNthSymmfdOrder22(&Xt2[index]);
        PDupwindNthAnti3Xt2 = PDupwindNthAntifdOrder23(&Xt2[index]);
        PDupwindNthSymm3Xt2 = PDupwindNthSymmfdOrder23(&Xt2[index]);
        PDupwindNthAnti1Xt3 = PDupwindNthAntifdOrder21(&Xt3[index]);
        PDupwindNthSymm1Xt3 = PDupwindNthSymmfdOrder21(&Xt3[index]);
        PDupwindNthAnti2Xt3 = PDupwindNthAntifdOrder22(&Xt3[index]);
        PDupwindNthSymm2Xt3 = PDupwindNthSymmfdOrder22(&Xt3[index]);
        PDupwindNthAnti3Xt3 = PDupwindNthAntifdOrder23(&Xt3[index]);
        PDupwindNthSymm3Xt3 = PDupwindNthSymmfdOrder23(&Xt3[index]);
        break;
      }
      
      case 4:
      {
        PDupwindNthAnti1A = PDupwindNthAntifdOrder41(&A[index]);
        PDupwindNthSymm1A = PDupwindNthSymmfdOrder41(&A[index]);
        PDupwindNthAnti2A = PDupwindNthAntifdOrder42(&A[index]);
        PDupwindNthSymm2A = PDupwindNthSymmfdOrder42(&A[index]);
        PDupwindNthAnti3A = PDupwindNthAntifdOrder43(&A[index]);
        PDupwindNthSymm3A = PDupwindNthSymmfdOrder43(&A[index]);
        PDupwindNthAnti1alpha = PDupwindNthAntifdOrder41(&alpha[index]);
        PDupwindNthSymm1alpha = PDupwindNthSymmfdOrder41(&alpha[index]);
        PDupwindNthAnti2alpha = PDupwindNthAntifdOrder42(&alpha[index]);
        PDupwindNthSymm2alpha = PDupwindNthSymmfdOrder42(&alpha[index]);
        PDupwindNthAnti3alpha = PDupwindNthAntifdOrder43(&alpha[index]);
        PDupwindNthSymm3alpha = PDupwindNthSymmfdOrder43(&alpha[index]);
        PDupwindNthAnti1At11 = PDupwindNthAntifdOrder41(&At11[index]);
        PDupwindNthSymm1At11 = PDupwindNthSymmfdOrder41(&At11[index]);
        PDupwindNthAnti2At11 = PDupwindNthAntifdOrder42(&At11[index]);
        PDupwindNthSymm2At11 = PDupwindNthSymmfdOrder42(&At11[index]);
        PDupwindNthAnti3At11 = PDupwindNthAntifdOrder43(&At11[index]);
        PDupwindNthSymm3At11 = PDupwindNthSymmfdOrder43(&At11[index]);
        PDupwindNthAnti1At12 = PDupwindNthAntifdOrder41(&At12[index]);
        PDupwindNthSymm1At12 = PDupwindNthSymmfdOrder41(&At12[index]);
        PDupwindNthAnti2At12 = PDupwindNthAntifdOrder42(&At12[index]);
        PDupwindNthSymm2At12 = PDupwindNthSymmfdOrder42(&At12[index]);
        PDupwindNthAnti3At12 = PDupwindNthAntifdOrder43(&At12[index]);
        PDupwindNthSymm3At12 = PDupwindNthSymmfdOrder43(&At12[index]);
        PDupwindNthAnti1At13 = PDupwindNthAntifdOrder41(&At13[index]);
        PDupwindNthSymm1At13 = PDupwindNthSymmfdOrder41(&At13[index]);
        PDupwindNthAnti2At13 = PDupwindNthAntifdOrder42(&At13[index]);
        PDupwindNthSymm2At13 = PDupwindNthSymmfdOrder42(&At13[index]);
        PDupwindNthAnti3At13 = PDupwindNthAntifdOrder43(&At13[index]);
        PDupwindNthSymm3At13 = PDupwindNthSymmfdOrder43(&At13[index]);
        PDupwindNthAnti1At22 = PDupwindNthAntifdOrder41(&At22[index]);
        PDupwindNthSymm1At22 = PDupwindNthSymmfdOrder41(&At22[index]);
        PDupwindNthAnti2At22 = PDupwindNthAntifdOrder42(&At22[index]);
        PDupwindNthSymm2At22 = PDupwindNthSymmfdOrder42(&At22[index]);
        PDupwindNthAnti3At22 = PDupwindNthAntifdOrder43(&At22[index]);
        PDupwindNthSymm3At22 = PDupwindNthSymmfdOrder43(&At22[index]);
        PDupwindNthAnti1At23 = PDupwindNthAntifdOrder41(&At23[index]);
        PDupwindNthSymm1At23 = PDupwindNthSymmfdOrder41(&At23[index]);
        PDupwindNthAnti2At23 = PDupwindNthAntifdOrder42(&At23[index]);
        PDupwindNthSymm2At23 = PDupwindNthSymmfdOrder42(&At23[index]);
        PDupwindNthAnti3At23 = PDupwindNthAntifdOrder43(&At23[index]);
        PDupwindNthSymm3At23 = PDupwindNthSymmfdOrder43(&At23[index]);
        PDupwindNthAnti1At33 = PDupwindNthAntifdOrder41(&At33[index]);
        PDupwindNthSymm1At33 = PDupwindNthSymmfdOrder41(&At33[index]);
        PDupwindNthAnti2At33 = PDupwindNthAntifdOrder42(&At33[index]);
        PDupwindNthSymm2At33 = PDupwindNthSymmfdOrder42(&At33[index]);
        PDupwindNthAnti3At33 = PDupwindNthAntifdOrder43(&At33[index]);
        PDupwindNthSymm3At33 = PDupwindNthSymmfdOrder43(&At33[index]);
        PDupwindNthAnti1B1 = PDupwindNthAntifdOrder41(&B1[index]);
        PDupwindNthSymm1B1 = PDupwindNthSymmfdOrder41(&B1[index]);
        PDupwindNthAnti2B1 = PDupwindNthAntifdOrder42(&B1[index]);
        PDupwindNthSymm2B1 = PDupwindNthSymmfdOrder42(&B1[index]);
        PDupwindNthAnti3B1 = PDupwindNthAntifdOrder43(&B1[index]);
        PDupwindNthSymm3B1 = PDupwindNthSymmfdOrder43(&B1[index]);
        PDupwindNthAnti1B2 = PDupwindNthAntifdOrder41(&B2[index]);
        PDupwindNthSymm1B2 = PDupwindNthSymmfdOrder41(&B2[index]);
        PDupwindNthAnti2B2 = PDupwindNthAntifdOrder42(&B2[index]);
        PDupwindNthSymm2B2 = PDupwindNthSymmfdOrder42(&B2[index]);
        PDupwindNthAnti3B2 = PDupwindNthAntifdOrder43(&B2[index]);
        PDupwindNthSymm3B2 = PDupwindNthSymmfdOrder43(&B2[index]);
        PDupwindNthAnti1B3 = PDupwindNthAntifdOrder41(&B3[index]);
        PDupwindNthSymm1B3 = PDupwindNthSymmfdOrder41(&B3[index]);
        PDupwindNthAnti2B3 = PDupwindNthAntifdOrder42(&B3[index]);
        PDupwindNthSymm2B3 = PDupwindNthSymmfdOrder42(&B3[index]);
        PDupwindNthAnti3B3 = PDupwindNthAntifdOrder43(&B3[index]);
        PDupwindNthSymm3B3 = PDupwindNthSymmfdOrder43(&B3[index]);
        PDupwindNthAnti1beta1 = PDupwindNthAntifdOrder41(&beta1[index]);
        PDupwindNthSymm1beta1 = PDupwindNthSymmfdOrder41(&beta1[index]);
        PDupwindNthAnti2beta1 = PDupwindNthAntifdOrder42(&beta1[index]);
        PDupwindNthSymm2beta1 = PDupwindNthSymmfdOrder42(&beta1[index]);
        PDupwindNthAnti3beta1 = PDupwindNthAntifdOrder43(&beta1[index]);
        PDupwindNthSymm3beta1 = PDupwindNthSymmfdOrder43(&beta1[index]);
        PDupwindNthAnti1beta2 = PDupwindNthAntifdOrder41(&beta2[index]);
        PDupwindNthSymm1beta2 = PDupwindNthSymmfdOrder41(&beta2[index]);
        PDupwindNthAnti2beta2 = PDupwindNthAntifdOrder42(&beta2[index]);
        PDupwindNthSymm2beta2 = PDupwindNthSymmfdOrder42(&beta2[index]);
        PDupwindNthAnti3beta2 = PDupwindNthAntifdOrder43(&beta2[index]);
        PDupwindNthSymm3beta2 = PDupwindNthSymmfdOrder43(&beta2[index]);
        PDupwindNthAnti1beta3 = PDupwindNthAntifdOrder41(&beta3[index]);
        PDupwindNthSymm1beta3 = PDupwindNthSymmfdOrder41(&beta3[index]);
        PDupwindNthAnti2beta3 = PDupwindNthAntifdOrder42(&beta3[index]);
        PDupwindNthSymm2beta3 = PDupwindNthSymmfdOrder42(&beta3[index]);
        PDupwindNthAnti3beta3 = PDupwindNthAntifdOrder43(&beta3[index]);
        PDupwindNthSymm3beta3 = PDupwindNthSymmfdOrder43(&beta3[index]);
        PDupwindNthAnti1gt11 = PDupwindNthAntifdOrder41(&gt11[index]);
        PDupwindNthSymm1gt11 = PDupwindNthSymmfdOrder41(&gt11[index]);
        PDupwindNthAnti2gt11 = PDupwindNthAntifdOrder42(&gt11[index]);
        PDupwindNthSymm2gt11 = PDupwindNthSymmfdOrder42(&gt11[index]);
        PDupwindNthAnti3gt11 = PDupwindNthAntifdOrder43(&gt11[index]);
        PDupwindNthSymm3gt11 = PDupwindNthSymmfdOrder43(&gt11[index]);
        PDupwindNthAnti1gt12 = PDupwindNthAntifdOrder41(&gt12[index]);
        PDupwindNthSymm1gt12 = PDupwindNthSymmfdOrder41(&gt12[index]);
        PDupwindNthAnti2gt12 = PDupwindNthAntifdOrder42(&gt12[index]);
        PDupwindNthSymm2gt12 = PDupwindNthSymmfdOrder42(&gt12[index]);
        PDupwindNthAnti3gt12 = PDupwindNthAntifdOrder43(&gt12[index]);
        PDupwindNthSymm3gt12 = PDupwindNthSymmfdOrder43(&gt12[index]);
        PDupwindNthAnti1gt13 = PDupwindNthAntifdOrder41(&gt13[index]);
        PDupwindNthSymm1gt13 = PDupwindNthSymmfdOrder41(&gt13[index]);
        PDupwindNthAnti2gt13 = PDupwindNthAntifdOrder42(&gt13[index]);
        PDupwindNthSymm2gt13 = PDupwindNthSymmfdOrder42(&gt13[index]);
        PDupwindNthAnti3gt13 = PDupwindNthAntifdOrder43(&gt13[index]);
        PDupwindNthSymm3gt13 = PDupwindNthSymmfdOrder43(&gt13[index]);
        PDupwindNthAnti1gt22 = PDupwindNthAntifdOrder41(&gt22[index]);
        PDupwindNthSymm1gt22 = PDupwindNthSymmfdOrder41(&gt22[index]);
        PDupwindNthAnti2gt22 = PDupwindNthAntifdOrder42(&gt22[index]);
        PDupwindNthSymm2gt22 = PDupwindNthSymmfdOrder42(&gt22[index]);
        PDupwindNthAnti3gt22 = PDupwindNthAntifdOrder43(&gt22[index]);
        PDupwindNthSymm3gt22 = PDupwindNthSymmfdOrder43(&gt22[index]);
        PDupwindNthAnti1gt23 = PDupwindNthAntifdOrder41(&gt23[index]);
        PDupwindNthSymm1gt23 = PDupwindNthSymmfdOrder41(&gt23[index]);
        PDupwindNthAnti2gt23 = PDupwindNthAntifdOrder42(&gt23[index]);
        PDupwindNthSymm2gt23 = PDupwindNthSymmfdOrder42(&gt23[index]);
        PDupwindNthAnti3gt23 = PDupwindNthAntifdOrder43(&gt23[index]);
        PDupwindNthSymm3gt23 = PDupwindNthSymmfdOrder43(&gt23[index]);
        PDupwindNthAnti1gt33 = PDupwindNthAntifdOrder41(&gt33[index]);
        PDupwindNthSymm1gt33 = PDupwindNthSymmfdOrder41(&gt33[index]);
        PDupwindNthAnti2gt33 = PDupwindNthAntifdOrder42(&gt33[index]);
        PDupwindNthSymm2gt33 = PDupwindNthSymmfdOrder42(&gt33[index]);
        PDupwindNthAnti3gt33 = PDupwindNthAntifdOrder43(&gt33[index]);
        PDupwindNthSymm3gt33 = PDupwindNthSymmfdOrder43(&gt33[index]);
        PDupwindNthAnti1phi = PDupwindNthAntifdOrder41(&phi[index]);
        PDupwindNthSymm1phi = PDupwindNthSymmfdOrder41(&phi[index]);
        PDupwindNthAnti2phi = PDupwindNthAntifdOrder42(&phi[index]);
        PDupwindNthSymm2phi = PDupwindNthSymmfdOrder42(&phi[index]);
        PDupwindNthAnti3phi = PDupwindNthAntifdOrder43(&phi[index]);
        PDupwindNthSymm3phi = PDupwindNthSymmfdOrder43(&phi[index]);
        PDupwindNthAnti1trK = PDupwindNthAntifdOrder41(&trK[index]);
        PDupwindNthSymm1trK = PDupwindNthSymmfdOrder41(&trK[index]);
        PDupwindNthAnti2trK = PDupwindNthAntifdOrder42(&trK[index]);
        PDupwindNthSymm2trK = PDupwindNthSymmfdOrder42(&trK[index]);
        PDupwindNthAnti3trK = PDupwindNthAntifdOrder43(&trK[index]);
        PDupwindNthSymm3trK = PDupwindNthSymmfdOrder43(&trK[index]);
        PDupwindNthAnti1Xt1 = PDupwindNthAntifdOrder41(&Xt1[index]);
        PDupwindNthSymm1Xt1 = PDupwindNthSymmfdOrder41(&Xt1[index]);
        PDupwindNthAnti2Xt1 = PDupwindNthAntifdOrder42(&Xt1[index]);
        PDupwindNthSymm2Xt1 = PDupwindNthSymmfdOrder42(&Xt1[index]);
        PDupwindNthAnti3Xt1 = PDupwindNthAntifdOrder43(&Xt1[index]);
        PDupwindNthSymm3Xt1 = PDupwindNthSymmfdOrder43(&Xt1[index]);
        PDupwindNthAnti1Xt2 = PDupwindNthAntifdOrder41(&Xt2[index]);
        PDupwindNthSymm1Xt2 = PDupwindNthSymmfdOrder41(&Xt2[index]);
        PDupwindNthAnti2Xt2 = PDupwindNthAntifdOrder42(&Xt2[index]);
        PDupwindNthSymm2Xt2 = PDupwindNthSymmfdOrder42(&Xt2[index]);
        PDupwindNthAnti3Xt2 = PDupwindNthAntifdOrder43(&Xt2[index]);
        PDupwindNthSymm3Xt2 = PDupwindNthSymmfdOrder43(&Xt2[index]);
        PDupwindNthAnti1Xt3 = PDupwindNthAntifdOrder41(&Xt3[index]);
        PDupwindNthSymm1Xt3 = PDupwindNthSymmfdOrder41(&Xt3[index]);
        PDupwindNthAnti2Xt3 = PDupwindNthAntifdOrder42(&Xt3[index]);
        PDupwindNthSymm2Xt3 = PDupwindNthSymmfdOrder42(&Xt3[index]);
        PDupwindNthAnti3Xt3 = PDupwindNthAntifdOrder43(&Xt3[index]);
        PDupwindNthSymm3Xt3 = PDupwindNthSymmfdOrder43(&Xt3[index]);
        break;
      }
      
      case 6:
      {
        PDupwindNthAnti1A = PDupwindNthAntifdOrder61(&A[index]);
        PDupwindNthSymm1A = PDupwindNthSymmfdOrder61(&A[index]);
        PDupwindNthAnti2A = PDupwindNthAntifdOrder62(&A[index]);
        PDupwindNthSymm2A = PDupwindNthSymmfdOrder62(&A[index]);
        PDupwindNthAnti3A = PDupwindNthAntifdOrder63(&A[index]);
        PDupwindNthSymm3A = PDupwindNthSymmfdOrder63(&A[index]);
        PDupwindNthAnti1alpha = PDupwindNthAntifdOrder61(&alpha[index]);
        PDupwindNthSymm1alpha = PDupwindNthSymmfdOrder61(&alpha[index]);
        PDupwindNthAnti2alpha = PDupwindNthAntifdOrder62(&alpha[index]);
        PDupwindNthSymm2alpha = PDupwindNthSymmfdOrder62(&alpha[index]);
        PDupwindNthAnti3alpha = PDupwindNthAntifdOrder63(&alpha[index]);
        PDupwindNthSymm3alpha = PDupwindNthSymmfdOrder63(&alpha[index]);
        PDupwindNthAnti1At11 = PDupwindNthAntifdOrder61(&At11[index]);
        PDupwindNthSymm1At11 = PDupwindNthSymmfdOrder61(&At11[index]);
        PDupwindNthAnti2At11 = PDupwindNthAntifdOrder62(&At11[index]);
        PDupwindNthSymm2At11 = PDupwindNthSymmfdOrder62(&At11[index]);
        PDupwindNthAnti3At11 = PDupwindNthAntifdOrder63(&At11[index]);
        PDupwindNthSymm3At11 = PDupwindNthSymmfdOrder63(&At11[index]);
        PDupwindNthAnti1At12 = PDupwindNthAntifdOrder61(&At12[index]);
        PDupwindNthSymm1At12 = PDupwindNthSymmfdOrder61(&At12[index]);
        PDupwindNthAnti2At12 = PDupwindNthAntifdOrder62(&At12[index]);
        PDupwindNthSymm2At12 = PDupwindNthSymmfdOrder62(&At12[index]);
        PDupwindNthAnti3At12 = PDupwindNthAntifdOrder63(&At12[index]);
        PDupwindNthSymm3At12 = PDupwindNthSymmfdOrder63(&At12[index]);
        PDupwindNthAnti1At13 = PDupwindNthAntifdOrder61(&At13[index]);
        PDupwindNthSymm1At13 = PDupwindNthSymmfdOrder61(&At13[index]);
        PDupwindNthAnti2At13 = PDupwindNthAntifdOrder62(&At13[index]);
        PDupwindNthSymm2At13 = PDupwindNthSymmfdOrder62(&At13[index]);
        PDupwindNthAnti3At13 = PDupwindNthAntifdOrder63(&At13[index]);
        PDupwindNthSymm3At13 = PDupwindNthSymmfdOrder63(&At13[index]);
        PDupwindNthAnti1At22 = PDupwindNthAntifdOrder61(&At22[index]);
        PDupwindNthSymm1At22 = PDupwindNthSymmfdOrder61(&At22[index]);
        PDupwindNthAnti2At22 = PDupwindNthAntifdOrder62(&At22[index]);
        PDupwindNthSymm2At22 = PDupwindNthSymmfdOrder62(&At22[index]);
        PDupwindNthAnti3At22 = PDupwindNthAntifdOrder63(&At22[index]);
        PDupwindNthSymm3At22 = PDupwindNthSymmfdOrder63(&At22[index]);
        PDupwindNthAnti1At23 = PDupwindNthAntifdOrder61(&At23[index]);
        PDupwindNthSymm1At23 = PDupwindNthSymmfdOrder61(&At23[index]);
        PDupwindNthAnti2At23 = PDupwindNthAntifdOrder62(&At23[index]);
        PDupwindNthSymm2At23 = PDupwindNthSymmfdOrder62(&At23[index]);
        PDupwindNthAnti3At23 = PDupwindNthAntifdOrder63(&At23[index]);
        PDupwindNthSymm3At23 = PDupwindNthSymmfdOrder63(&At23[index]);
        PDupwindNthAnti1At33 = PDupwindNthAntifdOrder61(&At33[index]);
        PDupwindNthSymm1At33 = PDupwindNthSymmfdOrder61(&At33[index]);
        PDupwindNthAnti2At33 = PDupwindNthAntifdOrder62(&At33[index]);
        PDupwindNthSymm2At33 = PDupwindNthSymmfdOrder62(&At33[index]);
        PDupwindNthAnti3At33 = PDupwindNthAntifdOrder63(&At33[index]);
        PDupwindNthSymm3At33 = PDupwindNthSymmfdOrder63(&At33[index]);
        PDupwindNthAnti1B1 = PDupwindNthAntifdOrder61(&B1[index]);
        PDupwindNthSymm1B1 = PDupwindNthSymmfdOrder61(&B1[index]);
        PDupwindNthAnti2B1 = PDupwindNthAntifdOrder62(&B1[index]);
        PDupwindNthSymm2B1 = PDupwindNthSymmfdOrder62(&B1[index]);
        PDupwindNthAnti3B1 = PDupwindNthAntifdOrder63(&B1[index]);
        PDupwindNthSymm3B1 = PDupwindNthSymmfdOrder63(&B1[index]);
        PDupwindNthAnti1B2 = PDupwindNthAntifdOrder61(&B2[index]);
        PDupwindNthSymm1B2 = PDupwindNthSymmfdOrder61(&B2[index]);
        PDupwindNthAnti2B2 = PDupwindNthAntifdOrder62(&B2[index]);
        PDupwindNthSymm2B2 = PDupwindNthSymmfdOrder62(&B2[index]);
        PDupwindNthAnti3B2 = PDupwindNthAntifdOrder63(&B2[index]);
        PDupwindNthSymm3B2 = PDupwindNthSymmfdOrder63(&B2[index]);
        PDupwindNthAnti1B3 = PDupwindNthAntifdOrder61(&B3[index]);
        PDupwindNthSymm1B3 = PDupwindNthSymmfdOrder61(&B3[index]);
        PDupwindNthAnti2B3 = PDupwindNthAntifdOrder62(&B3[index]);
        PDupwindNthSymm2B3 = PDupwindNthSymmfdOrder62(&B3[index]);
        PDupwindNthAnti3B3 = PDupwindNthAntifdOrder63(&B3[index]);
        PDupwindNthSymm3B3 = PDupwindNthSymmfdOrder63(&B3[index]);
        PDupwindNthAnti1beta1 = PDupwindNthAntifdOrder61(&beta1[index]);
        PDupwindNthSymm1beta1 = PDupwindNthSymmfdOrder61(&beta1[index]);
        PDupwindNthAnti2beta1 = PDupwindNthAntifdOrder62(&beta1[index]);
        PDupwindNthSymm2beta1 = PDupwindNthSymmfdOrder62(&beta1[index]);
        PDupwindNthAnti3beta1 = PDupwindNthAntifdOrder63(&beta1[index]);
        PDupwindNthSymm3beta1 = PDupwindNthSymmfdOrder63(&beta1[index]);
        PDupwindNthAnti1beta2 = PDupwindNthAntifdOrder61(&beta2[index]);
        PDupwindNthSymm1beta2 = PDupwindNthSymmfdOrder61(&beta2[index]);
        PDupwindNthAnti2beta2 = PDupwindNthAntifdOrder62(&beta2[index]);
        PDupwindNthSymm2beta2 = PDupwindNthSymmfdOrder62(&beta2[index]);
        PDupwindNthAnti3beta2 = PDupwindNthAntifdOrder63(&beta2[index]);
        PDupwindNthSymm3beta2 = PDupwindNthSymmfdOrder63(&beta2[index]);
        PDupwindNthAnti1beta3 = PDupwindNthAntifdOrder61(&beta3[index]);
        PDupwindNthSymm1beta3 = PDupwindNthSymmfdOrder61(&beta3[index]);
        PDupwindNthAnti2beta3 = PDupwindNthAntifdOrder62(&beta3[index]);
        PDupwindNthSymm2beta3 = PDupwindNthSymmfdOrder62(&beta3[index]);
        PDupwindNthAnti3beta3 = PDupwindNthAntifdOrder63(&beta3[index]);
        PDupwindNthSymm3beta3 = PDupwindNthSymmfdOrder63(&beta3[index]);
        PDupwindNthAnti1gt11 = PDupwindNthAntifdOrder61(&gt11[index]);
        PDupwindNthSymm1gt11 = PDupwindNthSymmfdOrder61(&gt11[index]);
        PDupwindNthAnti2gt11 = PDupwindNthAntifdOrder62(&gt11[index]);
        PDupwindNthSymm2gt11 = PDupwindNthSymmfdOrder62(&gt11[index]);
        PDupwindNthAnti3gt11 = PDupwindNthAntifdOrder63(&gt11[index]);
        PDupwindNthSymm3gt11 = PDupwindNthSymmfdOrder63(&gt11[index]);
        PDupwindNthAnti1gt12 = PDupwindNthAntifdOrder61(&gt12[index]);
        PDupwindNthSymm1gt12 = PDupwindNthSymmfdOrder61(&gt12[index]);
        PDupwindNthAnti2gt12 = PDupwindNthAntifdOrder62(&gt12[index]);
        PDupwindNthSymm2gt12 = PDupwindNthSymmfdOrder62(&gt12[index]);
        PDupwindNthAnti3gt12 = PDupwindNthAntifdOrder63(&gt12[index]);
        PDupwindNthSymm3gt12 = PDupwindNthSymmfdOrder63(&gt12[index]);
        PDupwindNthAnti1gt13 = PDupwindNthAntifdOrder61(&gt13[index]);
        PDupwindNthSymm1gt13 = PDupwindNthSymmfdOrder61(&gt13[index]);
        PDupwindNthAnti2gt13 = PDupwindNthAntifdOrder62(&gt13[index]);
        PDupwindNthSymm2gt13 = PDupwindNthSymmfdOrder62(&gt13[index]);
        PDupwindNthAnti3gt13 = PDupwindNthAntifdOrder63(&gt13[index]);
        PDupwindNthSymm3gt13 = PDupwindNthSymmfdOrder63(&gt13[index]);
        PDupwindNthAnti1gt22 = PDupwindNthAntifdOrder61(&gt22[index]);
        PDupwindNthSymm1gt22 = PDupwindNthSymmfdOrder61(&gt22[index]);
        PDupwindNthAnti2gt22 = PDupwindNthAntifdOrder62(&gt22[index]);
        PDupwindNthSymm2gt22 = PDupwindNthSymmfdOrder62(&gt22[index]);
        PDupwindNthAnti3gt22 = PDupwindNthAntifdOrder63(&gt22[index]);
        PDupwindNthSymm3gt22 = PDupwindNthSymmfdOrder63(&gt22[index]);
        PDupwindNthAnti1gt23 = PDupwindNthAntifdOrder61(&gt23[index]);
        PDupwindNthSymm1gt23 = PDupwindNthSymmfdOrder61(&gt23[index]);
        PDupwindNthAnti2gt23 = PDupwindNthAntifdOrder62(&gt23[index]);
        PDupwindNthSymm2gt23 = PDupwindNthSymmfdOrder62(&gt23[index]);
        PDupwindNthAnti3gt23 = PDupwindNthAntifdOrder63(&gt23[index]);
        PDupwindNthSymm3gt23 = PDupwindNthSymmfdOrder63(&gt23[index]);
        PDupwindNthAnti1gt33 = PDupwindNthAntifdOrder61(&gt33[index]);
        PDupwindNthSymm1gt33 = PDupwindNthSymmfdOrder61(&gt33[index]);
        PDupwindNthAnti2gt33 = PDupwindNthAntifdOrder62(&gt33[index]);
        PDupwindNthSymm2gt33 = PDupwindNthSymmfdOrder62(&gt33[index]);
        PDupwindNthAnti3gt33 = PDupwindNthAntifdOrder63(&gt33[index]);
        PDupwindNthSymm3gt33 = PDupwindNthSymmfdOrder63(&gt33[index]);
        PDupwindNthAnti1phi = PDupwindNthAntifdOrder61(&phi[index]);
        PDupwindNthSymm1phi = PDupwindNthSymmfdOrder61(&phi[index]);
        PDupwindNthAnti2phi = PDupwindNthAntifdOrder62(&phi[index]);
        PDupwindNthSymm2phi = PDupwindNthSymmfdOrder62(&phi[index]);
        PDupwindNthAnti3phi = PDupwindNthAntifdOrder63(&phi[index]);
        PDupwindNthSymm3phi = PDupwindNthSymmfdOrder63(&phi[index]);
        PDupwindNthAnti1trK = PDupwindNthAntifdOrder61(&trK[index]);
        PDupwindNthSymm1trK = PDupwindNthSymmfdOrder61(&trK[index]);
        PDupwindNthAnti2trK = PDupwindNthAntifdOrder62(&trK[index]);
        PDupwindNthSymm2trK = PDupwindNthSymmfdOrder62(&trK[index]);
        PDupwindNthAnti3trK = PDupwindNthAntifdOrder63(&trK[index]);
        PDupwindNthSymm3trK = PDupwindNthSymmfdOrder63(&trK[index]);
        PDupwindNthAnti1Xt1 = PDupwindNthAntifdOrder61(&Xt1[index]);
        PDupwindNthSymm1Xt1 = PDupwindNthSymmfdOrder61(&Xt1[index]);
        PDupwindNthAnti2Xt1 = PDupwindNthAntifdOrder62(&Xt1[index]);
        PDupwindNthSymm2Xt1 = PDupwindNthSymmfdOrder62(&Xt1[index]);
        PDupwindNthAnti3Xt1 = PDupwindNthAntifdOrder63(&Xt1[index]);
        PDupwindNthSymm3Xt1 = PDupwindNthSymmfdOrder63(&Xt1[index]);
        PDupwindNthAnti1Xt2 = PDupwindNthAntifdOrder61(&Xt2[index]);
        PDupwindNthSymm1Xt2 = PDupwindNthSymmfdOrder61(&Xt2[index]);
        PDupwindNthAnti2Xt2 = PDupwindNthAntifdOrder62(&Xt2[index]);
        PDupwindNthSymm2Xt2 = PDupwindNthSymmfdOrder62(&Xt2[index]);
        PDupwindNthAnti3Xt2 = PDupwindNthAntifdOrder63(&Xt2[index]);
        PDupwindNthSymm3Xt2 = PDupwindNthSymmfdOrder63(&Xt2[index]);
        PDupwindNthAnti1Xt3 = PDupwindNthAntifdOrder61(&Xt3[index]);
        PDupwindNthSymm1Xt3 = PDupwindNthSymmfdOrder61(&Xt3[index]);
        PDupwindNthAnti2Xt3 = PDupwindNthAntifdOrder62(&Xt3[index]);
        PDupwindNthSymm2Xt3 = PDupwindNthSymmfdOrder62(&Xt3[index]);
        PDupwindNthAnti3Xt3 = PDupwindNthAntifdOrder63(&Xt3[index]);
        PDupwindNthSymm3Xt3 = PDupwindNthSymmfdOrder63(&Xt3[index]);
        break;
      }
      
      case 8:
      {
        PDupwindNthAnti1A = PDupwindNthAntifdOrder81(&A[index]);
        PDupwindNthSymm1A = PDupwindNthSymmfdOrder81(&A[index]);
        PDupwindNthAnti2A = PDupwindNthAntifdOrder82(&A[index]);
        PDupwindNthSymm2A = PDupwindNthSymmfdOrder82(&A[index]);
        PDupwindNthAnti3A = PDupwindNthAntifdOrder83(&A[index]);
        PDupwindNthSymm3A = PDupwindNthSymmfdOrder83(&A[index]);
        PDupwindNthAnti1alpha = PDupwindNthAntifdOrder81(&alpha[index]);
        PDupwindNthSymm1alpha = PDupwindNthSymmfdOrder81(&alpha[index]);
        PDupwindNthAnti2alpha = PDupwindNthAntifdOrder82(&alpha[index]);
        PDupwindNthSymm2alpha = PDupwindNthSymmfdOrder82(&alpha[index]);
        PDupwindNthAnti3alpha = PDupwindNthAntifdOrder83(&alpha[index]);
        PDupwindNthSymm3alpha = PDupwindNthSymmfdOrder83(&alpha[index]);
        PDupwindNthAnti1At11 = PDupwindNthAntifdOrder81(&At11[index]);
        PDupwindNthSymm1At11 = PDupwindNthSymmfdOrder81(&At11[index]);
        PDupwindNthAnti2At11 = PDupwindNthAntifdOrder82(&At11[index]);
        PDupwindNthSymm2At11 = PDupwindNthSymmfdOrder82(&At11[index]);
        PDupwindNthAnti3At11 = PDupwindNthAntifdOrder83(&At11[index]);
        PDupwindNthSymm3At11 = PDupwindNthSymmfdOrder83(&At11[index]);
        PDupwindNthAnti1At12 = PDupwindNthAntifdOrder81(&At12[index]);
        PDupwindNthSymm1At12 = PDupwindNthSymmfdOrder81(&At12[index]);
        PDupwindNthAnti2At12 = PDupwindNthAntifdOrder82(&At12[index]);
        PDupwindNthSymm2At12 = PDupwindNthSymmfdOrder82(&At12[index]);
        PDupwindNthAnti3At12 = PDupwindNthAntifdOrder83(&At12[index]);
        PDupwindNthSymm3At12 = PDupwindNthSymmfdOrder83(&At12[index]);
        PDupwindNthAnti1At13 = PDupwindNthAntifdOrder81(&At13[index]);
        PDupwindNthSymm1At13 = PDupwindNthSymmfdOrder81(&At13[index]);
        PDupwindNthAnti2At13 = PDupwindNthAntifdOrder82(&At13[index]);
        PDupwindNthSymm2At13 = PDupwindNthSymmfdOrder82(&At13[index]);
        PDupwindNthAnti3At13 = PDupwindNthAntifdOrder83(&At13[index]);
        PDupwindNthSymm3At13 = PDupwindNthSymmfdOrder83(&At13[index]);
        PDupwindNthAnti1At22 = PDupwindNthAntifdOrder81(&At22[index]);
        PDupwindNthSymm1At22 = PDupwindNthSymmfdOrder81(&At22[index]);
        PDupwindNthAnti2At22 = PDupwindNthAntifdOrder82(&At22[index]);
        PDupwindNthSymm2At22 = PDupwindNthSymmfdOrder82(&At22[index]);
        PDupwindNthAnti3At22 = PDupwindNthAntifdOrder83(&At22[index]);
        PDupwindNthSymm3At22 = PDupwindNthSymmfdOrder83(&At22[index]);
        PDupwindNthAnti1At23 = PDupwindNthAntifdOrder81(&At23[index]);
        PDupwindNthSymm1At23 = PDupwindNthSymmfdOrder81(&At23[index]);
        PDupwindNthAnti2At23 = PDupwindNthAntifdOrder82(&At23[index]);
        PDupwindNthSymm2At23 = PDupwindNthSymmfdOrder82(&At23[index]);
        PDupwindNthAnti3At23 = PDupwindNthAntifdOrder83(&At23[index]);
        PDupwindNthSymm3At23 = PDupwindNthSymmfdOrder83(&At23[index]);
        PDupwindNthAnti1At33 = PDupwindNthAntifdOrder81(&At33[index]);
        PDupwindNthSymm1At33 = PDupwindNthSymmfdOrder81(&At33[index]);
        PDupwindNthAnti2At33 = PDupwindNthAntifdOrder82(&At33[index]);
        PDupwindNthSymm2At33 = PDupwindNthSymmfdOrder82(&At33[index]);
        PDupwindNthAnti3At33 = PDupwindNthAntifdOrder83(&At33[index]);
        PDupwindNthSymm3At33 = PDupwindNthSymmfdOrder83(&At33[index]);
        PDupwindNthAnti1B1 = PDupwindNthAntifdOrder81(&B1[index]);
        PDupwindNthSymm1B1 = PDupwindNthSymmfdOrder81(&B1[index]);
        PDupwindNthAnti2B1 = PDupwindNthAntifdOrder82(&B1[index]);
        PDupwindNthSymm2B1 = PDupwindNthSymmfdOrder82(&B1[index]);
        PDupwindNthAnti3B1 = PDupwindNthAntifdOrder83(&B1[index]);
        PDupwindNthSymm3B1 = PDupwindNthSymmfdOrder83(&B1[index]);
        PDupwindNthAnti1B2 = PDupwindNthAntifdOrder81(&B2[index]);
        PDupwindNthSymm1B2 = PDupwindNthSymmfdOrder81(&B2[index]);
        PDupwindNthAnti2B2 = PDupwindNthAntifdOrder82(&B2[index]);
        PDupwindNthSymm2B2 = PDupwindNthSymmfdOrder82(&B2[index]);
        PDupwindNthAnti3B2 = PDupwindNthAntifdOrder83(&B2[index]);
        PDupwindNthSymm3B2 = PDupwindNthSymmfdOrder83(&B2[index]);
        PDupwindNthAnti1B3 = PDupwindNthAntifdOrder81(&B3[index]);
        PDupwindNthSymm1B3 = PDupwindNthSymmfdOrder81(&B3[index]);
        PDupwindNthAnti2B3 = PDupwindNthAntifdOrder82(&B3[index]);
        PDupwindNthSymm2B3 = PDupwindNthSymmfdOrder82(&B3[index]);
        PDupwindNthAnti3B3 = PDupwindNthAntifdOrder83(&B3[index]);
        PDupwindNthSymm3B3 = PDupwindNthSymmfdOrder83(&B3[index]);
        PDupwindNthAnti1beta1 = PDupwindNthAntifdOrder81(&beta1[index]);
        PDupwindNthSymm1beta1 = PDupwindNthSymmfdOrder81(&beta1[index]);
        PDupwindNthAnti2beta1 = PDupwindNthAntifdOrder82(&beta1[index]);
        PDupwindNthSymm2beta1 = PDupwindNthSymmfdOrder82(&beta1[index]);
        PDupwindNthAnti3beta1 = PDupwindNthAntifdOrder83(&beta1[index]);
        PDupwindNthSymm3beta1 = PDupwindNthSymmfdOrder83(&beta1[index]);
        PDupwindNthAnti1beta2 = PDupwindNthAntifdOrder81(&beta2[index]);
        PDupwindNthSymm1beta2 = PDupwindNthSymmfdOrder81(&beta2[index]);
        PDupwindNthAnti2beta2 = PDupwindNthAntifdOrder82(&beta2[index]);
        PDupwindNthSymm2beta2 = PDupwindNthSymmfdOrder82(&beta2[index]);
        PDupwindNthAnti3beta2 = PDupwindNthAntifdOrder83(&beta2[index]);
        PDupwindNthSymm3beta2 = PDupwindNthSymmfdOrder83(&beta2[index]);
        PDupwindNthAnti1beta3 = PDupwindNthAntifdOrder81(&beta3[index]);
        PDupwindNthSymm1beta3 = PDupwindNthSymmfdOrder81(&beta3[index]);
        PDupwindNthAnti2beta3 = PDupwindNthAntifdOrder82(&beta3[index]);
        PDupwindNthSymm2beta3 = PDupwindNthSymmfdOrder82(&beta3[index]);
        PDupwindNthAnti3beta3 = PDupwindNthAntifdOrder83(&beta3[index]);
        PDupwindNthSymm3beta3 = PDupwindNthSymmfdOrder83(&beta3[index]);
        PDupwindNthAnti1gt11 = PDupwindNthAntifdOrder81(&gt11[index]);
        PDupwindNthSymm1gt11 = PDupwindNthSymmfdOrder81(&gt11[index]);
        PDupwindNthAnti2gt11 = PDupwindNthAntifdOrder82(&gt11[index]);
        PDupwindNthSymm2gt11 = PDupwindNthSymmfdOrder82(&gt11[index]);
        PDupwindNthAnti3gt11 = PDupwindNthAntifdOrder83(&gt11[index]);
        PDupwindNthSymm3gt11 = PDupwindNthSymmfdOrder83(&gt11[index]);
        PDupwindNthAnti1gt12 = PDupwindNthAntifdOrder81(&gt12[index]);
        PDupwindNthSymm1gt12 = PDupwindNthSymmfdOrder81(&gt12[index]);
        PDupwindNthAnti2gt12 = PDupwindNthAntifdOrder82(&gt12[index]);
        PDupwindNthSymm2gt12 = PDupwindNthSymmfdOrder82(&gt12[index]);
        PDupwindNthAnti3gt12 = PDupwindNthAntifdOrder83(&gt12[index]);
        PDupwindNthSymm3gt12 = PDupwindNthSymmfdOrder83(&gt12[index]);
        PDupwindNthAnti1gt13 = PDupwindNthAntifdOrder81(&gt13[index]);
        PDupwindNthSymm1gt13 = PDupwindNthSymmfdOrder81(&gt13[index]);
        PDupwindNthAnti2gt13 = PDupwindNthAntifdOrder82(&gt13[index]);
        PDupwindNthSymm2gt13 = PDupwindNthSymmfdOrder82(&gt13[index]);
        PDupwindNthAnti3gt13 = PDupwindNthAntifdOrder83(&gt13[index]);
        PDupwindNthSymm3gt13 = PDupwindNthSymmfdOrder83(&gt13[index]);
        PDupwindNthAnti1gt22 = PDupwindNthAntifdOrder81(&gt22[index]);
        PDupwindNthSymm1gt22 = PDupwindNthSymmfdOrder81(&gt22[index]);
        PDupwindNthAnti2gt22 = PDupwindNthAntifdOrder82(&gt22[index]);
        PDupwindNthSymm2gt22 = PDupwindNthSymmfdOrder82(&gt22[index]);
        PDupwindNthAnti3gt22 = PDupwindNthAntifdOrder83(&gt22[index]);
        PDupwindNthSymm3gt22 = PDupwindNthSymmfdOrder83(&gt22[index]);
        PDupwindNthAnti1gt23 = PDupwindNthAntifdOrder81(&gt23[index]);
        PDupwindNthSymm1gt23 = PDupwindNthSymmfdOrder81(&gt23[index]);
        PDupwindNthAnti2gt23 = PDupwindNthAntifdOrder82(&gt23[index]);
        PDupwindNthSymm2gt23 = PDupwindNthSymmfdOrder82(&gt23[index]);
        PDupwindNthAnti3gt23 = PDupwindNthAntifdOrder83(&gt23[index]);
        PDupwindNthSymm3gt23 = PDupwindNthSymmfdOrder83(&gt23[index]);
        PDupwindNthAnti1gt33 = PDupwindNthAntifdOrder81(&gt33[index]);
        PDupwindNthSymm1gt33 = PDupwindNthSymmfdOrder81(&gt33[index]);
        PDupwindNthAnti2gt33 = PDupwindNthAntifdOrder82(&gt33[index]);
        PDupwindNthSymm2gt33 = PDupwindNthSymmfdOrder82(&gt33[index]);
        PDupwindNthAnti3gt33 = PDupwindNthAntifdOrder83(&gt33[index]);
        PDupwindNthSymm3gt33 = PDupwindNthSymmfdOrder83(&gt33[index]);
        PDupwindNthAnti1phi = PDupwindNthAntifdOrder81(&phi[index]);
        PDupwindNthSymm1phi = PDupwindNthSymmfdOrder81(&phi[index]);
        PDupwindNthAnti2phi = PDupwindNthAntifdOrder82(&phi[index]);
        PDupwindNthSymm2phi = PDupwindNthSymmfdOrder82(&phi[index]);
        PDupwindNthAnti3phi = PDupwindNthAntifdOrder83(&phi[index]);
        PDupwindNthSymm3phi = PDupwindNthSymmfdOrder83(&phi[index]);
        PDupwindNthAnti1trK = PDupwindNthAntifdOrder81(&trK[index]);
        PDupwindNthSymm1trK = PDupwindNthSymmfdOrder81(&trK[index]);
        PDupwindNthAnti2trK = PDupwindNthAntifdOrder82(&trK[index]);
        PDupwindNthSymm2trK = PDupwindNthSymmfdOrder82(&trK[index]);
        PDupwindNthAnti3trK = PDupwindNthAntifdOrder83(&trK[index]);
        PDupwindNthSymm3trK = PDupwindNthSymmfdOrder83(&trK[index]);
        PDupwindNthAnti1Xt1 = PDupwindNthAntifdOrder81(&Xt1[index]);
        PDupwindNthSymm1Xt1 = PDupwindNthSymmfdOrder81(&Xt1[index]);
        PDupwindNthAnti2Xt1 = PDupwindNthAntifdOrder82(&Xt1[index]);
        PDupwindNthSymm2Xt1 = PDupwindNthSymmfdOrder82(&Xt1[index]);
        PDupwindNthAnti3Xt1 = PDupwindNthAntifdOrder83(&Xt1[index]);
        PDupwindNthSymm3Xt1 = PDupwindNthSymmfdOrder83(&Xt1[index]);
        PDupwindNthAnti1Xt2 = PDupwindNthAntifdOrder81(&Xt2[index]);
        PDupwindNthSymm1Xt2 = PDupwindNthSymmfdOrder81(&Xt2[index]);
        PDupwindNthAnti2Xt2 = PDupwindNthAntifdOrder82(&Xt2[index]);
        PDupwindNthSymm2Xt2 = PDupwindNthSymmfdOrder82(&Xt2[index]);
        PDupwindNthAnti3Xt2 = PDupwindNthAntifdOrder83(&Xt2[index]);
        PDupwindNthSymm3Xt2 = PDupwindNthSymmfdOrder83(&Xt2[index]);
        PDupwindNthAnti1Xt3 = PDupwindNthAntifdOrder81(&Xt3[index]);
        PDupwindNthSymm1Xt3 = PDupwindNthSymmfdOrder81(&Xt3[index]);
        PDupwindNthAnti2Xt3 = PDupwindNthAntifdOrder82(&Xt3[index]);
        PDupwindNthSymm2Xt3 = PDupwindNthSymmfdOrder82(&Xt3[index]);
        PDupwindNthAnti3Xt3 = PDupwindNthAntifdOrder83(&Xt3[index]);
        PDupwindNthSymm3Xt3 = PDupwindNthSymmfdOrder83(&Xt3[index]);
        break;
      }
      default:
        CCTK_BUILTIN_UNREACHABLE();
    }
    
    /* Calculate temporaries and grid functions */
    ptrdiff_t dir1 CCTK_ATTRIBUTE_UNUSED = isgn(beta1L);
    
    ptrdiff_t dir2 CCTK_ATTRIBUTE_UNUSED = isgn(beta2L);
    
    ptrdiff_t dir3 CCTK_ATTRIBUTE_UNUSED = isgn(beta3L);
    
    phirhsL = phirhsL + beta1L*PDupwindNthAnti1phi + 
      beta2L*PDupwindNthAnti2phi + beta3L*PDupwindNthAnti3phi + 
      PDupwindNthSymm1phi*fabs(beta1L) + PDupwindNthSymm2phi*fabs(beta2L) + 
      PDupwindNthSymm3phi*fabs(beta3L);
    
    gt11rhsL = gt11rhsL + beta1L*PDupwindNthAnti1gt11 + 
      beta2L*PDupwindNthAnti2gt11 + beta3L*PDupwindNthAnti3gt11 + 
      PDupwindNthSymm1gt11*fabs(beta1L) + PDupwindNthSymm2gt11*fabs(beta2L) + 
      PDupwindNthSymm3gt11*fabs(beta3L);
    
    gt12rhsL = gt12rhsL + beta1L*PDupwindNthAnti1gt12 + 
      beta2L*PDupwindNthAnti2gt12 + beta3L*PDupwindNthAnti3gt12 + 
      PDupwindNthSymm1gt12*fabs(beta1L) + PDupwindNthSymm2gt12*fabs(beta2L) + 
      PDupwindNthSymm3gt12*fabs(beta3L);
    
    gt13rhsL = gt13rhsL + beta1L*PDupwindNthAnti1gt13 + 
      beta2L*PDupwindNthAnti2gt13 + beta3L*PDupwindNthAnti3gt13 + 
      PDupwindNthSymm1gt13*fabs(beta1L) + PDupwindNthSymm2gt13*fabs(beta2L) + 
      PDupwindNthSymm3gt13*fabs(beta3L);
    
    gt22rhsL = gt22rhsL + beta1L*PDupwindNthAnti1gt22 + 
      beta2L*PDupwindNthAnti2gt22 + beta3L*PDupwindNthAnti3gt22 + 
      PDupwindNthSymm1gt22*fabs(beta1L) + PDupwindNthSymm2gt22*fabs(beta2L) + 
      PDupwindNthSymm3gt22*fabs(beta3L);
    
    gt23rhsL = gt23rhsL + beta1L*PDupwindNthAnti1gt23 + 
      beta2L*PDupwindNthAnti2gt23 + beta3L*PDupwindNthAnti3gt23 + 
      PDupwindNthSymm1gt23*fabs(beta1L) + PDupwindNthSymm2gt23*fabs(beta2L) + 
      PDupwindNthSymm3gt23*fabs(beta3L);
    
    gt33rhsL = gt33rhsL + beta1L*PDupwindNthAnti1gt33 + 
      beta2L*PDupwindNthAnti2gt33 + beta3L*PDupwindNthAnti3gt33 + 
      PDupwindNthSymm1gt33*fabs(beta1L) + PDupwindNthSymm2gt33*fabs(beta2L) + 
      PDupwindNthSymm3gt33*fabs(beta3L);
    
    Xt1rhsL = Xt1rhsL + beta1L*PDupwindNthAnti1Xt1 + 
      beta2L*PDupwindNthAnti2Xt1 + beta3L*PDupwindNthAnti3Xt1 + 
      PDupwindNthSymm1Xt1*fabs(beta1L) + PDupwindNthSymm2Xt1*fabs(beta2L) + 
      PDupwindNthSymm3Xt1*fabs(beta3L);
    
    Xt2rhsL = Xt2rhsL + beta1L*PDupwindNthAnti1Xt2 + 
      beta2L*PDupwindNthAnti2Xt2 + beta3L*PDupwindNthAnti3Xt2 + 
      PDupwindNthSymm1Xt2*fabs(beta1L) + PDupwindNthSymm2Xt2*fabs(beta2L) + 
      PDupwindNthSymm3Xt2*fabs(beta3L);
    
    Xt3rhsL = Xt3rhsL + beta1L*PDupwindNthAnti1Xt3 + 
      beta2L*PDupwindNthAnti2Xt3 + beta3L*PDupwindNthAnti3Xt3 + 
      PDupwindNthSymm1Xt3*fabs(beta1L) + PDupwindNthSymm2Xt3*fabs(beta2L) + 
      PDupwindNthSymm3Xt3*fabs(beta3L);
    
    trKrhsL = trKrhsL + beta1L*PDupwindNthAnti1trK + 
      beta2L*PDupwindNthAnti2trK + beta3L*PDupwindNthAnti3trK + 
      PDupwindNthSymm1trK*fabs(beta1L) + PDupwindNthSymm2trK*fabs(beta2L) + 
      PDupwindNthSymm3trK*fabs(beta3L);
    
    At11rhsL = At11rhsL + beta1L*PDupwindNthAnti1At11 + 
      beta2L*PDupwindNthAnti2At11 + beta3L*PDupwindNthAnti3At11 + 
      PDupwindNthSymm1At11*fabs(beta1L) + PDupwindNthSymm2At11*fabs(beta2L) + 
      PDupwindNthSymm3At11*fabs(beta3L);
    
    At12rhsL = At12rhsL + beta1L*PDupwindNthAnti1At12 + 
      beta2L*PDupwindNthAnti2At12 + beta3L*PDupwindNthAnti3At12 + 
      PDupwindNthSymm1At12*fabs(beta1L) + PDupwindNthSymm2At12*fabs(beta2L) + 
      PDupwindNthSymm3At12*fabs(beta3L);
    
    At13rhsL = At13rhsL + beta1L*PDupwindNthAnti1At13 + 
      beta2L*PDupwindNthAnti2At13 + beta3L*PDupwindNthAnti3At13 + 
      PDupwindNthSymm1At13*fabs(beta1L) + PDupwindNthSymm2At13*fabs(beta2L) + 
      PDupwindNthSymm3At13*fabs(beta3L);
    
    At22rhsL = At22rhsL + beta1L*PDupwindNthAnti1At22 + 
      beta2L*PDupwindNthAnti2At22 + beta3L*PDupwindNthAnti3At22 + 
      PDupwindNthSymm1At22*fabs(beta1L) + PDupwindNthSymm2At22*fabs(beta2L) + 
      PDupwindNthSymm3At22*fabs(beta3L);
    
    At23rhsL = At23rhsL + beta1L*PDupwindNthAnti1At23 + 
      beta2L*PDupwindNthAnti2At23 + beta3L*PDupwindNthAnti3At23 + 
      PDupwindNthSymm1At23*fabs(beta1L) + PDupwindNthSymm2At23*fabs(beta2L) + 
      PDupwindNthSymm3At23*fabs(beta3L);
    
    At33rhsL = At33rhsL + beta1L*PDupwindNthAnti1At33 + 
      beta2L*PDupwindNthAnti2At33 + beta3L*PDupwindNthAnti3At33 + 
      PDupwindNthSymm1At33*fabs(beta1L) + PDupwindNthSymm2At33*fabs(beta2L) + 
      PDupwindNthSymm3At33*fabs(beta3L);
    
    alpharhsL = alpharhsL + (beta1L*PDupwindNthAnti1alpha + 
      beta2L*PDupwindNthAnti2alpha + beta3L*PDupwindNthAnti3alpha + 
      PDupwindNthSymm1alpha*fabs(beta1L) + PDupwindNthSymm2alpha*fabs(beta2L) 
      + PDupwindNthSymm3alpha*fabs(beta3L))*ToReal(LapseAdvectionCoeff);
    
    ArhsL = ArhsL + ToReal(LapseACoeff)*(-((beta1L*PDupwindNthAnti1trK + 
      beta2L*PDupwindNthAnti2trK + beta3L*PDupwindNthAnti3trK + 
      PDupwindNthSymm1trK*fabs(beta1L) + PDupwindNthSymm2trK*fabs(beta2L) + 
      PDupwindNthSymm3trK*fabs(beta3L))*(-1 + ToReal(LapseAdvectionCoeff))) + 
      (beta1L*PDupwindNthAnti1A + beta2L*PDupwindNthAnti2A + 
      beta3L*PDupwindNthAnti3A + PDupwindNthSymm1A*fabs(beta1L) + 
      PDupwindNthSymm2A*fabs(beta2L) + 
      PDupwindNthSymm3A*fabs(beta3L))*ToReal(LapseAdvectionCoeff));
    
    beta1rhsL = beta1rhsL + (beta1L*PDupwindNthAnti1beta1 + 
      beta2L*PDupwindNthAnti2beta1 + beta3L*PDupwindNthAnti3beta1 + 
      PDupwindNthSymm1beta1*fabs(beta1L) + PDupwindNthSymm2beta1*fabs(beta2L) 
      + PDupwindNthSymm3beta1*fabs(beta3L))*ToReal(ShiftAdvectionCoeff);
    
    beta2rhsL = beta2rhsL + (beta1L*PDupwindNthAnti1beta2 + 
      beta2L*PDupwindNthAnti2beta2 + beta3L*PDupwindNthAnti3beta2 + 
      PDupwindNthSymm1beta2*fabs(beta1L) + PDupwindNthSymm2beta2*fabs(beta2L) 
      + PDupwindNthSymm3beta2*fabs(beta3L))*ToReal(ShiftAdvectionCoeff);
    
    beta3rhsL = beta3rhsL + (beta1L*PDupwindNthAnti1beta3 + 
      beta2L*PDupwindNthAnti2beta3 + beta3L*PDupwindNthAnti3beta3 + 
      PDupwindNthSymm1beta3*fabs(beta1L) + PDupwindNthSymm2beta3*fabs(beta2L) 
      + PDupwindNthSymm3beta3*fabs(beta3L))*ToReal(ShiftAdvectionCoeff);
    
    B1rhsL = B1rhsL + (-((beta1L*PDupwindNthAnti1Xt1 + 
      beta2L*PDupwindNthAnti2Xt1 + beta3L*PDupwindNthAnti3Xt1 + 
      PDupwindNthSymm1Xt1*fabs(beta1L) + PDupwindNthSymm2Xt1*fabs(beta2L) + 
      PDupwindNthSymm3Xt1*fabs(beta3L))*(-1 + ToReal(ShiftAdvectionCoeff))) + 
      (beta1L*PDupwindNthAnti1B1 + beta2L*PDupwindNthAnti2B1 + 
      beta3L*PDupwindNthAnti3B1 + PDupwindNthSymm1B1*fabs(beta1L) + 
      PDupwindNthSymm2B1*fabs(beta2L) + 
      PDupwindNthSymm3B1*fabs(beta3L))*ToReal(ShiftAdvectionCoeff))*ToReal(ShiftBCoeff);
    
    B2rhsL = B2rhsL + (-((beta1L*PDupwindNthAnti1Xt2 + 
      beta2L*PDupwindNthAnti2Xt2 + beta3L*PDupwindNthAnti3Xt2 + 
      PDupwindNthSymm1Xt2*fabs(beta1L) + PDupwindNthSymm2Xt2*fabs(beta2L) + 
      PDupwindNthSymm3Xt2*fabs(beta3L))*(-1 + ToReal(ShiftAdvectionCoeff))) + 
      (beta1L*PDupwindNthAnti1B2 + beta2L*PDupwindNthAnti2B2 + 
      beta3L*PDupwindNthAnti3B2 + PDupwindNthSymm1B2*fabs(beta1L) + 
      PDupwindNthSymm2B2*fabs(beta2L) + 
      PDupwindNthSymm3B2*fabs(beta3L))*ToReal(ShiftAdvectionCoeff))*ToReal(ShiftBCoeff);
    
    B3rhsL = B3rhsL + (-((beta1L*PDupwindNthAnti1Xt3 + 
      beta2L*PDupwindNthAnti2Xt3 + beta3L*PDupwindNthAnti3Xt3 + 
      PDupwindNthSymm1Xt3*fabs(beta1L) + PDupwindNthSymm2Xt3*fabs(beta2L) + 
      PDupwindNthSymm3Xt3*fabs(beta3L))*(-1 + ToReal(ShiftAdvectionCoeff))) + 
      (beta1L*PDupwindNthAnti1B3 + beta2L*PDupwindNthAnti2B3 + 
      beta3L*PDupwindNthAnti3B3 + PDupwindNthSymm1B3*fabs(beta1L) + 
      PDupwindNthSymm2B3*fabs(beta2L) + 
      PDupwindNthSymm3B3*fabs(beta3L))*ToReal(ShiftAdvectionCoeff))*ToReal(ShiftBCoeff);
    
    /* Copy local copies back to grid functions */
    alpharhs[index] = alpharhsL;
    Arhs[index] = ArhsL;
    At11rhs[index] = At11rhsL;
    At12rhs[index] = At12rhsL;
    At13rhs[index] = At13rhsL;
    At22rhs[index] = At22rhsL;
    At23rhs[index] = At23rhsL;
    At33rhs[index] = At33rhsL;
    B1rhs[index] = B1rhsL;
    B2rhs[index] = B2rhsL;
    B3rhs[index] = B3rhsL;
    beta1rhs[index] = beta1rhsL;
    beta2rhs[index] = beta2rhsL;
    beta3rhs[index] = beta3rhsL;
    gt11rhs[index] = gt11rhsL;
    gt12rhs[index] = gt12rhsL;
    gt13rhs[index] = gt13rhsL;
    gt22rhs[index] = gt22rhsL;
    gt23rhs[index] = gt23rhsL;
    gt33rhs[index] = gt33rhsL;
    phirhs[index] = phirhsL;
    trKrhs[index] = trKrhsL;
    Xt1rhs[index] = Xt1rhsL;
    Xt2rhs[index] = Xt2rhsL;
    Xt3rhs[index] = Xt3rhsL;
  }
  CCTK_ENDLOOP3(ML_BSSN_Advect);
}

extern "C" void ML_BSSN_Advect(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Entering ML_BSSN_Advect_Body");
  }
  
  if (cctk_iteration % ML_BSSN_Advect_calc_every != ML_BSSN_Advect_calc_offset)
  {
    return;
  }
  
  const char* const groups[] = {
    "ML_BSSN::ML_curv",
    "ML_BSSN::ML_curvrhs",
    "ML_BSSN::ML_dtlapse",
    "ML_BSSN::ML_dtlapserhs",
    "ML_BSSN::ML_dtshift",
    "ML_BSSN::ML_dtshiftrhs",
    "ML_BSSN::ML_Gamma",
    "ML_BSSN::ML_Gammarhs",
    "ML_BSSN::ML_lapse",
    "ML_BSSN::ML_lapserhs",
    "ML_BSSN::ML_log_confac",
    "ML_BSSN::ML_log_confacrhs",
    "ML_BSSN::ML_metric",
    "ML_BSSN::ML_metricrhs",
    "ML_BSSN::ML_shift",
    "ML_BSSN::ML_shiftrhs",
    "ML_BSSN::ML_trace_curv",
    "ML_BSSN::ML_trace_curvrhs"};
  GenericFD_AssertGroupStorage(cctkGH, "ML_BSSN_Advect", 18, groups);
  
  switch (fdOrder)
  {
    case 2:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Advect", 2, 2, 2);
      break;
    }
    
    case 4:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Advect", 3, 3, 3);
      break;
    }
    
    case 6:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Advect", 4, 4, 4);
      break;
    }
    
    case 8:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Advect", 5, 5, 5);
      break;
    }
    default:
      CCTK_BUILTIN_UNREACHABLE();
  }
  
  GenericFD_LoopOverInterior(cctkGH, ML_BSSN_Advect_Body);
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Leaving ML_BSSN_Advect_Body");
  }
}
