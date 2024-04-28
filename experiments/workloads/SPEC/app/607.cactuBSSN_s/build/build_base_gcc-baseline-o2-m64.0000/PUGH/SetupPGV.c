 /*@@
   @file      SetupPGV.c
   @date      Wed Oct 27 11:09:05 1999
   @author    Tom Goodale
   @desc
   Sets up a PUGH grid variable.
   @enddesc
   @version $Header$
 @@*/

/* #define DEBUG_PUGH 1 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
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



static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_SetupPGV_c)


/********************************************************************
 *********************     Local Data Types   ***********************
 ********************************************************************/

/********************************************************************
 ********************* Local Routine Prototypes *********************
 ********************************************************************/

static int PUGH_SetupPGExtrasMemory(int dim, int total_procs, pGExtras *this);
static int PUGH_SetupPGExtrasOwnership(int dim,
                                       int *perme,
                                       int this_proc,
                                       int nprocs,
                                       int **neighbours,
                                       pGExtras *this);
static int PUGH_SetupBoundingBox(int is_gf,
                                 int dim,
                                 int *nghosts,
                                 int total_procs,
                                 int *nprocs,
                                 pGExtras *this);
static int PUGH_SetupRemoteSizes(int dim, int total_procs, pGExtras *this);
static int PUGH_SetupPGExtrasSizes(int is_gf,
                                   int dim,
                                   int *sh,
                                   int *nghosts,
                                   int total_procs,
                                   int *nprocs,
                                   int this_proc,
                                   pGExtras *this);
static int PUGH_ComposeIJK(int dim, int *nprocs, int *pos);
static int PUGH_DecomposeIJK(int dim, int ijk, int *nprocs, int *pos);

/********************************************************************
 ********************* Other Routine Prototypes *********************
 ********************************************************************/

