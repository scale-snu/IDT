 /*@@
   @file      GetHyperslab.c
   @date      Sun 2 Dec 2001
   @author    Thomas Radke
   @desc
              Routines to extract hyperslabs from CCTK array variables
   @enddesc
   @version   $Id: GetHyperslab.c 156 2012-07-04 01:29:29Z eschnett $
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
#ifdef SPEC 
#include "PUGHSlab/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 

#include "CactusPUGH/PUGH/src/include/pugh.h"
#include "PUGHSlab.h"
#include "PUGHSlabi.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGHSlab_GetHyperslab_c)

/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* define this if you want debugging output */
/* #define DEBUG 1 */

/* define some defaults for optional parameters */
#define DEFAULT_PROCESSOR  0
#define DEFAULT_TIMELEVEL  0

/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static int GetLocalHyperslab (const cGH *GH,
                              const hslab_mapping_t *mapping,
                              int vindex,
                              int timelevel,
                              int hdatatype,
                              void *hdata);
static int GetDiagonalFromFrom3D (const cGH *GH,
                                  const hslab_mapping_t *mapping,
                                  int vindex,
                                  int timelevel,
                                  int hdatatype,
                                  t_hslabConversionFn conversion_fn,
                                  void *hdata);
#ifdef CCTK_MPI
static void SortIntoUnchunkedHyperslab (const hslab_mapping_t *mapping,
                                        int nprocs, int hdata_size,
                                        const void *chunked_hdata,
                                        void *unchunked_hdata);
#endif


CCTK_INT PUGHSlab_Get (CCTK_POINTER_TO_CONST _GH,
                       CCTK_INT   mapping_handle,
                       CCTK_INT   proc,
                       CCTK_INT   vindex,
                       CCTK_INT   timelevel,
                       CCTK_INT   hdatatype,
                       void      *hdata)
{
  CCTK_INT retval;
  const cGH *GH = _GH;


  retval = PUGHSlab_GetList (GH, mapping_handle, 1, &proc, &vindex, &timelevel,
                             hdatatype >= 0 ? &hdatatype : NULL, &hdata, NULL);

  return (retval == 1 ? 0 : -1);
}


