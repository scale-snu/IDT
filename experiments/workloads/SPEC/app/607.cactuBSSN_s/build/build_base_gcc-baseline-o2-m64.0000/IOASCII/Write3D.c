/*@@
   @file      Write3D.c
   @date      Wed 12 Dec 2001
   @author    Thomas Radke
   @desc
              Three-dimensional output of variables in ASCII gnuplot format.
   @enddesc
   @version   $Id: Write3D.c 223 2012-11-20 23:14:47Z eschnett $
 @@*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SPEC 
#include "IOASCII/cctk.h" 
#define THORN_DIR "IOASCII" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOASCII/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "util_Table.h"
#include "CactusBase/IOUtil/src/ioutil_AdvertisedFiles.h"
#include "ioASCIIGH.h"

/* the rcs ID and its dummy function to use it */
static const char *rcsid = "$Header$";
CCTK_FILEVERSION(CactusBase_IOASCII_Write3D_c)


/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* Macro to output a 3D array (with coordinates if available) as typed data
   For readability, the macro definition implicitely uses the following
   variables (which are defined in the calling routine):
     hdata, hsize, is_complex, format[2], file */
#define WRITE_DATA(cctk_type, c_type, fmt)                                    \
        {                                                                     \
          int _i, _j, _k;                                                     \
          const cctk_type *_hdata = (const cctk_type *) hdata[0];             \
          const CCTK_REAL *_coord[3];                                         \
                                                                              \
                                                                              \
                                                                              \
          _coord[0] = (const CCTK_REAL *) hdata[1];                           \
          _coord[1] = (const CCTK_REAL *) hdata[2];                           \
          _coord[2] = (const CCTK_REAL *) hdata[3];                           \
          for (_k = 0; _k < hsize[2]; _k++)                                   \
          {                                                                   \
            for (_j = 0; _j < hsize[1]; _j++)                                 \
            {                                                                 \
              for (_i = 0; _i < hsize[0]; _i++)                               \
              {                                                               \
                if (_coord[0])                                                \
                {                                                             \
                  fprintf (file, fmt,                                         \
                           (double) *_coord[0]++, (double) *_coord[1]++,      \
                           (double) *_coord[2]++, (c_type) *_hdata++);        \
                }                                                             \
                else                                                          \
                {                                                             \
                  fprintf (file, fmt, _i, _j, _k, (c_type) *_hdata++);        \
                }                                                             \
                                                                              \
                if (is_complex)                                               \
                {                                                             \
                  fprintf (file, format[2], (c_type) *_hdata++);              \
                }                                                             \
                fputc ('\n', file);                                           \
              }                                                               \
              fputc ('\n', file);                                             \
            }                                                                 \
            fputc ('\n', file);                                               \
          }                                                                   \
          fputc ('\n', file);                                                 \
        }


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static FILE *OpenFile (const cGH *GH, const char *fullname, const char *alias);
static void WriteData (int vtype,
                       const char *header,
                       char format[3][64],
                       const CCTK_INT hsize[3],
                       void *const *const hdata,
                       FILE *file);

