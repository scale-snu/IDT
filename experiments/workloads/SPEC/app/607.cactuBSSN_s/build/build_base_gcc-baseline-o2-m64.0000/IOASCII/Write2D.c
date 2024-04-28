/*@@
   @file      Write2D.c
   @date      Thu May 11 2000
   @author    Thomas Radke
   @desc
              Output two-dimensional slices in ASCII gnuplot format.
   @enddesc
   @version   $Id: Write2D.c 223 2012-11-20 23:14:47Z eschnett $
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
static const char *rcsid = "$Id: Write2D.c 223 2012-11-20 23:14:47Z eschnett $";
CCTK_FILEVERSION(CactusBase_IOASCII_Write2D_c)


/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* Macro to output a 2D slice (with coordinates if available) as typed data
   For readability, the macro definition implicitely uses the following
   variables (which are defined in the calling routine):
     hdata, hsize, is_complex, format[2], file */
#define WRITE_DATA(cctk_type, c_type, fmt)                                    \
        {                                                                     \
          int _i, _j;                                                         \
          const cctk_type *_hdata = hdata[0];                                 \
          const CCTK_REAL *_coord[2];                                         \
          const int _di = is_complex ? 2 : 1;                                 \
                                                                              \
                                                                              \
          _coord[0] = (const CCTK_REAL *) hdata[1];                           \
          _coord[1] = (const CCTK_REAL *) hdata[2];                           \
          for (_i = 0; _i < hsize[0]; _i++)                                   \
          {                                                                   \
            for (_j = 0; _j < hsize[1]; _j++)                                 \
            {                                                                 \
              if (_coord[0])                                                  \
              {                                                               \
                fprintf (file, fmt, (double) _coord[0][_i+hsize[0]*_j],       \
                         (double) _coord[1][_i+hsize[0]*_j],                  \
                         (c_type) _hdata[_di*(_i+hsize[0]*_j)]);              \
              }                                                               \
              else                                                            \
              {                                                               \
                fprintf (file, fmt, _i, _j,                                   \
                         (c_type) _hdata[_di*(_i+hsize[0]*_j)]);              \
              }                                                               \
                                                                              \
              if (is_complex)                                                 \
              {                                                               \
                fprintf (file, format[2],                                     \
                         (c_type) _hdata[_di*(_i+hsize[0]*_j)+1]);            \
              }                                                               \
              fputc ('\n', file);                                             \
            }                                                                 \
            fputc ('\n', file);                                               \
          }                                                                   \
          _coord[0] += hsize[0]*_j;                                           \
          _coord[1] += hsize[0]*_j;                                           \
          _hdata += _di*hsize[0]*_j;                                          \
        }


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static FILE **OpenFile (const cGH *GH,
                        const char *fullname,
                        const char *alias,
                        int dim,
                        const CCTK_REAL *coord_lower,
                        int maxdir);
static void WriteData (int vtype,
                       const char *header,
                       char format[3][64],
                       const CCTK_INT hsize[2],
                       void *const *const hdata,
                       FILE *file);


