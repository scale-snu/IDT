 /*@@
   @file      Comm.c
   @date      Thu Feb  4 11:34:29 1999
   @author    Tom Goodale
   @desc
              PUGH communication functions
   @enddesc
   @version   $Id: Comm.c 515 2012-07-04 01:29:14Z eschnett $
 @@*/

/* #define DEBUG_PUGH 1 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef SPEC 
#include "PUGH/cctk.h" 
#define THORN_DIR "PUGH" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 

#include "pugh.h"
#include "pughi.h"
#include "pugh_Comm.h"

static const char *rcsid="$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_Comm_c)


/* local function prototypes */
static int PUGH_EnableGArrayGroupComm(pGH *pughGH, int first_var, int commflag);
static int PUGH_EnableComm(pGH *pughGH, pComm *comm, int commflag);
static int PUGH_DisableComm(pGA *GA, pComm *comm);
static int PUGH_SyncGArrayGroup(pGH *pughGH, int first_var);
static int PUGH_Sync(pGH *pughGH, pComm *comm);
static int PUGH_SyncSingleProc(pGH *pughGH, pComm *comm);


/*@@
   @routine    PUGH_SyncGroup
   @author     Thomas Radke
   @date       30 Mar 1999
   @desc
               Synchronizes all variables in the group indicated by groupname.
   @enddesc
   @calls      PUGH_SyncGArrayGroup

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of the group to be synchronized
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                  return code of @seeroutine PUGH_SyncGArrayGroup
                  for grid array groups<BR>
               -1 for an unknown group or group type
   @endreturndesc
@@*/
int PUGH_SyncGroup(const cGH *GH, const char *groupname)
{
  cGroup pgroup;
  int group, retval;


#ifdef DEBUG_PUGH
  printf (" PUGH_SyncGroup: request for group '%s'\n", groupname);
  fflush (stdout);
#endif

  /* get the group info from its index */
  group = CCTK_GroupIndex(groupname);
  if (group < 0)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "PUGH_SyncGroup: Unknown group: %s", groupname);
    retval = -1;
  }
  else if (CCTK_NumVarsInGroupI(group) == 0)
  {
    retval = 0;  /* do nothing */
  }
  else
  {
    CCTK_GroupData(group, &pgroup);

    if (pgroup.numvars == 0)
    {
      retval = 0;
    }
    else
    {
      retval = PUGH_SyncGArrayGroup(PUGH_pGH(GH), CCTK_FirstVarIndexI(group));
    }
  }

  return (retval);
}


/*@@
   @routine    PUGH_SyncGroupsByDirI
   @author     Thomas Radke
   @date       29 June 2006
   @desc
               Synchronizes a list of groups in the given directions.
   @enddesc
   @calls      PUGH_SyncGArrayGroup

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        num_groups
   @vdesc      number of groups to be synchronised
   @vtype      int
   @vio        in
   @endvar
   @var        groups
   @vdesc      list of indices of groups to be synchronised
   @vtype      const int *
   @vio        in
   @endvar
   @var        directions
   @vdesc      list of directions to be synchronised
   @vtype      const int *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               returns the number of groups synchronised successfully
   @endreturndesc
@@*/
int PUGH_SyncGroupsByDirI(const cGH *GH,
                          int num_groups,
                          const int *groups,
                          const int *directions)
{
  int group, first_var, retval = 0;
  pGH *pughGH = PUGH_pGH (GH); 


  /* individual directions aren't supported (yet?) */
  if (directions != NULL)
  {
    CCTK_WARN (0, "PUGH doesn't support synchronisation of individual "
                  "directions");
  }

  /* synchronise all groups one by one */
  for (group = 0; group < num_groups; group++)
  {
#ifdef DEBUG_PUGH
    char *groupname = CCTK_GroupName (groups[group]);
    printf (" PUGH_SyncGroupsByDirI: request for group '%s'\n", groupname);
    fflush (stdout);
    free (groupname);
#endif

    first_var = CCTK_FirstVarIndexI (groups[group]);
    if (first_var >= 0 && PUGH_SyncGArrayGroup (pughGH, first_var) == 0)
    {
      retval++;
    }
  }

  return (retval);
}


