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

extern "C" void ML_BSSN_Dissipation_SelectBCs(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  if (cctk_iteration % ML_BSSN_Dissipation_calc_every != ML_BSSN_Dissipation_calc_offset)
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

static void ML_BSSN_Dissipation_Body(const cGH* restrict const cctkGH, const int dir, const int face, const CCTK_REAL normal[3], const CCTK_REAL tangentA[3], const CCTK_REAL tangentB[3], const int imin[3], const int imax[3], const int n_subblock_gfs, CCTK_REAL* restrict const subblock_gfs[])
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
  CCTK_LOOP3(ML_BSSN_Dissipation,
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
    CCTK_REAL PDdissipationNth1A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3A CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3alpha CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3At11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3At12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3At13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3At22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3At23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3At33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3B1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3B2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3B3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3beta1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3beta2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3beta3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3gt11 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3gt12 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3gt13 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3gt22 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3gt23 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3gt33 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3phi CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3trK CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3Xt1 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3Xt2 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth1Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth2Xt3 CCTK_ATTRIBUTE_UNUSED;
    CCTK_REAL PDdissipationNth3Xt3 CCTK_ATTRIBUTE_UNUSED;
    
    switch (fdOrder)
    {
      case 2:
      {
        PDdissipationNth1A = PDdissipationNthfdOrder21(&A[index]);
        PDdissipationNth2A = PDdissipationNthfdOrder22(&A[index]);
        PDdissipationNth3A = PDdissipationNthfdOrder23(&A[index]);
        PDdissipationNth1alpha = PDdissipationNthfdOrder21(&alpha[index]);
        PDdissipationNth2alpha = PDdissipationNthfdOrder22(&alpha[index]);
        PDdissipationNth3alpha = PDdissipationNthfdOrder23(&alpha[index]);
        PDdissipationNth1At11 = PDdissipationNthfdOrder21(&At11[index]);
        PDdissipationNth2At11 = PDdissipationNthfdOrder22(&At11[index]);
        PDdissipationNth3At11 = PDdissipationNthfdOrder23(&At11[index]);
        PDdissipationNth1At12 = PDdissipationNthfdOrder21(&At12[index]);
        PDdissipationNth2At12 = PDdissipationNthfdOrder22(&At12[index]);
        PDdissipationNth3At12 = PDdissipationNthfdOrder23(&At12[index]);
        PDdissipationNth1At13 = PDdissipationNthfdOrder21(&At13[index]);
        PDdissipationNth2At13 = PDdissipationNthfdOrder22(&At13[index]);
        PDdissipationNth3At13 = PDdissipationNthfdOrder23(&At13[index]);
        PDdissipationNth1At22 = PDdissipationNthfdOrder21(&At22[index]);
        PDdissipationNth2At22 = PDdissipationNthfdOrder22(&At22[index]);
        PDdissipationNth3At22 = PDdissipationNthfdOrder23(&At22[index]);
        PDdissipationNth1At23 = PDdissipationNthfdOrder21(&At23[index]);
        PDdissipationNth2At23 = PDdissipationNthfdOrder22(&At23[index]);
        PDdissipationNth3At23 = PDdissipationNthfdOrder23(&At23[index]);
        PDdissipationNth1At33 = PDdissipationNthfdOrder21(&At33[index]);
        PDdissipationNth2At33 = PDdissipationNthfdOrder22(&At33[index]);
        PDdissipationNth3At33 = PDdissipationNthfdOrder23(&At33[index]);
        PDdissipationNth1B1 = PDdissipationNthfdOrder21(&B1[index]);
        PDdissipationNth2B1 = PDdissipationNthfdOrder22(&B1[index]);
        PDdissipationNth3B1 = PDdissipationNthfdOrder23(&B1[index]);
        PDdissipationNth1B2 = PDdissipationNthfdOrder21(&B2[index]);
        PDdissipationNth2B2 = PDdissipationNthfdOrder22(&B2[index]);
        PDdissipationNth3B2 = PDdissipationNthfdOrder23(&B2[index]);
        PDdissipationNth1B3 = PDdissipationNthfdOrder21(&B3[index]);
        PDdissipationNth2B3 = PDdissipationNthfdOrder22(&B3[index]);
        PDdissipationNth3B3 = PDdissipationNthfdOrder23(&B3[index]);
        PDdissipationNth1beta1 = PDdissipationNthfdOrder21(&beta1[index]);
        PDdissipationNth2beta1 = PDdissipationNthfdOrder22(&beta1[index]);
        PDdissipationNth3beta1 = PDdissipationNthfdOrder23(&beta1[index]);
        PDdissipationNth1beta2 = PDdissipationNthfdOrder21(&beta2[index]);
        PDdissipationNth2beta2 = PDdissipationNthfdOrder22(&beta2[index]);
        PDdissipationNth3beta2 = PDdissipationNthfdOrder23(&beta2[index]);
        PDdissipationNth1beta3 = PDdissipationNthfdOrder21(&beta3[index]);
        PDdissipationNth2beta3 = PDdissipationNthfdOrder22(&beta3[index]);
        PDdissipationNth3beta3 = PDdissipationNthfdOrder23(&beta3[index]);
        PDdissipationNth1gt11 = PDdissipationNthfdOrder21(&gt11[index]);
        PDdissipationNth2gt11 = PDdissipationNthfdOrder22(&gt11[index]);
        PDdissipationNth3gt11 = PDdissipationNthfdOrder23(&gt11[index]);
        PDdissipationNth1gt12 = PDdissipationNthfdOrder21(&gt12[index]);
        PDdissipationNth2gt12 = PDdissipationNthfdOrder22(&gt12[index]);
        PDdissipationNth3gt12 = PDdissipationNthfdOrder23(&gt12[index]);
        PDdissipationNth1gt13 = PDdissipationNthfdOrder21(&gt13[index]);
        PDdissipationNth2gt13 = PDdissipationNthfdOrder22(&gt13[index]);
        PDdissipationNth3gt13 = PDdissipationNthfdOrder23(&gt13[index]);
        PDdissipationNth1gt22 = PDdissipationNthfdOrder21(&gt22[index]);
        PDdissipationNth2gt22 = PDdissipationNthfdOrder22(&gt22[index]);
        PDdissipationNth3gt22 = PDdissipationNthfdOrder23(&gt22[index]);
        PDdissipationNth1gt23 = PDdissipationNthfdOrder21(&gt23[index]);
        PDdissipationNth2gt23 = PDdissipationNthfdOrder22(&gt23[index]);
        PDdissipationNth3gt23 = PDdissipationNthfdOrder23(&gt23[index]);
        PDdissipationNth1gt33 = PDdissipationNthfdOrder21(&gt33[index]);
        PDdissipationNth2gt33 = PDdissipationNthfdOrder22(&gt33[index]);
        PDdissipationNth3gt33 = PDdissipationNthfdOrder23(&gt33[index]);
        PDdissipationNth1phi = PDdissipationNthfdOrder21(&phi[index]);
        PDdissipationNth2phi = PDdissipationNthfdOrder22(&phi[index]);
        PDdissipationNth3phi = PDdissipationNthfdOrder23(&phi[index]);
        PDdissipationNth1trK = PDdissipationNthfdOrder21(&trK[index]);
        PDdissipationNth2trK = PDdissipationNthfdOrder22(&trK[index]);
        PDdissipationNth3trK = PDdissipationNthfdOrder23(&trK[index]);
        PDdissipationNth1Xt1 = PDdissipationNthfdOrder21(&Xt1[index]);
        PDdissipationNth2Xt1 = PDdissipationNthfdOrder22(&Xt1[index]);
        PDdissipationNth3Xt1 = PDdissipationNthfdOrder23(&Xt1[index]);
        PDdissipationNth1Xt2 = PDdissipationNthfdOrder21(&Xt2[index]);
        PDdissipationNth2Xt2 = PDdissipationNthfdOrder22(&Xt2[index]);
        PDdissipationNth3Xt2 = PDdissipationNthfdOrder23(&Xt2[index]);
        PDdissipationNth1Xt3 = PDdissipationNthfdOrder21(&Xt3[index]);
        PDdissipationNth2Xt3 = PDdissipationNthfdOrder22(&Xt3[index]);
        PDdissipationNth3Xt3 = PDdissipationNthfdOrder23(&Xt3[index]);
        break;
      }
      
      case 4:
      {
        PDdissipationNth1A = PDdissipationNthfdOrder41(&A[index]);
        PDdissipationNth2A = PDdissipationNthfdOrder42(&A[index]);
        PDdissipationNth3A = PDdissipationNthfdOrder43(&A[index]);
        PDdissipationNth1alpha = PDdissipationNthfdOrder41(&alpha[index]);
        PDdissipationNth2alpha = PDdissipationNthfdOrder42(&alpha[index]);
        PDdissipationNth3alpha = PDdissipationNthfdOrder43(&alpha[index]);
        PDdissipationNth1At11 = PDdissipationNthfdOrder41(&At11[index]);
        PDdissipationNth2At11 = PDdissipationNthfdOrder42(&At11[index]);
        PDdissipationNth3At11 = PDdissipationNthfdOrder43(&At11[index]);
        PDdissipationNth1At12 = PDdissipationNthfdOrder41(&At12[index]);
        PDdissipationNth2At12 = PDdissipationNthfdOrder42(&At12[index]);
        PDdissipationNth3At12 = PDdissipationNthfdOrder43(&At12[index]);
        PDdissipationNth1At13 = PDdissipationNthfdOrder41(&At13[index]);
        PDdissipationNth2At13 = PDdissipationNthfdOrder42(&At13[index]);
        PDdissipationNth3At13 = PDdissipationNthfdOrder43(&At13[index]);
        PDdissipationNth1At22 = PDdissipationNthfdOrder41(&At22[index]);
        PDdissipationNth2At22 = PDdissipationNthfdOrder42(&At22[index]);
        PDdissipationNth3At22 = PDdissipationNthfdOrder43(&At22[index]);
        PDdissipationNth1At23 = PDdissipationNthfdOrder41(&At23[index]);
        PDdissipationNth2At23 = PDdissipationNthfdOrder42(&At23[index]);
        PDdissipationNth3At23 = PDdissipationNthfdOrder43(&At23[index]);
        PDdissipationNth1At33 = PDdissipationNthfdOrder41(&At33[index]);
        PDdissipationNth2At33 = PDdissipationNthfdOrder42(&At33[index]);
        PDdissipationNth3At33 = PDdissipationNthfdOrder43(&At33[index]);
        PDdissipationNth1B1 = PDdissipationNthfdOrder41(&B1[index]);
        PDdissipationNth2B1 = PDdissipationNthfdOrder42(&B1[index]);
        PDdissipationNth3B1 = PDdissipationNthfdOrder43(&B1[index]);
        PDdissipationNth1B2 = PDdissipationNthfdOrder41(&B2[index]);
        PDdissipationNth2B2 = PDdissipationNthfdOrder42(&B2[index]);
        PDdissipationNth3B2 = PDdissipationNthfdOrder43(&B2[index]);
        PDdissipationNth1B3 = PDdissipationNthfdOrder41(&B3[index]);
        PDdissipationNth2B3 = PDdissipationNthfdOrder42(&B3[index]);
        PDdissipationNth3B3 = PDdissipationNthfdOrder43(&B3[index]);
        PDdissipationNth1beta1 = PDdissipationNthfdOrder41(&beta1[index]);
        PDdissipationNth2beta1 = PDdissipationNthfdOrder42(&beta1[index]);
        PDdissipationNth3beta1 = PDdissipationNthfdOrder43(&beta1[index]);
        PDdissipationNth1beta2 = PDdissipationNthfdOrder41(&beta2[index]);
        PDdissipationNth2beta2 = PDdissipationNthfdOrder42(&beta2[index]);
        PDdissipationNth3beta2 = PDdissipationNthfdOrder43(&beta2[index]);
        PDdissipationNth1beta3 = PDdissipationNthfdOrder41(&beta3[index]);
        PDdissipationNth2beta3 = PDdissipationNthfdOrder42(&beta3[index]);
        PDdissipationNth3beta3 = PDdissipationNthfdOrder43(&beta3[index]);
        PDdissipationNth1gt11 = PDdissipationNthfdOrder41(&gt11[index]);
        PDdissipationNth2gt11 = PDdissipationNthfdOrder42(&gt11[index]);
        PDdissipationNth3gt11 = PDdissipationNthfdOrder43(&gt11[index]);
        PDdissipationNth1gt12 = PDdissipationNthfdOrder41(&gt12[index]);
        PDdissipationNth2gt12 = PDdissipationNthfdOrder42(&gt12[index]);
        PDdissipationNth3gt12 = PDdissipationNthfdOrder43(&gt12[index]);
        PDdissipationNth1gt13 = PDdissipationNthfdOrder41(&gt13[index]);
        PDdissipationNth2gt13 = PDdissipationNthfdOrder42(&gt13[index]);
        PDdissipationNth3gt13 = PDdissipationNthfdOrder43(&gt13[index]);
        PDdissipationNth1gt22 = PDdissipationNthfdOrder41(&gt22[index]);
        PDdissipationNth2gt22 = PDdissipationNthfdOrder42(&gt22[index]);
        PDdissipationNth3gt22 = PDdissipationNthfdOrder43(&gt22[index]);
        PDdissipationNth1gt23 = PDdissipationNthfdOrder41(&gt23[index]);
        PDdissipationNth2gt23 = PDdissipationNthfdOrder42(&gt23[index]);
        PDdissipationNth3gt23 = PDdissipationNthfdOrder43(&gt23[index]);
        PDdissipationNth1gt33 = PDdissipationNthfdOrder41(&gt33[index]);
        PDdissipationNth2gt33 = PDdissipationNthfdOrder42(&gt33[index]);
        PDdissipationNth3gt33 = PDdissipationNthfdOrder43(&gt33[index]);
        PDdissipationNth1phi = PDdissipationNthfdOrder41(&phi[index]);
        PDdissipationNth2phi = PDdissipationNthfdOrder42(&phi[index]);
        PDdissipationNth3phi = PDdissipationNthfdOrder43(&phi[index]);
        PDdissipationNth1trK = PDdissipationNthfdOrder41(&trK[index]);
        PDdissipationNth2trK = PDdissipationNthfdOrder42(&trK[index]);
        PDdissipationNth3trK = PDdissipationNthfdOrder43(&trK[index]);
        PDdissipationNth1Xt1 = PDdissipationNthfdOrder41(&Xt1[index]);
        PDdissipationNth2Xt1 = PDdissipationNthfdOrder42(&Xt1[index]);
        PDdissipationNth3Xt1 = PDdissipationNthfdOrder43(&Xt1[index]);
        PDdissipationNth1Xt2 = PDdissipationNthfdOrder41(&Xt2[index]);
        PDdissipationNth2Xt2 = PDdissipationNthfdOrder42(&Xt2[index]);
        PDdissipationNth3Xt2 = PDdissipationNthfdOrder43(&Xt2[index]);
        PDdissipationNth1Xt3 = PDdissipationNthfdOrder41(&Xt3[index]);
        PDdissipationNth2Xt3 = PDdissipationNthfdOrder42(&Xt3[index]);
        PDdissipationNth3Xt3 = PDdissipationNthfdOrder43(&Xt3[index]);
        break;
      }
      
      case 6:
      {
        PDdissipationNth1A = PDdissipationNthfdOrder61(&A[index]);
        PDdissipationNth2A = PDdissipationNthfdOrder62(&A[index]);
        PDdissipationNth3A = PDdissipationNthfdOrder63(&A[index]);
        PDdissipationNth1alpha = PDdissipationNthfdOrder61(&alpha[index]);
        PDdissipationNth2alpha = PDdissipationNthfdOrder62(&alpha[index]);
        PDdissipationNth3alpha = PDdissipationNthfdOrder63(&alpha[index]);
        PDdissipationNth1At11 = PDdissipationNthfdOrder61(&At11[index]);
        PDdissipationNth2At11 = PDdissipationNthfdOrder62(&At11[index]);
        PDdissipationNth3At11 = PDdissipationNthfdOrder63(&At11[index]);
        PDdissipationNth1At12 = PDdissipationNthfdOrder61(&At12[index]);
        PDdissipationNth2At12 = PDdissipationNthfdOrder62(&At12[index]);
        PDdissipationNth3At12 = PDdissipationNthfdOrder63(&At12[index]);
        PDdissipationNth1At13 = PDdissipationNthfdOrder61(&At13[index]);
        PDdissipationNth2At13 = PDdissipationNthfdOrder62(&At13[index]);
        PDdissipationNth3At13 = PDdissipationNthfdOrder63(&At13[index]);
        PDdissipationNth1At22 = PDdissipationNthfdOrder61(&At22[index]);
        PDdissipationNth2At22 = PDdissipationNthfdOrder62(&At22[index]);
        PDdissipationNth3At22 = PDdissipationNthfdOrder63(&At22[index]);
        PDdissipationNth1At23 = PDdissipationNthfdOrder61(&At23[index]);
        PDdissipationNth2At23 = PDdissipationNthfdOrder62(&At23[index]);
        PDdissipationNth3At23 = PDdissipationNthfdOrder63(&At23[index]);
        PDdissipationNth1At33 = PDdissipationNthfdOrder61(&At33[index]);
        PDdissipationNth2At33 = PDdissipationNthfdOrder62(&At33[index]);
        PDdissipationNth3At33 = PDdissipationNthfdOrder63(&At33[index]);
        PDdissipationNth1B1 = PDdissipationNthfdOrder61(&B1[index]);
        PDdissipationNth2B1 = PDdissipationNthfdOrder62(&B1[index]);
        PDdissipationNth3B1 = PDdissipationNthfdOrder63(&B1[index]);
        PDdissipationNth1B2 = PDdissipationNthfdOrder61(&B2[index]);
        PDdissipationNth2B2 = PDdissipationNthfdOrder62(&B2[index]);
        PDdissipationNth3B2 = PDdissipationNthfdOrder63(&B2[index]);
        PDdissipationNth1B3 = PDdissipationNthfdOrder61(&B3[index]);
        PDdissipationNth2B3 = PDdissipationNthfdOrder62(&B3[index]);
        PDdissipationNth3B3 = PDdissipationNthfdOrder63(&B3[index]);
        PDdissipationNth1beta1 = PDdissipationNthfdOrder61(&beta1[index]);
        PDdissipationNth2beta1 = PDdissipationNthfdOrder62(&beta1[index]);
        PDdissipationNth3beta1 = PDdissipationNthfdOrder63(&beta1[index]);
        PDdissipationNth1beta2 = PDdissipationNthfdOrder61(&beta2[index]);
        PDdissipationNth2beta2 = PDdissipationNthfdOrder62(&beta2[index]);
        PDdissipationNth3beta2 = PDdissipationNthfdOrder63(&beta2[index]);
        PDdissipationNth1beta3 = PDdissipationNthfdOrder61(&beta3[index]);
        PDdissipationNth2beta3 = PDdissipationNthfdOrder62(&beta3[index]);
        PDdissipationNth3beta3 = PDdissipationNthfdOrder63(&beta3[index]);
        PDdissipationNth1gt11 = PDdissipationNthfdOrder61(&gt11[index]);
        PDdissipationNth2gt11 = PDdissipationNthfdOrder62(&gt11[index]);
        PDdissipationNth3gt11 = PDdissipationNthfdOrder63(&gt11[index]);
        PDdissipationNth1gt12 = PDdissipationNthfdOrder61(&gt12[index]);
        PDdissipationNth2gt12 = PDdissipationNthfdOrder62(&gt12[index]);
        PDdissipationNth3gt12 = PDdissipationNthfdOrder63(&gt12[index]);
        PDdissipationNth1gt13 = PDdissipationNthfdOrder61(&gt13[index]);
        PDdissipationNth2gt13 = PDdissipationNthfdOrder62(&gt13[index]);
        PDdissipationNth3gt13 = PDdissipationNthfdOrder63(&gt13[index]);
        PDdissipationNth1gt22 = PDdissipationNthfdOrder61(&gt22[index]);
        PDdissipationNth2gt22 = PDdissipationNthfdOrder62(&gt22[index]);
        PDdissipationNth3gt22 = PDdissipationNthfdOrder63(&gt22[index]);
        PDdissipationNth1gt23 = PDdissipationNthfdOrder61(&gt23[index]);
        PDdissipationNth2gt23 = PDdissipationNthfdOrder62(&gt23[index]);
        PDdissipationNth3gt23 = PDdissipationNthfdOrder63(&gt23[index]);
        PDdissipationNth1gt33 = PDdissipationNthfdOrder61(&gt33[index]);
        PDdissipationNth2gt33 = PDdissipationNthfdOrder62(&gt33[index]);
        PDdissipationNth3gt33 = PDdissipationNthfdOrder63(&gt33[index]);
        PDdissipationNth1phi = PDdissipationNthfdOrder61(&phi[index]);
        PDdissipationNth2phi = PDdissipationNthfdOrder62(&phi[index]);
        PDdissipationNth3phi = PDdissipationNthfdOrder63(&phi[index]);
        PDdissipationNth1trK = PDdissipationNthfdOrder61(&trK[index]);
        PDdissipationNth2trK = PDdissipationNthfdOrder62(&trK[index]);
        PDdissipationNth3trK = PDdissipationNthfdOrder63(&trK[index]);
        PDdissipationNth1Xt1 = PDdissipationNthfdOrder61(&Xt1[index]);
        PDdissipationNth2Xt1 = PDdissipationNthfdOrder62(&Xt1[index]);
        PDdissipationNth3Xt1 = PDdissipationNthfdOrder63(&Xt1[index]);
        PDdissipationNth1Xt2 = PDdissipationNthfdOrder61(&Xt2[index]);
        PDdissipationNth2Xt2 = PDdissipationNthfdOrder62(&Xt2[index]);
        PDdissipationNth3Xt2 = PDdissipationNthfdOrder63(&Xt2[index]);
        PDdissipationNth1Xt3 = PDdissipationNthfdOrder61(&Xt3[index]);
        PDdissipationNth2Xt3 = PDdissipationNthfdOrder62(&Xt3[index]);
        PDdissipationNth3Xt3 = PDdissipationNthfdOrder63(&Xt3[index]);
        break;
      }
      
      case 8:
      {
        PDdissipationNth1A = PDdissipationNthfdOrder81(&A[index]);
        PDdissipationNth2A = PDdissipationNthfdOrder82(&A[index]);
        PDdissipationNth3A = PDdissipationNthfdOrder83(&A[index]);
        PDdissipationNth1alpha = PDdissipationNthfdOrder81(&alpha[index]);
        PDdissipationNth2alpha = PDdissipationNthfdOrder82(&alpha[index]);
        PDdissipationNth3alpha = PDdissipationNthfdOrder83(&alpha[index]);
        PDdissipationNth1At11 = PDdissipationNthfdOrder81(&At11[index]);
        PDdissipationNth2At11 = PDdissipationNthfdOrder82(&At11[index]);
        PDdissipationNth3At11 = PDdissipationNthfdOrder83(&At11[index]);
        PDdissipationNth1At12 = PDdissipationNthfdOrder81(&At12[index]);
        PDdissipationNth2At12 = PDdissipationNthfdOrder82(&At12[index]);
        PDdissipationNth3At12 = PDdissipationNthfdOrder83(&At12[index]);
        PDdissipationNth1At13 = PDdissipationNthfdOrder81(&At13[index]);
        PDdissipationNth2At13 = PDdissipationNthfdOrder82(&At13[index]);
        PDdissipationNth3At13 = PDdissipationNthfdOrder83(&At13[index]);
        PDdissipationNth1At22 = PDdissipationNthfdOrder81(&At22[index]);
        PDdissipationNth2At22 = PDdissipationNthfdOrder82(&At22[index]);
        PDdissipationNth3At22 = PDdissipationNthfdOrder83(&At22[index]);
        PDdissipationNth1At23 = PDdissipationNthfdOrder81(&At23[index]);
        PDdissipationNth2At23 = PDdissipationNthfdOrder82(&At23[index]);
        PDdissipationNth3At23 = PDdissipationNthfdOrder83(&At23[index]);
        PDdissipationNth1At33 = PDdissipationNthfdOrder81(&At33[index]);
        PDdissipationNth2At33 = PDdissipationNthfdOrder82(&At33[index]);
        PDdissipationNth3At33 = PDdissipationNthfdOrder83(&At33[index]);
        PDdissipationNth1B1 = PDdissipationNthfdOrder81(&B1[index]);
        PDdissipationNth2B1 = PDdissipationNthfdOrder82(&B1[index]);
        PDdissipationNth3B1 = PDdissipationNthfdOrder83(&B1[index]);
        PDdissipationNth1B2 = PDdissipationNthfdOrder81(&B2[index]);
        PDdissipationNth2B2 = PDdissipationNthfdOrder82(&B2[index]);
        PDdissipationNth3B2 = PDdissipationNthfdOrder83(&B2[index]);
        PDdissipationNth1B3 = PDdissipationNthfdOrder81(&B3[index]);
        PDdissipationNth2B3 = PDdissipationNthfdOrder82(&B3[index]);
        PDdissipationNth3B3 = PDdissipationNthfdOrder83(&B3[index]);
        PDdissipationNth1beta1 = PDdissipationNthfdOrder81(&beta1[index]);
        PDdissipationNth2beta1 = PDdissipationNthfdOrder82(&beta1[index]);
        PDdissipationNth3beta1 = PDdissipationNthfdOrder83(&beta1[index]);
        PDdissipationNth1beta2 = PDdissipationNthfdOrder81(&beta2[index]);
        PDdissipationNth2beta2 = PDdissipationNthfdOrder82(&beta2[index]);
        PDdissipationNth3beta2 = PDdissipationNthfdOrder83(&beta2[index]);
        PDdissipationNth1beta3 = PDdissipationNthfdOrder81(&beta3[index]);
        PDdissipationNth2beta3 = PDdissipationNthfdOrder82(&beta3[index]);
        PDdissipationNth3beta3 = PDdissipationNthfdOrder83(&beta3[index]);
        PDdissipationNth1gt11 = PDdissipationNthfdOrder81(&gt11[index]);
        PDdissipationNth2gt11 = PDdissipationNthfdOrder82(&gt11[index]);
        PDdissipationNth3gt11 = PDdissipationNthfdOrder83(&gt11[index]);
        PDdissipationNth1gt12 = PDdissipationNthfdOrder81(&gt12[index]);
        PDdissipationNth2gt12 = PDdissipationNthfdOrder82(&gt12[index]);
        PDdissipationNth3gt12 = PDdissipationNthfdOrder83(&gt12[index]);
        PDdissipationNth1gt13 = PDdissipationNthfdOrder81(&gt13[index]);
        PDdissipationNth2gt13 = PDdissipationNthfdOrder82(&gt13[index]);
        PDdissipationNth3gt13 = PDdissipationNthfdOrder83(&gt13[index]);
        PDdissipationNth1gt22 = PDdissipationNthfdOrder81(&gt22[index]);
        PDdissipationNth2gt22 = PDdissipationNthfdOrder82(&gt22[index]);
        PDdissipationNth3gt22 = PDdissipationNthfdOrder83(&gt22[index]);
        PDdissipationNth1gt23 = PDdissipationNthfdOrder81(&gt23[index]);
        PDdissipationNth2gt23 = PDdissipationNthfdOrder82(&gt23[index]);
        PDdissipationNth3gt23 = PDdissipationNthfdOrder83(&gt23[index]);
        PDdissipationNth1gt33 = PDdissipationNthfdOrder81(&gt33[index]);
        PDdissipationNth2gt33 = PDdissipationNthfdOrder82(&gt33[index]);
        PDdissipationNth3gt33 = PDdissipationNthfdOrder83(&gt33[index]);
        PDdissipationNth1phi = PDdissipationNthfdOrder81(&phi[index]);
        PDdissipationNth2phi = PDdissipationNthfdOrder82(&phi[index]);
        PDdissipationNth3phi = PDdissipationNthfdOrder83(&phi[index]);
        PDdissipationNth1trK = PDdissipationNthfdOrder81(&trK[index]);
        PDdissipationNth2trK = PDdissipationNthfdOrder82(&trK[index]);
        PDdissipationNth3trK = PDdissipationNthfdOrder83(&trK[index]);
        PDdissipationNth1Xt1 = PDdissipationNthfdOrder81(&Xt1[index]);
        PDdissipationNth2Xt1 = PDdissipationNthfdOrder82(&Xt1[index]);
        PDdissipationNth3Xt1 = PDdissipationNthfdOrder83(&Xt1[index]);
        PDdissipationNth1Xt2 = PDdissipationNthfdOrder81(&Xt2[index]);
        PDdissipationNth2Xt2 = PDdissipationNthfdOrder82(&Xt2[index]);
        PDdissipationNth3Xt2 = PDdissipationNthfdOrder83(&Xt2[index]);
        PDdissipationNth1Xt3 = PDdissipationNthfdOrder81(&Xt3[index]);
        PDdissipationNth2Xt3 = PDdissipationNthfdOrder82(&Xt3[index]);
        PDdissipationNth3Xt3 = PDdissipationNthfdOrder83(&Xt3[index]);
        break;
      }
      default:
        CCTK_BUILTIN_UNREACHABLE();
    }
    
    /* Calculate temporaries and grid functions */
    CCTK_REAL epsdiss1 CCTK_ATTRIBUTE_UNUSED = ToReal(EpsDiss);
    
    CCTK_REAL epsdiss2 CCTK_ATTRIBUTE_UNUSED = ToReal(EpsDiss);
    
    CCTK_REAL epsdiss3 CCTK_ATTRIBUTE_UNUSED = ToReal(EpsDiss);
    
    phirhsL = phirhsL + epsdiss1*PDdissipationNth1phi + 
      epsdiss2*PDdissipationNth2phi + epsdiss3*PDdissipationNth3phi;
    
    gt11rhsL = gt11rhsL + epsdiss1*PDdissipationNth1gt11 + 
      epsdiss2*PDdissipationNth2gt11 + epsdiss3*PDdissipationNth3gt11;
    
    gt12rhsL = gt12rhsL + epsdiss1*PDdissipationNth1gt12 + 
      epsdiss2*PDdissipationNth2gt12 + epsdiss3*PDdissipationNth3gt12;
    
    gt13rhsL = gt13rhsL + epsdiss1*PDdissipationNth1gt13 + 
      epsdiss2*PDdissipationNth2gt13 + epsdiss3*PDdissipationNth3gt13;
    
    gt22rhsL = gt22rhsL + epsdiss1*PDdissipationNth1gt22 + 
      epsdiss2*PDdissipationNth2gt22 + epsdiss3*PDdissipationNth3gt22;
    
    gt23rhsL = gt23rhsL + epsdiss1*PDdissipationNth1gt23 + 
      epsdiss2*PDdissipationNth2gt23 + epsdiss3*PDdissipationNth3gt23;
    
    gt33rhsL = gt33rhsL + epsdiss1*PDdissipationNth1gt33 + 
      epsdiss2*PDdissipationNth2gt33 + epsdiss3*PDdissipationNth3gt33;
    
    Xt1rhsL = Xt1rhsL + epsdiss1*PDdissipationNth1Xt1 + 
      epsdiss2*PDdissipationNth2Xt1 + epsdiss3*PDdissipationNth3Xt1;
    
    Xt2rhsL = Xt2rhsL + epsdiss1*PDdissipationNth1Xt2 + 
      epsdiss2*PDdissipationNth2Xt2 + epsdiss3*PDdissipationNth3Xt2;
    
    Xt3rhsL = Xt3rhsL + epsdiss1*PDdissipationNth1Xt3 + 
      epsdiss2*PDdissipationNth2Xt3 + epsdiss3*PDdissipationNth3Xt3;
    
    trKrhsL = trKrhsL + epsdiss1*PDdissipationNth1trK + 
      epsdiss2*PDdissipationNth2trK + epsdiss3*PDdissipationNth3trK;
    
    At11rhsL = At11rhsL + epsdiss1*PDdissipationNth1At11 + 
      epsdiss2*PDdissipationNth2At11 + epsdiss3*PDdissipationNth3At11;
    
    At12rhsL = At12rhsL + epsdiss1*PDdissipationNth1At12 + 
      epsdiss2*PDdissipationNth2At12 + epsdiss3*PDdissipationNth3At12;
    
    At13rhsL = At13rhsL + epsdiss1*PDdissipationNth1At13 + 
      epsdiss2*PDdissipationNth2At13 + epsdiss3*PDdissipationNth3At13;
    
    At22rhsL = At22rhsL + epsdiss1*PDdissipationNth1At22 + 
      epsdiss2*PDdissipationNth2At22 + epsdiss3*PDdissipationNth3At22;
    
    At23rhsL = At23rhsL + epsdiss1*PDdissipationNth1At23 + 
      epsdiss2*PDdissipationNth2At23 + epsdiss3*PDdissipationNth3At23;
    
    At33rhsL = At33rhsL + epsdiss1*PDdissipationNth1At33 + 
      epsdiss2*PDdissipationNth2At33 + epsdiss3*PDdissipationNth3At33;
    
    alpharhsL = alpharhsL + epsdiss1*PDdissipationNth1alpha + 
      epsdiss2*PDdissipationNth2alpha + epsdiss3*PDdissipationNth3alpha;
    
    ArhsL = ArhsL + epsdiss1*PDdissipationNth1A + 
      epsdiss2*PDdissipationNth2A + epsdiss3*PDdissipationNth3A;
    
    beta1rhsL = beta1rhsL + epsdiss1*PDdissipationNth1beta1 + 
      epsdiss2*PDdissipationNth2beta1 + epsdiss3*PDdissipationNth3beta1;
    
    beta2rhsL = beta2rhsL + epsdiss1*PDdissipationNth1beta2 + 
      epsdiss2*PDdissipationNth2beta2 + epsdiss3*PDdissipationNth3beta2;
    
    beta3rhsL = beta3rhsL + epsdiss1*PDdissipationNth1beta3 + 
      epsdiss2*PDdissipationNth2beta3 + epsdiss3*PDdissipationNth3beta3;
    
    B1rhsL = B1rhsL + epsdiss1*PDdissipationNth1B1 + 
      epsdiss2*PDdissipationNth2B1 + epsdiss3*PDdissipationNth3B1;
    
    B2rhsL = B2rhsL + epsdiss1*PDdissipationNth1B2 + 
      epsdiss2*PDdissipationNth2B2 + epsdiss3*PDdissipationNth3B2;
    
    B3rhsL = B3rhsL + epsdiss1*PDdissipationNth1B3 + 
      epsdiss2*PDdissipationNth2B3 + epsdiss3*PDdissipationNth3B3;
    
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
  CCTK_ENDLOOP3(ML_BSSN_Dissipation);
}

extern "C" void ML_BSSN_Dissipation(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Entering ML_BSSN_Dissipation_Body");
  }
  
  if (cctk_iteration % ML_BSSN_Dissipation_calc_every != ML_BSSN_Dissipation_calc_offset)
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
  GenericFD_AssertGroupStorage(cctkGH, "ML_BSSN_Dissipation", 18, groups);
  
  switch (fdOrder)
  {
    case 2:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Dissipation", 2, 2, 2);
      break;
    }
    
    case 4:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Dissipation", 3, 3, 3);
      break;
    }
    
    case 6:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Dissipation", 4, 4, 4);
      break;
    }
    
    case 8:
    {
      GenericFD_EnsureStencilFits(cctkGH, "ML_BSSN_Dissipation", 5, 5, 5);
      break;
    }
    default:
      CCTK_BUILTIN_UNREACHABLE();
  }
  
  GenericFD_LoopOverInterior(cctkGH, ML_BSSN_Dissipation_Body);
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Leaving ML_BSSN_Dissipation_Body");
  }
}