/*@@
   @routine   IOASCII_Write2D
   @date      Thu May 11 2000
   @author    Thomas Radke
   @desc
              Writes the 2D slices of a variable into separate ASCII files.
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
int IOASCII_Write2D (const cGH *GH, int vindex, const char *alias)
{
  asciiioGH *myGH;
  int i, total_hsize, num_requested_hslabs, num_returned_hslabs;
  int dir, dir_i, dir_j, maxdir, myproc, gindex, have_coords;
  int mapping;
  cGroup gdata;
  int coord_index[3];
  CCTK_INT coord_system_handle, coord_handles[3];
  CCTK_REAL coord_lower[3];
  char format[3][64];
  char header[40], buffer[30];
  char *fullname, *groupname;
  int extent_int[3];
  CCTK_INT vindices[3], origin[3], extent[2], direction[6], downsample[2],
           hsize[2];
  void *hdata[3];
  FILE **fileset;
  DECLARE_CCTK_PARAMETERS


  /* get the variable name and group information */
  fullname = CCTK_FullName (vindex);
  gindex = CCTK_GroupIndexFromVarI (vindex);
  CCTK_GroupData (gindex, &gdata);

  /* check if variable has storage assigned */
  if (! CCTK_QueryGroupStorageI (GH, gindex))
  {
    CCTK_VWarn (2, __LINE__, __FILE__, CCTK_THORNSTRING,
                "No IOASCII 2D output for '%s' (no storage)", fullname);
    free (fullname);
    return (-1);
  }

  /* get the handle for IOASCII extension */
  myGH = CCTK_GHExtension (GH, "IOASCII");

  /* get the number of slices to output */
  /* in general: maxdir = gdata.dim * (gdata.dim - 1) / 2; */
  maxdir = gdata.dim == 2 ? 1 : 3;

  dir = gdata.dim < 3 ? gdata.dim : 3;

  have_coords = CCTK_IsFunctionAliased ("Coord_GroupSystem");
  if (have_coords)
  {
    /* get the coordinate system associated with this grid variable */
    groupname = CCTK_GroupName (gindex);
    coord_system_handle = Coord_GroupSystem (GH, groupname);
    free (groupname);

    have_coords = coord_system_handle >= 0 &&
                  Util_TableGetIntArray (coord_system_handle, dir,
                                         coord_handles, "COORDINATES") >= 0;
  }
  if (have_coords)
  {
    /* get the coordinate functions and coordinate physical minimum */
    for (i = 0; i < dir; i++)
    {
      coord_index[i] = -1;
      coord_lower[i] = 0;
      Util_TableGetInt (coord_handles[i], &coord_index[i], "GAINDEX");
      Util_TableGetReal (coord_handles[i], &coord_lower[i], "COMPMIN");
      have_coords &= coord_index[i] >= 0;
    }
  }
  num_requested_hslabs = have_coords ? 3 : 1;

  /* set header and data format strings */
  sprintf (buffer, "\n\n#Time = %%%s\n", out_format);
  sprintf (header, buffer, GH->cctk_time);
  dir = 0;
  if (CCTK_Equals (out2D_style, "gnuplot f(t,x,y)"))
  {
    sprintf (buffer,  "%%%s\t\t", out_format);
    sprintf (format[0], buffer, (double) GH->cctk_time);
    sprintf (format[1], buffer, (double) GH->cctk_time);
    dir = strlen (format[0]);
  }
  if (have_coords)
  {
    sprintf (format[0] + dir, "%%%s\t\t%%%s\t\t%%d",
             out_format, out_format);
    sprintf (format[1] + dir, "%%%s\t\t%%%s\t\t%%%s",
             out_format, out_format, out_format);
  }
  else
  {
    sprintf (format[0] + dir, "%%d\t\t%%d\t\t%%d");
    sprintf (format[1] + dir, "%%d\t\t%%d\t\t%%%s", out_format);
  }
  sprintf (format[2], "\t\t%%%s", out_format);

  /* processor 0 opens the files on the first trip through */
  fileset = NULL;
  myproc = CCTK_MyProc (GH);
  if (myproc == 0)
  {
    fileset = OpenFile (GH, fullname, alias, gdata.dim,
                        have_coords ? coord_lower : NULL, maxdir);
  }

  /* get the extents of the variable */
  CCTK_GroupgshVI (GH, gdata.dim, extent_int, vindex);

  /* get the total number of grid points to check for zero-sized variables */
  for (dir = 0, hsize[0] = 1; dir < gdata.dim; dir++)
  {
    hsize[0] *= extent_int[dir];
  }

  /* now do the actual I/O looping over all directions */
  for (dir = 0; dir < maxdir; dir++)
  {
    if (hsize[0] <= 0)
    {
      continue;
    }

    /* get the directions to span the hyperslab */
    if (dir == 0)
    {
      dir_i = 0; dir_j = 1;   /* xy */
      downsample[0] = out_downsample_x; downsample[1] = out_downsample_y;
    }
    else if (dir == 1)
    {
      dir_i = 0; dir_j = 2;   /* xz */
      downsample[0] = out_downsample_x; downsample[1] = out_downsample_z;
    }
    else
    {
      dir_i = 1; dir_j = 2;   /* yz */
      downsample[0] = out_downsample_y; downsample[1] = out_downsample_z;
    }

    /* set the extent vector */
    extent[0] = extent_int[dir_i];
    extent[1] = extent_int[dir_j];

    /* set the origin using the slice center from IOUtil */
    memset (origin, 0, sizeof (origin));
    if (have_coords)
    {
      origin[maxdir-dir-1] = myGH->sp2xyz[gdata.dim-1][dir];
    }

    /* set the direction vector */
    memset (direction, 0, sizeof (direction));
    direction[dir_i] = direction[gdata.dim + dir_j] = 1;

    mapping = Hyperslab_GlobalMappingByIndex (GH, vindex, 2,
                                              direction, origin, extent,
                                              downsample,
                                              -1,   /* table handle */
                                              NULL  /* conversion fn */,
                                              hsize);
    if (mapping < 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOASCII_Write2D: Failed to define hyperslab mapping for "
                  "variable '%s'", fullname);
      continue;
    }
    total_hsize = hsize[0] * hsize[1];
    if (total_hsize <= 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOASCII_Write2D: selected hyperslab has zero size for "
                  "variable '%s' direction %d", fullname, dir);
      Hyperslab_FreeMapping (mapping);
      continue;
    }

    hdata[0] = hdata[1] = hdata[2];
    if (myproc == 0)
    {
      /* allocate hyperslab buffers */
      hdata[0] = malloc (total_hsize * CCTK_VarTypeSize (gdata.vartype));
      hdata[1] = have_coords ? malloc (2 * total_hsize * sizeof (CCTK_REAL)) :
                               NULL;
      hdata[2] = (CCTK_REAL *) hdata[1] + total_hsize;
    }

    /* get the hyperslabs */
    vindices[0] = vindex;
    vindices[1] = coord_index[dir_i];
    vindices[2] = coord_index[dir_j];
    num_returned_hslabs = Hyperslab_GetList (GH, mapping, num_requested_hslabs,
                                             NULL, vindices, NULL, NULL, hdata,
                                             NULL);

    /* release the mapping structure */
    Hyperslab_FreeMapping (mapping);

    /* and dump the data to file */
    if (fileset)
    {
      if (num_returned_hslabs == num_requested_hslabs)
      {
        WriteData (gdata.vartype, header, format, hsize, hdata, fileset[dir]);
      }
      else
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "IOASCII_Write2D: Failed to extract hyperslab for "
                    "variable '%s'", fullname);
      }

      /* clean up */
      free (hdata[0]);
      free (hdata[1]);

    } /* end of outputting the data by processor 0 */

  } /* end of looping through xyz directions */

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
               Opens a set of ASCII files for a given alias name.
               If this is the first time through, it will create the files,
               write some header information, and advertise them to IOUtil.
   @enddesc

   @returntype FILE **
   @returndesc
               the set of open file descriptors,<BR>
               or NULL if a file couldn't be created
   @endreturndesc
 @@*/
