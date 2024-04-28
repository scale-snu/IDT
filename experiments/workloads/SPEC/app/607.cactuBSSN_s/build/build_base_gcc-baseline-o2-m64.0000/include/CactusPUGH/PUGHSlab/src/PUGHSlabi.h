 /*@@
   @header    PUGHSlabi.h
   @date      Sun 28 May 2000
   @author    Thomas Radke
   @desc 
              Internal function declarations of thorn PUGHSlab
   @enddesc
   @version   $Header$
 @@*/


#ifndef _PUGHSLAB_PUGHSLABI_H_
#define _PUGHSLAB_PUGHSLABI_H_ 1

#include "cctk_Groups.h"
#include "PUGHSlab.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* structure describing a hyperslab mapping */
typedef struct hslab_mapping_t
{
  int handle;
  int hdim;
  int vdim;
  int *do_dir;                           /* vdim */
  int *local_startpoint;                 /* vdim */
  int *local_endpoint;                   /* vdim */
  int *global_startpoint;                /* vdim */
  int *global_endpoint;                  /* vdim */
  int *downsample;                       /* vdim */
  int *displs;                           /* nprocs */
  int *recvcnts;                         /* nprocs */
  CCTK_INT *global_hsize;                /* hdim */
  CCTK_INT *global_hoffset;              /* hdim */
  CCTK_INT *local_hsize;                 /* hdim+1 */
  CCTK_INT *chunk;                       /* nprocs*(2*hdim+1) */
  int totals;
  int totals_global;
  int is_global_hyperslab;
  int is_full_hyperslab;
  int is_diagonal_in_3D;
  t_hslabConversionFn conversion_fn;
  struct hslab_mapping_t *prev, *next;
  cGroup vinfo;
} hslab_mapping_t;


/* utility routines */
hslab_mapping_t *PUGHSlabi_GetMapping (int mapping_handle);
t_hslabConversionFn PUGHSlabi_GetDatatypeConversionFn (int src_type,
                                                       int dst_type);


#ifdef __cplusplus
}
#endif

#endif  /* _PUGHSLAB_PUGHSLABI_H_ */
