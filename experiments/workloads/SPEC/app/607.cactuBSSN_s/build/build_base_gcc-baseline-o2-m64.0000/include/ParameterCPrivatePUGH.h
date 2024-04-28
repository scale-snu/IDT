#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  const char * info;
  const char * initialize_memory;
  const char * partition;
  const char * partition_1d_x;
  const char * partition_2d_x;
  const char * partition_2d_y;
  const char * partition_3d_x;
  const char * partition_3d_y;
  const char * partition_3d_z;
  const char * physical2logical;
  const char * processor_topology;
  const char * storage_verbose;
  CCTK_INT cacheline_mult;
  CCTK_INT enable_all_storage;
  CCTK_INT ghost_size;
  CCTK_INT ghost_size_x;
  CCTK_INT ghost_size_y;
  CCTK_INT ghost_size_z;
  CCTK_INT global_nsize;
  CCTK_INT global_nx;
  CCTK_INT global_ny;
  CCTK_INT global_nz;
  CCTK_INT local_nsize;
  CCTK_INT local_nx;
  CCTK_INT local_ny;
  CCTK_INT local_nz;
  CCTK_INT local_size_includes_ghosts;
  CCTK_INT overloadabort;
  CCTK_INT overloadarraygroupsizeb;
  CCTK_INT overloadbarrier;
  CCTK_INT overloaddisablegroupcomm;
  CCTK_INT overloaddisablegroupstorage;
  CCTK_INT overloadenablegroupcomm;
  CCTK_INT overloadenablegroupstorage;
  CCTK_INT overloadevolve;
  CCTK_INT overloadexit;
  CCTK_INT overloadgroupdynamicdata;
  CCTK_INT overloadmyproc;
  CCTK_INT overloadnprocs;
  CCTK_INT overloadparallelinit;
  CCTK_INT overloadquerygroupstorageb;
  CCTK_INT overloadsyncgroup;
  CCTK_INT overloadsyncgroupsbydiri;
  CCTK_INT padding_active;
  CCTK_INT padding_address_spacing;
  CCTK_INT padding_cacheline_bits;
  CCTK_INT padding_size;
  CCTK_INT processor_topology_1d_x;
  CCTK_INT processor_topology_2d_x;
  CCTK_INT processor_topology_2d_y;
  CCTK_INT processor_topology_3d_x;
  CCTK_INT processor_topology_3d_y;
  CCTK_INT processor_topology_3d_z;
  CCTK_INT storage_report_every;
  CCTK_INT timer_output;
} PRIVATE_PUGH_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_PRIVATE_PUGH_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (const char * const, info, CCTK_PARAMETER__PUGH__info); \
  CCTK_DECLARE_INIT (const char * const, initialize_memory, CCTK_PARAMETER__PUGH__initialize_memory); \
  CCTK_DECLARE_INIT (const char * const, partition, CCTK_PARAMETER__PUGH__partition); \
  CCTK_DECLARE_INIT (const char * const, partition_1d_x, CCTK_PARAMETER__PUGH__partition_1d_x); \
  CCTK_DECLARE_INIT (const char * const, partition_2d_x, CCTK_PARAMETER__PUGH__partition_2d_x); \
  CCTK_DECLARE_INIT (const char * const, partition_2d_y, CCTK_PARAMETER__PUGH__partition_2d_y); \
  CCTK_DECLARE_INIT (const char * const, partition_3d_x, CCTK_PARAMETER__PUGH__partition_3d_x); \
  CCTK_DECLARE_INIT (const char * const, partition_3d_y, CCTK_PARAMETER__PUGH__partition_3d_y); \
  CCTK_DECLARE_INIT (const char * const, partition_3d_z, CCTK_PARAMETER__PUGH__partition_3d_z); \
  CCTK_DECLARE_INIT (const char * const, physical2logical, CCTK_PARAMETER__PUGH__physical2logical); \
  CCTK_DECLARE_INIT (const char * const, processor_topology, CCTK_PARAMETER__PUGH__processor_topology); \
  CCTK_DECLARE_INIT (const char * const, storage_verbose, CCTK_PARAMETER__PUGH__storage_verbose); \
  CCTK_DECLARE_INIT (CCTK_INT const, cacheline_mult, CCTK_PARAMETER__PUGH__cacheline_mult); \
  CCTK_DECLARE_INIT (CCTK_INT const, enable_all_storage, CCTK_PARAMETER__PUGH__enable_all_storage); \
  CCTK_DECLARE_INIT (CCTK_INT const, ghost_size, CCTK_PARAMETER__PUGH__ghost_size); \
  CCTK_DECLARE_INIT (CCTK_INT const, ghost_size_x, CCTK_PARAMETER__PUGH__ghost_size_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, ghost_size_y, CCTK_PARAMETER__PUGH__ghost_size_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, ghost_size_z, CCTK_PARAMETER__PUGH__ghost_size_z); \
  CCTK_DECLARE_INIT (CCTK_INT const, global_nsize, CCTK_PARAMETER__PUGH__global_nsize); \
  CCTK_DECLARE_INIT (CCTK_INT const, global_nx, CCTK_PARAMETER__PUGH__global_nx); \
  CCTK_DECLARE_INIT (CCTK_INT const, global_ny, CCTK_PARAMETER__PUGH__global_ny); \
  CCTK_DECLARE_INIT (CCTK_INT const, global_nz, CCTK_PARAMETER__PUGH__global_nz); \
  CCTK_DECLARE_INIT (CCTK_INT const, local_nsize, CCTK_PARAMETER__PUGH__local_nsize); \
  CCTK_DECLARE_INIT (CCTK_INT const, local_nx, CCTK_PARAMETER__PUGH__local_nx); \
  CCTK_DECLARE_INIT (CCTK_INT const, local_ny, CCTK_PARAMETER__PUGH__local_ny); \
  CCTK_DECLARE_INIT (CCTK_INT const, local_nz, CCTK_PARAMETER__PUGH__local_nz); \
  CCTK_DECLARE_INIT (CCTK_INT const, local_size_includes_ghosts, CCTK_PARAMETER__PUGH__local_size_includes_ghosts); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadabort, CCTK_PARAMETER__PUGH__overloadabort); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadarraygroupsizeb, CCTK_PARAMETER__PUGH__overloadarraygroupsizeb); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadbarrier, CCTK_PARAMETER__PUGH__overloadbarrier); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloaddisablegroupcomm, CCTK_PARAMETER__PUGH__overloaddisablegroupcomm); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloaddisablegroupstorage, CCTK_PARAMETER__PUGH__overloaddisablegroupstorage); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadenablegroupcomm, CCTK_PARAMETER__PUGH__overloadenablegroupcomm); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadenablegroupstorage, CCTK_PARAMETER__PUGH__overloadenablegroupstorage); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadevolve, CCTK_PARAMETER__PUGH__overloadevolve); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadexit, CCTK_PARAMETER__PUGH__overloadexit); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadgroupdynamicdata, CCTK_PARAMETER__PUGH__overloadgroupdynamicdata); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadmyproc, CCTK_PARAMETER__PUGH__overloadmyproc); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadnprocs, CCTK_PARAMETER__PUGH__overloadnprocs); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadparallelinit, CCTK_PARAMETER__PUGH__overloadparallelinit); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadquerygroupstorageb, CCTK_PARAMETER__PUGH__overloadquerygroupstorageb); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadsyncgroup, CCTK_PARAMETER__PUGH__overloadsyncgroup); \
  CCTK_DECLARE_INIT (CCTK_INT const, overloadsyncgroupsbydiri, CCTK_PARAMETER__PUGH__overloadsyncgroupsbydiri); \
  CCTK_DECLARE_INIT (CCTK_INT const, padding_active, CCTK_PARAMETER__PUGH__padding_active); \
  CCTK_DECLARE_INIT (CCTK_INT const, padding_address_spacing, CCTK_PARAMETER__PUGH__padding_address_spacing); \
  CCTK_DECLARE_INIT (CCTK_INT const, padding_cacheline_bits, CCTK_PARAMETER__PUGH__padding_cacheline_bits); \
  CCTK_DECLARE_INIT (CCTK_INT const, padding_size, CCTK_PARAMETER__PUGH__padding_size); \
  CCTK_DECLARE_INIT (CCTK_INT const, processor_topology_1d_x, CCTK_PARAMETER__PUGH__processor_topology_1d_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, processor_topology_2d_x, CCTK_PARAMETER__PUGH__processor_topology_2d_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, processor_topology_2d_y, CCTK_PARAMETER__PUGH__processor_topology_2d_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, processor_topology_3d_x, CCTK_PARAMETER__PUGH__processor_topology_3d_x); \
  CCTK_DECLARE_INIT (CCTK_INT const, processor_topology_3d_y, CCTK_PARAMETER__PUGH__processor_topology_3d_y); \
  CCTK_DECLARE_INIT (CCTK_INT const, processor_topology_3d_z, CCTK_PARAMETER__PUGH__processor_topology_3d_z); \
  CCTK_DECLARE_INIT (CCTK_INT const, storage_report_every, CCTK_PARAMETER__PUGH__storage_report_every); \
  CCTK_DECLARE_INIT (CCTK_INT const, timer_output, CCTK_PARAMETER__PUGH__timer_output); \


