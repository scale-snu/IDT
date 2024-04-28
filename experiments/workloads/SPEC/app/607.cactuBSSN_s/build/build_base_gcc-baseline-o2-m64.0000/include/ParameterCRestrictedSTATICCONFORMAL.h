#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * conformal_storage;
} RESTRICTED_STATICCONFORMAL_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_STATICCONFORMAL_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, conformal_storage, CCTK_PARAMETER__STATICCONFORMAL__conformal_storage); \


#ifndef CCTK_PARAMETER__STATICCONFORMAL__conformal_storage
#  define CCTK_PARAMETER__STATICCONFORMAL__conformal_storage RESTRICTED_STATICCONFORMAL_STRUCT.conformal_storage
#endif

