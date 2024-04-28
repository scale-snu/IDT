 /*@@
   @header    iobasicGH.h
   @date      Friday 18th September 1999
   @author    Gabrielle Allen
   @desc
              The extensions to the GH structure from IOBasic.
   @enddesc
   @version   $Header$
 @@*/

#ifndef _IOBASIC_IOBASICGH_H_
#define _IOBASIC_IOBASICGH_H_ 1

#include "StoreNamedData.h"


typedef struct IOBASIC_REDUCTION
{
  int handle;
  char *name;
  char is_valid;
  CCTK_REAL value;
  struct IOBASIC_REDUCTION *next;
} iobasic_reduction_t;

typedef struct IOBASIC_REDUCTIONLIST
{
  unsigned int num_reductions;
  iobasic_reduction_t *reductions;
} iobasic_reductionlist_t;

typedef struct IOBASIC_PARSEINFO
{
  const char *reductions_string;
  iobasic_reductionlist_t *reduction_list;
} iobasic_parseinfo_t;

typedef struct IOBASIC_GH
{
  /* how often to output */
  int       outScalar_every, outInfo_every;
  CCTK_REAL outScalar_dt, outInfo_dt;
  char      info_reductions_changed;

  /* directory in which to place scalar output */
  char *out_dir;

  /* The last iteration output */
  int *outInfo_last, *outScalar_last;

  /* The reduction lists for info and scalar output for all variables */
  iobasic_reductionlist_t *info_reductions, *scalar_reductions;

  /* database for names of output files that were already created */
  pNamedData *filenameListScalar;

  /* stop on I/O parameter parsing errors ? */
  int stop_on_parse_errors;

} iobasicGH;


/* prototypes of functions to be registered */
int IOBasic_InfoOutputGH (const cGH *GH);
int IOBasic_OutputVarAs (const cGH *GH, const char *fullname,const char *alias);
int IOBasic_TriggerInfoOutput (const cGH *GH, int vindex);
int IOBasic_TimeForInfoOutput (const cGH *GH, int vindex);
int IOBasic_ScalarOutputGH (const cGH *GH);
int IOBasic_TriggerScalarOutput (const cGH *GH, int vindex);
int IOBasic_TimeForScalarOutput (const cGH *GH, int vindex);

/* other function prototypes */
void IOBasic_AssignReductionList (int vindex, const char *optstring, void *arg);
int IOBasic_WriteInfo (const cGH *GH, int vindex);
int IOBasic_WriteScalar (const cGH *GH, int vindex, const char *alias);
void IOBasic_CheckSteerableInfoParameters (iobasicGH *myGH);
void IOBasic_CheckSteerableScalarParameters (iobasicGH *myGH);

#endif /* _IOBASIC_IOBASICGH_H_ */