#ifndef CCTK_PARAMETER__PUGH__info
#  define CCTK_PARAMETER__PUGH__info PRIVATE_PUGH_STRUCT.info
#endif
#ifndef CCTK_PARAMETER__PUGH__initialize_memory
#  define CCTK_PARAMETER__PUGH__initialize_memory PRIVATE_PUGH_STRUCT.initialize_memory
#endif
#ifndef CCTK_PARAMETER__PUGH__partition
#  define CCTK_PARAMETER__PUGH__partition PRIVATE_PUGH_STRUCT.partition
#endif
#ifndef CCTK_PARAMETER__PUGH__partition_1d_x
#  define CCTK_PARAMETER__PUGH__partition_1d_x PRIVATE_PUGH_STRUCT.partition_1d_x
#endif
#ifndef CCTK_PARAMETER__PUGH__partition_2d_x
#  define CCTK_PARAMETER__PUGH__partition_2d_x PRIVATE_PUGH_STRUCT.partition_2d_x
#endif
#ifndef CCTK_PARAMETER__PUGH__partition_2d_y
#  define CCTK_PARAMETER__PUGH__partition_2d_y PRIVATE_PUGH_STRUCT.partition_2d_y
#endif
#ifndef CCTK_PARAMETER__PUGH__partition_3d_x
#  define CCTK_PARAMETER__PUGH__partition_3d_x PRIVATE_PUGH_STRUCT.partition_3d_x
#endif
#ifndef CCTK_PARAMETER__PUGH__partition_3d_y
#  define CCTK_PARAMETER__PUGH__partition_3d_y PRIVATE_PUGH_STRUCT.partition_3d_y
#endif
#ifndef CCTK_PARAMETER__PUGH__partition_3d_z
#  define CCTK_PARAMETER__PUGH__partition_3d_z PRIVATE_PUGH_STRUCT.partition_3d_z
#endif
#ifndef CCTK_PARAMETER__PUGH__physical2logical
#  define CCTK_PARAMETER__PUGH__physical2logical PRIVATE_PUGH_STRUCT.physical2logical
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology
#  define CCTK_PARAMETER__PUGH__processor_topology PRIVATE_PUGH_STRUCT.processor_topology
#endif
#ifndef CCTK_PARAMETER__PUGH__storage_verbose
#  define CCTK_PARAMETER__PUGH__storage_verbose PRIVATE_PUGH_STRUCT.storage_verbose
#endif
#ifndef CCTK_PARAMETER__PUGH__cacheline_mult
#  define CCTK_PARAMETER__PUGH__cacheline_mult PRIVATE_PUGH_STRUCT.cacheline_mult
#endif
#ifndef CCTK_PARAMETER__PUGH__enable_all_storage
#  define CCTK_PARAMETER__PUGH__enable_all_storage PRIVATE_PUGH_STRUCT.enable_all_storage
#endif
#ifndef CCTK_PARAMETER__PUGH__ghost_size
#  define CCTK_PARAMETER__PUGH__ghost_size PRIVATE_PUGH_STRUCT.ghost_size
#endif
#ifndef CCTK_PARAMETER__PUGH__ghost_size_x
#  define CCTK_PARAMETER__PUGH__ghost_size_x PRIVATE_PUGH_STRUCT.ghost_size_x
#endif
#ifndef CCTK_PARAMETER__PUGH__ghost_size_y
#  define CCTK_PARAMETER__PUGH__ghost_size_y PRIVATE_PUGH_STRUCT.ghost_size_y
#endif
#ifndef CCTK_PARAMETER__PUGH__ghost_size_z
#  define CCTK_PARAMETER__PUGH__ghost_size_z PRIVATE_PUGH_STRUCT.ghost_size_z
#endif
#ifndef CCTK_PARAMETER__PUGH__global_nsize
#  define CCTK_PARAMETER__PUGH__global_nsize PRIVATE_PUGH_STRUCT.global_nsize
#endif
#ifndef CCTK_PARAMETER__PUGH__global_nx
#  define CCTK_PARAMETER__PUGH__global_nx PRIVATE_PUGH_STRUCT.global_nx
#endif
#ifndef CCTK_PARAMETER__PUGH__global_ny
#  define CCTK_PARAMETER__PUGH__global_ny PRIVATE_PUGH_STRUCT.global_ny
#endif
#ifndef CCTK_PARAMETER__PUGH__global_nz
#  define CCTK_PARAMETER__PUGH__global_nz PRIVATE_PUGH_STRUCT.global_nz
#endif
#ifndef CCTK_PARAMETER__PUGH__local_nsize
#  define CCTK_PARAMETER__PUGH__local_nsize PRIVATE_PUGH_STRUCT.local_nsize
#endif
#ifndef CCTK_PARAMETER__PUGH__local_nx
#  define CCTK_PARAMETER__PUGH__local_nx PRIVATE_PUGH_STRUCT.local_nx
#endif
#ifndef CCTK_PARAMETER__PUGH__local_ny
#  define CCTK_PARAMETER__PUGH__local_ny PRIVATE_PUGH_STRUCT.local_ny
#endif
#ifndef CCTK_PARAMETER__PUGH__local_nz
#  define CCTK_PARAMETER__PUGH__local_nz PRIVATE_PUGH_STRUCT.local_nz
#endif
#ifndef CCTK_PARAMETER__PUGH__local_size_includes_ghosts
#  define CCTK_PARAMETER__PUGH__local_size_includes_ghosts PRIVATE_PUGH_STRUCT.local_size_includes_ghosts
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadabort
#  define CCTK_PARAMETER__PUGH__overloadabort PRIVATE_PUGH_STRUCT.overloadabort
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadarraygroupsizeb
#  define CCTK_PARAMETER__PUGH__overloadarraygroupsizeb PRIVATE_PUGH_STRUCT.overloadarraygroupsizeb
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadbarrier
#  define CCTK_PARAMETER__PUGH__overloadbarrier PRIVATE_PUGH_STRUCT.overloadbarrier
#endif
#ifndef CCTK_PARAMETER__PUGH__overloaddisablegroupcomm
#  define CCTK_PARAMETER__PUGH__overloaddisablegroupcomm PRIVATE_PUGH_STRUCT.overloaddisablegroupcomm
#endif
#ifndef CCTK_PARAMETER__PUGH__overloaddisablegroupstorage
#  define CCTK_PARAMETER__PUGH__overloaddisablegroupstorage PRIVATE_PUGH_STRUCT.overloaddisablegroupstorage
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadenablegroupcomm
#  define CCTK_PARAMETER__PUGH__overloadenablegroupcomm PRIVATE_PUGH_STRUCT.overloadenablegroupcomm
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadenablegroupstorage
#  define CCTK_PARAMETER__PUGH__overloadenablegroupstorage PRIVATE_PUGH_STRUCT.overloadenablegroupstorage
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadevolve
#  define CCTK_PARAMETER__PUGH__overloadevolve PRIVATE_PUGH_STRUCT.overloadevolve
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadexit
#  define CCTK_PARAMETER__PUGH__overloadexit PRIVATE_PUGH_STRUCT.overloadexit
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadgroupdynamicdata
#  define CCTK_PARAMETER__PUGH__overloadgroupdynamicdata PRIVATE_PUGH_STRUCT.overloadgroupdynamicdata
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadmyproc
#  define CCTK_PARAMETER__PUGH__overloadmyproc PRIVATE_PUGH_STRUCT.overloadmyproc
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadnprocs
#  define CCTK_PARAMETER__PUGH__overloadnprocs PRIVATE_PUGH_STRUCT.overloadnprocs
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadparallelinit
#  define CCTK_PARAMETER__PUGH__overloadparallelinit PRIVATE_PUGH_STRUCT.overloadparallelinit
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadquerygroupstorageb
#  define CCTK_PARAMETER__PUGH__overloadquerygroupstorageb PRIVATE_PUGH_STRUCT.overloadquerygroupstorageb
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadsyncgroup
#  define CCTK_PARAMETER__PUGH__overloadsyncgroup PRIVATE_PUGH_STRUCT.overloadsyncgroup
#endif
#ifndef CCTK_PARAMETER__PUGH__overloadsyncgroupsbydiri
#  define CCTK_PARAMETER__PUGH__overloadsyncgroupsbydiri PRIVATE_PUGH_STRUCT.overloadsyncgroupsbydiri
#endif
#ifndef CCTK_PARAMETER__PUGH__padding_active
#  define CCTK_PARAMETER__PUGH__padding_active PRIVATE_PUGH_STRUCT.padding_active
#endif
#ifndef CCTK_PARAMETER__PUGH__padding_address_spacing
#  define CCTK_PARAMETER__PUGH__padding_address_spacing PRIVATE_PUGH_STRUCT.padding_address_spacing
#endif
#ifndef CCTK_PARAMETER__PUGH__padding_cacheline_bits
#  define CCTK_PARAMETER__PUGH__padding_cacheline_bits PRIVATE_PUGH_STRUCT.padding_cacheline_bits
#endif
#ifndef CCTK_PARAMETER__PUGH__padding_size
#  define CCTK_PARAMETER__PUGH__padding_size PRIVATE_PUGH_STRUCT.padding_size
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology_1d_x
#  define CCTK_PARAMETER__PUGH__processor_topology_1d_x PRIVATE_PUGH_STRUCT.processor_topology_1d_x
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology_2d_x
#  define CCTK_PARAMETER__PUGH__processor_topology_2d_x PRIVATE_PUGH_STRUCT.processor_topology_2d_x
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology_2d_y
#  define CCTK_PARAMETER__PUGH__processor_topology_2d_y PRIVATE_PUGH_STRUCT.processor_topology_2d_y
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology_3d_x
#  define CCTK_PARAMETER__PUGH__processor_topology_3d_x PRIVATE_PUGH_STRUCT.processor_topology_3d_x
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology_3d_y
#  define CCTK_PARAMETER__PUGH__processor_topology_3d_y PRIVATE_PUGH_STRUCT.processor_topology_3d_y
#endif
#ifndef CCTK_PARAMETER__PUGH__processor_topology_3d_z
#  define CCTK_PARAMETER__PUGH__processor_topology_3d_z PRIVATE_PUGH_STRUCT.processor_topology_3d_z
#endif
#ifndef CCTK_PARAMETER__PUGH__storage_report_every
#  define CCTK_PARAMETER__PUGH__storage_report_every PRIVATE_PUGH_STRUCT.storage_report_every
#endif
#ifndef CCTK_PARAMETER__PUGH__timer_output
#  define CCTK_PARAMETER__PUGH__timer_output PRIVATE_PUGH_STRUCT.timer_output
#endif

