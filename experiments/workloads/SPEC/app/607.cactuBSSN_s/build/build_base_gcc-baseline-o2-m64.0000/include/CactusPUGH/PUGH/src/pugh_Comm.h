 /*@@
   @header    pugh_Comm.h
   @date      Thu Feb  4 11:42:50 1999
   @author    Tom Goodale
   @desc
              Prototypes of functions to be overloaded by PUGH
   @enddesc
   @version   $Header$
 @@*/

#ifndef _PUGH_COMM_H_
#define _PUGH_COMM_H_ 1

#ifdef __cplusplus
extern "C" 
{
#endif

int PUGH_Barrier (const cGH *GH);
int PUGH_SyncGroup (const cGH *GH, const char *group);
int PUGH_SyncGroupsByDirI (const cGH *GH,
                           int num_groups,
                           const int *groups,
                           const int *directions);

int PUGH_EnableGroupStorage (const cGH *GH, const char *group);
int PUGH_DisableGroupStorage (const cGH *GH, const char *group);

int PUGH_GroupStorageIncrease(const cGH *GH, int n_groups,const int *groups,const int *timelevels, int *status);
int PUGH_GroupStorageDecrease(const cGH *GH, int n_groups,const int *groups,const int *timelevels, int *status);

int PUGH_EnableGroupComm (const cGH *GH, const char *group);
int PUGH_DisableGroupComm (const cGH *GH, const char *group);

float PUGH_ReturnTotalStorage(void);
float PUGH_ReturnMaxStorage(void);

const int *PUGH_ArrayGroupSize (const cGH *GH, int dir, int vindex, const char *groupname);
int PUGH_QueryGroupStorage (const cGH *GH, int group, const char *groupname);

int PUGH_GroupDynamicData (const cGH *GH, int group, cGroupDynamicData *data);

#ifdef __cplusplus
}
#endif

#endif  /* _PUGH_COMM_H_ */
