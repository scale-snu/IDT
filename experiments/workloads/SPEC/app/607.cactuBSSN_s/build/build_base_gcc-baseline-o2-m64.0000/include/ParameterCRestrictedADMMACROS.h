#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT spatial_order;
} RESTRICTED_ADMMACROS_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_ADMMACROS_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, spatial_order, CCTK_PARAMETER__ADMMACROS__spatial_order); \


#ifndef CCTK_PARAMETER__ADMMACROS__spatial_order
#  define CCTK_PARAMETER__ADMMACROS__spatial_order RESTRICTED_ADMMACROS_STRUCT.spatial_order
#endif

