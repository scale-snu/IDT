/*@@
   @routine    WriteScalar.c
   @date       18th September 1999
   @author     Gabrielle Allen
   @desc
               Dumps data for IOBasic's "Scalar" I/O method to output files
   @enddesc
   @version    $Id: WriteScalar.c 169 2005-11-01 18:50:38Z yye00 $
@@*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>      /* strlen(3) */
#include <sys/types.h>
#include <sys/stat.h>    /* stat(2) */

#ifdef SPEC 
#include "IOBasic/cctk.h" 
#define THORN_DIR "IOBasic" 
#else 
#include "cctk.h"
#endif /* END SPEC */ 
#ifdef SPEC 
#include "IOBasic/cctk_Parameters.h" 
#else 
#include "cctk_Parameters.h"
#endif /* END SPEC */ 
#include "CactusBase/IOUtil/src/ioutil_AdvertisedFiles.h"
#include "iobasicGH.h"

static const char *rcsid = "$Header$";

CCTK_FILEVERSION(CactusBase_IOBasic_WriteScalar_c)


static int IOBasic_WriteScalarGS (const cGH *GH, int vindex, const char *alias);
static int IOBasic_WriteScalarGA (const cGH *GH, int vindex, const char *alias);
static FILE *OpenScalarFile (const cGH *GH,
                             int vindex,
                             const char *filename,
                             const char *slicename,
                             const char *description,
                             const char *aliasname);

 /*@@
   @routine   IOBasic_WriteScalar
   @date      Mon Jun 19 2000
   @author    Thomas Radke
   @desc
              Write a scalar value of a CCTK variable into an ASCII file
              suitable for postprocessing by either gnuplot or xgraph.
              This routine just calls the appropriate output routine
              according to the variable type.
   @enddesc

   @calls     CCTK_QueryGroupStorageI
              CCTK_GroupIndexFromVarI
              CCTK_GroupTypeFromVarI
              IOBasic_WriteScalarGS
              IOBasic_WriteScalarGA

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
               return code of either @seeroutine IOBasic_WriteScalarGS or
               @seeroutine IOBasic_WriteScalarGA, or
               -1 if variable has no storage assigned
   @endreturndesc
@@*/
int IOBasic_WriteScalar (const cGH *GH, int vindex, const char *alias)
{
  int retval;
  char *fullname;


  /* check if variable has storage assigned */
  if (! CCTK_QueryGroupStorageI (GH, CCTK_GroupIndexFromVarI (vindex)))
  {
    fullname = CCTK_FullName (vindex);
    CCTK_VWarn (2, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOBasic_WriteScalar: No scalar output for '%s' (no storage)",
                fullname);
    free (fullname);
    retval = -1;
  }
  else if (CCTK_GroupTypeFromVarI (vindex) == CCTK_SCALAR)
  {
    retval = IOBasic_WriteScalarGS (GH, vindex, alias);
  }
  else
  {
    retval = IOBasic_WriteScalarGA (GH, vindex, alias);
  }

  return (retval);
}


 /*@@
   @routine   IOBasic_WriteScalarGA
   @date      Mon Jun 19 2000
   @author    Thomas Radke
   @desc
              Apply the given reduction operators on the CCTK_ARRAY or
              CCTK_GF variable and output the result into a text file
              suitable for postprocessing by either gnuplot or xgraph.
   @enddesc

   @calls     CCTK_ReduceGridArrays
              CCTK_ReductionHandle
              IOUtil_RestartFromRecovery
              IOUtil_AdvertiseFile

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
                0 for success
   @endreturndesc
@@*/
static int IOBasic_WriteScalarGA (const cGH *GH, int vindex, const char *alias)
{
  iobasicGH *myGH;
  FILE *file;
  char *filename;
  char format_str[15];
  const char *file_extension;
  iobasic_reduction_t *reduction;
  DECLARE_CCTK_PARAMETERS

  int input_array[1];
  CCTK_INT output_array_type_codes[1];
  CCTK_REAL output_value;
  void * reduction_value[1];
 
  input_array[0] = vindex; 
  output_array_type_codes[0] = CCTK_VARIABLE_REAL;
  reduction_value[0] = &output_value;
  
  /* set output format */
  sprintf (format_str, "%%%s\t%%%s\n", out_format, out_format);

  /* set the output file extension and the output style */
  file_extension = CCTK_Equals (outScalar_style, "gnuplot") ? ".asc" : ".xg";

  /* get the GH extension handle for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");
  reduction = myGH->scalar_reductions[vindex].reductions;

  /* now loop over all reduction operators */
  while (reduction)
  {
    /* do the reduction (all processors) */
    reduction->is_valid = CCTK_ReduceGridArrays (GH, 0, reduction->handle, -1, 1, input_array , 1, output_array_type_codes, reduction_value) == 0;
    reduction->value = output_value;
    
    if (! reduction->is_valid)
    {
      CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                  "IOBasic_WriteInfo: Internal error in reduction '%s'",
                  reduction->name);
    }
    else if (CCTK_MyProc (GH) == 0)
    {
      /* dump the reduction value to file by processor 0 */
      if (new_filename_scheme)
      {
        /* allocate filename string buffer and build the filename */
        filename = malloc (strlen (myGH->out_dir) +
                           strlen (alias) + strlen (reduction->name) +
                           strlen (file_extension) + 2);
        sprintf (filename, "%s%s_%s%s", myGH->out_dir, alias,
                                        reduction->name, file_extension);
      }
      else
      {
        /* FIXME: this check can go if we switch to the new filename scheme */
        if (strcmp (reduction->name, "minimum") == 0)
        {
          file_extension = "min";
        }
        else if (strcmp (reduction->name, "maximum") == 0)
        {
          file_extension = "max";
        }
        else if (strcmp (reduction->name, "norm1") == 0)
        {
          file_extension = "nm1";
        }
        else if (strcmp (reduction->name, "norm2") == 0)
        {
          file_extension = "nm2";
        }
        else
        {
          file_extension = reduction->name;
        }

        /* allocate filename string buffer and build the filename */
        filename = malloc (strlen (myGH->out_dir) +
                           strlen (alias) + strlen (file_extension)+5);
        sprintf (filename, "%s%s_%s.tl", myGH->out_dir, alias,
                 file_extension);
      }

      file = OpenScalarFile (GH, vindex, filename, reduction->name,
                             "Reduction on Grid Arrays", alias);
      if (file)
      {
        /* write the data and close the file */
        fprintf (file, format_str, GH->cctk_time, reduction->value);
        fclose (file);
      }
      else
      {
        CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                    "IOBasic_WriteScalarGA: Could not open output file '%s'",
                    filename);
      }
      free (filename);
    }

    reduction = reduction->next;
  }

  return (0);
}


 /*@@
   @routine   IOBasic_WriteScalarGS
   @date      Mon Jun 19 2000
   @author    Thomas Radke
   @desc
              Output the value of a CCTK_SCALAR variable into an ASCII file
              suitable for postprocessing by either gnuplot or xgraph.
   @enddesc

   @calls     CCTK_MyProc
              CCTK_GHExtension
              CCTK_Equals
              OpenScalarFile
              CCTK_VarDataPtrI
              CCTK_VarTypeI
              CCTK_WARN
              CCTK_VWarn

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
                0 for success
   @endreturndesc
@@*/
static int IOBasic_WriteScalarGS (const cGH *GH, int vindex, const char *alias)
{
  FILE *file;
  void *data;
  iobasicGH *myGH;
  char *filename;
  const char *file_extension;
  char format_str_real[15], format_str_int[15];
  DECLARE_CCTK_PARAMETERS


  /* output is done by processor 0 only */
  if (CCTK_MyProc (GH) != 0)
  {
    return (0);
  }

  /* set the output format string for the desired notation */
  sprintf (format_str_real, "%%%s\t%%%s\n", out_format, out_format);
  sprintf (format_str_int,  "%%%s\t%%d\n", out_format);

  /* get the GH extensions for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  /* set the output file extension according to the output style */
  if (new_filename_scheme)
  {
    file_extension = CCTK_Equals (outScalar_style, "gnuplot") ? ".asc" : ".xg";
  }
  else
  {
    file_extension = ".tl";
  }

  /* build the output filename */
  filename = malloc (strlen (myGH->out_dir) + strlen (alias) +
                     strlen (file_extension) + 1);
  sprintf (filename, "%s%s%s", myGH->out_dir, alias, file_extension);

  /* create/reopen the file */
  file = OpenScalarFile (GH, vindex, filename, "tl", "Scalar value", alias);
  if (file)
  {
    /* get the data pointer */
    data = CCTK_VarDataPtrI (GH, 0, vindex);

    switch (CCTK_VarTypeI (vindex))
    {
      case CCTK_VARIABLE_REAL:
        fprintf (file, format_str_real, GH->cctk_time,
                 (double) *(CCTK_REAL *) data);
        break;
      case CCTK_VARIABLE_INT:
        fprintf (file, format_str_int, GH->cctk_time,
                 (int) *(CCTK_INT *) data);
        break;
      default:
        CCTK_WARN (3, "Unsupported data type");
        break;
    }

    /* close the output file */
    fclose (file);
  }
  else
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "IOBasic_WriteScalarGS: Could not open output file '%s'",
                filename);
  }

  /* clean up */
  free (filename);

  return (0);
}


