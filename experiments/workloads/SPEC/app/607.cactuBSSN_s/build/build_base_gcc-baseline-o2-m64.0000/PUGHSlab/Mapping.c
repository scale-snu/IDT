 /*@@
   @file      Mapping.c
   @date      Sun 2 Dec 2001
   @author    Thomas Radke
   @desc
              Routines to define hyperslab mappings
   @enddesc
   @version   $Id: Mapping.c 156 2012-07-04 01:29:29Z eschnett $
 @@*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SPEC 
#include "PUGHSlab/cctk.h" 
#define THORN_DIR "PUGHSlab" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "util_Table.h"

#include "CactusPUGH/PUGH/src/include/pugh.h"

#include "PUGHSlab.h"
#include "PUGHSlabi.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGHSlab_Mapping_c)

/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* define this if you want debugging output */
/* #define DEBUG 1 */

/* macros for getting the minimum/maximum value */
#ifdef MIN
#undef MIN
#endif
#define MIN(x, y)             ((x) < (y) ? (x) : (y))
#ifdef MAX
#undef MAX
#endif
#define MAX(x, y)             ((x) > (y) ? (x) : (y))


/********************************************************************
 ********************    Static Variables    ************************
 ********************************************************************/
static int nmapping_list = 0;
static hslab_mapping_t *mapping_list = NULL;


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static CCTK_INT DefineMapping (const cGH      *GH,
                               CCTK_INT        vindex,
                               CCTK_INT        hdim,
                               const CCTK_INT *direction,
                               const CCTK_INT *origin,
                               const CCTK_INT *extent,
                               const CCTK_INT *downsample,
                               CCTK_INT        table_handle,
                               t_hslabConversionFn function,
                               int             is_global_hyperslab,
                               CCTK_INT       *hsize_local,
                               CCTK_INT       *hsize_global,
                               CCTK_INT       *hoffset_global);
static int IsFullHyperslab (const pGA *GA,
                            const CCTK_INT *origin,
                            const CCTK_INT *extent,
                            const hslab_mapping_t *mapping);


CCTK_INT PUGHSlab_LocalMappingByIndex (CCTK_POINTER_TO_CONST _GH,
                                       CCTK_INT vindex,
                                       CCTK_INT hdim,
                                       const CCTK_INT *direction,
                                       const CCTK_INT *origin,
                                       const CCTK_INT *extent,
                                       const CCTK_INT *downsample,
                                       CCTK_INT table_handle,
                                       t_hslabConversionFn function,
                                       CCTK_INT *hsize_local,
                                       CCTK_INT *hsize_global,
                                       CCTK_INT *hoffset_global)
{
  CCTK_INT retval;
  const cGH *GH = _GH;


  retval = DefineMapping (GH, vindex, hdim, direction, origin, extent,
                          downsample, table_handle, function, 0,
                          hsize_local, hsize_global, hoffset_global);
  return (retval);
}


CCTK_INT PUGHSlab_GlobalMappingByIndex (CCTK_POINTER_TO_CONST _GH,
                                        CCTK_INT vindex,
                                        CCTK_INT hdim,
                                        const CCTK_INT *direction,
                                        const CCTK_INT *origin,
                                        const CCTK_INT *extent,
                                        const CCTK_INT *downsample,
                                        CCTK_INT table_handle,
                                        t_hslabConversionFn function,
                                        CCTK_INT *hsize)
{
  CCTK_INT retval;
  const cGH *GH = _GH;


  retval = DefineMapping (GH, vindex, hdim, direction, origin, extent,
                          downsample, table_handle, function, 1,
                          NULL, hsize, NULL);
  return (retval);
}


