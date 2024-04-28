#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT verbose;
} PRIVATE_SYMBASE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_SYMBASE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, verbose, CCTK_PARAMETER__SYMBASE__verbose); \


#ifndef CCTK_PARAMETER__SYMBASE__verbose
#  define CCTK_PARAMETER__SYMBASE__verbose PRIVATE_SYMBASE_STRUCT.verbose
#endif