/*@@
   @routine PUGH_EnableGroupComm
   @author     Thomas Radke
   @date       30 Mar 1999
   @desc
               Enables communication for all variables in the group
               indicated by groupname.
   @enddesc
   @calls      PUGH_EnableGArrayGroupComm

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of the group to be synchronized
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                  return code of @seeroutine PUGH_EnableGArrayGroupComm
                  for grid array groups<BR>
               -1 for an unknown group type
   @endreturndesc
 @@*/
int PUGH_EnableGroupComm(const cGH *GH, const char *groupname)
{
  int group, retval;
  cGroup pgroup;


#ifdef DEBUG_PUGH
  printf(" PUGH_EnableGroupComm: request for group '%s'\n", groupname);
  fflush(stdout);
#endif

  /* get the group info from its index */
  group = CCTK_GroupIndex(groupname);
  CCTK_GroupData(group, &pgroup);

  retval = PUGH_EnableGArrayGroupComm(PUGH_pGH(GH),
                                      CCTK_FirstVarIndexI(group),
                                      PUGH_ALLCOMM);

  return (retval);
}


/*@@
   @routine    PUGH_DisableGroupComm
   @author     Thomas Radke
   @date       30 Mar 1999
   @desc
               Disables communication for all variables in the group
               indicated by groupname.
   @enddesc
   @calls      PUGH_DisableGArrayGroupComm

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar
   @var        groupname
   @vdesc      name of the group to be synchronized
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
                  return code of @seeroutine PUGH_DisableGArrayGroupComm
                  for grid array groups<BR>
               -1 for an unknown group type
   @endreturndesc
 @@*/
int PUGH_DisableGroupComm(const cGH *GH, const char *groupname)
{
  int group, first_var, retval;
  cGroup pgroup;
  pGA *GA;
  pGH *pughGH;


#ifdef DEBUG_PUGH
  printf(" PUGH_DisableGroupComm: request for group '%s'\n", groupname);
  fflush(stdout);
#endif

  /* get the group info from its index */
  group = CCTK_GroupIndex(groupname);
  CCTK_GroupData(group, &pgroup);

  first_var = CCTK_FirstVarIndexI(group);
  if (first_var < 0)
  {
    CCTK_WARN (0, "Illegal group index -- group has no variables");
  }
  pughGH = PUGH_pGH(GH);
  GA = (pGA *) pughGH->variables[first_var][0];

  /* FIXME:  workaround.  This one is really bad ! */
  retval = PUGH_DisableGArrayGroupComm(pughGH, first_var, GA->groupcomm);

  return (retval);
}


/*@@
   @routine    PUGH_EnableGArrayComm
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Enables communication for a single array.
   @enddesc

   @var        GA
   @vdesc      Pointer to grid array structure
   @vtype      pGA *
   @vio        in
   @endvar
   @var        commflag
   @vdesc      flag indicating in which directions to communicate
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine PUGH_EnableComm
   @endreturndesc
 @@*/
int PUGH_EnableGArrayComm(pGA *GA, int commflag)
{

#ifdef DEBUG_PUGH
  printf(" PUGH_EnableGArrayComm: request for var '%s' commflag %d\n",
          GA->name, commflag);
  fflush(stdout);
#endif

  return (PUGH_EnableComm((pGH *) GA->parent, GA->comm, commflag));
}


/*@@
   @routine    PUGH_DisableGArrayComm
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Disables communication for a single array.
   @enddesc

   @var        GA
   @vdesc      Pointer to grid array structure
   @vtype      pGA *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine PUGH_DisableComm
   @endreturndesc
 @@*/
int PUGH_DisableGArrayComm(pGA *GA)
{
#ifdef DEBUG_PUGH
  printf(" PUGH_DisableGArrayComm: request for var '%s'\n", GA->name);
  fflush(stdout);
#endif

  return (PUGH_DisableComm(GA, GA->comm));
}


/*@@
   @routine    PUGH_SyncGArrayGroup
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Synchronizes a group of arrays
               given the first variable within this group.
   @enddesc

   @var        pughGH
   @vdesc      Pointer to PUGH grid hierarchy extension
   @vtype      pGH *
   @vio        in
   @endvar
   @var        first_var
   @vdesc      grid index of first variable in the group
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine PUGH_Sync
   @endreturndesc
 @@*/