static CCTK_INT DefineMapping (const cGH      *GH,
                               CCTK_INT        vindex,
                               CCTK_INT        hdim,
                               const CCTK_INT *direction,     /* vdim*hdim */
                               const CCTK_INT *origin,        /* vdim */
                               const CCTK_INT *extent,        /* hdim */
                               const CCTK_INT *downsample,    /* hdim */
                               CCTK_INT        table,
                               t_hslabConversionFn function,
                               int             is_global_hyperslab,
                               CCTK_INT       *hsize_local,   /* hdim */
                               CCTK_INT       *hsize_global,  /* hdim */
                               CCTK_INT       *hoffset_global /* hdim */)
{
  int vdim, dim, num_dirs, iterator;
  int retval;
  int myproc, nprocs;
  int i, j, k, npoints;
  int my_local_startpoint, my_local_endpoint;
  int my_global_startpoint, my_global_endpoint;
  hslab_mapping_t *mapping;
  const char *error_msg;
  const pGH *pughGH;              /* pointer to the current pGH */
  const pGA *GA;                  /* the variable's GA structure from PUGH */
  const pGExtras *extras;
  cGroup vinfo;
  char key[128];
  CCTK_INT type, nelems, with_ghostzones;
  CCTK_INT *real_extent;


  /* prevent compiler warnings */
  mapping = NULL;
  pughGH = NULL;

  /* identify myself */
  myproc = CCTK_MyProc (GH);
  nprocs = CCTK_nProcs (GH);

  /* parse the table information if there is any */
  with_ghostzones = 0;
  if (table >= 0)
  {
    /* warn about other options */
    iterator = Util_TableItCreate (table);
    for (iterator = Util_TableItCreate (table);
         Util_TableItQueryIsNonNull (iterator) > 0 &&
         Util_TableItQueryKeyValueInfo (iterator, sizeof (key),
                                        key, &type, &nelems) > 0;
         Util_TableItAdvance (iterator))
    {
      if (CCTK_Equals (key, "with_ghostzones"))
      {
        if (type == CCTK_VARIABLE_INT && nelems == 1)
        {
          Util_TableGetInt (table, &with_ghostzones, "with_ghostzones");
        }
        else
        {
          CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                      "Invalid value for key 'with_ghostzones' in table "
                      "(must be an integer). Key/value pair will be ignored.");
        }
      }
      else
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "Found unrecognized key '%s' in table. Key/value pair will "
                    "be ignored.", key);
      }
    }
    Util_TableItDestroy (iterator);
  }

#ifdef DEBUG
  fprintf (stderr, "proc %d: mapping will %sinclude ghostzones\n",
           CCTK_MyProc (GH), with_ghostzones ? "" : "not ");
  CCTK_Barrier (GH);
