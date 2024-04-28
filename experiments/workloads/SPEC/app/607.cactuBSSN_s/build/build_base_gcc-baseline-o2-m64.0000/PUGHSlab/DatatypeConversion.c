 /*@@
   @file      DatatypeConversion.c
   @date      Thursday Nov 23 2000
   @author    Thomas Radke
   @desc
              Predefined datatype conversion routines for hyperslab extraction.
   @enddesc
   @version   $Id: DatatypeConversion.c 160 2013-03-08 20:31:39Z eschnett $
 @@*/


#include <stddef.h>

#ifdef SPEC 
#include "PUGHSlab/cctk.h" 
#define THORN_DIR "PUGHSlab" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "PUGHSlab.h"
#include "PUGHSlabi.h"


/* the rcs ID and its dummy function to use it */
static const char *rcsid="$Id: DatatypeConversion.c 160 2013-03-08 20:31:39Z eschnett $";
CCTK_FILEVERSION(CactusPUGH_PUGHSlab_DatatypeConversion_c)


/* macro to generate a predefined conversion function
   along with its prototype */
#define CONVERSION_FUNCTION(source_type, dest_type, conversion)               \
static CCTK_INT Convert##source_type##to##dest_type (CCTK_INT nelems,         \
                                                     CCTK_INT src_stride,     \
                                                     CCTK_INT dst_stride,     \
                                                     CCTK_INT src_type,       \
                                                     CCTK_INT dst_type,       \
                                                     CCTK_POINTER_TO_CONST src,\
                                                     CCTK_POINTER dst);       \
                                                                              \
static CCTK_INT Convert##source_type##to##dest_type (CCTK_INT nelems,         \
                                                     CCTK_INT src_stride,     \
                                                     CCTK_INT dst_stride,     \
                                                     CCTK_INT src_type,       \
                                                     CCTK_INT dst_type,       \
                                                     CCTK_POINTER_TO_CONST src,\
                                                     CCTK_POINTER dst)        \
{                                                                             \
  CCTK_INT _i;                                                                \
  CCTK_##dest_type *_dst = dst;                                               \
  const CCTK_##source_type *_src = src;                                       \
                                                                              \
                                                                              \
  (void) (src_type + 0);                                                      \
  (void) (dst_type + 0);                                                      \
  for (_i = 0; _i < nelems; _i++, _src += src_stride, _dst += dst_stride)     \
  {                                                                           \
    conversion (*_src, *_dst, dest_type);                                     \
  }                                                                           \
                                                                              \
  return (nelems);                                                            \
}

/* the conversion of scalar datatypes (integers and reals) */
#ifdef  CONVERT
#undef  CONVERT
#endif
#define CONVERT(src, dst, dst_type)         (dst) = (CCTK_##dst_type) (src)

/* predefined conversion functions for integers */
#if defined(CCTK_INT1) && defined(CCTK_INT2)
CONVERSION_FUNCTION (INT2, INT1, CONVERT)
#endif
#if defined(CCTK_INT1) && defined(CCTK_INT4)
CONVERSION_FUNCTION (INT4, INT1, CONVERT)
#endif
#if defined(CCTK_INT2) && defined(CCTK_INT4)
CONVERSION_FUNCTION (INT4, INT2, CONVERT)
#endif
#if defined(CCTK_INT1) && defined(CCTK_INT8)
CONVERSION_FUNCTION (INT8, INT1, CONVERT)
#endif
#if defined(CCTK_INT2) && defined(CCTK_INT8)
CONVERSION_FUNCTION (INT8, INT2, CONVERT)
#endif
#if defined(CCTK_INT4) && defined(CCTK_INT8)
CONVERSION_FUNCTION (INT8, INT4, CONVERT)
#endif

/* predefined conversion functions for reals */
#if defined(CCTK_REAL4) && defined(CCTK_REAL8)
CONVERSION_FUNCTION (REAL8, REAL4, CONVERT)
#endif
#if defined(CCTK_REAL4) && defined(CCTK_REAL16)
CONVERSION_FUNCTION (REAL16, REAL4, CONVERT)
#endif
#if defined(CCTK_REAL8) && defined(CCTK_REAL16)
CONVERSION_FUNCTION (REAL16, REAL8, CONVERT)
#endif

/* predefined conversion functions for complex */
#if defined(CCTK_REAL4) && defined(CCTK_REAL8)
CONVERSION_FUNCTION (COMPLEX16, COMPLEX8, CONVERT)
#endif
#if defined(CCTK_REAL4) && defined(CCTK_REAL16)
CONVERSION_FUNCTION (COMPLEX32, COMPLEX16, CONVERT)
#endif
#if defined(CCTK_REAL8) && defined(CCTK_REAL16)
CONVERSION_FUNCTION (COMPLEX32, COMPLEX8, CONVERT)
#endif


