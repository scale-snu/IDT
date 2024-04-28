 /*@@
   @file      Check.c
   @date      19 May 2003
   @author    David Rideout
   @desc
   Check that the dimension of any grid variables is not greater than
   what the faces specification can handle (currently 15).
   @enddesc
   @history
   @hdate     
   @hauthor   
   @hdesc     
   @endhistory
   @version   $Header$
 @@*/

#ifdef SPEC 
#include "Boundary/cctk.h" 
#define THORN_DIR "Boundary" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "Boundary/cctk_Arguments.h" 
#else 
#include "cctk_Arguments.h"
#endif /* END SPEC */ 

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusBase_Boundary_Check_c)

/* Maximum dimension for grid variables to be selected for BC.
   Restriction is representation of set of faces in a 32 bit signed integer. */
#define MAX_DIM 15

/********************************************************************
 ********************    Prototypes   *******************************
 ********************************************************************/

void Boundary_Check(CCTK_ARGUMENTS);

/********************************************************************
 ********************    Scheduled Routines   ***********************
 ********************************************************************/

/*@@
   @routine    Boundary_Check
   @date       19 May 2003
   @author     David Rideout
   @desc
   Check that the dimension of any grid variables is not greater than
   what the faces specification can handle (currently 15).
   @enddesc
   @calls      
   @history
   Note that in future versions of Cactus this will require the GH
   passed through CCTK_ARGUMENTS.
   @endhistory
   @var        CCTK_ARGUMENTS
   @vdesc      standard Cactus argument list
   @vtype      various
   @vio        in
   @endvar
   @returntype int
   @returndesc
               0 success
              -1 cactus executable contains variable group with too many 
                dimensions
   @endreturndesc
@@*/

void Boundary_Check(CCTK_ARGUMENTS)
{
  DECLARE_CCTK_ARGUMENTS

  if (CCTK_MaxDim() > 15)
  {
    CCTK_VWarn(1, __LINE__, __FILE__, CCTK_THORNSTRING,
               "Executable contains variable group with more than 15 "
               "dimensions.  Thorn Boundary will not function properly for "
               "these variable groups.");
  }
}
