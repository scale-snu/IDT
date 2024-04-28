#ifdef __cplusplus
extern "C"
{
#endif

extern struct
{
  CCTK_INT MoL_Max_Evolved_Array_Size;
  CCTK_INT MoL_Max_Evolved_ComplexArray_Size;
  CCTK_INT MoL_Num_ArrayConstrained_Vars;
  CCTK_INT MoL_Num_ArrayEvolved_Vars;
  CCTK_INT MoL_Num_ArraySaveAndRestore_Vars;
  CCTK_INT MoL_Num_ComplexArrayConstrained_Vars;
  CCTK_INT MoL_Num_ComplexArrayEvolved_Vars;
  CCTK_INT MoL_Num_ComplexArraySaveAndRestore_Vars;
  CCTK_INT MoL_Num_ComplexConstrained_Vars;
  CCTK_INT MoL_Num_ComplexEvolved_Vars;
  CCTK_INT MoL_Num_ComplexSaveAndRestore_Vars;
  CCTK_INT MoL_Num_Constrained_Vars;
  CCTK_INT MoL_Num_Evolved_Vars;
  CCTK_INT MoL_Num_Evolved_Vars_Slow;
  CCTK_INT MoL_Num_SaveAndRestore_Vars;
  CCTK_INT MoL_Num_Scratch_Levels;
} RESTRICTED_METHODOFLINES_STRUCT;

#ifdef __cplusplus
}
#endif

#define DECLARE_RESTRICTED_METHODOFLINES_STRUCT_PARAMS \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Max_Evolved_Array_Size, CCTK_PARAMETER__METHODOFLINES__MoL_Max_Evolved_Array_Size); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Max_Evolved_ComplexArray_Size, CCTK_PARAMETER__METHODOFLINES__MoL_Max_Evolved_ComplexArray_Size); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ArrayConstrained_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArrayConstrained_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ArrayEvolved_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArrayEvolved_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ArraySaveAndRestore_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArraySaveAndRestore_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ComplexArrayConstrained_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArrayConstrained_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ComplexArrayEvolved_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArrayEvolved_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ComplexArraySaveAndRestore_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArraySaveAndRestore_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ComplexConstrained_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexConstrained_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ComplexEvolved_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexEvolved_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_ComplexSaveAndRestore_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexSaveAndRestore_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_Constrained_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_Constrained_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_Evolved_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_Evolved_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_Evolved_Vars_Slow, CCTK_PARAMETER__METHODOFLINES__MoL_Num_Evolved_Vars_Slow); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_SaveAndRestore_Vars, CCTK_PARAMETER__METHODOFLINES__MoL_Num_SaveAndRestore_Vars); \
  CCTK_DECLARE_INIT (CCTK_INT const, MoL_Num_Scratch_Levels, CCTK_PARAMETER__METHODOFLINES__MoL_Num_Scratch_Levels); \


#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Max_Evolved_Array_Size
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Max_Evolved_Array_Size RESTRICTED_METHODOFLINES_STRUCT.MoL_Max_Evolved_Array_Size
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Max_Evolved_ComplexArray_Size
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Max_Evolved_ComplexArray_Size RESTRICTED_METHODOFLINES_STRUCT.MoL_Max_Evolved_ComplexArray_Size
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArrayConstrained_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArrayConstrained_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ArrayConstrained_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArrayEvolved_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArrayEvolved_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ArrayEvolved_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArraySaveAndRestore_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ArraySaveAndRestore_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ArraySaveAndRestore_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArrayConstrained_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArrayConstrained_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ComplexArrayConstrained_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArrayEvolved_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArrayEvolved_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ComplexArrayEvolved_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArraySaveAndRestore_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexArraySaveAndRestore_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ComplexArraySaveAndRestore_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexConstrained_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexConstrained_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ComplexConstrained_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexEvolved_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexEvolved_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ComplexEvolved_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexSaveAndRestore_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_ComplexSaveAndRestore_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_ComplexSaveAndRestore_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_Constrained_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_Constrained_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_Constrained_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_Evolved_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_Evolved_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_Evolved_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_Evolved_Vars_Slow
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_Evolved_Vars_Slow RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_Evolved_Vars_Slow
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_SaveAndRestore_Vars
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_SaveAndRestore_Vars RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_SaveAndRestore_Vars
#endif
#ifndef CCTK_PARAMETER__METHODOFLINES__MoL_Num_Scratch_Levels
#  define CCTK_PARAMETER__METHODOFLINES__MoL_Num_Scratch_Levels RESTRICTED_METHODOFLINES_STRUCT.MoL_Num_Scratch_Levels
#endif