#endif

  /* check parameter consistency */
  retval = 0;
  error_msg = NULL;
  if (CCTK_GroupData (CCTK_GroupIndexFromVarI (vindex), &vinfo) < 0)
  {
    error_msg = "invalid variable index given";
    retval = -1;
  }
  else if (vinfo.grouptype != CCTK_GF && vinfo.grouptype != CCTK_ARRAY)
  {
    error_msg = "invalid variable group type given "
                "(not a CCTK_GF or CCTK_ARRAY type)";
    retval = -2;
  }
  else if (hdim < 0 || hdim > vinfo.dim)
  {
    error_msg = "invalid hyperslab dimension given";
    retval = -2;
  }
  else if (! direction || ! origin || ! extent)
  {
    error_msg = "NULL pointer(s) passed for direction/origin/extent "
                "parameters";
    retval = -3;
  }
  else if ((pughGH = (const pGH *) PUGH_pGH (GH)) == NULL)
  {
    error_msg = "no PUGH GH extension registered (PUGH not activated ?)";
    retval = -4;
  }
  else
  {
    for (vdim = 0; vdim < vinfo.dim; vdim++)
    {
      retval |= origin[vdim] < 0;
      if (vdim < hdim)
      {
        retval |= extent[vdim] == 0;
        if (downsample)
        {
          retval |= downsample[vdim] <= 0;
        }
      }
    }
    if (retval)
    {
      error_msg = "invalid hyperslab origin/extent/downsample vectors given";
      retval = -5;
    }
  }
  if (! retval)
  {
    mapping = malloc (sizeof (hslab_mapping_t));
    if (mapping)
    {
      mapping->do_dir = malloc ((6*vinfo.dim + 2*nprocs) * sizeof(int));
      mapping->global_hsize = malloc (((2*hdim + 1) * (nprocs+1) + hdim) *
                                      sizeof (CCTK_INT));
    }
    if (! mapping || ! mapping->do_dir || ! mapping->global_hsize)
    {
      if (mapping->do_dir)
      {
        free (mapping->do_dir);
      }
      if (mapping->global_hsize)
      {
        free (mapping->global_hsize);
      }
      if (mapping)
      {
        free (mapping);
      }
      error_msg = "couldn't allocate hyperslab mapping structure";
      retval = -6;
    }
  }

  /* return in case of errors */
  if (retval)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "PUGHlab_GlobalMappingByIndex: %s", error_msg);
    return (retval);
  }

  mapping->hdim = hdim;
  mapping->vinfo = vinfo;
  mapping->conversion_fn = function;

  /* assign memory for the other vectors */
  mapping->local_startpoint  = mapping->do_dir + 1*vinfo.dim;
  mapping->local_endpoint    = mapping->do_dir + 2*vinfo.dim;
  mapping->global_startpoint = mapping->do_dir + 3*vinfo.dim;
  mapping->global_endpoint   = mapping->do_dir + 4*vinfo.dim;
  mapping->downsample        = mapping->do_dir + 5*vinfo.dim;
  mapping->displs            = mapping->do_dir + 6*vinfo.dim;
  mapping->recvcnts          = mapping->do_dir + 6*vinfo.dim + nprocs;

  mapping->global_hoffset    = mapping->global_hsize + 1*hdim;
  mapping->local_hsize       = mapping->global_hsize + 2*hdim;
  mapping->chunk             = mapping->global_hsize + 3*hdim + 1;

  /* check direction vectors */
  for (dim = 0; dim < mapping->hdim; dim++)
  {
    num_dirs = 0;
    for (vdim = 0; vdim < vinfo.dim; vdim++)
    {
      if (direction[dim*vinfo.dim + vdim])
      {
        num_dirs++;
      }
    }
    if (num_dirs == 0)
    {
      free (mapping->do_dir); free (mapping->global_hsize); free (mapping);
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGHlab_GlobalMappingByIndex: %d-direction vector "
                  "is a null vector", dim);
      return (-7);
    }

    mapping->is_diagonal_in_3D = num_dirs == 3 && mapping->hdim == 1;
    if (num_dirs != 1 && ! mapping->is_diagonal_in_3D)
    {
      free (mapping->do_dir); free (mapping->global_hsize); free (mapping);
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGHlab_GlobalMappingByIndex: %d-direction vector "
                  "isn't axis-orthogonal", dim);
      return (-7);
    }
  }

  for (vdim = 0; vdim < vinfo.dim; vdim++)
  {
    mapping->do_dir[vdim] = 0;
    for (dim = 0; dim < mapping->hdim; dim++)
    {
      if (direction[dim*vinfo.dim + vdim])
      {
        mapping->do_dir[vdim]++;
      }
    }
    if (mapping->do_dir[vdim] > 1)
    {
      free (mapping->do_dir); free (mapping->global_hsize); free (mapping);
      CCTK_WARN (1, "PUGHlab_GlobalMappingByIndex: duplicate direction "
                    "vectors given");
      return (-8);
    }
  }

  /* get the pGH pointer and the variable's GA structure */
  GA     = (const pGA *) pughGH->variables[vindex][0];
  extras = GA->extras;

  /* check extent */
  real_extent = malloc (vinfo.dim * sizeof (CCTK_INT));
  for (vdim = dim = 0; vdim < vinfo.dim; vdim++)
  {
    real_extent[vdim] = vdim < hdim ? extent[vdim] : -1;
    if (real_extent[vdim] < 0)
    {
      real_extent[vdim] = extras->nsize[vdim];
    }
    if (mapping->do_dir[vdim] && dim < mapping->hdim)
    {
      if (origin[vdim] + real_extent[dim] > extras->nsize[vdim])
      {
        free (mapping->do_dir); free (mapping->global_hsize); free (mapping);
        free (real_extent);
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "PUGHlab_GlobalMappingByIndex: extent in "
                    "%d-direction exceeds grid size", dim);
        return (-8);
      }
      dim++;
    }
    else if (mapping->is_diagonal_in_3D &&
             origin[vdim] + real_extent[0] > extras->nsize[vdim])
    {
      free (mapping->do_dir); free (mapping->global_hsize); free (mapping);
      free (real_extent);
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "PUGHlab_GlobalMappingByIndex: extent in "
                  "%d-direction exceeds grid size", vdim);
      return (-8);
    }
  }

  /* now fill out the hyperslab mapping structure */
  mapping->global_hsize[0] = 0;
  for (vdim = dim = 0; vdim < vinfo.dim; vdim++)
  {
    mapping->downsample[vdim] = 1;

    if (mapping->is_diagonal_in_3D)
    {
      mapping->totals =
        (real_extent[0] + mapping->downsample[0]-1) / mapping->downsample[0];

      /* subtract ghostzones for periodic BC */
      if (! with_ghostzones)
      {
        if (GA->connectivity->perme[vdim])
        {
          mapping->totals -= 2 * extras->nghostzones[vdim];
        }
        else if (GA->connectivity->perme[(vdim + 1) % 3] ||
                 GA->connectivity->perme[(vdim + 2) % 3])
        {
          mapping->totals -= extras->nghostzones[vdim];
        }
      }
      if (mapping->global_hsize[0] < mapping->totals)
      {
        mapping->global_hsize[0] = mapping->totals;
      }
    }
    else if (mapping->do_dir[vdim] && dim < mapping->hdim)
    {
      if (downsample)
      {
        mapping->downsample[vdim] = downsample[dim];
      }
      mapping->global_hsize[dim] =
        (real_extent[dim] + mapping->downsample[vdim]-1) / mapping->downsample[vdim];

      /* subtract ghostzones for periodic BC */
      if (! with_ghostzones && GA->connectivity->perme[vdim])
      {
        mapping->global_hsize[dim] -= 2 * extras->nghostzones[vdim];
      }
      dim++;
    }
  }

  /* check whether the full local data patch was requested as hyperslab */
  mapping->is_full_hyperslab = IsFullHyperslab (GA, origin, real_extent, mapping);
  if (mapping->is_full_hyperslab)
  {
    memset (mapping->local_startpoint, 0, vinfo.dim * sizeof (int));
    memcpy (mapping->local_endpoint, extras->lnsize, vinfo.dim * sizeof (int));
    mapping->totals = extras->npoints;
  }
  else if (mapping->is_diagonal_in_3D)
  {
    /* just initialize the downsample and global_startpoint vectors */
    for (vdim = 0; vdim < vinfo.dim; vdim++)
    {
      mapping->downsample[vdim] = mapping->downsample[0];
      mapping->global_startpoint[vdim] = origin[vdim];
    }

    mapping->totals = 0;
    i = mapping->global_startpoint[0] - extras->lb[myproc][0];
    j = mapping->global_startpoint[1] - extras->lb[myproc][1];
    k = mapping->global_startpoint[2] - extras->lb[myproc][2];
    while (i < extras->ownership[1][0] &&
           j < extras->ownership[1][1] &&
           k < extras->ownership[1][2])
    {
      if (i >= extras->ownership[0][0] &&
          j >= extras->ownership[0][1] &&
          k >= extras->ownership[0][2])
      {
        mapping->totals++;
      }
      i += mapping->downsample[0];
      j += mapping->downsample[1];
      k += mapping->downsample[2];
    }
    mapping->local_hsize[0] = mapping->totals;
  }
  else
  {
    /* compute the global endpoint */
    for (vdim = dim = 0; vdim < vinfo.dim; vdim++)
    {
      mapping->global_endpoint[vdim] =
        origin[vdim] + (mapping->do_dir[vdim] ? real_extent[dim++] : 1);
    }

    /* compute this processor's global startpoint from the global ranges */
    for (vdim = 0; vdim < vinfo.dim; vdim++)
    {
      my_local_startpoint = extras->ownership[0][vdim];
      my_local_endpoint   = extras->ownership[1][vdim];

      /* include ghostzones if explicitely requested,
         otherwise only for local hyperslabs */
      if (with_ghostzones || ! is_global_hyperslab)
      {
        my_local_startpoint -= extras->nghostzones[vdim];
        my_local_endpoint   += extras->nghostzones[vdim];
      }

      my_global_startpoint = extras->lb[myproc][vdim] + my_local_startpoint;
      my_global_endpoint   = extras->lb[myproc][vdim] + my_local_endpoint;

      mapping->global_startpoint[vdim] = -1;
      if (origin[vdim] < my_global_endpoint)
      {
        mapping->global_startpoint[vdim] = origin[vdim];
        if (origin[vdim] < my_global_startpoint)
        {
          npoints = (my_global_startpoint - origin[vdim] +
                     mapping->downsample[vdim]-1) / mapping->downsample[vdim];
          mapping->global_startpoint[vdim] += npoints*mapping->downsample[vdim];
        }
      }
    }

    /* compute the local start- and endpoint from the global ranges */
    mapping->totals = 1;
    for (vdim = dim = 0; vdim < vinfo.dim; vdim++)
    {
      /* only non-periodic local hyperslabs include ghostzones by default */
      my_local_startpoint = extras->ownership[0][vdim];
      my_local_endpoint   = extras->ownership[1][vdim];
      if (with_ghostzones ||
          ! (is_global_hyperslab || GA->connectivity->perme[vdim]))
      {
        my_local_startpoint -= extras->nghostzones[vdim];
        my_local_endpoint   += extras->nghostzones[vdim];
      }

      my_global_startpoint = extras->lb[myproc][vdim] + my_local_startpoint;
      my_global_endpoint   = extras->lb[myproc][vdim] + my_local_endpoint;

      mapping->local_startpoint[vdim] = -1;
      if (mapping->global_startpoint[vdim] >= 0 &&
          mapping->global_startpoint[vdim] <  my_global_endpoint)
      {
        mapping->local_startpoint[vdim] =
          MAX (my_local_startpoint,
               mapping->global_startpoint[vdim] - extras->lb[myproc][vdim]);
      }

      mapping->local_endpoint[vdim] = -1;
      if (mapping->global_endpoint[vdim] > my_global_startpoint)
      {
        mapping->local_endpoint[vdim] =
          MIN (my_local_endpoint,
               mapping->global_endpoint[vdim] - extras->lb[myproc][vdim]);
      }

#ifdef DEBUG
      fprintf (stderr, "direction %d: global ranges [%d, %d), "
               "local ranges [%d, %d)\n", vdim,
               mapping->global_startpoint[vdim], mapping->global_endpoint[vdim],
               mapping->local_startpoint[vdim], mapping->local_endpoint[vdim]);
#endif

      if (mapping->local_endpoint[vdim] < 0 ||
          mapping->local_startpoint[vdim] < 0)
      {
        mapping->totals = 0;
        mapping->local_endpoint[vdim] = mapping->local_startpoint[vdim];
      }

      if (mapping->do_dir[vdim])
      {
        /* compute the local size in each hyperslab dimension */
        mapping->local_hsize[dim] =
          (mapping->local_endpoint[vdim] - mapping->local_startpoint[vdim] +
           mapping->downsample[vdim]-1) / mapping->downsample[vdim];
        mapping->totals *= mapping->local_hsize[dim];
        dim++;
      }
    }
  } /* end of else branch for 'if (mapping->is_full_hyperslab)' */

  free (real_extent);

