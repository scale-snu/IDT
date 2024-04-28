/*@@
  @file      ChooseOutput.c
  @author    Gabrielle Allen
  @date      July 6 2000
  @desc
             Choose what 1D slices and 2D planes to output by IOASCII.
  @enddesc

  @version   $Id: ChooseOutput.c 218 2012-04-20 05:25:51Z knarf $
 @@*/

#include <stdlib.h>
#include <string.h>

#ifdef SPEC 
#include "IOASCII/cctk.h" 
#define THORN_DIR "IOASCII" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOASCII/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOASCII/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "CactusBase/IOUtil/src/ioutil_Utils.h"
#include "ioASCIIGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusBase_IOASCII_ChooseOutput_c)


/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* macro to choose origin according actual parameter settings:
     1. Indices from IOASCII
     2. Indices from IOUtil
     3. Coords from IOASCII
     4. Coords from IOUtil
 */
#define GET_SLICE(IOASCII_param, IOUtil_param, index, coord)                  \
        {                                                                     \
          index = IOASCII_param##i >= 0 ? IOASCII_param##i : IOUtil_param##i; \
          coord = IOASCII_param != -424242 ? IOASCII_param : IOUtil_param;    \
        }


/********************************************************************
 ********************    External Routines   ************************
 ********************************************************************/
void IOASCII_Choose1D (CCTK_ARGUMENTS);
void IOASCII_Choose2D (CCTK_ARGUMENTS);


/*@@
   @routine   IOASCII_Choose1D
   @author    Gabrielle Allen
   @date      July 6 2000
   @desc
              Use parameters to choose the 1D slices through the output data.
   @enddesc

   @calls     IOUtil_1DLines

   @var       GH
   @vdesc     pointer to CCTK grid hierarchy
   @vtype     const cGH *
   @vio       in
   @endvar
@@*/
void IOASCII_Choose1D (CCTK_ARGUMENTS)
{
  int i, j, maxdim;
  asciiioGH *myGH;
  int *origin_index[3];
  CCTK_REAL *origin_phys[3];
  DECLARE_CCTK_PARAMETERS


  /* allocate arrays for origins */
  origin_phys[0] = calloc (3 * 3, sizeof (CCTK_REAL));
  origin_phys[1] = origin_phys[0] + 3;
  origin_phys[2] = origin_phys[1] + 3;
  origin_index[0] = calloc (3 * 3, sizeof (int));
  origin_index[1] = origin_index[0] + 3;
  origin_index[2] = origin_index[1] + 3;

  /* get slice points */
  GET_SLICE (out1D_xline_y, out_xline_y, origin_index[0][1], origin_phys[0][1]);
  GET_SLICE (out1D_xline_z, out_xline_z, origin_index[0][2], origin_phys[0][2]);
  GET_SLICE (out1D_yline_x, out_yline_x, origin_index[1][0], origin_phys[1][0]);
  GET_SLICE (out1D_yline_z, out_yline_z, origin_index[1][2], origin_phys[1][2]);
  GET_SLICE (out1D_zline_x, out_zline_x, origin_index[2][0], origin_phys[2][0]);
  GET_SLICE (out1D_zline_y, out_zline_y, origin_index[2][1], origin_phys[2][1]);

  maxdim = CCTK_MaxDim ();
  myGH = CCTK_GHExtension (cctkGH, "IOASCII");
  myGH->spxyz = malloc (maxdim * sizeof (int **));

  for (i = 0; i < maxdim; i++)
  {
    myGH->spxyz[i] = malloc ((i + 1) * sizeof (int *));

    for (j = 0; j <= i; j++)
    {
      myGH->spxyz[i][j] = calloc (i + 1, sizeof (int));
    }

    if (i < 3)
    {
      IOUtil_1DLines (cctkGH, i + 1, origin_index, origin_phys, myGH->spxyz[i]);
    }
  }

  /* free allocated resources */
  free (origin_phys[0]);
  free (origin_index[0]);
}


/*@@
   @routine   IOASCII_Choose2D
   @author    Gabrielle Allen
   @date      July 6 2000
   @desc
              Use parameters to choose the 2D slices through the output data.
   @enddesc

   @calls     IOUtil_2DPlanes

   @var       GH
   @vdesc     Pointer to CCTK grid hierarchy
   @vtype     const cGH *
   @vio       in
   @endvar
 @@*/
void IOASCII_Choose2D (CCTK_ARGUMENTS)
{
  int i, maxdim;
  asciiioGH *myGH;
  int origin_index[3];
  CCTK_REAL origin_phys[3];
  DECLARE_CCTK_PARAMETERS


  GET_SLICE (out2D_xyplane_z, out_xyplane_z, origin_index[0], origin_phys[0]);
  GET_SLICE (out2D_xzplane_y, out_xzplane_y, origin_index[1], origin_phys[1]);
  GET_SLICE (out2D_yzplane_x, out_yzplane_x, origin_index[2], origin_phys[2]);

  maxdim = CCTK_MaxDim ();
  myGH = CCTK_GHExtension (cctkGH, "IOASCII");
  myGH->sp2xyz = malloc (maxdim * sizeof (int *));

  for (i = 0; i < maxdim; i++)
  {
    myGH->sp2xyz[i] = calloc (i + 1, sizeof (int));

    if (i == 2)
    {
      IOUtil_2DPlanes (cctkGH, i + 1, origin_index, origin_phys, myGH->sp2xyz[i]);
    }
  }
}