CCTK_INT PUGHSlab_GetList (CCTK_POINTER_TO_CONST _GH,
                           CCTK_INT mapping_handle,
                           CCTK_INT num_arrays,
                           const CCTK_INT *procs        /* num_arrays */,
                           const CCTK_INT *vindices     /* num_arrays */,
                           const CCTK_INT *timelevels   /* num_arrays */,
                           const CCTK_INT *hdatatypes   /* num_arrays */,
                           const CCTK_POINTER *hdata    /* num_arrays */,
                           CCTK_INT *retvals            /* num_arrays */)
{
  int i, timelevel, hdatatype, hdata_size, retval;
  CCTK_INT result, *result_ptr;
  hslab_mapping_t *mapping;
  void *local_hdata;
  const cGH *GH = _GH;
#ifdef CCTK_MPI
  int myproc, nprocs, proc;
  MPI_Comm comm;
  void *chunked_hdata;
  MPI_Datatype mpi_vtype;
#endif


  retval = 0;
  if (num_arrays <= 0)
  {
    return (0);
  }

  mapping = PUGHSlabi_GetMapping (mapping_handle);
  if (mapping)
  {
    /* check mapping consistency */
    /*** FIXME ***/
    /* all vindices[] must match the given mapping */

    /* check if there's any data to extract */
    if (mapping->totals_global <= 0)
    {
      retval = num_arrays;
    }
  }
  else
  {
    retval = -1;
  }

  /* immediately return in case of errors or if there's nothing to do */
  if (retval)
  {
    /* set the retvals[] according to the return value */
    if (retvals)
    {
      for (i = 0; i < num_arrays; i++)
      {
        retvals[i] = retval > 0 ? 0 : retval;
      }
    }

    return (retval);
  }

#ifdef CCTK_MPI
  myproc = CCTK_MyProc (GH);
  nprocs = CCTK_nProcs (GH);
  comm = PUGH_pGH (GH)->PUGH_COMM_WORLD;
#else
  /* suppress compiler warnings about unused parameters */
  (void) (procs + 0);
#endif

  /* now loop over all hyperslabs */
  /*** FIXME: should optimize this loop away ***/
  for (i = 0; i < num_arrays; i++)
  {
    timelevel = timelevels ? timelevels[i] : DEFAULT_TIMELEVEL;
    hdatatype = hdatatypes ? hdatatypes[i] : CCTK_VarTypeI (vindices[i]);
    hdata_size = CCTK_VarTypeSize (hdatatype);
    result_ptr = retvals ? &retvals[i] : &result;

    if (mapping->totals <= 0)
    {
      local_hdata = NULL;
      *result_ptr = 0;
    }
    else
    {
      /* allocate temporary array to keep the local hyperslab data */
      local_hdata = mapping->is_global_hyperslab ?
                    malloc (mapping->totals * hdata_size) : hdata[i];

      /* get the processor-local hyperslab */
      *result_ptr = GetLocalHyperslab (GH, mapping, vindices[i], timelevel,
                                       hdatatype, local_hdata);
    }

    if (*result_ptr == 0)
    {
      retval++;
    }

    if (! mapping->is_global_hyperslab)
    {
      continue;
    }

#ifdef CCTK_MPI
    proc = procs ? procs[i] : DEFAULT_PROCESSOR;
    if (proc < 0 || proc == myproc)
    {
      /* allocate temporary array to receive the chunks from all processors */
      /* for the case of hdim == 1, the sorting is done by MPI */
      chunked_hdata = mapping->hdim > 1 ?
                      malloc (mapping->totals_global * hdata_size) : hdata[i];
    }
    else
    {
      chunked_hdata = NULL;
    }

    /* detect the MPI datatype to use */
    mpi_vtype = PUGH_MPIDataType (PUGH_pGH (GH), hdatatype);

    /* collect the hyperslab chunks from all processors */
    if (proc < 0)
    {
      CACTUS_MPI_ERROR (MPI_Allgatherv (local_hdata, mapping->totals, mpi_vtype,
                                        chunked_hdata, mapping->recvcnts,
                                        mapping->displs, mpi_vtype, comm));
    }
    else
    {
      CACTUS_MPI_ERROR (MPI_Gatherv (local_hdata, mapping->totals, mpi_vtype,
                                     chunked_hdata, mapping->recvcnts,
                                     mapping->displs, mpi_vtype, proc, comm));
    }

    /* free processor-local chunk */
    if (local_hdata)
    {
      free (local_hdata);
    }

    /* Now we got the global hyperslab data in a chunked array.
       The user wants it unchunked, so let's sort it... */
    if (mapping->hdim > 1 && (proc < 0 || proc == myproc))
    {
      SortIntoUnchunkedHyperslab (mapping, nprocs, hdata_size, chunked_hdata,
                                  hdata[i]);
      free (chunked_hdata);
    }
#endif
  }

  return (retval);
}

