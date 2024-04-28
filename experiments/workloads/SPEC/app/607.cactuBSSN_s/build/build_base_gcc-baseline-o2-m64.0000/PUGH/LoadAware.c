 /*@@
   @file    LoadAware.c
   @date    Fri Feb 21 10:21:36 2000
   @author  Matei Ripeanu
   @desc

   @enddesc
   @version $Id: LoadAware.c 429 2004-02-05 22:55:16Z goodale $
 @@*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pugh.h"
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
#if defined(SPEC) && !defined(strdup)
#include "util_String.h"
#define strdup Util_Strdup
#endif
static const char *rcsid="$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGH_LoadAware_c)


/* prototypes of routines defined in this source file */
int PUGH_SetPartitionInfo (int dim, const char *partition_info[]);
static void PUGH_GetSliceSizes (int np,
                                int grid_points,
                                const char *slicesS,
                                int **slices,
                                int manual);

/* static pointers to strings which contain partition information if set */
static char *partition_1D[1] = {NULL};
static char *partition_2D[2] = {NULL, NULL};
static char *partition_3D[3] = {NULL, NULL, NULL};


 /*@@
   @routine    PUGH_SetPartitionInfo
   @date       Tue 13 Feb 2001
   @author     Thomas Radke
   @desc
               Set/reset the static partition info string variables.

               Note that using such static variables here instead of the
               partition_[123]d_[xyz] parameters is just a hack because
               we don't want to make those steerable (this could make
               people think they can change the processor topology at
               runtime).
               But eg. BAM wants to set up a new topology when creating
               new grids.
   @enddesc

   @var        dim
   @vdesc      number of dimensions of processor topology
   @vtype      int
   @vio        in
   @endvar
   @var        partition_info
   @vdesc      list of strings with partition information
   @vtype      const char *[dim]
   @vio        in
   @endvar

   @returntype int
   @returndesc
                0: OK
               -1: invalid dimension
   @endreturndesc
@@*/
int PUGH_SetPartitionInfo (int dim, const char *partition_info[])
{
  int retval;


  retval = 0;
  if (dim == 1)
  {
    if (partition_1D[0])
    {
      free (partition_1D[0]);
    }
    partition_1D[0] = strdup (partition_info[0]);
  }
  else if (dim == 2)
  {
    if (partition_2D[0])
    {
      free (partition_2D[0]);
    }
    if (partition_2D[1])
    {
      free (partition_2D[1]);
    }
    partition_2D[0] = strdup (partition_info[0]);
    partition_2D[1] = strdup (partition_info[1]);
  }
  else if (dim == 3)
  {
    if (partition_3D[0])
    {
      free (partition_3D[0]);
    }
    if (partition_3D[1])
    {
      free (partition_3D[1]);
    }
    if (partition_3D[2])
    {
      free (partition_3D[2]);
    }
    partition_3D[0] = strdup (partition_info[0]);
    partition_3D[1] = strdup (partition_info[1]);
    partition_3D[2] = strdup (partition_info[2]);
  }
  else
  {
    CCTK_WARN (1, "Only 1D, 2D, and 3D supported");
    retval = -1;
  }

  return (retval);
}


 /*@@
   @routine    PUGH_GetBounds
   @date       Fri Feb 21 10:21:36 2000
   @author     Matei Ripeanu
   @desc

   @enddesc
   @calls      PUGH_GetSliceSizes

   @var        is_gf
   @vdesc      flag indicating that bounds are to be set up for a grid function
   @vtype      int
   @vio        in
   @endvar
   @var        dim
   @vdesc      dimension to set up bounds for
   @vtype      int
   @vio        in
   @endvar
   @var        bounds
   @vdesc      bounds to be allocated and set up
   @vtype      *int [dim-1]
   @vio        out
   @endvar
   @var        dim
   @vdesc      processors in every direction
   @vtype      int [dim-1]
   @vio        in
   @endvar
   @var        nsize
   @vdesc      points in every direction
   @vtype      int [dim-1]
   @vio        in
   @endvar

   @returntype int
   @returndesc
                0: OK
               -1: invalid dimension
   @endreturndesc
@@*/
int PUGH_GetBounds (int is_gf, int dim, int *bounds[], int nprocs[], int nsize[])
{
  DECLARE_CCTK_PARAMETERS
  int i, retval, manual, external_manual;
  const char *partition_info;


  retval = 0;
  manual = CCTK_Equals (partition, "manual");

  if (dim == 1)
  {
    external_manual = partition_1D[0] != NULL;
    partition_info = external_manual ? (const char *) partition_1D[0] :
                                       partition_1d_x;
    PUGH_GetSliceSizes (nprocs[0], nsize[0], partition_info, &bounds[0],
                        is_gf && (manual || external_manual));
  }
  else if (dim == 2)
  {
    external_manual = partition_2D[0] != NULL;
    partition_info = external_manual ? (const char *) partition_2D[0] :
                                       partition_2d_x;
    PUGH_GetSliceSizes (nprocs[0], nsize[0], partition_info, &bounds[0],
                        is_gf && (manual || external_manual));
    external_manual = partition_2D[1] != NULL;
    partition_info = external_manual ? (const char *) partition_2D[1] :
                                       partition_2d_y;
    PUGH_GetSliceSizes (nprocs[1], nsize[1], partition_info, &bounds[1],
                        is_gf && (manual || external_manual));
  }
  else if (dim == 3)
  {
    external_manual = partition_3D[0] != NULL;
    partition_info = external_manual ? (const char *) partition_3D[0] :
                                       partition_3d_x;
    PUGH_GetSliceSizes (nprocs[0], nsize[0], partition_info, &bounds[0],
                        is_gf && (manual || external_manual));
    external_manual = partition_3D[1] != NULL;
    partition_info = external_manual ? (const char *) partition_3D[1] :
                                       partition_3d_y;
    PUGH_GetSliceSizes (nprocs[1], nsize[1], partition_info, &bounds[1],
                        is_gf && (manual || external_manual));
    external_manual = partition_3D[2] != NULL;
    partition_info = external_manual ? (const char *) partition_3D[2] :
                                       partition_3d_z;
    PUGH_GetSliceSizes (nprocs[2], nsize[2], partition_info, &bounds[2],
                        is_gf && (manual || external_manual));
  }
  else
  {
    if (is_gf && manual)
    {
      CCTK_WARN (1, "Only 1D, 2D, and 3D supported in manual topology setup");
      retval = -1;
    }
    else
    {
      for (i = 0; i < dim; i++)
      {
        PUGH_GetSliceSizes (nprocs[i], nsize[i], NULL, &bounds[i], 0);
      }
    }
  }

  return (retval);
}


 /*@@
   @routine    PUGH_GetSliceSizes
   @date       Fri Feb 21 10:21:36 2000
   @author     Matei Ripeanu
   @desc
               Computes partition based on parameter information
   @enddesc

   @var        np
   @vdesc      number of processors in this direction
   @vtype      int
   @vio        in
   @endvar
   @var        grid_points
   @vdesc      number of grid points in this direction
   @vtype      int
   @vio        in
   @endvar
   @var        slicesS
   @vdesc      line in parameter file, format "10:10:10"
   @vtype      char *
   @vio        in
   @endvar
   @var        slices
   @vdesc      grid points foe each processor, int array
   @vtype      int **
   @vio        inout
   @endvar
   @var        manual
   @vdesc      manual or automatic partition
   @vtype      int
   @vio        in
   @endvar
@@*/
static void PUGH_GetSliceSizes (int np,
                                int grid_points,
                                const char *slicesS,
                                int **slices,
                                int manual)
{
  int tmp, i=0, rt=0;


  *slices = malloc (np * sizeof (int));
  if (! *slices)
  {
    CCTK_WARN (0, "Not enough memory");
  }

  if (manual && *slicesS)
  {
    while (*slicesS)
    {
      if (i >= np)
      {
        CCTK_WARN (0, "Wrong partition parameters: "
                      "expect data for fewer processors");
      }
      sscanf (slicesS, "%d", &tmp);
      (*slices)[i] = i ? rt - 1 : 0;
      rt += tmp;
      i++;
      while (isdigit ((int) *slicesS))
      {
        slicesS++;
      }
      while (*slicesS && ! isdigit ((int) *slicesS))
      {
        slicesS++;
      }
    }

    if (i != np)
    {
      CCTK_WARN (0, "Wrong partition parameters: "
                    "expect data for more processors");
    }
    if (rt != grid_points)
    {
      CCTK_WARN (0, "Wrong partition parameters: "
                    "total number of grid points doesnt match");
    }
  }
  else
  {
    for (i = 0; i < np; i++)
    {
      (*slices)[i] = rt;
      rt += (grid_points - rt -1) / (np - i);
    }
  }
}
