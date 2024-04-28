#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL out1D_xline_y;
  CCTK_REAL out1D_xline_z;
  CCTK_REAL out1D_yline_x;
  CCTK_REAL out1D_yline_z;
  CCTK_REAL out1D_zline_x;
  CCTK_REAL out1D_zline_y;
  CCTK_REAL out2D_xyplane_z;
  CCTK_REAL out2D_xzplane_y;
  CCTK_REAL out2D_yzplane_x;
  const char * out1D_dir;
  const char * out1D_style;
  const char * out1D_vars;
  const char * out2D_dir;
  const char * out2D_style;
  const char * out2D_vars;
  const char * out3D_dir;
  const char * out3D_style;
  const char * out3D_vars;
  const char * out_format;
  CCTK_INT out1D_d;
  CCTK_INT out1D_every;
  CCTK_INT out1D_x;
  CCTK_INT out1D_xline_yi;
  CCTK_INT out1D_xline_zi;
  CCTK_INT out1D_y;
  CCTK_INT out1D_yline_xi;
  CCTK_INT out1D_yline_zi;
  CCTK_INT out1D_z;
  CCTK_INT out1D_zline_xi;
  CCTK_INT out1D_zline_yi;
  CCTK_INT out2D_every;
  CCTK_INT out2D_xyplane_zi;
  CCTK_INT out2D_xzplane_yi;
  CCTK_INT out2D_yzplane_xi;
  CCTK_INT out3D_every;
} PRIVATE_IOASCII_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_IOASCII_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, out1D_xline_y, CCTK_PARAMETER__IOASCII__out1D_xline_y); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out1D_xline_z, CCTK_PARAMETER__IOASCII__out1D_xline_z); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out1D_yline_x, CCTK_PARAMETER__IOASCII__out1D_yline_x); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out1D_yline_z, CCTK_PARAMETER__IOASCII__out1D_yline_z); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out1D_zline_x, CCTK_PARAMETER__IOASCII__out1D_zline_x); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out1D_zline_y, CCTK_PARAMETER__IOASCII__out1D_zline_y); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out2D_xyplane_z, CCTK_PARAMETER__IOASCII__out2D_xyplane_z); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out2D_xzplane_y, CCTK_PARAMETER__IOASCII__out2D_xzplane_y); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out2D_yzplane_x, CCTK_PARAMETER__IOASCII__out2D_yzplane_x); \
  CCTK_DECLARE_INIT (const char * const, out1D_dir, CCTK_PARAMETER__IOASCII__out1D_dir); \
  CCTK_DECLARE_INIT (const char * const, out1D_style, CCTK_PARAMETER__IOASCII__out1D_style); \
  CCTK_DECLARE_INIT (const char * const, out1D_vars, CCTK_PARAMETER__IOASCII__out1D_vars); \
  CCTK_DECLARE_INIT (const char * const, out2D_dir, CCTK_PARAMETER__IOASCII__out2D_dir); \
  CCTK_DECLARE_INIT (const char * const, out2D_style, CCTK_PARAMETER__IOASCII__out2D_style); \
  CCTK_DECLARE_INIT (const char * const, out2D_vars, CCTK_PARAMETER__IOASCII__out2D_vars); \
  CCTK_DECLARE_INIT (const char * const, out3D_dir, CCTK_PARAMETER__IOASCII__out3D_dir); \
  CCTK_DECLARE_INIT (const char * const, out3D_style, CCTK_PARAMETER__IOASCII__out3D_style); \
  CCTK_DECLARE_INIT (const char * const, out3D_vars, CCTK_PARAMETER__IOASCII__out3D_vars); \
  CCTK_DECLARE_INIT (const char * const, out_format, CCTK_PARAMETER__IOASCII__out_format); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_d, CCTK_PARAMETER__IOASCII__out1D_d); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_every, CCTK_PARAMETER__IOASCII__out1D_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_x, CCTK_PARAMETER__IOASCII__out1D_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_xline_yi, CCTK_PARAMETER__IOASCII__out1D_xline_yi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_xline_zi, CCTK_PARAMETER__IOASCII__out1D_xline_zi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_y, CCTK_PARAMETER__IOASCII__out1D_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_yline_xi, CCTK_PARAMETER__IOASCII__out1D_yline_xi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_yline_zi, CCTK_PARAMETER__IOASCII__out1D_yline_zi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_z, CCTK_PARAMETER__IOASCII__out1D_z); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_zline_xi, CCTK_PARAMETER__IOASCII__out1D_zline_xi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out1D_zline_yi, CCTK_PARAMETER__IOASCII__out1D_zline_yi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out2D_every, CCTK_PARAMETER__IOASCII__out2D_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, out2D_xyplane_zi, CCTK_PARAMETER__IOASCII__out2D_xyplane_zi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out2D_xzplane_yi, CCTK_PARAMETER__IOASCII__out2D_xzplane_yi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out2D_yzplane_xi, CCTK_PARAMETER__IOASCII__out2D_yzplane_xi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out3D_every, CCTK_PARAMETER__IOASCII__out3D_every); \


