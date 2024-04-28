/*@@
   @file      cctk_DebugDefines.h
   @date      Sun 28 Dec 2003
   @author    Erik Schnetter
   @desc
              Routines to provide some debugging support for the Cactus code.
   @enddesc
   @version   $Id: cctk_DebugDefines.h 4814 2012-05-07 23:16:28Z eschnett $
 @@*/

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

#ifndef _CCTK_DEBUGDEFINES_H_
#define _CCTK_DEBUGDEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

int CCTK_GFIndex0D (const cGH *GH);
int CCTK_GFIndex1D (const cGH *GH, int i);
int CCTK_GFIndex2D (const cGH *GH, int i, int j);
int CCTK_GFIndex3D (const cGH *GH, int i, int j, int k);
int CCTK_GFIndex4D (const cGH *GH, int i, int j, int k, int l);

int CCTK_VectGFIndex0D (const cGH *GH, int n);
int CCTK_VectGFIndex1D (const cGH *GH, int i, int n);
int CCTK_VectGFIndex2D (const cGH *GH, int i, int j, int n);
int CCTK_VectGFIndex3D (const cGH *GH, int i, int j, int k, int n);
int CCTK_VectGFIndex4D (const cGH *GH, int i, int j, int k, int l, int n);

#ifdef __cplusplus
}   
#endif

#endif