static int PUGH_SyncGArrayGroup(pGH *pughGH, int first_var)
{
  int retval;
  pGA *firstGA;


  if (first_var<0 || first_var>=CCTK_NumVars())
  {
    CCTK_WARN (0, "internal error");
  }

  firstGA = (pGA *) pughGH->variables [first_var][0];

#ifdef DEBUG_PUGH
  printf(" PUGH_SyncGArrayGroup: request for group with first var '%s'\n",
         firstGA->name);
  fflush(stdout);
#endif

  /* Automatically enable communication for this group. */
  PUGH_EnableGArrayGroupComm(pughGH, first_var, PUGH_ALLCOMM);
  retval = PUGH_Sync(pughGH, firstGA->groupcomm);

  return (retval);
}


/*@@
   @routine    PUGH_SyncGArray
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Synchronizes a single array variable given the GA structure
               of this variable.
   @enddesc

   @var        GA
   @vdesc      Pointer to grid array structure
   @vtype      pGA *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine PUGH_Sync
   @endreturndesc
 @@*/
int PUGH_SyncGArray(pGA *GA)
{

#ifdef DEBUG_PUGH
  printf(" PUGH_SyncGArray: request for var '%s'\n", GA->name);
  fflush(stdout);
#endif

  return (PUGH_Sync((pGH *) GA->parent, GA->comm));
}


/*@@
   @routine    PUGH_DisableGArrayGroupComm
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Disables communication for a group of arrays
               given the first variable within this group.
   @enddesc

   @var        pughGH
   @vdesc      Pointer to PUGH grid hierarchy extension
   @vtype      pGH *
   @vio        in
   @endvar
   @var        first_var
   @vdesc      grid index of first variable in the group
   @vtype      int
   @vio        in
   @endvar
   @var        groupcomm
   @vdesc      pointer to comm structure for this group
   @vtype      pComm *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine PUGH_DisableComm
   @endreturndesc
@@*/
int PUGH_DisableGArrayGroupComm(pGH *pughGH, int first_var, pComm *groupcomm)
{
  pGA *GA;                 /* first variable in group */


  if (first_var < 0)
  {
    CCTK_WARN (0, "Illegal variable index");
  }

  GA = (pGA *) pughGH->variables[first_var][0];

#ifdef DEBUG_PUGH
  printf(" PUGH_DisableGArrayGroupComm: request for group "
         "with first var '%s'\n", GA->name);
  fflush(stdout);
#endif

  return (PUGH_DisableComm(GA, groupcomm));
}


/*@@
   @routine    PUGH_Barrier
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Synchronize all processors at a given point of execution.
   @enddesc

   @var        GH
   @vdesc      Pointer to CCTK grid hierarchy
   @vtype      const cGH *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               0 for success
   @endreturndesc
 @@*/
int PUGH_Barrier(const cGH *GH)
{
#ifdef CCTK_MPI
  CACTUS_MPI_ERROR (MPI_Barrier (GH ? PUGH_pGH (GH)->PUGH_COMM_WORLD :
                                      MPI_COMM_WORLD));
#else
  GH = GH;
#endif

  return (0);
}


/*****************************************************************************/
/* local functions                                                           */
/*****************************************************************************/

/*@@
   @routine    PUGH_EnableGArrayGroupComm
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Enables communication for a group of arrays
               given the first variable within this group.
   @enddesc

   @var        pughGH
   @vdesc      Pointer to PUGH grid hierarchy extension
   @vtype      pGH *
   @vio        in
   @endvar
   @var        first_var
   @vdesc      grid index of first variable in the group
   @vtype      int
   @vio        in
   @endvar
   @var        commflag
   @vdesc      flag indicating in which directions to communicate
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine PUGH_EnableComm
   @endreturndesc
 @@*/
static int PUGH_EnableGArrayGroupComm(pGH *pughGH, int first_var, int commflag)
{
  pGA *GA;                 /* first variable in group */


  if (first_var < 0)
  {
    CCTK_WARN (0, "Illegal variable index");
  }

  GA = pughGH->variables [first_var][0];

#ifdef DEBUG_PUGH
  printf(" PUGH_EnableGArrayGroupComm: request for group "
         "with first var '%s', commflag %d\n", GA->name, commflag);
  fflush(stdout);
#endif

  return (PUGH_EnableComm(pughGH, GA->groupcomm, commflag));
}


