 /*@@
   @file      Startup.c
   @date      Wed Feb  3 23:10:19 1999
   @author    Tom Goodale
   @desc
              Startup routines for PUGHReduce.
   @enddesc
   @version   $Header$
@@*/

#ifdef SPEC 
#include "PUGHReduce/cctk.h" 
#define THORN_DIR "PUGHReduce" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "pugh_reductions.h"

static const char *rcsid="$Header$";

CCTK_FILEVERSION(CactusPUGH_PUGHReduce_Startup_c)


/* prototypes of routines defined in this source file */
int PUGHReduce_Startup(void);


 /*@@
   @routine    PUGHReduce_Startup
   @date       Wed Feb  3 23:14:38 1999
   @author     Tom Goodale
   @desc
               The startup registration routine for PUGHReduce.
   @enddesc

   @calls      CCTK_RegisterReductionOperator
               CCTK_RegisterReductionArrayOperator

   @returntype void
   @returndesc
   @endreturndesc
@@*/
int PUGHReduce_Startup (void)
{
  /* Register the Global reduction operator */
  CCTK_RegisterGridArrayReductionOperator(PUGH_ReduceGridArrays);
  
  /* Register the Array reducction globally operator */
  CCTK_RegisterReduceArraysGloballyOperator(PUGH_ReduceArraysGlobally);
  
  /* Register the reduction operators provided by PUGH */
  CCTK_RegisterReductionOperator (PUGH_ReductionAvgGVs,     "average");
  CCTK_RegisterReductionOperator (PUGH_ReductionAvgGVs,     "mean");
  CCTK_RegisterReductionOperator (PUGH_ReductionCountGVs,   "count");
  CCTK_RegisterReductionOperator (PUGH_ReductionMaxValGVs,  "maximum");
  CCTK_RegisterReductionOperator (PUGH_ReductionMinValGVs,  "minimum");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm1GVs,   "norm1");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm1GVs,   "L1Norm");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm2GVs,   "norm2");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm2GVs,   "L2Norm");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm3GVs,   "norm3");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm3GVs,   "L3Norm");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm4GVs,   "norm4");
  CCTK_RegisterReductionOperator (PUGH_ReductionNorm4GVs,   "L4Norm");
  CCTK_RegisterReductionOperator (PUGH_ReductionNormInfGVs, "norm_inf");
  CCTK_RegisterReductionOperator (PUGH_ReductionSumGVs,     "sum");

  CCTK_RegisterReductionArrayOperator (PUGH_ReductionAvgArrays,     "average");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionAvgArrays,     "mean");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionCountArrays,   "count");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionMaxValArrays,  "maximum");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionMinValArrays,  "minimum");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm1Arrays,   "norm1");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm1Arrays,   "L1Norm");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm2Arrays,   "norm2");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm2Arrays,   "L2Norm");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm3Arrays,   "norm3");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm3Arrays,   "L3Norm");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm4Arrays,   "norm4");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNorm4Arrays,   "L4Norm");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionNormInfArrays, "norm_inf");
  CCTK_RegisterReductionArrayOperator (PUGH_ReductionSumArrays,     "sum");

  return (0);
}
