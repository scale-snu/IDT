/*  File produced by Kranc */

#define KRANC_C

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef SPEC 
#include "GaugeWave/cctk.h" 
#define THORN_DIR "GaugeWave" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "GaugeWave/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "GaugeWave/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "GenericFD.h"
#include "EinsteinExact/GaugeWave/src/Differencing.h"
#include "cctk_Loop.h"
#include "loopcontrol.h"

/* Define macros used in calculations */
#define INITVALUE (42)
#define INV(x) ((CCTK_REAL)1.0 / (x))
#define SQR(x) ((x) * (x))
#define CUB(x) ((x) * SQR(x))
#define QAD(x) (SQR(SQR(x)))

static void GaugeWave_initial_Body(const cGH* restrict const cctkGH, const int dir, const int face, const CCTK_REAL normal[3], const CCTK_REAL tangentA[3], const CCTK_REAL tangentB[3], const int imin[3], const int imax[3], const int n_subblock_gfs, CCTK_REAL* restrict const subblock_gfs[])
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
  const CCTK_REAL kthird CCTK_ATTRIBUTE_UNUSED = 1/3.0;
  const CCTK_REAL ktwothird CCTK_ATTRIBUTE_UNUSED = 2.0/3.0;
  const CCTK_REAL kfourthird CCTK_ATTRIBUTE_UNUSED = 4.0/3.0;
  const CCTK_REAL keightthird CCTK_ATTRIBUTE_UNUSED = 8.0/3.0;
  const CCTK_REAL hdxi CCTK_ATTRIBUTE_UNUSED = 0.5 * dxi;
  const CCTK_REAL hdyi CCTK_ATTRIBUTE_UNUSED = 0.5 * dyi;
  const CCTK_REAL hdzi CCTK_ATTRIBUTE_UNUSED = 0.5 * dzi;
  
  /* Initialize predefined quantities */
  
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
  CCTK_LOOP3(GaugeWave_initial,
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
    CCTK_REAL dtbetaxL CCTK_ATTRIBUTE_UNUSED = dtbetax[index];
    CCTK_REAL dtbetayL CCTK_ATTRIBUTE_UNUSED = dtbetay[index];
    CCTK_REAL dtbetazL CCTK_ATTRIBUTE_UNUSED = dtbetaz[index];
    CCTK_REAL gxxL CCTK_ATTRIBUTE_UNUSED = gxx[index];
    CCTK_REAL gxyL CCTK_ATTRIBUTE_UNUSED = gxy[index];
    CCTK_REAL gxzL CCTK_ATTRIBUTE_UNUSED = gxz[index];
    CCTK_REAL gyyL CCTK_ATTRIBUTE_UNUSED = gyy[index];
    CCTK_REAL gyzL CCTK_ATTRIBUTE_UNUSED = gyz[index];
    CCTK_REAL gzzL CCTK_ATTRIBUTE_UNUSED = gzz[index];
    CCTK_REAL xL CCTK_ATTRIBUTE_UNUSED = x[index];
    CCTK_REAL yL CCTK_ATTRIBUTE_UNUSED = y[index];
    CCTK_REAL zL CCTK_ATTRIBUTE_UNUSED = z[index];
    
    
    /* Include user supplied include files */
    
    /* Precompute derivatives */
    
    /* Calculate temporaries and grid functions */
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L00 = 1;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L01 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L02 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L03 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L10 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp0 = cos(ToReal(phi));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp1 = cos(ToReal(psi));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp2 = cos(ToReal(theta));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp3 = sin(ToReal(phi));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp4 = sin(ToReal(psi));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L11 = csetemp0*csetemp1 - 
      csetemp2*csetemp3*csetemp4;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L12 = csetemp1*csetemp3 + 
      csetemp0*csetemp2*csetemp4;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp5 = sin(ToReal(theta));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L13 = csetemp4*csetemp5;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L20 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L21 = 
      -(csetemp1*csetemp2*csetemp3) - csetemp0*csetemp4;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L22 = csetemp0*csetemp1*csetemp2 
      - csetemp3*csetemp4;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L23 = csetemp1*csetemp5;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L30 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L31 = csetemp3*csetemp5;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L32 = -(csetemp0*csetemp5);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform1L33 = csetemp2;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp6 = SQR(ToReal(boostx));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp7 = SQR(ToReal(boosty));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp8 = SQR(ToReal(boostz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp9 = INV(ToReal(lapsefactor));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L00 = csetemp9*INV((-1 + 
      csetemp6 + csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(1 - csetemp6 - csetemp7 - csetemp8 + sqrt(1 - csetemp6 - 
      csetemp7 - csetemp8))*(-1 + ToReal(boostx)*ToReal(shiftaddx) + 
      ToReal(boosty)*ToReal(shiftaddy) + ToReal(boostz)*ToReal(shiftaddz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L01 = csetemp9*INV((-1 + 
      csetemp6 + csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(1 - csetemp6 - csetemp7 - csetemp8 + sqrt(1 - csetemp6 - 
      csetemp7 - csetemp8))*ToReal(boostx);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L02 = csetemp9*INV((-1 + 
      csetemp6 + csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(1 - csetemp6 - csetemp7 - csetemp8 + sqrt(1 - csetemp6 - 
      csetemp7 - csetemp8))*ToReal(boosty);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L03 = csetemp9*INV((-1 + 
      csetemp6 + csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(1 - csetemp6 - csetemp7 - csetemp8 + sqrt(1 - csetemp6 - 
      csetemp7 - csetemp8))*ToReal(boostz);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L10 = INV((-1 + csetemp6 + 
      csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*((csetemp6 + (-1 + csetemp7 + csetemp8)*(1 + sqrt(1 - 
      csetemp6 - csetemp7 - csetemp8)))*ToReal(shiftaddx) - 
      ToReal(boostx)*(-1 + csetemp6 + csetemp7 + csetemp8 + sqrt(1 - csetemp6 
      - csetemp7 - csetemp8)*(-1 + ToReal(boosty)*ToReal(shiftaddy) + 
      ToReal(boostz)*ToReal(shiftaddz))));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L11 = INV((-1 + csetemp6 + 
      csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(csetemp6 + (-1 + csetemp7 + csetemp8)*(1 + sqrt(1 - 
      csetemp6 - csetemp7 - csetemp8)));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L12 = -(INV(-1 + csetemp6 + 
      csetemp7 + csetemp8 - sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8))*ToReal(boostx)*ToReal(boosty));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L13 = -(INV(-1 + csetemp6 + 
      csetemp7 + csetemp8 - sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8))*ToReal(boostx)*ToReal(boostz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L20 = INV((-1 + csetemp6 + 
      csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*((csetemp7 + (-1 + csetemp6 + csetemp8)*(1 + sqrt(1 - 
      csetemp6 - csetemp7 - csetemp8)))*ToReal(shiftaddy) - 
      ToReal(boosty)*(-1 + csetemp6 + csetemp7 + csetemp8 + sqrt(1 - csetemp6 
      - csetemp7 - csetemp8)*(-1 + ToReal(boostx)*ToReal(shiftaddx) + 
      ToReal(boostz)*ToReal(shiftaddz))));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L21 = -(INV(-1 + csetemp6 + 
      csetemp7 + csetemp8 - sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8))*ToReal(boostx)*ToReal(boosty));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L22 = INV((-1 + csetemp6 + 
      csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(csetemp7 + (-1 + csetemp6 + csetemp8)*(1 + sqrt(1 - 
      csetemp6 - csetemp7 - csetemp8)));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L23 = -(INV(-1 + csetemp6 + 
      csetemp7 + csetemp8 - sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8))*ToReal(boosty)*ToReal(boostz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L30 = INV((-1 + csetemp6 + 
      csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(-(ToReal(boostz)*(-1 + csetemp6 + csetemp7 + csetemp8 + 
      sqrt(1 - csetemp6 - csetemp7 - csetemp8)*(-1 + 
      ToReal(boostx)*ToReal(shiftaddx) + ToReal(boosty)*ToReal(shiftaddy)))) 
      + (-1 + csetemp6 + csetemp7 + csetemp8 + (-1 + csetemp6 + 
      csetemp7)*sqrt(1 - csetemp6 - csetemp7 - csetemp8))*ToReal(shiftaddz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L31 = -(INV(-1 + csetemp6 + 
      csetemp7 + csetemp8 - sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8))*ToReal(boostx)*ToReal(boostz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L32 = -(INV(-1 + csetemp6 + 
      csetemp7 + csetemp8 - sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8))*ToReal(boosty)*ToReal(boostz));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xform2L33 = INV((-1 + csetemp6 + 
      csetemp7 + csetemp8)*(1 + sqrt(1 - csetemp6 - csetemp7 - 
      csetemp8)))*(-1 + csetemp6 + csetemp7 + csetemp8 + (-1 + csetemp6 + 
      csetemp7)*sqrt(1 - csetemp6 - csetemp7 - csetemp8));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL00 = xform1L00*xform2L00 + 
      xform1L01*xform2L10 + xform1L02*xform2L20 + xform1L03*xform2L30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL01 = xform1L00*xform2L01 + 
      xform1L01*xform2L11 + xform1L02*xform2L21 + xform1L03*xform2L31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL02 = xform1L00*xform2L02 + 
      xform1L01*xform2L12 + xform1L02*xform2L22 + xform1L03*xform2L32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL03 = xform1L00*xform2L03 + 
      xform1L01*xform2L13 + xform1L02*xform2L23 + xform1L03*xform2L33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL10 = xform1L10*xform2L00 + 
      xform1L11*xform2L10 + xform1L12*xform2L20 + xform1L13*xform2L30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL11 = xform1L10*xform2L01 + 
      xform1L11*xform2L11 + xform1L12*xform2L21 + xform1L13*xform2L31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL12 = xform1L10*xform2L02 + 
      xform1L11*xform2L12 + xform1L12*xform2L22 + xform1L13*xform2L32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL13 = xform1L10*xform2L03 + 
      xform1L11*xform2L13 + xform1L12*xform2L23 + xform1L13*xform2L33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL20 = xform1L20*xform2L00 + 
      xform1L21*xform2L10 + xform1L22*xform2L20 + xform1L23*xform2L30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL21 = xform1L20*xform2L01 + 
      xform1L21*xform2L11 + xform1L22*xform2L21 + xform1L23*xform2L31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL22 = xform1L20*xform2L02 + 
      xform1L21*xform2L12 + xform1L22*xform2L22 + xform1L23*xform2L32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL23 = xform1L20*xform2L03 + 
      xform1L21*xform2L13 + xform1L22*xform2L23 + xform1L23*xform2L33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL30 = xform1L30*xform2L00 + 
      xform1L31*xform2L10 + xform1L32*xform2L20 + xform1L33*xform2L30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL31 = xform1L30*xform2L01 + 
      xform1L31*xform2L11 + xform1L32*xform2L21 + xform1L33*xform2L31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL32 = xform1L30*xform2L02 + 
      xform1L31*xform2L12 + xform1L32*xform2L22 + xform1L33*xform2L32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xformL33 = xform1L30*xform2L03 + 
      xform1L31*xform2L13 + xform1L32*xform2L23 + xform1L33*xform2L33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xx0 = t - ToReal(timeoffset);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xx1 = xL - ToReal(positionx);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xx2 = yL - ToReal(positiony);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED xx3 = zL - ToReal(positionz);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED txx0 = xformL00*xx0 + xformL01*xx1 + 
      xformL02*xx2 + xformL03*xx3;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED txx1 = xformL10*xx0 + xformL11*xx1 + 
      xformL12*xx2 + xformL13*xx3;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED T = txx0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED X = txx1;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp10 = -T + X;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp11 = INV(ToReal(period));
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg400 = -1 + 
      sin(2*csetemp10*csetemp11*Pi)*ToReal(amp);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg401 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg402 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg403 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg411 = 1 - 
      sin(2*csetemp10*csetemp11*Pi)*ToReal(amp);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg412 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg413 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg422 = 1;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg423 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tg433 = 1;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4000 = 
      -2*csetemp11*Pi*cos(2*csetemp10*csetemp11*Pi)*ToReal(amp);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4001 = 
      2*csetemp11*Pi*cos(2*csetemp10*csetemp11*Pi)*ToReal(amp);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4002 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4003 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4010 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4011 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4012 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4013 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4020 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4021 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4022 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4023 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4030 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4031 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4032 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4033 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4110 = 
      2*csetemp11*Pi*cos(2*csetemp10*csetemp11*Pi)*ToReal(amp);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4111 = 
      -2*csetemp11*Pi*cos(2*csetemp10*csetemp11*Pi)*ToReal(amp);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4112 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4113 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4120 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4121 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4122 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4123 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4130 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4131 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4132 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4133 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4220 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4221 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4222 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4223 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4230 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4231 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4232 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4233 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4330 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4331 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4332 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED tdg4333 = 0;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g400 = 2*(tg423*xformL20*xformL30 + 
      xformL00*(tg401*xformL10 + tg402*xformL20 + tg403*xformL30) + 
      xformL10*(tg412*xformL20 + tg413*xformL30)) + tg400*SQR(xformL00) + 
      tg411*SQR(xformL10) + tg422*SQR(xformL20) + tg433*SQR(xformL30);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp12 = tg400*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp13 = tg401*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp14 = tg402*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp15 = tg403*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp16 = tg401*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp17 = tg411*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp18 = tg412*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp19 = tg413*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp20 = tg402*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp21 = tg412*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp22 = tg422*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp23 = tg423*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp24 = tg403*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp25 = tg413*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp26 = tg423*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp27 = tg433*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g401 = (csetemp12 + csetemp13 + 
      csetemp14 + csetemp15)*xformL00 + (csetemp16 + csetemp17 + csetemp18 + 
      csetemp19)*xformL10 + (csetemp20 + csetemp21 + csetemp22 + 
      csetemp23)*xformL20 + (csetemp24 + csetemp25 + csetemp26 + 
      csetemp27)*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp28 = tg400*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp29 = tg401*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp30 = tg402*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp31 = tg403*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp32 = tg401*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp33 = tg411*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp34 = tg412*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp35 = tg413*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp36 = tg402*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp37 = tg412*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp38 = tg422*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp39 = tg423*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp40 = tg403*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp41 = tg413*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp42 = tg423*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp43 = tg433*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g402 = (csetemp28 + csetemp29 + 
      csetemp30 + csetemp31)*xformL00 + (csetemp32 + csetemp33 + csetemp34 + 
      csetemp35)*xformL10 + (csetemp36 + csetemp37 + csetemp38 + 
      csetemp39)*xformL20 + (csetemp40 + csetemp41 + csetemp42 + 
      csetemp43)*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp44 = tg400*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp45 = tg401*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp46 = tg402*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp47 = tg403*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp48 = tg401*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp49 = tg411*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp50 = tg412*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp51 = tg413*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp52 = tg402*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp53 = tg412*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp54 = tg422*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp55 = tg423*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp56 = tg403*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp57 = tg413*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp58 = tg423*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp59 = tg433*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g403 = (csetemp44 + csetemp45 + 
      csetemp46 + csetemp47)*xformL00 + (csetemp48 + csetemp49 + csetemp50 + 
      csetemp51)*xformL10 + (csetemp52 + csetemp53 + csetemp54 + 
      csetemp55)*xformL20 + (csetemp56 + csetemp57 + csetemp58 + 
      csetemp59)*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g411 = (csetemp12 + csetemp13 + 
      csetemp14 + csetemp15)*xformL01 + (csetemp16 + csetemp17 + csetemp18 + 
      csetemp19)*xformL11 + (csetemp20 + csetemp21 + csetemp22 + 
      csetemp23)*xformL21 + (csetemp24 + csetemp25 + csetemp26 + 
      csetemp27)*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g412 = (csetemp28 + csetemp29 + 
      csetemp30 + csetemp31)*xformL01 + (csetemp32 + csetemp33 + csetemp34 + 
      csetemp35)*xformL11 + (csetemp36 + csetemp37 + csetemp38 + 
      csetemp39)*xformL21 + (csetemp40 + csetemp41 + csetemp42 + 
      csetemp43)*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g413 = (csetemp44 + csetemp45 + 
      csetemp46 + csetemp47)*xformL01 + (csetemp48 + csetemp49 + csetemp50 + 
      csetemp51)*xformL11 + (csetemp52 + csetemp53 + csetemp54 + 
      csetemp55)*xformL21 + (csetemp56 + csetemp57 + csetemp58 + 
      csetemp59)*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g422 = (csetemp28 + csetemp29 + 
      csetemp30 + csetemp31)*xformL02 + (csetemp32 + csetemp33 + csetemp34 + 
      csetemp35)*xformL12 + (csetemp36 + csetemp37 + csetemp38 + 
      csetemp39)*xformL22 + (csetemp40 + csetemp41 + csetemp42 + 
      csetemp43)*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g423 = (csetemp44 + csetemp45 + 
      csetemp46 + csetemp47)*xformL02 + (csetemp48 + csetemp49 + csetemp50 + 
      csetemp51)*xformL12 + (csetemp52 + csetemp53 + csetemp54 + 
      csetemp55)*xformL22 + (csetemp56 + csetemp57 + csetemp58 + 
      csetemp59)*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED g433 = (csetemp44 + csetemp45 + 
      csetemp46 + csetemp47)*xformL03 + (csetemp48 + csetemp49 + csetemp50 + 
      csetemp51)*xformL13 + (csetemp52 + csetemp53 + csetemp54 + 
      csetemp55)*xformL23 + (csetemp56 + csetemp57 + csetemp58 + 
      csetemp59)*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp60 = tdg4000*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp61 = tdg4001*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp62 = tdg4002*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp63 = tdg4003*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp64 = tdg4010*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp65 = tdg4011*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp66 = tdg4012*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp67 = tdg4013*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp68 = tdg4020*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp69 = tdg4021*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp70 = tdg4022*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp71 = tdg4023*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp72 = tdg4030*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp73 = tdg4031*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp74 = tdg4032*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp75 = tdg4033*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp76 = tdg4110*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp77 = tdg4111*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp78 = tdg4112*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp79 = tdg4113*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp80 = tdg4120*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp81 = tdg4121*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp82 = tdg4122*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp83 = tdg4123*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp84 = tdg4130*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp85 = tdg4131*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp86 = tdg4132*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp87 = tdg4133*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp88 = tdg4220*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp89 = tdg4221*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp90 = tdg4222*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp91 = tdg4223*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp92 = tdg4230*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp93 = tdg4231*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp94 = tdg4232*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp95 = tdg4233*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp96 = tdg4330*xformL00;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp97 = tdg4331*xformL10;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp98 = tdg4332*xformL20;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp99 = tdg4333*xformL30;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4000 = xformL00*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL00 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL10 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL20 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL30) + xformL10*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL00 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL10 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL20 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL30) + xformL20*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL00 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL10 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL20 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL30) + xformL30*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL00 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL10 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL20 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL30);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4010 = xformL00*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL01 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL11 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL21 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL31) + xformL10*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL01 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL11 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL21 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL31) + xformL20*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL01 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL11 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL21 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL31) + xformL30*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL01 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL11 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL21 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp100 = tdg4000*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp101 = tdg4001*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp102 = tdg4002*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp103 = tdg4003*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp104 = tdg4010*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp105 = tdg4011*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp106 = tdg4012*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp107 = tdg4013*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp108 = tdg4020*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp109 = tdg4021*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp110 = tdg4022*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp111 = tdg4023*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp112 = tdg4030*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp113 = tdg4031*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp114 = tdg4032*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp115 = tdg4033*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp116 = tdg4110*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp117 = tdg4111*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp118 = tdg4112*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp119 = tdg4113*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp120 = tdg4120*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp121 = tdg4121*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp122 = tdg4122*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp123 = tdg4123*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp124 = tdg4130*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp125 = tdg4131*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp126 = tdg4132*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp127 = tdg4133*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp128 = tdg4220*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp129 = tdg4221*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp130 = tdg4222*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp131 = tdg4223*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp132 = tdg4230*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp133 = tdg4231*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp134 = tdg4232*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp135 = tdg4233*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp136 = tdg4330*xformL01;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp137 = tdg4331*xformL11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp138 = tdg4332*xformL21;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp139 = tdg4333*xformL31;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4011 = xformL00*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL01 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL11 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL21 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL31) + xformL10*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL01 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL11 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL21 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL31) + xformL20*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL01 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL11 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL21 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL31) + xformL30*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL01 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL11 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL21 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp140 = tdg4000*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp141 = tdg4001*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp142 = tdg4002*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp143 = tdg4003*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp144 = tdg4010*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp145 = tdg4011*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp146 = tdg4012*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp147 = tdg4013*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp148 = tdg4020*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp149 = tdg4021*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp150 = tdg4022*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp151 = tdg4023*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp152 = tdg4030*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp153 = tdg4031*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp154 = tdg4032*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp155 = tdg4033*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp156 = tdg4110*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp157 = tdg4111*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp158 = tdg4112*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp159 = tdg4113*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp160 = tdg4120*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp161 = tdg4121*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp162 = tdg4122*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp163 = tdg4123*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp164 = tdg4130*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp165 = tdg4131*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp166 = tdg4132*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp167 = tdg4133*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp168 = tdg4220*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp169 = tdg4221*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp170 = tdg4222*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp171 = tdg4223*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp172 = tdg4230*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp173 = tdg4231*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp174 = tdg4232*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp175 = tdg4233*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp176 = tdg4330*xformL02;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp177 = tdg4331*xformL12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp178 = tdg4332*xformL22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp179 = tdg4333*xformL32;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4012 = xformL00*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL01 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL11 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL21 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL31) + xformL10*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL01 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL11 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL21 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL31) + xformL20*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL01 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL11 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL21 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL31) + xformL30*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL01 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL11 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL21 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp180 = tdg4000*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp181 = tdg4001*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp182 = tdg4002*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp183 = tdg4003*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp184 = tdg4010*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp185 = tdg4011*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp186 = tdg4012*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp187 = tdg4013*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp188 = tdg4020*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp189 = tdg4021*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp190 = tdg4022*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp191 = tdg4023*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp192 = tdg4030*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp193 = tdg4031*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp194 = tdg4032*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp195 = tdg4033*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp196 = tdg4110*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp197 = tdg4111*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp198 = tdg4112*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp199 = tdg4113*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp200 = tdg4120*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp201 = tdg4121*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp202 = tdg4122*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp203 = tdg4123*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp204 = tdg4130*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp205 = tdg4131*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp206 = tdg4132*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp207 = tdg4133*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp208 = tdg4220*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp209 = tdg4221*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp210 = tdg4222*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp211 = tdg4223*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp212 = tdg4230*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp213 = tdg4231*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp214 = tdg4232*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp215 = tdg4233*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp216 = tdg4330*xformL03;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp217 = tdg4331*xformL13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp218 = tdg4332*xformL23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp219 = tdg4333*xformL33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4013 = xformL00*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL01 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL11 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL21 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL31) + xformL10*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL01 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL11 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL21 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL31) + xformL20*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL01 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL11 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL21 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL31) + xformL30*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL01 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL11 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL21 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4020 = xformL00*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL02 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL12 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL22 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL32) + xformL10*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL02 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL12 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL22 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL32) + xformL20*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL02 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL12 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL22 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL32) + xformL30*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL02 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL12 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL22 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4021 = xformL00*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL02 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL12 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL22 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL32) + xformL10*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL02 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL12 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL22 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL32) + xformL20*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL02 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL12 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL22 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL32) + xformL30*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL02 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL12 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL22 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4022 = xformL00*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL02 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL12 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL22 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL32) + xformL10*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL02 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL12 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL22 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL32) + xformL20*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL02 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL12 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL22 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL32) + xformL30*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL02 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL12 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL22 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4023 = xformL00*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL02 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL12 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL22 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL32) + xformL10*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL02 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL12 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL22 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL32) + xformL20*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL02 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL12 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL22 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL32) + xformL30*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL02 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL12 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL22 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4030 = xformL00*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL03 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL13 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL23 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL33) + xformL10*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL03 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL13 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL23 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL33) + xformL20*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL03 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL13 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL23 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL33) + xformL30*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL03 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL13 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL23 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4031 = xformL00*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL03 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL13 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL23 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL33) + xformL10*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL03 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL13 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL23 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL33) + xformL20*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL03 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL13 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL23 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL33) + xformL30*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL03 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL13 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL23 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4032 = xformL00*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL03 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL13 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL23 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL33) + xformL10*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL03 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL13 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL23 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL33) + xformL20*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL03 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL13 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL23 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL33) + xformL30*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL03 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL13 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL23 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4033 = xformL00*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL03 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL13 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL23 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL33) + xformL10*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL03 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL13 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL23 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL33) + xformL20*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL03 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL13 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL23 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL33) + xformL30*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL03 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL13 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL23 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4110 = xformL01*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL01 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL11 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL21 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL31) + xformL11*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL01 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL11 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL21 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL31) + xformL21*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL01 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL11 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL21 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL31) + xformL31*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL01 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL11 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL21 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4111 = xformL01*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL01 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL11 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL21 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL31) + xformL11*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL01 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL11 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL21 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL31) + xformL21*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL01 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL11 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL21 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL31) + xformL31*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL01 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL11 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL21 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4112 = xformL01*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL01 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL11 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL21 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL31) + xformL11*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL01 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL11 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL21 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL31) + xformL21*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL01 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL11 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL21 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL31) + xformL31*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL01 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL11 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL21 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4113 = xformL01*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL01 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL11 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL21 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL31) + xformL11*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL01 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL11 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL21 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL31) + xformL21*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL01 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL11 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL21 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL31) + xformL31*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL01 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL11 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL21 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL31);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4120 = xformL01*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL02 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL12 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL22 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL32) + xformL11*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL02 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL12 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL22 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL32) + xformL21*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL02 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL12 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL22 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL32) + xformL31*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL02 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL12 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL22 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4121 = xformL01*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL02 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL12 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL22 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL32) + xformL11*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL02 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL12 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL22 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL32) + xformL21*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL02 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL12 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL22 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL32) + xformL31*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL02 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL12 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL22 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4122 = xformL01*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL02 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL12 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL22 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL32) + xformL11*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL02 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL12 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL22 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL32) + xformL21*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL02 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL12 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL22 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL32) + xformL31*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL02 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL12 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL22 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4123 = xformL01*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL02 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL12 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL22 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL32) + xformL11*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL02 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL12 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL22 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL32) + xformL21*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL02 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL12 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL22 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL32) + xformL31*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL02 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL12 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL22 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4130 = xformL01*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL03 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL13 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL23 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL33) + xformL11*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL03 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL13 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL23 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL33) + xformL21*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL03 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL13 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL23 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL33) + xformL31*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL03 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL13 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL23 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4131 = xformL01*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL03 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL13 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL23 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL33) + xformL11*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL03 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL13 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL23 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL33) + xformL21*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL03 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL13 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL23 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL33) + xformL31*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL03 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL13 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL23 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4132 = xformL01*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL03 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL13 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL23 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL33) + xformL11*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL03 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL13 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL23 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL33) + xformL21*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL03 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL13 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL23 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL33) + xformL31*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL03 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL13 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL23 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4133 = xformL01*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL03 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL13 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL23 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL33) + xformL11*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL03 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL13 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL23 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL33) + xformL21*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL03 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL13 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL23 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL33) + xformL31*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL03 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL13 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL23 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4220 = xformL02*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL02 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL12 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL22 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL32) + xformL12*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL02 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL12 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL22 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL32) + xformL22*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL02 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL12 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL22 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL32) + xformL32*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL02 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL12 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL22 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4221 = xformL02*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL02 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL12 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL22 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL32) + xformL12*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL02 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL12 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL22 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL32) + xformL22*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL02 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL12 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL22 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL32) + xformL32*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL02 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL12 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL22 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4222 = xformL02*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL02 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL12 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL22 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL32) + xformL12*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL02 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL12 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL22 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL32) + xformL22*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL02 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL12 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL22 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL32) + xformL32*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL02 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL12 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL22 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4223 = xformL02*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL02 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL12 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL22 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL32) + xformL12*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL02 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL12 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL22 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL32) + xformL22*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL02 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL12 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL22 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL32) + xformL32*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL02 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL12 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL22 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL32);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4230 = xformL02*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL03 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL13 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL23 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL33) + xformL12*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL03 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL13 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL23 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL33) + xformL22*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL03 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL13 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL23 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL33) + xformL32*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL03 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL13 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL23 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4231 = xformL02*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL03 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL13 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL23 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL33) + xformL12*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL03 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL13 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL23 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL33) + xformL22*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL03 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL13 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL23 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL33) + xformL32*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL03 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL13 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL23 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4232 = xformL02*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL03 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL13 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL23 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL33) + xformL12*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL03 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL13 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL23 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL33) + xformL22*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL03 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL13 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL23 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL33) + xformL32*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL03 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL13 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL23 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4233 = xformL02*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL03 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL13 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL23 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL33) + xformL12*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL03 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL13 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL23 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL33) + xformL22*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL03 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL13 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL23 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL33) + xformL32*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL03 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL13 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL23 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4330 = xformL03*((csetemp60 + 
      csetemp61 + csetemp62 + csetemp63)*xformL03 + (csetemp64 + csetemp65 + 
      csetemp66 + csetemp67)*xformL13 + (csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL23 + (csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL33) + xformL13*((csetemp64 + csetemp65 + csetemp66 + 
      csetemp67)*xformL03 + (csetemp76 + csetemp77 + csetemp78 + 
      csetemp79)*xformL13 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL23 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL33) + xformL23*((csetemp68 + csetemp69 + csetemp70 + 
      csetemp71)*xformL03 + (csetemp80 + csetemp81 + csetemp82 + 
      csetemp83)*xformL13 + (csetemp88 + csetemp89 + csetemp90 + 
      csetemp91)*xformL23 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL33) + xformL33*((csetemp72 + csetemp73 + csetemp74 + 
      csetemp75)*xformL03 + (csetemp84 + csetemp85 + csetemp86 + 
      csetemp87)*xformL13 + (csetemp92 + csetemp93 + csetemp94 + 
      csetemp95)*xformL23 + (csetemp96 + csetemp97 + csetemp98 + 
      csetemp99)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4331 = xformL03*((csetemp100 + 
      csetemp101 + csetemp102 + csetemp103)*xformL03 + (csetemp104 + 
      csetemp105 + csetemp106 + csetemp107)*xformL13 + (csetemp108 + 
      csetemp109 + csetemp110 + csetemp111)*xformL23 + (csetemp112 + 
      csetemp113 + csetemp114 + csetemp115)*xformL33) + xformL13*((csetemp104 
      + csetemp105 + csetemp106 + csetemp107)*xformL03 + (csetemp116 + 
      csetemp117 + csetemp118 + csetemp119)*xformL13 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL23 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL33) + xformL23*((csetemp108 
      + csetemp109 + csetemp110 + csetemp111)*xformL03 + (csetemp120 + 
      csetemp121 + csetemp122 + csetemp123)*xformL13 + (csetemp128 + 
      csetemp129 + csetemp130 + csetemp131)*xformL23 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL33) + xformL33*((csetemp112 
      + csetemp113 + csetemp114 + csetemp115)*xformL03 + (csetemp124 + 
      csetemp125 + csetemp126 + csetemp127)*xformL13 + (csetemp132 + 
      csetemp133 + csetemp134 + csetemp135)*xformL23 + (csetemp136 + 
      csetemp137 + csetemp138 + csetemp139)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4332 = xformL03*((csetemp140 + 
      csetemp141 + csetemp142 + csetemp143)*xformL03 + (csetemp144 + 
      csetemp145 + csetemp146 + csetemp147)*xformL13 + (csetemp148 + 
      csetemp149 + csetemp150 + csetemp151)*xformL23 + (csetemp152 + 
      csetemp153 + csetemp154 + csetemp155)*xformL33) + xformL13*((csetemp144 
      + csetemp145 + csetemp146 + csetemp147)*xformL03 + (csetemp156 + 
      csetemp157 + csetemp158 + csetemp159)*xformL13 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL23 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL33) + xformL23*((csetemp148 
      + csetemp149 + csetemp150 + csetemp151)*xformL03 + (csetemp160 + 
      csetemp161 + csetemp162 + csetemp163)*xformL13 + (csetemp168 + 
      csetemp169 + csetemp170 + csetemp171)*xformL23 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL33) + xformL33*((csetemp152 
      + csetemp153 + csetemp154 + csetemp155)*xformL03 + (csetemp164 + 
      csetemp165 + csetemp166 + csetemp167)*xformL13 + (csetemp172 + 
      csetemp173 + csetemp174 + csetemp175)*xformL23 + (csetemp176 + 
      csetemp177 + csetemp178 + csetemp179)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg4333 = xformL03*((csetemp180 + 
      csetemp181 + csetemp182 + csetemp183)*xformL03 + (csetemp184 + 
      csetemp185 + csetemp186 + csetemp187)*xformL13 + (csetemp188 + 
      csetemp189 + csetemp190 + csetemp191)*xformL23 + (csetemp192 + 
      csetemp193 + csetemp194 + csetemp195)*xformL33) + xformL13*((csetemp184 
      + csetemp185 + csetemp186 + csetemp187)*xformL03 + (csetemp196 + 
      csetemp197 + csetemp198 + csetemp199)*xformL13 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL23 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL33) + xformL23*((csetemp188 
      + csetemp189 + csetemp190 + csetemp191)*xformL03 + (csetemp200 + 
      csetemp201 + csetemp202 + csetemp203)*xformL13 + (csetemp208 + 
      csetemp209 + csetemp210 + csetemp211)*xformL23 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL33) + xformL33*((csetemp192 
      + csetemp193 + csetemp194 + csetemp195)*xformL03 + (csetemp204 + 
      csetemp205 + csetemp206 + csetemp207)*xformL13 + (csetemp212 + 
      csetemp213 + csetemp214 + csetemp215)*xformL23 + (csetemp216 + 
      csetemp217 + csetemp218 + csetemp219)*xformL33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED betal1 = g401;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED betal2 = g402;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED betal3 = g403;
    
    gxxL = g411;
    
    gxyL = g412;
    
    gxzL = g413;
    
    gyyL = g422;
    
    gyzL = g423;
    
    gzzL = g433;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp220 = SQR(gxzL);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp221 = SQR(gyzL);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp222 = SQR(gxyL);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED detg = 2*gxyL*gxzL*gyzL + 
      gyyL*(gxxL*gzzL - csetemp220) - gxxL*csetemp221 - 
      gzzL*csetemp222;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp223 = INV(detg);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED gu11 = (gyyL*gzzL - 
      csetemp221)*csetemp223;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED gu12 = (gxzL*gyzL - 
      gxyL*gzzL)*csetemp223;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED gu13 = (-(gxzL*gyyL) + 
      gxyL*gyzL)*csetemp223;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED gu22 = (gxxL*gzzL - 
      csetemp220)*csetemp223;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED gu23 = (gxyL*gxzL - 
      gxxL*gyzL)*csetemp223;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED gu33 = (gxxL*gyyL - 
      csetemp222)*csetemp223;
    
    betaxL = betal1*gu11 + betal2*gu12 + betal3*gu13;
    
    betayL = betal1*gu12 + betal2*gu22 + betal3*gu23;
    
    betazL = betal1*gu13 + betal2*gu23 + betal3*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED betasq = betaxL*betal1 + 
      betayL*betal2 + betazL*betal3;
    
    alpL = sqrt(betasq - g400);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtg11 = dg4110;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtg12 = dg4120;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtg13 = dg4130;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtg22 = dg4220;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtg23 = dg4230;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtg33 = dg4330;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg111 = dg4111;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg112 = dg4112;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg113 = dg4113;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg121 = dg4121;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg122 = dg4122;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg123 = dg4123;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg131 = dg4131;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg132 = dg4132;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg133 = dg4133;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg221 = dg4221;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg222 = dg4222;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg223 = dg4223;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg231 = dg4231;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg232 = dg4232;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg233 = dg4233;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg331 = dg4331;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg332 = dg4332;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dg333 = dg4333;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp224 = dtg11*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp225 = dtg12*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp226 = dtg13*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp227 = dtg12*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp228 = dtg22*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp229 = dtg23*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp230 = dtg13*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp231 = dtg23*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp232 = dtg33*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtgu11 = -((csetemp224 + csetemp225 + 
      csetemp226)*gu11) - (csetemp227 + csetemp228 + csetemp229)*gu12 - 
      (csetemp230 + csetemp231 + csetemp232)*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtgu12 = -((csetemp224 + csetemp225 + 
      csetemp226)*gu12) - (csetemp227 + csetemp228 + csetemp229)*gu22 - 
      (csetemp230 + csetemp231 + csetemp232)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtgu13 = -((csetemp224 + csetemp225 + 
      csetemp226)*gu13) - (csetemp227 + csetemp228 + csetemp229)*gu23 - 
      (csetemp230 + csetemp231 + csetemp232)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp233 = dtg11*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp234 = dtg12*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp235 = dtg13*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp236 = dtg22*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp237 = dtg23*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp238 = dtg13*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp239 = dtg23*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp240 = dtg33*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtgu22 = -((csetemp233 + csetemp234 + 
      csetemp235)*gu12) - (csetemp225 + csetemp236 + csetemp237)*gu22 - 
      (csetemp238 + csetemp239 + csetemp240)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtgu23 = -((csetemp233 + csetemp234 + 
      csetemp235)*gu13) - (csetemp225 + csetemp236 + csetemp237)*gu23 - 
      (csetemp238 + csetemp239 + csetemp240)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtgu33 = -(gu13*(dtg11*gu13 + 
      dtg12*gu23 + dtg13*gu33)) - gu23*(dtg12*gu13 + dtg22*gu23 + dtg23*gu33) 
      - gu33*(csetemp226 + csetemp237 + dtg33*gu33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp241 = dg111*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp242 = dg121*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp243 = dg131*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp244 = dg121*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp245 = dg221*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp246 = dg231*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp247 = dg131*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp248 = dg231*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp249 = dg331*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu111 = -((csetemp241 + csetemp242 + 
      csetemp243)*gu11) - (csetemp244 + csetemp245 + csetemp246)*gu12 - 
      (csetemp247 + csetemp248 + csetemp249)*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu121 = -((csetemp241 + csetemp242 + 
      csetemp243)*gu12) - (csetemp244 + csetemp245 + csetemp246)*gu22 - 
      (csetemp247 + csetemp248 + csetemp249)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu131 = -((csetemp241 + csetemp242 + 
      csetemp243)*gu13) - (csetemp244 + csetemp245 + csetemp246)*gu23 - 
      (csetemp247 + csetemp248 + csetemp249)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp250 = dg111*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp251 = dg121*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp252 = dg131*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp253 = dg221*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp254 = dg231*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp255 = dg131*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp256 = dg231*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp257 = dg331*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu221 = -((csetemp250 + csetemp251 + 
      csetemp252)*gu12) - (csetemp242 + csetemp253 + csetemp254)*gu22 - 
      (csetemp255 + csetemp256 + csetemp257)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu231 = -((csetemp250 + csetemp251 + 
      csetemp252)*gu13) - (csetemp242 + csetemp253 + csetemp254)*gu23 - 
      (csetemp255 + csetemp256 + csetemp257)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu331 = -(gu13*(dg111*gu13 + 
      dg121*gu23 + dg131*gu33)) - gu23*(dg121*gu13 + dg221*gu23 + dg231*gu33) 
      - gu33*(csetemp243 + csetemp254 + dg331*gu33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp258 = dg112*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp259 = dg122*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp260 = dg132*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp261 = dg122*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp262 = dg222*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp263 = dg232*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp264 = dg132*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp265 = dg232*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp266 = dg332*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu112 = -((csetemp258 + csetemp259 + 
      csetemp260)*gu11) - (csetemp261 + csetemp262 + csetemp263)*gu12 - 
      (csetemp264 + csetemp265 + csetemp266)*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu122 = -((csetemp258 + csetemp259 + 
      csetemp260)*gu12) - (csetemp261 + csetemp262 + csetemp263)*gu22 - 
      (csetemp264 + csetemp265 + csetemp266)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu132 = -((csetemp258 + csetemp259 + 
      csetemp260)*gu13) - (csetemp261 + csetemp262 + csetemp263)*gu23 - 
      (csetemp264 + csetemp265 + csetemp266)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp267 = dg112*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp268 = dg122*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp269 = dg132*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp270 = dg222*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp271 = dg232*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp272 = dg132*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp273 = dg232*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp274 = dg332*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu222 = -((csetemp267 + csetemp268 + 
      csetemp269)*gu12) - (csetemp259 + csetemp270 + csetemp271)*gu22 - 
      (csetemp272 + csetemp273 + csetemp274)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu232 = -((csetemp267 + csetemp268 + 
      csetemp269)*gu13) - (csetemp259 + csetemp270 + csetemp271)*gu23 - 
      (csetemp272 + csetemp273 + csetemp274)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu332 = -(gu13*(dg112*gu13 + 
      dg122*gu23 + dg132*gu33)) - gu23*(dg122*gu13 + dg222*gu23 + dg232*gu33) 
      - gu33*(csetemp260 + csetemp271 + dg332*gu33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp275 = dg113*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp276 = dg123*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp277 = dg133*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp278 = dg123*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp279 = dg223*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp280 = dg233*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp281 = dg133*gu11;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp282 = dg233*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp283 = dg333*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu113 = -((csetemp275 + csetemp276 + 
      csetemp277)*gu11) - (csetemp278 + csetemp279 + csetemp280)*gu12 - 
      (csetemp281 + csetemp282 + csetemp283)*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu123 = -((csetemp275 + csetemp276 + 
      csetemp277)*gu12) - (csetemp278 + csetemp279 + csetemp280)*gu22 - 
      (csetemp281 + csetemp282 + csetemp283)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu133 = -((csetemp275 + csetemp276 + 
      csetemp277)*gu13) - (csetemp278 + csetemp279 + csetemp280)*gu23 - 
      (csetemp281 + csetemp282 + csetemp283)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp284 = dg113*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp285 = dg123*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp286 = dg133*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp287 = dg223*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp288 = dg233*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp289 = dg133*gu12;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp290 = dg233*gu22;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp291 = dg333*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu223 = -((csetemp284 + csetemp285 + 
      csetemp286)*gu12) - (csetemp276 + csetemp287 + csetemp288)*gu22 - 
      (csetemp289 + csetemp290 + csetemp291)*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu233 = -((csetemp284 + csetemp285 + 
      csetemp286)*gu13) - (csetemp276 + csetemp287 + csetemp288)*gu23 - 
      (csetemp289 + csetemp290 + csetemp291)*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dgu333 = -(gu13*(dg113*gu13 + 
      dg123*gu23 + dg133*gu33)) - gu23*(dg123*gu13 + dg223*gu23 + dg233*gu33) 
      - gu33*(csetemp277 + csetemp288 + dg333*gu33);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtbetal1 = dg4010;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtbetal2 = dg4020;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtbetal3 = dg4030;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal11 = dg4011;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal12 = dg4012;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal13 = dg4013;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal21 = dg4021;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal22 = dg4022;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal23 = dg4023;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal31 = dg4031;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal32 = dg4032;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbetal33 = dg4033;
    
    dtbetaxL = betal1*dtgu11 + betal2*dtgu12 + betal3*dtgu13 + 
      dtbetal1*gu11 + dtbetal2*gu12 + dtbetal3*gu13;
    
    dtbetayL = betal1*dtgu12 + betal2*dtgu22 + betal3*dtgu23 + 
      dtbetal1*gu12 + dtbetal2*gu22 + dtbetal3*gu23;
    
    dtbetazL = betal1*dtgu13 + betal2*dtgu23 + betal3*dtgu33 + 
      dtbetal1*gu13 + dtbetal2*gu23 + dtbetal3*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta11 = betal1*dgu111 + 
      betal2*dgu121 + betal3*dgu131 + dbetal11*gu11 + dbetal21*gu12 + 
      dbetal31*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta21 = betal1*dgu121 + 
      betal2*dgu221 + betal3*dgu231 + dbetal11*gu12 + dbetal21*gu22 + 
      dbetal31*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta31 = betal1*dgu131 + 
      betal2*dgu231 + betal3*dgu331 + dbetal11*gu13 + dbetal21*gu23 + 
      dbetal31*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta12 = betal1*dgu112 + 
      betal2*dgu122 + betal3*dgu132 + dbetal12*gu11 + dbetal22*gu12 + 
      dbetal32*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta22 = betal1*dgu122 + 
      betal2*dgu222 + betal3*dgu232 + dbetal12*gu12 + dbetal22*gu22 + 
      dbetal32*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta32 = betal1*dgu132 + 
      betal2*dgu232 + betal3*dgu332 + dbetal12*gu13 + dbetal22*gu23 + 
      dbetal32*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta13 = betal1*dgu113 + 
      betal2*dgu123 + betal3*dgu133 + dbetal13*gu11 + dbetal23*gu12 + 
      dbetal33*gu13;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta23 = betal1*dgu123 + 
      betal2*dgu223 + betal3*dgu233 + dbetal13*gu12 + dbetal23*gu22 + 
      dbetal33*gu23;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dbeta33 = betal1*dgu133 + 
      betal2*dgu233 + betal3*dgu333 + dbetal13*gu13 + dbetal23*gu23 + 
      dbetal33*gu33;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtbetasq = dtbetaxL*betal1 + 
      dtbetayL*betal2 + dtbetazL*betal3 + betaxL*dtbetal1 + 
      betayL*dtbetal2 + betazL*dtbetal3;
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED csetemp292 = INV(alpL);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED dtalpL = 0.5*csetemp292*(-dg4000 + 
      dtbetasq);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED kxxL = 
      0.5*csetemp292*(2*(gxxL*dbeta11 + gxyL*dbeta21 + gxzL*dbeta31) + 
      betaxL*dg111 + betayL*dg112 + betazL*dg113 - dtg11);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED kxyL = 0.5*csetemp292*(gxxL*dbeta12 
      + gyyL*dbeta21 + gxyL*(dbeta11 + dbeta22) + gyzL*dbeta31 + 
      gxzL*dbeta32 + betaxL*dg121 + betayL*dg122 + betazL*dg123 - 
      dtg12);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED kxzL = 0.5*csetemp292*(gxxL*dbeta13 
      + gyzL*dbeta21 + gxyL*dbeta23 + gzzL*dbeta31 + gxzL*(dbeta11 + 
      dbeta33) + betaxL*dg131 + betayL*dg132 + betazL*dg133 - dtg13);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED kyyL = 
      0.5*csetemp292*(2*(gxyL*dbeta12 + gyyL*dbeta22 + gyzL*dbeta32) + 
      betaxL*dg221 + betayL*dg222 + betazL*dg223 - dtg22);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED kyzL = 0.5*csetemp292*(gxzL*dbeta12 
      + gxyL*dbeta13 + gyyL*dbeta23 + gzzL*dbeta32 + gyzL*(dbeta22 + 
      dbeta33) + betaxL*dg231 + betayL*dg232 + betazL*dg233 - dtg23);
    
    CCTK_REAL CCTK_ATTRIBUTE_UNUSED kzzL = 
      0.5*csetemp292*(2*(gxzL*dbeta13 + gyzL*dbeta23 + gzzL*dbeta33) + 
      betaxL*dg331 + betayL*dg332 + betazL*dg333 - dtg33);
    
    /* Copy local copies back to grid functions */
    alp[index] = alpL;
    betax[index] = betaxL;
    betay[index] = betayL;
    betaz[index] = betazL;
    dtalp[index] = dtalpL;
    dtbetax[index] = dtbetaxL;
    dtbetay[index] = dtbetayL;
    dtbetaz[index] = dtbetazL;
    gxx[index] = gxxL;
    gxy[index] = gxyL;
    gxz[index] = gxzL;
    gyy[index] = gyyL;
    gyz[index] = gyzL;
    gzz[index] = gzzL;
    kxx[index] = kxxL;
    kxy[index] = kxyL;
    kxz[index] = kxzL;
    kyy[index] = kyyL;
    kyz[index] = kyzL;
    kzz[index] = kzzL;
  }
  CCTK_ENDLOOP3(GaugeWave_initial);
}

extern "C" void GaugeWave_initial(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS
  
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Entering GaugeWave_initial_Body");
  }
  
  if (cctk_iteration % GaugeWave_initial_calc_every != GaugeWave_initial_calc_offset)
  {
    return;
  }
  
  const char* const groups[] = {
    "admbase::curv",
    "admbase::dtlapse",
    "admbase::dtshift",
    "admbase::lapse",
    "admbase::metric",
    "admbase::shift",
    "grid::coordinates"};
  GenericFD_AssertGroupStorage(cctkGH, "GaugeWave_initial", 7, groups);
  
  
  GenericFD_LoopOverEverything(cctkGH, GaugeWave_initial_Body);
  
  if (verbose > 1)
  {
    CCTK_VInfo(CCTK_THORNSTRING,"Leaving GaugeWave_initial_Body");
  }
}
