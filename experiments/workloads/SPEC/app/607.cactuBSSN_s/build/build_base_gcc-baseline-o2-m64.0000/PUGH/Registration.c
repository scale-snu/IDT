/*@@
  @file      Registration.c
  @date      Sat Oct  8 19:29:25 CDT 2005
  @author    Dylan Stark (TommyD)
  @desc
             Allows registration of topology and 
             physical to logical processor mapping routines.

             Mostly taken from work done by Thomas Dramlitsch.
  @enddesc
  @version   $Header$
 @@*/

#include <stdio.h>
#include <stdlib.h>

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
#include "Register.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_Registerables_c)

/********************************************************************
 *********************  Macro Definitions  **************************
 ********************************************************************/

/********************************************************************
 *********************  Local Data Types  ***************************
 ********************************************************************/

/********************************************************************
 *********************  Aliased Routine Prototypes  *****************
 ********************************************************************/

/********************************************************************
 *********************  Scheduled Routine Prototypes  ***************
 ********************************************************************/

/********************************************************************
 *********************  Fortran Wrapper Prototypes  *****************
 ********************************************************************/

/********************************************************************
 *********************  Local Routine Prototypes  *******************
 ********************************************************************/

static int GenerateTopologyUnavailable(int dim, 
                                       int total_procs, 
                                       const int *nsize, 
                                       const int *nghostzones, 
                                       int *nprocs);
static void *CreateP2LTranslationUnavailable(int dim, 
                                             int *nprocs, 
                                             int np);
static int DestroyP2LTranslationUnavailable(void *table);

static int PhysicalToLogicalUnavailable (void *table, int proc);

static int LogicalToPhysicalUnavailable(void *table, int num);

/********************************************************************
 *********************  Local Data  *********************************
 ********************************************************************/

int (*PUGH_GenerateTopology) (int dim, 
                              int total_procs, 
                              const int *nsize, 
                              const int *nghostzones, 
                              int *nprocs) = GenerateTopologyUnavailable;

void *(*PUGHi_CreateP2LTranslation) (int dim, 
                                     int *nprocs, 
                                     int np) = CreateP2LTranslationUnavailable;

int   (*PUGHi_DestroyP2LTranslation) (void *table) = DestroyP2LTranslationUnavailable;
int   (*PUGHi_PhysicalToLogical) (void *table, int proc) = PhysicalToLogicalUnavailable;
int   (*PUGHi_LogicalToPhysical) (void *table, int num) = LogicalToPhysicalUnavailable;

/********************************************************************
 ************************   External Routines    ********************
 ********************************************************************/

/* Registration routines */