/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
/*@@
  @routine    GetLocalHyperslab
  @date       Fri May 12 2000
  @author     Thomas Radke
  @desc
              Extract a hyperslab from the processor-local chunk
              of a domain-decomposed Cactus array variable.

              This routine delivers the local hyperslab data
              to be collected into a global hyperslab by PUGHlab_GetHyperslab().
              IO methods can call this routine as well to collect the
              local hyperslab data and output it in parallel.
  @enddesc

  @calls      PUGHSlab_GetDatatypeConversionFn

  @var        GH
  @vdesc      Pointer to CCTK grid hierarchy
  @vtype      const cGH *
  @vio        in
  @endvar
  @var        mapping
  @vdesc      pointer to the mapping structure for this hyperslab request
  @vtype      const hslab_mapping_t *
  @vio        in
  @endvar
  @var        vindex
  @vdesc      index of variable to get a hyperslab from
  @vtype      int
  @vio        in
  @endvar
  @var        timelevel
  @vdesc      timelvl of variable to get a hyperslab from
  @vtype      int
  @vio        in
  @endvar
  @var        hdatatype
  @vdesc      CCTK datatype of the requested hyperslab
  @vtype      int
  @vio        in
  @endvar
  @var        hdata
  @vdesc      reference to the user-supplied hyperslab data buffer
  @vtype      void *
  @vio        out
  @endvar
@@*/
static int GetLocalHyperslab (const cGH *GH,
                              const hslab_mapping_t *mapping,
                              int vindex,
                              int timelevel,
                              int hdatatype,
                              void *hdata)
{
  int *point;                    /* looper over hyperslab dimensions */
  int *startpoint,               /* hyperslab's local start and endpoint */
      *endpoint;                 /* within the variable's grid dimensions */
  int *downsample;               /* the downsample[] vector extended to vdim */
  int *points_per_dim;           /* points per subvolume */
  int i;                         /* general looper */
  int vdim;                      /* looper over all source dimensions */
  int vdata_size,                /* size of one data point in bytes for */
      hdata_size;                /* source and hyperslab data */
  int dim0_points;               /* number of hyperslab points in dim 0 */
  int dim0_hsize;                /* byte size of hyperslab points in dim 0 */
  const char *typed_vdata;             /* byte pointers into source and */
  char *typed_hdata;             /* hyperslab data arrays */
  const void *vdata;
  int retval;                    /* the return value (0 for success) */
  cGroup vinfo;                  /* variable's group info */
  pGH *pughGH;                   /* pointer to the current pGH */
  pGA *GA;                       /* the variable's GA structure from PUGH */
  const char *errormsg;          /* error message string */
  t_hslabConversionFn conversion_fn;


  vdata = NULL;

  /* do some plausibility checks */
  errormsg = NULL;
  if (! GH || ! mapping || (mapping->totals > 0 && ! hdata))
  {
    errormsg = "NULL pointer(s) passed for GH/mapping/hdata arguments";
  }
  else if (CCTK_GroupData (CCTK_GroupIndexFromVarI (vindex), &vinfo) < 0)
  {
    errormsg = "Invalid variable index given";
  }
  else if (timelevel < 0 || timelevel >= vinfo.numtimelevels)
  {
    errormsg = "Invalid timelevel given";
  }
  else if (vinfo.grouptype != mapping->vinfo.grouptype ||
           vinfo.disttype  != mapping->vinfo.disttype ||
           vinfo.dim       != mapping->vinfo.dim)
  {
    errormsg = "Group data for template variable in mapping and given variable "
               "don't match";
  }
  else
  {
    vdata = CCTK_VarDataPtrI (GH, timelevel, vindex);
    if (vdata == NULL)
    {
      errormsg = "Variable has no storage allocated";
    }
  }

  /* immediately return in case of errors */
  if (errormsg)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "GetLocalHyperslab: %s", errormsg);
    return (-1);
  }

  /* if datatype conversion was requested
     get the appropriate predefined datatype conversion routine
     in case the user didn't supply one by her own */
  conversion_fn = mapping->conversion_fn;
  if (vinfo.vartype != hdatatype)
  {
    if (conversion_fn == NULL)
    {
      conversion_fn = PUGHSlabi_GetDatatypeConversionFn (vinfo.vartype,
                                                         hdatatype);
      if (! conversion_fn)
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "No predefined PUGHSlab routine available to convert "
                    "'%s' into '%s'", CCTK_VarTypeName (vinfo.vartype),
                    CCTK_VarTypeName (hdatatype));
        return (-1);
      }
    }
  }
  else if (conversion_fn)
  {
    CCTK_WARN (8, "Datatype conversion routine supplied but no datatype "
                  "conversion requested. Ignoring conversion routine...");
    conversion_fn = NULL;
  }

  /* diagonals from 3D variables are treated special */
  if (mapping->is_diagonal_in_3D)
  {
    retval = GetDiagonalFromFrom3D (GH, mapping, vindex, timelevel, hdatatype,
                                    conversion_fn, hdata);
    return (retval);
  }

  /* allocate the temporary arrays */
  point          = malloc (5 * vinfo.dim * sizeof (int));
  startpoint     = point + 1*vinfo.dim;
  endpoint       = point + 2*vinfo.dim;
  downsample     = point + 3*vinfo.dim;
  points_per_dim = point + 4*vinfo.dim;

  memcpy (startpoint, mapping->local_startpoint, vinfo.dim * sizeof (int));
  memcpy (endpoint, mapping->local_endpoint, vinfo.dim * sizeof (int));
  memcpy (downsample, mapping->downsample, vinfo.dim * sizeof (int));

  /* get the pGH pointer and the variable's GA structure */
  pughGH = PUGH_pGH (GH);
  GA     = (pGA *) pughGH->variables[vindex][timelevel];

  /* nested loop over vinfo.dim dimensions */
  /* NOTE: the following code assumes startpoint[vdim] < endpoint[vdim] */
  if (mapping->is_full_hyperslab && conversion_fn == NULL)
  {
    memcpy (hdata, vdata, mapping->totals * CCTK_VarTypeSize (vinfo.vartype));
  }
  else
  {
    /* get the byte size of a single data point
       in the variable and hyperslab data array */
    vdata_size = CCTK_VarTypeSize (vinfo.vartype);
    hdata_size = CCTK_VarTypeSize (hdatatype);

    typed_hdata = (char *) hdata;

    /* compute the points_per_dim[] vector */
    /* NOTE: this could be computed at startup and kept in a GH extension
             once we have one for thorn PUGHSlab */
    points_per_dim[0] = 1;
    for (vdim = 1; vdim < vinfo.dim; vdim++)
    {
      points_per_dim[vdim] = points_per_dim[vdim-1] *
                             GA->extras->lnsize[vdim-1];
    }

    /* get the number of hyperslab points in lowest dimension
       and their size in bytes */
    dim0_points = (endpoint[0] - startpoint[0]) / downsample[0];
    if ((endpoint[0] - startpoint[0]) % downsample[0])
    {
      dim0_points++;
    }
    dim0_hsize = dim0_points * hdata_size;

    /* transform the ranges into byte ranges */
    for (i = 0; i < vinfo.dim; i++)
    {
      startpoint[i] *= vdata_size;
      endpoint[i]   *= vdata_size;
      downsample[i] *= vdata_size;
    }

    /* initialize the index vector to the local startpoint */
    memcpy (point, startpoint, vinfo.dim * sizeof (point[0]));

    /* do the nested loops starting with the innermost */
    vdim = 1;
    while (1)
    {
      /* check for end of current loop */
      if (vinfo.dim > 1 && point[vdim] >= endpoint[vdim])
      {
        /* increment outermost loopers */
        for (vdim++; vdim < vinfo.dim; vdim++)
        {
          point[vdim] += downsample[vdim];
          if (point[vdim] < endpoint[vdim])
          {
            break;
          }
        }

        /* done if beyond outermost loop */
        if (vdim >= vinfo.dim)
        {
          break;
        }

        /* reset innermost loopers */
        for (vdim--; vdim > 0; vdim--)
        {
          point[vdim] = startpoint[vdim];
        }
        vdim = 1;
      }

      /* get the byte pointer into the source array */
      typed_vdata = (const char *) vdata + point[0];
      for (i = 1; i < vinfo.dim; i++)
      {
        typed_vdata += point[i] * points_per_dim[i];
      }

      /* copy the data in lowest dimension: if possible copy all data points
         in a row otherwise do it one by one */
      if (downsample[0] == vdata_size)
      {
        if (conversion_fn)
        {
          if (conversion_fn (dim0_points, 1, 1, vinfo.vartype, hdatatype,
                             typed_vdata, typed_hdata) != dim0_points)
          {
            CCTK_WARN (1, "datatype conversion routine failed");
          }
        }
        else
        {
          memcpy (typed_hdata, typed_vdata, dim0_hsize);
        }
      }
      else
      {
        if (conversion_fn)
        {
          if (conversion_fn (dim0_points,  mapping->downsample[0], 1,
                             vinfo.vartype, hdatatype, typed_vdata,
                             typed_hdata) != dim0_points)
          {
            CCTK_WARN (1, "datatype conversion routine failed");
          }
          typed_vdata += downsample[0] * dim0_points;
        }
        else
        {
          for (i = 0; i < dim0_hsize; i += hdata_size)
          {
            memcpy (typed_hdata + i, typed_vdata, vdata_size);
            typed_vdata += downsample[0];
          }
        }
      }
      typed_hdata += dim0_hsize;

      if (vinfo.dim > 1)
      {
        /* increment current looper */
        point[vdim] += downsample[vdim];
      }
      else
      {
        /* exit loop if hyperslab dim is only 1D */
        break;
      }

    } /* end of nested loops over all dimensions */
  } /* end of branch extracting the hyperslab data */

  /* free allocated temporary memory */
  free (point);

  return (0);
}


