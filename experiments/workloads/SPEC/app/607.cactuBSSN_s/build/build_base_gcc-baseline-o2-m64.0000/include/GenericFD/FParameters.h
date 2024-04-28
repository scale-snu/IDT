#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_STRING,jacobian_derivative_group,)&&\
CCTK_DECLARE(CCTK_STRING,jacobian_determinant_group,)&&\
CCTK_DECLARE(CCTK_STRING,jacobian_group,)&&\
CCTK_DECLARE(CCTK_STRING,jacobian_inverse_group,)&&\
CCTK_DECLARE(CCTK_INT,allocate_genericfd_jacobian,)&&\
CCTK_DECLARE(CCTK_INT,assume_stress_energy_state,)&&\
CCTK_DECLARE(CCTK_INT,assume_use_jacobian,)&&\
CCTK_DECLARE(CCTK_INT,boundary_width,)&&\
CCTK_DECLARE(CCTK_INT,jacobian_identity_map,)&&\
CCTK_DECLARE(CCTK_INT,stencil_width,)&&\
CCTK_DECLARE(CCTK_INT,stencil_width_x,)&&\
CCTK_DECLARE(CCTK_INT,stencil_width_y,)&&\
CCTK_DECLARE(CCTK_INT,stencil_width_z,)&&\
COMMON /GenericFDrest/jacobian_derivative_group, jacobian_determinant_group, jacobian_group, jacobian_inverse_group, allocate_genericfd_jacobian, assume_stress_energy_state, assume_use_jacobian, boundary_width, jacobian_identity_map, stencil_width, stencil_width_x, stencil_width_y, stencil_width_z&&\

