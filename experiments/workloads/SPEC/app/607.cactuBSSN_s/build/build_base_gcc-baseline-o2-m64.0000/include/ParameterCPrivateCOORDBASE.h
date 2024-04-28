#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL dx;
  CCTK_REAL dy;
  CCTK_REAL dz;
  CCTK_REAL xextent;
  CCTK_REAL xmax;
  CCTK_REAL xmin;
  CCTK_REAL yextent;
  CCTK_REAL ymax;
  CCTK_REAL ymin;
  CCTK_REAL zextent;
  CCTK_REAL zmax;
  CCTK_REAL zmin;
  const char * domainsize;
  const char * spacing;
  CCTK_INT boundary_internal_x_lower;
  CCTK_INT boundary_internal_x_upper;
  CCTK_INT boundary_internal_y_lower;
  CCTK_INT boundary_internal_y_upper;
  CCTK_INT boundary_internal_z_lower;
  CCTK_INT boundary_internal_z_upper;
  CCTK_INT boundary_shiftout_x_lower;
  CCTK_INT boundary_shiftout_x_upper;
  CCTK_INT boundary_shiftout_y_lower;
  CCTK_INT boundary_shiftout_y_upper;
  CCTK_INT boundary_shiftout_z_lower;
  CCTK_INT boundary_shiftout_z_upper;
  CCTK_INT boundary_size_x_lower;
  CCTK_INT boundary_size_x_upper;
  CCTK_INT boundary_size_y_lower;
  CCTK_INT boundary_size_y_upper;
  CCTK_INT boundary_size_z_lower;
  CCTK_INT boundary_size_z_upper;
  CCTK_INT boundary_staggered_x_lower;
  CCTK_INT boundary_staggered_x_upper;
  CCTK_INT boundary_staggered_y_lower;
  CCTK_INT boundary_staggered_y_upper;
  CCTK_INT boundary_staggered_z_lower;
  CCTK_INT boundary_staggered_z_upper;
  CCTK_INT ncells_x;
  CCTK_INT ncells_y;
  CCTK_INT ncells_z;
  CCTK_INT zero_origin_x;
  CCTK_INT zero_origin_y;
  CCTK_INT zero_origin_z;
} PRIVATE_COORDBASE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_COORDBASE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, dx, CCTK_PARAMETER__COORDBASE__dx); \
  CCTK_DECLARE_INIT (CCTK_REAL const, dy, CCTK_PARAMETER__COORDBASE__dy); \
  CCTK_DECLARE_INIT (CCTK_REAL const, dz, CCTK_PARAMETER__COORDBASE__dz); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xextent, CCTK_PARAMETER__COORDBASE__xextent); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xmax, CCTK_PARAMETER__COORDBASE__xmax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, xmin, CCTK_PARAMETER__COORDBASE__xmin); \
  CCTK_DECLARE_INIT (CCTK_REAL const, yextent, CCTK_PARAMETER__COORDBASE__yextent); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ymax, CCTK_PARAMETER__COORDBASE__ymax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, ymin, CCTK_PARAMETER__COORDBASE__ymin); \
  CCTK_DECLARE_INIT (CCTK_REAL const, zextent, CCTK_PARAMETER__COORDBASE__zextent); \
  CCTK_DECLARE_INIT (CCTK_REAL const, zmax, CCTK_PARAMETER__COORDBASE__zmax); \
  CCTK_DECLARE_INIT (CCTK_REAL const, zmin, CCTK_PARAMETER__COORDBASE__zmin); \
  CCTK_DECLARE_INIT (const char * const, domainsize, CCTK_PARAMETER__COORDBASE__domainsize); \
  CCTK_DECLARE_INIT (const char * const, spacing, CCTK_PARAMETER__COORDBASE__spacing); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_internal_x_lower, CCTK_PARAMETER__COORDBASE__boundary_internal_x_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_internal_x_upper, CCTK_PARAMETER__COORDBASE__boundary_internal_x_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_internal_y_lower, CCTK_PARAMETER__COORDBASE__boundary_internal_y_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_internal_y_upper, CCTK_PARAMETER__COORDBASE__boundary_internal_y_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_internal_z_lower, CCTK_PARAMETER__COORDBASE__boundary_internal_z_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_internal_z_upper, CCTK_PARAMETER__COORDBASE__boundary_internal_z_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_shiftout_x_lower, CCTK_PARAMETER__COORDBASE__boundary_shiftout_x_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_shiftout_x_upper, CCTK_PARAMETER__COORDBASE__boundary_shiftout_x_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_shiftout_y_lower, CCTK_PARAMETER__COORDBASE__boundary_shiftout_y_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_shiftout_y_upper, CCTK_PARAMETER__COORDBASE__boundary_shiftout_y_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_shiftout_z_lower, CCTK_PARAMETER__COORDBASE__boundary_shiftout_z_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_shiftout_z_upper, CCTK_PARAMETER__COORDBASE__boundary_shiftout_z_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_size_x_lower, CCTK_PARAMETER__COORDBASE__boundary_size_x_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_size_x_upper, CCTK_PARAMETER__COORDBASE__boundary_size_x_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_size_y_lower, CCTK_PARAMETER__COORDBASE__boundary_size_y_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_size_y_upper, CCTK_PARAMETER__COORDBASE__boundary_size_y_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_size_z_lower, CCTK_PARAMETER__COORDBASE__boundary_size_z_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_size_z_upper, CCTK_PARAMETER__COORDBASE__boundary_size_z_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_staggered_x_lower, CCTK_PARAMETER__COORDBASE__boundary_staggered_x_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_staggered_x_upper, CCTK_PARAMETER__COORDBASE__boundary_staggered_x_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_staggered_y_lower, CCTK_PARAMETER__COORDBASE__boundary_staggered_y_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_staggered_y_upper, CCTK_PARAMETER__COORDBASE__boundary_staggered_y_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_staggered_z_lower, CCTK_PARAMETER__COORDBASE__boundary_staggered_z_lower); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_staggered_z_upper, CCTK_PARAMETER__COORDBASE__boundary_staggered_z_upper); \
  CCTK_DECLARE_INIT (CCTK_INT const, ncells_x, CCTK_PARAMETER__COORDBASE__ncells_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, ncells_y, CCTK_PARAMETER__COORDBASE__ncells_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, ncells_z, CCTK_PARAMETER__COORDBASE__ncells_z); \
  CCTK_DECLARE_INIT (CCTK_INT const, zero_origin_x, CCTK_PARAMETER__COORDBASE__zero_origin_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, zero_origin_y, CCTK_PARAMETER__COORDBASE__zero_origin_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, zero_origin_z, CCTK_PARAMETER__COORDBASE__zero_origin_z); \