static int GetDiagonalFromFrom3D (const cGH *GH,
                                  const hslab_mapping_t *mapping,
                                  int vindex,
                                  int timelevel,
                                  int hdatatype,
                                  t_hslabConversionFn conversion_fn,
                                  void *hdata)
{
  int i, j, k, myproc, linear_idx;
  int vdatatype, hdata_size, vdata_size;
  const char *vdata;
  const pGExtras *extras;


  /* get the variable's GA structure */
  extras = ((const pGA *) PUGH_pGH (GH)->variables[vindex][timelevel])->extras;

  vdatatype = CCTK_VarTypeI (vindex);
  hdata_size = CCTK_VarTypeSize (hdatatype);
  vdata_size = CCTK_VarTypeSize (vdatatype);
  vdata = CCTK_VarDataPtrI (GH, timelevel, vindex);

  myproc = CCTK_MyProc (GH);

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
      linear_idx = i + j*extras->hyper_volume[1] + k*extras->hyper_volume[2];
      if (conversion_fn)
      {
        if (conversion_fn (1, 1, 1, vdatatype, hdatatype,
                           vdata + linear_idx*vdata_size, hdata) != 1)
        {
          CCTK_WARN (1, "datatype conversion routine failed");
        }
      }
      else
      {
        memcpy (hdata, vdata + linear_idx*vdata_size, hdata_size);
      }
      hdata = (char *) hdata + hdata_size;
    }
    i += mapping->downsample[0];
    j += mapping->downsample[1];
    k += mapping->downsample[2];
  }

  return (0);
}


