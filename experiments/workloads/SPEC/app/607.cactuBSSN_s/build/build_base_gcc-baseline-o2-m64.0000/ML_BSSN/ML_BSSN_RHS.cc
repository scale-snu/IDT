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

extern "C" void ML_BSSN_RHS_SelectBCs(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  if (cctk_iteration % ML_BSSN_RHS_calc_every != ML_BSSN_RHS_calc_offset)
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

static void ML_BSSN_RHS_Body(const cGH* restrict const cctkGH, const int dir, const int face, const CCTK_REAL normal[3], const CCTK_REAL tangentA[3], const CCTK_REAL tangentB[3], const int imin[3], const int imax[3], const int n_subblock_gfs, CCTK_REAL* restrict const subblock_gfs[])
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
  CCTK_LOOP3(ML_BSSN_RHS,
    i,j,k, imin0,imin1,imin2, imax0,imax1,imax2,
    cctk_ash[0],cctk_ash[1],cctk_ash[2])
  {
    const ptrdiff_t index CCTK_ATTRIBUTE_UNUSED = di*i + dj*j + dk*k;
    // ++vec_iter_counter;
    
    /* Assign local copies of grid functions */
    
    CCTK_REAL AL CCTK_ATTRIBUTE_UNUSED = A[index];
    CCTK_REAL alphaL CCTK_ATTRIBUTE_UNUSED = alpha[index];
    CCTK_REAL At11L CCTK_ATTRIBUTE_UNUSED = At11[index];
    CCTK_REAL At12L CCTK_ATTRIBUTE_UNUSED = At12[index];
    CCTK_REAL At13L CCTK_ATTRIBUTE_UNUSED = At13[index];
    CCTK_REAL At22L CCTK_ATTRIBUTE_UNUSED = At22[index];
    CCTK_REAL At23L CCTK_ATTRIBUTE_UNUSED = At23[index];
    CCTK_REAL At33L CCTK_ATTRIBUTE_UNUSED = At33[index];
    CCTK_REAL B1L CCTK_ATTRIBUTE_UNUSED = B1[index];
    CCTK_REAL B2L CCTK_ATTRIBUTE_UNUSED = B2[index];
    CCTK_REAL B3L CCTK_ATTRIBUTE_UNUSED = B3[index];
    CCTK_REAL beta1L CCTK_ATTRIBUTE_UNUSED = beta1[index];
    CCTK_REAL beta2L CCTK_ATTRIBUTE_UNUSED = beta2[index];
    CCTK_REAL beta3L CCTK_ATTRIBUTE_UNUSED = beta3[index];
    CCTK_REAL gt11L CCTK_ATTRIBUTE_UNUSED = gt11[index];
    CCTK_REAL gt12L CCTK_ATTRIBUTE_UNUSED = gt12[index];
    CCTK_REAL gt13L CCTK_ATTRIBUTE_UNUSED = gt13[index];
    CCTK_REAL gt22L CCTK_ATTRIBUTE_UNUSED = gt22[index];
    CCTK_REAL gt23L CCTK_ATTRIBUTE_UNUSED = gt23[index];
    CCTK_REAL gt33L CCTK_ATTRIBUTE_UNUSED = gt33[index];
    CCTK_REAL phiL CCTK_ATTRIBUTE_UNUSED = phi[index];
    CCTK_REAL rL CCTK_ATTRIBUTE_UNUSED = r[index];
    CCTK_REAL trKL CCTK_ATTRIBUTE_UNUSED = trK[index];
    CCTK_REAL Xt1L CCTK_ATTRIBUTE_UNUSED = Xt1[index];
    CCTK_REAL Xt2L CCTK_ATTRIBUTE_UNUSED = Xt2[index];
    CCTK_REAL Xt3L CCTK_ATTRIBUTE_UNUSED = Xt3[index];
    
    
    /* Include user supplied include files */
    
    /* Precompute derivatives */
    CCTK_REAL PDstandardNth1alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth11phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth22phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth33phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth12phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth13phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth23phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3Xt3 CCTK_ATTRIBUTE_UNUSED;
    
    switch (fdOrder)
    {
      case 2:
      {
        PDstandardNth1alpha = PDstandardNthfdOrder21(&alpha[index]);
        PDstandardNth2alpha = PDstandardNthfdOrder22(&alpha[index]);
        PDstandardNth3alpha = PDstandardNthfdOrder23(&alpha[index]);
        PDstandardNth11alpha = PDstandardNthfdOrder211(&alpha[index]);
        PDstandardNth22alpha = PDstandardNthfdOrder222(&alpha[index]);
        PDstandardNth33alpha = PDstandardNthfdOrder233(&alpha[index]);
        PDstandardNth12alpha = PDstandardNthfdOrder212(&alpha[index]);
        PDstandardNth13alpha = PDstandardNthfdOrder213(&alpha[index]);
        PDstandardNth23alpha = PDstandardNthfdOrder223(&alpha[index]);
        PDstandardNth1beta1 = PDstandardNthfdOrder21(&beta1[index]);
        PDstandardNth2beta1 = PDstandardNthfdOrder22(&beta1[index]);
        PDstandardNth3beta1 = PDstandardNthfdOrder23(&beta1[index]);
        PDstandardNth11beta1 = PDstandardNthfdOrder211(&beta1[index]);
        PDstandardNth22beta1 = PDstandardNthfdOrder222(&beta1[index]);
        PDstandardNth33beta1 = PDstandardNthfdOrder233(&beta1[index]);
        PDstandardNth12beta1 = PDstandardNthfdOrder212(&beta1[index]);
        PDstandardNth13beta1 = PDstandardNthfdOrder213(&beta1[index]);
        PDstandardNth23beta1 = PDstandardNthfdOrder223(&beta1[index]);
        PDstandardNth1beta2 = PDstandardNthfdOrder21(&beta2[index]);
        PDstandardNth2beta2 = PDstandardNthfdOrder22(&beta2[index]);
        PDstandardNth3beta2 = PDstandardNthfdOrder23(&beta2[index]);
        PDstandardNth11beta2 = PDstandardNthfdOrder211(&beta2[index]);
        PDstandardNth22beta2 = PDstandardNthfdOrder222(&beta2[index]);
        PDstandardNth33beta2 = PDstandardNthfdOrder233(&beta2[index]);
        PDstandardNth12beta2 = PDstandardNthfdOrder212(&beta2[index]);
        PDstandardNth13beta2 = PDstandardNthfdOrder213(&beta2[index]);
        PDstandardNth23beta2 = PDstandardNthfdOrder223(&beta2[index]);
        PDstandardNth1beta3 = PDstandardNthfdOrder21(&beta3[index]);
        PDstandardNth2beta3 = PDstandardNthfdOrder22(&beta3[index]);
        PDstandardNth3beta3 = PDstandardNthfdOrder23(&beta3[index]);
        PDstandardNth11beta3 = PDstandardNthfdOrder211(&beta3[index]);
        PDstandardNth22beta3 = PDstandardNthfdOrder222(&beta3[index]);
        PDstandardNth33beta3 = PDstandardNthfdOrder233(&beta3[index]);
        PDstandardNth12beta3 = PDstandardNthfdOrder212(&beta3[index]);
        PDstandardNth13beta3 = PDstandardNthfdOrder213(&beta3[index]);
        PDstandardNth23beta3 = PDstandardNthfdOrder223(&beta3[index]);
        PDstandardNth1gt11 = PDstandardNthfdOrder21(&gt11[index]);
        PDstandardNth2gt11 = PDstandardNthfdOrder22(&gt11[index]);
        PDstandardNth3gt11 = PDstandardNthfdOrder23(&gt11[index]);
        PDstandardNth11gt11 = PDstandardNthfdOrder211(&gt11[index]);
        PDstandardNth22gt11 = PDstandardNthfdOrder222(&gt11[index]);
        PDstandardNth33gt11 = PDstandardNthfdOrder233(&gt11[index]);
        PDstandardNth12gt11 = PDstandardNthfdOrder212(&gt11[index]);
        PDstandardNth13gt11 = PDstandardNthfdOrder213(&gt11[index]);
        PDstandardNth23gt11 = PDstandardNthfdOrder223(&gt11[index]);
        PDstandardNth1gt12 = PDstandardNthfdOrder21(&gt12[index]);
        PDstandardNth2gt12 = PDstandardNthfdOrder22(&gt12[index]);
        PDstandardNth3gt12 = PDstandardNthfdOrder23(&gt12[index]);
        PDstandardNth11gt12 = PDstandardNthfdOrder211(&gt12[index]);
        PDstandardNth22gt12 = PDstandardNthfdOrder222(&gt12[index]);
        PDstandardNth33gt12 = PDstandardNthfdOrder233(&gt12[index]);
        PDstandardNth12gt12 = PDstandardNthfdOrder212(&gt12[index]);
        PDstandardNth13gt12 = PDstandardNthfdOrder213(&gt12[index]);
        PDstandardNth23gt12 = PDstandardNthfdOrder223(&gt12[index]);
        PDstandardNth1gt13 = PDstandardNthfdOrder21(&gt13[index]);
        PDstandardNth2gt13 = PDstandardNthfdOrder22(&gt13[index]);
        PDstandardNth3gt13 = PDstandardNthfdOrder23(&gt13[index]);
        PDstandardNth11gt13 = PDstandardNthfdOrder211(&gt13[index]);
        PDstandardNth22gt13 = PDstandardNthfdOrder222(&gt13[index]);
        PDstandardNth33gt13 = PDstandardNthfdOrder233(&gt13[index]);
        PDstandardNth12gt13 = PDstandardNthfdOrder212(&gt13[index]);
        PDstandardNth13gt13 = PDstandardNthfdOrder213(&gt13[index]);
        PDstandardNth23gt13 = PDstandardNthfdOrder223(&gt13[index]);
        PDstandardNth1gt22 = PDstandardNthfdOrder21(&gt22[index]);
        PDstandardNth2gt22 = PDstandardNthfdOrder22(&gt22[index]);
        PDstandardNth3gt22 = PDstandardNthfdOrder23(&gt22[index]);
        PDstandardNth11gt22 = PDstandardNthfdOrder211(&gt22[index]);
        PDstandardNth22gt22 = PDstandardNthfdOrder222(&gt22[index]);
        PDstandardNth33gt22 = PDstandardNthfdOrder233(&gt22[index]);
        PDstandardNth12gt22 = PDstandardNthfdOrder212(&gt22[index]);
        PDstandardNth13gt22 = PDstandardNthfdOrder213(&gt22[index]);
        PDstandardNth23gt22 = PDstandardNthfdOrder223(&gt22[index]);
        PDstandardNth1gt23 = PDstandardNthfdOrder21(&gt23[index]);
        PDstandardNth2gt23 = PDstandardNthfdOrder22(&gt23[index]);
        PDstandardNth3gt23 = PDstandardNthfdOrder23(&gt23[index]);
        PDstandardNth11gt23 = PDstandardNthfdOrder211(&gt23[index]);
        PDstandardNth22gt23 = PDstandardNthfdOrder222(&gt23[index]);
        PDstandardNth33gt23 = PDstandardNthfdOrder233(&gt23[index]);
        PDstandardNth12gt23 = PDstandardNthfdOrder212(&gt23[index]);
        PDstandardNth13gt23 = PDstandardNthfdOrder213(&gt23[index]);
        PDstandardNth23gt23 = PDstandardNthfdOrder223(&gt23[index]);
        PDstandardNth1gt33 = PDstandardNthfdOrder21(&gt33[index]);
        PDstandardNth2gt33 = PDstandardNthfdOrder22(&gt33[index]);
        PDstandardNth3gt33 = PDstandardNthfdOrder23(&gt33[index]);
        PDstandardNth11gt33 = PDstandardNthfdOrder211(&gt33[index]);
        PDstandardNth22gt33 = PDstandardNthfdOrder222(&gt33[index]);
        PDstandardNth33gt33 = PDstandardNthfdOrder233(&gt33[index]);
        PDstandardNth12gt33 = PDstandardNthfdOrder212(&gt33[index]);
        PDstandardNth13gt33 = PDstandardNthfdOrder213(&gt33[index]);
        PDstandardNth23gt33 = PDstandardNthfdOrder223(&gt33[index]);
        PDstandardNth1phi = PDstandardNthfdOrder21(&phi[index]);
        PDstandardNth2phi = PDstandardNthfdOrder22(&phi[index]);
        PDstandardNth3phi = PDstandardNthfdOrder23(&phi[index]);
        PDstandardNth11phi = PDstandardNthfdOrder211(&phi[index]);
        PDstandardNth22phi = PDstandardNthfdOrder222(&phi[index]);
        PDstandardNth33phi = PDstandardNthfdOrder233(&phi[index]);
        PDstandardNth12phi = PDstandardNthfdOrder212(&phi[index]);
        PDstandardNth13phi = PDstandardNthfdOrder213(&phi[index]);
        PDstandardNth23phi = PDstandardNthfdOrder223(&phi[index]);
        PDstandardNth1trK = PDstandardNthfdOrder21(&trK[index]);
        PDstandardNth2trK = PDstandardNthfdOrder22(&trK[index]);
        PDstandardNth3trK = PDstandardNthfdOrder23(&trK[index]);
        PDstandardNth1Xt1 = PDstandardNthfdOrder21(&Xt1[index]);
        PDstandardNth2Xt1 = PDstandardNthfdOrder22(&Xt1[index]);
        PDstandardNth3Xt1 = PDstandardNthfdOrder23(&Xt1[index]);
        PDstandardNth1Xt2 = PDstandardNthfdOrder21(&Xt2[index]);
        PDstandardNth2Xt2 = PDstandardNthfdOrder22(&Xt2[index]);
        PDstandardNth3Xt2 = PDstandardNthfdOrder23(&Xt2[index]);
        PDstandardNth1Xt3 = PDstandardNthfdOrder21(&Xt3[index]);
        PDstandardNth2Xt3 = PDstandardNthfdOrder22(&Xt3[index]);
        PDstandardNth3Xt3 = PDstandardNthfdOrder23(&Xt3[index]);
        break;
      }
      
      case 4:
      {
        PDstandardNth1alpha = PDstandardNthfdOrder41(&alpha[index]);
        PDstandardNth2alpha = PDstandardNthfdOrder42(&alpha[index]);
        PDstandardNth3alpha = PDstandardNthfdOrder43(&alpha[index]);
        PDstandardNth11alpha = PDstandardNthfdOrder411(&alpha[index]);
        PDstandardNth22alpha = PDstandardNthfdOrder422(&alpha[index]);
        PDstandardNth33alpha = PDstandardNthfdOrder433(&alpha[index]);
        PDstandardNth12alpha = PDstandardNthfdOrder412(&alpha[index]);
        PDstandardNth13alpha = PDstandardNthfdOrder413(&alpha[index]);
        PDstandardNth23alpha = PDstandardNthfdOrder423(&alpha[index]);
        PDstandardNth1beta1 = PDstandardNthfdOrder41(&beta1[index]);
        PDstandardNth2beta1 = PDstandardNthfdOrder42(&beta1[index]);
        PDstandardNth3beta1 = PDstandardNthfdOrder43(&beta1[index]);
        PDstandardNth11beta1 = PDstandardNthfdOrder411(&beta1[index]);
        PDstandardNth22beta1 = PDstandardNthfdOrder422(&beta1[index]);
        PDstandardNth33beta1 = PDstandardNthfdOrder433(&beta1[index]);
        PDstandardNth12beta1 = PDstandardNthfdOrder412(&beta1[index]);
        PDstandardNth13beta1 = PDstandardNthfdOrder413(&beta1[index]);
        PDstandardNth23beta1 = PDstandardNthfdOrder423(&beta1[index]);
        PDstandardNth1beta2 = PDstandardNthfdOrder41(&beta2[index]);
        PDstandardNth2beta2 = PDstandardNthfdOrder42(&beta2[index]);
        PDstandardNth3beta2 = PDstandardNthfdOrder43(&beta2[index]);
        PDstandardNth11beta2 = PDstandardNthfdOrder411(&beta2[index]);
        PDstandardNth22beta2 = PDstandardNthfdOrder422(&beta2[index]);
        PDstandardNth33beta2 = PDstandardNthfdOrder433(&beta2[index]);
        PDstandardNth12beta2 = PDstandardNthfdOrder412(&beta2[index]);
        PDstandardNth13beta2 = PDstandardNthfdOrder413(&beta2[index]);
        PDstandardNth23beta2 = PDstandardNthfdOrder423(&beta2[index]);
        PDstandardNth1beta3 = PDstandardNthfdOrder41(&beta3[index]);
        PDstandardNth2beta3 = PDstandardNthfdOrder42(&beta3[index]);
        PDstandardNth3beta3 = PDstandardNthfdOrder43(&beta3[index]);
        PDstandardNth11beta3 = PDstandardNthfdOrder411(&beta3[index]);
        PDstandardNth22beta3 = PDstandardNthfdOrder422(&beta3[index]);
        PDstandardNth33beta3 = PDstandardNthfdOrder433(&beta3[index]);
        PDstandardNth12beta3 = PDstandardNthfdOrder412(&beta3[index]);
        PDstandardNth13beta3 = PDstandardNthfdOrder413(&beta3[index]);
        PDstandardNth23beta3 = PDstandardNthfdOrder423(&beta3[index]);
        PDstandardNth1gt11 = PDstandardNthfdOrder41(&gt11[index]);
        PDstandardNth2gt11 = PDstandardNthfdOrder42(&gt11[index]);
        PDstandardNth3gt11 = PDstandardNthfdOrder43(&gt11[index]);
        PDstandardNth11gt11 = PDstandardNthfdOrder411(&gt11[index]);
        PDstandardNth22gt11 = PDstandardNthfdOrder422(&gt11[index]);
        PDstandardNth33gt11 = PDstandardNthfdOrder433(&gt11[index]);
        PDstandardNth12gt11 = PDstandardNthfdOrder412(&gt11[index]);
        PDstandardNth13gt11 = PDstandardNthfdOrder413(&gt11[index]);
        PDstandardNth23gt11 = PDstandardNthfdOrder423(&gt11[index]);
        PDstandardNth1gt12 = PDstandardNthfdOrder41(&gt12[index]);
        PDstandardNth2gt12 = PDstandardNthfdOrder42(&gt12[index]);
        PDstandardNth3gt12 = PDstandardNthfdOrder43(&gt12[index]);
        PDstandardNth11gt12 = PDstandardNthfdOrder411(&gt12[index]);
        PDstandardNth22gt12 = PDstandardNthfdOrder422(&gt12[index]);
        PDstandardNth33gt12 = PDstandardNthfdOrder433(&gt12[index]);
        PDstandardNth12gt12 = PDstandardNthfdOrder412(&gt12[index]);
        PDstandardNth13gt12 = PDstandardNthfdOrder413(&gt12[index]);
        PDstandardNth23gt12 = PDstandardNthfdOrder423(&gt12[index]);
        PDstandardNth1gt13 = PDstandardNthfdOrder41(&gt13[index]);
        PDstandardNth2gt13 = PDstandardNthfdOrder42(&gt13[index]);
        PDstandardNth3gt13 = PDstandardNthfdOrder43(&gt13[index]);
        PDstandardNth11gt13 = PDstandardNthfdOrder411(&gt13[index]);
        PDstandardNth22gt13 = PDstandardNthfdOrder422(&gt13[index]);
        PDstandardNth33gt13 = PDstandardNthfdOrder433(&gt13[index]);
        PDstandardNth12gt13 = PDstandardNthfdOrder412(&gt13[index]);
        PDstandardNth13gt13 = PDstandardNthfdOrder413(&gt13[index]);
        PDstandardNth23gt13 = PDstandardNthfdOrder423(&gt13[index]);
        PDstandardNth1gt22 = PDstandardNthfdOrder41(&gt22[index]);
        PDstandardNth2gt22 = PDstandardNthfdOrder42(&gt22[index]);
        PDstandardNth3gt22 = PDstandardNthfdOrder43(&gt22[index]);
        PDstandardNth11gt22 = PDstandardNthfdOrder411(&gt22[index]);
        PDstandardNth22gt22 = PDstandardNthfdOrder422(&gt22[index]);
        PDstandardNth33gt22 = PDstandardNthfdOrder433(&gt22[index]);
        PDstandardNth12gt22 = PDstandardNthfdOrder412(&gt22[index]);
        PDstandardNth13gt22 = PDstandardNthfdOrder413(&gt22[index]);
        PDstandardNth23gt22 = PDstandardNthfdOrder423(&gt22[index]);
        PDstandardNth1gt23 = PDstandardNthfdOrder41(&gt23[index]);
        PDstandardNth2gt23 = PDstandardNthfdOrder42(&gt23[index]);
        PDstandardNth3gt23 = PDstandardNthfdOrder43(&gt23[index]);
        PDstandardNth11gt23 = PDstandardNthfdOrder411(&gt23[index]);
        PDstandardNth22gt23 = PDstandardNthfdOrder422(&gt23[index]);
        PDstandardNth33gt23 = PDstandardNthfdOrder433(&gt23[index]);
        PDstandardNth12gt23 = PDstandardNthfdOrder412(&gt23[index]);
        PDstandardNth13gt23 = PDstandardNthfdOrder413(&gt23[index]);
        PDstandardNth23gt23 = PDstandardNthfdOrder423(&gt23[index]);
        PDstandardNth1gt33 = PDstandardNthfdOrder41(&gt33[index]);
        PDstandardNth2gt33 = PDstandardNthfdOrder42(&gt33[index]);
        PDstandardNth3gt33 = PDstandardNthfdOrder43(&gt33[index]);
        PDstandardNth11gt33 = PDstandardNthfdOrder411(&gt33[index]);
        PDstandardNth22gt33 = PDstandardNthfdOrder422(&gt33[index]);
        PDstandardNth33gt33 = PDstandardNthfdOrder433(&gt33[index]);
        PDstandardNth12gt33 = PDstandardNthfdOrder412(&gt33[index]);
        PDstandardNth13gt33 = PDstandardNthfdOrder413(&gt33[index]);
        PDstandardNth23gt33 = PDstandardNthfdOrder423(&gt33[index]);
        PDstandardNth1phi = PDstandardNthfdOrder41(&phi[index]);
        PDstandardNth2phi = PDstandardNthfdOrder42(&phi[index]);
        PDstandardNth3phi = PDstandardNthfdOrder43(&phi[index]);
        PDstandardNth11phi = PDstandardNthfdOrder411(&phi[index]);
        PDstandardNth22phi = PDstandardNthfdOrder422(&phi[index]);
        PDstandardNth33phi = PDstandardNthfdOrder433(&phi[index]);
        PDstandardNth12phi = PDstandardNthfdOrder412(&phi[index]);
        PDstandardNth13phi = PDstandardNthfdOrder413(&phi[index]);
        PDstandardNth23phi = PDstandardNthfdOrder423(&phi[index]);
        PDstandardNth1trK = PDstandardNthfdOrder41(&trK[index]);
        PDstandardNth2trK = PDstandardNthfdOrder42(&trK[index]);
        PDstandardNth3trK = PDstandardNthfdOrder43(&trK[index]);
        PDstandardNth1Xt1 = PDstandardNthfdOrder41(&Xt1[index]);
        PDstandardNth2Xt1 = PDstandardNthfdOrder42(&Xt1[index]);
        PDstandardNth3Xt1 = PDstandardNthfdOrder43(&Xt1[index]);
        PDstandardNth1Xt2 = PDstandardNthfdOrder41(&Xt2[index]);
        PDstandardNth2Xt2 = PDstandardNthfdOrder42(&Xt2[index]);
        PDstandardNth3Xt2 = PDstandardNthfdOrder43(&Xt2[index]);
        PDstandardNth1Xt3 = PDstandardNthfdOrder41(&Xt3[index]);
        PDstandardNth2Xt3 = PDstandardNthfdOrder42(&Xt3[index]);
        PDstandardNth3Xt3 = PDstandardNthfdOrder43(&Xt3[index]);
        break;
      }
      
      case 6:
      {
        PDstandardNth1alpha = PDstandardNthfdOrder61(&alpha[index]);
        PDstandardNth2alpha = PDstandardNthfdOrder62(&alpha[index]);
        PDstandardNth3alpha = PDstandardNthfdOrder63(&alpha[index]);
        PDstandardNth11alpha = PDstandardNthfdOrder611(&alpha[index]);
        PDstandardNth22alpha = PDstandardNthfdOrder622(&alpha[index]);
        PDstandardNth33alpha = PDstandardNthfdOrder633(&alpha[index]);
        PDstandardNth12alpha = PDstandardNthfdOrder612(&alpha[index]);
        PDstandardNth13alpha = PDstandardNthfdOrder613(&alpha[index]);
        PDstandardNth23alpha = PDstandardNthfdOrder623(&alpha[index]);
        PDstandardNth1beta1 = PDstandardNthfdOrder61(&beta1[index]);
        PDstandardNth2beta1 = PDstandardNthfdOrder62(&beta1[index]);
        PDstandardNth3beta1 = PDstandardNthfdOrder63(&beta1[index]);
        PDstandardNth11beta1 = PDstandardNthfdOrder611(&beta1[index]);
        PDstandardNth22beta1 = PDstandardNthfdOrder622(&beta1[index]);
        PDstandardNth33beta1 = PDstandardNthfdOrder633(&beta1[index]);
        PDstandardNth12beta1 = PDstandardNthfdOrder612(&beta1[index]);
        PDstandardNth13beta1 = PDstandardNthfdOrder613(&beta1[index]);
        PDstandardNth23beta1 = PDstandardNthfdOrder623(&beta1[index]);
        PDstandardNth1beta2 = PDstandardNthfdOrder61(&beta2[index]);
        PDstandardNth2beta2 = PDstandardNthfdOrder62(&beta2[index]);
        PDstandardNth3beta2 = PDstandardNthfdOrder63(&beta2[index]);
        PDstandardNth11beta2 = PDstandardNthfdOrder611(&beta2[index]);
        PDstandardNth22beta2 = PDstandardNthfdOrder622(&beta2[index]);
        PDstandardNth33beta2 = PDstandardNthfdOrder633(&beta2[index]);
        PDstandardNth12beta2 = PDstandardNthfdOrder612(&beta2[index]);
        PDstandardNth13beta2 = PDstandardNthfdOrder613(&beta2[index]);
        PDstandardNth23beta2 = PDstandardNthfdOrder623(&beta2[index]);
        PDstandardNth1beta3 = PDstandardNthfdOrder61(&beta3[index]);
        PDstandardNth2beta3 = PDstandardNthfdOrder62(&beta3[index]);
        PDstandardNth3beta3 = PDstandardNthfdOrder63(&beta3[index]);
        PDstandardNth11beta3 = PDstandardNthfdOrder611(&beta3[index]);
        PDstandardNth22beta3 = PDstandardNthfdOrder622(&beta3[index]);
        PDstandardNth33beta3 = PDstandardNthfdOrder633(&beta3[index]);
        PDstandardNth12beta3 = PDstandardNthfdOrder612(&beta3[index]);
        PDstandardNth13beta3 = PDstandardNthfdOrder613(&beta3[index]);
        PDstandardNth23beta3 = PDstandardNthfdOrder623(&beta3[index]);
        PDstandardNth1gt11 = PDstandardNthfdOrder61(&gt11[index]);
        PDstandardNth2gt11 = PDstandardNthfdOrder62(&gt11[index]);
        PDstandardNth3gt11 = PDstandardNthfdOrder63(&gt11[index]);
        PDstandardNth11gt11 = PDstandardNthfdOrder611(&gt11[index]);
        PDstandardNth22gt11 = PDstandardNthfdOrder622(&gt11[index]);
        PDstandardNth33gt11 = PDstandardNthfdOrder633(&gt11[index]);
        PDstandardNth12gt11 = PDstandardNthfdOrder612(&gt11[index]);
        PDstandardNth13gt11 = PDstandardNthfdOrder613(&gt11[index]);
        PDstandardNth23gt11 = PDstandardNthfdOrder623(&gt11[index]);
        PDstandardNth1gt12 = PDstandardNthfdOrder61(&gt12[index]);
        PDstandardNth2gt12 = PDstandardNthfdOrder62(&gt12[index]);
        PDstandardNth3gt12 = PDstandardNthfdOrder63(&gt12[index]);
        PDstandardNth11gt12 = PDstandardNthfdOrder611(&gt12[index]);
        PDstandardNth22gt12 = PDstandardNthfdOrder622(&gt12[index]);
        PDstandardNth33gt12 = PDstandardNthfdOrder633(&gt12[index]);
        PDstandardNth12gt12 = PDstandardNthfdOrder612(&gt12[index]);
        PDstandardNth13gt12 = PDstandardNthfdOrder613(&gt12[index]);
        PDstandardNth23gt12 = PDstandardNthfdOrder623(&gt12[index]);
        PDstandardNth1gt13 = PDstandardNthfdOrder61(&gt13[index]);
        PDstandardNth2gt13 = PDstandardNthfdOrder62(&gt13[index]);
        PDstandardNth3gt13 = PDstandardNthfdOrder63(&gt13[index]);
        PDstandardNth11gt13 = PDstandardNthfdOrder611(&gt13[index]);
        PDstandardNth22gt13 = PDstandardNthfdOrder622(&gt13[index]);
        PDstandardNth33gt13 = PDstandardNthfdOrder633(&gt13[index]);
        PDstandardNth12gt13 = PDstandardNthfdOrder612(&gt13[index]);
        PDstandardNth13gt13 = PDstandardNthfdOrder613(&gt13[index]);
        PDstandardNth23gt13 = PDstandardNthfdOrder623(&gt13[index]);
        PDstandardNth1gt22 = PDstandardNthfdOrder61(&gt22[index]);
        PDstandardNth2gt22 = PDstandardNthfdOrder62(&gt22[index]);
        PDstandardNth3gt22 = PDstandardNthfdOrder63(&gt22[index]);
        PDstandardNth11gt22 = PDstandardNthfdOrder611(&gt22[index]);
        PDstandardNth22gt22 = PDstandardNthfdOrder622(&gt22[index]);
        PDstandardNth33gt22 = PDstandardNthfdOrder633(&gt22[index]);
        PDstandardNth12gt22 = PDstandardNthfdOrder612(&gt22[index]);
        PDstandardNth13gt22 = PDstandardNthfdOrder613(&gt22[index]);
        PDstandardNth23gt22 = PDstandardNthfdOrder623(&gt22[index]);
        PDstandardNth1gt23 = PDstandardNthfdOrder61(&gt23[index]);
        PDstandardNth2gt23 = PDstandardNthfdOrder62(&gt23[index]);
        PDstandardNth3gt23 = PDstandardNthfdOrder63(&gt23[index]);
        PDstandardNth11gt23 = PDstandardNthfdOrder611(&gt23[index]);
        PDstandardNth22gt23 = PDstandardNthfdOrder622(&gt23[index]);
        PDstandardNth33gt23 = PDstandardNthfdOrder633(&gt23[index]);
        PDstandardNth12gt23 = PDstandardNthfdOrder612(&gt23[index]);
        PDstandardNth13gt23 = PDstandardNthfdOrder613(&gt23[index]);
        PDstandardNth23gt23 = PDstandardNthfdOrder623(&gt23[index]);
        PDstandardNth1gt33 = PDstandardNthfdOrder61(&gt33[index]);
        PDstandardNth2gt33 = PDstandardNthfdOrder62(&gt33[index]);
        PDstandardNth3gt33 = PDstandardNthfdOrder63(&gt33[index]);
        PDstandardNth11gt33 = PDstandardNthfdOrder611(&gt33[index]);
        PDstandardNth22gt33 = PDstandardNthfdOrder622(&gt33[index]);
        PDstandardNth33gt33 = PDstandardNthfdOrder633(&gt33[index]);
        PDstandardNth12gt33 = PDstandardNthfdOrder612(&gt33[index]);
        PDstandardNth13gt33 = PDstandardNthfdOrder613(&gt33[index]);
        PDstandardNth23gt33 = PDstandardNthfdOrder623(&gt33[index]);
        PDstandardNth1phi = PDstandardNthfdOrder61(&phi[index]);
        PDstandardNth2phi = PDstandardNthfdOrder62(&phi[index]);
        PDstandardNth3phi = PDstandardNthfdOrder63(&phi[index]);
        PDstandardNth11phi = PDstandardNthfdOrder611(&phi[index]);
        PDstandardNth22phi = PDstandardNthfdOrder622(&phi[index]);
        PDstandardNth33phi = PDstandardNthfdOrder633(&phi[index]);
        PDstandardNth12phi = PDstandardNthfdOrder612(&phi[index]);
        PDstandardNth13phi = PDstandardNthfdOrder613(&phi[index]);
        PDstandardNth23phi = PDstandardNthfdOrder623(&phi[index]);
        PDstandardNth1trK = PDstandardNthfdOrder61(&trK[index]);
        PDstandardNth2trK = PDstandardNthfdOrder62(&trK[index]);
        PDstandardNth3trK = PDstandardNthfdOrder63(&trK[index]);
        PDstandardNth1Xt1 = PDstandardNthfdOrder61(&Xt1[index]);
        PDstandardNth2Xt1 = PDstandardNthfdOrder62(&Xt1[index]);
        PDstandardNth3Xt1 = PDstandardNthfdOrder63(&Xt1[index]);
        PDstandardNth1Xt2 = PDstandardNthfdOrder61(&Xt2[index]);
        PDstandardNth2Xt2 = PDstandardNthfdOrder62(&Xt2[index]);
        PDstandardNth3Xt2 = PDstandardNthfdOrder63(&Xt2[index]);
        PDstandardNth1Xt3 = PDstandardNthfdOrder61(&Xt3[index]);
        PDstandardNth2Xt3 = PDstandardNthfdOrder62(&Xt3[index]);
        PDstandardNth3Xt3 = PDstandardNthfdOrder63(&Xt3[index]);
        break;
      }
      
      case 8:
      {
        PDstandardNth1alpha = PDstandardNthfdOrder81(&alpha[index]);
        PDstandardNth2alpha = PDstandardNthfdOrder82(&alpha[index]);
        PDstandardNth3alpha = PDstandardNthfdOrder83(&alpha[index]);
        PDstandardNth11alpha = PDstandardNthfdOrder811(&alpha[index]);
        PDstandardNth22alpha = PDstandardNthfdOrder822(&alpha[index]);
        PDstandardNth33alpha = PDstandardNthfdOrder833(&alpha[index]);
        PDstandardNth12alpha = PDstandardNthfdOrder812(&alpha[index]);
        PDstandardNth13alpha = PDstandardNthfdOrder813(&alpha[index]);
        PDstandardNth23alpha = PDstandardNthfdOrder823(&alpha[index]);
        PDstandardNth1beta1 = PDstandardNthfdOrder81(&beta1[index]);
        PDstandardNth2beta1 = PDstandardNthfdOrder82(&beta1[index]);
        PDstandardNth3beta1 = PDstandardNthfdOrder83(&beta1[index]);
        PDstandardNth11beta1 = PDstandardNthfdOrder811(&beta1[index]);
        PDstandardNth22beta1 = PDstandardNthfdOrder822(&beta1[index]);
        PDstandardNth33beta1 = PDstandardNthfdOrder833(&beta1[index]);
        PDstandardNth12beta1 = PDstandardNthfdOrder812(&beta1[index]);
        PDstandardNth13beta1 = PDstandardNthfdOrder813(&beta1[index]);
        PDstandardNth23beta1 = PDstandardNthfdOrder823(&beta1[index]);
        PDstandardNth1beta2 = PDstandardNthfdOrder81(&beta2[index]);
        PDstandardNth2beta2 = PDstandardNthfdOrder82(&beta2[index]);
        PDstandardNth3beta2 = PDstandardNthfdOrder83(&beta2[index]);
        PDstandardNth11beta2 = PDstandardNthfdOrder811(&beta2[index]);
        PDstandardNth22beta2 = PDstandardNthfdOrder822(&beta2[index]);
        PDstandardNth33beta2 = PDstandardNthfdOrder833(&beta2[index]);
        PDstandardNth12beta2 = PDstandardNthfdOrder812(&beta2[index]);
        PDstandardNth13beta2 = PDstandardNthfdOrder813(&beta2[index]);
        PDstandardNth23beta2 = PDstandardNthfdOrder823(&beta2[index]);
        PDstandardNth1beta3 = PDstandardNthfdOrder81(&beta3[index]);
        PDstandardNth2beta3 = PDstandardNthfdOrder82(&beta3[index]);
        PDstandardNth3beta3 = PDstandardNthfdOrder83(&beta3[index]);
        PDstandardNth11beta3 = PDstandardNthfdOrder811(&beta3[index]);
        PDstandardNth22beta3 = PDstandardNthfdOrder822(&beta3[index]);
        PDstandardNth33beta3 = PDstandardNthfdOrder833(&beta3[index]);
        PDstandardNth12beta3 = PDstandardNthfdOrder812(&beta3[index]);
        PDstandardNth13beta3 = PDstandardNthfdOrder813(&beta3[index]);
        PDstandardNth23beta3 = PDstandardNthfdOrder823(&beta3[index]);
        PDstandardNth1gt11 = PDstandardNthfdOrder81(&gt11[index]);
        PDstandardNth2gt11 = PDstandardNthfdOrder82(&gt11[index]);
        PDstandardNth3gt11 = PDstandardNthfdOrder83(&gt11[index]);
        PDstandardNth11gt11 = PDstandardNthfdOrder811(&gt11[index]);
        PDstandardNth22gt11 = PDstandardNthfdOrder822(&gt11[index]);
        PDstandardNth33gt11 = PDstandardNthfdOrder833(&gt11[index]);
        PDstandardNth12gt11 = PDstandardNthfdOrder812(&gt11[index]);
        PDstandardNth13gt11 = PDstandardNthfdOrder813(&gt11[index]);
        PDstandardNth23gt11 = PDstandardNthfdOrder823(&gt11[index]);
        PDstandardNth1gt12 = PDstandardNthfdOrder81(&gt12[index]);
        PDstandardNth2gt12 = PDstandardNthfdOrder82(&gt12[index]);
        PDstandardNth3gt12 = PDstandardNthfdOrder83(&gt12[index]);
        PDstandardNth11gt12 = PDstandardNthfdOrder811(&gt12[index]);
        PDstandardNth22gt12 = PDstandardNthfdOrder822(&gt12[index]);
        PDstandardNth33gt12 = PDstandardNthfdOrder833(&gt12[index]);
        PDstandardNth12gt12 = PDstandardNthfdOrder812(&gt12[index]);
        PDstandardNth13gt12 = PDstandardNthfdOrder813(&gt12[index]);
        PDstandardNth23gt12 = PDstandardNthfdOrder823(&gt12[index]);
        PDstandardNth1gt13 = PDstandardNthfdOrder81(&gt13[index]);
        PDstandardNth2gt13 = PDstandardNthfdOrder82(&gt13[index]);
        PDstandardNth3gt13 = PDstandardNthfdOrder83(&gt13[index]);
        PDstandardNth11gt13 = PDstandardNthfdOrder811(&gt13[index]);
        PDstandardNth22gt13 = PDstandardNthfdOrder822(&gt13[index]);
        PDstandardNth33gt13 = PDstandardNthfdOrder833(&gt13[index]);
        PDstandardNth12gt13 = PDstandardNthfdOrder812(&gt13[index]);
        PDstandardNth13gt13 = PDstandardNthfdOrder813(&gt13[index]);
        PDstandardNth23gt13 = PDstandardNthfdOrder823(&gt13[index]);
        PDstandardNth1gt22 = PDstandardNthfdOrder81(&gt22[index]);
        PDstandardNth2gt22 = PDstandardNthfdOrder82(&gt22[index]);
        PDstandardNth3gt22 = PDstandardNthfdOrder83(&gt22[index]);
        PDstandardNth11gt22 = PDstandardNthfdOrder811(&gt22[index]);
        PDstandardNth22gt22 = PDstandardNthfdOrder822(&gt22[index]);
        PDstandardNth33gt22 = PDstandardNthfdOrder833(&gt22[index]);
        PDstandardNth12gt22 = PDstandardNthfdOrder812(&gt22[index]);
        PDstandardNth13gt22 = PDstandardNthfdOrder813(&gt22[index]);
        PDstandardNth23gt22 = PDstandardNthfdOrder823(&gt22[index]);
        PDstandardNth1gt23 = PDstandardNthfdOrder81(&gt23[index]);
        PDstandardNth2gt23 = PDstandardNthfdOrder82(&gt23[index]);
        PDstandardNth3gt23 = PDstandardNthfdOrder83(&gt23[index]);
        PDstandardNth11gt23 = PDstandardNthfdOrder811(&gt23[index]);
        PDstandardNth22gt23 = PDstandardNthfdOrder822(&gt23[index]);
        PDstandardNth33gt23 = PDstandardNthfdOrder833(&gt23[index]);
        PDstandardNth12gt23 = PDstandardNthfdOrder812(&gt23[index]);
        PDstandardNth13gt23 = PDstandardNthfdOrder813(&gt23[index]);
        PDstandardNth23gt23 = PDstandardNthfdOrder823(&gt23[index]);
        PDstandardNth1gt33 = PDstandardNthfdOrder81(&gt33[index]);
        PDstandardNth2gt33 = PDstandardNthfdOrder82(&gt33[index]);
        PDstandardNth3gt33 = PDstandardNthfdOrder83(&gt33[index]);
        PDstandardNth11gt33 = PDstandardNthfdOrder811(&gt33[index]);
        PDstandardNth22gt33 = PDstandardNthfdOrder822(&gt33[index]);
        PDstandardNth33gt33 = PDstandardNthfdOrder833(&gt33[index]);
        PDstandardNth12gt33 = PDstandardNthfdOrder812(&gt33[index]);
        PDstandardNth13gt33 = PDstandardNthfdOrder813(&gt33[index]);
        PDstandardNth23gt33 = PDstandardNthfdOrder823(&gt33[index]);
        PDstandardNth1phi = PDstandardNthfdOrder81(&phi[index]);
        PDstandardNth2phi = PDstandardNthfdOrder82(&phi[index]);
        PDstandardNth3phi = PDstandardNthfdOrder83(&phi[index]);
        PDstandardNth11phi = PDstandardNthfdOrder811(&phi[index]);
        PDstandardNth22phi = PDstandardNthfdOrder822(&phi[index]);
        PDstandardNth33phi = PDstandardNthfdOrder833(&phi[index]);
        PDstandardNth12phi = PDstandardNthfdOrder812(&phi[index]);
        PDstandardNth13phi = PDstandardNthfdOrder813(&phi[index]);
        PDstandardNth23phi = PDstandardNthfdOrder823(&phi[index]);
        PDstandardNth1trK = PDstandardNthfdOrder81(&trK[index]);
        PDstandardNth2trK = PDstandardNthfdOrder82(&trK[index]);
        PDstandardNth3trK = PDstandardNthfdOrder83(&trK[index]);
        PDstandardNth1Xt1 = PDstandardNthfdOrder81(&Xt1[index]);
        PDstandardNth2Xt1 = PDstandardNthfdOrder82(&Xt1[index]);
        PDstandardNth3Xt1 = PDstandardNthfdOrder83(&Xt1[index]);
        PDstandardNth1Xt2 = PDstandardNthfdOrder81(&Xt2[index]);
        PDstandardNth2Xt2 = PDstandardNthfdOrder82(&Xt2[index]);
        PDstandardNth3Xt2 = PDstandardNthfdOrder83(&Xt2[index]);
        PDstandardNth1Xt3 = PDstandardNthfdOrder81(&Xt3[index]);
        PDstandardNth2Xt3 = PDstandardNthfdOrder82(&Xt3[index]);
        PDstandardNth3Xt3 = PDstandardNthfdOrder83(&Xt3[index]);
        break;
      }
      default:
        CCTK_BUILTIN_UNREACHABLE();
    }
    
    /* Calculate temporaries and grid functions */
    ptrdiff_t dir1 CCTK_ATTRIBUTE_UNUSED = isgn(beta1L);
    
    ptrdiff_t dir2 CCTK_ATTRIBUTE_UNUSED = isgn(beta2L);
    
    ptrdiff_t dir3 CCTK_ATTRIBUTE_UNUSED = isgn(beta3L);
    
    CCTK_REAL detgt CCTK_ATTRIBUTE_UNUSED = 1;
    
    CCTK_REAL gtu11 CCTK_ATTRIBUTE_UNUSED = INV(detgt)*(gt22L*gt33L - 
      SQR(gt23L));
    
    CCTK_REAL gtu12 CCTK_ATTRIBUTE_UNUSED = (gt13L*gt23L - 
      gt12L*gt33L)*INV(detgt);
    
    CCTK_REAL gtu13 CCTK_ATTRIBUTE_UNUSED = (-(gt13L*gt22L) + 
      gt12L*gt23L)*INV(detgt);
    
    CCTK_REAL gtu22 CCTK_ATTRIBUTE_UNUSED = INV(detgt)*(gt11L*gt33L - 
      SQR(gt13L));
    
    CCTK_REAL gtu23 CCTK_ATTRIBUTE_UNUSED = (gt12L*gt13L - 
      gt11L*gt23L)*INV(detgt);
    
    CCTK_REAL gtu33 CCTK_ATTRIBUTE_UNUSED = INV(detgt)*(gt11L*gt22L - 
      SQR(gt12L));
    
    CCTK_REAL Gtl111 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth1gt11;
    
    CCTK_REAL Gtl112 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth2gt11;
    
    CCTK_REAL Gtl113 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth3gt11;
    
    CCTK_REAL Gtl122 CCTK_ATTRIBUTE_UNUSED = -0.5*PDstandardNth1gt22 + 
      PDstandardNth2gt12;
    
    CCTK_REAL Gtl123 CCTK_ATTRIBUTE_UNUSED = 0.5*(-PDstandardNth1gt23 + 
      PDstandardNth2gt13 + PDstandardNth3gt12);
    
    CCTK_REAL Gtl133 CCTK_ATTRIBUTE_UNUSED = -0.5*PDstandardNth1gt33 + 
      PDstandardNth3gt13;
    
    CCTK_REAL Gtl211 CCTK_ATTRIBUTE_UNUSED = PDstandardNth1gt12 - 
      0.5*PDstandardNth2gt11;
    
    CCTK_REAL Gtl212 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth1gt22;
    
    CCTK_REAL Gtl213 CCTK_ATTRIBUTE_UNUSED = 0.5*(PDstandardNth1gt23 - 
      PDstandardNth2gt13 + PDstandardNth3gt12);
    
    CCTK_REAL Gtl222 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth2gt22;
    
    CCTK_REAL Gtl223 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth3gt22;
    
    CCTK_REAL Gtl233 CCTK_ATTRIBUTE_UNUSED = -0.5*PDstandardNth2gt33 + 
      PDstandardNth3gt23;
    
    CCTK_REAL Gtl311 CCTK_ATTRIBUTE_UNUSED = PDstandardNth1gt13 - 
      0.5*PDstandardNth3gt11;
    
    CCTK_REAL Gtl312 CCTK_ATTRIBUTE_UNUSED = 0.5*(PDstandardNth1gt23 + 
      PDstandardNth2gt13 - PDstandardNth3gt12);
    
    CCTK_REAL Gtl313 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth1gt33;
    
    CCTK_REAL Gtl322 CCTK_ATTRIBUTE_UNUSED = PDstandardNth2gt23 - 
      0.5*PDstandardNth3gt22;
    
    CCTK_REAL Gtl323 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth2gt33;
    
    CCTK_REAL Gtl333 CCTK_ATTRIBUTE_UNUSED = 0.5*PDstandardNth3gt33;
    
    CCTK_REAL Gtlu111 CCTK_ATTRIBUTE_UNUSED = Gtl111*gtu11 + Gtl112*gtu12 
      + Gtl113*gtu13;
    
    CCTK_REAL Gtlu112 CCTK_ATTRIBUTE_UNUSED = Gtl111*gtu12 + Gtl112*gtu22 
      + Gtl113*gtu23;
    
    CCTK_REAL Gtlu113 CCTK_ATTRIBUTE_UNUSED = Gtl111*gtu13 + Gtl112*gtu23 
      + Gtl113*gtu33;
    
    CCTK_REAL Gtlu121 CCTK_ATTRIBUTE_UNUSED = Gtl112*gtu11 + Gtl122*gtu12 
      + Gtl123*gtu13;
    
    CCTK_REAL Gtlu122 CCTK_ATTRIBUTE_UNUSED = Gtl112*gtu12 + Gtl122*gtu22 
      + Gtl123*gtu23;
    
    CCTK_REAL Gtlu123 CCTK_ATTRIBUTE_UNUSED = Gtl112*gtu13 + Gtl122*gtu23 
      + Gtl123*gtu33;
    
    CCTK_REAL Gtlu131 CCTK_ATTRIBUTE_UNUSED = Gtl113*gtu11 + Gtl123*gtu12 
      + Gtl133*gtu13;
    
    CCTK_REAL Gtlu132 CCTK_ATTRIBUTE_UNUSED = Gtl113*gtu12 + Gtl123*gtu22 
      + Gtl133*gtu23;
    
    CCTK_REAL Gtlu133 CCTK_ATTRIBUTE_UNUSED = Gtl113*gtu13 + Gtl123*gtu23 
      + Gtl133*gtu33;
    
    CCTK_REAL Gtlu211 CCTK_ATTRIBUTE_UNUSED = Gtl211*gtu11 + Gtl212*gtu12 
      + Gtl213*gtu13;
    
    CCTK_REAL Gtlu212 CCTK_ATTRIBUTE_UNUSED = Gtl211*gtu12 + Gtl212*gtu22 
      + Gtl213*gtu23;
    
    CCTK_REAL Gtlu213 CCTK_ATTRIBUTE_UNUSED = Gtl211*gtu13 + Gtl212*gtu23 
      + Gtl213*gtu33;
    
    CCTK_REAL Gtlu221 CCTK_ATTRIBUTE_UNUSED = Gtl212*gtu11 + Gtl222*gtu12 
      + Gtl223*gtu13;
    
    CCTK_REAL Gtlu222 CCTK_ATTRIBUTE_UNUSED = Gtl212*gtu12 + Gtl222*gtu22 
      + Gtl223*gtu23;
    
    CCTK_REAL Gtlu223 CCTK_ATTRIBUTE_UNUSED = Gtl212*gtu13 + Gtl222*gtu23 
      + Gtl223*gtu33;
    
    CCTK_REAL Gtlu231 CCTK_ATTRIBUTE_UNUSED = Gtl213*gtu11 + Gtl223*gtu12 
      + Gtl233*gtu13;
    
    CCTK_REAL Gtlu232 CCTK_ATTRIBUTE_UNUSED = Gtl213*gtu12 + Gtl223*gtu22 
      + Gtl233*gtu23;
    
    CCTK_REAL Gtlu233 CCTK_ATTRIBUTE_UNUSED = Gtl213*gtu13 + Gtl223*gtu23 
      + Gtl233*gtu33;
    
    CCTK_REAL Gtlu311 CCTK_ATTRIBUTE_UNUSED = Gtl311*gtu11 + Gtl312*gtu12 
      + Gtl313*gtu13;
    
    CCTK_REAL Gtlu312 CCTK_ATTRIBUTE_UNUSED = Gtl311*gtu12 + Gtl312*gtu22 
      + Gtl313*gtu23;
    
    CCTK_REAL Gtlu313 CCTK_ATTRIBUTE_UNUSED = Gtl311*gtu13 + Gtl312*gtu23 
      + Gtl313*gtu33;
    
    CCTK_REAL Gtlu321 CCTK_ATTRIBUTE_UNUSED = Gtl312*gtu11 + Gtl322*gtu12 
      + Gtl323*gtu13;
    
    CCTK_REAL Gtlu322 CCTK_ATTRIBUTE_UNUSED = Gtl312*gtu12 + Gtl322*gtu22 
      + Gtl323*gtu23;
    
    CCTK_REAL Gtlu323 CCTK_ATTRIBUTE_UNUSED = Gtl312*gtu13 + Gtl322*gtu23 
      + Gtl323*gtu33;
    
    CCTK_REAL Gtlu331 CCTK_ATTRIBUTE_UNUSED = Gtl313*gtu11 + Gtl323*gtu12 
      + Gtl333*gtu13;
    
    CCTK_REAL Gtlu332 CCTK_ATTRIBUTE_UNUSED = Gtl313*gtu12 + Gtl323*gtu22 
      + Gtl333*gtu23;
    
    CCTK_REAL Gtlu333 CCTK_ATTRIBUTE_UNUSED = Gtl313*gtu13 + Gtl323*gtu23 
      + Gtl333*gtu33;
    
    CCTK_REAL Gt111 CCTK_ATTRIBUTE_UNUSED = Gtl111*gtu11 + Gtl211*gtu12 + 
      Gtl311*gtu13;
    
    CCTK_REAL Gt211 CCTK_ATTRIBUTE_UNUSED = Gtl111*gtu12 + Gtl211*gtu22 + 
      Gtl311*gtu23;
    
    CCTK_REAL Gt311 CCTK_ATTRIBUTE_UNUSED = Gtl111*gtu13 + Gtl211*gtu23 + 
      Gtl311*gtu33;
    
    CCTK_REAL Gt112 CCTK_ATTRIBUTE_UNUSED = Gtl112*gtu11 + Gtl212*gtu12 + 
      Gtl312*gtu13;
    
    CCTK_REAL Gt212 CCTK_ATTRIBUTE_UNUSED = Gtl112*gtu12 + Gtl212*gtu22 + 
      Gtl312*gtu23;
    
    CCTK_REAL Gt312 CCTK_ATTRIBUTE_UNUSED = Gtl112*gtu13 + Gtl212*gtu23 + 
      Gtl312*gtu33;
    
    CCTK_REAL Gt113 CCTK_ATTRIBUTE_UNUSED = Gtl113*gtu11 + Gtl213*gtu12 + 
      Gtl313*gtu13;
    
    CCTK_REAL Gt213 CCTK_ATTRIBUTE_UNUSED = Gtl113*gtu12 + Gtl213*gtu22 + 
      Gtl313*gtu23;
    
    CCTK_REAL Gt313 CCTK_ATTRIBUTE_UNUSED = Gtl113*gtu13 + Gtl213*gtu23 + 
      Gtl313*gtu33;
    
    CCTK_REAL Gt122 CCTK_ATTRIBUTE_UNUSED = Gtl122*gtu11 + Gtl222*gtu12 + 
      Gtl322*gtu13;
    
    CCTK_REAL Gt222 CCTK_ATTRIBUTE_UNUSED = Gtl122*gtu12 + Gtl222*gtu22 + 
      Gtl322*gtu23;
    
    CCTK_REAL Gt322 CCTK_ATTRIBUTE_UNUSED = Gtl122*gtu13 + Gtl222*gtu23 + 
      Gtl322*gtu33;
    
    CCTK_REAL Gt123 CCTK_ATTRIBUTE_UNUSED = Gtl123*gtu11 + Gtl223*gtu12 + 
      Gtl323*gtu13;
    
    CCTK_REAL Gt223 CCTK_ATTRIBUTE_UNUSED = Gtl123*gtu12 + Gtl223*gtu22 + 
      Gtl323*gtu23;
    
    CCTK_REAL Gt323 CCTK_ATTRIBUTE_UNUSED = Gtl123*gtu13 + Gtl223*gtu23 + 
      Gtl323*gtu33;
    
    CCTK_REAL Gt133 CCTK_ATTRIBUTE_UNUSED = Gtl133*gtu11 + Gtl233*gtu12 + 
      Gtl333*gtu13;
    
    CCTK_REAL Gt233 CCTK_ATTRIBUTE_UNUSED = Gtl133*gtu12 + Gtl233*gtu22 + 
      Gtl333*gtu23;
    
    CCTK_REAL Gt333 CCTK_ATTRIBUTE_UNUSED = Gtl133*gtu13 + Gtl233*gtu23 + 
      Gtl333*gtu33;
    
    CCTK_REAL Xtn1 CCTK_ATTRIBUTE_UNUSED = Gt111*gtu11 + Gt122*gtu22 + 
      2*(Gt112*gtu12 + Gt113*gtu13 + Gt123*gtu23) + Gt133*gtu33;
    
    CCTK_REAL Xtn2 CCTK_ATTRIBUTE_UNUSED = Gt211*gtu11 + Gt222*gtu22 + 
      2*(Gt212*gtu12 + Gt213*gtu13 + Gt223*gtu23) + Gt233*gtu33;
    
    CCTK_REAL Xtn3 CCTK_ATTRIBUTE_UNUSED = Gt311*gtu11 + Gt322*gtu22 + 
      2*(Gt312*gtu12 + Gt313*gtu13 + Gt323*gtu23) + Gt333*gtu33;
    
    CCTK_REAL e4phi CCTK_ATTRIBUTE_UNUSED = IfThen(conformalMethod == 
      1,INV(SQR(phiL)),exp(4*phiL));
    
    CCTK_REAL em4phi CCTK_ATTRIBUTE_UNUSED = INV(e4phi);
    
    CCTK_REAL g11 CCTK_ATTRIBUTE_UNUSED = gt11L*e4phi;
    
    CCTK_REAL g12 CCTK_ATTRIBUTE_UNUSED = gt12L*e4phi;
    
    CCTK_REAL g13 CCTK_ATTRIBUTE_UNUSED = gt13L*e4phi;
    
    CCTK_REAL g22 CCTK_ATTRIBUTE_UNUSED = gt22L*e4phi;
    
    CCTK_REAL g23 CCTK_ATTRIBUTE_UNUSED = gt23L*e4phi;
    
    CCTK_REAL g33 CCTK_ATTRIBUTE_UNUSED = gt33L*e4phi;
    
    CCTK_REAL gu11 CCTK_ATTRIBUTE_UNUSED = em4phi*gtu11;
    
    CCTK_REAL gu12 CCTK_ATTRIBUTE_UNUSED = em4phi*gtu12;
    
    CCTK_REAL gu13 CCTK_ATTRIBUTE_UNUSED = em4phi*gtu13;
    
    CCTK_REAL gu22 CCTK_ATTRIBUTE_UNUSED = em4phi*gtu22;
    
    CCTK_REAL gu23 CCTK_ATTRIBUTE_UNUSED = em4phi*gtu23;
    
    CCTK_REAL gu33 CCTK_ATTRIBUTE_UNUSED = em4phi*gtu33;
    
    CCTK_REAL Rt11 CCTK_ATTRIBUTE_UNUSED = 3*(Gt111*Gtlu111 + 
      Gt112*Gtlu112 + Gt113*Gtlu113) + 2*(Gt211*Gtlu121 + Gt212*Gtlu122 + 
      Gt213*Gtlu123 + Gt311*Gtlu131 + Gt312*Gtlu132 + Gt313*Gtlu133) + 
      Gt211*Gtlu211 + Gt212*Gtlu212 + Gt213*Gtlu213 + Gt311*Gtlu311 + 
      Gt312*Gtlu312 + Gt313*Gtlu313 + gt11L*PDstandardNth1Xt1 + 
      gt12L*PDstandardNth1Xt2 + gt13L*PDstandardNth1Xt3 + 
      0.5*(-(gtu11*PDstandardNth11gt11) - 2*gtu12*PDstandardNth12gt11 - 
      2*gtu13*PDstandardNth13gt11 - gtu22*PDstandardNth22gt11 - 
      2*gtu23*PDstandardNth23gt11 - gtu33*PDstandardNth33gt11) + Gtl111*Xtn1 
      + Gtl112*Xtn2 + Gtl113*Xtn3;
    
    CCTK_REAL Rt12 CCTK_ATTRIBUTE_UNUSED = 0.5*(4*(Gt211*Gtlu221 + 
      Gt212*Gtlu222 + Gt213*Gtlu223) + 2*(Gt112*Gtlu111 + Gt122*Gtlu112 + 
      Gt123*Gtlu113 + Gt111*Gtlu121 + Gt212*Gtlu121 + Gt112*Gtlu122 + 
      Gt222*Gtlu122 + Gt113*Gtlu123 + Gt223*Gtlu123 + Gt312*Gtlu131 + 
      Gt322*Gtlu132 + Gt323*Gtlu133 + Gt111*Gtlu211 + Gt112*Gtlu212 + 
      Gt113*Gtlu213 + Gt311*Gtlu231 + Gt312*Gtlu232 + Gt313*Gtlu233 + 
      Gt311*Gtlu321 + Gt312*Gtlu322 + Gt313*Gtlu323) - 
      gtu11*PDstandardNth11gt12 - 2*gtu12*PDstandardNth12gt12 - 
      2*gtu13*PDstandardNth13gt12 + gt12L*PDstandardNth1Xt1 + 
      gt22L*PDstandardNth1Xt2 + gt23L*PDstandardNth1Xt3 - 
      gtu22*PDstandardNth22gt12 - 2*gtu23*PDstandardNth23gt12 + 
      gt11L*PDstandardNth2Xt1 + gt12L*PDstandardNth2Xt2 + 
      gt13L*PDstandardNth2Xt3 - gtu33*PDstandardNth33gt12 + Gtl112*Xtn1 + 
      Gtl211*Xtn1 + Gtl122*Xtn2 + Gtl212*Xtn2 + Gtl123*Xtn3 + Gtl213*Xtn3);
    
    CCTK_REAL Rt13 CCTK_ATTRIBUTE_UNUSED = 0.5*(2*(Gt113*Gtlu111 + 
      Gt123*Gtlu112 + Gt133*Gtlu113 + Gt213*Gtlu121 + Gt223*Gtlu122 + 
      Gt233*Gtlu123 + Gt111*Gtlu131 + Gt313*Gtlu131 + Gt112*Gtlu132 + 
      Gt323*Gtlu132 + Gt113*Gtlu133 + Gt333*Gtlu133 + Gt211*Gtlu231 + 
      Gt212*Gtlu232 + Gt213*Gtlu233 + Gt111*Gtlu311 + Gt112*Gtlu312 + 
      Gt113*Gtlu313 + Gt211*Gtlu321 + Gt212*Gtlu322 + Gt213*Gtlu323) + 
      4*(Gt311*Gtlu331 + Gt312*Gtlu332 + Gt313*Gtlu333) - 
      gtu11*PDstandardNth11gt13 - 2*gtu12*PDstandardNth12gt13 - 
      2*gtu13*PDstandardNth13gt13 + gt13L*PDstandardNth1Xt1 + 
      gt23L*PDstandardNth1Xt2 + gt33L*PDstandardNth1Xt3 - 
      gtu22*PDstandardNth22gt13 - 2*gtu23*PDstandardNth23gt13 - 
      gtu33*PDstandardNth33gt13 + gt11L*PDstandardNth3Xt1 + 
      gt12L*PDstandardNth3Xt2 + gt13L*PDstandardNth3Xt3 + Gtl113*Xtn1 + 
      Gtl311*Xtn1 + Gtl123*Xtn2 + Gtl312*Xtn2 + Gtl133*Xtn3 + Gtl313*Xtn3);
    
    CCTK_REAL Rt22 CCTK_ATTRIBUTE_UNUSED = Gt112*(Gtlu121 + 2*Gtlu211) + 
      Gt122*(Gtlu122 + 2*Gtlu212) + Gt123*(Gtlu123 + 2*Gtlu213) + 
      3*(Gt212*Gtlu221 + Gt222*Gtlu222 + Gt223*Gtlu223) + 2*(Gt312*Gtlu231 + 
      Gt322*Gtlu232 + Gt323*Gtlu233) + Gt312*Gtlu321 + Gt322*Gtlu322 + 
      Gt323*Gtlu323 + gt12L*PDstandardNth2Xt1 + gt22L*PDstandardNth2Xt2 + 
      gt23L*PDstandardNth2Xt3 + 0.5*(-(gtu11*PDstandardNth11gt22) - 
      2*gtu12*PDstandardNth12gt22 - 2*gtu13*PDstandardNth13gt22 - 
      gtu22*PDstandardNth22gt22 - 2*gtu23*PDstandardNth23gt22 - 
      gtu33*PDstandardNth33gt22) + Gtl212*Xtn1 + Gtl222*Xtn2 + Gtl223*Xtn3;
    
    CCTK_REAL Rt23 CCTK_ATTRIBUTE_UNUSED = 0.5*(2*(Gt112*Gtlu131 + 
      Gt122*Gtlu132 + Gt123*Gtlu133 + Gt113*Gtlu211 + Gt123*Gtlu212 + 
      Gt133*Gtlu213 + Gt213*Gtlu221 + Gt223*Gtlu222 + Gt233*Gtlu223 + 
      Gt212*Gtlu231 + Gt313*Gtlu231 + Gt222*Gtlu232 + Gt323*Gtlu232 + 
      Gt223*Gtlu233 + Gt333*Gtlu233 + Gt112*Gtlu311 + Gt122*Gtlu312 + 
      Gt123*Gtlu313 + Gt212*Gtlu321 + Gt222*Gtlu322 + Gt223*Gtlu323) + 
      4*(Gt312*Gtlu331 + Gt322*Gtlu332 + Gt323*Gtlu333) - 
      gtu11*PDstandardNth11gt23 - 2*gtu12*PDstandardNth12gt23 - 
      2*gtu13*PDstandardNth13gt23 - gtu22*PDstandardNth22gt23 - 
      2*gtu23*PDstandardNth23gt23 + gt13L*PDstandardNth2Xt1 + 
      gt23L*PDstandardNth2Xt2 + gt33L*PDstandardNth2Xt3 - 
      gtu33*PDstandardNth33gt23 + gt12L*PDstandardNth3Xt1 + 
      gt22L*PDstandardNth3Xt2 + gt23L*PDstandardNth3Xt3 + Gtl213*Xtn1 + 
      Gtl312*Xtn1 + Gtl223*Xtn2 + Gtl322*Xtn2 + Gtl233*Xtn3 + Gtl323*Xtn3);
    
    CCTK_REAL Rt33 CCTK_ATTRIBUTE_UNUSED = Gt113*(Gtlu131 + 2*Gtlu311) + 
      Gt123*(Gtlu132 + 2*Gtlu312) + Gt133*(Gtlu133 + 2*Gtlu313) + 
      Gt213*(Gtlu231 + 2*Gtlu321) + Gt223*(Gtlu232 + 2*Gtlu322) + 
      Gt233*(Gtlu233 + 2*Gtlu323) + 3*(Gt313*Gtlu331 + Gt323*Gtlu332 + 
      Gt333*Gtlu333) + 0.5*(-(gtu11*PDstandardNth11gt33) - 
      2*gtu12*PDstandardNth12gt33 - 2*gtu13*PDstandardNth13gt33 - 
      gtu22*PDstandardNth22gt33 - 2*gtu23*PDstandardNth23gt33 - 
      gtu33*PDstandardNth33gt33) + gt13L*PDstandardNth3Xt1 + 
      gt23L*PDstandardNth3Xt2 + gt33L*PDstandardNth3Xt3 + Gtl313*Xtn1 + 
      Gtl323*Xtn2 + Gtl333*Xtn3;
    
    CCTK_REAL fac1 CCTK_ATTRIBUTE_UNUSED = IfThen(conformalMethod == 
      1,-0.5*INV(phiL),1);
    
    CCTK_REAL cdphi1 CCTK_ATTRIBUTE_UNUSED = fac1*PDstandardNth1phi;
    
    CCTK_REAL cdphi2 CCTK_ATTRIBUTE_UNUSED = fac1*PDstandardNth2phi;
    
    CCTK_REAL cdphi3 CCTK_ATTRIBUTE_UNUSED = fac1*PDstandardNth3phi;
    
    CCTK_REAL fac2 CCTK_ATTRIBUTE_UNUSED = IfThen(conformalMethod == 
      1,0.5*INV(SQR(phiL)),0);
    
    CCTK_REAL cdphi211 CCTK_ATTRIBUTE_UNUSED = -(fac1*(-PDstandardNth11phi 
      + Gt111*PDstandardNth1phi + Gt211*PDstandardNth2phi + 
      Gt311*PDstandardNth3phi)) + fac2*SQR(PDstandardNth1phi);
    
    CCTK_REAL cdphi212 CCTK_ATTRIBUTE_UNUSED = 
      fac2*PDstandardNth1phi*PDstandardNth2phi - fac1*(-PDstandardNth12phi + 
      Gt112*PDstandardNth1phi + Gt212*PDstandardNth2phi + 
      Gt312*PDstandardNth3phi);
    
    CCTK_REAL cdphi213 CCTK_ATTRIBUTE_UNUSED = 
      fac2*PDstandardNth1phi*PDstandardNth3phi - fac1*(-PDstandardNth13phi + 
      Gt113*PDstandardNth1phi + Gt213*PDstandardNth2phi + 
      Gt313*PDstandardNth3phi);
    
    CCTK_REAL cdphi222 CCTK_ATTRIBUTE_UNUSED = 
      -(fac1*(Gt122*PDstandardNth1phi - PDstandardNth22phi + 
      Gt222*PDstandardNth2phi + Gt322*PDstandardNth3phi)) + 
      fac2*SQR(PDstandardNth2phi);
    
    CCTK_REAL cdphi223 CCTK_ATTRIBUTE_UNUSED = 
      fac2*PDstandardNth2phi*PDstandardNth3phi - 
      fac1*(Gt123*PDstandardNth1phi - PDstandardNth23phi + 
      Gt223*PDstandardNth2phi + Gt323*PDstandardNth3phi);
    
    CCTK_REAL cdphi233 CCTK_ATTRIBUTE_UNUSED = 
      -(fac1*(Gt133*PDstandardNth1phi + Gt233*PDstandardNth2phi - 
      PDstandardNth33phi + Gt333*PDstandardNth3phi)) + 
      fac2*SQR(PDstandardNth3phi);
    
    CCTK_REAL Rphi11 CCTK_ATTRIBUTE_UNUSED = -2*(cdphi211 + 2*(-1 + 
      gt11L*gtu11)*SQR(cdphi1) + gt11L*(cdphi211*gtu11 + 
      4*(cdphi1*(cdphi2*gtu12 + cdphi3*gtu13) + cdphi2*cdphi3*gtu23) + 
      cdphi233*gtu33 + gtu22*(cdphi222 + 2*SQR(cdphi2)) + 2*(cdphi212*gtu12 + 
      cdphi213*gtu13 + cdphi223*gtu23 + gtu33*SQR(cdphi3))));
    
    CCTK_REAL Rphi12 CCTK_ATTRIBUTE_UNUSED = -2*(cdphi212 + 
      cdphi1*(cdphi2*(-2 + 4*gt12L*gtu12) + 4*gt12L*cdphi3*gtu13) + 
      gt12L*(cdphi211*gtu11 + 4*cdphi2*cdphi3*gtu23 + 2*(cdphi212*gtu12 + 
      cdphi213*gtu13 + cdphi223*gtu23 + gtu11*SQR(cdphi1)) + gtu22*(cdphi222 
      + 2*SQR(cdphi2)) + gtu33*(cdphi233 + 2*SQR(cdphi3))));
    
    CCTK_REAL Rphi13 CCTK_ATTRIBUTE_UNUSED = -2*(cdphi213 + 
      cdphi1*(4*gt13L*cdphi2*gtu12 + cdphi3*(-2 + 4*gt13L*gtu13)) + 
      gt13L*(cdphi211*gtu11 + 4*cdphi2*cdphi3*gtu23 + 2*(cdphi212*gtu12 + 
      cdphi213*gtu13 + cdphi223*gtu23 + gtu11*SQR(cdphi1)) + gtu22*(cdphi222 
      + 2*SQR(cdphi2)) + gtu33*(cdphi233 + 2*SQR(cdphi3))));
    
    CCTK_REAL Rphi22 CCTK_ATTRIBUTE_UNUSED = -2*(cdphi222 + 2*(-1 + 
      gt22L*gtu22)*SQR(cdphi2) + gt22L*(cdphi222*gtu22 + 
      4*(cdphi1*cdphi3*gtu13 + cdphi2*(cdphi1*gtu12 + cdphi3*gtu23)) + 
      cdphi233*gtu33 + gtu11*(cdphi211 + 2*SQR(cdphi1)) + 2*(cdphi212*gtu12 + 
      cdphi213*gtu13 + cdphi223*gtu23 + gtu33*SQR(cdphi3))));
    
    CCTK_REAL Rphi23 CCTK_ATTRIBUTE_UNUSED = -2*(cdphi223 + 
      cdphi2*(4*gt23L*cdphi1*gtu12 + cdphi3*(-2 + 4*gt23L*gtu23)) + 
      gt23L*(4*cdphi1*cdphi3*gtu13 + cdphi222*gtu22 + gtu11*(cdphi211 + 
      2*SQR(cdphi1)) + 2*(cdphi212*gtu12 + cdphi213*gtu13 + cdphi223*gtu23 + 
      gtu22*SQR(cdphi2)) + gtu33*(cdphi233 + 2*SQR(cdphi3))));
    
    CCTK_REAL Rphi33 CCTK_ATTRIBUTE_UNUSED = -2*(cdphi233 + 
      gt33L*((4*cdphi1*cdphi2 + 2*cdphi212)*gtu12 + 4*cdphi3*(cdphi1*gtu13 + 
      cdphi2*gtu23) + 2*(cdphi213*gtu13 + cdphi223*gtu23) + cdphi233*gtu33 + 
      gtu11*(cdphi211 + 2*SQR(cdphi1)) + gtu22*(cdphi222 + 2*SQR(cdphi2))) + 
      2*(-1 + gt33L*gtu33)*SQR(cdphi3));
    
    CCTK_REAL Atm11 CCTK_ATTRIBUTE_UNUSED = At11L*gtu11 + At12L*gtu12 + 
      At13L*gtu13;
    
    CCTK_REAL Atm21 CCTK_ATTRIBUTE_UNUSED = At11L*gtu12 + At12L*gtu22 + 
      At13L*gtu23;
    
    CCTK_REAL Atm31 CCTK_ATTRIBUTE_UNUSED = At11L*gtu13 + At12L*gtu23 + 
      At13L*gtu33;
    
    CCTK_REAL Atm12 CCTK_ATTRIBUTE_UNUSED = At12L*gtu11 + At22L*gtu12 + 
      At23L*gtu13;
    
    CCTK_REAL Atm22 CCTK_ATTRIBUTE_UNUSED = At12L*gtu12 + At22L*gtu22 + 
      At23L*gtu23;
    
    CCTK_REAL Atm32 CCTK_ATTRIBUTE_UNUSED = At12L*gtu13 + At22L*gtu23 + 
      At23L*gtu33;
    
    CCTK_REAL Atm13 CCTK_ATTRIBUTE_UNUSED = At13L*gtu11 + At23L*gtu12 + 
      At33L*gtu13;
    
    CCTK_REAL Atm23 CCTK_ATTRIBUTE_UNUSED = At13L*gtu12 + At23L*gtu22 + 
      At33L*gtu23;
    
    CCTK_REAL Atm33 CCTK_ATTRIBUTE_UNUSED = At13L*gtu13 + At23L*gtu23 + 
      At33L*gtu33;
    
    CCTK_REAL Atu11 CCTK_ATTRIBUTE_UNUSED = Atm11*gtu11 + Atm12*gtu12 + 
      Atm13*gtu13;
    
    CCTK_REAL Atu12 CCTK_ATTRIBUTE_UNUSED = Atm11*gtu12 + Atm12*gtu22 + 
      Atm13*gtu23;
    
    CCTK_REAL Atu13 CCTK_ATTRIBUTE_UNUSED = Atm11*gtu13 + Atm12*gtu23 + 
      Atm13*gtu33;
    
    CCTK_REAL Atu22 CCTK_ATTRIBUTE_UNUSED = Atm21*gtu12 + Atm22*gtu22 + 
      Atm23*gtu23;
    
    CCTK_REAL Atu23 CCTK_ATTRIBUTE_UNUSED = Atm21*gtu13 + Atm22*gtu23 + 
      Atm23*gtu33;
    
    CCTK_REAL Atu33 CCTK_ATTRIBUTE_UNUSED = Atm31*gtu13 + Atm32*gtu23 + 
      Atm33*gtu33;
    
    CCTK_REAL R11 CCTK_ATTRIBUTE_UNUSED = Rphi11 + Rt11;
    
    CCTK_REAL R12 CCTK_ATTRIBUTE_UNUSED = Rphi12 + Rt12;
    
    CCTK_REAL R13 CCTK_ATTRIBUTE_UNUSED = Rphi13 + Rt13;
    
    CCTK_REAL R22 CCTK_ATTRIBUTE_UNUSED = Rphi22 + Rt22;
    
    CCTK_REAL R23 CCTK_ATTRIBUTE_UNUSED = Rphi23 + Rt23;
    
    CCTK_REAL R33 CCTK_ATTRIBUTE_UNUSED = Rphi33 + Rt33;
    
    CCTK_REAL phirhsL CCTK_ATTRIBUTE_UNUSED = (alphaL*trKL - 
      PDstandardNth1beta1 - PDstandardNth2beta2 - 
      PDstandardNth3beta3)*IfThen(conformalMethod == 
      1,0.333333333333333333333333333333*phiL,-0.166666666666666666666666666667);
    
    CCTK_REAL gt11rhsL CCTK_ATTRIBUTE_UNUSED = -2*alphaL*At11L + 
      2*(gt11L*PDstandardNth1beta1 + gt12L*PDstandardNth1beta2 + 
      gt13L*PDstandardNth1beta3) - 
      0.666666666666666666666666666667*gt11L*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3);
    
    CCTK_REAL gt12rhsL CCTK_ATTRIBUTE_UNUSED = 
      0.333333333333333333333333333333*(-6*alphaL*At12L + 
      3*(gt22L*PDstandardNth1beta2 + gt23L*PDstandardNth1beta3 + 
      gt11L*PDstandardNth2beta1 + gt13L*PDstandardNth2beta3) + 
      gt12L*(PDstandardNth1beta1 + PDstandardNth2beta2 - 
      2*PDstandardNth3beta3));
    
    CCTK_REAL gt13rhsL CCTK_ATTRIBUTE_UNUSED = 
      0.333333333333333333333333333333*(-6*alphaL*At13L + 
      3*(gt23L*PDstandardNth1beta2 + gt33L*PDstandardNth1beta3 + 
      gt11L*PDstandardNth3beta1 + gt12L*PDstandardNth3beta2) + 
      gt13L*(PDstandardNth1beta1 - 2*PDstandardNth2beta2 + 
      PDstandardNth3beta3));
    
    CCTK_REAL gt22rhsL CCTK_ATTRIBUTE_UNUSED = -2*alphaL*At22L + 
      2*(gt12L*PDstandardNth2beta1 + gt22L*PDstandardNth2beta2 + 
      gt23L*PDstandardNth2beta3) - 
      0.666666666666666666666666666667*gt22L*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3);
    
    CCTK_REAL gt23rhsL CCTK_ATTRIBUTE_UNUSED = 
      0.333333333333333333333333333333*(-6*alphaL*At23L + 
      3*(gt13L*PDstandardNth2beta1 + gt33L*PDstandardNth2beta3 + 
      gt12L*PDstandardNth3beta1 + gt22L*PDstandardNth3beta2) + 
      gt23L*(-2*PDstandardNth1beta1 + PDstandardNth2beta2 + 
      PDstandardNth3beta3));
    
    CCTK_REAL gt33rhsL CCTK_ATTRIBUTE_UNUSED = -2*alphaL*At33L - 
      0.666666666666666666666666666667*gt33L*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3) + 
      2*(gt13L*PDstandardNth3beta1 + gt23L*PDstandardNth3beta2 + 
      gt33L*PDstandardNth3beta3);
    
    CCTK_REAL dotXt1 CCTK_ATTRIBUTE_UNUSED = gtu11*PDstandardNth11beta1 + 
      gtu22*PDstandardNth22beta1 + gtu33*PDstandardNth33beta1 + 
      0.333333333333333333333333333333*(gtu11*(PDstandardNth11beta1 + 
      PDstandardNth12beta2 + PDstandardNth13beta3) + 
      gtu12*(PDstandardNth12beta1 + PDstandardNth22beta2 + 
      PDstandardNth23beta3) + gtu13*(PDstandardNth13beta1 + 
      PDstandardNth23beta2 + PDstandardNth33beta3)) - 
      2*(Atu11*PDstandardNth1alpha + Atu12*PDstandardNth2alpha + 
      Atu13*PDstandardNth3alpha) + 2*(gtu12*PDstandardNth12beta1 + 
      gtu13*PDstandardNth13beta1 + gtu23*PDstandardNth23beta1 + 
      alphaL*(6*(Atu11*cdphi1 + Atu12*cdphi2 + Atu13*cdphi3) + Atu11*Gt111 + 
      2*Atu12*Gt112 + 2*Atu13*Gt113 + Atu22*Gt122 + 2*Atu23*Gt123 + 
      Atu33*Gt133 - 0.666666666666666666666666666667*(gtu11*PDstandardNth1trK 
      + gtu12*PDstandardNth2trK + gtu13*PDstandardNth3trK))) + 
      (-PDstandardNth1beta1 + 
      0.666666666666666666666666666667*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3))*Xtn1 - 
      PDstandardNth2beta1*Xtn2 - PDstandardNth3beta1*Xtn3;
    
    CCTK_REAL dotXt2 CCTK_ATTRIBUTE_UNUSED = gtu11*PDstandardNth11beta2 + 
      gtu22*PDstandardNth22beta2 + gtu33*PDstandardNth33beta2 + 
      0.333333333333333333333333333333*(gtu12*(PDstandardNth11beta1 + 
      PDstandardNth12beta2 + PDstandardNth13beta3) + 
      gtu22*(PDstandardNth12beta1 + PDstandardNth22beta2 + 
      PDstandardNth23beta3) + gtu23*(PDstandardNth13beta1 + 
      PDstandardNth23beta2 + PDstandardNth33beta3)) - 
      2*(Atu12*PDstandardNth1alpha + Atu22*PDstandardNth2alpha + 
      Atu23*PDstandardNth3alpha) + 2*(gtu12*PDstandardNth12beta2 + 
      gtu13*PDstandardNth13beta2 + gtu23*PDstandardNth23beta2 + 
      alphaL*(6*(Atu12*cdphi1 + Atu22*cdphi2 + Atu23*cdphi3) + Atu11*Gt211 + 
      2*Atu12*Gt212 + 2*Atu13*Gt213 + Atu22*Gt222 + 2*Atu23*Gt223 + 
      Atu33*Gt233 - 0.666666666666666666666666666667*(gtu12*PDstandardNth1trK 
      + gtu22*PDstandardNth2trK + gtu23*PDstandardNth3trK))) - 
      PDstandardNth1beta2*Xtn1 + (-PDstandardNth2beta2 + 
      0.666666666666666666666666666667*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3))*Xtn2 - 
      PDstandardNth3beta2*Xtn3;
    
    CCTK_REAL dotXt3 CCTK_ATTRIBUTE_UNUSED = gtu11*PDstandardNth11beta3 + 
      gtu22*PDstandardNth22beta3 + gtu33*PDstandardNth33beta3 + 
      0.333333333333333333333333333333*(gtu13*(PDstandardNth11beta1 + 
      PDstandardNth12beta2 + PDstandardNth13beta3) + 
      gtu23*(PDstandardNth12beta1 + PDstandardNth22beta2 + 
      PDstandardNth23beta3) + gtu33*(PDstandardNth13beta1 + 
      PDstandardNth23beta2 + PDstandardNth33beta3)) - 
      2*(Atu13*PDstandardNth1alpha + Atu23*PDstandardNth2alpha + 
      Atu33*PDstandardNth3alpha) + 2*(gtu12*PDstandardNth12beta3 + 
      gtu13*PDstandardNth13beta3 + gtu23*PDstandardNth23beta3 + 
      alphaL*(6*(Atu13*cdphi1 + Atu23*cdphi2 + Atu33*cdphi3) + Atu11*Gt311 + 
      2*Atu12*Gt312 + 2*Atu13*Gt313 + Atu22*Gt322 + 2*Atu23*Gt323 + 
      Atu33*Gt333 - 0.666666666666666666666666666667*(gtu13*PDstandardNth1trK 
      + gtu23*PDstandardNth2trK + gtu33*PDstandardNth3trK))) - 
      PDstandardNth1beta3*Xtn1 - PDstandardNth2beta3*Xtn2 + 
      (-PDstandardNth3beta3 + 
      0.666666666666666666666666666667*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3))*Xtn3;
    
    CCTK_REAL Xt1rhsL CCTK_ATTRIBUTE_UNUSED = dotXt1;
    
    CCTK_REAL Xt2rhsL CCTK_ATTRIBUTE_UNUSED = dotXt2;
    
    CCTK_REAL Xt3rhsL CCTK_ATTRIBUTE_UNUSED = dotXt3;
    
    CCTK_REAL dottrK CCTK_ATTRIBUTE_UNUSED = 
      -(em4phi*(gtu11*PDstandardNth11alpha + gtu22*PDstandardNth22alpha + 
      gtu33*(PDstandardNth33alpha + 2*cdphi3*PDstandardNth3alpha) + 
      2*(gtu12*PDstandardNth12alpha + gtu13*(PDstandardNth13alpha + 
      cdphi1*PDstandardNth3alpha) + gtu23*(PDstandardNth23alpha + 
      cdphi2*PDstandardNth3alpha)) + PDstandardNth1alpha*(2*(cdphi1*gtu11 + 
      cdphi2*gtu12 + cdphi3*gtu13) - Xtn1) + 
      PDstandardNth2alpha*(2*(cdphi1*gtu12 + cdphi2*gtu22 + cdphi3*gtu23) - 
      Xtn2) - PDstandardNth3alpha*Xtn3)) + alphaL*(2*(Atm12*Atm21 + 
      Atm13*Atm31 + Atm23*Atm32) + 0.333333333333333333333333333333*SQR(trKL) 
      + SQR(Atm11) + SQR(Atm22) + SQR(Atm33));
    
    CCTK_REAL trKrhsL CCTK_ATTRIBUTE_UNUSED = dottrK;
    
    CCTK_REAL Ats11 CCTK_ATTRIBUTE_UNUSED = -PDstandardNth11alpha + 
      (4*cdphi1 + Gt111)*PDstandardNth1alpha + Gt211*PDstandardNth2alpha + 
      Gt311*PDstandardNth3alpha + alphaL*R11;
    
    CCTK_REAL Ats12 CCTK_ATTRIBUTE_UNUSED = -PDstandardNth12alpha + 
      (2*cdphi2 + Gt112)*PDstandardNth1alpha + (2*cdphi1 + 
      Gt212)*PDstandardNth2alpha + Gt312*PDstandardNth3alpha + alphaL*R12;
    
    CCTK_REAL Ats13 CCTK_ATTRIBUTE_UNUSED = -PDstandardNth13alpha + 
      (2*cdphi3 + Gt113)*PDstandardNth1alpha + Gt213*PDstandardNth2alpha + 
      (2*cdphi1 + Gt313)*PDstandardNth3alpha + alphaL*R13;
    
    CCTK_REAL Ats22 CCTK_ATTRIBUTE_UNUSED = Gt122*PDstandardNth1alpha - 
      PDstandardNth22alpha + (4*cdphi2 + Gt222)*PDstandardNth2alpha + 
      Gt322*PDstandardNth3alpha + alphaL*R22;
    
    CCTK_REAL Ats23 CCTK_ATTRIBUTE_UNUSED = Gt123*PDstandardNth1alpha - 
      PDstandardNth23alpha + (2*cdphi3 + Gt223)*PDstandardNth2alpha + 
      (2*cdphi2 + Gt323)*PDstandardNth3alpha + alphaL*R23;
    
    CCTK_REAL Ats33 CCTK_ATTRIBUTE_UNUSED = Gt133*PDstandardNth1alpha + 
      Gt233*PDstandardNth2alpha - PDstandardNth33alpha + (4*cdphi3 + 
      Gt333)*PDstandardNth3alpha + alphaL*R33;
    
    CCTK_REAL trAts CCTK_ATTRIBUTE_UNUSED = Ats11*gu11 + Ats22*gu22 + 
      2*(Ats12*gu12 + Ats13*gu13 + Ats23*gu23) + Ats33*gu33;
    
    CCTK_REAL At11rhsL CCTK_ATTRIBUTE_UNUSED = alphaL*(At11L*trKL - 
      2*(At11L*Atm11 + At12L*Atm21 + At13L*Atm31)) + 
      2*(At11L*PDstandardNth1beta1 + At12L*PDstandardNth1beta2 + 
      At13L*PDstandardNth1beta3) - 
      0.666666666666666666666666666667*At11L*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3) + em4phi*(Ats11 - 
      0.333333333333333333333333333333*g11*trAts);
    
    CCTK_REAL At12rhsL CCTK_ATTRIBUTE_UNUSED = alphaL*(At12L*trKL - 
      2*(At11L*Atm12 + At12L*Atm22 + At13L*Atm32)) + 
      At22L*PDstandardNth1beta2 + At23L*PDstandardNth1beta3 + 
      At11L*PDstandardNth2beta1 + At13L*PDstandardNth2beta3 + 
      At12L*(PDstandardNth1beta1 + PDstandardNth2beta2 - 
      0.666666666666666666666666666667*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3)) + em4phi*(Ats12 - 
      0.333333333333333333333333333333*g12*trAts);
    
    CCTK_REAL At13rhsL CCTK_ATTRIBUTE_UNUSED = alphaL*(At13L*trKL - 
      2*(At11L*Atm13 + At12L*Atm23 + At13L*Atm33)) + 
      At23L*PDstandardNth1beta2 + At33L*PDstandardNth1beta3 + 
      At11L*PDstandardNth3beta1 + At12L*PDstandardNth3beta2 + 
      At13L*(PDstandardNth1beta1 + PDstandardNth3beta3 - 
      0.666666666666666666666666666667*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3)) + em4phi*(Ats13 - 
      0.333333333333333333333333333333*g13*trAts);
    
    CCTK_REAL At22rhsL CCTK_ATTRIBUTE_UNUSED = alphaL*(At22L*trKL - 
      2*(At12L*Atm12 + At22L*Atm22 + At23L*Atm32)) + 
      2*(At12L*PDstandardNth2beta1 + At22L*PDstandardNth2beta2 + 
      At23L*PDstandardNth2beta3) - 
      0.666666666666666666666666666667*At22L*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3) + em4phi*(Ats22 - 
      0.333333333333333333333333333333*g22*trAts);
    
    CCTK_REAL At23rhsL CCTK_ATTRIBUTE_UNUSED = alphaL*(At23L*trKL - 
      2*(At12L*Atm13 + At22L*Atm23 + At23L*Atm33)) + 
      At13L*PDstandardNth2beta1 + At33L*PDstandardNth2beta3 + 
      At12L*PDstandardNth3beta1 + At22L*PDstandardNth3beta2 + 
      At23L*(PDstandardNth2beta2 + PDstandardNth3beta3 - 
      0.666666666666666666666666666667*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3)) + em4phi*(Ats23 - 
      0.333333333333333333333333333333*g23*trAts);
    
    CCTK_REAL At33rhsL CCTK_ATTRIBUTE_UNUSED = alphaL*(At33L*trKL - 
      2*(At13L*Atm13 + At23L*Atm23 + At33L*Atm33)) - 
      0.666666666666666666666666666667*At33L*(PDstandardNth1beta1 + 
      PDstandardNth2beta2 + PDstandardNth3beta3) + 
      2*(At13L*PDstandardNth3beta1 + At23L*PDstandardNth3beta2 + 
      At33L*PDstandardNth3beta3) + em4phi*(Ats33 - 
      0.333333333333333333333333333333*g33*trAts);
    
    CCTK_REAL alpharhsL CCTK_ATTRIBUTE_UNUSED = 
      -(pow(alphaL,ToReal(harmonicN))*ToReal(harmonicF)*((trKL + (-1 + 
      alphaL)*ToReal(AlphaDriver))*(1 - ToReal(LapseACoeff)) + 
      AL*ToReal(LapseACoeff)));
    
    CCTK_REAL ArhsL CCTK_ATTRIBUTE_UNUSED = (dottrK - 
      AL*ToReal(AlphaDriver))*ToReal(LapseACoeff);
    
    CCTK_REAL eta CCTK_ATTRIBUTE_UNUSED = 
      INV(fmax(rL,ToReal(SpatialBetaDriverRadius)))*ToReal(SpatialBetaDriverRadius);
    
    CCTK_REAL theta CCTK_ATTRIBUTE_UNUSED = fmin(1,exp(1 - 
      rL*INV(ToReal(SpatialShiftGammaCoeffRadius))));
    
    CCTK_REAL Ddetgt1 CCTK_ATTRIBUTE_UNUSED = gtu11*PDstandardNth1gt11 + 
      gtu22*PDstandardNth1gt22 + 2*(gtu12*PDstandardNth1gt12 + 
      gtu13*PDstandardNth1gt13 + gtu23*PDstandardNth1gt23) + 
      gtu33*PDstandardNth1gt33;
    
    CCTK_REAL Ddetgt2 CCTK_ATTRIBUTE_UNUSED = gtu11*PDstandardNth2gt11 + 
      gtu22*PDstandardNth2gt22 + 2*(gtu12*PDstandardNth2gt12 + 
      gtu13*PDstandardNth2gt13 + gtu23*PDstandardNth2gt23) + 
      gtu33*PDstandardNth2gt33;
    
    CCTK_REAL Ddetgt3 CCTK_ATTRIBUTE_UNUSED = gtu11*PDstandardNth3gt11 + 
      gtu22*PDstandardNth3gt22 + 2*(gtu12*PDstandardNth3gt12 + 
      gtu13*PDstandardNth3gt13 + gtu23*PDstandardNth3gt23) + 
      gtu33*PDstandardNth3gt33;
    
    CCTK_REAL beta1rhsL CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL beta2rhsL CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL beta3rhsL CCTK_ATTRIBUTE_UNUSED;
    
    if (harmonicShift)
    {
      beta1rhsL = -0.5*alphaL*em4phi*(gtu11*(2*PDstandardNth1alpha + 
        alphaL*(Ddetgt1 - 2*(gtu11*PDstandardNth1gt11 + 
        gtu12*(PDstandardNth1gt12 + PDstandardNth2gt11) + 
        gtu22*PDstandardNth2gt12 + gtu13*(PDstandardNth1gt13 + 
        PDstandardNth3gt11) + gtu23*(PDstandardNth2gt13 + PDstandardNth3gt12) + 
        gtu33*PDstandardNth3gt13 + 
        PDstandardNth1phi*IfThen(ToReal(conformalMethod) == 1,INV(phiL),-2)))) 
        + gtu12*(2*PDstandardNth2alpha + alphaL*(Ddetgt2 - 
        2*(gtu11*PDstandardNth1gt12 + gtu12*(PDstandardNth1gt22 + 
        PDstandardNth2gt12) + gtu22*PDstandardNth2gt22 + 
        gtu13*(PDstandardNth1gt23 + PDstandardNth3gt12) + 
        gtu23*(PDstandardNth2gt23 + PDstandardNth3gt22) + 
        gtu33*PDstandardNth3gt23 + 
        PDstandardNth2phi*IfThen(ToReal(conformalMethod) == 1,INV(phiL),-2)))) 
        + gtu13*(2*PDstandardNth3alpha + alphaL*(Ddetgt3 - 
        2*(gtu11*PDstandardNth1gt13 + gtu12*(PDstandardNth1gt23 + 
        PDstandardNth2gt13) + gtu22*PDstandardNth2gt23 + 
        gtu13*(PDstandardNth1gt33 + PDstandardNth3gt13) + 
        gtu23*(PDstandardNth2gt33 + PDstandardNth3gt23) + 
        gtu33*PDstandardNth3gt33 + 
        PDstandardNth3phi*IfThen(ToReal(conformalMethod) == 
        1,INV(phiL),-2)))));
      
      beta2rhsL = -0.5*alphaL*em4phi*(gtu12*(2*PDstandardNth1alpha + 
        alphaL*(Ddetgt1 - 2*(gtu11*PDstandardNth1gt11 + 
        gtu12*(PDstandardNth1gt12 + PDstandardNth2gt11) + 
        gtu22*PDstandardNth2gt12 + gtu13*(PDstandardNth1gt13 + 
        PDstandardNth3gt11) + gtu23*(PDstandardNth2gt13 + PDstandardNth3gt12) + 
        gtu33*PDstandardNth3gt13 + 
        PDstandardNth1phi*IfThen(ToReal(conformalMethod) == 1,INV(phiL),-2)))) 
        + gtu22*(2*PDstandardNth2alpha + alphaL*(Ddetgt2 - 
        2*(gtu11*PDstandardNth1gt12 + gtu12*(PDstandardNth1gt22 + 
        PDstandardNth2gt12) + gtu22*PDstandardNth2gt22 + 
        gtu13*(PDstandardNth1gt23 + PDstandardNth3gt12) + 
        gtu23*(PDstandardNth2gt23 + PDstandardNth3gt22) + 
        gtu33*PDstandardNth3gt23 + 
        PDstandardNth2phi*IfThen(ToReal(conformalMethod) == 1,INV(phiL),-2)))) 
        + gtu23*(2*PDstandardNth3alpha + alphaL*(Ddetgt3 - 
        2*(gtu11*PDstandardNth1gt13 + gtu12*(PDstandardNth1gt23 + 
        PDstandardNth2gt13) + gtu22*PDstandardNth2gt23 + 
        gtu13*(PDstandardNth1gt33 + PDstandardNth3gt13) + 
        gtu23*(PDstandardNth2gt33 + PDstandardNth3gt23) + 
        gtu33*PDstandardNth3gt33 + 
        PDstandardNth3phi*IfThen(ToReal(conformalMethod) == 
        1,INV(phiL),-2)))));
      
      beta3rhsL = -0.5*alphaL*em4phi*(gtu13*(2*PDstandardNth1alpha + 
        alphaL*(Ddetgt1 - 2*(gtu11*PDstandardNth1gt11 + 
        gtu12*(PDstandardNth1gt12 + PDstandardNth2gt11) + 
        gtu22*PDstandardNth2gt12 + gtu13*(PDstandardNth1gt13 + 
        PDstandardNth3gt11) + gtu23*(PDstandardNth2gt13 + PDstandardNth3gt12) + 
        gtu33*PDstandardNth3gt13 + 
        PDstandardNth1phi*IfThen(ToReal(conformalMethod) == 1,INV(phiL),-2)))) 
        + gtu23*(2*PDstandardNth2alpha + alphaL*(Ddetgt2 - 
        2*(gtu11*PDstandardNth1gt12 + gtu12*(PDstandardNth1gt22 + 
        PDstandardNth2gt12) + gtu22*PDstandardNth2gt22 + 
        gtu13*(PDstandardNth1gt23 + PDstandardNth3gt12) + 
        gtu23*(PDstandardNth2gt23 + PDstandardNth3gt22) + 
        gtu33*PDstandardNth3gt23 + 
        PDstandardNth2phi*IfThen(ToReal(conformalMethod) == 1,INV(phiL),-2)))) 
        + gtu33*(2*PDstandardNth3alpha + alphaL*(Ddetgt3 - 
        2*(gtu11*PDstandardNth1gt13 + gtu12*(PDstandardNth1gt23 + 
        PDstandardNth2gt13) + gtu22*PDstandardNth2gt23 + 
        gtu13*(PDstandardNth1gt33 + PDstandardNth3gt13) + 
        gtu23*(PDstandardNth2gt33 + PDstandardNth3gt23) + 
        gtu33*PDstandardNth3gt33 + 
        PDstandardNth3phi*IfThen(ToReal(conformalMethod) == 
        1,INV(phiL),-2)))));
    }
    else
    {
      beta1rhsL = theta*(Xt1L + beta1L*eta*ToReal(BetaDriver)*(-1 + 
        ToReal(ShiftBCoeff)) + (B1L - 
        Xt1L)*ToReal(ShiftBCoeff))*ToReal(ShiftGammaCoeff);
      
      beta2rhsL = theta*(Xt2L + beta2L*eta*ToReal(BetaDriver)*(-1 + 
        ToReal(ShiftBCoeff)) + (B2L - 
        Xt2L)*ToReal(ShiftBCoeff))*ToReal(ShiftGammaCoeff);
      
      beta3rhsL = theta*(Xt3L + beta3L*eta*ToReal(BetaDriver)*(-1 + 
        ToReal(ShiftBCoeff)) + (B3L - 
        Xt3L)*ToReal(ShiftBCoeff))*ToReal(ShiftGammaCoeff);
    }
    
    CCTK_REAL B1rhsL CCTK_ATTRIBUTE_UNUSED = (dotXt1 - 
      B1L*eta*ToReal(BetaDriver))*ToReal(ShiftBCoeff);
    
    CCTK_REAL B2rhsL CCTK_ATTRIBUTE_UNUSED = (dotXt2 - 
      B2L*eta*ToReal(BetaDriver))*ToReal(ShiftBCoeff);
    
    CCTK_REAL B3rhsL CCTK_ATTRIBUTE_UNUSED = (dotXt3 - 
      B3L*eta*ToReal(BetaDriver))*ToReal(ShiftBCoeff);
    
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
  CCTK_ENDLOOP3(ML_BSSN_RHS);
}

extern "C" void ML_BSSN_RHS(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Entering ML_BSSN_RHS_Body");
  }
  
  if (cctk_iteration % ML_BSSN_RHS_calc_every != ML_BSSN_RHS_calc_offset)
  {
    return;
  }
  
  const char* const groups[] = {
    "grid::coordinates",
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
  GenericFD_AssertGroupStorage(cctkGH, "ML_BSSN_RHS", 19, groups);
  
  switch (fdOrder)
  {
    case 2:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_RHS", 1, 1, 1);
      break;
    }
    
    case 4:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_RHS", 2, 2, 2);
      break;
    }
    
    case 6:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_RHS", 3, 3, 3);
      break;
    }
    
    case 8:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_RHS", 4, 4, 4);
      break;
    }
    default:
      CCTK_BUILTIN_UNREACHABLE();
  }
  
  GenericFD_LoopOverInterior(cctkGH, ML_BSSN_RHS_Body);
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Leaving ML_BSSN_RHS_Body");
  }
}
