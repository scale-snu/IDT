 /*@@
   @file    Storage.c
   @date    Tue Jun 13 2000
   @author  Thomas Radke
   @desc
            Pugh storage functions
   @enddesc
   @version $Id: Storage.c 524 2013-08-02 21:34:14Z rhaas $
 @@*/

/* #define DEBUG_PUGH 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include "pughi.h"
#include "pugh_Comm.h"

static const char *rcsid="$Header:$";
CCTK_FILEVERSION(CactusPUGH_PUGH_Storage_c)


/********************************************************************
 ********************    Static Variables   *************************
 ********************************************************************/
static double totalstorage = 0; /* Storage for GVs in Bytes */
static double maxstorage = 0;   /* Maximum storage for GVs in Bytes */
static int totalnumberGVTL = 0; /* Number of stored GV time levels */
static int numberGVTL = 0;      /* Number of GV time levels at max */

/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static int PUGH_EnableGArrayGroupStorage (pGH *pughGH,
                                          int first_var,
                                          int n_variables,
                                          int n_timelevels);


static int PUGHi_EnableGArrayGroupStorage (pGH *pughGH,
                                           int first_var,
                                           int n_variables,
                                           int max_timelevels,
                                           int n_timelevels);

static int PUGHi_DisableGArrayGroupStorage (pGH *pughGH,
                                            int first_var,
                                            int n_variables,
                                            int max_timelevels,
                                            int n_timelevels);

/********************************************************************
 ********************    External Routines   ************************
 ********************************************************************/
