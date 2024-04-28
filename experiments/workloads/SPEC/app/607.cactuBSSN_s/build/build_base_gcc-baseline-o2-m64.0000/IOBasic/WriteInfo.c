/*@@
   @file      WriteInfo.c
   @date      June 31 1999
   @author    Gabrielle Allen, Paul Walker, Thomas Radke
   @desc
              Gets the data for IOBasic's info output.
   @enddesc
   @version   $Id: /cactusdevcvs/CactusBase/IOBasic/src/OutputInfo.c,v 1.24 2001
@@*/

#include <stdlib.h>

#ifdef SPEC 
#include "IOBasic/cctk.h" 
#define THORN_DIR "IOBasic" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#include "iobasicGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusBase_IOBasic_WriteInfo_c)


 /*@@
   @routine    IOBasic_WriteInfo
   @date       Tue 31 July 2001
   @author     Thomas Radke
   @desc
               Gets the data values to output for a given CCTK variable.
               For CCTK_SCALAR variables, their value is taken,
               for CCTK_GF and CCTK_ARRAY variables the appropriate
               reduction operations are performed and their results taken.
   @enddesc

   @calls      CCTK_VarDataPtrI
               CCTK_ReduceGridArrays

   @var       GH
   @vdesc     Pointer to CCTK grid hierarchy
   @vtype     const cGH *
   @vio       in
   @endvar
   @var       vindex
   @vdesc     CCTK index of the variable to output
   @vtype     int
   @vio       in
   @endvar
   @var       alias
   @vdesc     alias name to use for building the output filename
   @vtype     const char *
   @vio       in
   @endvar

   @returntype int
   @returndesc
                0 for success, or<BR>
               -1 if variable has no storage assigned
   @endreturndesc
@@*/
int IOBasic_WriteInfo (const cGH *GH, int vindex)
{
  int vtype;
  char *fullname;
  iobasicGH *myGH;
  void *ptr;
  iobasic_reduction_t *reduction;
  
  int input_array[1];
  CCTK_INT output_array_type_codes[1];
  CCTK_REAL output_value;
  void * reduction_value[1];
 
  input_array[0] = vindex; 
  output_array_type_codes[0] = CCTK_VARIABLE_REAL;
  reduction_value[0] = &output_value;
  
  myGH = CCTK_GHExtension (GH, "IOBasic");
  reduction = myGH->info_reductions[vindex].reductions;

  /* first, check if variable has storage assigned */
  if (! CCTK_QueryGroupStorageI (GH, CCTK_GroupIndexFromVarI (vindex)))
  {
    fullname = CCTK_FullName (vindex);
    CCTK_VWarn (2, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOBasic_WriteInfo: No info output for '%s' (no storage)",
                fullname);
    free (fullname);

    /* invalidate data buffer for this variable */
    while (reduction)
    {
      reduction->is_valid = 0;
      reduction = reduction->next;
    }
    return (-1);
  }

  /* CCTK scalars are printed by their value, cast into a C double */
  if (CCTK_GroupTypeFromVarI (vindex) == CCTK_SCALAR)
  {
    reduction->is_valid = 1;

    /* get the variable's data type and data pointer */
    ptr = CCTK_VarDataPtrI (GH, 0, vindex);
    vtype = CCTK_VarTypeI (vindex);
    switch (vtype)
    {
    case CCTK_VARIABLE_BYTE:
      reduction->value = (double) *(CCTK_BYTE *) ptr;
      break;
    case CCTK_VARIABLE_INT:
      reduction->value = (double) *(CCTK_INT *) ptr;
      break;
    case CCTK_VARIABLE_REAL:
      reduction->value = (double) *(CCTK_REAL *) ptr;
      break;
    case CCTK_VARIABLE_COMPLEX:
      reduction->value = (double) ((CCTK_REAL *) ptr)[0];
      reduction->next->value = (double) ((CCTK_REAL *) ptr)[1];
      reduction->next->is_valid = 1;
      break;
#ifdef CCTK_INT1
    case CCTK_VARIABLE_INT1:
      reduction->value = (double) *(CCTK_INT1 *) ptr;
      break;
#endif
#ifdef CCTK_INT2
    case CCTK_VARIABLE_INT2:
      reduction->value = (double) *(CCTK_INT2 *) ptr;
      break;
#endif
#ifdef CCTK_INT4
    case CCTK_VARIABLE_INT4:
      reduction->value = (double) *(CCTK_INT4 *) ptr;
      break;
#endif
#ifdef CCTK_INT8
    case CCTK_VARIABLE_INT8:
      reduction->value = (double) *(CCTK_INT8 *) ptr;
      break;
#endif
#ifdef CCTK_REAL4
    case CCTK_VARIABLE_REAL4:
      reduction->value = (double) *(CCTK_REAL4 *) ptr;
      break;
    case CCTK_VARIABLE_COMPLEX8:
      reduction->value = (double) ((CCTK_REAL4 *) ptr)[0];
      reduction->next->value = (double) ((CCTK_REAL4 *) ptr)[1];
      reduction->next->is_valid = 1;
      break;
#endif
#ifdef CCTK_REAL8
    case CCTK_VARIABLE_REAL8:
      reduction->value = (double) *(CCTK_REAL8 *) ptr;
      break;
    case CCTK_VARIABLE_COMPLEX16:
      reduction->value = (double) ((CCTK_REAL8 *) ptr)[0];
      reduction->next->value = (double) ((CCTK_REAL8 *) ptr)[1];
      reduction->next->is_valid = 1;
      break;
#endif
#ifdef CCTK_REAL16
    case CCTK_VARIABLE_REAL16:
      reduction->value = (double) *(CCTK_REAL16 *) ptr;
      break;
    case CCTK_VARIABLE_COMPLEX32:
      reduction->value = (double) ((CCTK_REAL16 *) ptr)[0];
      reduction->next->value = (double) ((CCTK_REAL16 *) ptr)[1];
      reduction->next->is_valid = 1;
      break;
#endif
    default:
      CCTK_WARN (3, "IOBasic_WriteInfo: Unsupported data type");
      reduction->is_valid = 0;
      break;
    }
  }
  else
  {
    /* for CCTK_GF and CCTK_ARRAY variables: loop over all reductions */
    while (reduction)
    {
      reduction->is_valid = CCTK_ReduceGridArrays (GH, -1, reduction->handle, -1, 1, input_array , 1, output_array_type_codes, reduction_value) == 0;
      reduction->value = output_value;
      					 
      if (! reduction->is_valid)
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "IOBasic_WriteInfo: Internal error in reduction '%s'",
                    reduction->name);
      }

      reduction = reduction->next;
    }
  }

  return (0);
}
