#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL dx;
  CCTK_REAL dxyz;
  CCTK_REAL dy;
  CCTK_REAL dz;
  CCTK_REAL xmax;
  CCTK_REAL xmin;
  CCTK_REAL xyzmax;
  CCTK_REAL xyzmin;
  CCTK_REAL ymax;
  CCTK_REAL ymin;
  CCTK_REAL zmax;
  CCTK_REAL zmin;
  const char * bitant_plane;
  const char * domain;
  const char * quadrant_direction;
  const char * rotation_axis;
  const char * type;
  CCTK_INT symmetry_xmax;
  CCTK_INT symmetry_xmin;
  CCTK_INT symmetry_ymax;
  CCTK_INT symmetry_ymin;
  CCTK_INT symmetry_zmax;
  CCTK_INT symmetry_zmin;
} RESTRICTED_GRID_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_GRID_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, dx, CCTK_PARAMETER__GRID__dx); \
  CCTK_DECLARE_INIT (CCTK_REAL const, dxyz, CCTK_PARAMETER__GRID__dxyz); \
  CCTK_DECLARE_INIT (CCTK_REAL const, dy, CCTK_PARAMETER__GRID__dy); \
  CCTK_DECLARE_INIT (CCTK_REAL const, dz, CCTK_PARAMETER__GRID__dz); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xmax, CCTK_PARAMETER__GRID__xmax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xmin, CCTK_PARAMETER__GRID__xmin); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xyzmax, CCTK_PARAMETER__GRID__xyzmax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xyzmin, CCTK_PARAMETER__GRID__xyzmin); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ymax, CCTK_PARAMETER__GRID__ymax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ymin, CCTK_PARAMETER__GRID__ymin); \
  CCTK_DECLARE_INIT (CCTK_REAL const, zmax, CCTK_PARAMETER__GRID__zmax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, zmin, CCTK_PARAMETER__GRID__zmin); \
  CCTK_DECLARE_INIT (const char * const, bitant_plane, CCTK_PARAMETER__GRID__bitant_plane); \
  CCTK_DECLARE_INIT (const char * const, domain, CCTK_PARAMETER__GRID__domain); \
  CCTK_DECLARE_INIT (const char * const, quadrant_direction, CCTK_PARAMETER__GRID__quadrant_direction); \
  CCTK_DECLARE_INIT (const char * const, rotation_axis, CCTK_PARAMETER__GRID__rotation_axis); \
  CCTK_DECLARE_INIT (const char * const, type, CCTK_PARAMETER__GRID__type); \
  CCTK_DECLARE_INIT (CCTK_INT const, symmetry_xmax, CCTK_PARAMETER__GRID__symmetry_xmax); \
  CCTK_DECLARE_INIT (CCTK_INT const, symmetry_xmin, CCTK_PARAMETER__GRID__symmetry_xmin); \
  CCTK_DECLARE_INIT (CCTK_INT const, symmetry_ymax, CCTK_PARAMETER__GRID__symmetry_ymax); \
  CCTK_DECLARE_INIT (CCTK_INT const, symmetry_ymin, CCTK_PARAMETER__GRID__symmetry_ymin); \
  CCTK_DECLARE_INIT (CCTK_INT const, symmetry_zmax, CCTK_PARAMETER__GRID__symmetry_zmax); \
  CCTK_DECLARE_INIT (CCTK_INT const, symmetry_zmin, CCTK_PARAMETER__GRID__symmetry_zmin); \


