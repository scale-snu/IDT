 /*@@
   @header    Register.h
   @date      Wed Dec 14 17:43:42 2005
   @author    Tom Goodale
   @desc 
   This is a template header function
   @enddesc
   @version $Header$
 @@*/

#ifndef _PUGH_REGISTER_H_
#define _PUGH_REGISTER_H_ 1

#ifdef __cplusplus
extern "C" 
{
#endif

int PUGH_RegisterGenerateTopology(int (*GenerateTopology) (int dim,
                                                           int total_procs, 
                                                           const int *nsize, 
                                                           const int *nghostzones, 
                                                           int *nprocs), 
                                  const char *topologyname);

int PUGH_RegisterP2L(
                     void *(*CreateP2LTranslation) (int dim, 
                                                    int *nprocs, 
                                                    int np),
                     int   (*DestroyP2LTranslation) (void *table),
                     int   (*PhysicalToLogical) (void *table, 
                                                 int proc),
                     int   (*LogicalToPhysical) (void *table, 
                                                 int num),
                     const char *p2lname);


#ifdef __cplusplus
}
#endif

#endif /* _PUGH_REGISTER_H_ */
