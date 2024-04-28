#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_REAL checkpoint_every_walltime_hours;
  CCTK_REAL out_dt;
  CCTK_REAL out_xline_y;
  CCTK_REAL out_xline_z;
  CCTK_REAL out_xyplane_z;
  CCTK_REAL out_xzplane_y;
  CCTK_REAL out_yline_x;
  CCTK_REAL out_yline_z;
  CCTK_REAL out_yzplane_x;
  CCTK_REAL out_zline_x;
  CCTK_REAL out_zline_y;
  const char * checkpoint_ID_file;
  const char * checkpoint_dir;
  const char * checkpoint_file;
  const char * filereader_ID_dir;
  const char * filereader_ID_files;
  const char * filereader_ID_vars;
  const char * out_criterion;
  const char * out_dir;
  const char * out_fileinfo;
  const char * out_mode;
  const char * out_save_parameters;
  const char * parfile_name;
  const char * parfile_write;
  const char * recover;
  const char * recover_dir;
  const char * recover_file;
  const char * verbose;
  CCTK_INT abort_on_io_errors;
  CCTK_INT checkpoint_ID;
  CCTK_INT checkpoint_every;
  CCTK_INT checkpoint_keep;
  CCTK_INT checkpoint_on_terminate;
  CCTK_INT new_filename_scheme;
  CCTK_INT out3D_septimefiles;
  CCTK_INT out_downsample_x;
  CCTK_INT out_downsample_y;
  CCTK_INT out_downsample_z;
  CCTK_INT out_every;
  CCTK_INT out_proc_every;
  CCTK_INT out_single_precision;
  CCTK_INT out_timesteps_per_file;
  CCTK_INT out_unchunked;
  CCTK_INT out_xline_yi;
  CCTK_INT out_xline_zi;
  CCTK_INT out_xyplane_zi;
  CCTK_INT out_xzplane_yi;
  CCTK_INT out_yline_xi;
  CCTK_INT out_yline_zi;
  CCTK_INT out_yzplane_xi;
  CCTK_INT out_zline_xi;
  CCTK_INT out_zline_yi;
  CCTK_INT parfile_update_every;
  CCTK_INT print_timing_info;
  CCTK_INT recover_and_remove;
  CCTK_INT require_empty_output_directory;
  CCTK_INT strict_io_parameter_check;
} RESTRICTED_IO_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_IO_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_REAL const, checkpoint_every_walltime_hours, CCTK_PARAMETER__IO__checkpoint_every_walltime_hours); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_dt, CCTK_PARAMETER__IO__out_dt); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_xline_y, CCTK_PARAMETER__IO__out_xline_y); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_xline_z, CCTK_PARAMETER__IO__out_xline_z); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_xyplane_z, CCTK_PARAMETER__IO__out_xyplane_z); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_xzplane_y, CCTK_PARAMETER__IO__out_xzplane_y); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_yline_x, CCTK_PARAMETER__IO__out_yline_x); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_yline_z, CCTK_PARAMETER__IO__out_yline_z); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_yzplane_x, CCTK_PARAMETER__IO__out_yzplane_x); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_zline_x, CCTK_PARAMETER__IO__out_zline_x); \
  CCTK_DECLARE_INIT (CCTK_REAL const, out_zline_y, CCTK_PARAMETER__IO__out_zline_y); \
  CCTK_DECLARE_INIT (const char * const, checkpoint_ID_file, CCTK_PARAMETER__IO__checkpoint_ID_file); \
  CCTK_DECLARE_INIT (const char * const, checkpoint_dir, CCTK_PARAMETER__IO__checkpoint_dir); \
  CCTK_DECLARE_INIT (const char * const, checkpoint_file, CCTK_PARAMETER__IO__checkpoint_file); \
  CCTK_DECLARE_INIT (const char * const, filereader_ID_dir, CCTK_PARAMETER__IO__filereader_ID_dir); \
  CCTK_DECLARE_INIT (const char * const, filereader_ID_files, CCTK_PARAMETER__IO__filereader_ID_files); \
  CCTK_DECLARE_INIT (const char * const, filereader_ID_vars, CCTK_PARAMETER__IO__filereader_ID_vars); \
  CCTK_DECLARE_INIT (const char * const, out_criterion, CCTK_PARAMETER__IO__out_criterion); \
  CCTK_DECLARE_INIT (const char * const, out_dir, CCTK_PARAMETER__IO__out_dir); \
  CCTK_DECLARE_INIT (const char * const, out_fileinfo, CCTK_PARAMETER__IO__out_fileinfo); \
  CCTK_DECLARE_INIT (const char * const, out_mode, CCTK_PARAMETER__IO__out_mode); \
  CCTK_DECLARE_INIT (const char * const, out_save_parameters, CCTK_PARAMETER__IO__out_save_parameters); \
  CCTK_DECLARE_INIT (const char * const, parfile_name, CCTK_PARAMETER__IO__parfile_name); \
  CCTK_DECLARE_INIT (const char * const, parfile_write, CCTK_PARAMETER__IO__parfile_write); \
  CCTK_DECLARE_INIT (const char * const, recover, CCTK_PARAMETER__IO__recover); \
  CCTK_DECLARE_INIT (const char * const, recover_dir, CCTK_PARAMETER__IO__recover_dir); \
  CCTK_DECLARE_INIT (const char * const, recover_file, CCTK_PARAMETER__IO__recover_file); \
  CCTK_DECLARE_INIT (const char * const, verbose, CCTK_PARAMETER__IO__verbose); \
  CCTK_DECLARE_INIT (CCTK_INT const, abort_on_io_errors, CCTK_PARAMETER__IO__abort_on_io_errors); \
  CCTK_DECLARE_INIT (CCTK_INT const, checkpoint_ID, CCTK_PARAMETER__IO__checkpoint_ID); \
  CCTK_DECLARE_INIT (CCTK_INT const, checkpoint_every, CCTK_PARAMETER__IO__checkpoint_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, checkpoint_keep, CCTK_PARAMETER__IO__checkpoint_keep); \
  CCTK_DECLARE_INIT (CCTK_INT const, checkpoint_on_terminate, CCTK_PARAMETER__IO__checkpoint_on_terminate); \
  CCTK_DECLARE_INIT (CCTK_INT const, new_filename_scheme, CCTK_PARAMETER__IO__new_filename_scheme); \
  CCTK_DECLARE_INIT (CCTK_INT const, out3D_septimefiles, CCTK_PARAMETER__IO__out3D_septimefiles); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_downsample_x, CCTK_PARAMETER__IO__out_downsample_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_downsample_y, CCTK_PARAMETER__IO__out_downsample_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_downsample_z, CCTK_PARAMETER__IO__out_downsample_z); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_every, CCTK_PARAMETER__IO__out_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_proc_every, CCTK_PARAMETER__IO__out_proc_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_single_precision, CCTK_PARAMETER__IO__out_single_precision); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_timesteps_per_file, CCTK_PARAMETER__IO__out_timesteps_per_file); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_unchunked, CCTK_PARAMETER__IO__out_unchunked); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_xline_yi, CCTK_PARAMETER__IO__out_xline_yi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_xline_zi, CCTK_PARAMETER__IO__out_xline_zi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_xyplane_zi, CCTK_PARAMETER__IO__out_xyplane_zi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_xzplane_yi, CCTK_PARAMETER__IO__out_xzplane_yi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_yline_xi, CCTK_PARAMETER__IO__out_yline_xi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_yline_zi, CCTK_PARAMETER__IO__out_yline_zi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_yzplane_xi, CCTK_PARAMETER__IO__out_yzplane_xi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_zline_xi, CCTK_PARAMETER__IO__out_zline_xi); \
  CCTK_DECLARE_INIT (CCTK_INT const, out_zline_yi, CCTK_PARAMETER__IO__out_zline_yi); \
  CCTK_DECLARE_INIT (CCTK_INT const, parfile_update_every, CCTK_PARAMETER__IO__parfile_update_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, print_timing_info, CCTK_PARAMETER__IO__print_timing_info); \
  CCTK_DECLARE_INIT (CCTK_INT const, recover_and_remove, CCTK_PARAMETER__IO__recover_and_remove); \
  CCTK_DECLARE_INIT (CCTK_INT const, require_empty_output_directory, CCTK_PARAMETER__IO__require_empty_output_directory); \
  CCTK_DECLARE_INIT (CCTK_INT const, strict_io_parameter_check, CCTK_PARAMETER__IO__strict_io_parameter_check); \