#ifdef DEBUG
  fprintf (stderr, "total number of hyperslab data points: %d\n",
           mapping->totals);
#endif

  /* calculate the offset of the local chunk into the global hyperslab */
  /* NOTE: the global offsets for diagonals are calculated later after
           gathering all the local sizes */
  if (! mapping->is_diagonal_in_3D && mapping->totals > 0)
  {
    for (vdim = dim = 0; vdim < vinfo.dim; vdim++)
    {
      if (mapping->do_dir[vdim])
      {
        mapping->global_hoffset[dim] = extras->lb[myproc][vdim];
        if (! mapping->is_full_hyperslab)
        {
          mapping->global_hoffset[dim] = (mapping->global_startpoint[vdim] -
                                          origin[vdim]) / mapping->downsample[vdim];
          if (! with_ghostzones && GA->connectivity->perme[vdim])
          {
            if (mapping->global_hoffset[dim] < extras->nghostzones[vdim])
            {
              mapping->global_hoffset[dim] = 0;
            }
            else
            {
              mapping->global_hoffset[dim] -= extras->nghostzones[vdim];
            }
          }
        }
#ifdef DEBUG
        fprintf (stderr, "proc %d: global_hoffset, local_hsize in direction "
                 "%d: %d, %d\n", CCTK_MyProc (GH), dim,
                 mapping->global_hoffset[dim], mapping->local_hsize[dim]);
#endif
        dim++;
      }
    }
  }

  mapping->totals_global = mapping->totals;
  mapping->is_global_hyperslab = is_global_hyperslab && nprocs > 1;