int PUGHi_NumTimeLevelsArray(const pGH *pughGH, int var, int timelevels);
void PUGHi_PrintStorageReport (void);


 /*@@
   @routine    PUGH_ArrayGroupSize
   @author     Gabrielle Allen
   @date       11 Apr 1999
   @desc
               Returns size of arrays in a group, in a given direction.
               Either group index or its name must be given,
               the name takes precedence.
   @enddesc
   @calls      CCTK_GroupIndex
               CCTK_FirstVarIndexI


   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        dir
   @vdesc      direction to return size of
   @vtype      int
   @vio        in
   @endvar
   @var        group
   @vdesc      index of group
   @vtype      int
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of group
   @vtype      const char *
   @vio        in
   @endvar

   @returntype const int *
   @returndesc
               pointer to the size variable for the given direction, or
               NULL if given direction or group index/name are invalid
   @endreturndesc
@@*/
const int *PUGH_ArrayGroupSize (const cGH *GH,
                                int dir,
                                int group,
                                const char *groupname)
{
  int first_var;
  pGA *GA;
  const int *retval;
  static const int one = 1;


  if (groupname)
  {
    group = CCTK_GroupIndex (groupname);
  }

  first_var = CCTK_FirstVarIndexI (group);
  if (first_var >= 0)
  {
    /* get pointer to pGA for a timelevel of first variable in this group */
    GA = (pGA *) PUGH_pGH (GH)->variables[first_var][0];

    if (GA->storage == PUGH_STORAGE)
    {
      if (dir >= 0 && dir < GA->extras->dim)
      {
        retval = &GA->extras->lnsize[dir];
      }
      else
      {
        CCTK_WARN (1, "Wrong value for dir");
        retval = NULL;
      }
    }
    else
    {
      retval = &one;
    }
  }
  else if (CCTK_NumVarsInGroupI (group) == 0)
  {
    retval = &one;
  }
  else
  {
    if (groupname)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_ArrayGroupSize: Invalid group name '%s'",
                  groupname);
    }
    else
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_ArrayGroupSize: Invalid group ID %d",
                  group);
    }

    retval = NULL;
  }

  return (retval);
}


 /*@@
   @routine    PUGH_QueryGroupStorage
   @author     Gabrielle Allen
   @date       13 Apr 1999
   @desc
               Checks if group has storage assigned.
               Either group index or its name must be given,
               the name takes precedence.
   @enddesc
   @calls      CCTK_GroupIndex
               CCTK_FirstVarIndexI
               CCTK_GroupTypeI

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        group
   @vdesc      index of group
   @vtype      int
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of the group to be queried
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 if storage for this group is assigned
                0 if storage for this group is not assigned
               -1 if given group index/name is invalid
   @endreturndesc
@@*/
int PUGH_QueryGroupStorage (const cGH *GH, int group, const char *groupname)
{
  int first_var;
  int storage;
  int retval;
  pGH *pughGH;


  retval = -1;

  if (groupname)
  {
    group = CCTK_GroupIndex (groupname);
  }

  /* get first variable in group */
  first_var = CCTK_FirstVarIndexI (group);
  if (first_var >= 0)
  {
    pughGH = PUGH_pGH (GH);
    storage = ((pGA *) pughGH->variables[first_var][0])->storage;

    if (storage == PUGH_STORAGE)
    {
      retval = 1;
    }
    else if (storage == PUGH_NOSTORAGE)
    {
      retval = 0;
    }
    else
    {
      CCTK_WARN (1, "Inconsistency in PUGH_QueryGroupStorage");
    }
  }
  else
  {
    if (groupname)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_QueryGroupStorage: Invalid group name '%s'", groupname);
    }
    else
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_QueryGroupStorage: Invalid group ID %d", group);
    }
  }

  return (retval);
}


 /*@@
   @routine    PUGH_EnableGroupStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Enables storage for all variables in the group
               indicated by groupname.
   @enddesc
   @calls      CCTK_GroupIndex
               CCTK_GroupData
               PUGH_EnableGArrayGroupStorage

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of the group to enable storage for
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                return code of @seeroutine PUGH_EnableGArrayGroupStorage: <BR>
                1 if storage was already enabled, or <BR>
                0 if storage was successfully enabled <BR>
               -1 if group type is not one of the above <BR>
               -2 if an invalid GH pointer was given <BR>
               -3 if invalid groupname was given
   @endreturndesc
@@*/
int PUGH_EnableGroupStorage (const cGH *GH, const char *groupname)
{
  DECLARE_CCTK_PARAMETERS
  int group;               /* group index */
  int first_var;           /* first variable's index */
  cGroup pgroup;           /* group information */
  int retval;
  pGH *pughGH;


#ifdef DEBUG_PUGH
  printf (" PUGH_EnableGroupStorage: request for group '%s'\n", groupname);
  fflush (stdout);
#endif

  pughGH = PUGH_pGH (GH);
  group = CCTK_GroupIndex (groupname);

  if (pughGH && group >= 0)
  {
    first_var = CCTK_FirstVarIndexI (group);

    if (first_var >= 0)
    {

      /* get the group info from its index */
      CCTK_GroupData (group, &pgroup);

      retval = PUGH_EnableGArrayGroupStorage (pughGH,
                                              first_var,
                                              pgroup.numvars,
                                              pgroup.numtimelevels);
    }
    else
    {
      /* don't know what to say here; 0 seems safe */
      retval = 0;
    }

  }
  else
  {
    if (! pughGH)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_EnableGroupStorage: Error with cctkGH pointer "
                  "for group %s", groupname);
      retval = -2;
    }
    else
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGH_EnableGroupStorage: Invalid group %s", groupname);
      retval = -3;
    }
  }

  return (retval);
}


 /*@@
   @routine    PUGH_DisableGroupStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Disables storage for all variables in the group
               indicated by groupname.
   @enddesc
   @calls      CCTK_GroupIndex
               CCTK_GroupData
               CCTK_FirstVarIndexI
               PUGH_DisableGArrayGroupStorage

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of the group to enable storage for
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 if storage for given group was disabled
               -1 if group type is invalid
   @endreturndesc
@@*/
int PUGH_DisableGroupStorage (const cGH *GH, const char *groupname)
 {
  DECLARE_CCTK_PARAMETERS
  int group;               /* group index */
  cGroup pgroup;           /* group information */
  int retval;
  pGA ***variables;
  int first_var, var, level;
  int unchanged;           /* count how many aren't toggled */


#ifdef DEBUG_PUGH
  printf (" PUGH_DisableGroupStorage: request for group '%s'\n", groupname);
  fflush (stdout);
#endif

  group = CCTK_GroupIndex (groupname);
  CCTK_GroupData (group, &pgroup);

  /* get global index of first variable in group */
  first_var = CCTK_FirstVarIndexI (group);
  if (first_var < 0)
  {
    CCTK_WARN (0, "Illegal group index -- group has no variables");
  }

  variables = (pGA ***) PUGH_pGH (GH)->variables;

  /* get the group info from its index */
  unchanged = 0;

  retval = 1;
  for (var = first_var; var < first_var+pgroup.numvars; var++)
  {
    for (level = 0; level < pgroup.numtimelevels; level++)
    {
      unchanged += PUGH_DisableGArrayDataStorage (variables[var][level]);
    }
  }

  return (retval);
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/

 /*@@
   @routine    PUGH_EnableGArrayGroupStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Enables storage for a set of variables
   @enddesc
   @calls      PUGH_EnableGArrayDataStorage

   @var        pughGH
   @vdesc      Pointer to PUGH GH extensions
   @vtype      pGH *
   @vio        in
   @endvar
   @var        first_var
   @vdesc      index of the first variable to enable storage for
   @vtype      int
   @vio        in
   @endvar
   @var        n_variables
   @vdesc      total number of variables to enable storage for
   @vtype      int
   @vio        in
   @endvar
   @var        n_timelevels
   @vdesc      total number of timelevels to enable storage for
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 if storage was already enabled
                0 if storage was enabled (for at least one variable)
               -1 if there was an inconsistency in storage allocation
   @endreturndesc
@@*/
static int PUGH_EnableGArrayGroupStorage (pGH *pughGH,
                                          int first_var,
                                          int n_variables,
                                          int n_timelevels)
{
  DECLARE_CCTK_PARAMETERS
  int nstorage;   /* Number of Arrays for which storage was set */
  int nnostorage; /* Number of Arrays for which no storage was set */
  int retval;
  int var;
  pGA *GA;
  int level;


  nstorage = nnostorage = 0;

  for (var = first_var; var < first_var + n_variables; var++)
  {
    for (level = 0; level < n_timelevels; level++)
    {
      GA = pughGH->variables[var][level];
      if (GA->storage == PUGH_NOSTORAGE)
      {
#ifdef DEBUG_PUGH
        printf (" PUGH_EnableGArrayGroupStorage: request for var '%s' "
                "timelevel %d\n", GA->name, level);
        fflush (stdout);
#endif
        PUGH_EnableGArrayDataStorage (GA,
                                      pughGH->myproc,
                                      initialize_memory,
                                      padding_active,
                                      padding_cacheline_bits,
                                      padding_size,
                                      padding_address_spacing);

        nnostorage++;
      }
      else
      {
        nstorage++;
      }

      /* set the variable's data pointer in the cGH structure */
      ((cGH *) pughGH->callerid)->data[var][level] = GA->data;
    }
  }

  if (nstorage > 0 && nnostorage > 0)
  {
    CCTK_WARN (0, "Group storage violation in PUGH_EnableGArrayGroupStorage");
    retval = -1;
  }
  else
  {
    retval = nstorage > 0 ? 1 : 0;
  }

  return (retval);
}


 /*@@
   @routine    PUGH_EnableGArrayDataStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Allocates storage for a single variable.
               For now this routine cannot be made static because it's used
               in BAM :-(
   @enddesc
   @calls      Util_CacheMalloc

   @var        GA
   @vdesc      Pointer to the variable's info structure
   @vtype      pGA *
   @vio        in
   @endvar
   @var        this_proc
   @vdesc      the processor ID
   @vtype      int
   @vio        unused
   @endvar
   @var        initialize_memory
   @vdesc      how to initialize allocated memory
   @vtype      const char *
   @vio        in
   @endvar
   @var        padding_active, padding_cacheline_bits, padding_size,
               padding_address_spacing
   @vdesc      padding information
   @vtype      int
   @vio        unused
   @endvar

   @returntype int
   @returndesc
                0 if storage was enabled
               -1 if memory allocation failed
   @endreturndesc
@@*/
/* static */ int PUGH_EnableGArrayDataStorage (pGA *GA,
                                         int this_proc,
                                         const char *my_initialize_memory,
                                         int my_padding_active,
                                         int my_padding_cacheline_bits,
                                         int my_padding_size,
                                         int my_padding_address_spacing)
{
  DECLARE_CCTK_PARAMETERS
  int i, global_size, retval;


  /* avoid compiler warnings about unused parameters */
  this_proc = this_proc;
  my_padding_active = my_padding_active;
  my_padding_cacheline_bits = my_padding_cacheline_bits;
  my_padding_size = my_padding_size;
  my_padding_address_spacing = my_padding_address_spacing;

  retval = 0;
  if (GA->storage == PUGH_NOSTORAGE)
  {

#ifdef DEBUG_PUGH
    printf (" PUGH_EnableGArrayDataStorage: allocating storage "
            "for var '%s'\n", GA->name);
    fflush (stdout);
#endif

    if(GA->vector_size > 1 && GA->vector_entry > 0)
    {
      GA->data = (char *) GA->vector_base->data +
                 GA->extras->npoints * GA->varsize * GA->vector_entry;
      retval = 0;
    }
    else
    {

      /* Now assign memory for the variable itself */
      if (GA->padddata)
      {
        free (GA->padddata);
        GA->padddata = NULL;
      }

      if ((size_t) GA->extras->npoints * GA->varsize * GA->vector_size <= 0)
      {
        /* only warn if the global array size is also zero */
        for (i = 0, global_size = 1; i < GA->extras->dim; i++)
        {
          global_size *= GA->extras->nsize[i];
        }
        if (global_size <= 0)
        {
          CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                      "PUGH_EnableGArrayDataStorage: Tried to allocate storage "
                      "for zero-sized variable '%s'", GA->name);
        }
        GA->data = GA->padddata = NULL;
      }
      else if (! my_padding_active)
      {
        /* Easy case. */
        size_t align = PUGH_GetVectorSize() * GA->varsize;
        GA->padddata = malloc ((size_t) GA->extras->npoints * GA->varsize
			      * GA->vector_size + align-1);
        if (((size_t) GA->padddata) % align)
        {
          GA->data = (char *) GA->padddata + align - (((size_t) GA->padddata) % align);
        }
        else
        {
          GA->data = GA->padddata;
        }
      }
      else
      {
        /* Use the Cactus Cache alignment function */
        GA->data = Util_CacheMalloc (GA->arrayid,
                                     (size_t) GA->extras->npoints * GA->varsize
				     * GA->vector_size, &GA->padddata);
      }
      if (GA->extras->npoints * GA->varsize * GA->vector_size > 0 &&
          GA->padddata == NULL)
      {
	CCTK_VWarn(0, __LINE__, __FILE__, CCTK_THORNSTRING, "Error allocating memory "
		   "block of size %.0f",
		   (double) GA->extras->npoints * GA->varsize * GA->vector_size);
      }
      GA->npoints = GA->extras->npoints;
#ifdef DEBUG_PUGH
      printf (" PUGH_EnableGArrayDataStorage: new pointer is %p (%p)\n",
              GA->padddata, GA->data);
      fflush (stdout);
#endif

      /* Keep track of allocated memory */
      totalstorage += GA->npoints * GA->varsize * GA->vector_size;
      totalnumberGVTL += GA->vector_size;
      if (totalstorage > maxstorage)
      {
        numberGVTL = totalnumberGVTL;
        maxstorage = totalstorage;
      }

      /* Report on memory usage */
      if (CCTK_Equals(storage_verbose,"yes"))
      {
        CCTK_VInfo (CCTK_THORNSTRING, "Switched memory on for variable '%s'"
                    "  [GV TLs: %d Total Size: %6.2f MB]",
                    GA->name,
                    totalnumberGVTL, totalstorage / (double)(1024*1024));
      }

      /* Initialize the memory if desired. */
      if (GA->data)
      {
        PUGH_InitializeMemory (my_initialize_memory, GA->vtype,
                               GA->extras->npoints * GA->varsize * GA->vector_size, GA->data);
      }

      if (GA->extras->npoints * GA->varsize * GA->vector_size > 0 && GA->padddata == NULL)
      {
        CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "PUGH_EnableGArrayDataStorage: Cannot allocate data for "
                    "'%s' [%d]", GA->name, GA->id);
        retval = -1;
      }
    }
  }

  GA->storage = PUGH_STORAGE;

  return (retval);
}


 /*@@
   @routine    PUGH_DisableGArrayDataStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Frees allocated storage for a variable
               For now this routine cannot be made static because it's used
               in BAM :-(
   @enddesc

   @var        GA
   @vdesc      Pointer to the variable's info structure
   @vtype      pGA *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 if storage was already disabled
                0 if storage was freed
   @endreturndesc
@@*/
/* static */ int PUGH_DisableGArrayDataStorage (pGA *GA)
{
  DECLARE_CCTK_PARAMETERS
  int retval;

  retval = GA->storage != PUGH_STORAGE;

  if (GA->storage == PUGH_STORAGE)
  {

#ifdef DEBUG_PUGH
    printf (" PUGH_DisableGArrayDataStorage: freeing storage for var '%s'\n",
                GA->name);
    fflush (stdout);
#endif

    if(GA->vector_size > 1 && GA->vector_entry > 0)
    {
      GA->data = GA->padddata;
    }
    else
    {
      if (GA->padddata)
      {
#ifdef DEBUG_PUGH
        printf (" PUGH_DisableGArrayDataStorage: old pointer is %p (%p)\n",
                GA->padddata, GA->data);
        fflush (stdout);
#endif
        free (GA->padddata);
      }
      GA->padddata = NULL;
      GA->data = GA->padddata;
    }

    /* Keep track of allocated memory */
    totalstorage -= GA->npoints * GA->varsize * GA->vector_size;
    totalnumberGVTL -= GA->vector_size;

    /* Report on memory usage */
    if (CCTK_Equals(storage_verbose,"yes"))
    {
      CCTK_VInfo (CCTK_THORNSTRING, "Switched memory off for variable '%s'"
                  "  [GV TLs: %d Total Size: %6.2f MB]",
                  GA->name,
                  totalnumberGVTL, totalstorage / (double)(1024*1024));
    }

    GA->storage = PUGH_NOSTORAGE;
  }

  return retval;
}


 /*@@
   @routine    PUGH_InitializeMemory
   @author     Thomas Radke
   @date       Thu 30 Aug 2001
   @desc
               Initializes allocated memory to all zeros (all variable types)
               or NaNs (floating point types only)
   @enddesc

   @var        do_initialize_memory
   @vdesc      keyword describing how to initialize memory
   @vtype      const char *
   @vio        in
   @endvar
   @var        vtype
   @vdesc      CCTK variable type of the variable to initialize
   @vtype      int
   @vio        in
   @endvar
   @var        bytes
   @vdesc      total number of bytes to initialize
   @vtype      int
   @vio        in
   @endvar
   @var        data
   @vdesc      pointer to data to initialize
   @vtype      void *
   @vio        in
   @endvar
@@*/
void PUGH_InitializeMemory (const char *do_initialize_memory,
                            int vtype,
                            int bytes,
                            void *data)
{
  DECLARE_CCTK_PARAMETERS
  const char *vtypename;


  /* zero out variable */
  if (CCTK_Equals (do_initialize_memory, "zero"))
  {
    memset (data, 0, bytes);
  }
  /* set elements to Not-a-Number values (floating point variables only) */
  else if (CCTK_Equals (do_initialize_memory, "NaN"))
  {
    vtypename = CCTK_VarTypeName (vtype);
    if (strncmp (vtypename, "CCTK_VARIABLE_REAL",    18) == 0 ||
        strncmp (vtypename, "CCTK_VARIABLE_COMPLEX", 22) == 0)
    {
      memset (data, -1, bytes);
    }
  }
  else if (! CCTK_Equals (do_initialize_memory, "none"))
  {
    CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING,
                "InitializeMemory: Unknown value '%s' for "
                "argument 'do_initialize_memory'", do_initialize_memory);
  }
}



 /*@@
   @routine    PUGHi_PrintStorageReport
   @author     Gabrielle Allen
   @date       16th Sept 2001
   @desc
               Print a report about the use of storage
   @enddesc
@@*/
void PUGHi_PrintStorageReport ()
{
  CCTK_INFO("Storage statistics");
  CCTK_VInfo(CCTK_THORNSTRING, "  Maximum storage: %6.2f MB",
                               maxstorage / (double)(1024*1024));
  CCTK_VInfo(CCTK_THORNSTRING, "  [%d grid variable time levels]",
                               numberGVTL);
}


 /*@@
   @routine    PUGH_GroupStorageIncrease
   @date       Tue Apr 16 16:01:22 2002
   @author     Tom Goodale
   @desc
   The increase group storage routine should increase the allocated memory
   to the specified number of timelevels of each listed group, returning the
   previous number of timelevels enable for that group in the status array,
   if that is not NULL.  It should never decrease the number of timelevels enabled,
   i.e., if it is asked to enable less timelevels than are already enable it should
   not change the storage for that group.

   @enddesc
   @calls   PUGHi_EnableGArrayGroupStorage

   @var     GH
   @vdesc   Pointer to CCTK grid hierarchy
   @vtype   const cGH *
   @vio     inout
   @vcomment
   A driver should replace the appropriate GV pointers on this
   structure when they change the storage state of a GV.
   @endvar
   @var     n_groups
   @vdesc   Number of groups in group array
   @vtype   int
   @vio     in
   @endvar
   @var     groups
   @vdesc   Groups to allocate storage for
   @vtype   const int *
   @vio     in
   @vcomment
   This should be a list of group indices. -1 is treated as a flag to ignore this entry.
   @endvar
   @var     timelevels
   @vdesc   Number of timelevels to allocate storage for for each group.
   @vtype   const int *
   @vio     in
   @vcomment
   This array should have n_groups elements.
   @endvar
   @var     status
   @vdesc   Optional return array to contain previous state of storage for each group.
   @vtype   const int *
   @vio     in
   @vcomment
   If this array is not NULL, it will, on return, contain the number of timelevels which
   were previously allocated storage for each group.
   @endvar

   @returntype int
   @returndesc
      The total number of timelevels with storage enabled for all groups queried or
      modified.
   @endreturndesc
 @@*/