/* prototypes of routines defined in this source file */
static int PUGHSlabi_PrecisionVarType (int type);


 /*@@
   @routine    PUGHSlabi_GetDatatypeConversionFn
   @date       Fri 23 Nov 2000
   @author     Thomas Radke
   @desc
               Finds a predefined datatype conversion function.
   @enddesc

   @var        src_type
   @vdesc      CCTK datatype of the source
   @vtype      int
   @vio        in
   @endvar
   @var        dst_type
   @vdesc      CCTK datatype of the destination (hyperslab datatype)
   @vtype      int
   @vio        in
   @endvar

   @returntype PUGHSlab_conversion_fn
   @returndesc
               the appropriate function pointer
               or NULL if no predefined conversion function was found
   @endreturndesc
@@*/
t_hslabConversionFn PUGHSlabi_GetDatatypeConversionFn (int src_type,
                                                       int dst_type)
{
  t_hslabConversionFn retval;


  /* get the precision datatype for generic CCTK_TYPE variable types */
  src_type = PUGHSlabi_PrecisionVarType (src_type);
  dst_type = PUGHSlabi_PrecisionVarType (dst_type);

  /* get the appropriate conversion routine */
  if (0)
  {
    /* this is just because of the following #ifdef'ed else branches */
  }
#if defined(CCTK_INT1) && defined(CCTK_INT2)
  else if (src_type == CCTK_VARIABLE_INT2 && dst_type == CCTK_VARIABLE_INT1)
  {
    retval = ConvertINT2toINT1;
  }
#endif
#if defined(CCTK_INT1) && defined(CCTK_INT4)
  else if (src_type == CCTK_VARIABLE_INT4 && dst_type == CCTK_VARIABLE_INT1)
  {
    retval = ConvertINT4toINT1;
  }
#endif
#if defined(CCTK_INT2) && defined(CCTK_INT4)
  else if (src_type == CCTK_VARIABLE_INT4 && dst_type == CCTK_VARIABLE_INT2)
  {
    retval = ConvertINT4toINT2;
  }
#endif
#if defined(CCTK_INT1) && defined(CCTK_INT8)
  else if (src_type == CCTK_VARIABLE_INT8 && dst_type == CCTK_VARIABLE_INT1)
  {
    retval = ConvertINT8toINT1;
  }
#endif
#if defined(CCTK_INT2) && defined(CCTK_INT8)
  else if (src_type == CCTK_VARIABLE_INT8 && dst_type == CCTK_VARIABLE_INT2)
  {
    retval = ConvertINT8toINT2;
  }
#endif
#if defined(CCTK_INT4) && defined(CCTK_INT8)
  else if (src_type == CCTK_VARIABLE_INT8 && dst_type == CCTK_VARIABLE_INT4)
  {
    retval = ConvertINT8toINT4;
  }
#endif
#if defined(CCTK_REAL4) && defined(CCTK_REAL8)
  else if (src_type == CCTK_VARIABLE_REAL8 && dst_type == CCTK_VARIABLE_REAL4)
  {
    retval = ConvertREAL8toREAL4;
  }
#endif
#if defined(CCTK_REAL4) && defined(CCTK_REAL16)
  else if (src_type == CCTK_VARIABLE_REAL16 && dst_type == CCTK_VARIABLE_REAL4)
  {
    retval = ConvertREAL16toREAL4;
  }
#endif
#if defined(CCTK_REAL8) && defined(CCTK_REAL16)
  else if (src_type == CCTK_VARIABLE_REAL16 && dst_type == CCTK_VARIABLE_REAL8)
  {
    retval = ConvertREAL16toREAL8;
  }
#endif
#if defined(CCTK_REAL4) && defined(CCTK_REAL8)
  else if (src_type == CCTK_VARIABLE_COMPLEX16 && dst_type == CCTK_VARIABLE_COMPLEX8)
  {
    retval = ConvertCOMPLEX16toCOMPLEX8;
  }
#endif
#if defined(CCTK_REAL8) && defined(CCTK_REAL16)
  else if (src_type == CCTK_VARIABLE_COMPLEX32 && dst_type == CCTK_VARIABLE_COMPLEX8)
  {
    retval = ConvertCOMPLEX32toCOMPLEX8;
  }
#endif
#if defined(CCTK_REAL8) && defined(CCTK_REAL16)
  else if (src_type == CCTK_VARIABLE_COMPLEX32 && dst_type == CCTK_VARIABLE_COMPLEX16)
  {
    retval = ConvertCOMPLEX32toCOMPLEX16;
  }
#endif
  else
  {
    retval = NULL;
  }

  return (retval);
}


/**************************************************************************/
/*                             local functions                            */
/**************************************************************************/
/* get the precision datatype for generic CCTK_TYPE variable types */
static int PUGHSlabi_PrecisionVarType (int type)
{
  if (type == CCTK_VARIABLE_INT)
  {
#ifdef CCTK_INTEGER_PRECISION_8
    type = CCTK_VARIABLE_INT8;
#elif  CCTK_INTEGER_PRECISION_4
    type = CCTK_VARIABLE_INT4;
#elif  CCTK_INTEGER_PRECISION_2
    type = CCTK_VARIABLE_INT2;
#elif  CCTK_INTEGER_PRECISION_1
    type = CCTK_VARIABLE_INT1;
#endif
  }
  else if (type == CCTK_VARIABLE_REAL)
  {
#ifdef CCTK_REAL_PRECISION_16
    type = CCTK_VARIABLE_REAL16;
#elif  CCTK_REAL_PRECISION_8
    type = CCTK_VARIABLE_REAL8;
#elif  CCTK_REAL_PRECISION_4
    type = CCTK_VARIABLE_REAL4;
#endif
  }
  else if (type == CCTK_VARIABLE_COMPLEX)
  {
#ifdef CCTK_REAL_PRECISION_16
    type = CCTK_VARIABLE_COMPLEX32;
#elif  CCTK_REAL_PRECISION_8
    type = CCTK_VARIABLE_COMPLEX16;
#elif  CCTK_REAL_PRECISION_4
    type = CCTK_VARIABLE_COMPLEX8;
#endif
  }

  return (type);
}