/*@@
   @routine   IOASCII_Write3D
   @date      Wed 12 Dec 2001
   @author    Thomas Radke
   @desc
              Writes the 3D volume of a variable into a gnuplot ASCII file.
   @enddesc
   @calls     Hyperslab_GlobalMappingByIndex
              Hyperslab_FreeMapping
              Hyperslab_GetList
              OpenFile
              WriteData

   @var       GH
   @vdesc     Pointer to CCTK GH
   @vtype     const cGH *
   @vio       in
   @endvar
   @var       vindex
   @vdesc     index of variable to output
   @vtype     int
   @vio       in
   @endvar
   @var       alias
   @vdesc     alias name of variable to output
   @vtype     const char *
   @vio       in
   @endvar

   @returntype int
   @returndesc
                0 for success, or<BR>
               -1 if variable has no storage assigned<BR>
               -2 if output file couldn't be opened<BR>
               -3 if hyperslab for coordinates and/or variable couldn't be
                  extracted
   @endreturndesc
@@*/
int IOASCII_Write3D (const cGH *GH, int vindex, const char *alias)
{
  int i, total_hsize;
  int myproc, gindex, have_coords;
  int num_requested_hslabs, num_returned_hslabs;
  char header[40], buffer[30];
  char format[3][64];
  FILE *file;
  cGroup gdata;
  CCTK_INT coord_system_handle, coord_handles[3];
  char *fullname, *groupname;
  void *hdata[4];
  int extent_int[3];
  int mapping;
  CCTK_INT vindices[4], extent[3], downsample[3], hsize[3];
  const CCTK_INT origin[] = {0, 0, 0},
                 direction[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  DECLARE_CCTK_PARAMETERS


  /* get the variable group information */
  fullname = CCTK_FullName (vindex);
  gindex = CCTK_GroupIndexFromVarI (vindex);
  CCTK_GroupData (gindex, &gdata);

  /* check if variable has storage assigned */
  if (! CCTK_QueryGroupStorageI (GH, gindex))
  {
    CCTK_VWarn (2, __LINE__, __FILE__, CCTK_THORNSTRING,
                "No IOASCII 3D output for '%s' (no storage)", fullname);
    free (fullname);
    return (-1);
  }

  vindices[0] = vindex;
  have_coords = CCTK_IsFunctionAliased ("Coord_GroupSystem");
  if (have_coords)
  {
    /* get the coordinate system associated with this grid variable */
    groupname = CCTK_GroupName (gindex);
    coord_system_handle = Coord_GroupSystem (GH, groupname);
    free (groupname);

    have_coords = coord_system_handle >= 0 &&
                  Util_TableGetIntArray (coord_system_handle, 3,
                                         coord_handles, "COORDINATES") >= 0;
  }
  if (have_coords)
  {
    /* get the coordinate functions and coordinate physical minimum */
    for (i = 1; i <= 3; i++)
    {
      vindices[i] = -1;
      Util_TableGetInt (coord_handles[i-1], &vindices[i], "GAINDEX");
      have_coords &= vindices[i] >= 0;
    }
  }
  num_requested_hslabs = have_coords ? 4 : 1;

  /* set header and data format strings */
  sprintf (buffer, "#Time = %%%s\n", out_format);
  sprintf (header, buffer, (double) GH->cctk_time);
  i = 0;
  if (CCTK_Equals (out3D_style, "gnuplot f(t,x,y,z)"))
  {
    sprintf (buffer, "%%%s\t\t", out_format);
    sprintf (format[0], buffer, (double) GH->cctk_time);
    sprintf (format[1], buffer, (double) GH->cctk_time);
    i = (int) strlen (format[0]);
  }
  if (have_coords)
  {
    sprintf (format[0] + i, "%%%s\t\t%%%s\t\t%%d\t\t%%d",
             out_format, out_format);
    sprintf (format[1] + i, "%%%s\t\t%%%s\t\t%%%s\t\t%%%s",
             out_format, out_format, out_format, out_format);
  }
  else
  {
    sprintf (format[0] + i, "%%d\t\t%%d\t\t%%d\t\t%%d");
    sprintf (format[1] + i, "%%d\t\t%%d\t\t%%d\t\t%%%s", out_format);
  }
  sprintf (format[2], "\t\t%%%s", out_format);

  /* What processor are we on? */
  myproc = CCTK_MyProc (GH);

  /* Open the file on processor 0 */
  file = myproc == 0 ? OpenFile (GH, fullname, alias) : NULL;

  /* get the total number of grid points to check for zero-sized variables */
  /* set the extent vector (copy from 'int' to 'CCTK_INT' */
  CCTK_GroupgshVI (GH, 3, extent_int, vindex);
  for (i = 0, total_hsize = 1; i < 3; i++)
  {
    total_hsize *= extent_int[i];
    extent[i] = extent_int[i];
  }
  if (total_hsize <= 0)
  {
    free (fullname);
    return (0);
  }

  downsample[0] = out_downsample_x;
  downsample[1] = out_downsample_y;
  downsample[2] = out_downsample_z;

  /* get the hyperslab mapping */
  mapping = Hyperslab_GlobalMappingByIndex (GH, vindex, 3,
                                            direction, origin, extent,
                                            downsample,
                                            -1,   /* table handle */
                                            NULL  /* conversion fn */,
                                            hsize);
  if (mapping < 0)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOASCII_Write3D: Failed to define hyperslab mapping for "
                "variable '%s'", fullname);
    free (fullname);
    return (-1);
  }
  total_hsize = hsize[0] * hsize[1] * hsize[2];
  if (total_hsize <= 0)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOASCII_Write3D: selected hyperslab has zero size for "
                "variable '%s'", fullname);
    Hyperslab_FreeMapping (mapping);
    free (fullname);
    return (-1);
  }

  hdata[0] = hdata[1] = hdata[2] = hdata[3];
  if (myproc == 0)
  {
    /* allocate hyperslab buffers */
    hdata[0] = malloc (total_hsize * CCTK_VarTypeSize (gdata.vartype));
    hdata[1] = have_coords ? malloc (3 * total_hsize * sizeof (CCTK_REAL)) :
                             NULL;
    hdata[2] = (CCTK_REAL *) hdata[1] + 1*total_hsize;
    hdata[3] = (CCTK_REAL *) hdata[1] + 2*total_hsize;
  }
 
  /* get the hyperslabs */
  num_returned_hslabs = Hyperslab_GetList (GH, mapping, num_requested_hslabs,
                                           NULL, vindices, NULL, NULL, hdata,
                                           NULL);

  /* And dump the data to file */
  if (myproc == 0)
  {
    if (num_returned_hslabs == num_requested_hslabs)
    {
      WriteData (gdata.vartype, header, format, hsize, hdata, file);
    }
    else
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOASCII_Write3D: Failed to extract hyperslab for "
                  "variable '%s'", fullname);
    }

    /* clean up */
    free (hdata[0]);
    free (hdata[1]);
  } /* end of outputting the data by processor 0 */

  free (fullname);

  return (0);
}


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
/*@@
   @routine    OpenFile
   @date       Wed 12 Dec 2001
   @author     Thomas Radke
   @desc
               Opens an ASCII file for a given alias name.
               If this is the first time through, it will create the file,
               write some header information, and advertise it to IOUtil.
   @enddesc

   @returntype FILE *
   @returndesc
               the file descriptor, or NULL if the file couldn't be created
   @endreturndesc
 @@*/