int PUGH_GroupStorageIncrease(const cGH *GH, int n_groups,const int *groups,const int *timelevels, int *status)
{
  int retval;
  int group;
  pGH *pughGH;

  retval = 0;
  pughGH = PUGH_pGH (GH);

  for(group = 0; group < n_groups; group++)
  {
    if(groups[group] > -1)
    {
      int previous;
      int first_var;
      int tlevels;
      cGroup pgroup;

      /* get the group info from its index */
      CCTK_GroupData (groups[group], &pgroup);

      first_var = CCTK_FirstVarIndexI (groups[group]);

      if (first_var >= 0)
      {

        /* Enable all timelevels or just some */
        if(timelevels[group] == -1)
        {
          tlevels = pgroup.numtimelevels;
        }
        else
        {
          tlevels = timelevels[group];
        }

        previous = PUGHi_EnableGArrayGroupStorage (pughGH,
                                                   first_var,
                                                   pgroup.numvars,
                                                   pgroup.numtimelevels,
                                                   tlevels);

        if(status)
        {
          status[group] = previous;
        }

        retval += previous;
      }
    }
  }


  /* FIXME: need to add stuff to record how juch storage is in use. */
  return retval;
}

 /*@@
   @routine    PUGH_GroupStorageDecrease
   @date       Tue Apr 16 16:01:22 2002
   @author     Tom Goodale
   @desc
   The decrease group storage routine should decrease the memory allocated
   to the specified number of timelevels for each listed group, returning the
   previous number of timelevels enable for that group in the status array,
   if that is not NULL.  It should never increase the number of timelevels enabled,
   i.e. if it is asked to reduce to more timelevels than are enable it should
   not change the storage for that group.

   @enddesc
   @calls   PUGHi_DisableGArrayGroupStorage

   @var     GH
   @vdesc   Pointer to CCTK grid hierarchy
   @vtype   const cGH *
   @vio     inout
   @vcomment
   A driver should replace the appropriate GV pointers on this
   structure when they change the storage state of a GV.
   @endvar
   @var     n_groups
   @vdesc   Number of groups in group array
   @vtype   int
   @vio     in
   @endvar
   @var     groups
   @vdesc   Groups to reduce storage for
   @vtype   const int *
   @vio     in
   @vcomment
   This should be a list of group indices. -1 is treated as a flag to ignore this entry.
   @endvar
   @var     timelevels
   @vdesc   Number of timelevels to reduce storage for for each group.
   @vtype   const int *
   @vio     in
   @vcomment
   This array should have n_groups elements.
   @endvar
   @var     status
   @vdesc   Optional return array to contain previous state of storage for each group.
   @vtype   const int *
   @vio     in
   @vcomment
   If this array is not NULL, it will, on return, contain the number of timelevels which
   were previously allocated storage for each group.
   @endvar

   @returntype int
   @returndesc
      The total number of timelevels with storage enabled for all groups queried or
      modified.
   @endreturndesc
 @@*/