#ifdef CCTK_MPI
  /* for global hyperslabs: build the communication buffers to gather
     the sizes and offsets of processor chunks */
  if (mapping->is_global_hyperslab)
  {
    /* append the total size of the chunk to the local_hsize[] array */
    mapping->local_hsize[mapping->hdim] = mapping->totals;

    /* gather sizes and offsets of processor chunks */
    CACTUS_MPI_ERROR (MPI_Allgather (mapping->global_hoffset, 2*mapping->hdim+1,
                                     PUGH_MPI_INT, mapping->chunk,
                                     2*mapping->hdim+1, PUGH_MPI_INT,
                                     pughGH->PUGH_COMM_WORLD));

    /* set the displacements/receive counts for the data gather operation
       in the following PUGHSlab_Get() call;
       also sum up the total number of data points of the global hyperslab */
    mapping->totals_global = 0;
    for (i = 0; i < nprocs; i++)
    {
      mapping->displs[i] = i ? mapping->displs[i-1]+mapping->recvcnts[i-1] : 0;
      mapping->recvcnts[i] = mapping->chunk[(i+1)*2*mapping->hdim + i];
      mapping->totals_global += mapping->recvcnts[i];
    }
  }
#endif

  /* add this mapping to the mapping list */
  if (mapping_list)
  {
    mapping_list->prev = mapping;
  }
  mapping->prev = NULL;
  mapping->next = mapping_list;
  mapping_list = mapping;

  mapping->handle = nmapping_list++;

  /* set the return vectors if given */
  for (dim = 0; dim < mapping->hdim; dim++)
  {
    if (hoffset_global)
    {
      hoffset_global[dim] = mapping->global_hoffset[dim];
    }
    if (hsize_global)
    {
      hsize_global[dim] = mapping->global_hsize[dim];
    }
    if (hsize_local)
    {
      hsize_local[dim] = mapping->local_hsize[dim];
    }
  }

  return (mapping->handle);
}