/*@@
   @routine    PUGH_EnableComm
   @date       Sun Jan 23 12:46:23 2000
   @author     Gabrielle Allen
   @desc
               This sets the docomm[2*dim] array of the GA based
               on the setting of the comm flag and allocates the comm buffers.
   @enddesc
   @history
               @date Mon Jun 05 2000 @author Thomas Radke
               Moved buffer allocation from PUGH_EnableGArrayDataStorage
   @endhistory

   @var        pughGH
   @vdesc      Pointer to PUGH grid hierarchy extension
   @vtype      pGH *
   @vio        in
   @endvar
   @var        comm
   @vdesc      Pointer to comm structure
   @vtype      pComm *
   @vio        in
   @endvar
   @var        commflag
   @vdesc      flag indicating in which directions to communicate
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               1 if communication was enabled,<BR>
               or negative otherwise
   @endreturndesc
@@*/
static int PUGH_EnableComm(pGH *pughGH, pComm *comm, int commflag)
{
  int retval;              /* return value */
#ifdef CCTK_MPI
  pGA *GA;                 /* GA structure the pComm belongs to */
  int i, idir;             /* loopers */
  int dir;                 /* direction */
  int sz;                  /* buffer size */
#endif


  retval = 1;

#ifdef CCTK_MPI
  /* check whether this comm buffer is already set up */
  if (comm->commflag == commflag)
  {
    return (retval);
  }

  /* get the GA structure the comm structure belongs to
     For a group comm structure this GA is the first one within the group. */
  GA = (pGA *) pughGH->variables [comm->first_var][comm->sync_timelevel];

  if (comm->commflag == PUGH_NOCOMM)
  {

#ifdef DEBUG_PUGH
    printf (" PUGH_EnableComm: allocating comm buffer for %d vars starting "
            "with %d '%s'\n", comm->n_vars, comm->first_var, GA->name);
    fflush (stdout);
#endif

    /* allocate memory for communication buffers: 2 faces per direction */
    for (i = 0; i < 2 * GA->extras->dim; i++)
    {
      comm->buffer_sz[i]   = 0;
      comm->send_buffer[i] = NULL;
      comm->recv_buffer[i] = NULL;

      if (GA->connectivity->neighbours[pughGH->myproc][i] >= 0)
      {
        dir = i/2;

        /* no ghostzones -> no comm buffers */
        sz = comm->n_vars * GA->extras->nghostzones[dir];
        if (sz > 0)
        {
          for (idir = 0; idir < GA->extras->dim; idir++)
          {
            if (idir != dir)
            {
              sz *= GA->extras->lnsize[idir];
            }
          }
          comm->buffer_sz[i]   = sz;
          comm->send_buffer[i] = malloc(sz * GA->varsize);
          comm->recv_buffer[i] = malloc(sz * GA->varsize);

          if (! (comm->send_buffer[i] && comm->recv_buffer[i]))
          {
            for (; i >=0 ; i--)
            {
              free(comm->send_buffer[i]);
              free(comm->recv_buffer[i]);
              comm->buffer_sz[i] = 0;
            }

            CCTK_WARN(1, "Out of memory !");
            retval = -1;
            break;
          }
        }
      }
    }
  }

  /* set up comm flags for each face */
  if (retval >= 0)
  {
    /* Copy commflag */
    comm->commflag = commflag;

    /* First set all communcation off */
    for (idir = 0; idir < 2 * GA->extras->dim; idir++)
    {
      comm->docomm[idir] = 0;
    }

    if (commflag == PUGH_ALLCOMM)
    {
      for (idir = 0; idir < 2 * GA->extras->dim; idir++)
      {
        comm->docomm[idir] = comm->buffer_sz[idir] > 0;
      }
    }
    else if (commflag == PUGH_PLUSFACESCOMM)
    {
      for (idir = 0; idir < GA->extras->dim; idir++)
      {
        comm->docomm[2*idir+1] = comm->buffer_sz[2*idir+1] > 0;
      }
    }
    else if (commflag == PUGH_MINUSFACESCOMM)
    {
      for (idir = 0; idir < GA->extras->dim; idir++)
      {
        comm->docomm[2*idir] = comm->buffer_sz[2*idir] > 0;
      }
    }
    else
    {
      for (idir = 0; idir < GA->extras->dim; idir++)
      {
        if (commflag == PUGH_COMM(idir))
        {
          comm->docomm[2*idir] = comm->buffer_sz[2*idir] > 0;
          comm->docomm[2*idir+1] = comm->buffer_sz[2*idir+1] > 0;
        }
      }
    }

    /* FIXME Add back the check that you have a valid COMM model: Gab */

    /* Handle nsize = 1 type cases. This is only important for one
       processor MPI periodic boundaries */
    for (idir = 0; idir < GA->extras->dim; idir++)
    {
      if (GA->extras->nsize[idir] == 1)
      {
        comm->docomm[2*idir] = 0;
        comm->docomm[2*idir+1] = 0;
      }
    }
  }

#else

  /* get rid of compiler warning about unused parameters */
  pughGH = pughGH;
  comm = comm;
  commflag = commflag;

#endif   /* CCTK_MPI */

  return retval;
}


