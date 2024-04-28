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

extern "C" void ML_BSSN_constraints_SelectBCs(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  if (cctk_iteration % ML_BSSN_constraints_calc_every != ML_BSSN_constraints_calc_offset)
    return;
  CCTK_INT ierr CCTK_ATTRIBUTE_UNUSED = 0;
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_cons_detg","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_cons_detg.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_cons_Gamma","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_cons_Gamma.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_cons_traceA","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_cons_traceA.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_Ham","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_Ham.");
  ierr = Boundary_SelectGroupForBC(cctkGH, CCTK_ALL_FACES, GenericFD_GetBoundaryWidth(cctkGH), -1 /* no table */, "ML_BSSN::ML_mom","flat");
  if (ierr < 0)
    CCTK_WARN(1, "Failed to register flat BC for ML_BSSN::ML_mom.");
  return;
}

static void ML_BSSN_constraints_Body(const cGH* restrict const cctkGH, const int dir, const int face, const CCTK_REAL normal[3], const CCTK_REAL tangentA[3], const CCTK_REAL tangentB[3], const int imin[3], const int imax[3], const int n_subblock_gfs, CCTK_REAL* restrict const subblock_gfs[])
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
  CCTK_LOOP3(ML_BSSN_constraints,
    i,j,k, imin0,imin1,imin2, imax0,imax1,imax2,
    cctk_ash[0],cctk_ash[1],cctk_ash[2])
  {
    const ptrdiff_t index CCTK_ATTRIBUTE_UNUSED = di*i + dj*j + dk*k;
    // ++vec_iter_counter;
    
    /* Assign local copies of grid functions */
    
    CCTK_REAL At11L CCTK_ATTRIBUTE_UNUSED = At11[index];
    CCTK_REAL At12L CCTK_ATTRIBUTE_UNUSED = At12[index];
    CCTK_REAL At13L CCTK_ATTRIBUTE_UNUSED = At13[index];
    CCTK_REAL At22L CCTK_ATTRIBUTE_UNUSED = At22[index];
    CCTK_REAL At23L CCTK_ATTRIBUTE_UNUSED = At23[index];
    CCTK_REAL At33L CCTK_ATTRIBUTE_UNUSED = At33[index];
    CCTK_REAL gt11L CCTK_ATTRIBUTE_UNUSED = gt11[index];
    CCTK_REAL gt12L CCTK_ATTRIBUTE_UNUSED = gt12[index];
    CCTK_REAL gt13L CCTK_ATTRIBUTE_UNUSED = gt13[index];
    CCTK_REAL gt22L CCTK_ATTRIBUTE_UNUSED = gt22[index];
    CCTK_REAL gt23L CCTK_ATTRIBUTE_UNUSED = gt23[index];
    CCTK_REAL gt33L CCTK_ATTRIBUTE_UNUSED = gt33[index];
    CCTK_REAL phiL CCTK_ATTRIBUTE_UNUSED = phi[index];
    CCTK_REAL trKL CCTK_ATTRIBUTE_UNUSED = trK[index];
    CCTK_REAL Xt1L CCTK_ATTRIBUTE_UNUSED = Xt1[index];
    CCTK_REAL Xt2L CCTK_ATTRIBUTE_UNUSED = Xt2[index];
    CCTK_REAL Xt3L CCTK_ATTRIBUTE_UNUSED = Xt3[index];
    
    
    /* Include user supplied include files */
    
    /* Precompute derivatives */
    CCTK_REAL PDstandardNth1At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth1At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth2At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDstandardNth3At33 CCTK_ATTRIBUTE_UNUSED;
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
        PDstandardNth1At11 = PDstandardNthfdOrder21(&At11[index]);
        PDstandardNth2At11 = PDstandardNthfdOrder22(&At11[index]);
        PDstandardNth3At11 = PDstandardNthfdOrder23(&At11[index]);
        PDstandardNth1At12 = PDstandardNthfdOrder21(&At12[index]);
        PDstandardNth2At12 = PDstandardNthfdOrder22(&At12[index]);
        PDstandardNth3At12 = PDstandardNthfdOrder23(&At12[index]);
        PDstandardNth1At13 = PDstandardNthfdOrder21(&At13[index]);
        PDstandardNth2At13 = PDstandardNthfdOrder22(&At13[index]);
        PDstandardNth3At13 = PDstandardNthfdOrder23(&At13[index]);
        PDstandardNth1At22 = PDstandardNthfdOrder21(&At22[index]);
        PDstandardNth2At22 = PDstandardNthfdOrder22(&At22[index]);
        PDstandardNth3At22 = PDstandardNthfdOrder23(&At22[index]);
        PDstandardNth1At23 = PDstandardNthfdOrder21(&At23[index]);
        PDstandardNth2At23 = PDstandardNthfdOrder22(&At23[index]);
        PDstandardNth3At23 = PDstandardNthfdOrder23(&At23[index]);
        PDstandardNth1At33 = PDstandardNthfdOrder21(&At33[index]);
        PDstandardNth2At33 = PDstandardNthfdOrder22(&At33[index]);
        PDstandardNth3At33 = PDstandardNthfdOrder23(&At33[index]);
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
        PDstandardNth1At11 = PDstandardNthfdOrder41(&At11[index]);
        PDstandardNth2At11 = PDstandardNthfdOrder42(&At11[index]);
        PDstandardNth3At11 = PDstandardNthfdOrder43(&At11[index]);
        PDstandardNth1At12 = PDstandardNthfdOrder41(&At12[index]);
        PDstandardNth2At12 = PDstandardNthfdOrder42(&At12[index]);
        PDstandardNth3At12 = PDstandardNthfdOrder43(&At12[index]);
        PDstandardNth1At13 = PDstandardNthfdOrder41(&At13[index]);
        PDstandardNth2At13 = PDstandardNthfdOrder42(&At13[index]);
        PDstandardNth3At13 = PDstandardNthfdOrder43(&At13[index]);
        PDstandardNth1At22 = PDstandardNthfdOrder41(&At22[index]);
        PDstandardNth2At22 = PDstandardNthfdOrder42(&At22[index]);
        PDstandardNth3At22 = PDstandardNthfdOrder43(&At22[index]);
        PDstandardNth1At23 = PDstandardNthfdOrder41(&At23[index]);
        PDstandardNth2At23 = PDstandardNthfdOrder42(&At23[index]);
        PDstandardNth3At23 = PDstandardNthfdOrder43(&At23[index]);
        PDstandardNth1At33 = PDstandardNthfdOrder41(&At33[index]);
        PDstandardNth2At33 = PDstandardNthfdOrder42(&At33[index]);
        PDstandardNth3At33 = PDstandardNthfdOrder43(&At33[index]);
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
        PDstandardNth1At11 = PDstandardNthfdOrder61(&At11[index]);
        PDstandardNth2At11 = PDstandardNthfdOrder62(&At11[index]);
        PDstandardNth3At11 = PDstandardNthfdOrder63(&At11[index]);
        PDstandardNth1At12 = PDstandardNthfdOrder61(&At12[index]);
        PDstandardNth2At12 = PDstandardNthfdOrder62(&At12[index]);
        PDstandardNth3At12 = PDstandardNthfdOrder63(&At12[index]);
        PDstandardNth1At13 = PDstandardNthfdOrder61(&At13[index]);
        PDstandardNth2At13 = PDstandardNthfdOrder62(&At13[index]);
        PDstandardNth3At13 = PDstandardNthfdOrder63(&At13[index]);
        PDstandardNth1At22 = PDstandardNthfdOrder61(&At22[index]);
        PDstandardNth2At22 = PDstandardNthfdOrder62(&At22[index]);
        PDstandardNth3At22 = PDstandardNthfdOrder63(&At22[index]);
        PDstandardNth1At23 = PDstandardNthfdOrder61(&At23[index]);
        PDstandardNth2At23 = PDstandardNthfdOrder62(&At23[index]);
        PDstandardNth3At23 = PDstandardNthfdOrder63(&At23[index]);
        PDstandardNth1At33 = PDstandardNthfdOrder61(&At33[index]);
        PDstandardNth2At33 = PDstandardNthfdOrder62(&At33[index]);
        PDstandardNth3At33 = PDstandardNthfdOrder63(&At33[index]);
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
        PDstandardNth1At11 = PDstandardNthfdOrder81(&At11[index]);
        PDstandardNth2At11 = PDstandardNthfdOrder82(&At11[index]);
        PDstandardNth3At11 = PDstandardNthfdOrder83(&At11[index]);
        PDstandardNth1At12 = PDstandardNthfdOrder81(&At12[index]);
        PDstandardNth2At12 = PDstandardNthfdOrder82(&At12[index]);
        PDstandardNth3At12 = PDstandardNthfdOrder83(&At12[index]);
        PDstandardNth1At13 = PDstandardNthfdOrder81(&At13[index]);
        PDstandardNth2At13 = PDstandardNthfdOrder82(&At13[index]);
        PDstandardNth3At13 = PDstandardNthfdOrder83(&At13[index]);
        PDstandardNth1At22 = PDstandardNthfdOrder81(&At22[index]);
        PDstandardNth2At22 = PDstandardNthfdOrder82(&At22[index]);
        PDstandardNth3At22 = PDstandardNthfdOrder83(&At22[index]);
        PDstandardNth1At23 = PDstandardNthfdOrder81(&At23[index]);
        PDstandardNth2At23 = PDstandardNthfdOrder82(&At23[index]);
        PDstandardNth3At23 = PDstandardNthfdOrder83(&At23[index]);
        PDstandardNth1At33 = PDstandardNthfdOrder81(&At33[index]);
        PDstandardNth2At33 = PDstandardNthfdOrder82(&At33[index]);
        PDstandardNth3At33 = PDstandardNthfdOrder83(&At33[index]);
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
    
    CCTK_REAL R11 CCTK_ATTRIBUTE_UNUSED = Rphi11 + Rt11;
    
    CCTK_REAL R12 CCTK_ATTRIBUTE_UNUSED = Rphi12 + Rt12;
    
    CCTK_REAL R13 CCTK_ATTRIBUTE_UNUSED = Rphi13 + Rt13;
    
    CCTK_REAL R22 CCTK_ATTRIBUTE_UNUSED = Rphi22 + Rt22;
    
    CCTK_REAL R23 CCTK_ATTRIBUTE_UNUSED = Rphi23 + Rt23;
    
    CCTK_REAL R33 CCTK_ATTRIBUTE_UNUSED = Rphi33 + Rt33;
    
    CCTK_REAL trR CCTK_ATTRIBUTE_UNUSED = gu11*R11 + gu22*R22 + 
      2*(gu12*R12 + gu13*R13 + gu23*R23) + gu33*R33;
    
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
    
    CCTK_REAL HL CCTK_ATTRIBUTE_UNUSED = -2*(Atm12*Atm21 + Atm13*Atm31 + 
      Atm23*Atm32) + trR + 0.666666666666666666666666666667*SQR(trKL) - 
      SQR(Atm11) - SQR(Atm22) - SQR(Atm33);
    
    CCTK_REAL M1L CCTK_ATTRIBUTE_UNUSED = (-(At22L*Gt212) + At12L*(-Gt112 
      - Gt222) - At23L*Gt312)*gtu22 + (6*At13L*cdphi2 - At23L*Gt212 - 
      At33L*Gt312)*gtu23 + (6*At12L*cdphi3 - At22L*Gt213 - At23L*Gt313)*gtu23 
      + At12L*((6*cdphi1 - Gt111)*gtu12 - 3*Gt213*gtu13 + 6*cdphi2*gtu22 - 
      Gt113*gtu23) - 2*(At12L*Gt211*gtu11 + At13L*Gt311*gtu11 + 
      At11L*Gt123*gtu23) + (6*At13L*cdphi3 - At23L*Gt213 - At33L*Gt313)*gtu33 
      + At13L*(-2*Gt323*gtu23 - Gt113*gtu33) + At11L*(6*cdphi1*gtu11 - 
      2*Gt111*gtu11 + 6*cdphi2*gtu12 + 6*cdphi3*gtu13 - Gt122*gtu22 - 
      Gt133*gtu33) + At12L*(-2*Gt223*gtu23 - Gt233*gtu33) + At13L*((6*cdphi1 
      - Gt111)*gtu13 - Gt322*gtu22 - Gt112*gtu23 - Gt333*gtu33) + 
      gtu11*PDstandardNth1At11 + gtu12*(-3*At11L*Gt112 - At22L*Gt211 - 
      3*At12L*Gt212 - At23L*Gt311 - 3*At13L*Gt312 + PDstandardNth1At12) + 
      gtu13*(-3*At11L*Gt113 - At23L*Gt211 - At33L*Gt311 - 3*At13L*Gt313 + 
      PDstandardNth1At13) - 
      0.666666666666666666666666666667*PDstandardNth1trK + 
      gtu12*PDstandardNth2At11 + gtu22*PDstandardNth2At12 + 
      gtu23*PDstandardNth2At13 + gtu13*PDstandardNth3At11 + 
      gtu23*PDstandardNth3At12 + gtu33*PDstandardNth3At13;
    
    CCTK_REAL M2L CCTK_ATTRIBUTE_UNUSED = (-(At11L*Gt112) - At22L*Gt211 - 
      At12L*Gt212 - At23L*Gt311 - At13L*Gt312)*gtu11 + (6*At22L*cdphi1 - 
      At11L*Gt122)*gtu12 + (6*At12L*cdphi2 - At13L*Gt322)*gtu12 + 
      (6*At23L*cdphi1 - At11L*Gt123)*gtu13 + (6*At12L*cdphi3 - 
      At13L*Gt323)*gtu13 + Gt112*(-3*At12L*gtu12 - At13L*gtu13) + 
      Gt212*(-3*At22L*gtu12 - At23L*gtu13) + Gt312*(-3*At23L*gtu12 - 
      At33L*gtu13) + At12L*((6*cdphi1 - Gt111)*gtu11 - Gt222*gtu12 - 
      Gt223*gtu13) + 6*At22L*cdphi2*gtu22 - 2*At22L*Gt222*gtu22 + 
      6*At22L*cdphi3*gtu23 - 3*At22L*Gt223*gtu23 + Gt122*(-2*At12L*gtu22 - 
      At13L*gtu23) + Gt322*(-2*At23L*gtu22 - At33L*gtu23) + 
      6*At23L*cdphi3*gtu33 + Gt123*(-3*At12L*gtu23 - At13L*gtu33) + 
      Gt323*(-3*At23L*gtu23 - At33L*gtu33) + At12L*(-2*Gt113*gtu13 - 
      Gt133*gtu33) + At23L*(6*cdphi2*gtu23 - Gt223*gtu33) + 
      At22L*(-2*Gt213*gtu13 - Gt233*gtu33) + At23L*(-2*Gt313*gtu13 - 
      Gt222*gtu23 - Gt333*gtu33) + gtu11*PDstandardNth1At12 + 
      gtu12*PDstandardNth1At22 + gtu13*PDstandardNth1At23 + 
      gtu12*PDstandardNth2At12 + gtu22*PDstandardNth2At22 + 
      gtu23*PDstandardNth2At23 - 
      0.666666666666666666666666666667*PDstandardNth2trK + 
      gtu13*PDstandardNth3At12 + gtu23*PDstandardNth3At22 + 
      gtu33*PDstandardNth3At23;
    
    CCTK_REAL M3L CCTK_ATTRIBUTE_UNUSED = (-(At11L*Gt113) - At23L*Gt211 - 
      At12L*Gt213 - At33L*Gt311 - At13L*Gt313)*gtu11 + (-2*At13L*Gt112 - 
      At22L*Gt213)*gtu12 + (6*At23L*cdphi1 + At12L*(-Gt113 - Gt223))*gtu12 + 
      (6*At13L*cdphi2 - At11L*Gt123 - At23L*Gt313)*gtu12 + (6*At33L*cdphi1 - 
      At11L*Gt133)*gtu13 + (6*At13L*cdphi3 - At12L*Gt233)*gtu13 - 
      3*At33L*Gt313*gtu13 + At13L*((6*cdphi1 - Gt111)*gtu11 - Gt323*gtu12 - 
      Gt333*gtu13) + (6*At23L*cdphi2 - At12L*Gt123 - At22L*Gt223)*gtu22 + 
      At13L*(-3*Gt113*gtu13 - Gt122*gtu22) + At33L*(-2*Gt312*gtu12 - 
      Gt322*gtu22) + At23L*(-3*Gt213*gtu13 - Gt323*gtu22) - 
      3*At13L*Gt123*gtu23 + (6*At33L*cdphi2 - At12L*Gt133)*gtu23 - 
      3*At23L*Gt223*gtu23 + (6*At23L*cdphi3 - At22L*Gt233)*gtu23 - 
      3*At33L*Gt323*gtu23 + At23L*(-2*Gt212*gtu12 - Gt222*gtu22 - 
      Gt333*gtu23) + 6*At33L*cdphi3*gtu33 - 2*At13L*Gt133*gtu33 - 
      2*At23L*Gt233*gtu33 - 2*At33L*Gt333*gtu33 + gtu11*PDstandardNth1At13 + 
      gtu12*PDstandardNth1At23 + gtu13*PDstandardNth1At33 + 
      gtu12*PDstandardNth2At13 + gtu22*PDstandardNth2At23 + 
      gtu23*PDstandardNth2At33 + gtu13*PDstandardNth3At13 + 
      gtu23*PDstandardNth3At23 + gtu33*PDstandardNth3At33 - 
      0.666666666666666666666666666667*PDstandardNth3trK;
    
    CCTK_REAL cSL CCTK_ATTRIBUTE_UNUSED = log(detgt);
    
    CCTK_REAL cXt1L CCTK_ATTRIBUTE_UNUSED = -Xt1L + Gt111*gtu11 + 
      Gt122*gtu22 + 2*(Gt112*gtu12 + Gt113*gtu13 + Gt123*gtu23) + 
      Gt133*gtu33;
    
    CCTK_REAL cXt2L CCTK_ATTRIBUTE_UNUSED = -Xt2L + Gt211*gtu11 + 
      Gt222*gtu22 + 2*(Gt212*gtu12 + Gt213*gtu13 + Gt223*gtu23) + 
      Gt233*gtu33;
    
    CCTK_REAL cXt3L CCTK_ATTRIBUTE_UNUSED = -Xt3L + Gt311*gtu11 + 
      Gt322*gtu22 + 2*(Gt312*gtu12 + Gt313*gtu13 + Gt323*gtu23) + 
      Gt333*gtu33;
    
    CCTK_REAL cAL CCTK_ATTRIBUTE_UNUSED = At11L*gtu11 + At22L*gtu22 + 
      2*(At12L*gtu12 + At13L*gtu13 + At23L*gtu23) + At33L*gtu33;
    
    /* Copy local copies back to grid functions */
    cA[index] = cAL;
    cS[index] = cSL;
    cXt1[index] = cXt1L;
    cXt2[index] = cXt2L;
    cXt3[index] = cXt3L;
    H[index] = HL;
    M1[index] = M1L;
    M2[index] = M2L;
    M3[index] = M3L;
  }
  CCTK_ENDLOOP3(ML_BSSN_constraints);
}

