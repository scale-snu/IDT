 /*@@
   @header    ioASCIIGH.h
   @date      Tue 9th Jan 1999
   @author    Gabrielle Allen
   @desc 
              The extensions to the GH structure from IOASCII.
   @version   $Header$
 @@*/

#ifndef _IOASCII_IOASCIIGH_H_
#define _IOASCII_IOASCIIGH_H_ 1

#include "StoreNamedData.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct IOASCIIGH
{
  /* default number of times to output */
  int out1D_every_default, out2D_every_default, out3D_every_default;

  /* number of times to output every variable */
  CCTK_INT *out1D_every, *out2D_every, *out3D_every;

  /* lists of variables to output */
  char *out1D_vars, *out2D_vars, *out3D_vars;

  /* directories in which to output */
  char *out1D_dir, *out2D_dir, *out3D_dir;

  /* the last iteration output for var [i] */
  int *out1D_last, *out2D_last, *out3D_last;

  /* database for names of output files that were already created */
  pNamedData *filenameList1D, *fileList_2D, *fileList_3D;

  /* for 1D lines, we define the index where to start the line:
     spxyz[maxdim][XYZdirection][xyzstart_index] */
  int ***spxyz;

  /* for 2D planes, we define the index where to locate the plane
     sp2xyz[maxdim][perpendicular direction] */
  int **sp2xyz;

  /* stop on I/O parameter parsing errors ? */
  int stop_on_parse_errors;

} asciiioGH;


/* prototypes of functions to be registered as I/O methods */
int IOASCII_Output1DGH (const cGH *GH);
int IOASCII_TriggerOutput1D (const cGH *GH, int);
int IOASCII_TimeFor1D (const cGH *GH, int);
int IOASCII_Output1DVarAs (const cGH *GH, const char *var, const char *alias);
int IOASCII_Output2DGH (const cGH *GH);
int IOASCII_TriggerOutput2D (const cGH *GH, int);
int IOASCII_TimeFor2D (const cGH *GH, int);
int IOASCII_Output2DVarAs (const cGH *GH, const char *var, const char *alias);
int IOASCII_Output3DGH (const cGH *GH);
int IOASCII_TriggerOutput3D (const cGH *GH, int);
int IOASCII_TimeFor3D (const cGH *GH, int);
int IOASCII_Output3DVarAs (const cGH *GH, const char *var, const char *alias);

/* other function prototypes */
int IOASCII_Write1D (const cGH *GH, int vindex, const char *alias);
int IOASCII_Write2D (const cGH *GH, int vindex, const char *alias);
int IOASCII_Write3D (const cGH *GH, int vindex, const char *alias);

void IOASCII_CheckSteerableParameters1D (asciiioGH *myGH);
void IOASCII_CheckSteerableParameters2D (asciiioGH *myGH);
void IOASCII_CheckSteerableParameters3D (asciiioGH *myGH);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* _IOASCII_IOASCIIGH_H_ */
