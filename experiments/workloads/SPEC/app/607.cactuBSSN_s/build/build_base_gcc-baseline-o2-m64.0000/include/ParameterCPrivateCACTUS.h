#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * cctk_run_title;
  const char * cctk_timer_output;
  const char * info_format;
  const char * recovery_mode;
  const char * schedule_sort_mode;
  CCTK_INT allow_mixeddim_gfs;
  CCTK_INT cctk_brief_output;
  CCTK_INT cctk_full_warnings;
  CCTK_INT cctk_show_banners;
  CCTK_INT cctk_show_schedule;
  CCTK_INT cctk_strong_param_check;
  CCTK_INT highlight_warning_messages;
  CCTK_INT manual_cache_setup;
  CCTK_INT manual_cache_size;
  CCTK_INT manual_cacheline_bytes;
  CCTK_INT schedule_sort_warnings;
} PRIVATE_CACTUS_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_CACTUS_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, cctk_run_title, CCTK_PARAMETER__CACTUS__cctk_run_title); \
  CCTK_DECLARE_INIT (const char * const, cctk_timer_output, CCTK_PARAMETER__CACTUS__cctk_timer_output); \
  CCTK_DECLARE_INIT (const char * const, info_format, CCTK_PARAMETER__CACTUS__info_format); \
  CCTK_DECLARE_INIT (const char * const, recovery_mode, CCTK_PARAMETER__CACTUS__recovery_mode); \
  CCTK_DECLARE_INIT (const char * const, schedule_sort_mode, CCTK_PARAMETER__CACTUS__schedule_sort_mode); \
  CCTK_DECLARE_INIT (CCTK_INT const, allow_mixeddim_gfs, CCTK_PARAMETER__CACTUS__allow_mixeddim_gfs); \
  CCTK_DECLARE_INIT (CCTK_INT const, cctk_brief_output, CCTK_PARAMETER__CACTUS__cctk_brief_output); \
  CCTK_DECLARE_INIT (CCTK_INT const, cctk_full_warnings, CCTK_PARAMETER__CACTUS__cctk_full_warnings); \
  CCTK_DECLARE_INIT (CCTK_INT const, cctk_show_banners, CCTK_PARAMETER__CACTUS__cctk_show_banners); \
  CCTK_DECLARE_INIT (CCTK_INT const, cctk_show_schedule, CCTK_PARAMETER__CACTUS__cctk_show_schedule); \
  CCTK_DECLARE_INIT (CCTK_INT const, cctk_strong_param_check, CCTK_PARAMETER__CACTUS__cctk_strong_param_check); \
  CCTK_DECLARE_INIT (CCTK_INT const, highlight_warning_messages, CCTK_PARAMETER__CACTUS__highlight_warning_messages); \
  CCTK_DECLARE_INIT (CCTK_INT const, manual_cache_setup, CCTK_PARAMETER__CACTUS__manual_cache_setup); \
  CCTK_DECLARE_INIT (CCTK_INT const, manual_cache_size, CCTK_PARAMETER__CACTUS__manual_cache_size); \
  CCTK_DECLARE_INIT (CCTK_INT const, manual_cacheline_bytes, CCTK_PARAMETER__CACTUS__manual_cacheline_bytes); \
  CCTK_DECLARE_INIT (CCTK_INT const, schedule_sort_warnings, CCTK_PARAMETER__CACTUS__schedule_sort_warnings); \


#ifndef CCTK_PARAMETER__CACTUS__cctk_run_title
#  define CCTK_PARAMETER__CACTUS__cctk_run_title PRIVATE_CACTUS_STRUCT.cctk_run_title
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_timer_output
#  define CCTK_PARAMETER__CACTUS__cctk_timer_output PRIVATE_CACTUS_STRUCT.cctk_timer_output
#endif
#ifndef CCTK_PARAMETER__CACTUS__info_format
#  define CCTK_PARAMETER__CACTUS__info_format PRIVATE_CACTUS_STRUCT.info_format
#endif
#ifndef CCTK_PARAMETER__CACTUS__recovery_mode
#  define CCTK_PARAMETER__CACTUS__recovery_mode PRIVATE_CACTUS_STRUCT.recovery_mode
#endif
#ifndef CCTK_PARAMETER__CACTUS__schedule_sort_mode
#  define CCTK_PARAMETER__CACTUS__schedule_sort_mode PRIVATE_CACTUS_STRUCT.schedule_sort_mode
#endif
#ifndef CCTK_PARAMETER__CACTUS__allow_mixeddim_gfs
#  define CCTK_PARAMETER__CACTUS__allow_mixeddim_gfs PRIVATE_CACTUS_STRUCT.allow_mixeddim_gfs
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_brief_output
#  define CCTK_PARAMETER__CACTUS__cctk_brief_output PRIVATE_CACTUS_STRUCT.cctk_brief_output
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_full_warnings
#  define CCTK_PARAMETER__CACTUS__cctk_full_warnings PRIVATE_CACTUS_STRUCT.cctk_full_warnings
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_show_banners
#  define CCTK_PARAMETER__CACTUS__cctk_show_banners PRIVATE_CACTUS_STRUCT.cctk_show_banners
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_show_schedule
#  define CCTK_PARAMETER__CACTUS__cctk_show_schedule PRIVATE_CACTUS_STRUCT.cctk_show_schedule
#endif
#ifndef CCTK_PARAMETER__CACTUS__cctk_strong_param_check
#  define CCTK_PARAMETER__CACTUS__cctk_strong_param_check PRIVATE_CACTUS_STRUCT.cctk_strong_param_check
#endif
#ifndef CCTK_PARAMETER__CACTUS__highlight_warning_messages
#  define CCTK_PARAMETER__CACTUS__highlight_warning_messages PRIVATE_CACTUS_STRUCT.highlight_warning_messages
#endif
#ifndef CCTK_PARAMETER__CACTUS__manual_cache_setup
#  define CCTK_PARAMETER__CACTUS__manual_cache_setup PRIVATE_CACTUS_STRUCT.manual_cache_setup
#endif
#ifndef CCTK_PARAMETER__CACTUS__manual_cache_size
#  define CCTK_PARAMETER__CACTUS__manual_cache_size PRIVATE_CACTUS_STRUCT.manual_cache_size
#endif
#ifndef CCTK_PARAMETER__CACTUS__manual_cacheline_bytes
#  define CCTK_PARAMETER__CACTUS__manual_cacheline_bytes PRIVATE_CACTUS_STRUCT.manual_cacheline_bytes
#endif
#ifndef CCTK_PARAMETER__CACTUS__schedule_sort_warnings
#  define CCTK_PARAMETER__CACTUS__schedule_sort_warnings PRIVATE_CACTUS_STRUCT.schedule_sort_warnings
#endif

