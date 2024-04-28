#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_REAL,dx,)&&\
CCTK_DECLARE(CCTK_REAL,dxyz,)&&\
CCTK_DECLARE(CCTK_REAL,dy,)&&\
CCTK_DECLARE(CCTK_REAL,dz,)&&\
CCTK_DECLARE(CCTK_REAL,xmax,)&&\
CCTK_DECLARE(CCTK_REAL,xmin,)&&\
CCTK_DECLARE(CCTK_REAL,xyzmax,)&&\
CCTK_DECLARE(CCTK_REAL,xyzmin,)&&\
CCTK_DECLARE(CCTK_REAL,ymax,)&&\
CCTK_DECLARE(CCTK_REAL,ymin,)&&\
CCTK_DECLARE(CCTK_REAL,zmax,)&&\
CCTK_DECLARE(CCTK_REAL,zmin,)&&\
CCTK_DECLARE(CCTK_STRING,bitant_plane,)&&\
CCTK_DECLARE(CCTK_STRING,domain,)&&\
CCTK_DECLARE(CCTK_STRING,quadrant_direction,)&&\
CCTK_DECLARE(CCTK_STRING,rotation_axis,)&&\
CCTK_DECLARE(CCTK_STRING,type,)&&\
CCTK_DECLARE(CCTK_INT,symmetry_xmax,)&&\
CCTK_DECLARE(CCTK_INT,symmetry_xmin,)&&\
CCTK_DECLARE(CCTK_INT,symmetry_ymax,)&&\
CCTK_DECLARE(CCTK_INT,symmetry_ymin,)&&\
CCTK_DECLARE(CCTK_INT,symmetry_zmax,)&&\
CCTK_DECLARE(CCTK_INT,symmetry_zmin,)&&\
COMMON /gridrest/dx, dxyz, dy, dz, xmax, xmin, xyzmax, xyzmin, ymax, ymin, zmax, zmin, bitant_plane, domain, quadrant_direction, rotation_axis, type, symmetry_xmax, symmetry_xmin, symmetry_ymax, symmetry_ymin, symmetry_zmax, symmetry_zmin&&\
CCTK_DECLARE(CCTK_STRING,set_coordinate_ranges_on,)&&\
CCTK_DECLARE(CCTK_INT,avoid_origin,)&&\
CCTK_DECLARE(CCTK_INT,avoid_originx,)&&\
CCTK_DECLARE(CCTK_INT,avoid_originy,)&&\
CCTK_DECLARE(CCTK_INT,avoid_originz,)&&\
CCTK_DECLARE(CCTK_INT,no_origin,)&&\
CCTK_DECLARE(CCTK_INT,no_originx,)&&\
CCTK_DECLARE(CCTK_INT,no_originy,)&&\
CCTK_DECLARE(CCTK_INT,no_originz,)&&\
CCTK_DECLARE(CCTK_INT,register_default_coordinate_systems,)&&\
COMMON /CartGrid3Dpriv/set_coordinate_ranges_on, avoid_origin, avoid_originx, avoid_originy, avoid_originz, no_origin, no_originx, no_originy, no_originz, register_default_coordinate_systems&&\
CCTK_DECLARE(CCTK_INT,periodic,)&&\
CCTK_DECLARE(CCTK_INT,periodic_x,)&&\
CCTK_DECLARE(CCTK_INT,periodic_y,)&&\
CCTK_DECLARE(CCTK_INT,periodic_z,)&&\
COMMON /DRIVERrest/periodic, periodic_x, periodic_y, periodic_z&&\

