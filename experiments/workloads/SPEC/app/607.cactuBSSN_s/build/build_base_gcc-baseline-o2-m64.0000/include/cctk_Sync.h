 /*@@
   @header    cctk_Sync.h
   @date      Thu Jan 27 18:04:12 2000
   @author    Tom Goodale
   @desc
              Various CCTK synchronisation functions.
   @enddesc
   @version   $Header$
 @@*/

#ifndef _CCTK_SYNC_H_
#define _CCTK_SYNC_H_

#ifdef __cplusplus
extern "C"
{
#endif

int CCTK_SyncGroupI (const cGH *GH, int group);
int CCTK_SyncGroupWithVar (const cGH *GH, const char *varname);
int CCTK_SyncGroupWithVarI (const cGH *GH, int var);
int CCTK_SyncGroupsI (const cGH *GH, int n_groups, const int *groups);

#ifdef __cplusplus
}
#endif

#endif /* _CCTK_SYNC_H_ */
