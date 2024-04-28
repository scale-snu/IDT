 /*@@
   @file      Termination.c
   @date      Thu March 15 7pm
   @author    Gabrielle Allen
   @desc
              Termination conditions
   @enddesc
   @version   $Id: Termination.c 3446 2003-10-28 13:48:26Z schnetter $
 @@*/

#include "cctk_Flesh.h"
#include "cctk_Termination.h"

static const char *rcsid="$Header$";

CCTK_FILEVERSION(main_Termination_c)

/********************************************************************
 *********************     Local Data   *****************************
 ********************************************************************/
static int termination_reached = 0;

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/
void CCTK_FCALL cctk_terminatenext_ (const cGH **GH);


 /*@@
   @routine    CCTK_TerminationReached
   @date       Thu March 15 7pm
   @author     Gabrielle Allen
   @desc
               Returns true if Cactus will terminate on the next iteration.
   @enddesc
   @var        GH
   @vdesc      pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                true (1) or false (0)
   @endreturndesc
@@*/
int CCTK_TerminationReached (const cGH *GH)
{
  GH = GH;

  return (termination_reached);
}


 /*@@
   @routine    CCTK_TerminateNext
   @date       Thu March 15 7pm
   @author     Gabrielle Allen
   @desc
               Sets termination for next iteration.
   @enddesc
   @var        GH
   @vdesc      pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
@@*/
void CCTK_TerminateNext (const cGH *GH)
{
  GH = GH;

  termination_reached = 1;
}

void CCTK_FCALL cctk_terminatenext_ (const cGH **GH)
{
  CCTK_TerminateNext (*GH);
}
