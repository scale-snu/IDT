/*@@
   @header  PUGHSlab_Prototypes.h
   @author  Automatically generated by CreateFunctionBindings.pl
   @desc
            Prototypes for overloaded functions used by this thorn
   @enddesc
  @@*/


#ifndef _PUGHSLAB_PROTOTYPES_H_
#define _PUGHSLAB_PROTOTYPES_H_  1

#ifdef CCODE
#ifdef __cplusplus
extern "C" {
#endif

CCTK_INT Hyperslab_FreeMapping(const CCTK_INT mapping_handle);
CCTK_INT PUGHSlab_FreeMapping(const CCTK_INT mapping_handle);

CCTK_INT Hyperslab_Get(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT mapping_handle ,
 const CCTK_INT proc ,
 const CCTK_INT vindex ,
 const CCTK_INT timelevel ,
 const CCTK_INT hdatatype ,
 const CCTK_POINTER hdata);
CCTK_INT PUGHSlab_Get(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT mapping_handle ,
 const CCTK_INT proc ,
 const CCTK_INT vindex ,
 const CCTK_INT timelevel ,
 const CCTK_INT hdatatype ,
 const CCTK_POINTER hdata);

CCTK_INT Hyperslab_GetList(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT mapping_handle ,
 const CCTK_INT num_arrays ,
 const CCTK_INT* procs ,
 const CCTK_INT* vindices ,
 const CCTK_INT* timelevels ,
 const CCTK_INT* hdatatypes ,
 const CCTK_POINTER* hdata ,
 CCTK_INT* retvals);
CCTK_INT PUGHSlab_GetList(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT mapping_handle ,
 const CCTK_INT num_arrays ,
 const CCTK_INT* procs ,
 const CCTK_INT* vindices ,
 const CCTK_INT* timelevels ,
 const CCTK_INT* hdatatypes ,
 const CCTK_POINTER* hdata ,
 CCTK_INT* retvals);

CCTK_INT Hyperslab_GlobalMappingByIndex(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT vindex ,
 const CCTK_INT hdim ,
 const CCTK_INT* direction ,
 const CCTK_INT* origin ,
 const CCTK_INT* extent ,
 const CCTK_INT* downsample ,
 const CCTK_INT table_handle ,
 CCTK_INT (*const conversion_fn)(const CCTK_INT nelems ,
 const CCTK_INT src_stride ,
 const CCTK_INT dst_stride ,
 const CCTK_INT src_type ,
 const CCTK_INT dst_type ,
 const CCTK_POINTER_TO_CONST from ,
 const CCTK_POINTER to) ,
 CCTK_INT* hsize);
CCTK_INT PUGHSlab_GlobalMappingByIndex(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT vindex ,
 const CCTK_INT hdim ,
 const CCTK_INT* direction ,
 const CCTK_INT* origin ,
 const CCTK_INT* extent ,
 const CCTK_INT* downsample ,
 const CCTK_INT table_handle ,
 CCTK_INT (*const conversion_fn)(const CCTK_INT nelems ,
 const CCTK_INT src_stride ,
 const CCTK_INT dst_stride ,
 const CCTK_INT src_type ,
 const CCTK_INT dst_type ,
 const CCTK_POINTER_TO_CONST from ,
 const CCTK_POINTER to) ,
 CCTK_INT* hsize);

CCTK_INT Hyperslab_LocalMappingByIndex(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT vindex ,
 const CCTK_INT hdim ,
 const CCTK_INT* direction ,
 const CCTK_INT* origin ,
 const CCTK_INT* extent ,
 const CCTK_INT* downsample ,
 const CCTK_INT table_handle ,
 CCTK_INT (*const conversion_fn)(const CCTK_INT nelems ,
 const CCTK_INT src_stride ,
 const CCTK_INT dst_stride ,
 const CCTK_INT src_type ,
 const CCTK_INT dst_type ,
 const CCTK_POINTER_TO_CONST from ,
 const CCTK_POINTER to) ,
 CCTK_INT* hsize_local ,
 CCTK_INT* hsize_global ,
 CCTK_INT* hoffset_global);
CCTK_INT PUGHSlab_LocalMappingByIndex(const CCTK_POINTER_TO_CONST cctkGH ,
 const CCTK_INT vindex ,
 const CCTK_INT hdim ,
 const CCTK_INT* direction ,
 const CCTK_INT* origin ,
 const CCTK_INT* extent ,
 const CCTK_INT* downsample ,
 const CCTK_INT table_handle ,
 CCTK_INT (*const conversion_fn)(const CCTK_INT nelems ,
 const CCTK_INT src_stride ,
 const CCTK_INT dst_stride ,
 const CCTK_INT src_type ,
 const CCTK_INT dst_type ,
 const CCTK_POINTER_TO_CONST from ,
 const CCTK_POINTER to) ,
 CCTK_INT* hsize_local ,
 CCTK_INT* hsize_global ,
 CCTK_INT* hoffset_global);

#ifdef __cplusplus
}
#endif
#endif /* CCODE */

