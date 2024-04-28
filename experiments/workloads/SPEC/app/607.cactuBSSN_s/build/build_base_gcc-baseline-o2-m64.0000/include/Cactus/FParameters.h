#define DECLARE_CCTK_PARAMETERS \
CCTK_DECLARE(CCTK_REAL,cctk_final_time,)&&\
CCTK_DECLARE(CCTK_REAL,cctk_initial_time,)&&\
CCTK_DECLARE(CCTK_REAL,max_runtime,)&&\
CCTK_DECLARE(CCTK_STRING,terminate,)&&\
CCTK_DECLARE(CCTK_INT,cctk_itlast,)&&\
CCTK_DECLARE(CCTK_INT,terminate_next,)&&\
COMMON /Cactusrest/cctk_final_time, cctk_initial_time, max_runtime, terminate, cctk_itlast, terminate_next&&\
CCTK_DECLARE(CCTK_STRING,cctk_run_title,)&&\
CCTK_DECLARE(CCTK_STRING,cctk_timer_output,)&&\
CCTK_DECLARE(CCTK_STRING,info_format,)&&\
CCTK_DECLARE(CCTK_STRING,recovery_mode,)&&\
CCTK_DECLARE(CCTK_STRING,schedule_sort_mode,)&&\
CCTK_DECLARE(CCTK_INT,allow_mixeddim_gfs,)&&\
CCTK_DECLARE(CCTK_INT,cctk_brief_output,)&&\
CCTK_DECLARE(CCTK_INT,cctk_full_warnings,)&&\
CCTK_DECLARE(CCTK_INT,cctk_show_banners,)&&\
CCTK_DECLARE(CCTK_INT,cctk_show_schedule,)&&\
CCTK_DECLARE(CCTK_INT,cctk_strong_param_check,)&&\
CCTK_DECLARE(CCTK_INT,highlight_warning_messages,)&&\
CCTK_DECLARE(CCTK_INT,manual_cache_setup,)&&\
CCTK_DECLARE(CCTK_INT,manual_cache_size,)&&\
CCTK_DECLARE(CCTK_INT,manual_cacheline_bytes,)&&\
CCTK_DECLARE(CCTK_INT,schedule_sort_warnings,)&&\
COMMON /Cactuspriv/cctk_run_title, cctk_timer_output, info_format, recovery_mode, schedule_sort_mode, allow_mixeddim_gfs, cctk_brief_output, cctk_full_warnings, cctk_show_banners, cctk_show_schedule, cctk_strong_param_check, highlight_warning_messages, manual_cache_setup, manual_cache_size, manual_cacheline_bytes, schedule_sort_warnings&&\

