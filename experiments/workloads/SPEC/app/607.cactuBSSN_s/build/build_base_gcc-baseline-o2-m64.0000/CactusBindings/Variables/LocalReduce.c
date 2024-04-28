/*@@
   @file    LocalReduce.c
   @author  Automatically generated by GridFuncStuff.pl
   @desc
            Creates the CCTK variables for thorn LocalReduce
   @enddesc
 @@*/



#include <stddef.h>

#ifdef SPEC 
#include "LocalReduce/cctk.h" 
#define THORN_DIR "LocalReduce" 
#else 
#include "LocalReduce/cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "LocalReduce/cctk_Arguments.h" 
#else 
#include "LocalReduce/cctk_Arguments.h"
#endif /* END SPEC */ 
#include "cctk_Parameter.h"
#include "cctk_WarnLevel.h"
#include "cctki_Groups.h"
#include "cctki_FortranWrappers.h"
#include "LocalReduce/cctk_ScheduleFunctions.h"

int CactusBindingsVariables_LocalReduce_Initialise(void);
static int CCTKi_BindingsFortranWrapperLocalReduce(void *_GH, void *fpointer);

static int CCTKi_BindingsFortranWrapperLocalReduce(void *_GH, void *fpointer)
{
  cGH const *const GH = _GH;
  const int _cctk_zero = 0;
  void (*function)(LOCALREDUCE_C2F_PROTO);
  DECLARE_LOCALREDUCE_C2F
  INITIALISE_LOCALREDUCE_C2F
  (void) (_cctk_zero + 0);

  function = (void (*) (LOCALREDUCE_C2F_PROTO)) fpointer;
  function (PASS_LOCALREDUCE_C2F (GH));

  return (0);
}

int CactusBindingsVariables_LocalReduce_Initialise(void)
{
  const char * warn_mixeddim_gfs = "";
  int warn_mixeddim = 0;
  const CCTK_INT *allow_mixeddim_gfs;


  allow_mixeddim_gfs = CCTK_ParameterGet ("allow_mixeddim_gfs", "Cactus", 0);


  if (*warn_mixeddim_gfs)
  {
    if (allow_mixeddim_gfs && *allow_mixeddim_gfs)
    {
      CCTK_VWarn (2, __LINE__, __FILE__, "Cactus",
                  "CCTKi_CreateGroup: Working dimension already set, "
                  "creating GF group '%s' with different dimension %d",
                  warn_mixeddim_gfs, warn_mixeddim);
    }
    else
    {
      CCTK_VWarn (0, __LINE__, __FILE__, "Cactus",
                  "CCTKi_CreateGroup: Working dimension already set, "
                  "cannot create GF group '%s' with dimension %d",
                  warn_mixeddim_gfs, warn_mixeddim);
    }
 }

  CCTKi_RegisterFortranWrapper("LocalReduce", CCTKi_BindingsFortranWrapperLocalReduce);

  return 0;
}
