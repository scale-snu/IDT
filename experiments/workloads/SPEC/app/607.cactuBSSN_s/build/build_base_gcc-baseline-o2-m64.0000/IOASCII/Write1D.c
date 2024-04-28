/*@@
   @file      Write1D.c
   @date      March 1997
   @author    Paul Walker
   @desc
              Output one-dimensional lines in ASCII xgraph/gnuplot format.
   @enddesc

   @history
   @hauthor   Thomas Radke
   @hdate     30 May 2000
   @hdesc
              Get rid of all the PUGH stuff by using thorn Hyperslab.
   @endhdesc
   @hendhistory
   @version   $Id: Write1D.c 220 2012-07-04 01:28:15Z eschnett $
@@*/

#include <math.h>      /* sqrt(3) */
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
CCTK_FILEVERSION(CactusBase_IOASCII_Write1D_c)


/********************************************************************
 ********************    Macro Definitions   ************************
 ********************************************************************/
/* Macro to output a 1D line (with coordinates if available) as typed data
   For readability, the macro definition implicitely uses the following
   variables (which are defined in the calling routine):
     hdata, hstride, hsize, format[2] */
#define WRITE_DATA(hstart, cctk_type, c_type, is_complex, fmt, file)          \
        {                                                                     \
          int _h;                                                             \
          int _offset = is_complex ? 2 : 1;                                   \
          const cctk_type *_hdata = hdata[0];                                 \
          const CCTK_REAL *_coord = hdata[1];                                 \
                                                                              \
                                                                              \
          _hdata += hstart;                                                   \
          for (_h = 0; _h < hsize; _h++)                                      \
          {                                                                   \
            if (_coord)                                                       \
            {                                                                 \
              fprintf (file, fmt, (double) _coord[_h],                        \
                       (c_type) _hdata[_h * _offset]);                        \
            }                                                                 \
            else                                                              \
            {                                                                 \
              fprintf (file, fmt, _h, (c_type) _hdata[_h * _offset]);         \
            }                                                                 \
                                                                              \
            if (is_complex && hstride == 1)                                   \
            {                                                                 \
              fprintf (file, format[2], (c_type) _hdata[_h * _offset + 1]);   \
            }                                                                 \
            fputc ('\n', file);                                               \
          }                                                                   \
        }


/********************************************************************
 ********************    Internal Routines   ************************
 ********************************************************************/
static void OpenFile (const cGH *GH,
                      const char *fullname,
                      const char *alias,
                      const cGroup *gdata,
                      const int *do_dir,
                      const CCTK_REAL *coord_lower,
                      int num_files,
                      FILE *file[]);
static void WriteData (int vtype,
                       const char *header,
                       char format[3][64],
                       int hsize, int hstride,
                       void *const *const hdata,
                       FILE *const file[]);


