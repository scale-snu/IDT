#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT truncate_files;
  CCTK_INT truncate_files_after_recovering;
} PRIVATE_IOUTIL_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_IOUTIL_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, truncate_files, CCTK_PARAMETER__IOUTIL__truncate_files); \
  CCTK_DECLARE_INIT (CCTK_INT const, truncate_files_after_recovering, CCTK_PARAMETER__IOUTIL__truncate_files_after_recovering); \


#ifndef CCTK_PARAMETER__IOUTIL__truncate_files
#  define CCTK_PARAMETER__IOUTIL__truncate_files PRIVATE_IOUTIL_STRUCT.truncate_files
#endif
#ifndef CCTK_PARAMETER__IOUTIL__truncate_files_after_recovering
#  define CCTK_PARAMETER__IOUTIL__truncate_files_after_recovering PRIVATE_IOUTIL_STRUCT.truncate_files_after_recovering
#endif

