 /*@@
   @header    PUGHSlab.h
   @date      Sun 28 May 2000
   @author    Thomas Radke
   @desc
              Global function declarations of thorn PUGHSlab
   @enddesc
   @version   $Header$
 @@*/


#ifndef _PUGHSLAB_PUGHSLAB_H_
#define _PUGHSLAB_PUGHSLAB_H_ 1

#ifdef __cplusplus
extern "C"
{
#endif

/* prototype of a datatype conversion routine */
typedef CCTK_INT (*t_hslabConversionFn) (CCTK_INT nelems,
                                         CCTK_INT src_stride,
                                         CCTK_INT dst_stride,
                                         CCTK_INT src_type,
                                         CCTK_INT dst_type,
                                         CCTK_POINTER_TO_CONST src,
                                         CCTK_POINTER dst);

/* function prototypes */
CCTK_INT PUGHSlab_Get (CCTK_POINTER_TO_CONST _GH,
                       CCTK_INT mapping_handle,
                       CCTK_INT proc,
                       CCTK_INT vindex,
                       CCTK_INT timelevel,
                       CCTK_INT hdatatype,
                       CCTK_POINTER hdata);
CCTK_INT PUGHSlab_GetList (CCTK_POINTER_TO_CONST _GH,
                           CCTK_INT mapping_handle,
                           CCTK_INT num_arrays,
                           const CCTK_INT *procs,       /* num_arrays */
                           const CCTK_INT *vindices,    /* num_arrays */
                           const CCTK_INT *timelevels,  /* num_arrays */
                           const CCTK_INT *hdatatypes,  /* num_arrays */
                           const CCTK_POINTER *hdata,   /* num_arrays */
                           CCTK_INT *retvals            /* num_arrays */);
CCTK_INT PUGHSlab_LocalMappingByIndex (
           CCTK_POINTER_TO_CONST _GH,
           CCTK_INT vindex,
           CCTK_INT hdim,
           const CCTK_INT *direction, /* vdim*hdim */
           const CCTK_INT *origin,    /* vdim */
           const CCTK_INT *extent,    /* hdim */
           const CCTK_INT *downsample,/* hdim */
           CCTK_INT table_handle,
           t_hslabConversionFn conversion_fn,
           CCTK_INT *hsize_local,     /* hdim */
           CCTK_INT *hsize_global,    /* hdim */
           CCTK_INT *hoffset_global   /* hdim */);

CCTK_INT PUGHSlab_GlobalMappingByIndex (
           CCTK_POINTER_TO_CONST _GH,
           CCTK_INT vindex,
           CCTK_INT hdim,
           const CCTK_INT *direction, /* vdim*hdim */
           const CCTK_INT *origin,    /* vdim */
           const CCTK_INT *extent,    /* hdim */
           const CCTK_INT *downsample,/* hdim */
           CCTK_INT table_handle,
           t_hslabConversionFn conversion_fn,
           CCTK_INT *hsize            /* hdim */);
CCTK_INT PUGHSlab_FreeMapping (CCTK_INT mapping_handle);


#ifdef __cplusplus
}
#endif

#endif  /* _PUGHSLAB_PUGHSLAB_H_ */
