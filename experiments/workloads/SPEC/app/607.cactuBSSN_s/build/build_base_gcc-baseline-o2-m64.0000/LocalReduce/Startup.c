 /*@@
   @file      Startup.c
   @date      
   @author    
   @desc
              Startup routines for LocalReduce.
   @enddesc
   @version   $Id: Startup.c 72 2005-11-16 19:17:35Z yye00 $
@@*/

#ifdef SPEC 
#include "LocalReduce/cctk.h" 
#define THORN_DIR "LocalReduce" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "CactusNumerical/LocalReduce/src/local_reductions.h"

static const char *rcsid="$Id: Startup.c 72 2005-11-16 19:17:35Z yye00 $";

CCTK_FILEVERSION(CactusBase_LocalReduce_Startup_c)


/* prototypes of routines defined in this source file */
int LocalReduce_Startup(void);


int LocalReduce_Startup (void)
{
  /* Register the reduction operators */

  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_Mean,     "mean");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_Mean,     "average");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_Max,  "maximum");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_Min,  "minimum");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_Count,   "count");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_Sum,     "sum");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_L1,   "norm1");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_L2,   "norm2");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_L3,   "norm3");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_L4,   "norm4");
  CCTK_RegisterLocalArrayReductionOperator (LocalReduce_LInf, "norm_inf"); 


  return (0);
}