static FILE *OpenScalarFile (const cGH *GH,
                             int vindex,
                             const char *filename,
                             const char *slicename,
                             const char *description,
                             const char *aliasname)
{
  int first_time_through;
  FILE *file;
  iobasicGH *myGH;
  char *fullname;
  char comment_char, buffer[128];
  ioAdvertisedFileDesc advertised_file;
  DECLARE_CCTK_PARAMETERS


  /* get the GH extension handle for IOBasic */
  myGH = CCTK_GHExtension (GH, "IOBasic");

  /* get the variable's full name */
  fullname = CCTK_FullName (vindex);

  /* create the output file the first time through
     If restart from recovery, existing files are opened in append mode. */
  first_time_through = GetNamedData (myGH->filenameListScalar, filename)==NULL;
  file = fopen (filename,
                IO_TruncateOutputFiles (GH) && first_time_through ? "w" : "a");
  if (! file)
  {
    CCTK_VWarn (1, __LINE__, __FILE__, CCTK_THORNSTRING,
                "OpenScalarFile: Cannot open output file '%s'", filename);
  }
  else if (first_time_through)
  {
    if (CCTK_Equals (outScalar_style, "gnuplot"))
    {
      comment_char = '#';
      advertised_file.mimetype = "application/gnuplot";
    }
    else
    {
      comment_char = '"';    /* this is for xgraph */
      advertised_file.mimetype = "application/x-graph";
    }

    /* just store a non-NULL pointer in database */
    StoreNamedData (&myGH->filenameListScalar, filename, (void *) 1);

    /* advertise the file for downloading */
    advertised_file.slice = slicename;
    advertised_file.thorn = CCTK_THORNSTRING;
    advertised_file.varname = fullname;
    advertised_file.description = description;

    IOUtil_AdvertiseFile (GH, filename, &advertised_file);

    /* write the file info and the header */
    if (CCTK_Equals (out_fileinfo, "parameter filename") ||
        CCTK_Equals (out_fileinfo, "all"))
    {
      buffer[0] = 0;
      CCTK_ParameterFilename (sizeof (buffer), buffer);
      fprintf (file, "%cParameter file %s\n", comment_char, buffer);
    }
    if (CCTK_Equals (out_fileinfo, "creation date") ||
        CCTK_Equals (out_fileinfo, "all"))
    {
      buffer[0] = 0;
      Util_CurrentDate (sizeof (buffer), buffer);
      fprintf (file, "%cCreated %s ", comment_char, buffer);
      Util_CurrentTime (sizeof (buffer), buffer);
      fprintf (file, "%s\n", buffer);
    }
    if (CCTK_Equals (out_fileinfo, "axis labels") ||
        CCTK_Equals (out_fileinfo, "all"))
    {
      fprintf (file, "%cx-label time\n", comment_char);
      fprintf (file, "%cy-label %s\n", comment_char, advertised_file.varname);
    }
    fprintf (file, "%c%s v time\n", comment_char, aliasname);

  }

  free (fullname);

  return (file);
}