static FILE **OpenFile (const cGH *GH,
                        const char *fullname,
                        const char *alias,
                        int dim,
                        const CCTK_REAL *coord_lower,
                        int maxdir)
{
  int dir;
  FILE **fileset;
  char *filename;
  asciiioGH *myGH;
  ioAdvertisedFileDesc advertised_file;
  char slicename[30], zlabel_fmt_string[30], buffer[128];
  const char *extensions[] = {"xy", "xz", "yz"};
  DECLARE_CCTK_PARAMETERS


  /* get handle for IOASCII extension */
  myGH = CCTK_GHExtension (GH, "IOASCII");

  /* see if we are the first time through */
  fileset = (FILE **) GetNamedData (myGH->fileList_2D, alias);
  if (fileset)
  {
    return (fileset);
  }

  sprintf (zlabel_fmt_string, " (%%c = %%%s),", out_format);
  fileset = malloc (3 * sizeof (FILE *));
  filename = malloc (strlen (myGH->out2D_dir) + strlen (alias) +
                     sizeof (slicename) + 20);

  /* open/create files for each slice */
  for (dir = 0; dir < maxdir; dir++)
  {
    if (new_filename_scheme)
    {
      if (dim == 2)
      {
        strcpy (slicename, "2D");
      }
      else
      {
        /* give the slice origin as range [1 .. n] */
        sprintf (slicename, "%s_[%d]", extensions[dir],
                 myGH->sp2xyz[dim-1][dir]);
      }

      sprintf (filename, "%s%s_%s.asc", myGH->out2D_dir, alias, slicename);
    }
    else
    {
      if (dim == 2)
      {
        sprintf (filename, "%s%s_2D.gnuplot", myGH->out2D_dir, alias);
      }
      else
      {
        sprintf (filename, "%s%s_2d_%s.gnuplot", myGH->out2D_dir, alias,
                 extensions[dir]);
      }
    }

    /* if restart from recovery, try to open an existing file ... */
    fileset[dir] = fopen (filename, IO_TruncateOutputFiles (GH) ? "w" : "a");
    if (! fileset[dir])
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "Cannot open 2D output file '%s'", filename);
      free (fileset);
      free (filename);
      return (NULL);
    }

    /* advertise the file for downloading and write file info */
    advertised_file.slice = new_filename_scheme ?  slicename : extensions[dir];
    advertised_file.thorn = CCTK_THORNSTRING;
    advertised_file.varname = fullname;
    advertised_file.description = "Two-dimensional slice plots";
    advertised_file.mimetype = "application/gnuplot";

    IOUtil_AdvertiseFile (GH, filename, &advertised_file);

    if (CCTK_Equals (out_fileinfo, "parameter filename") ||
        CCTK_Equals (out_fileinfo, "all"))
    {
      buffer[0] = 0;
      CCTK_ParameterFilename (sizeof (buffer), buffer);
      fprintf (fileset[dir], "#Parameter file %s\n", buffer);
    }
    if (CCTK_Equals (out_fileinfo, "creation date") ||
        CCTK_Equals (out_fileinfo, "all"))
    {
      buffer[0] = 0;
      Util_CurrentDate (sizeof (buffer), buffer);
      fprintf (fileset[dir], "#Created %s ", buffer);
      Util_CurrentTime (sizeof (buffer), buffer);
      fprintf (fileset[dir], "%s\n", buffer);
    }
    if (CCTK_Equals (out_fileinfo, "axis labels") ||
        CCTK_Equals (out_fileinfo, "all"))
    {
      fprintf (fileset[dir], "#x-label %c\n", extensions[dir][0]);
      fprintf (fileset[dir], "#y-label %c\n", extensions[dir][1]);
      fprintf (fileset[dir], "#z-label %s", advertised_file.varname);
      if (dim != 2)
      {
        if (coord_lower)
        {
          fprintf (fileset[dir], zlabel_fmt_string, 'x' + (maxdir-dir-1),
                   coord_lower[maxdir-dir-1] +
                   GH->cctk_delta_space[maxdir-dir-1] *
                   myGH->sp2xyz[dim-1][dir]);
        }
        fprintf (fileset[dir], " (%ci = %d)",
                 'x' + (maxdir-dir-1), myGH->sp2xyz[dim-1][dir]);
      }
    }
  }

  /* store file desriptors in database */
  StoreNamedData (&myGH->fileList_2D, alias, fileset);

  free (filename);

  return (fileset);
}


/*@@
   @routine   WriteData
   @date      Wed 12 Dec 2001
   @author    Thomas Radke
   @desc
              Writes the given hyperslabs into the appropriate output files.
   @enddesc
 @@*/
static void WriteData (int vtype,
                       const char *header,
                       char format[3][64],
                       const CCTK_INT hsize[2],
                       void *const *const hdata,
                       FILE *file)
{
  int is_complex;
  char complex_fmt_string[30];
  DECLARE_CCTK_PARAMETERS


  is_complex = !strncmp (CCTK_VarTypeName (vtype), "CCTK_VARIABLE_COMPLEX", 21);
  sprintf (complex_fmt_string, "\t\t%%%s", out_format);

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
