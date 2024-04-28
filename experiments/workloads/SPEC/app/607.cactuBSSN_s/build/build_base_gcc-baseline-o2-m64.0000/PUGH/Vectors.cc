 /*@@
   @file    Vectors.cc
   @date    Tue Jul 30 20:25:08 PDT 2013
   @author  Roland Haas
   @desc
            Helper routine to return vector size from Vectors thorn
   @enddesc
 @@*/

#include <cstddef>

#include "vectors.h"

#include "pugh.h"
#include "pughi.h"

extern "C"
size_t PUGH_GetVectorSize()
{
#ifdef CCTK_REAL_VEC_SIZE
  return  CCTK_REAL_VEC_SIZE;
#else
  return 1;
#endif
}
