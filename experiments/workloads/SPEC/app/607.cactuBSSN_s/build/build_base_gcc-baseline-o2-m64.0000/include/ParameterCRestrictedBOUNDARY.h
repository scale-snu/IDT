#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT radpower;
  CCTK_INT register_copy;
  CCTK_INT register_flat;
  CCTK_INT register_none;
  CCTK_INT register_radiation;
  CCTK_INT register_robin;
  CCTK_INT register_scalar;
  CCTK_INT register_static;
} RESTRICTED_BOUNDARY_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_BOUNDARY_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, radpower, CCTK_PARAMETER__BOUNDARY__radpower); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_copy, CCTK_PARAMETER__BOUNDARY__register_copy); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_flat, CCTK_PARAMETER__BOUNDARY__register_flat); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_none, CCTK_PARAMETER__BOUNDARY__register_none); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_radiation, CCTK_PARAMETER__BOUNDARY__register_radiation); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_robin, CCTK_PARAMETER__BOUNDARY__register_robin); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_scalar, CCTK_PARAMETER__BOUNDARY__register_scalar); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_static, CCTK_PARAMETER__BOUNDARY__register_static); \


#ifndef CCTK_PARAMETER__BOUNDARY__radpower
#  define CCTK_PARAMETER__BOUNDARY__radpower RESTRICTED_BOUNDARY_STRUCT.radpower
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_copy
#  define CCTK_PARAMETER__BOUNDARY__register_copy RESTRICTED_BOUNDARY_STRUCT.register_copy
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_flat
#  define CCTK_PARAMETER__BOUNDARY__register_flat RESTRICTED_BOUNDARY_STRUCT.register_flat
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_none
#  define CCTK_PARAMETER__BOUNDARY__register_none RESTRICTED_BOUNDARY_STRUCT.register_none
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_radiation
#  define CCTK_PARAMETER__BOUNDARY__register_radiation RESTRICTED_BOUNDARY_STRUCT.register_radiation
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_robin
#  define CCTK_PARAMETER__BOUNDARY__register_robin RESTRICTED_BOUNDARY_STRUCT.register_robin
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_scalar
#  define CCTK_PARAMETER__BOUNDARY__register_scalar RESTRICTED_BOUNDARY_STRUCT.register_scalar
#endif
#ifndef CCTK_PARAMETER__BOUNDARY__register_static
#  define CCTK_PARAMETER__BOUNDARY__register_static RESTRICTED_BOUNDARY_STRUCT.register_static
#endif

