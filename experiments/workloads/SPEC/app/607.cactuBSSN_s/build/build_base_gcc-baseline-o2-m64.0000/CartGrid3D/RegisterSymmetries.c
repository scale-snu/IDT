/* $Header$ */

#include <stdlib.h>

#ifdef SPEC 
#include "CartGrid3D/cctk.h" 
#define THORN_DIR "CartGrid3D" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "CartGrid3D/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "CartGrid3D/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

#include "Symmetry.h"

void DecodeSymParameters3D(int sym[6]);
void RegisterSymmetryBoundaries (CCTK_ARGUMENTS);

void RegisterSymmetryBoundaries (CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS
  DECLARE_CCTK_PARAMETERS

  int sym[6];
  int n;
  CCTK_INT faces[6];
  CCTK_INT width[6];
  CCTK_INT handle;

  DecodeSymParameters3D (sym);

  for (n=0; n<6; ++n)
  {
    faces[n] = sym[n];
    width[n] = cctk_nghostzones[n/2];
  }

  handle = SymmetryRegister ("CartGrid3D");
  if (handle < 0)
  {
    CCTK_WARN (0, "Could not register symmetry boundary condition");
  }

  if (SymmetryRegisterGrid (cctkGH, handle, faces, width) < 0)
  {
    CCTK_WARN (0, "Could not register the symmetry boundaries -- probably some other thorn has already registered the same boundary faces for a different symmetry");
  }
}