#ifndef CCTK_PARAMETER__GRID__dx
#  define CCTK_PARAMETER__GRID__dx RESTRICTED_GRID_STRUCT.dx
#endif
#ifndef CCTK_PARAMETER__GRID__dxyz
#  define CCTK_PARAMETER__GRID__dxyz RESTRICTED_GRID_STRUCT.dxyz
#endif
#ifndef CCTK_PARAMETER__GRID__dy
#  define CCTK_PARAMETER__GRID__dy RESTRICTED_GRID_STRUCT.dy
#endif
#ifndef CCTK_PARAMETER__GRID__dz
#  define CCTK_PARAMETER__GRID__dz RESTRICTED_GRID_STRUCT.dz
#endif
#ifndef CCTK_PARAMETER__GRID__xmax
#  define CCTK_PARAMETER__GRID__xmax RESTRICTED_GRID_STRUCT.xmax
#endif
#ifndef CCTK_PARAMETER__GRID__xmin
#  define CCTK_PARAMETER__GRID__xmin RESTRICTED_GRID_STRUCT.xmin
#endif
#ifndef CCTK_PARAMETER__GRID__xyzmax
#  define CCTK_PARAMETER__GRID__xyzmax RESTRICTED_GRID_STRUCT.xyzmax
#endif
#ifndef CCTK_PARAMETER__GRID__xyzmin
#  define CCTK_PARAMETER__GRID__xyzmin RESTRICTED_GRID_STRUCT.xyzmin
#endif
#ifndef CCTK_PARAMETER__GRID__ymax
#  define CCTK_PARAMETER__GRID__ymax RESTRICTED_GRID_STRUCT.ymax
#endif
#ifndef CCTK_PARAMETER__GRID__ymin
#  define CCTK_PARAMETER__GRID__ymin RESTRICTED_GRID_STRUCT.ymin
#endif
#ifndef CCTK_PARAMETER__GRID__zmax
#  define CCTK_PARAMETER__GRID__zmax RESTRICTED_GRID_STRUCT.zmax
#endif
#ifndef CCTK_PARAMETER__GRID__zmin
#  define CCTK_PARAMETER__GRID__zmin RESTRICTED_GRID_STRUCT.zmin
#endif
#ifndef CCTK_PARAMETER__GRID__bitant_plane
#  define CCTK_PARAMETER__GRID__bitant_plane RESTRICTED_GRID_STRUCT.bitant_plane
#endif
#ifndef CCTK_PARAMETER__GRID__domain
#  define CCTK_PARAMETER__GRID__domain RESTRICTED_GRID_STRUCT.domain
#endif
#ifndef CCTK_PARAMETER__GRID__quadrant_direction
#  define CCTK_PARAMETER__GRID__quadrant_direction RESTRICTED_GRID_STRUCT.quadrant_direction
#endif
#ifndef CCTK_PARAMETER__GRID__rotation_axis
#  define CCTK_PARAMETER__GRID__rotation_axis RESTRICTED_GRID_STRUCT.rotation_axis
#endif
#ifndef CCTK_PARAMETER__GRID__type
#  define CCTK_PARAMETER__GRID__type RESTRICTED_GRID_STRUCT.type
#endif
#ifndef CCTK_PARAMETER__GRID__symmetry_xmax
#  define CCTK_PARAMETER__GRID__symmetry_xmax RESTRICTED_GRID_STRUCT.symmetry_xmax
#endif
#ifndef CCTK_PARAMETER__GRID__symmetry_xmin
#  define CCTK_PARAMETER__GRID__symmetry_xmin RESTRICTED_GRID_STRUCT.symmetry_xmin
#endif
#ifndef CCTK_PARAMETER__GRID__symmetry_ymax
#  define CCTK_PARAMETER__GRID__symmetry_ymax RESTRICTED_GRID_STRUCT.symmetry_ymax
#endif
#ifndef CCTK_PARAMETER__GRID__symmetry_ymin
#  define CCTK_PARAMETER__GRID__symmetry_ymin RESTRICTED_GRID_STRUCT.symmetry_ymin
#endif
#ifndef CCTK_PARAMETER__GRID__symmetry_zmax
#  define CCTK_PARAMETER__GRID__symmetry_zmax RESTRICTED_GRID_STRUCT.symmetry_zmax
#endif
#ifndef CCTK_PARAMETER__GRID__symmetry_zmin
#  define CCTK_PARAMETER__GRID__symmetry_zmin RESTRICTED_GRID_STRUCT.symmetry_zmin
#endif