/*@@
   @routine IOASCII_Write1D
   @date    March 1999
   @author  Gabrielle Allen
   @desc
            This routine does 1D line output along the orthogonals
            and the diagonal (in case of a cubed grid).
            <p>
            It writes to ASCII files suitable for gnuplot and xgraph.
            A header telling the physical time prefixes the output data.
   @enddesc
   @calls   Hyperslab_GlobalMappingByIndex
            Hyperslab_FreeMapping
            Hyperslab_GetList
            OpenFile
            WriteData

   @var     GH
   @vdesc   Pointer to CCTK GH
   @vtype   const cGH *
   @vio     in
   @endvar
   @var     vindex
   @vdesc   global index of variable to output
   @vtype   int
   @vio     in
   @endvar
   @var     alias
   @vdesc   alias name (used for creating the output filename)
   @vtype   const char *
   @vio     in
   @endvar

   @returntype int
   @returndesc
                0 for success, or<BR>
               -1 if variable has no storage assigned
   @endreturndesc
@@*/
int IOASCII_Write1D (const cGH *GH, int vindex, const char *alias)
{
  asciiioGH *myGH;
  int do_dir[4], coord_index[3];
  int i, dir, myproc, gindex, have_coords, mapping;
  int hstride, num_files, num_requested_hslabs, num_returned_hslabs;
  int *extent_int;
  cGroup gdata;
  char *fullname, *groupname;
  char header[40], buffer[30];
  CCTK_INT coord_system_handle, coord_handles[3];
  char format[3][64];
  FILE *file[8];
  CCTK_REAL offset;
  CCTK_REAL coord_lower[3];
  CCTK_INT downsample[4];
  CCTK_INT *origin, *direction;
  CCTK_INT hsize, extent;
  CCTK_INT vindices[2];
  void *hdata[2];
  DECLARE_CCTK_PARAMETERS


  /* get the variable's group index and its full name */
  gindex = CCTK_GroupIndexFromVarI (vindex);
  fullname = CCTK_FullName (vindex);

  /* check if variable has storage assigned */
  if (! CCTK_QueryGroupStorageI (GH, gindex))
  {
    CCTK_VWarn (2, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOASCII_Write1D: No IOASCII_1D output for '%s' (no storage)",
                fullname);
    free (fullname);
    return (-1);
  }

  /* get the handle for IOASCII extension */
  myGH = CCTK_GHExtension (GH, "IOASCII");

  /* get the variable's group information */
  CCTK_GroupData (gindex, &gdata);

  /* see what slices should be output */
  do_dir[0] = out1D_x && gdata.dim >= 1;
  do_dir[1] = out1D_y && gdata.dim >= 2;
  do_dir[2] = out1D_z && gdata.dim >= 3;
  /* diagonal slice is done only if variable is 3D */
  do_dir[3] = out1D_d && gdata.dim == 3;
  if (out1D_d && ! do_dir[3] && myGH->out1D_last[vindex] < 0)
  {
    CCTK_VWarn (3, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOASCII_Write1D: No IOASCII_1D diagonal output for '%s' "
                "(only implemented for non-staggered 3D variables)",
                fullname);
  }

  /* return if nothing to do */
  if (! (do_dir[0] || do_dir[1] || do_dir[2] || do_dir[3]))
  {
    free (fullname);
    return (0);
  }

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

  /* set header format string */
  if (CCTK_Equals (out1D_style, "xgraph"))
  {
    sprintf (buffer, "\n\n\"Time = %%%s\n", out_format);
  }
  else
  {
    sprintf (buffer, "\n#Time = %%%s\n", out_format);
  }
  sprintf (header, buffer, (double) GH->cctk_time);

  /* check whether to include physical time as separate column in the output */
  i = 0;
  if (CCTK_Equals (out1D_style, "gnuplot f(t,x)"))
  {
    sprintf (buffer, "%%%s\t\t", out_format);
    sprintf (format[0], buffer, (double) GH->cctk_time);
    sprintf (format[1], buffer, (double) GH->cctk_time);
    i = strlen (format[0]);
  }
  if (have_coords)
  {
    sprintf (format[0] + i, "%%%s\t\t%%d", out_format);
    sprintf (format[1] + i, "%%%s\t\t%%%s", out_format, out_format);
  }
  else
  {
    sprintf (format[0] + i, "%%d\t\t%%d");
    sprintf (format[1] + i, "%%d\t\t%%%s", out_format);
  }
  sprintf (format[2], "\t\t%%%s", out_format);

  /* get the stride into the data and the (maximum) number of files to write */
  hstride = gdata.vartype == CCTK_VARIABLE_COMPLEX &&
            CCTK_Equals (out1D_style, "xgraph") ? 2 : 1;
  num_files = hstride * 4;

  /* processor 0 opens the files with the appropriate name */
  myproc = CCTK_MyProc (GH);
  memset (file, 0, sizeof (file));
  if (myproc == 0)
  {
    OpenFile (GH, fullname, alias, &gdata, do_dir,
              have_coords ? coord_lower : NULL, num_files, file);
  }

  origin     = calloc (2*gdata.dim, sizeof (CCTK_INT));
  direction  = origin + gdata.dim;
  extent_int = malloc ((gdata.dim + 1) * sizeof (int));

  /* set downsampling vector from I/O parameters */
  downsample[0] = out_downsample_x;
  downsample[1] = out_downsample_y;
  downsample[2] = out_downsample_z;
  downsample[3] = 1;

  /* get the variable's extents, compute the extent for 3D-diagonals as the
     minimum of grid points in each direction */
  CCTK_GroupgshVI (GH, gdata.dim, extent_int, vindex);
  if (gdata.dim == 3)
  {
    extent_int[3] = extent_int[0] < extent_int[1] ?
                    extent_int[0] : extent_int[1];
    if (extent_int[2] < extent_int[3])
    {
      extent_int[3] = extent_int[2];
    }
  }
  /* get the total number of grid points to check for zero-sized variables */
  for (dir = 0, hsize = 1; dir < gdata.dim; dir++)
  {
    hsize *= extent_int[dir];
  }

  /* now do the actual I/O looping over all directions */
  for (dir = 0; dir < 4; dir++)
  {
    if (hsize <= 0)
    {
      continue;
    }

    /* skip empty slices */
    if (! do_dir[dir])
    {
      continue;
    }

    /* get the number of hyperslabs to extract
       (ie. whether to get a coordinate hyperslab too or not) */
    num_requested_hslabs = have_coords && dir < 3 ? 2 : 1;

    /* set the direction vector */
    for (i = 0; i < gdata.dim; i++)
    {
      direction[i] = (dir == i || dir == 3) ? 1 : 0;
    }

    /* set the extent */
    extent = extent_int[dir];

    /* set the origin of the line */
    if (gdata.grouptype == CCTK_GF && dir < 3)
    {
      for (i = 0; i < gdata.dim; i++)
      {
        origin[i] = myGH->spxyz[gdata.dim-1][dir][i];
      }
      extent -= origin[dir];
    }
    else    /* origin for CCTK_ARRAYS is always (0, 0, 0) */
    {
      memset (origin, 0, gdata.dim * sizeof (CCTK_INT));
    }

    mapping = Hyperslab_GlobalMappingByIndex (GH, vindex, 1,
                                              direction, origin, &extent,
                                              &downsample[dir],
                                              -1,   /* table handle */
                                              NULL  /* conversion fn */,
                                              &hsize);
    if (mapping < 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOASCII_Write1D: Failed to define hyperslab mapping for "
                  "variable '%s'", fullname);
      continue;
    }
    if (hsize <= 0)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOASCII_Write1D: selected hyperslab has zero size for "
                  "variable '%s' direction %d", fullname, dir);
      Hyperslab_FreeMapping (mapping);
      continue;
    }

    /* allocate hyperslab buffers on I/O processor */
    hdata[0] = hdata[1] = NULL;
    if (myproc == 0)
    {
      hdata[0] = malloc (hsize * CCTK_VarTypeSize (gdata.vartype));
      hdata[1] = have_coords ? malloc (hsize * sizeof (CCTK_REAL)) : NULL;
    }

    /* get the hyperslabs */
    vindices[0] = vindex;
    vindices[1] = coord_index[dir];
    num_returned_hslabs = Hyperslab_GetList (GH, mapping, num_requested_hslabs,
                                             NULL, vindices, NULL, NULL, hdata,
                                             NULL);

    /* release the mapping structure */
    Hyperslab_FreeMapping (mapping);

    /* And dump the data to file */
    if (file[dir] != NULL)
    {
      if (num_returned_hslabs != num_requested_hslabs)
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "IOASCII_Write1D: Failed to extract hyperslab for "
                    "variable '%s'", fullname);
      }
      else
      {
        if (have_coords)
        {
          if (dir < 3)
          {
            /* do nothing */
          }
          else
          {
            /* calculate the diagonal coordinates */
            offset = GH->cctk_delta_space[0] * sqrt (3);
            for (i = 0; i < hsize; i++)
            {
              ((CCTK_REAL *) hdata[1])[i] = coord_lower[0]*sqrt (3) + i*offset;
            }
          }
        }

        WriteData (gdata.vartype, header, format, hsize, hstride, hdata,
                   file + dir);
      }
    }

    /* clean up */
    free (hdata[0]);
    free (hdata[1]);
  } /* end of loop through all directions */

  /* close the output file(s) */
  for (dir = 0; dir < 8; dir++)
  {
    if (file[dir])
    {
      fclose (file[dir]);
    }
  }

  /* free allocated resources */
  free (origin);
  free (fullname);
  free (extent_int);

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
 @@*/