extern "C" void ML_BSSN_constraints(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Entering ML_BSSN_constraints_Body");
  }
  
  if (cctk_iteration % ML_BSSN_constraints_calc_every != ML_BSSN_constraints_calc_offset)
  {
    return;
  }
  
  const char* const groups[] = {
    "ML_BSSN::ML_cons_detg",
    "ML_BSSN::ML_cons_Gamma",
    "ML_BSSN::ML_cons_traceA",
    "ML_BSSN::ML_curv",
    "ML_BSSN::ML_Gamma",
    "ML_BSSN::ML_Ham",
    "ML_BSSN::ML_log_confac",
    "ML_BSSN::ML_metric",
    "ML_BSSN::ML_mom",
    "ML_BSSN::ML_trace_curv"};
  GenericFD_AssertGroupStorage(cctkGH, "ML_BSSN_constraints", 10, groups);
  
  switch (fdOrder)
  {
    case 2:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_constraints", 1, 1, 1);
      break;
    }
    
    case 4:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_constraints", 2, 2, 2);
      break;
    }
    
    case 6:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_constraints", 3, 3, 3);
      break;
    }
    
    case 8:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_constraints", 4, 4, 4);
      break;
    }
    default:
      CCTK_BUILTIN_UNREACHABLE();
  }
  
  GenericFD_LoopOverInterior(cctkGH, ML_BSSN_constraints_Body);
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Leaving ML_BSSN_constraints_Body");
  }
}