#ifdef FCODE
#ifdef F90CODE
#define DECLARE_PUGHSLAB_FUNCTIONS _DECLARE_CCTK_FUNCTIONS \
  interface &&\
     CCTK_INT function Hyperslab_FreeMapping (mapping_handle) &&\
       implicit none &&\
       CCTK_INT mapping_handle &&\
     end function Hyperslab_FreeMapping &&\
  end interface &&\
  interface &&\
     CCTK_INT function Hyperslab_Get (cctkGH, mapping_handle, proc, vindex, timelevel, hdatatype, hdata) &&\
       implicit none &&\
       CCTK_POINTER_TO_CONST cctkGH &&\
       CCTK_INT mapping_handle &&\
       CCTK_INT proc &&\
       CCTK_INT vindex &&\
       CCTK_INT timelevel &&\
       CCTK_INT hdatatype &&\
       CCTK_POINTER hdata &&\
     end function Hyperslab_Get &&\
  end interface &&\
  interface &&\
     CCTK_INT function Hyperslab_GetList (cctkGH, mapping_handle, num_arrays, procs, vindices, timelevels, hdatatypes, hdata, retvals) &&\
       implicit none &&\
       CCTK_POINTER_TO_CONST cctkGH &&\
       CCTK_INT mapping_handle &&\
       CCTK_INT num_arrays &&\
       CCTK_INT procs(*) &&\
       CCTK_INT vindices(*) &&\
       CCTK_INT timelevels(*) &&\
       CCTK_INT hdatatypes(*) &&\
       CCTK_POINTER hdata(*) &&\
       CCTK_INT retvals(*) &&\
     end function Hyperslab_GetList &&\
  end interface &&\
  interface &&\
     CCTK_INT function Hyperslab_GlobalMappingByIndex (cctkGH, vindex, hdim, direction, origin, extent, downsample, table_handle, conversion_fn, hsize) &&\
       implicit none &&\
       CCTK_POINTER_TO_CONST cctkGH &&\
       CCTK_INT vindex &&\
       CCTK_INT hdim &&\
       CCTK_INT direction(*) &&\
       CCTK_INT origin(*) &&\
       CCTK_INT extent(*) &&\
       CCTK_INT downsample(*) &&\
       CCTK_INT table_handle &&\
       external conversion_fn &&\
       CCTK_INT conversion_fn &&\
       CCTK_INT hsize(*) &&\
     end function Hyperslab_GlobalMappingByIndex &&\
  end interface &&\
  interface &&\
     CCTK_INT function Hyperslab_LocalMappingByIndex (cctkGH, vindex, hdim, direction, origin, extent, downsample, table_handle, conversion_fn, hsize_local, hsize_global, hoffset_global) &&\
       implicit none &&\
       CCTK_POINTER_TO_CONST cctkGH &&\
       CCTK_INT vindex &&\
       CCTK_INT hdim &&\
       CCTK_INT direction(*) &&\
       CCTK_INT origin(*) &&\
       CCTK_INT extent(*) &&\
       CCTK_INT downsample(*) &&\
       CCTK_INT table_handle &&\
       external conversion_fn &&\
       CCTK_INT conversion_fn &&\
       CCTK_INT hsize_local(*) &&\
       CCTK_INT hsize_global(*) &&\
       CCTK_INT hoffset_global(*) &&\
     end function Hyperslab_LocalMappingByIndex &&\
  end interface &&\

#else /* ! F90CODE */

#define DECLARE_PUGHSLAB_FUNCTIONS _DECLARE_CCTK_FUNCTIONS \
  external Hyperslab_FreeMapping &&\
  CCTK_INT Hyperslab_FreeMapping &&\
  external Hyperslab_Get &&\
  CCTK_INT Hyperslab_Get &&\
  external Hyperslab_GetList &&\
  CCTK_INT Hyperslab_GetList &&\
  external Hyperslab_GlobalMappingByIndex &&\
  CCTK_INT Hyperslab_GlobalMappingByIndex &&\
  external Hyperslab_LocalMappingByIndex &&\
  CCTK_INT Hyperslab_LocalMappingByIndex &&\

#endif /* ! F90CODE */
#endif /* FCODE */

#endif

