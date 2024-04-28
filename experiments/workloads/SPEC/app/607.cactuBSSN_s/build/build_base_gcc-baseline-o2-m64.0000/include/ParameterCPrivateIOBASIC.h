#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL outInfo_dt;
  CCTK_REAL outScalar_dt;
  const char * outInfo_criterion;
  const char * outInfo_reductions;
  const char * outInfo_vars;
  const char * outScalar_criterion;
  const char * outScalar_reductions;
  const char * outScalar_style;
  const char * outScalar_vars;
  const char * out_dir;
  const char * out_format;
  CCTK_INT outInfo_every;
  CCTK_INT outScalar_every;
} PRIVATE_IOBASIC_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_IOBASIC_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, outInfo_dt, CCTK_PARAMETER__IOBASIC__outInfo_dt); \
  CCTK_DECLARE_INIT (CCTK_REAL const, outScalar_dt, CCTK_PARAMETER__IOBASIC__outScalar_dt); \
  CCTK_DECLARE_INIT (const char * const, outInfo_criterion, CCTK_PARAMETER__IOBASIC__outInfo_criterion); \
  CCTK_DECLARE_INIT (const char * const, outInfo_reductions, CCTK_PARAMETER__IOBASIC__outInfo_reductions); \
  CCTK_DECLARE_INIT (const char * const, outInfo_vars, CCTK_PARAMETER__IOBASIC__outInfo_vars); \
  CCTK_DECLARE_INIT (const char * const, outScalar_criterion, CCTK_PARAMETER__IOBASIC__outScalar_criterion); \
  CCTK_DECLARE_INIT (const char * const, outScalar_reductions, CCTK_PARAMETER__IOBASIC__outScalar_reductions); \
  CCTK_DECLARE_INIT (const char * const, outScalar_style, CCTK_PARAMETER__IOBASIC__outScalar_style); \
  CCTK_DECLARE_INIT (const char * const, outScalar_vars, CCTK_PARAMETER__IOBASIC__outScalar_vars); \
  CCTK_DECLARE_INIT (const char * const, out_dir, CCTK_PARAMETER__IOBASIC__out_dir); \
  CCTK_DECLARE_INIT (const char * const, out_format, CCTK_PARAMETER__IOBASIC__out_format); \
  CCTK_DECLARE_INIT (CCTK_INT const, outInfo_every, CCTK_PARAMETER__IOBASIC__outInfo_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, outScalar_every, CCTK_PARAMETER__IOBASIC__outScalar_every); \


#ifndef CCTK_PARAMETER__IOBASIC__outInfo_dt
#  define CCTK_PARAMETER__IOBASIC__outInfo_dt PRIVATE_IOBASIC_STRUCT.outInfo_dt
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outScalar_dt
#  define CCTK_PARAMETER__IOBASIC__outScalar_dt PRIVATE_IOBASIC_STRUCT.outScalar_dt
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outInfo_criterion
#  define CCTK_PARAMETER__IOBASIC__outInfo_criterion PRIVATE_IOBASIC_STRUCT.outInfo_criterion
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outInfo_reductions
#  define CCTK_PARAMETER__IOBASIC__outInfo_reductions PRIVATE_IOBASIC_STRUCT.outInfo_reductions
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outInfo_vars
#  define CCTK_PARAMETER__IOBASIC__outInfo_vars PRIVATE_IOBASIC_STRUCT.outInfo_vars
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outScalar_criterion
#  define CCTK_PARAMETER__IOBASIC__outScalar_criterion PRIVATE_IOBASIC_STRUCT.outScalar_criterion
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outScalar_reductions
#  define CCTK_PARAMETER__IOBASIC__outScalar_reductions PRIVATE_IOBASIC_STRUCT.outScalar_reductions
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outScalar_style
#  define CCTK_PARAMETER__IOBASIC__outScalar_style PRIVATE_IOBASIC_STRUCT.outScalar_style
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outScalar_vars
#  define CCTK_PARAMETER__IOBASIC__outScalar_vars PRIVATE_IOBASIC_STRUCT.outScalar_vars
#endif
#ifndef CCTK_PARAMETER__IOBASIC__out_dir
#  define CCTK_PARAMETER__IOBASIC__out_dir PRIVATE_IOBASIC_STRUCT.out_dir
#endif
#ifndef CCTK_PARAMETER__IOBASIC__out_format
#  define CCTK_PARAMETER__IOBASIC__out_format PRIVATE_IOBASIC_STRUCT.out_format
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outInfo_every
#  define CCTK_PARAMETER__IOBASIC__outInfo_every PRIVATE_IOBASIC_STRUCT.outInfo_every
#endif
#ifndef CCTK_PARAMETER__IOBASIC__outScalar_every
#  define CCTK_PARAMETER__IOBASIC__outScalar_every PRIVATE_IOBASIC_STRUCT.outScalar_every
#endif