/*@@
   @routine    PUGH_DisableComm
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               This frees the communication buffers of a given comm structure.
   @enddesc
   @history
               Separated from routine PUGH_DisableGArrayDataStorage()
   @endhistory

   @var        GA
   @vdesc      Pointer to grid array structure
   @vtype      pGA *
   @vio        in
   @endvar
   @var        comm
   @vdesc      Pointer to comm structure
   @vtype      pComm *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               1 for communication was disabled
   @endreturndesc
@@*/
static int PUGH_DisableComm(pGA *GA, pComm *comm)
{
#ifdef CCTK_MPI
  int i;                  /* looper */


#ifdef DEBUG_PUGH
  printf (" PUGH_DisableComm: freeing comm buffer for group of %d vars and "
          "first var '%s'\n", comm->n_vars, GA->name);
  fflush (stdout);
#endif

  if (comm->commflag != PUGH_NOCOMM)
  {
    /* free memory for communication buffers: 2 faces per direction */
    for (i = 0; i < 2 * GA->extras->dim; i++)
    {
      if (comm->send_buffer[i])
      {
        free(comm->send_buffer[i]);
        comm->send_buffer[i] = NULL;
      }

      if (comm->recv_buffer[i])
      {
        free(comm->recv_buffer[i]);
        comm->recv_buffer[i] = NULL;
      }

      comm->buffer_sz[i] = 0;
    }

    comm->commflag = PUGH_NOCOMM;
  }
  else
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "PUGH_DisableComm: Communication already disabled for "
                "group of %d vars and first var '%s'", comm->n_vars, GA->name);
  }

#else

  /* get rid of compiler warning about unused parameters */
  GA = GA;
  comm = comm;

#endif   /* CCTK_MPI */

  return (1);
}