int PUGH_GroupStorageDecrease(const cGH *GH, int n_groups,const int *groups,const int *timelevels, int *status)
{
  int retval;
  int group;
  pGH *pughGH;

  retval = 0;
  pughGH = PUGH_pGH (GH);

  for(group = 0; group < n_groups; group++)
  {
    if(groups[group] > -1)
    {
      int previous;
      int first_var;
      int tlevels;
      cGroup pgroup;

      /* get the group info from its index */
      CCTK_GroupData (groups[group], &pgroup);

      first_var = CCTK_FirstVarIndexI (groups[group]);

      if (first_var >= 0)
      {

        /* Disable all timelevels or just some */
        if(timelevels[group] == -1)
        {
          tlevels = 0;
        }
        else
        {
          tlevels = timelevels[group];
        }

        previous = PUGHi_DisableGArrayGroupStorage (pughGH,
                                                    first_var,
                                                    pgroup.numvars,
                                                    pgroup.numtimelevels,
                                                    tlevels);

        if(status)
        {
          status[group] = previous;
        }

        retval += previous;
      }
    }
  }


  /* FIXME: need to add stuff to record how juch storage is in use. */
  return retval;
}

 /*@@
   @routine    PUGHi_EnableGArrayGroupStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Enables storage for a set of variables
   @enddesc
   @calls      PUGH_EnableGArrayDataStorage

   @var        pughGH
   @vdesc      Pointer to PUGH GH extensions
   @vtype      pGH *
   @vio        in
   @endvar
   @var        first_var
   @vdesc      index of the first variable to enable storage for
   @vtype      int
   @vio        in
   @endvar
   @var        n_variables
   @vdesc      total number of variables to enable storage for
   @vtype      int
   @vio        in
   @endvar
   @var        max_timelevels
   @vdesc      total number of timelevels for this group
   @vtype      int
   @vio        in
   @var        n_timelevels
   @vdesc      number of timelevels to enable storage for
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
       The number of timelevels previously allocated
   @endreturndesc
@@*/
static int PUGHi_EnableGArrayGroupStorage (pGH *pughGH,
                                           int first_var,
                                           int n_variables,
                                           int max_timelevels,
                                           int n_timelevels)
{
  DECLARE_CCTK_PARAMETERS
  int nstorage;   /* Number of Arrays for which storage was set */
  int nnostorage; /* Number of Arrays for which no storage was set */
  int retval;
  int var;
  pGA *GA;
  int level;


  if (first_var < 0)
  {
    CCTK_WARN (0, "Illegal variable index");
  }

  retval = 0;
  nstorage = 0;
  nnostorage = 0;

  for (var = first_var; var < first_var + n_variables; var++)
  {
    for (level = 0; level < max_timelevels; level++)
    {
      GA = (pGA *) pughGH->variables[var][level];

      if (GA->storage == PUGH_NOSTORAGE)
      {

        if(level < n_timelevels)
        {
#ifdef DEBUG_PUGH
          printf (" PUGHi_EnableGArrayGroupStorage: request for var '%s' "
                  "timelevel %d\n", GA->name, level);
          fflush (stdout);
#endif
          PUGH_EnableGArrayDataStorage (GA,
                                        pughGH->myproc,
                                        initialize_memory,
                                        padding_active,
                                        padding_cacheline_bits,
                                        padding_size,
                                        padding_address_spacing);
        }

        nnostorage++;
      }
      else
      {
        nstorage++;

        /* Count the number of timelevels which were previously allocated. */
        if(var == first_var)
        {
          retval++;
        }
      }

      /* set the variable's data pointer in the cGH structure */
      ((cGH *) pughGH->callerid)->data[var][level] = GA->data;
    }
  }

  return retval;
}

 /*@@
   @routine    PUGHi_DisableGArrayGroupStorage
   @author     Tom Goodale
   @date       30 Mar 1999
   @desc
               Disables storage for a set of variables
   @enddesc
   @calls      PUGH_DisableGArrayDataStorage

   @var        pughGH
   @vdesc      Pointer to PUGH GH extensions
   @vtype      pGH *
   @vio        in
   @endvar
   @var        first_var
   @vdesc      index of the first variable to Disable storage for
   @vtype      int
   @vio        in
   @endvar
   @var        n_variables
   @vdesc      total number of variables to Disable storage for
   @vtype      int
   @vio        in
   @endvar
   @var        max_timelevels
   @vdesc      maximum number of timelevels for this group
   @vtype      int
   @vio        in
   @endvar
   @var        n_timelevels
   @vdesc      number of timelevels to leave storage for
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
       The number of timelevels previously allocated
   @endreturndesc
@@*/
static int PUGHi_DisableGArrayGroupStorage (pGH *pughGH,
                                            int first_var,
                                            int n_variables,
                                            int max_timelevels,
                                            int n_timelevels)
{
  DECLARE_CCTK_PARAMETERS
  int nstorage;   /* Number of Arrays for which storage was set */
  int nnostorage; /* Number of Arrays for which no storage was set */
  int retval;
  int var;
  pGA *GA;
  int level;


  if (first_var < 0)
  {
    CCTK_WARN (0, "Illegal variable index");
  }

  retval = 0;
  nstorage = 0;
  nnostorage = 0;

  for (var = first_var; var < first_var + n_variables; var++)
  {
    for (level = max_timelevels-1; level >= 0; level--)
    {
      GA = (pGA *) pughGH->variables[var][level];

      if (GA->storage == PUGH_STORAGE)
      {
        /* Count the number of timelevels which were previously allocated. */
        if(var == first_var)
        {
          retval++;
        }

        if(level >= n_timelevels)
        {
#ifdef DEBUG_PUGH
          printf (" PUGHi_DisableGArrayGroupStorage: request for var '%s' "
                  "timelevel %d\n", GA->name, level);
          fflush (stdout);
#endif
          PUGH_DisableGArrayDataStorage (GA);

          /* set the variable's data pointer in the cGH structure */
          ((cGH *) pughGH->callerid)->data[var][level] = NULL;
        }
        nstorage++;
      }
      else
      {
        nnostorage++;
      }
    }
  }

  return retval;
}

 /*@@
   @routine    PUGH_NumTimeLevels
   @date       Tue Apr 16 19:03:30 2002
   @author     Tom Goodale
   @desc
   Work out the number of enabled timelevels for a variable.
   @enddesc
   @calls      PUGHi_NumTimeLevelsArray

   @var     pughGH
   @vdesc   a PUGH GH
   @vtype   const pGH *
   @vio     in
   @var     var
   @vdesc   The variable index to get the info for
   @vtype   int
   @vio     in

   @returntype int
   @returndesc
     The number of timelevels enabled for this variable
   @endreturndesc
 @@*/
