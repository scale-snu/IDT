 /*@@
   @file      Startup.c
   @date      Wed Feb  3 23:10:19 1999
   @author    Tom Goodale
   @desc 
   Startup routines for pugh.
   @enddesc 
   @version $Header$
 @@*/

#ifdef SPEC 
#include "PUGH/cctk.h" 
#define THORN_DIR "PUGH" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "PUGH/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

#include "pugh.h"
#include "pugh_extension.h"
#include "pugh_Comm.h"

static const char *rcsid="$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_Startup_c)

int PUGH_Startup(void);

 /*@@
   @routine    PUGH_Startup
   @date       Wed Feb  3 23:14:38 1999
   @author     Tom Goodale
   @desc 
   The startup registration routine for PUGH.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
@@*/
int PUGH_Startup(void)
{
  int pugh_GHExtension;

  DECLARE_CCTK_PARAMETERS

  /* Register the stuff in the GH extension. */
  pugh_GHExtension = CCTK_RegisterGHExtension("PUGH");

  CCTK_RegisterGHExtensionSetupGH(pugh_GHExtension, PUGH_SetupGH);

  CCTK_RegisterGHExtensionInitGH(pugh_GHExtension, PUGH_InitGH);

  CCTK_RegisterGHExtensionScheduleTraverseGH(pugh_GHExtension, PUGH_ScheduleTraverseGH);

  /* Overload some functions */ 
  if (overloadevolve)
  {
    CCTK_OverloadEvolve(PUGH_Evolve);  
  }
  if (overloadsyncgroup)
  {
    CCTK_OverloadSyncGroup(PUGH_SyncGroup);
  }
  if (overloadsyncgroupsbydiri)
  {
    CCTK_OverloadSyncGroupsByDirI(PUGH_SyncGroupsByDirI);
  }
  if (overloadenablegroupstorage)
  {
    CCTK_OverloadGroupStorageIncrease(PUGH_GroupStorageIncrease);
  }
  if (overloaddisablegroupstorage)
  {
    CCTK_OverloadGroupStorageDecrease(PUGH_GroupStorageDecrease);
  }
  if (overloadenablegroupcomm)
  {
    CCTK_OverloadEnableGroupComm(PUGH_EnableGroupComm);
  }
  if (overloaddisablegroupcomm)
  {
    CCTK_OverloadDisableGroupComm(PUGH_DisableGroupComm);
  }
  if (overloadbarrier)
  {
    CCTK_OverloadBarrier(PUGH_Barrier);
  }
  if (overloadparallelinit)
  {
    CCTK_OverloadParallelInit(PUGH_ParallelInit);
  }
  if (overloadexit)
  {
    CCTK_OverloadExit(PUGH_Exit);
  }
  if (overloadabort)
  {
    CCTK_OverloadAbort(PUGH_Abort);
  }
  if (overloadmyproc)
  {
    CCTK_OverloadMyProc(PUGH_MyProc);
  }
  if (overloadnprocs)
  {
    CCTK_OverloadnProcs(PUGH_nProcs);
  }
  if (overloadarraygroupsizeb)
  {
    CCTK_OverloadArrayGroupSizeB(PUGH_ArrayGroupSize);
  }
  if (overloadquerygroupstorageb)
  {
    CCTK_OverloadQueryGroupStorageB(PUGH_QueryGroupStorage);
  }
  if (overloadgroupdynamicdata)
  {
    CCTK_OverloadGroupDynamicData(PUGH_GroupDynamicData);
  }

  /* Register the PUGH banner */
  CCTK_RegisterBanner("Driver provided by PUGH");

  return 0;
}


#if 0
 /*@@
   @routine    WhatIsPugh
   @date       Mon Apr 14 22:13:12 1997
   @author     Paul Walker... with some by Joan...
   @desc 
   Returns a quasi-random saying which describes exactly what
   pugh stands for. It doesn't have the REAL answer in the
   list though ... but it is hidden somewhere in the source
   code ...
   @enddesc 
   @calledby   main
@@*/

static int npn=18;
static char pughnames[18][70] =
  {"Peculiar Unwieldy Green Hairball",
   "Paranoid Upity Gorilla Hater",
   "Pathetic Ugly Game Hall",
   "Position Under Gargantuan Hills",
   "Pair Up Gangly Hyacinths",
   "Pickles? Ungodly Green Hors d'ouvres",
   "POW!!!!!!    (Ugh......)",
   "Practically Unusable Gross Hack (Thanks to MT)",
   "Pluck Unpleasant Growing Hairs",
   "Artichoke Inversion Subsystem Manager", 
   "Piles 'uv Guava Hoops",
   "Perl Ueber Goober Hoover",
   "Paul's Un-Thesis with Good HotPeppers!",
   "Purple Unwearable Garish Horror",
   "Parsiminious Unbearable Gazelle Hunter",
   "Pedos Ululeantes Gorgojeando Hostias",
   "Paridas Urdidas Gracias a Hijos de...",
   "Putes Untades de Greix i Homides"
  };
   
static char *WhatIsPugh(void) 
{
  int somerand = 0;             /* A # between 1 and 1000 */
  int which;

  which = (int)(somerand * npn / 1000);
  if (which > npn-1) which = npn-1;
  return (pughnames[which]);

}
#endif