#ifdef CCTK_MPI
static void SortIntoUnchunkedHyperslab (const hslab_mapping_t *mapping,
                                        int nprocs, int hdata_size,
                                        const void *chunked_hdata,
                                        void *unchunked_hdata)
{
  int i, j, proc, linear_hoffset;
  int *point, *points_per_hdim;
  char *copy_to;
  const char *copy_from;
  const CCTK_INT *hsize_chunk, *hoffset_chunk;


  /* allocate temporary vectors */
  point = malloc (2 * mapping->hdim * sizeof (int));
  points_per_hdim = point + mapping->hdim;

  points_per_hdim[0] = 1;
  for (i = 1; i < mapping->hdim; i++)
  {
    points_per_hdim[i] = points_per_hdim[i-1] * mapping->global_hsize[i-1];
  }

  /* use char pointers for easy incrementing when copying */
  copy_from = (const char *) chunked_hdata;
  copy_to   = (char *) unchunked_hdata;

  /* now copy the chunks from each processor into the global hyperslab */
  for (proc = 0; proc < nprocs; proc++)
  {
    /* get the pointers to processor i's chunk offset and size */
    hoffset_chunk = mapping->chunk + proc * (2*mapping->hdim+1);
    hsize_chunk = hoffset_chunk + mapping->hdim;

    /* skip processors which didn't contribute any data */
    if (hsize_chunk[mapping->hdim] <= 0)
    {
      continue;
    }

    /* set startpoint to zero */
    memset (point, 0, mapping->hdim * sizeof (int));

    i = 1;
    while (1)
    {
      /* check for end of current loop */
      if (point[i] >= hsize_chunk[i])
      {
        /* increment outermost loopers */
        for (i++; i < mapping->hdim; i++)
        {
          if (++point[i] < hsize_chunk[i])
          {
            break;
          }
        }

        /* done if beyond outermost loop */
        if (i >= mapping->hdim)
        {
          break;
        }

        /* reset innermost loopers */
        for (i--; i > 0; i--)
        {
          point[i] = 0;
        }
        i = 1;
      }

      /* get the linear offset */
      linear_hoffset = hoffset_chunk[0];
      for (j = 1; j < mapping->hdim; j++)
      {
        linear_hoffset += (hoffset_chunk[j] + point[j]) * points_per_hdim[j];
      }
      /* copy the line */
      memcpy (copy_to + linear_hoffset * hdata_size, copy_from,
              hsize_chunk[0] * hdata_size);
      copy_from += hsize_chunk[0] * hdata_size;

      /* increment current looper */
      point[i]++;

    } /* end of nested loops over all dimensions */
  } /* end of loop over all processors */

  /* free allocated temporary vectors */
  free (point);
}
#endif
