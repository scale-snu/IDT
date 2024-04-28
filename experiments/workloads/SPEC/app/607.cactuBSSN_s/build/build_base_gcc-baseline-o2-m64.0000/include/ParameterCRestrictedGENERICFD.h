#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * jacobian_derivative_group;
  const char * jacobian_determinant_group;
  const char * jacobian_group;
  const char * jacobian_inverse_group;
  CCTK_INT allocate_genericfd_jacobian;
  CCTK_INT assume_stress_energy_state;
  CCTK_INT assume_use_jacobian;
  CCTK_INT boundary_width;
  CCTK_INT jacobian_identity_map;
  CCTK_INT stencil_width;
  CCTK_INT stencil_width_x;
  CCTK_INT stencil_width_y;
  CCTK_INT stencil_width_z;
} RESTRICTED_GENERICFD_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_GENERICFD_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, jacobian_derivative_group, CCTK_PARAMETER__GENERICFD__jacobian_derivative_group); \
  CCTK_DECLARE_INIT (const char * const, jacobian_determinant_group, CCTK_PARAMETER__GENERICFD__jacobian_determinant_group); \
  CCTK_DECLARE_INIT (const char * const, jacobian_group, CCTK_PARAMETER__GENERICFD__jacobian_group); \
  CCTK_DECLARE_INIT (const char * const, jacobian_inverse_group, CCTK_PARAMETER__GENERICFD__jacobian_inverse_group); \
  CCTK_DECLARE_INIT (CCTK_INT const, allocate_genericfd_jacobian, CCTK_PARAMETER__GENERICFD__allocate_genericfd_jacobian); \
  CCTK_DECLARE_INIT (CCTK_INT const, assume_stress_energy_state, CCTK_PARAMETER__GENERICFD__assume_stress_energy_state); \
  CCTK_DECLARE_INIT (CCTK_INT const, assume_use_jacobian, CCTK_PARAMETER__GENERICFD__assume_use_jacobian); \
  CCTK_DECLARE_INIT (CCTK_INT const, boundary_width, CCTK_PARAMETER__GENERICFD__boundary_width); \
  CCTK_DECLARE_INIT (CCTK_INT const, jacobian_identity_map, CCTK_PARAMETER__GENERICFD__jacobian_identity_map); \
  CCTK_DECLARE_INIT (CCTK_INT const, stencil_width, CCTK_PARAMETER__GENERICFD__stencil_width); \
  CCTK_DECLARE_INIT (CCTK_INT const, stencil_width_x, CCTK_PARAMETER__GENERICFD__stencil_width_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, stencil_width_y, CCTK_PARAMETER__GENERICFD__stencil_width_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, stencil_width_z, CCTK_PARAMETER__GENERICFD__stencil_width_z); \


#ifndef CCTK_PARAMETER__GENERICFD__jacobian_derivative_group
#  define CCTK_PARAMETER__GENERICFD__jacobian_derivative_group RESTRICTED_GENERICFD_STRUCT.jacobian_derivative_group
#endif
#ifndef CCTK_PARAMETER__GENERICFD__jacobian_determinant_group
#  define CCTK_PARAMETER__GENERICFD__jacobian_determinant_group RESTRICTED_GENERICFD_STRUCT.jacobian_determinant_group
#endif
#ifndef CCTK_PARAMETER__GENERICFD__jacobian_group
#  define CCTK_PARAMETER__GENERICFD__jacobian_group RESTRICTED_GENERICFD_STRUCT.jacobian_group
#endif
#ifndef CCTK_PARAMETER__GENERICFD__jacobian_inverse_group
#  define CCTK_PARAMETER__GENERICFD__jacobian_inverse_group RESTRICTED_GENERICFD_STRUCT.jacobian_inverse_group
#endif
#ifndef CCTK_PARAMETER__GENERICFD__allocate_genericfd_jacobian
#  define CCTK_PARAMETER__GENERICFD__allocate_genericfd_jacobian RESTRICTED_GENERICFD_STRUCT.allocate_genericfd_jacobian
#endif
#ifndef CCTK_PARAMETER__GENERICFD__assume_stress_energy_state
#  define CCTK_PARAMETER__GENERICFD__assume_stress_energy_state RESTRICTED_GENERICFD_STRUCT.assume_stress_energy_state
#endif
#ifndef CCTK_PARAMETER__GENERICFD__assume_use_jacobian
#  define CCTK_PARAMETER__GENERICFD__assume_use_jacobian RESTRICTED_GENERICFD_STRUCT.assume_use_jacobian
#endif
#ifndef CCTK_PARAMETER__GENERICFD__boundary_width
#  define CCTK_PARAMETER__GENERICFD__boundary_width RESTRICTED_GENERICFD_STRUCT.boundary_width
#endif
#ifndef CCTK_PARAMETER__GENERICFD__jacobian_identity_map
#  define CCTK_PARAMETER__GENERICFD__jacobian_identity_map RESTRICTED_GENERICFD_STRUCT.jacobian_identity_map
#endif
#ifndef CCTK_PARAMETER__GENERICFD__stencil_width
#  define CCTK_PARAMETER__GENERICFD__stencil_width RESTRICTED_GENERICFD_STRUCT.stencil_width
#endif
#ifndef CCTK_PARAMETER__GENERICFD__stencil_width_x
#  define CCTK_PARAMETER__GENERICFD__stencil_width_x RESTRICTED_GENERICFD_STRUCT.stencil_width_x
#endif
#ifndef CCTK_PARAMETER__GENERICFD__stencil_width_y
#  define CCTK_PARAMETER__GENERICFD__stencil_width_y RESTRICTED_GENERICFD_STRUCT.stencil_width_y
#endif
#ifndef CCTK_PARAMETER__GENERICFD__stencil_width_z
#  define CCTK_PARAMETER__GENERICFD__stencil_width_z RESTRICTED_GENERICFD_STRUCT.stencil_width_z
#endif

