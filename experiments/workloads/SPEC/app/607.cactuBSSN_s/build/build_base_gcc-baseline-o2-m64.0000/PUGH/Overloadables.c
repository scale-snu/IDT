/*@@
   @file      Overloadables.c
   @author    Gabrielle Allen
   @date      2000/06/22
   @desc
              PUGH routines which overload CCTK routines in the flesh
   @enddesc
   @version   $Id: Overloadables.c 515 2012-07-04 01:29:14Z eschnett $
 @@*/

#include <stdlib.h>
#ifdef SPEC 
#include "PUGH/cctk.h" 
#define THORN_DIR "PUGH" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "pugh.h"
#include "pugh_Comm.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_Overloadables_c)


int PUGHi_NumTimeLevelsArray(const pGH *pughGH, int var, int timelevels);


/*@@
   @routine    PUGH_GroupDynamicData
   @author     Gabrielle Allen
   @date       2000/06/22
   @desc
               Returns the driver's internal data for a given group
   @enddesc
   @calls      CCTK_GroupTypeI
               CCTK_FirstVarIndexI

   @var        GH
   @vdesc      Pointer to CCTK GH
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        group
   @vdesc      index of group
   @vtype      int
   @vio        in
   @endvar
   @var        data
   @vdesc      Pointer to caller-supplied data structure to store group data
   @vtype      cGroupDynamicData *
   @vio        out
   @endvar

   @returntype int
   @returndesc
                0 for success <BR>
               -1 if given pointer to data structure is NULL <BR>
               -3 if given GH pointer is invalid
               -77 if group has zero variables
   @endreturndesc
@@*/
int PUGH_GroupDynamicData (const cGH *GH, int group, cGroupDynamicData *data)
{
  int var;
  pGH *pughGH;
  pGExtras *extras;
  int retval;


  if (data)
  {
    /* Get the first variable in the group */
    var = CCTK_FirstVarIndexI (group);
    if (var<0)
    {
      retval = -77;
      return retval;
    }

    pughGH = PUGH_pGH (GH);
    if (pughGH)
    {
      extras = ((pGA ***) pughGH->variables)[var][0]->extras;

      data->dim         = extras->dim;
      data->lsh         = extras->lnsize;
      data->ash         = extras->lnsize;
      data->gsh         = extras->nsize;
      data->lbnd        = extras->lb[pughGH->myproc];
      data->ubnd        = extras->ub[pughGH->myproc];
      data->nghostzones = extras->nghostzones;
      data->bbox        = extras->bbox;
      data->activetimelevels
        = PUGHi_NumTimeLevelsArray (pughGH, var, CCTK_MaxTimeLevelsGI (group));

      retval = 0;
    }
    else
    {
      retval = -3;
    }
  }
  else
  {
    retval = -1;
  }

  return (retval);
}
