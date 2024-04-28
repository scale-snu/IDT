#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * set_coordinate_ranges_on;
  CCTK_INT avoid_origin;
  CCTK_INT avoid_originx;
  CCTK_INT avoid_originy;
  CCTK_INT avoid_originz;
  CCTK_INT no_origin;
  CCTK_INT no_originx;
  CCTK_INT no_originy;
  CCTK_INT no_originz;
  CCTK_INT register_default_coordinate_systems;
} PRIVATE_CARTGRID3D_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_CARTGRID3D_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, set_coordinate_ranges_on, CCTK_PARAMETER__CARTGRID3D__set_coordinate_ranges_on); \
  CCTK_DECLARE_INIT (CCTK_INT const, avoid_origin, CCTK_PARAMETER__CARTGRID3D__avoid_origin); \
  CCTK_DECLARE_INIT (CCTK_INT const, avoid_originx, CCTK_PARAMETER__CARTGRID3D__avoid_originx); \
  CCTK_DECLARE_INIT (CCTK_INT const, avoid_originy, CCTK_PARAMETER__CARTGRID3D__avoid_originy); \
  CCTK_DECLARE_INIT (CCTK_INT const, avoid_originz, CCTK_PARAMETER__CARTGRID3D__avoid_originz); \
  CCTK_DECLARE_INIT (CCTK_INT const, no_origin, CCTK_PARAMETER__CARTGRID3D__no_origin); \
  CCTK_DECLARE_INIT (CCTK_INT const, no_originx, CCTK_PARAMETER__CARTGRID3D__no_originx); \
  CCTK_DECLARE_INIT (CCTK_INT const, no_originy, CCTK_PARAMETER__CARTGRID3D__no_originy); \
  CCTK_DECLARE_INIT (CCTK_INT const, no_originz, CCTK_PARAMETER__CARTGRID3D__no_originz); \
  CCTK_DECLARE_INIT (CCTK_INT const, register_default_coordinate_systems, CCTK_PARAMETER__CARTGRID3D__register_default_coordinate_systems); \


#ifndef CCTK_PARAMETER__CARTGRID3D__set_coordinate_ranges_on
#  define CCTK_PARAMETER__CARTGRID3D__set_coordinate_ranges_on PRIVATE_CARTGRID3D_STRUCT.set_coordinate_ranges_on
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__avoid_origin
#  define CCTK_PARAMETER__CARTGRID3D__avoid_origin PRIVATE_CARTGRID3D_STRUCT.avoid_origin
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__avoid_originx
#  define CCTK_PARAMETER__CARTGRID3D__avoid_originx PRIVATE_CARTGRID3D_STRUCT.avoid_originx
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__avoid_originy
#  define CCTK_PARAMETER__CARTGRID3D__avoid_originy PRIVATE_CARTGRID3D_STRUCT.avoid_originy
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__avoid_originz
#  define CCTK_PARAMETER__CARTGRID3D__avoid_originz PRIVATE_CARTGRID3D_STRUCT.avoid_originz
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__no_origin
#  define CCTK_PARAMETER__CARTGRID3D__no_origin PRIVATE_CARTGRID3D_STRUCT.no_origin
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__no_originx
#  define CCTK_PARAMETER__CARTGRID3D__no_originx PRIVATE_CARTGRID3D_STRUCT.no_originx
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__no_originy
#  define CCTK_PARAMETER__CARTGRID3D__no_originy PRIVATE_CARTGRID3D_STRUCT.no_originy
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__no_originz
#  define CCTK_PARAMETER__CARTGRID3D__no_originz PRIVATE_CARTGRID3D_STRUCT.no_originz
#endif
#ifndef CCTK_PARAMETER__CARTGRID3D__register_default_coordinate_systems
#  define CCTK_PARAMETER__CARTGRID3D__register_default_coordinate_systems PRIVATE_CARTGRID3D_STRUCT.register_default_coordinate_systems
#endif

