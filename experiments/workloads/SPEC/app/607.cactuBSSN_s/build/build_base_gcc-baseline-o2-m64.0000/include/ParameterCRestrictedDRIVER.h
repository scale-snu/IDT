#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT periodic;
  CCTK_INT periodic_x;
  CCTK_INT periodic_y;
  CCTK_INT periodic_z;
} RESTRICTED_DRIVER_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_DRIVER_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, periodic, CCTK_PARAMETER__DRIVER__periodic); \
  CCTK_DECLARE_INIT (CCTK_INT const, periodic_x, CCTK_PARAMETER__DRIVER__periodic_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, periodic_y, CCTK_PARAMETER__DRIVER__periodic_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, periodic_z, CCTK_PARAMETER__DRIVER__periodic_z); \


#ifndef CCTK_PARAMETER__DRIVER__periodic
#  define CCTK_PARAMETER__DRIVER__periodic RESTRICTED_DRIVER_STRUCT.periodic
#endif
#ifndef CCTK_PARAMETER__DRIVER__periodic_x
#  define CCTK_PARAMETER__DRIVER__periodic_x RESTRICTED_DRIVER_STRUCT.periodic_x
#endif
#ifndef CCTK_PARAMETER__DRIVER__periodic_y
#  define CCTK_PARAMETER__DRIVER__periodic_y RESTRICTED_DRIVER_STRUCT.periodic_y
#endif
#ifndef CCTK_PARAMETER__DRIVER__periodic_z
#  define CCTK_PARAMETER__DRIVER__periodic_z RESTRICTED_DRIVER_STRUCT.periodic_z
#endif