/*@@
   @routine    PUGH_Sync
   @date       Mon Jun 05 2000
   @author     Thomas Radke
   @desc
               Finally synchronizes a variable or group of variables
               according to a given comm structure.
   @enddesc
   @calls      PUGH_SyncSingleProc

   @var        pughGH
   @vdesc      Pointer to PUGH grid extensions
   @vtype      pGH *
   @vio        in
   @endvar
   @var        comm
   @vdesc      Pointer to comm structure
   @vtype      pComm *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               0 for success
   @endreturndesc
@@*/
static int PUGH_Sync(pGH *pughGH, pComm *comm)
{
#ifdef CCTK_MPI
  int dir;
  pGA *GA;
  MPI_Status mss;
#ifdef PUGH_WITH_DERIVED_DATATYPES
  int i;
  MPI_Request *sr;
#endif
#ifdef COMM_TIMING
  double t1, t2;
#endif
#endif


  /* single-processor case is handled in separate routine */
  if (pughGH->nprocs == 1)
  {
    return (PUGH_SyncSingleProc (pughGH, comm));
  }

#ifdef CCTK_MPI

  /* start the timer for communication time */
  if (pughGH->comm_time >= 0)
  {
    CCTK_TimerStartI (pughGH->comm_time);
  }

  GA = (pGA *) pughGH->variables [comm->first_var][comm->sync_timelevel];

#ifdef PUGH_WITH_DERIVED_DATATYPES
  if (pughGH->commmodel == PUGH_DERIVEDTYPES)
  {
    /* 2 faces, send and receive is the 2 * 2 */
    sr = (MPI_Request *) malloc(comm->n_vars * 2 * 2 * sizeof(MPI_Request));
  }
#endif

#ifdef DEBUG_PUGH
  printf (" PUGH_Sync: syncing group of %d vars with first var '%s'\n",
          comm->n_vars, GA->name);
  fflush (stdout);
#endif

  for (dir = 0; dir < GA->extras->dim; dir ++)
  {

#ifdef COMM_TIMING
    t1 = MPI_Wtime();
#endif

    PostReceiveGA(pughGH, 2*dir, comm);
    PostReceiveGA(pughGH, 2*dir+1, comm);

#ifdef COMM_TIMING
    t2 = MPI_Wtime();
    printf("PR : %f\n",t2-t1);
#endif

    PostSendGA(pughGH, 2*dir, comm);
    PostSendGA(pughGH, 2*dir+1, comm);

#ifdef COMM_TIMING
    t1 = MPI_Wtime();
    printf("PS : %f\n",t1-t2);
#endif

    /* Now comes the big difference between derived types and
       allocated buffers. With derived types, we now have to
       wait on all our recieve AND SEND buffers so we can
       keep on using the send buffers ( as communications are
       in-place). With the allocated we have to wait on each
       recieve, but not on the send, since we don't need the
       send buffer until we pack a send again (above)
    */

    if (pughGH->commmodel == PUGH_ALLOCATEDBUFFERS)
    {
      /* Do a wait any on the receives */
      MPI_Wait(&comm->rreq[2*dir], &mss);
      FinishReceiveGA(pughGH, 2*dir, comm);
      MPI_Wait(&comm->rreq[2*dir+1], &mss);
      FinishReceiveGA(pughGH, 2*dir+1, comm);
    }
#ifdef PUGH_WITH_DERIVED_DATATYPES
    else if (pughGH->commmodel == PUGH_DERIVEDTYPES)
    {
      /* Load up the thing for the waitall */
      for (i = 0; i < comm->n_vars; i++)
      {
        int id = i * 2 * 2;
        pGA *GA = (pGA *) pughGH->variables [i][comm->sync_timelevel];

        if (GA->comm->docomm[2*dir] &&
            GA->storage)
        {
          sr[id] = GA->comm->sreq[2*dir];
          sr[id+1] = GA->comm->rreq[2*dir];
        }
        else
        {
          sr[id] = MPI_REQUEST_NULL;
          sr[id+1] = MPI_REQUEST_NULL;
        }

        if (GA->comm->docomm[2*dir+1] &&
            GA->storage)
        {
          sr[id+2] = GA->comm->sreq[2*dir+1];
          sr[id+3] = GA->comm->rreq[2*dir+1];
        }
        else
        {
          sr[id+2] = MPI_REQUEST_NULL;
          sr[id+3] = MPI_REQUEST_NULL;
        }
      }
      /* Now do a waitall */
      MPI_Waitall(4*comm->n_vars, sr, &mss);
    }
#endif

#ifdef COMM_TIMING
    t2 = MPI_Wtime();
    printf("FR : %f\n",t2-t1);
#endif

  }

#ifdef PUGH_WITH_DERIVED_DATATYPES
  if (pughGH->commmodel == PUGH_DERIVEDTYPES)
  {
    free(sr);
  }
  else
#endif
  {
    /* wait for MPI to finish all outstanding send requests */
    CACTUS_MPI_ERROR (MPI_Waitall (2 * GA->extras->dim, comm->sreq,
                                   comm->sstatus));
  }

  /* get the time spent in communication */
  if (pughGH->comm_time >= 0)
  {
    CCTK_TimerStopI(pughGH->comm_time);
  }

#endif /* CCTK_MPI */

  return (0);
}