#ifndef CCTK_PARAMETER__IO__checkpoint_every_walltime_hours
#  define CCTK_PARAMETER__IO__checkpoint_every_walltime_hours RESTRICTED_IO_STRUCT.checkpoint_every_walltime_hours
#endif
#ifndef CCTK_PARAMETER__IO__out_dt
#  define CCTK_PARAMETER__IO__out_dt RESTRICTED_IO_STRUCT.out_dt
#endif
#ifndef CCTK_PARAMETER__IO__out_xline_y
#  define CCTK_PARAMETER__IO__out_xline_y RESTRICTED_IO_STRUCT.out_xline_y
#endif
#ifndef CCTK_PARAMETER__IO__out_xline_z
#  define CCTK_PARAMETER__IO__out_xline_z RESTRICTED_IO_STRUCT.out_xline_z
#endif
#ifndef CCTK_PARAMETER__IO__out_xyplane_z
#  define CCTK_PARAMETER__IO__out_xyplane_z RESTRICTED_IO_STRUCT.out_xyplane_z
#endif
#ifndef CCTK_PARAMETER__IO__out_xzplane_y
#  define CCTK_PARAMETER__IO__out_xzplane_y RESTRICTED_IO_STRUCT.out_xzplane_y
#endif
#ifndef CCTK_PARAMETER__IO__out_yline_x
#  define CCTK_PARAMETER__IO__out_yline_x RESTRICTED_IO_STRUCT.out_yline_x
#endif
#ifndef CCTK_PARAMETER__IO__out_yline_z
#  define CCTK_PARAMETER__IO__out_yline_z RESTRICTED_IO_STRUCT.out_yline_z
#endif
#ifndef CCTK_PARAMETER__IO__out_yzplane_x
#  define CCTK_PARAMETER__IO__out_yzplane_x RESTRICTED_IO_STRUCT.out_yzplane_x
#endif
#ifndef CCTK_PARAMETER__IO__out_zline_x
#  define CCTK_PARAMETER__IO__out_zline_x RESTRICTED_IO_STRUCT.out_zline_x
#endif
#ifndef CCTK_PARAMETER__IO__out_zline_y
#  define CCTK_PARAMETER__IO__out_zline_y RESTRICTED_IO_STRUCT.out_zline_y
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_ID_file
#  define CCTK_PARAMETER__IO__checkpoint_ID_file RESTRICTED_IO_STRUCT.checkpoint_ID_file
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_dir
#  define CCTK_PARAMETER__IO__checkpoint_dir RESTRICTED_IO_STRUCT.checkpoint_dir
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_file
#  define CCTK_PARAMETER__IO__checkpoint_file RESTRICTED_IO_STRUCT.checkpoint_file
#endif
#ifndef CCTK_PARAMETER__IO__filereader_ID_dir
#  define CCTK_PARAMETER__IO__filereader_ID_dir RESTRICTED_IO_STRUCT.filereader_ID_dir
#endif
#ifndef CCTK_PARAMETER__IO__filereader_ID_files
#  define CCTK_PARAMETER__IO__filereader_ID_files RESTRICTED_IO_STRUCT.filereader_ID_files
#endif
#ifndef CCTK_PARAMETER__IO__filereader_ID_vars
#  define CCTK_PARAMETER__IO__filereader_ID_vars RESTRICTED_IO_STRUCT.filereader_ID_vars
#endif
#ifndef CCTK_PARAMETER__IO__out_criterion
#  define CCTK_PARAMETER__IO__out_criterion RESTRICTED_IO_STRUCT.out_criterion
#endif
#ifndef CCTK_PARAMETER__IO__out_dir
#  define CCTK_PARAMETER__IO__out_dir RESTRICTED_IO_STRUCT.out_dir
#endif
#ifndef CCTK_PARAMETER__IO__out_fileinfo
#  define CCTK_PARAMETER__IO__out_fileinfo RESTRICTED_IO_STRUCT.out_fileinfo
#endif
#ifndef CCTK_PARAMETER__IO__out_mode
#  define CCTK_PARAMETER__IO__out_mode RESTRICTED_IO_STRUCT.out_mode
#endif
#ifndef CCTK_PARAMETER__IO__out_save_parameters
#  define CCTK_PARAMETER__IO__out_save_parameters RESTRICTED_IO_STRUCT.out_save_parameters
#endif
#ifndef CCTK_PARAMETER__IO__parfile_name
#  define CCTK_PARAMETER__IO__parfile_name RESTRICTED_IO_STRUCT.parfile_name
#endif
#ifndef CCTK_PARAMETER__IO__parfile_write
#  define CCTK_PARAMETER__IO__parfile_write RESTRICTED_IO_STRUCT.parfile_write
#endif
#ifndef CCTK_PARAMETER__IO__recover
#  define CCTK_PARAMETER__IO__recover RESTRICTED_IO_STRUCT.recover
#endif
#ifndef CCTK_PARAMETER__IO__recover_dir
#  define CCTK_PARAMETER__IO__recover_dir RESTRICTED_IO_STRUCT.recover_dir
#endif
#ifndef CCTK_PARAMETER__IO__recover_file
#  define CCTK_PARAMETER__IO__recover_file RESTRICTED_IO_STRUCT.recover_file
#endif
#ifndef CCTK_PARAMETER__IO__verbose
#  define CCTK_PARAMETER__IO__verbose RESTRICTED_IO_STRUCT.verbose
#endif
#ifndef CCTK_PARAMETER__IO__abort_on_io_errors
#  define CCTK_PARAMETER__IO__abort_on_io_errors RESTRICTED_IO_STRUCT.abort_on_io_errors
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_ID
#  define CCTK_PARAMETER__IO__checkpoint_ID RESTRICTED_IO_STRUCT.checkpoint_ID
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_every
#  define CCTK_PARAMETER__IO__checkpoint_every RESTRICTED_IO_STRUCT.checkpoint_every
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_keep
#  define CCTK_PARAMETER__IO__checkpoint_keep RESTRICTED_IO_STRUCT.checkpoint_keep
#endif
#ifndef CCTK_PARAMETER__IO__checkpoint_on_terminate
#  define CCTK_PARAMETER__IO__checkpoint_on_terminate RESTRICTED_IO_STRUCT.checkpoint_on_terminate
#endif
#ifndef CCTK_PARAMETER__IO__new_filename_scheme
#  define CCTK_PARAMETER__IO__new_filename_scheme RESTRICTED_IO_STRUCT.new_filename_scheme
#endif
#ifndef CCTK_PARAMETER__IO__out3D_septimefiles
#  define CCTK_PARAMETER__IO__out3D_septimefiles RESTRICTED_IO_STRUCT.out3D_septimefiles
#endif
#ifndef CCTK_PARAMETER__IO__out_downsample_x
#  define CCTK_PARAMETER__IO__out_downsample_x RESTRICTED_IO_STRUCT.out_downsample_x
#endif
#ifndef CCTK_PARAMETER__IO__out_downsample_y
#  define CCTK_PARAMETER__IO__out_downsample_y RESTRICTED_IO_STRUCT.out_downsample_y
#endif
#ifndef CCTK_PARAMETER__IO__out_downsample_z
#  define CCTK_PARAMETER__IO__out_downsample_z RESTRICTED_IO_STRUCT.out_downsample_z
#endif
#ifndef CCTK_PARAMETER__IO__out_every
#  define CCTK_PARAMETER__IO__out_every RESTRICTED_IO_STRUCT.out_every
#endif
#ifndef CCTK_PARAMETER__IO__out_proc_every
#  define CCTK_PARAMETER__IO__out_proc_every RESTRICTED_IO_STRUCT.out_proc_every
#endif
#ifndef CCTK_PARAMETER__IO__out_single_precision
#  define CCTK_PARAMETER__IO__out_single_precision RESTRICTED_IO_STRUCT.out_single_precision
#endif
#ifndef CCTK_PARAMETER__IO__out_timesteps_per_file
#  define CCTK_PARAMETER__IO__out_timesteps_per_file RESTRICTED_IO_STRUCT.out_timesteps_per_file
#endif
#ifndef CCTK_PARAMETER__IO__out_unchunked
#  define CCTK_PARAMETER__IO__out_unchunked RESTRICTED_IO_STRUCT.out_unchunked
#endif
#ifndef CCTK_PARAMETER__IO__out_xline_yi
#  define CCTK_PARAMETER__IO__out_xline_yi RESTRICTED_IO_STRUCT.out_xline_yi
#endif
#ifndef CCTK_PARAMETER__IO__out_xline_zi
#  define CCTK_PARAMETER__IO__out_xline_zi RESTRICTED_IO_STRUCT.out_xline_zi
#endif
#ifndef CCTK_PARAMETER__IO__out_xyplane_zi
#  define CCTK_PARAMETER__IO__out_xyplane_zi RESTRICTED_IO_STRUCT.out_xyplane_zi
#endif
#ifndef CCTK_PARAMETER__IO__out_xzplane_yi
#  define CCTK_PARAMETER__IO__out_xzplane_yi RESTRICTED_IO_STRUCT.out_xzplane_yi
#endif
#ifndef CCTK_PARAMETER__IO__out_yline_xi
#  define CCTK_PARAMETER__IO__out_yline_xi RESTRICTED_IO_STRUCT.out_yline_xi
#endif
#ifndef CCTK_PARAMETER__IO__out_yline_zi
#  define CCTK_PARAMETER__IO__out_yline_zi RESTRICTED_IO_STRUCT.out_yline_zi
#endif
#ifndef CCTK_PARAMETER__IO__out_yzplane_xi
#  define CCTK_PARAMETER__IO__out_yzplane_xi RESTRICTED_IO_STRUCT.out_yzplane_xi
#endif
#ifndef CCTK_PARAMETER__IO__out_zline_xi
#  define CCTK_PARAMETER__IO__out_zline_xi RESTRICTED_IO_STRUCT.out_zline_xi
#endif
#ifndef CCTK_PARAMETER__IO__out_zline_yi
#  define CCTK_PARAMETER__IO__out_zline_yi RESTRICTED_IO_STRUCT.out_zline_yi
#endif
#ifndef CCTK_PARAMETER__IO__parfile_update_every
#  define CCTK_PARAMETER__IO__parfile_update_every RESTRICTED_IO_STRUCT.parfile_update_every
#endif
#ifndef CCTK_PARAMETER__IO__print_timing_info
#  define CCTK_PARAMETER__IO__print_timing_info RESTRICTED_IO_STRUCT.print_timing_info
#endif
#ifndef CCTK_PARAMETER__IO__recover_and_remove
#  define CCTK_PARAMETER__IO__recover_and_remove RESTRICTED_IO_STRUCT.recover_and_remove
#endif
#ifndef CCTK_PARAMETER__IO__require_empty_output_directory
#  define CCTK_PARAMETER__IO__require_empty_output_directory RESTRICTED_IO_STRUCT.require_empty_output_directory
#endif
#ifndef CCTK_PARAMETER__IO__strict_io_parameter_check
#  define CCTK_PARAMETER__IO__strict_io_parameter_check RESTRICTED_IO_STRUCT.strict_io_parameter_check
#endif