static void OpenFile (const cGH *GH,
                      const char *fullname,
                      const char *alias,
                      const cGroup *gdata,
                      const int *do_dir,
                      const CCTK_REAL *coord_lower,
                      int num_files,
                      FILE *file[])
{
  int i, dir, upper, lower, first_time_through;
  asciiioGH *myGH;
  char comment_char;
  char *filename, *type_extension;
  const char *file_extension;
  char slicename[20], buffer[128];
  char ylabel1_fmt_string[13], ylabel2_fmt_string[13];
  ioAdvertisedFileDesc advertised_file;
  const char *extensions[] = {"xl", "yl", "zl", "dl"};
  DECLARE_CCTK_PARAMETERS


  /* get handle for IOASCII extension */
  myGH = CCTK_GHExtension (GH, "IOASCII");

  /* set comment character and file extension */
  if (CCTK_Equals (out1D_style, "xgraph"))
  {
    comment_char = '"';
    file_extension = ".xg";
  }
  else
  {
    comment_char = '#';
    file_extension = ".asc";
  }

  sprintf (ylabel1_fmt_string, " (%%c = %%%s", out_format);
  sprintf (ylabel2_fmt_string, ", %%c = %%%s", out_format);

  /* 20 extra characters should be enough for '/',
     the type extension, the file extension, and the trailing '\0' */
  filename = malloc (strlen (myGH->out1D_dir) + strlen (alias) +
                     sizeof (slicename) + 20);

  for (i = 0; i < num_files; i++)
  {
    dir = i % 4;

    /* skip empty slices */
    if (! do_dir[dir])
    {
      continue;
    }

    /* add suffix for complex type variables in xgraph output */
    if (gdata->vartype == CCTK_VARIABLE_COMPLEX && num_files == 8)
    {
      if (new_filename_scheme)
      {
        type_extension = i < 4 ? "re_" : "im_";
      }
      else
      {
        type_extension = i < 4 ? "_re" : "_im";
      }
    }
    else
    {
      type_extension = "";
    }

    /* get the indices into spxyz[] */
    lower = (dir + 1) % 3;
    upper = (dir + 2) % 3;
    if (upper < lower)
    {
      upper = lower;
      lower = 0;
    }

    /* FIXME: this can go after the old filename scheme has gone */
    if (new_filename_scheme)
    {
      if ((i + 1) % 4)
      {
        if (gdata->dim == 1)
        {
          sprintf (slicename, "%s1D", type_extension);
        }
        else if (gdata->dim == 2)
        {
          /* give the slice origin as range [1 .. n] */
          sprintf (slicename, "%s%c_[%d]", type_extension, 'x' + dir,
                   gdata->grouptype == CCTK_GF ?
                   myGH->spxyz[gdata->dim-1][dir][lower] : 0);
        }
        else
        {
          /* give the slice origin as range [1 .. n] */
          sprintf (slicename, "%s%c_[%d][%d]", type_extension, 'x' + dir,
                   gdata->grouptype == CCTK_GF ?
                   myGH->spxyz[gdata->dim-1][dir][lower] : 0,
                   gdata->grouptype == CCTK_GF ?
                   myGH->spxyz[gdata->dim-1][dir][upper] : 0);
        }
      }
      else
      {
        sprintf (slicename, "%s%dD_diagonal", type_extension, gdata->dim);
      }

      sprintf (filename, "%s%s_%s%s", myGH->out1D_dir, alias, slicename,
               file_extension);
    }
    else
    {
      sprintf (filename, "%s%s%s.%s", myGH->out1D_dir, alias, type_extension,
               extensions[dir]);
    }

    /* create the output file the first time through
       If restart from recovery, existing files are opened in append mode. */
    first_time_through = GetNamedData (myGH->filenameList1D, filename) == NULL;
    file[i] = fopen (filename,
                     IO_TruncateOutputFiles (GH) && first_time_through ?  "w" : "a");
    if (! file[i])
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOASCII_Write1D: Cannot open 1D output file '%s'", filename);
    }
    else if (first_time_through)
    {
      /* advertise new files for downloading and add file info */
      /* FIXME: this can go after the old filename scheme has gone */
      advertised_file.slice = new_filename_scheme ? slicename:extensions[dir];
      advertised_file.thorn = CCTK_THORNSTRING;
      advertised_file.varname = fullname;
      advertised_file.description = "One-dimensional line plots";
      advertised_file.mimetype = comment_char == '#' ?
                           "application/gnuplot" : "application/x-graph";

      IOUtil_AdvertiseFile (GH, filename, &advertised_file);

      /* mark a restart point with an empty line in the output file,
         so that gnuplot/xgraph don't try to connect the last output
         before the abort and the first output after the restart */
      if (! IO_TruncateOutputFiles (GH))
      {
        fputc ('\n', file[i]);
      }

      /* add some file information to the output file */
      if (CCTK_Equals (out_fileinfo, "parameter filename") ||
          CCTK_Equals (out_fileinfo, "all"))
      {
        buffer[0] = 0;
        CCTK_ParameterFilename (sizeof (buffer), buffer);
        fprintf (file[i], "%cParameter file %s\n", comment_char, buffer);
      }
      if (CCTK_Equals (out_fileinfo, "creation date") ||
          CCTK_Equals (out_fileinfo, "all"))
      {
        buffer[0] = 0;
        Util_CurrentDate (sizeof (buffer), buffer);
        fprintf (file[i], "%cCreated %s ", comment_char, buffer);
        Util_CurrentTime (sizeof (buffer), buffer);
        fprintf (file[i], "%s\n", buffer);
      }
      if (CCTK_Equals (out_fileinfo, "axis labels") ||
          CCTK_Equals (out_fileinfo, "all"))
      {
        if (dir < 3)
        {
          fprintf (file[i], "%cx-label %c\n", comment_char, 'x' + dir);
        }
        else
        {
          fprintf (file[i], "%cx-label diagonal\n", comment_char);
        }
        fprintf (file[i], "%cy-label %s", comment_char, fullname);
        if (dir < 3 && gdata->dim > 1)
        {
          /* output the physical coordinates of the 1D line */
          if (coord_lower)
          {
            fprintf (file[i], ylabel1_fmt_string, 'x' + lower,
                     coord_lower[lower] + GH->cctk_delta_space[lower] *
                     myGH->spxyz[gdata->dim-1][dir][lower]);
            if (gdata->dim > 2)
            {
              fprintf (file[i], ylabel2_fmt_string, 'x' + upper,
                       coord_lower[upper] + GH->cctk_delta_space[upper] *
                       myGH->spxyz[gdata->dim-1][dir][upper]);
            }
            fputs ("),", file[i]);
          }
          /* output the index coordinates of the 1D line */
          fprintf (file[i], " (%ci = %d", 'x' + lower,
                   gdata->grouptype == CCTK_GF ?
                   myGH->spxyz[gdata->dim-1][dir][lower] : 0);
          if (gdata->dim > 2)
          {
            fprintf (file[i], ", %ci = %d", 'x' + upper,
                     gdata->grouptype == CCTK_GF ?
                     myGH->spxyz[gdata->dim-1][dir][upper] : 0);
          }
          fputs (")", file[i]);
        }
        fputc ('\n', file[i]);
      }

      StoreNamedData (&myGH->filenameList1D, filename, file[i]);
    }
  }

  free (filename);
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
                       int hsize, int hstride,
                       void *const *const hdata,
                       FILE *const file[])
{
  /* print out header */
  fputs (header, file[0]);

  if (vtype == CCTK_VARIABLE_BYTE)
  {
    WRITE_DATA (0, CCTK_BYTE, int, 0, format[0], file[0]);
  }
  else if (vtype == CCTK_VARIABLE_INT)
  {
    WRITE_DATA (0, CCTK_INT, int, 0, format[0], file[0]);
  }
  else if (vtype == CCTK_VARIABLE_REAL)
  {
    WRITE_DATA (0, CCTK_REAL, double, 0, format[1], file[0]);
  }
  else if (vtype == CCTK_VARIABLE_COMPLEX)
  {
    WRITE_DATA (0, CCTK_REAL, double, 1, format[1], file[0]);
    if (hstride == 2)
    {
      /* print out header */
      fputs (header, file[0 + 4]);
      WRITE_DATA (1, CCTK_REAL, double, 1, format[1], file[4]);
    }
  }
#ifdef CCTK_INT1
  else if (vtype == CCTK_VARIABLE_INT1)
  {
    WRITE_DATA (0, CCTK_INT1, int, 0, format[0], file[0]);
  }
#endif

#ifdef CCTK_INT2
  else if (vtype == CCTK_VARIABLE_INT2)
  {
    WRITE_DATA (0, CCTK_INT2, int, 0, format[0], file[0]);
  }
#endif

#ifdef CCTK_INT4
  else if (vtype == CCTK_VARIABLE_INT4)
  {
    WRITE_DATA (0, CCTK_INT4, int, 0, format[0], file[0]);
  }
#endif

#ifdef CCTK_INT8
  else if (vtype == CCTK_VARIABLE_INT8)
  {
    WRITE_DATA (0, CCTK_INT8, int, 0, format[0], file[0]);
  }
#endif

#ifdef CCTK_REAL4
  else if (vtype == CCTK_VARIABLE_REAL4)
  {
    WRITE_DATA (0, CCTK_REAL4, double, 0, format[1], file[0]);
  }
  else if (vtype == CCTK_VARIABLE_COMPLEX8)
  {
    WRITE_DATA (0, CCTK_REAL4, double, 1, format[1], file[0]);
    if (hstride == 2)
    {
      /* print out header */
      fputs (header, file[0 + 4]);
      WRITE_DATA (1, CCTK_REAL4, double, 1, format[1], file[0 + 4]);
    }
  }
#endif

#ifdef CCTK_REAL8
  else if (vtype == CCTK_VARIABLE_REAL8)
  {
    WRITE_DATA (0, CCTK_REAL8, double, 0, format[1], file[0]);
  }
  else if (vtype == CCTK_VARIABLE_COMPLEX16)
  {
    WRITE_DATA (0, CCTK_REAL8, double, 1, format[1], file[0]);
    if (hstride == 2)
    {
      /* print out header */
      fputs (header, file[0 + 4]);
      WRITE_DATA (1, CCTK_REAL8, double, 1, format[1], file[0 + 4]);
    }
  }
#endif

#ifdef CCTK_REAL16
  else if (vtype == CCTK_VARIABLE_REAL16)
  {
    WRITE_DATA (0, CCTK_REAL16, double, 0, format[1], file[0]);
  }
  else if (vtype == CCTK_VARIABLE_COMPLEX32)
  {
    WRITE_DATA (0, CCTK_REAL16, double, 1, format[1], file[0]);
    if (hstride == 2)
    {
      /* print out header */
      fputs (header, file[0 + 4]);
      WRITE_DATA (1, CCTK_REAL16, double, 1, format[1], file[0 + 4]);
    }
  }
#endif

  else
  {
    CCTK_WARN (1, "Unsupported variable type");
  }
}