/*@@
   @routine    PUGH_RegisterGenerateTopology
   @date       Sat Oct  8 19:29:25 CDT 2005
   @author     Dylan Stark (Thomas Dramlitsch)
   @desc 
   Registration routine for topology generators.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
int PUGH_RegisterGenerateTopology(int (*GenerateTopology) (int dim,
                                                           int total_procs, 
                                                           const int *nsize, 
                                                           const int *nghostzones, 
                                                           int *nprocs), 
                                  const char *topologyname)
{
  DECLARE_CCTK_PARAMETERS
  
  if (CCTK_Equals(processor_topology, topologyname))
  {
    if (PUGH_GenerateTopology != GenerateTopologyUnavailable)
    {
      CCTK_VWarn(0,__LINE__,__FILE__,CCTK_THORNSTRING,
                 "Attempted to register topology generator %s more than once!", 
                 topologyname);
    }
    else
    {
      PUGH_GenerateTopology = GenerateTopology;
      CCTK_VInfo(CCTK_THORNSTRING, "Using topology generator: %s", 
                 topologyname);
      
    }
  }

  return 0;
}

/*@@
   @routine    PUGH_RegisterP2L
   @date       Sat Oct  8 19:29:25 CDT 2005
   @author     Dylan Stark (Thomas Dramlitsch)
   @desc 
   Registration routine for physical to logical processor mappings.   
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
int PUGH_RegisterP2L(void *(*CreateP2LTranslation) (int dim, 
                                                    int *nprocs, 
                                                    int np),
                     int   (*DestroyP2LTranslation) (void *table),
                     int   (*PhysicalToLogical) (void *table, 
                                                 int proc),
                     int   (*LogicalToPhysical) (void *table, 
                                                 int num),
                     const char *p2lname)
{
  DECLARE_CCTK_PARAMETERS
  
  if (CCTK_Equals(physical2logical, p2lname))
  {
    if (PUGHi_CreateP2LTranslation != CreateP2LTranslationUnavailable)
    {
      CCTK_VWarn(0, __LINE__,__FILE__,CCTK_THORNSTRING,
                 "Attempted to register P2L methods %s more than once!", 
                 physical2logical);
    }
    else
    {
      PUGHi_CreateP2LTranslation  = CreateP2LTranslation;
      PUGHi_DestroyP2LTranslation = DestroyP2LTranslation;
      PUGHi_PhysicalToLogical     = PhysicalToLogical;
      PUGHi_LogicalToPhysical     = LogicalToPhysical;

      CCTK_VInfo(CCTK_THORNSTRING, "Using physical to logical mappings: %s",
                 p2lname);
    }
  }
 
  return 0;
}

/********************************************************************
 *********************  Local Routines  *****************************
 ********************************************************************/

 /*@@
   @routine    GenerateTopologyUnavailable
   @date       Wed Dec 14 21:14:55 2005
   @author     Tom Goodale
   @desc 
   Generates an error if the routine has not been registered.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int GenerateTopologyUnavailable(int dim, 
                                       int total_procs, 
                                       const int *nsize, 
                                       const int *nghostzones, 
                                       int *nprocs)
{
  DECLARE_CCTK_PARAMETERS

  CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING, 
              "No processor topology routine registered under the name '%s'"
              " set in the processor_topology parameter.",
              processor_topology);
  return 0;
}

 /*@@
   @routine    CreateP2LTranslationUnavailable
   @date       Wed Dec 14 21:14:55 2005
   @author     Tom Goodale
   @desc 
   Generates an error if the routine has not been registered.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static void *CreateP2LTranslationUnavailable(int dim, 
                                             int *nprocs, 
                                             int np)
{
  DECLARE_CCTK_PARAMETERS

  CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING, 
              "No physical to logical mapping routine registered under the name '%s'"
              " set in the physical2logical parameter.",
              physical2logical);
  return 0;
}

 /*@@
   @routine    DestroyP2LTranslationUnavailable
   @date       Wed Dec 14 21:14:55 2005
   @author     Tom Goodale
   @desc 
   Generates an error if the routine has not been registered.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int DestroyP2LTranslationUnavailable(void *table)
{
  DECLARE_CCTK_PARAMETERS

  CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING, 
              "No physical to logical mapping routine registered under the name '%s'"
              " set in the physical2logical parameter.",
              physical2logical);
  return 0;
}

 /*@@
   @routine    PhysicalToLogicalUnavailable
   @date       Wed Dec 14 21:14:55 2005
   @author     Tom Goodale
   @desc 
   Generates an error if the routine has not been registered.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int PhysicalToLogicalUnavailable (void *table, int proc)
{
  DECLARE_CCTK_PARAMETERS

  CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING, 
              "No physical to logical mapping routine registered under the name '%s'"
              " set in the physical2logical parameter.",
              physical2logical);
  return 0;
}

 /*@@
   @routine    LogicalToPhysicalUnavailable
   @date       Wed Dec 14 21:14:55 2005
   @author     Tom Goodale
   @desc 
   Generates an error if the routine has not been registered.
   @enddesc 
   @calls     
   @calledby   
   @history 
 
   @endhistory 
 
 @@*/
static int LogicalToPhysicalUnavailable(void *table, int num)
{
  DECLARE_CCTK_PARAMETERS

  CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING, 
              "No physical to logical mapping routine registered under the name '%s'"
              " set in the physical2logical parameter.",
              physical2logical);
  return 0;
}