#ifndef CCTK_PARAMETER__COORDBASE__dx
#  define CCTK_PARAMETER__COORDBASE__dx PRIVATE_COORDBASE_STRUCT.dx
#endif
#ifndef CCTK_PARAMETER__COORDBASE__dy
#  define CCTK_PARAMETER__COORDBASE__dy PRIVATE_COORDBASE_STRUCT.dy
#endif
#ifndef CCTK_PARAMETER__COORDBASE__dz
#  define CCTK_PARAMETER__COORDBASE__dz PRIVATE_COORDBASE_STRUCT.dz
#endif
#ifndef CCTK_PARAMETER__COORDBASE__xextent
#  define CCTK_PARAMETER__COORDBASE__xextent PRIVATE_COORDBASE_STRUCT.xextent
#endif
#ifndef CCTK_PARAMETER__COORDBASE__xmax
#  define CCTK_PARAMETER__COORDBASE__xmax PRIVATE_COORDBASE_STRUCT.xmax
#endif
#ifndef CCTK_PARAMETER__COORDBASE__xmin
#  define CCTK_PARAMETER__COORDBASE__xmin PRIVATE_COORDBASE_STRUCT.xmin
#endif
#ifndef CCTK_PARAMETER__COORDBASE__yextent
#  define CCTK_PARAMETER__COORDBASE__yextent PRIVATE_COORDBASE_STRUCT.yextent
#endif
#ifndef CCTK_PARAMETER__COORDBASE__ymax
#  define CCTK_PARAMETER__COORDBASE__ymax PRIVATE_COORDBASE_STRUCT.ymax
#endif
#ifndef CCTK_PARAMETER__COORDBASE__ymin
#  define CCTK_PARAMETER__COORDBASE__ymin PRIVATE_COORDBASE_STRUCT.ymin
#endif
#ifndef CCTK_PARAMETER__COORDBASE__zextent
#  define CCTK_PARAMETER__COORDBASE__zextent PRIVATE_COORDBASE_STRUCT.zextent
#endif
#ifndef CCTK_PARAMETER__COORDBASE__zmax
#  define CCTK_PARAMETER__COORDBASE__zmax PRIVATE_COORDBASE_STRUCT.zmax
#endif
#ifndef CCTK_PARAMETER__COORDBASE__zmin
#  define CCTK_PARAMETER__COORDBASE__zmin PRIVATE_COORDBASE_STRUCT.zmin
#endif
#ifndef CCTK_PARAMETER__COORDBASE__domainsize
#  define CCTK_PARAMETER__COORDBASE__domainsize PRIVATE_COORDBASE_STRUCT.domainsize
#endif
#ifndef CCTK_PARAMETER__COORDBASE__spacing
#  define CCTK_PARAMETER__COORDBASE__spacing PRIVATE_COORDBASE_STRUCT.spacing
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_internal_x_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_internal_x_lower PRIVATE_COORDBASE_STRUCT.boundary_internal_x_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_internal_x_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_internal_x_upper PRIVATE_COORDBASE_STRUCT.boundary_internal_x_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_internal_y_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_internal_y_lower PRIVATE_COORDBASE_STRUCT.boundary_internal_y_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_internal_y_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_internal_y_upper PRIVATE_COORDBASE_STRUCT.boundary_internal_y_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_internal_z_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_internal_z_lower PRIVATE_COORDBASE_STRUCT.boundary_internal_z_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_internal_z_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_internal_z_upper PRIVATE_COORDBASE_STRUCT.boundary_internal_z_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_shiftout_x_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_shiftout_x_lower PRIVATE_COORDBASE_STRUCT.boundary_shiftout_x_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_shiftout_x_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_shiftout_x_upper PRIVATE_COORDBASE_STRUCT.boundary_shiftout_x_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_shiftout_y_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_shiftout_y_lower PRIVATE_COORDBASE_STRUCT.boundary_shiftout_y_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_shiftout_y_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_shiftout_y_upper PRIVATE_COORDBASE_STRUCT.boundary_shiftout_y_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_shiftout_z_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_shiftout_z_lower PRIVATE_COORDBASE_STRUCT.boundary_shiftout_z_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_shiftout_z_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_shiftout_z_upper PRIVATE_COORDBASE_STRUCT.boundary_shiftout_z_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_size_x_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_size_x_lower PRIVATE_COORDBASE_STRUCT.boundary_size_x_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_size_x_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_size_x_upper PRIVATE_COORDBASE_STRUCT.boundary_size_x_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_size_y_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_size_y_lower PRIVATE_COORDBASE_STRUCT.boundary_size_y_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_size_y_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_size_y_upper PRIVATE_COORDBASE_STRUCT.boundary_size_y_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_size_z_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_size_z_lower PRIVATE_COORDBASE_STRUCT.boundary_size_z_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_size_z_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_size_z_upper PRIVATE_COORDBASE_STRUCT.boundary_size_z_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_staggered_x_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_staggered_x_lower PRIVATE_COORDBASE_STRUCT.boundary_staggered_x_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_staggered_x_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_staggered_x_upper PRIVATE_COORDBASE_STRUCT.boundary_staggered_x_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_staggered_y_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_staggered_y_lower PRIVATE_COORDBASE_STRUCT.boundary_staggered_y_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_staggered_y_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_staggered_y_upper PRIVATE_COORDBASE_STRUCT.boundary_staggered_y_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_staggered_z_lower
#  define CCTK_PARAMETER__COORDBASE__boundary_staggered_z_lower PRIVATE_COORDBASE_STRUCT.boundary_staggered_z_lower
#endif
#ifndef CCTK_PARAMETER__COORDBASE__boundary_staggered_z_upper
#  define CCTK_PARAMETER__COORDBASE__boundary_staggered_z_upper PRIVATE_COORDBASE_STRUCT.boundary_staggered_z_upper
#endif
#ifndef CCTK_PARAMETER__COORDBASE__ncells_x
#  define CCTK_PARAMETER__COORDBASE__ncells_x PRIVATE_COORDBASE_STRUCT.ncells_x
#endif
#ifndef CCTK_PARAMETER__COORDBASE__ncells_y
#  define CCTK_PARAMETER__COORDBASE__ncells_y PRIVATE_COORDBASE_STRUCT.ncells_y
#endif
#ifndef CCTK_PARAMETER__COORDBASE__ncells_z
#  define CCTK_PARAMETER__COORDBASE__ncells_z PRIVATE_COORDBASE_STRUCT.ncells_z
#endif
#ifndef CCTK_PARAMETER__COORDBASE__zero_origin_x
#  define CCTK_PARAMETER__COORDBASE__zero_origin_x PRIVATE_COORDBASE_STRUCT.zero_origin_x
#endif
#ifndef CCTK_PARAMETER__COORDBASE__zero_origin_y
#  define CCTK_PARAMETER__COORDBASE__zero_origin_y PRIVATE_COORDBASE_STRUCT.zero_origin_y
#endif
#ifndef CCTK_PARAMETER__COORDBASE__zero_origin_z
#  define CCTK_PARAMETER__COORDBASE__zero_origin_z PRIVATE_COORDBASE_STRUCT.zero_origin_z
#endif