CCTK_INT PUGHSlab_FreeMapping (CCTK_INT mapping_handle)
{
  hslab_mapping_t *mapping;


  mapping = mapping_list;
  while (mapping)
  {
    if (mapping->handle == mapping_handle)
    {
      if (mapping == mapping_list)
      {
        mapping_list = mapping_list->next;
      }
      else
      {
        if (mapping->next)
        {
          mapping->next->prev = mapping->prev;
        }
        if (mapping->prev)
        {
          mapping->prev->next = mapping->next;
        }
      }
      free (mapping->do_dir); free (mapping->global_hsize); free (mapping);
      return (0);
    }
    mapping = mapping->next;
  }

  return (-1);
}


hslab_mapping_t *PUGHSlabi_GetMapping (int mapping_handle)
{
  hslab_mapping_t *mapping;


  mapping = mapping_list;
  while (mapping && mapping->handle != mapping_handle)
  {
    mapping = mapping->next;
  }

  return (mapping);
}


static int IsFullHyperslab (const pGA *GA,
                            const CCTK_INT *origin,
                            const CCTK_INT *extent,
                            const hslab_mapping_t *mapping)
{
  int i, retval;


  retval = mapping->hdim == GA->extras->dim;
  if (retval)
  {
    for (i = 0; i < GA->extras->dim; i++)
    {
      retval &= (origin[i] == 0);
      retval &= (extent[i] <= 0);
      retval &= (mapping->downsample[i] <= 1);
      retval &= (GA->connectivity->perme[i] == 0);
    }
  }

  return (retval);
}