int PUGH_NumTimeLevels(const pGH *pughGH, int var)
{
  int retval;

  int timelevels;


  if (var < 0)
  {
    CCTK_WARN (0, "Illegal variable index");
  }

  timelevels = CCTK_MaxTimeLevelsVI(var);

  retval = PUGHi_NumTimeLevelsArray(pughGH, var, timelevels);

  return retval;
}

 /*@@
   @routine    PUGH_NumTimeLevelsArray
   @date       Tue Apr 16 19:03:30 2002
   @author     Tom Goodale
   @desc
   Work out the number of enabled timelevels for an array variable.
   @enddesc

   @var     pughGH
   @vdesc   a PUGH GH
   @vtype   const pGH *
   @vio     in
   @var     var
   @vdesc   The variable index to get the info for
   @vtype   int
   @vio     in
   @var     timelevels
   @vdesc   The maximum number of timelevels the variable can have
   @vtype   int
   @vio     in

   @returntype int
   @returndesc
     The number of timelevels enabled for this variable
   @endreturndesc
 @@*/
int PUGHi_NumTimeLevelsArray(const pGH *pughGH, int var, int timelevels)
{
  int retval;
  int level;
  pGA *GA;


  if (var < 0)
  {
    CCTK_WARN (0, "Illegal variable index");
  }

  retval = 0;

  for (level = 0; level < timelevels; level++)
  {
    GA = (pGA *) pughGH->variables[var][level];

    if (GA->storage == PUGH_STORAGE)
    {
      retval++;
    }
    else
    {
      break;
    }
  }

  return retval;
}
