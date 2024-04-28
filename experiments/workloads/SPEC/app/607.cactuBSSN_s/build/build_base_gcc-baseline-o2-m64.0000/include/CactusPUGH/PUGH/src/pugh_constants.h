 /*@@
   @file      pugh_constants.h
   @date      
   @author    
   @desc 
   @enddesc 
   @version $Id: pugh_constants.h 515 2012-07-04 01:29:14Z eschnett $
 @@*/

/* Pre-processor constants start with PUGH_ */

/* Constants for the comm directions */
#define PUGH_NOCOMM 0
#define PUGH_ALLCOMM 1
#define PUGH_PLUSFACESCOMM 2
#define PUGH_MINUSFACESCOMM 3
#define PUGH_COMM(i) i+4 

/* Constants for the storage */
#define PUGH_NOSTORAGE        0
#define PUGH_STORAGE          1

/* Comm types available */
#define PUGH_NOCOMMBUFFERS    0
#define PUGH_ALLOCATEDBUFFERS 1
#define PUGH_DERIVEDTYPES     2

/* Termination flags */
#define TERMINATION_NOT_RAISED     0    /* no termination flag, inital setting      */
#define TERMINATION_RAISED_LOCAL   1    /* raised on one pe, has to be broadcasted  */
#define TERMINATION_RAISED_BRDCAST 2    /* flag now available on all pes: termiante */

/* PUGH error flags */
#define PUGH_SUCCESS      0
#define PUGH_ERROR       -1
#define PUGH_ERRORMEMORY -2
