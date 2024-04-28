 /*@@
   @header    pugh_extension.h
   @date      Thu Feb  4 10:51:27 1999
   @author    Tom Goodale
   @desc 
   The header file for PUGH GH extension stuff.
   @enddesc 
   @version $Header$
 @@*/

#ifndef _PUGH_EXTENSION_H_
#define _PUGH_EXTENSION_H_ 1

#ifdef __cplusplus
extern "C" 
{
#endif

int PUGH_Evolve(tFleshConfig *config);
int PUGH_ScheduleTraverseGH(cGH *GH, const char *where);                

void *PUGH_SetupGH(tFleshConfig *config, 
                   int convergence_level, 
                   cGH *GH);

int PUGH_InitGH(cGH *GH);

int PUGH_ScheduleTraverseGH(cGH *GH, 
                            const char *where);

#ifdef __cplusplus
}
#endif

#endif /* _PUGH_EXTENSION_H_ */
