 /*@@
   @file      pughInterpGH.h
   @date      Sun Jul 04 1999
   @author    Thomas Radke
   @desc
              Definition of GH extensions structure for thorn PUGHInterp
              and declaration of function prototypes.
   @enddesc
   @version   $Header$
@@*/

#ifndef _PUGHINTERP_PUGHINTERP_H_
#define _PUGHINTERP_PUGHINTERP_H_ 1

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
  int *sendcnt;             /* number of elements to be sent */
  int *senddispl;           /* offset of each element to be sent */
  int *recvcnt;             /* number of elements to be received */
  int *recvdispl;           /* offset of each element to be received */

  CCTK_INT *N_points_to;    /* number of coordinate points to be sent */
  CCTK_INT *N_points_from;  /* number of coordinate points to be received */

  int *whichproc;           /* which processor does point i belong to */
  int *indices;             /* indices to sort from processor-sorted into */
} pughInterpGH;             /* point-sorted order */


/* prototype of PUGHInterp's routine which overloads CCTK_InterpGridArrays() */
int PUGHInterp_InterpGridArrays (const cGH *GH,
                                 int N_dims,
                                 int param_table_handle,
                                 int local_interp_handle,
                                 int coord_system_handle,
                                 int N_interp_points,
                                   int interp_coords_type,
                                   const void *const interp_coords[],
                                 int N_input_arrays,
                                   const CCTK_INT input_array_indices[],
                                 int N_output_arrays,
                                   const CCTK_INT output_array_types[],
                                   void *const output_arrays[]);


/* prototype of PUGHInterp's routine which provides the aliased
   function DriverInterpolate */
CCTK_INT
PUGHInterp_DriverInterpolate (CCTK_POINTER_TO_CONST const GH_,
                              CCTK_INT const N_dims,
                              CCTK_INT const local_interp_handle,
                              CCTK_INT const param_table_handle,
                              CCTK_INT const coord_system_handle,
                              CCTK_INT const N_points,
                              CCTK_INT const interp_coords_type,
                              CCTK_POINTER_TO_CONST const interp_coords [],
                              CCTK_INT const N_input_arrays,
                              CCTK_INT const input_array_indices [],
                              CCTK_INT const N_output_arrays,
                              CCTK_INT const output_array_types [],
                              CCTK_POINTER const output_arrays []);


#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* _PUGHINTERP_PUGHINTERP_H_ */
