#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT one;
} PRIVATE_FORTRAN_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_FORTRAN_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, one, CCTK_PARAMETER__FORTRAN__one); \


#ifndef CCTK_PARAMETER__FORTRAN__one
#  define CCTK_PARAMETER__FORTRAN__one PRIVATE_FORTRAN_STRUCT.one
#endif