static FILE *OpenFile (const cGH *GH, const char *fullname, const char *alias)
{
  FILE **file;
  char *filename;
  char buffer[128];
  asciiioGH *myGH;
  ioAdvertisedFileDesc advertised_file;
  DECLARE_CCTK_PARAMETERS


  /* get handle for IOASCII extension */
  myGH = CCTK_GHExtension (GH, "IOASCII");

  /* see if we are the first time through */
  file = GetNamedData (myGH->fileList_3D, alias);
  if (file)
  {
    return (*file);
  }

  file = malloc (sizeof (FILE *));
  filename = malloc (strlen (myGH->out3D_dir) + strlen (alias) + 9);

  /* open/create the file */
  sprintf (filename, "%s%s_3D.asc", myGH->out3D_dir, alias);

  /* if restart from recovery, try to open an existing file ... */
  *file = fopen (filename, IO_TruncateOutputFiles (GH) ? "w" : "a");
  if (! *file)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "Cannot open 3D IOASCII output file '%s'", filename);
    return (NULL);
  }

  /* advertise the file for downloading and write file info */
  advertised_file.slice = "";
  advertised_file.thorn = CCTK_THORNSTRING;
  advertised_file.varname = fullname;
  advertised_file.description = "Full-dimensional variable contents";
  advertised_file.mimetype = "application/gnuplot";

  IOUtil_AdvertiseFile (GH, filename, &advertised_file);

  if (CCTK_Equals (out_fileinfo, "parameter filename") ||
      CCTK_Equals (out_fileinfo, "all"))
  {
    buffer[0] = 0;
    CCTK_ParameterFilename (sizeof (buffer), buffer);
    fprintf (*file, "#Parameter file %s\n", buffer);
  }
  if (CCTK_Equals (out_fileinfo, "creation date") ||
      CCTK_Equals (out_fileinfo, "all"))
  {
    buffer[0] = 0;
    Util_CurrentDate (sizeof (buffer), buffer);
    fprintf (*file, "#Created %s ", buffer);
    Util_CurrentTime (sizeof (buffer), buffer);
    fprintf (*file, "%s\n", buffer);
  }
  if (CCTK_Equals (out_fileinfo, "axis labels") ||
      CCTK_Equals (out_fileinfo, "all"))
  {
    fprintf (*file, "#z-label %s\n", advertised_file.varname);
  }
  if (CCTK_Equals (out_fileinfo, "all"))
  {
    fprintf (*file, "\n\n");
  }

  /* store file desriptors in database */
  StoreNamedData (&myGH->fileList_3D, alias, file);

  free (filename);

  return (*file);
}