/********************************************************************
 *********************     Local Data   *****************************
 ********************************************************************/

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/

 /*@@
   @routine    PUGH_SetupPGExtras
   @date       Tue Nov  9 09:23:27 1999
   @author     Tom Goodale
   @desc
   Sets up a PGExtras structure.
   @enddesc
@@*/
pGExtras *PUGH_SetupPGExtras(int is_gf,
                             int dim,
                             int *perme,
                             int *sh,
                             int *nghosts,
                             int total_procs,
                             int *nprocs,
                             int **neighbours,
                             int this_proc)
{
  pGExtras *this;

  this = malloc(sizeof(pGExtras));

  /* Setup memory */
  if(this)
  {
    if (! PUGH_SetupPGExtrasMemory(dim, total_procs, this))
    {
      this->dim = dim;

      PUGH_SetupPGExtrasSizes(is_gf, dim, sh, nghosts,
                              total_procs, nprocs, this_proc,this);
      PUGH_SetupPGExtrasOwnership(dim, perme, this_proc, total_procs,
                                  neighbours, this);
    }
  }

  return this;
}

 /*@@
   @routine    PUGH_DestroyPGExtras
   @date       Mar 12 2000
   @author     Thomas Radke
   @desc
   Destroys a PGExtras structure.
   @enddesc
@@*/
void PUGH_DestroyPGExtras(pGExtras **PGExtras)
{
  if(PGExtras && *PGExtras)
  {
    int j;

    for(j = 0; j < 2; j++)
    {
      if((*PGExtras)->dim > 0)
      {
        free((*PGExtras)->ghosts[j][0]);
        free((*PGExtras)->overlap[j][0]);
      }
      free((*PGExtras)->ownership[j]);
      free((*PGExtras)->ghosts[j]);
      free((*PGExtras)->overlap[j]);
    }
    free((*PGExtras)->lb[0]);
    free((*PGExtras)->ub[0]);
    free((*PGExtras)->rnsize[0]);

    free((*PGExtras)->lb);
    free((*PGExtras)->ub);
    free((*PGExtras)->rnsize);
    free((*PGExtras)->rnpoints);
    free((*PGExtras)->nghostzones);
    free((*PGExtras)->nsize);
    free((*PGExtras)->lnsize);
    free((*PGExtras)->bbox);
    free((*PGExtras)->iterator);
    free((*PGExtras)->hyper_volume);

    free(*PGExtras);
    *PGExtras = NULL;
  }
}

 /*@@
   @routine    PUGH_SetupConnectivity
   @date       Fri Nov  5 11:32:12 1999
   @author     Tom Goodale
   @desc
   Create a connectivity structure containing
   all the details of processor connectivities
   for this processor layout.
   @enddesc
@@*/
pConnectivity *PUGH_SetupConnectivity(int dim,
                                      int total_procs,
                                      const int *nsize,
                                      const int *nghostzones,
                                      int *nprocs,
                                      int *perme)
{
  DECLARE_CCTK_PARAMETERS
  pConnectivity *this;

  int i;

  /* Allocate memory */
  this = malloc(sizeof(pConnectivity));

  if(this)
  {
    this->nprocs     = malloc(dim*sizeof(int));
    this->perme      = malloc(dim*sizeof(int));
    this->neighbours = malloc(total_procs*sizeof(int *));

    if(! dim || this->neighbours)
    {
      this->neighbours[0] = malloc(total_procs*2*dim*sizeof(int));
    }
    else
    {
      free(this->neighbours);
      this->neighbours = NULL;
    }

    if((dim && ! this->nprocs) || (dim && ! this->perme) || ! this->neighbours)
    {
      if(this->neighbours)
      {
        free(this->neighbours[0]);
      }

      free(this->neighbours);
      free(this->perme);
      free(this->nprocs);

      free(this);
      this = NULL;
    }
  }

  /* Fill in data */
  if(this)
  {
    this->dim = dim;

    for(i = 0 ; i < dim; i++)
    {
      this->nprocs[i] = nprocs[i];
      this->perme[i] = perme[i];
    }

    for(i = 1; i < total_procs; i++)
    {
      this->neighbours[i] = this->neighbours[0]+(2*dim*i);
    }

    PUGH_GenerateTopology(dim, total_procs, nsize, nghostzones, 
                          this->nprocs);

    PUGH_GenerateNeighbours(dim, total_procs, this->nprocs, this->neighbours,
                            this->perme);
  }

  return this;
}

 /*@@
   @routine    PUGH_DestroyConnectivity
   @date       Mar 12 2000
   @author     Thomas Radke
   @desc
   Destroys a connectivity structure containing
   all the details of processor connectivities
   for this processor layout.
   @enddesc
@@*/
void PUGH_DestroyConnectivity(pConnectivity **conn)
{
  if (conn && *conn)
  {
    free((*conn)->perme);
    free((*conn)->neighbours[0]);
    free((*conn)->neighbours);
    free((*conn)->nprocs);
    free(*conn);
    *conn = NULL;
  }
}

 /*@@
   @routine    PUGH_GenerateNeighbours
   @date       Mon Nov  8 08:15:08 1999
   @author     Tom Goodale
   @desc
   Works out the array of neighbouring processors for
   every processor.
   @enddesc
@@*/
int PUGH_GenerateNeighbours(int dim,
                            int total_procs,
                            int *nprocs,
                            int **neighbours,
                            int *perme)
{
  int retval;
  int i;

  int idim;
  int *pos;
  int temp;

  void *table;

  /* Create P2L translation table. */
  table = (void *)PUGHi_CreateP2LTranslation(dim, nprocs, total_procs);

  pos = malloc(dim*sizeof(int));

  if(dim == 0 || pos)
  {
    for(i = 0; i < total_procs; i++)
    {
      PUGH_DecomposeIJK(dim, PUGHi_PhysicalToLogical(table, i), nprocs, pos);

      for(idim = 0; idim < dim ; idim++)
      {
        /* Deal with minus neighbour in this direction */
        pos[idim]--;

        if(pos[idim] > -1)
        {
          neighbours[i][idim*2] = 
           PUGHi_LogicalToPhysical(table, PUGH_ComposeIJK(dim, nprocs, pos));
        }
        else if(perme[idim])
        {
          temp = pos[idim];
          pos[idim] = nprocs[idim]-1;
          neighbours[i][idim*2] = 
            PUGHi_LogicalToPhysical(table, PUGH_ComposeIJK(dim, nprocs, pos));
          pos[idim] = temp;
        }
        else
        {
          neighbours[i][idim*2] = -1;
        }

        pos[idim]++;

        /* Deal with plus neighbour in this direction */
        pos[idim]++;

        if(pos[idim] < nprocs[idim])
        {
          neighbours[i][idim*2+1] = 
            PUGHi_LogicalToPhysical(table, PUGH_ComposeIJK(dim, nprocs, pos));
        }
        else if(perme[idim])
        {
          temp = pos[idim];
          pos[idim] = 0;
          neighbours[i][idim*2+1] = 
            PUGHi_LogicalToPhysical(table, PUGH_ComposeIJK(dim, nprocs, pos));
          pos[idim] = temp;
        }
        else
        {
          neighbours[i][idim*2+1] = -1;
        }

        pos[idim]--;
      }
    }

    retval = 0;
  }
  else
  {
    retval = 1;
  }

  /* Destory P2L translation table. */
  PUGHi_DestroyP2LTranslation(table);
  
  free(pos);

#ifdef DEBUG_PUGH
  /* Print neighbours */
  printf("Neighbours (dim: %d)\n",dim);
  for (i=0;i<total_procs;i++)
  {
    printf("Proc %d: ",i);
    for (idim=0;idim<2*dim;idim++)
    {
      printf(" %d",neighbours[i][idim]);
    }
    printf("\n");
  }
#endif

  return retval;
}



 /*@@
   @routine    PUGH_DecomposeIJK
   @date       Fri Nov  5 11:29:43 1999
   @author     Tom Goodale
   @desc
   Decompose an ijk index into seperate components.
   Taken from libHLL.
   @enddesc
@@*/
static int PUGH_DecomposeIJK(int dim, int ijk, int *nprocs, int *pos)
{
  int idim;

  /* Locate position in coordinate space.
   *
   * e.g. ijk = i+nx*(j+ny*k)
   * => i = ijk % nx
   *    ijk/nx = j + ny*k
   */

  for(idim = 0; idim < dim; idim++)
  {
    pos[idim] = ijk % nprocs[idim];
    ijk /= nprocs[idim];
  }

  return 0;
}


 /*@@
   @routine    PUGH_ComposeIJK
   @date       Fri Nov  5 11:29:43 1999
   @author     Tom Goodale
   @desc
   Compose an ijk index from seperate components.
   Taken from libHLL.
   @enddesc
@@*/
static int PUGH_ComposeIJK(int dim, int *nprocs, int *pos)
{
  int ijk;
  int idim;

  /* Construct the position in the linear space.
   *
   * e.g. i+nx*(j+ny*k)
   */

  ijk = 0;
  for(idim = dim-1; idim >=0; idim--)
  {
    ijk = pos[idim] + nprocs[idim]*ijk;
  }

  return ijk;
}


 /*@@
   @routine    PUGH_SetupPGExtrasMemory
   @date       Mon Nov  8 08:16:02 1999
   @author     Tom Goodale
   @desc
   Allocate memory for the members of the pGExtras structure.
   @enddesc
@@*/
static int PUGH_SetupPGExtrasMemory(int dim, int total_procs, pGExtras *this)
{
  int retcode;
  int j,k;

  retcode = 0;

  if(this)
  {
    /* Do it in stages.
     * First: things depending on the number of processors
     */
    this->lb       = malloc(total_procs*sizeof(int *));
    this->ub       = malloc(total_procs*sizeof(int *));
    this->rnsize   = malloc(total_procs*sizeof(int *));
    this->rnpoints = malloc(total_procs*sizeof(int));

    /* Things just depending on dimension */
    this->nghostzones  = malloc(dim*sizeof(int));
    this->nsize        = malloc(dim*sizeof(int));
    this->lnsize       = malloc(dim*sizeof(int));
    this->bbox         = malloc(2*dim*sizeof(int));
    this->iterator     = malloc(dim*sizeof(int));
    this->hyper_volume = malloc(dim*sizeof(int));

    /* Check all the above succeeded and then get memory for
     * arrays hanging off the above.
     */
    if(this->lb          &&
       this->ub          &&
       this->rnsize      &&
       this->rnpoints    &&
       (dim == 0 || this->nghostzones ) &&
       (dim == 0 || this->nsize       ) &&
       (dim == 0 || this->lnsize      ) &&
       (dim == 0 || this->bbox        ) &&
       (dim == 0 || this->iterator    ) &&
       (dim == 0 || this->hyper_volume))
    {
      this->lb[0]     = malloc(total_procs  *dim*sizeof(int));
      this->ub[0]     = malloc(total_procs  *dim*sizeof(int));
      this->rnsize[0] = malloc(total_procs*2*dim*sizeof(int));

      if (dim > 0)
      {
        if(this->lb[0]     &&
           this->ub[0]     &&
           this->rnsize[0])
        {
          int i;
          for (i = 1; i < total_procs; i++)
          {
            this->lb[i]     = this->lb[0]     + i*dim;
            this->ub[i]     = this->ub[0]     + i*dim;
            this->rnsize[i] = this->rnsize[0] + i*2*dim;
          }
        }
        else
        {
          /* Free inner arrays */
          free(this->lb[0]);
          this->lb[0] = NULL;
          free(this->ub[0]);
          this->ub[0] = NULL;
          free(this->rnsize[0]);
          this->rnsize[0] = NULL;

          /* Free toplevel arrays */
          free(this->lb);
          this->lb = NULL;
          free(this->ub);
          this->ub = NULL;
          free(this->rnsize);
          this->rnsize = NULL;
          free(this->rnpoints);
          this->rnpoints = NULL;
          free(this->nghostzones);
          this->nghostzones = NULL;
          free(this->nsize);
          this->nsize = NULL;
          free(this->lnsize);
          this->lnsize = NULL;
          free(this->bbox);
          this->bbox = NULL;
          free(this->iterator);
          this->iterator = NULL;
          free(this->hyper_volume);
          this->hyper_volume = NULL;
        }
      }
    }
    else
    {
      /* Free toplevel arrays */
      free(this->lb);
      this->lb = NULL;
      free(this->ub);
      this->ub = NULL;
      free(this->rnsize);
      this->rnsize = NULL;
      free(this->rnpoints);
      this->rnpoints = NULL;
      free(this->nghostzones);
      this->nghostzones = NULL;
      free(this->nsize);
      this->nsize = NULL;
      free(this->lnsize);
      this->lnsize = NULL;
      free(this->bbox);
      this->bbox = NULL;
      free(this->iterator);
      this->iterator = NULL;
      free(this->hyper_volume);
      this->hyper_volume = NULL;
    }

    if(this->lb          &&
       this->ub          &&
       this->rnsize      &&
       this->rnpoints    &&
       (dim == 0 || this->nghostzones ) &&
       (dim == 0 || this->nsize       ) &&
       (dim == 0 || this->lnsize      ) &&
       (dim == 0 || this->bbox        ) &&
       (dim == 0 || this->iterator    ) &&
       (dim == 0 || this->hyper_volume))
    {
      retcode = 0;
      for (j = 0; j < 2; j++)
      {
        this->ownership[j]    = malloc(dim*sizeof(int));
        this->ghosts[j]       = malloc(2*dim*sizeof(int *));
        this->overlap[j]      = malloc(2*dim*sizeof(int *));
        if(dim > 0)
        {
          if(this->ghosts[j] &&
             this->overlap[j])
          {
            this->ghosts[j][0]  = malloc(2*dim*dim*sizeof(int));
            this->overlap[j][0] = malloc(2*dim*dim*sizeof(int));
            for (k=1; k < 2*dim; k++)
            {
              this->ghosts[j][k]  = this->ghosts[j][0] + k*dim;
              this->overlap[j][k] = this->overlap[j][0] + k*dim;
            }
          }
          else
          {
            free(this->ownership[j]);
            this->ownership[j] = NULL;

            free(this->ghosts[j]);
            this->ghosts[j] = NULL;

            free(this->overlap[j]);
            this->overlap[j] = NULL;
            retcode = 1;
            break;
          }
        }
        if(retcode)
        {
          for(j=1; j >=0 ; j--)
          {
            free(this->ownership[j]);
            this->ownership[j] = NULL;

            free(this->ghosts[j]);
            this->ghosts[j] = NULL;

            free(this->overlap[j]);
            this->overlap[j] = NULL;
          }
          break;
        }
      }
      if(retcode)
      {
        /* Loop back through the arrays freeing things */
        for(j=1; j >=0 ; j--)
        {
          if(dim > 0)
          {
            free(this->ghosts[j][0]);
            free(this->overlap[j][0]);
          }

          free(this->ownership[j]);
          this->ownership[j] = NULL;

          free(this->ghosts[j]);
          this->ghosts[j] = NULL;

          free(this->overlap[j]);
          this->overlap[j] = NULL;
        }

        free(this->ownership);
        free(this->ghosts);
        free(this->overlap);

        /* Free the stuff originally allocated */

        free(this->lb[0]);
        this->lb[0] = NULL;
        free(this->ub[0]);
        this->ub[0] = NULL;
        free(this->rnsize[0]);
        this->rnsize[0] = NULL;

        free(this->lb);
        this->lb = NULL;
        free(this->ub);
        this->ub = NULL;
        free(this->rnsize);
        this->rnsize = NULL;
        free(this->rnpoints);
        this->rnpoints = NULL;
        free(this->nghostzones);
        this->nghostzones = NULL;
        free(this->nsize);
        this->nsize = NULL;
        free(this->lnsize);
        this->lnsize = NULL;
        free(this->bbox);
        this->bbox = NULL;
        free(this->iterator);
        this->iterator = NULL;
        free(this->hyper_volume);
        this->hyper_volume = NULL;
      }
    }
  }
  else
  {
    retcode = -1;
  }

  return retcode;
}

 /*@@
   @routine    PUGH_SetupPGExtrasSizes
   @date       Mon Nov  8 08:59:33 1999
   @author     Tom Goodale
   @desc
   Sets up the size information in the pGExtras
   structure.
   @enddesc
@@*/
static int PUGH_SetupPGExtrasSizes(int is_gf,
                                   int dim,
                                   int *sh,
                                   int *nghosts,
                                   int total_procs,
                                   int *nprocs,
                                   int this_proc,
                                   pGExtras *this)
{
  DECLARE_CCTK_PARAMETERS
  int dir;
  int maxpoints, minpoints, avgpoints,proc;

  /* Setup the global shape */
  for (dir=0 ; dir < dim ; dir++)
  {
    /* A -ve size means constant load per proc */
    if (sh[dir] < 0)
    {
      if (local_size_includes_ghosts)
      {
        this->nsize[dir] = (nprocs[dir]-2) *
          (-sh[dir] - 2*nghosts[dir]) +
          2 * (-sh[dir] - nghosts[dir]);
      }
      else
      {
        this->nsize[dir] = nprocs[dir] * (-sh[dir]);
      }
    }
    else
    {
      this->nsize[dir] = sh[dir];
    }
  }

  /* Set the number of ghost zones. */
  for (dir = 0; dir < dim; dir++)
  {
    this->nghostzones[dir] = nghosts[dir];
  }

  /* Setup the bounding box stuff */
  PUGH_SetupBoundingBox(is_gf, dim, nghosts, total_procs, nprocs,this);

  /* Set the remote sizes */
  PUGH_SetupRemoteSizes(dim, total_procs, this);

  /* Set the local sizes */
  for (dir = 0; dir < dim; dir++)
  {
    this->lnsize[dir] = this->rnsize[this_proc][dir];
  }

  this->npoints = this->rnpoints[this_proc];

  /* Set up the maxskew */
  minpoints = INT_MAX;
  maxpoints = avgpoints = 0;
  for (proc = 0; proc < total_procs; proc++)
  {
    if (minpoints > this->rnpoints[proc])
    {
      minpoints = this->rnpoints[proc];
    }
    if (maxpoints < this->rnpoints[proc])
    {
      maxpoints = this->rnpoints[proc];
    }
    avgpoints += this->rnpoints[proc];
  }
  avgpoints /= total_procs;

  this->maxskew = avgpoints > 0 ? 100*(maxpoints-minpoints)/avgpoints : 0;

  return 0;
}


 /*@@
   @routine    PUGH_SetupPGExtrasOwnership
   @date       Mon Nov  8 09:00:10 1999
   @author     Tom Goodale
   @desc
   Sets up ownership, overlap, ghostzones, etc
   in a pGExtras structure.

   Mostly taken from original SetupOwnership by Paul.
   @enddesc
@@*/
static int PUGH_SetupPGExtrasOwnership(int dim,
                                       int *perme,
                                       int this_proc,
                                       int nprocs,
                                       int **neighbours,
                                       pGExtras *this)
{
  int tmp;
  int dir, idir;
  int istart, iend;


  /* Ownership is pretty easy. Remember ownership is indexed as
     [ijk][min/max]. See pGF_Reduction for a use of this,
     among others.
   */
  for (dir = 0 ; dir < dim; dir++)
  {
    /* Setup ownership and bbox using neighbours. */
    if (neighbours[this_proc][2*dir] == -1
        || neighbours[this_proc][2*dir+1] == -1) {
      this->ownership[0][dir] =
        (this->lb[this_proc][dir] == 0 ? 0 : this->nghostzones[dir]);
      this->ownership[1][dir] =
        (this->ub[this_proc][dir] == this->nsize[dir]-1 ?
         this->lnsize[dir] : this->lnsize[dir] - this->nghostzones[dir]);
      this->bbox[2*dir+0] = this->lb[this_proc][dir] == 0;
      this->bbox[2*dir+1] = this->ub[this_proc][dir] == this->nsize[dir]-1;
    } else {
      this->ownership[0][dir] = this->nghostzones[dir];
      this->ownership[1][dir] =
        this->lnsize[dir] - this->nghostzones[dir];
      this->bbox[2*dir+0] = 0;
      this->bbox[2*dir+1] = 0;
    }

    /* Do sanity check for periodic BC */
    if(perme[dir])
    {
      const int is_single_proc_in_dim
	= (this->lb[this_proc][dir] == 0
	   && this->ub[this_proc][dir] == this->nsize[dir]-1);
      if (! is_single_proc_in_dim
          && this->ownership[1][dir] -
             this->ownership[0][dir] < this->nghostzones[dir])
      {
        CCTK_VWarn (0, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "Not enough interior grid points in dimension %d ! "
                    "Please set the number of global grid points (currently "
                    "%d) to something >= (nprocs+2) * the number of ghostzones "
                    "(currently (%d+2)*%d=%d), "
                    "or make sure that there is only one processor in this "
                    "dimension.",
                    dir, this->nsize[dir], nprocs, this->nghostzones[dir],
                    (nprocs+2) * this->nghostzones[dir]);
      }
    }
  }

  for (dir = 0; dir < dim; dir++)
  {
    if (this->nsize[dir] == 1)
    {
      this->ownership[0][dir] = 0;
      this->ownership[1][dir] = 1;
    }
  }


  /* Great now set up the overlap region. This is the region we own,
     but which is covered by our neighbors ghost. So this is what we
     want to SEND
   */
  for (dir = 0; dir < 2*dim; dir++)
  {
    for (idir = 0; idir < dim; idir++)
    {
      istart = 0;
      iend   = this->lnsize[idir];

      /* We want to send at the dir - 1 */

      /* Correct to direction specific sweeps ... */
      if (dir == 2*idir)
      {
        /* Want to go from sw to sw + sw (eg, 1 to < 2, 2 to < 4) */
        istart = this->nghostzones[idir];
        iend   = istart + this->nghostzones[idir];
      }
      if (dir == 2*idir+1) {
        /* Want to go from nx - 2 sw to nx - sw
           (eg nx-2 to < nx-1, nx-4 to < nx-2)
           Luckily iend is aready set to nx ... */
        tmp = iend;
        istart = tmp - 2 * this->nghostzones[idir];
        iend   = tmp -     this->nghostzones[idir];
      }

      this->overlap[0][dir][idir] = istart;
      this->overlap[1][dir][idir] = iend;
    }
  }

  /* And finally the ghost indices. This used to be in
     pGF_FinishRecv.c, but now it is here.
   */
  for (dir = 0; dir < 2*dim; dir++)
  {
    /* We want to send at the dir - 1 */
    for (idir = 0; idir < dim; idir++)
    {
      istart = 0;
      iend = this->lnsize[idir];

      /* Correct to direction specific sweeps ... */
      /* Remember this is SW less than the other way... */
      if (dir == 2*idir)
      {
      /* Want to go from sw to sw + sw (eg, 1 to < 2, 2 to < 4) */
        istart = 0;
        iend   = this->nghostzones[idir];
      }
      if (dir == 2*idir+1)
      {
      /* Want to go from nx - 2 sw to nx - sw
         (eg nx-2 to < nx-1, nx-4 to < nx-2)
         Luckily iend is aready set to nx ... */
        tmp = iend;
        istart = tmp - this->nghostzones[idir];
        iend   = tmp;
      }

      this->ghosts[0][dir][idir] = istart;
      this->ghosts[1][dir][idir] = iend;
    }
  }

  return 0;
}

 /*@@
   @routine    PUGH_SetupBoundingBox
   @date       Mon Nov  8 09:03:40 1999
   @author     Tom Goodale
   @desc
   Sets up the bounding box info for a pgExtras structure.
   @enddesc
@@*/
static int PUGH_SetupBoundingBox(int is_gf,
                                 int dim,
                                 int *nghosts,
                                 int total_procs,
                                 int *nprocs,
                                 pGExtras *this)
{
  int pnum,dir;

  int **bounds;
  int *pos;

  bounds = malloc(dim*sizeof(int*));
  pos  = malloc(dim*sizeof(int));

  if(dim > 0 && bounds && pos)
  {
    /* Work out the bounds in each direction - either from parameters
       file or default*/
    PUGH_GetBounds(is_gf, dim, bounds, nprocs, this->nsize);

    /*    for (dir = 0 ; dir < dim; dir++)
    {
      step[dir] = (this->nsize[dir]-1) / nprocs[dir];
    }
    */

    for(pnum = 0; pnum < total_procs; pnum++)
    {

      PUGH_DecomposeIJK(dim, pnum, nprocs, pos);

      for(dir = 0 ; dir < dim; dir++)
      {
        if (pos[dir] == 0)
        {
          this->lb[pnum][dir] = 0;
        }
        else
        {
          this->lb[pnum][dir] = bounds[dir][pos[dir]] +1 - nghosts[dir];
        }

        if (pos[dir] == nprocs[dir]-1)
        {
          this->ub[pnum][dir] = this->nsize[dir]-1;
        }
        else
        {
          this->ub[pnum][dir] = bounds[dir][pos[dir]+1] + this->nghostzones[dir];
        }
      }
    }

  }

  for (dir=0; dir<dim;dir++)
  {
    free(bounds[dir]);
  }
  free(bounds);
  free(pos);

#ifdef DEBUG_PUGH
  {
    int i;
    for(i=0; i<total_procs; i++) {
      printf(" setup_Bounding_Box (%d):", i);
      for (dir = 0 ; dir < dim; dir++)
        printf(" (%d,%d)",
               this->lb[i][dir], this->ub[i][dir]);
      printf(" \n");
    }
  }
#endif

  return 0;
}


 /*@@
   @routine    PUGH_SetupRemoteSizes
   @date       Mon Nov  8 09:07:27 1999
   @author     Tom Goodale
   @desc
   Determines info about the sizes on each processor.
   @enddesc
@@*/
static int PUGH_SetupRemoteSizes(int dim, int total_procs, pGExtras *this)
{
  int pnum;
  int dir;

  /* Save the number of points on each processor */

  for(pnum = 0; pnum < total_procs; pnum++)
  {

    this->rnpoints[pnum] = 1;
    for (dir=0;dir<dim;dir++)
    {
      this->rnsize[pnum][dir] = (this->ub[pnum][dir]-this->lb[pnum][dir]+1);
      this->rnpoints[pnum]  *=  this->rnsize[pnum][dir];
    }
  }

  return 0;
}


 /*@@
   @routine    PUGH_SetupGArrayGroupComm
   @date       Tue 06 Jun 2000
   @author     Thomas Radke
   @desc
   Sets up a communication buffer for a group of GAs.
   @enddesc
@@*/
pComm *PUGH_SetupGArrayGroupComm(pGH *pughGH,
                                 int dim,
                                 int first_var,
                                 int n_vars,
                                 int vartype,
                                 int sync_timelevel,
                                 pGExtras *extras)
{
  int i;
  pComm *this;


#ifndef CCTK_MPI
  (void) (pughGH + 0);
  (void) (vartype + 0);
#endif

  this = calloc (1, sizeof (pComm));

  if (this)
  {
    this->buffer_sz   = malloc (2 * dim * sizeof (int));
    this->send_buffer = malloc (2 * dim * sizeof (void *));
    this->recv_buffer = malloc (2 * dim * sizeof (void *));
#ifdef CCTK_MPI
    this->sreq    = malloc (2 * dim * sizeof (MPI_Request));
    this->rreq    = malloc (2 * dim * sizeof (MPI_Request));
    this->sstatus = malloc (2 * dim * sizeof (MPI_Status));
#endif
    this->docomm      = malloc (2 * dim * sizeof (int));

    if((dim==0 || this->buffer_sz)   &&
       (dim==0 || this->send_buffer) &&
       (dim==0 || this->recv_buffer) &&
#ifdef CCTK_MPI
       (dim==0 || this->sreq)        &&
       (dim==0 || this->rreq)        &&
       (dim==0 || this->sstatus)     &&
#endif
       (dim==0 || this->docomm))
    {
      this->commflag       = PUGH_NOCOMM;
      this->first_var      = first_var;
      this->n_vars         = n_vars;
      this->sync_timelevel = sync_timelevel;

#ifdef CCTK_MPI
      this->mpi_type       = PUGH_MPIDataType (pughGH, vartype);
#endif

      for (i = 0; i < 2 * dim; i++)
      {
        this->buffer_sz[i]   = 0;
        this->send_buffer[i] = NULL;
        this->recv_buffer[i] = NULL;
#ifdef CCTK_MPI
        /* Null my send and receive requests */
        this->sreq[i]        = MPI_REQUEST_NULL;
        this->rreq[i]        = MPI_REQUEST_NULL;
#endif
        this->docomm[i]      = PUGH_NOCOMM;
      }
    }
    else
    {
      free (this->buffer_sz);
      free (this->send_buffer);
      free (this->recv_buffer);
#ifdef CCTK_MPI
      free (this->sreq);
      free (this->rreq);
      free (this->sstatus);
#endif
      free (this->docomm);

      free (this);
      this = NULL;
    }
  }

  /* Now set the hyper_volume vector in the extras structure
     which is used to copy the ghostzones from/to the comm buffers.
     NOTE: Sizes are counted in 'number of elements' here.
           hyper_volume[0] is unused.
   */
  if(this)
  {
    if (extras->dim > 0)
    {
      extras->hyper_volume[0] = 1;
      for (i = 1; i < extras->dim; i++)
      {
        extras->hyper_volume[i] = extras->hyper_volume[i-1] * extras->lnsize[i-1];
      }
    }
  }

  return this;
}


 /*@@
   @routine    PUGH_SetupGArrayComm
   @date       Tue 06 Jun 2000
   @author     Thomas Radke
   @desc
   Sets up a communication buffer for a GA.
   @enddesc
@@*/
pComm *PUGH_SetupGArrayComm(pGH *pughGH,
                            int dim,
                            int var,
                            int sync_timelevel,
                            int vartype,
                            pGExtras *extras)
{
  pComm *result;


  result = PUGH_SetupGArrayGroupComm(pughGH, dim, var, 1, vartype,
                                     sync_timelevel, extras);

  return (result);
}


 /*@@
   @routine    PUGH_DestroyComm
   @date       Tue 06 Jun 2000
   @author     Thomas Radke
   @desc
   Destroys a communication buffer.
   @enddesc
@@*/
void PUGH_DestroyComm(pComm **comm)
{
  free ((*comm)->buffer_sz);
  free ((*comm)->send_buffer);
  free ((*comm)->recv_buffer);
#ifdef CCTK_MPI
  free ((*comm)->sreq);
  free ((*comm)->rreq);
  free ((*comm)->sstatus);
#endif
  free ((*comm)->docomm);
  free (*comm);

  *comm = NULL;
}


 /*@@
   @routine    PUGH_SetupGArray
   @date       Mon Nov  8 16:29:34 1999
   @author     Tom Goodale
   @desc
   Sets up a new pGA.
   @enddesc
@@*/
pGA *PUGH_SetupGArray(void *parent,
                      pGExtras *extras,
                      pConnectivity *connectivity,
                      pComm *groupcomm,
                      const char *name,
                      int id,
                      int arrayid,
                      int varsize,
                      int vtype,
                      int vector_size,
                      int vector_entry,
                      pGA *vector_base)
{
  pGA *this;


  this = calloc(1, sizeof(pGA));
  if(this)
  {
    this->extras       = extras;
    this->connectivity = connectivity;
    this->comm         = NULL;
    this->groupcomm    = groupcomm;
    this->parent       = parent;
    this->varsize      = varsize;
    this->vtype        = vtype;
    this->id           = id;
    this->arrayid      = arrayid;
    this->storage      = PUGH_NOSTORAGE;

    this->name        = malloc((strlen(name)+1)*sizeof(char));
    this->padddata    = NULL;
    this->data        = this->padddata;

    this->vector_size  = vector_size;
    this->vector_entry = vector_entry;
    this->vector_base  = vector_base;

    if(this->name) /* && this->padddata) */
    {
      strcpy(this->name, name);
    }
    else
    {
      free(this->name);
      free(this->padddata);
      free(this);
      this = NULL;
    }
  }

  return this;
}

 /*@@
   @routine    PUGH_DestroyGArray
   @date       Mar 12 2000
   @author     Thomas Radke
   @desc
   Destroys a pGA object.
   @enddesc
@@*/
void PUGH_DestroyGArray(pGA **GA)
{
  if(GA && *GA)
  {
    /* free storage if still allocated */
    if((*GA)->storage != PUGH_NOSTORAGE)
    {
      PUGH_DisableGArrayDataStorage(*GA);
    }

    /* free comm buffer if created before */
    if((*GA)->comm)
    {
      if ((*GA)->comm->commflag != PUGH_NOCOMM)
      {
        PUGH_DisableGArrayComm(*GA);
      }
      PUGH_DestroyComm(&(*GA)->comm);
    }

    /* free allocated resources */
    free((*GA)->name);
    free((*GA)->padddata);
    free(*GA);
    *GA = NULL;
  }
}

/********************************************************************
 *********************     Local Routines   *************************
 ********************************************************************/