#ifndef CCTK_PARAMETER__IOASCII__out1D_xline_y
#  define CCTK_PARAMETER__IOASCII__out1D_xline_y PRIVATE_IOASCII_STRUCT.out1D_xline_y
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_xline_z
#  define CCTK_PARAMETER__IOASCII__out1D_xline_z PRIVATE_IOASCII_STRUCT.out1D_xline_z
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_yline_x
#  define CCTK_PARAMETER__IOASCII__out1D_yline_x PRIVATE_IOASCII_STRUCT.out1D_yline_x
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_yline_z
#  define CCTK_PARAMETER__IOASCII__out1D_yline_z PRIVATE_IOASCII_STRUCT.out1D_yline_z
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_zline_x
#  define CCTK_PARAMETER__IOASCII__out1D_zline_x PRIVATE_IOASCII_STRUCT.out1D_zline_x
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_zline_y
#  define CCTK_PARAMETER__IOASCII__out1D_zline_y PRIVATE_IOASCII_STRUCT.out1D_zline_y
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_xyplane_z
#  define CCTK_PARAMETER__IOASCII__out2D_xyplane_z PRIVATE_IOASCII_STRUCT.out2D_xyplane_z
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_xzplane_y
#  define CCTK_PARAMETER__IOASCII__out2D_xzplane_y PRIVATE_IOASCII_STRUCT.out2D_xzplane_y
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_yzplane_x
#  define CCTK_PARAMETER__IOASCII__out2D_yzplane_x PRIVATE_IOASCII_STRUCT.out2D_yzplane_x
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_dir
#  define CCTK_PARAMETER__IOASCII__out1D_dir PRIVATE_IOASCII_STRUCT.out1D_dir
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_style
#  define CCTK_PARAMETER__IOASCII__out1D_style PRIVATE_IOASCII_STRUCT.out1D_style
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_vars
#  define CCTK_PARAMETER__IOASCII__out1D_vars PRIVATE_IOASCII_STRUCT.out1D_vars
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_dir
#  define CCTK_PARAMETER__IOASCII__out2D_dir PRIVATE_IOASCII_STRUCT.out2D_dir
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_style
#  define CCTK_PARAMETER__IOASCII__out2D_style PRIVATE_IOASCII_STRUCT.out2D_style
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_vars
#  define CCTK_PARAMETER__IOASCII__out2D_vars PRIVATE_IOASCII_STRUCT.out2D_vars
#endif
#ifndef CCTK_PARAMETER__IOASCII__out3D_dir
#  define CCTK_PARAMETER__IOASCII__out3D_dir PRIVATE_IOASCII_STRUCT.out3D_dir
#endif
#ifndef CCTK_PARAMETER__IOASCII__out3D_style
#  define CCTK_PARAMETER__IOASCII__out3D_style PRIVATE_IOASCII_STRUCT.out3D_style
#endif
#ifndef CCTK_PARAMETER__IOASCII__out3D_vars
#  define CCTK_PARAMETER__IOASCII__out3D_vars PRIVATE_IOASCII_STRUCT.out3D_vars
#endif
#ifndef CCTK_PARAMETER__IOASCII__out_format
#  define CCTK_PARAMETER__IOASCII__out_format PRIVATE_IOASCII_STRUCT.out_format
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_d
#  define CCTK_PARAMETER__IOASCII__out1D_d PRIVATE_IOASCII_STRUCT.out1D_d
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_every
#  define CCTK_PARAMETER__IOASCII__out1D_every PRIVATE_IOASCII_STRUCT.out1D_every
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_x
#  define CCTK_PARAMETER__IOASCII__out1D_x PRIVATE_IOASCII_STRUCT.out1D_x
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_xline_yi
#  define CCTK_PARAMETER__IOASCII__out1D_xline_yi PRIVATE_IOASCII_STRUCT.out1D_xline_yi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_xline_zi
#  define CCTK_PARAMETER__IOASCII__out1D_xline_zi PRIVATE_IOASCII_STRUCT.out1D_xline_zi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_y
#  define CCTK_PARAMETER__IOASCII__out1D_y PRIVATE_IOASCII_STRUCT.out1D_y
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_yline_xi
#  define CCTK_PARAMETER__IOASCII__out1D_yline_xi PRIVATE_IOASCII_STRUCT.out1D_yline_xi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_yline_zi
#  define CCTK_PARAMETER__IOASCII__out1D_yline_zi PRIVATE_IOASCII_STRUCT.out1D_yline_zi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_z
#  define CCTK_PARAMETER__IOASCII__out1D_z PRIVATE_IOASCII_STRUCT.out1D_z
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_zline_xi
#  define CCTK_PARAMETER__IOASCII__out1D_zline_xi PRIVATE_IOASCII_STRUCT.out1D_zline_xi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out1D_zline_yi
#  define CCTK_PARAMETER__IOASCII__out1D_zline_yi PRIVATE_IOASCII_STRUCT.out1D_zline_yi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_every
#  define CCTK_PARAMETER__IOASCII__out2D_every PRIVATE_IOASCII_STRUCT.out2D_every
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_xyplane_zi
#  define CCTK_PARAMETER__IOASCII__out2D_xyplane_zi PRIVATE_IOASCII_STRUCT.out2D_xyplane_zi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_xzplane_yi
#  define CCTK_PARAMETER__IOASCII__out2D_xzplane_yi PRIVATE_IOASCII_STRUCT.out2D_xzplane_yi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out2D_yzplane_xi
#  define CCTK_PARAMETER__IOASCII__out2D_yzplane_xi PRIVATE_IOASCII_STRUCT.out2D_yzplane_xi
#endif
#ifndef CCTK_PARAMETER__IOASCII__out3D_every
#  define CCTK_PARAMETER__IOASCII__out3D_every PRIVATE_IOASCII_STRUCT.out3D_every
#endif

