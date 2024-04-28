/*@@
  @file      GetSymmetry.c
  @date      April 12 2002
  @author    Frank Herrmann
  @desc
             This file contains the routines for getting symmetry information
             code stolen from SetSymmetry.c
  @enddesc
  @version   $Id: GetSymmetry.c 233 2007-02-16 12:33:40Z tradke $
@@*/

#include <stdlib.h>

#ifdef SPEC 
#include "CartGrid3D/cctk.h" 
#define THORN_DIR "CartGrid3D" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "CartGrid3D/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "cctk_FortranString.h"
#include "Symmetry.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_CartGrid3D_GetSymmetry_c)

/********************************************************************
 *********************     Local Defines      ***********************
 ********************************************************************/
#define MAX_DIM  3
#define MAX_FACE 6


/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/
void DecodeSymParameters3D(int sym[6]);

void CCTK_FCALL getcartsymvi_
                          (int *ierr, const cGH **GH, int *sym, const int *vi);
void CCTK_FCALL getcartsymvn_
                          (int *ierr, const cGH **GH, int *sym, ONE_FORTSTRING_ARG);

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

/*@@
  @routine    GetCartSymVI
  @date       Mon Mar 15 15:10:58 1999
  @author     Frank Herrmann
  @desc
              This routine returns symmetry for variable index.
  @enddesc
@@*/
int GetCartSymVI(const cGH *GH, int *sym, int vi)
{
  int domainsym[MAX_FACE];
  SymmetryGHex *sGHex;
  int dir;
  DECLARE_CCTK_PARAMETERS


  if (vi < 0 || vi >= CCTK_NumVars ())
  {
    CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
               "Invalid variable index %d in GetCartSymVI", vi);
    return(-1);
  }

  /* Pointer to the SymmetryGHextension */
  sGHex = (SymmetryGHex *)CCTK_GHExtension(GH, "Symmetry");

  /* Reference the hash table in the GHex and get the kind of
   * symmetry being applied
   */

#ifdef SYM_DEBUG
   printf("GetSymmetry: %s [%d,%d,%d]\n",CCTK_VarName(vi),
                            sym[0],sym[1],sym[2]);
#endif

  DecodeSymParameters3D(domainsym);

  for (dir=0; dir<MAX_FACE; ++dir)
  {
    sym[dir/2] = GFSYM_UNKNOWN;
    if (domainsym[dir])
      sym[dir/2]=sGHex->GFSym[vi][dir];
  }

  return 0;
}

void CCTK_FCALL getcartsymvi_
                          (int *ierr, const cGH **GH, int *sym, const int *vi)
{
  *ierr = GetCartSymVI(*GH, sym, *vi);
}


/*@@
  @routine    GetCartSymVN
  @date       April 12 2002
  @author     Frank Herrmann
  @desc
              Gets symmetry boundary conditions from variable name
  @enddesc
@@*/
int GetCartSymVN(const cGH *GH, int *sym, const char *vn)
{
  int vi;
  vi = CCTK_VarIndex(vn);

  if (vi>-1)
  {
    return(GetCartSymVI(GH, sym, vi));
  }
  else
  {
    CCTK_VWarn(1,__LINE__,__FILE__,CCTK_THORNSTRING,
               "Cannot find variable %s in GetCartSymVN",vn);
    return(-1);
  }
}

void CCTK_FCALL getcartsymvn_
                          (int *ierr, const cGH **GH, int *sym, ONE_FORTSTRING_ARG)
{
ONE_FORTSTRING_CREATE(vn)
  *ierr = GetCartSymVN(*GH, sym, vn);
  free(vn);
}
