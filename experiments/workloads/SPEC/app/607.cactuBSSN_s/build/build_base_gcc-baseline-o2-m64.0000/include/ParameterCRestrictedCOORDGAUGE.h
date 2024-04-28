#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * lapse_list;
  const char * shift_list;
} RESTRICTED_COORDGAUGE_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_COORDGAUGE_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, lapse_list, CCTK_PARAMETER__COORDGAUGE__lapse_list); \
  CCTK_DECLARE_INIT (const char * const, shift_list, CCTK_PARAMETER__COORDGAUGE__shift_list); \


#ifndef CCTK_PARAMETER__COORDGAUGE__lapse_list
#  define CCTK_PARAMETER__COORDGAUGE__lapse_list RESTRICTED_COORDGAUGE_STRUCT.lapse_list
#endif
#ifndef CCTK_PARAMETER__COORDGAUGE__shift_list
#  define CCTK_PARAMETER__COORDGAUGE__shift_list RESTRICTED_COORDGAUGE_STRUCT.shift_list
#endif

