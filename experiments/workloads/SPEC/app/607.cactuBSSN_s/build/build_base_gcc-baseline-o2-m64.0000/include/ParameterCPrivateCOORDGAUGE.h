#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * slicing_verbose;
} PRIVATE_COORDGAUGE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_COORDGAUGE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, slicing_verbose, CCTK_PARAMETER__COORDGAUGE__slicing_verbose); \


#ifndef CCTK_PARAMETER__COORDGAUGE__slicing_verbose
#  define CCTK_PARAMETER__COORDGAUGE__slicing_verbose PRIVATE_COORDGAUGE_STRUCT.slicing_verbose
#endif