/*@@
   @routine    PUGH_SyncSingleProc
   @date       Sun Jan 14 2001
   @author     Thomas Radke
   @desc
               Finally synchronizes a variable or group of variables
               in the single-processor case.
   @enddesc

   @var        pughGH
   @vdesc      Pointer to PUGH grid extensions
   @vtype      pGH *
   @vio        in
   @endvar
   @var        comm
   @vdesc      Pointer to comm structure
   @vtype      pComm *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               0 for success
   @endreturndesc
@@*/
static int PUGH_SyncSingleProc(pGH *pughGH, pComm *comm)
{
  pGA *GA;
  int i, face, dim, copy_bytes, offset_from, offset_to;
  char *data;
  int *istart_from, *iend_from, *iterator_from;
  int *istart_to, *iterator_to;
  int pass, num_passes, pass_total, pass_each, pass_from, pass_to;


  GA = (pGA *) pughGH->variables [comm->first_var][comm->sync_timelevel];

  /* return if no storage assigned */
  if (! GA->storage)
  {
    CCTK_VWarn(2, __LINE__, __FILE__, CCTK_THORNSTRING,
               "Trying to synchronize variable '%s' with no storage", GA->name);
    return (0);
  }

  /* since we need two iterators here we need to allocate one */
  iterator_from = GA->extras->iterator;
  iterator_to = (int *) malloc (GA->extras->dim * sizeof (int));

  /* loop over all faces */
  for (face = 0; face < 2 * GA->extras->dim; face++)
  {

    /* there's only something to do for periodic boundary conditions */
    if (GA->connectivity->perme[face / 2])
    {
      /* if the domain is smaller than the number of ghost zones, then
         we have to copy in several passes */
      /* FIXME: these passes do not each need to copy the whole
         regaion, they could copy a part of it only */
      /* total number of points to copy */
      pass_total = GA->extras->nghostzones[face>>1];
      /* points copied in one pass */
      pass_each = (GA->extras->lnsize[face>>1]
                   - 2 * GA->extras->nghostzones[face>>1]);
      /* necessary number of passes */
      num_passes = (pass_total + pass_each - 1) / pass_each;
      for (pass = 0; pass < num_passes; pass++)
      {
        /* calculate what needs to be done in this pass */
        /* FIXME: these values are currently ignored */
        pass_from = pass * pass_each;
        pass_to = pass_from + pass_each - 1;
        if (pass_to > pass_total) pass_to = pass_total;

        /* get the index ranges for the nested loops */
        istart_from   = GA->extras->overlap[0][face];
        iend_from     = GA->extras->overlap[1][face];
        if (face & 1)
        {
          istart_to   = GA->extras->ghosts[0][face - 1];
        }
        else
        {
          istart_to   = GA->extras->ghosts[0][face + 1];
        }

        /* set iterators to the start vector */
        for(dim = 0; dim < GA->extras->dim; dim++)
        {
          iterator_from[dim] = istart_from[dim];
          iterator_to[dim] = istart_to[dim];
        }

        /* get the number of bytes to copy in the lowest dimension */
        copy_bytes = (iend_from[0] - istart_from[0]) * GA->varsize;

        /* now do the nested loops starting with the innermost */
        dim = 1;
        while (1)
        {
          /* check for end of current loop */
          if (GA->extras->dim > 1 && iterator_from[dim] >= iend_from[dim])
          {
            /* increment outermost loopers */
            for (dim++; dim < GA->extras->dim; dim++)
            {
              iterator_from[dim]++;
              iterator_to[dim]++;
              if (iterator_from[dim] < iend_from[dim])
              {
                break;
              }
            }

            /* done if beyond outermost loop */
            if (dim >= GA->extras->dim)
            {
              break;
            }

            /* reset innermost loopers */
            for (dim--; dim > 0; dim--)
            {
              iterator_from[dim] = istart_from[dim];
              iterator_to[dim] = istart_to[dim];
            }
            dim = 1;
          }

          /* get the linear offsets */
          offset_from = istart_from[0];
          offset_to   = istart_to[0];
          for (i = 1; i < GA->extras->dim; i++)
          {
            offset_from += iterator_from[i] * GA->extras->hyper_volume[i];
            offset_to   += iterator_to[i]   * GA->extras->hyper_volume[i];
          }
          offset_from *= GA->varsize;
          offset_to   *= GA->varsize;

          /* copy the data for all the variables in the comm structure */
          for (i = comm->first_var; i < comm->first_var + comm->n_vars; i++)
          {
            data = ((pGA *) pughGH->variables[i][comm->sync_timelevel])->data;
            memcpy (data + offset_to, data + offset_from, copy_bytes);
          }

          /* increment current loopers */
          if (GA->extras->dim > 1)
          {
            /* increment current looper */
            iterator_from[dim]++;
            iterator_to[dim]++;
          }
          else
          {
            /* exit loop if array dim is 1D */
            break;
          }

        } /* end of nested loops over all dimensions */
      } /* for pass */
    } /* if periodic */
  } /* for face */

  /* free the allocated iterator */
  free (iterator_to);

  return (0);
}