/*@@
   @routine   WriteData
   @date      Wed 12 Dec 2001
   @author    Thomas Radke
   @desc
              Writes the given hyperslab data into a file.
   @enddesc
 @@*/
static void WriteData (int vtype,
                       const char *header,
                       char format[3][64],
                       const CCTK_INT hsize[3],
                       void *const *const hdata,
                       FILE *file)
{
  int is_complex;


  if (file == NULL)
  {
    return;
  }

  is_complex = !strncmp (CCTK_VarTypeName (vtype), "CCTK_VARIABLE_COMPLEX", 21);

  /* print out header */
  fputs (header, file);

  if (vtype == CCTK_VARIABLE_BYTE)
  {
    WRITE_DATA (CCTK_BYTE, int, format[0]);
  }
  else if (vtype == CCTK_VARIABLE_INT)
  {
    WRITE_DATA (CCTK_INT, int, format[0]);
  }
  else if (vtype == CCTK_VARIABLE_REAL || vtype == CCTK_VARIABLE_COMPLEX)
  {
    WRITE_DATA (CCTK_REAL, double, format[1]);
  }

#ifdef CCTK_INT1
  else if (vtype == CCTK_VARIABLE_INT1)
  {
    WRITE_DATA (CCTK_INT1, int, format[0]);
  }
#endif

#ifdef CCTK_INT2
  else if (vtype == CCTK_VARIABLE_INT2)
  {
    WRITE_DATA (CCTK_INT2, int, format[0]);
  }
#endif

#ifdef CCTK_INT4
  else if (vtype == CCTK_VARIABLE_INT4)
  {
    WRITE_DATA (CCTK_INT4, int, format[0]);
  }
#endif

#ifdef CCTK_INT8
  else if (vtype == CCTK_VARIABLE_INT8)
  {
    WRITE_DATA (CCTK_INT8, int, format[0]);
  }
#endif

#ifdef CCTK_REAL4
  else if (vtype == CCTK_VARIABLE_REAL4 || vtype == CCTK_VARIABLE_COMPLEX8)
  {
    WRITE_DATA (CCTK_REAL4, double, format[1]);
  }
#endif

#ifdef CCTK_REAL8
  else if (vtype == CCTK_VARIABLE_REAL8 || vtype == CCTK_VARIABLE_COMPLEX16)
  {
    WRITE_DATA (CCTK_REAL8, double, format[1]);
  }
#endif

#ifdef CCTK_REAL16
  else if (vtype == CCTK_VARIABLE_REAL16 || vtype == CCTK_VARIABLE_COMPLEX32)
  {
    WRITE_DATA (CCTK_REAL16, double, format[1]);
  }
#endif

  else
  {
    CCTK_WARN (1, "Unsupported variable type");
  }

  /* keep the file open but flush it */
  fflush (file);
}
