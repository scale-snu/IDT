/*@@
   @file      WarnLevel.c
   @date      Wed Feb 17 00:30:09 1999
   @author    Tom Goodale
   @desc
              Routines to deal with warning levels.
   @enddesc
   @version   $Id: WarnLevel.c 5041 2013-08-20 19:22:56Z eschnett $
 @@*/

#include "cctk_Config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# elif HAVE_TIME_H
#  include <time.h>
# endif
#endif

#include "CommandLine.h"
#include "StoreKeyedData.h"

#include "cctk_Comm.h"
#include "cctk_Parameter.h"
#include "cctk_FortranString.h"
#include "cctk_WarnLevel.h"
#include "cctki_WarnLevel.h"

#include "util_String.h"
#include "util_Network.h"

static const char *rcsid = "$Header$";
CCTK_FILEVERSION(main_WarnLevel_c)


/********************************************************************
 *********************         Macros          **********************
 ********************************************************************/
/* Escape sequences to highlight warning messages.
 */

#ifndef WIN32
#define BOLD_ON  "\033[1m"
#define BOLD_OFF "\033[0m"
#else
#define BOLD_ON  ""
#define BOLD_OFF ""
#endif

typedef enum { ON = 0, OFF = 1 } BOLDING;

static void bold_stdout (BOLDING on)
{
  const char *val = (on == ON) ? BOLD_ON : BOLD_OFF;

  if (!isatty (STDOUT_FILENO))
    val = "";

  fprintf (stdout, "%s", val);
}

static void bold_stderr (BOLDING on)
{
  const char *val = (on == ON) ? BOLD_ON : BOLD_OFF;

  if (!isatty (STDERR_FILENO))
    val = "";

  fprintf (stderr, "%s", val);
}

/* maximum buffer length to hold the hostname */
#define MAXNAMELEN 255

/********************************************************************
 *********************     External Routines   **********************
 ********************************************************************/
/* prototypes for external C routines are declared in headers
   cctk_WarnLevel.h and cctki_WarnLevel.h
   here only follow the fortran wrapper prototypes */
#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctk_info
                           (TWO_FORTSTRING_ARG);
void CCTK_FCALL cctk_warn
                           (const int *level,
                            const int *line,
                            THREE_FORTSTRING_ARG);
void CCTK_FCALL cctk_error
                           (const int *line,
                            THREE_FORTSTRING_ARG);
void CCTK_FCALL cctk_paramwarn
                           (TWO_FORTSTRING_ARG);
int  CCTK_FCALL cctk_messageformat
                          (ONE_FORTSTRING_ARG);
void CCTK_FCALL cctki_notyetimplemented
                           (ONE_FORTSTRING_ARG);
#else
void CCTK_FCALL cctk_info_
                           (TWO_FORTSTRING_ARG);
void CCTK_FCALL cctk_warn_
                           (const int *level,
                            const int *line,
                            THREE_FORTSTRING_ARG);
void CCTK_FCALL cctk_error_
                           (const int *line,
                            THREE_FORTSTRING_ARG);
void CCTK_FCALL cctk_paramwarn_
                           (TWO_FORTSTRING_ARG);
int  CCTK_FCALL cctk_messageformat_
                          (ONE_FORTSTRING_ARG);
void CCTK_FCALL cctki_notyetimplemented_
                           (ONE_FORTSTRING_ARG);
#endif

/********************************************************************
 ********************    Internal Typedefs   ************************
 ********************************************************************/
/* structure holding a callback function pointer together with other 
 * information to warn/info*/

typedef struct warncallback
{
  struct warncallback *next;
  cctk_warnfunc function;
  void *data;
  int minlevel;
  int maxlevel;
} t_warncallback;

typedef struct infocallback
{
  struct infocallback *next;
  cctk_infofunc function;
  void *data;
} t_infocallback;


/********************************************************************
 ********************    Internal Functions  ************************
 ********************************************************************/
 
static void CCTKi_WarnCallbacksCall(int level,
                                    int line,
                                    const char *file,
                                    const char *thorn,
                                    const char *message);

static void CCTKi_InfoCallbacksCall(const char *thorn, const char *message);

static void CCTKi_ExitIfParamCheckOnly(void);

/********************************************************************
 *********************    Static Data   *****************************
 ********************************************************************/

/* Store the parameter checking level */
static int parameter_level = CCTK_PARAMETER_STRICT;

/* Store the number of parameter errors */
static int param_errors = 0;

/* Store the logging level - warnings of this severity or worse will
 * be reported on stdout
 */
static int logging_level = 0;

/* Store the warning level - warnings of this severity or worse will
 * be reported on stderr
 */
static int warning_level = 1;

/* Store the error level - warnings of this severity or worse will stop
 * the code.
 */
static int error_level = 0;

/* Store a list of format strings */
static int n_formats = 0;
static pKeyedData *formatlist = NULL;

/* Store registered warn and info methods */
static t_warncallback *warncallbacks = NULL;
static t_infocallback *infocallbacks = NULL;


/*@@
   @routine    CCTK_Info
   @date       Tue Mar 30 1999
   @author     Gabrielle Allen
   @desc
               Print an information message to stdout
   @enddesc

   @var        thorn
   @vdesc      Name of originating thorn
   @vtype      const char *
   @vio        in
   @endvar
   @var        message
   @vdesc      the warning message to output
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine CCTK_VInfo
   @endreturndesc
@@*/
int CCTK_Info (const char *thorn, const char *message)
{
  return (CCTK_VInfo (thorn, "%s", message));
}

#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctk_info
#else
void CCTK_FCALL cctk_info_
#endif
                           (TWO_FORTSTRING_ARG)
{
TWO_FORTSTRING_CREATE (thorn, message)
  CCTK_Info (thorn, message);
  free (thorn);
  free (message);
}


/*@@
   @routine    CCTK_VInfo
   @date       Mon Apr 10
   @author     Thomas Radke
   @desc
               Info output routine with variable argument list
   @enddesc

   @history
   @date       Mon Aug  4 17:56:06 CEST 2003
   @author     Jonathan Thornburg
   @hdesc      add  cactus::info_format  parameter
   @endhistory

   @var        thorn
   @vdesc      Name of originating thorn
   @vtype      const char *
   @vio        in
   @endvar
   @var        format
   @vdesc      format string for message
   @vtype      const char *
   @vio        in
   @endvar
   @var        ...
   @vdesc      variable argument list for format string
   @vtype      multiple arguments
   @vio        in
   @endvar

   @returntype int
   @returndesc
               0  - success
   @endreturndesc
@@*/
int CCTK_VInfo (const char *thorn, const char *format, ...)
{
  va_list ap;
  static int info_format_decoded = 0;   /* are the following two flags valid? */
  /* Boolean flags decoded from  cactus::info_format */
  static int info_format_numeric = 0;         /* print a numeric timestamp? */
  static int info_format_human_readable = 0;  /* print a human-readable timestamp? */

  /* necessary for wrapping up the final message */
  int msg_size;
  char *message = NULL;

  /* Start generating message only if the infocallback list is not NULL */
  if(infocallbacks)
  { 
    /* one way to get the final string size */
    va_start(ap,format);
    msg_size = Util_vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    /* Empty string is ok */
    if(msg_size >= 0)
    {
      message = (char *)malloc(msg_size+1);
    }
    
    /* Try to print in the allocated space. */
    if(message)
    {
      va_start(ap,format);
      Util_vsnprintf(message,msg_size+1,format,ap);
      va_end(ap);
    } 

    /* call the callback function */
    CCTKi_InfoCallbacksCall(thorn,message);

    /* free the memory allocated for temp messsage */
    free (message);
  }

  /*
   * if we haven't already decoded  cactus::info_format  into the
   * Boolean flags, do so
   */
  if (! info_format_decoded)
  {
    /* get cactus::info_format  and decode it into Boolean flags */
    const char* const info_format =
      * ( (const char*const *)
          CCTK_ParameterGet("info_format", "Cactus", NULL) );

    if      (CCTK_Equals(info_format, "basic"))
    {
      /* "basic" :: "INFO (ThornName): message" */
      info_format_numeric = 0;
      info_format_human_readable = 0;
    }
    else if (CCTK_Equals(info_format, "numeric time stamp"))
    {
      /* "numeric time stamp" :: "numeric_timestamp\tINFO (ThornName): message" */
      info_format_numeric = 1;
      info_format_human_readable = 0;
    }
    else if (CCTK_Equals(info_format, "human-readable time stamp"))
    {
      /* "human-readable time stamp" :: "human readable timestamp: INFO (ThornName): message" */
      info_format_numeric = 0;
      info_format_human_readable = 1;
    }
    else if (CCTK_Equals(info_format, "full time stamp"))
    {
      /* "full time stamp" :: "numeric_timestamp\thuman readable timestamp: INFO (ThornName): message" */
      info_format_numeric = 1;
      info_format_human_readable = 1;
    }
    info_format_decoded = 1;
  }

  /*
   * print any time stamps desired
   */
  if (info_format_numeric || info_format_human_readable)
  {
    double numeric_time;

    /* get the timing info */
#ifdef HAVE_TIME_GETTIMEOFDAY
    /* use  gettimeofday()  to get (up to) microsecond-level timing info */
    #define NUMERIC_TIME_FORMAT "%.6f"
    struct timeval timeval;
    gettimeofday(&timeval, NULL);
    numeric_time = timeval.tv_sec + 1.0e-6*timeval.tv_usec;
#else
    /* fall back to  time() ; this is ISO C, so everyone should have it */
    #define NUMERIC_TIME_FORMAT "%.0f"
#ifdef SPEC
// mec: This line triggers a gcc warning:
//  warning: cast from function call of type 'time_t' to non-matching type 'double' 
//  Since the benchmark does not use time, it's safe to simply set numeric_time to 0
//  and not fix the code  
    numeric_time = 0.0;
#else
    numeric_time = (double) time(NULL);
#endif
#endif

    if (info_format_numeric)
    {
      fprintf(stdout, NUMERIC_TIME_FORMAT "\t", numeric_time);
    }

    if (info_format_human_readable)
    {
      /*                                123456789 123456789 1234    */
      /* time_buffer --> a string like "Thu Nov 24 18:22:48 1986\n" */
      const time_t time_t_numeric_time = (time_t) numeric_time;
      const char* const time_buffer = ctime(& time_t_numeric_time);
      fprintf(stdout, "%.24s: ", time_buffer);
    }
  }

  /*
   * print the INFO message itself
   */
  fprintf (stdout, "INFO (%s): ", thorn);
  va_start (ap, format);
  vfprintf (stdout, format, ap);
  va_end (ap);
  fprintf (stdout, "\n");

  return 0;
}


/*@@
   @routine    CCTK_Warn
   @date       Wed Feb 17 00:45:07 1999
   @author     Tom Goodale
   @desc
               Warn the user of something if the warning level is suitable.
   @enddesc
   @calls      CCTK_VWarn

   @var        level
   @vdesc      The warning level
   @vtype      int
   @vio        in
   @endvar
   @var        line
   @vdesc      Line number of warning in originating file
   @vtype      int
   @vio        in
   @endvar
   @var        file
   @vdesc      Name of originating file
   @vtype      const char *
   @vio        in
   @endvar
   @var        thorn
   @vdesc      Name of originating thorn
   @vtype      const char *
   @vio        in
   @endvar
   @var        message
   @vdesc      warning message to output to stderr
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               return code of @seeroutine CCTK_VWarn
   @endreturndesc
@@*/
int CCTK_Warn (int level,
               int line,
               const char *file,
               const char *thorn,
               const char *message)
{
  return (CCTK_VWarn (level, line, file, thorn, "%s", message));
}

#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctk_warn
#else
void CCTK_FCALL cctk_warn_
#endif
                           (const int *level,
                            const int *line,
                            THREE_FORTSTRING_ARG)
{
THREE_FORTSTRING_CREATE (file, thorn, message)
  CCTK_Warn (*level, *line, file, thorn, message);
  free (thorn);
  free (message);
  free (file);
}


void CCTK_Error (int line,
                 const char *file,
                 const char *thorn,
                 const char *message)
{
  CCTK_Warn (0, line, file, thorn, message);
  CCTK_BUILTIN_UNREACHABLE();
}

#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctk_error
#else
void CCTK_FCALL cctk_error_
#endif
                           (const int *line,
                            THREE_FORTSTRING_ARG)
{
THREE_FORTSTRING_CREATE (file, thorn, message)
  CCTK_Error (*line, file, thorn, message);
  free (thorn);
  free (message);
  free (file);
}


/*@@
   @routine    CCTK_VWarn
   @date       Sun Nov 14 00:23:29 1999
   @author     Tom Goodale
   @desc
               Warning routine with variable argument list

               If the given warning level is less or equal to the current one,
               it will print the given warning message to stderr.
               On processors other than 0 it will also print it to stdout.
   @enddesc
   @calls      CCTK_ParameterGet

   @var        level
   @vdesc      The warning level
   @vtype      int
   @vio        in
   @endvar
   @var        line
   @vdesc      Line number of warning in originating file
   @vtype      int
   @vio        in
   @endvar
   @var        file
   @vdesc      Name of originating file
   @vtype      const char *
   @vio        in
   @endvar
   @var        thorn
   @vdesc      Name of originating thorn
   @vtype      const char *
   @vio        in
   @endvar
   @var        format
   @vdesc      Format string for following arguments
   @vtype      const char *
   @vio        in
   @endvar
   @var        ...
   @vdesc      variable argument list for format string
   @vtype      multiple arguments
   @vio        in
   @endvar

   @returntype int
   @returndesc
               0  - success
   @endreturndesc
@@*/
int CCTK_VWarn (int level,
                int line,
                const char *file,
                const char *thorn,
                const char *format,
                ...)
{
  const CCTK_INT *cctk_full_warnings_ptr, *highlight_warning_messages_ptr;
  CCTK_INT cctk_full_warnings, highlight_warning_messages;
  int param_type;
  int myproc;
  va_list ap;

  /* Necessary for wrapping up the final message */

  int msg_size;
  char *message = NULL;
  char hostname[MAXNAMELEN+1];

  /* Start generating message only if the warbcallback list is not NULL */
  if(warncallbacks)
  {
    va_start(ap,format);
    msg_size = Util_vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    /* Empty string is ok */
    if(msg_size >= 0)
    {
      message = (char *)malloc(msg_size+1);
    }
    
    /* Try to print in the allocated space. */
    if(message)
    {
      va_start(ap,format);
      Util_vsnprintf(message,msg_size+1,format,ap);
      va_end(ap);
    } 

    /* call the callback function */
    CCTKi_WarnCallbacksCall(level,line,file,thorn,message);

    /* free the memory allocated for temp messsage */
    free (message);
  }  

  if (level <= warning_level || level <= logging_level)
  {

    myproc = CCTK_MyProc(NULL);
    Util_GetHostName (hostname, MAXNAMELEN);

    cctk_full_warnings_ptr =
      CCTK_ParameterGet ("cctk_full_warnings", "Cactus", &param_type);
    /* Default to yes */
    cctk_full_warnings =
      cctk_full_warnings_ptr && *cctk_full_warnings_ptr;

    highlight_warning_messages_ptr =
      CCTK_ParameterGet ("highlight_warning_messages", "Cactus", &param_type);
    /* Default to no */
    highlight_warning_messages =
      ! highlight_warning_messages_ptr || *highlight_warning_messages_ptr;

    /* print to stderr if necessary */
    if (level <= warning_level)
    {

      if (highlight_warning_messages)
      {
        bold_stderr (ON);
      }

      if (level <= error_level || cctk_full_warnings)
      {
        fprintf (stderr, "WARNING level %d in thorn %s processor %d host %s\n"
                         "  (line %d of %s): \n"
                         "  ->",
                         level, thorn, myproc, hostname, line, file);
      }
      else
      {
        fprintf (stderr, "WARNING[L%d,P%d] (%s):",
                         level, myproc, thorn);
      }

      if (highlight_warning_messages)
      {
        bold_stderr (OFF);
      }

      fprintf (stderr, " ");
      va_start (ap, format);
      vfprintf (stderr, format, ap);
      va_end (ap);
      fprintf (stderr, "\n");

    }

    /* print to stdout if necessary */
    if (level <= logging_level || (myproc && level <= warning_level))
    {

      if (highlight_warning_messages)
      {
        bold_stdout (ON);
      }

      if (level <= error_level || cctk_full_warnings)
      {
        fprintf (stdout, "WARNING level %d in thorn %s processor %d host %s\n"
                         "  (line %d of %s): \n"
                         "  ->",
                         level, thorn, myproc, hostname, line, file);
      }
      else
      {
        fprintf (stdout, "WARNING[L%d,P%d] (%s):",
                         level, myproc, thorn);
      }

      if (highlight_warning_messages)
      {
        bold_stdout (OFF);
      }

      fprintf (stdout, " ");
      va_start (ap, format);
      vfprintf (stdout, format, ap);
      va_end (ap);
      fprintf (stdout, "\n");

    }

  }

  if (level <= error_level)
  {
    CCTK_Abort (NULL, 0);
  }

  return 0;
}


void CCTK_VError (int line,
                  const char *file,
                  const char *thorn,
                  const char *format,
                  ...)
{
  const CCTK_INT *highlight_warning_messages_ptr;
  CCTK_INT highlight_warning_messages;
  int param_type;
  int myproc;
  va_list ap;

  /* Necessary for wrapping up the final message */

  int msg_size;
  char *message = NULL;
  char hostname[MAXNAMELEN+1];

  /* Start generating message only if the warbcallback list is not NULL */
  if(warncallbacks)
  {
    va_start(ap,format);
    msg_size = Util_vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    /* Empty string is ok */
    if(msg_size >= 0)
    {
      message = (char *)malloc(msg_size+1);
    }
    
    /* Try to print in the allocated space. */
    if(message)
    {
      va_start(ap,format);
      Util_vsnprintf(message,msg_size+1,format,ap);
      va_end(ap);
    } 

    /* call the callback function */
    CCTKi_WarnCallbacksCall(0,line,file,thorn,message);

    /* free the memory allocated for temp messsage */
    free (message);
  }  

  myproc = CCTK_MyProc(NULL);
  Util_GetHostName (hostname, MAXNAMELEN);

  highlight_warning_messages_ptr =
    CCTK_ParameterGet ("highlight_warning_messages", "Cactus", &param_type);
  /* Default to no */
  highlight_warning_messages =
    ! highlight_warning_messages_ptr || *highlight_warning_messages_ptr;

  /* print to stderr */
  if (highlight_warning_messages)
  {
    bold_stderr (ON);
  }

  fprintf (stderr, "ERROR in thorn %s processor %d host %s\n"
           "  (line %d of %s): \n"
           "  ->",
           thorn, myproc, hostname, line, file);

  if (highlight_warning_messages)
  {
    bold_stderr (OFF);
  }

  fprintf (stderr, " ");
  va_start (ap, format);
  vfprintf (stderr, format, ap);
  va_end (ap);
  fprintf (stderr, "\n");

  /* print to stdout */
  if (highlight_warning_messages)
  {
    bold_stdout (ON);
  }

  fprintf (stdout, "ERROR in thorn %s processor %d host %s\n"
           "  (line %d of %s): \n"
           "  ->",
           thorn, myproc, hostname, line, file);

  if (highlight_warning_messages)
  {
    bold_stdout (OFF);
  }

  fprintf (stdout, " ");
  va_start (ap, format);
  vfprintf (stdout, format, ap);
  va_end (ap);
  fprintf (stdout, "\n");

  CCTK_Abort (NULL, 0);
}


 /*@@
   @routine    CCTK_ParameterLevel
   @date       Wed Feb 21 2001
   @author     Gabrielle Allen
   @desc
               Returns the parameter checking level
   @enddesc

   @returntype int
   @returndesc
               parameter checking level now being used
   @endreturndesc
@@*/
int CCTK_ParameterLevel (void)
{
  return (parameter_level);
}


/*@@
   @routine    CCTK_ParamWarn
   @date       Wed Feb 17 00:45:07 1999
   @author     Tom Goodale
   @desc
               Warn the user if a parameter error is found
   @enddesc
   @calls      CCTK_ParameterGet

   @var        thorn
   @vdesc      Name of originating thorn
   @vtype      const char *
   @vio        in
   @endvar
   @var        message
   @vdesc      Warning message to output
   @vtype      const char *
   @vio        in
   @endvar

   @returntype int
   @returndesc
               0  - success
   @endreturndesc
@@*/
int CCTK_ParamWarn (const char *thorn, const char *message)
{
  return (CCTK_VParamWarn (thorn, "%s", message));
}

#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctk_paramwarn
#else
void CCTK_FCALL cctk_paramwarn_
#endif
                           (TWO_FORTSTRING_ARG)
{
TWO_FORTSTRING_CREATE (thorn, message)
  CCTK_ParamWarn (thorn, message);
  free (thorn);
  free (message);
}


/*@@
   @routine    CCTK_VParamWarn
   @date       Mon May 20 2002
   @author     Gabrielle Allen
   @desc
               Warn the user if a parameter error is found using a variable
               argument list (extends CCTK_ParamWarn)
   @enddesc
   @calls      CCTK_ParameterGet

   @var        thorn
   @vdesc      Name of originating thorn
   @vtype      const char *
   @vio        in
   @endvar
   @var        format
   @vdesc      Format for variable argument list
   @vtype      const char *
   @vio        in
   @endvar
   @var        ...
   @vdesc      Variable argument list
   @vtype
   @vio
   @endvar

   @returntype int
   @returndesc
               0  - success
   @endreturndesc
@@*/
int CCTK_VParamWarn (const char *thorn,
                     const char *format,
                     ...)
{
  va_list ap;
  const CCTK_INT *cctk_strong_param_check_ptr, *highlight_warning_messages_ptr;
  CCTK_INT cctk_strong_param_check, highlight_warning_messages;
  int param_type;

  cctk_strong_param_check_ptr =
    CCTK_ParameterGet ("cctk_strong_param_check", "Cactus", &param_type);
  /* Default to yes */
  cctk_strong_param_check =
    cctk_strong_param_check_ptr && *cctk_strong_param_check_ptr;

  highlight_warning_messages_ptr =
    CCTK_ParameterGet ("highlight_warning_messages", "Cactus", &param_type);
  /* Default to no */
  highlight_warning_messages =
    ! highlight_warning_messages_ptr || *highlight_warning_messages_ptr;

  fflush (stdout);

  /* print to stderr */
  if (highlight_warning_messages)
  {
    bold_stderr (ON);
  }

  fprintf (stderr, "PARAMETER %s (%s): ",
           cctk_strong_param_check ? "ERROR" : "WARNING", thorn);

  va_start (ap, format);
  vfprintf (stderr, format, ap);
  va_end (ap);
  fprintf (stderr, "\n");

  if (highlight_warning_messages)
  {
    bold_stderr (OFF);
  }

  fflush (stderr);

  /* print to stdout */
  if (highlight_warning_messages)
  {
    bold_stdout (ON);
  }

  fprintf (stdout, "PARAMETER %s (%s): ",
           cctk_strong_param_check ? "ERROR" : "WARNING", thorn);

  va_start (ap, format);
  vfprintf (stdout, format, ap);
  va_end (ap);
  fprintf (stdout, "\n");

  if (highlight_warning_messages)
  {
    bold_stdout (OFF);
  }

  fflush (stdout);

  param_errors++;

  return (0);
}

/*@@
   @routine    CCTK_WarnCallbackRegister
   @date       05/17/2005
   @author     Jian Tao
   @desc
            Register warn callback function
   @enddesc
   @endvar
   @var     minlevel
   @vdesc   minimum warning level
   @vtype   int 
   @vio     in
   @endvar
   @var     maxlevel
   @vdesc   maximum warning level
   @vtype   int
   @vio     in
   @endvar
   @var     data
   @vdesc   
   @vtype   int*
   @vio     in
   @endvar
   @var     function
   @vdesc   Callback function (see cctk_WarnLevel.h)
   @vtype   cctk_warnfunc
   @vio     in
   @endvar
@@*/

int CCTK_WarnCallbackRegister(int minlevel,
                              int maxlevel,
                              void *data,
                              cctk_warnfunc callback)

{
  int retval;
  t_warncallback *newcallback;
   
  newcallback = (t_warncallback *)malloc(sizeof(t_warncallback));

/* Create a one way chain for all registered callbacks */
  if(newcallback) 
  {
    newcallback->next = warncallbacks;
    warncallbacks = newcallback;
    newcallback->function = callback;
    newcallback->data = data;
    newcallback->minlevel = minlevel;
    newcallback->maxlevel = maxlevel;
    retval = 0;                          
  }
  else
  {
    retval = -1;
  }
  return retval;
}


/*@@
   @routine    CCTK_InfoCallbackRegister
   @date       05/17/2005
   @author     Jian Tao
   @desc
            Register info callback function
   @enddesc
   @endvar
   @var     minlevel
   @vdesc   minimum warning level
   @vtype   int
   @vio     in
   @endvar
   @var     maxlevel
   @vdesc   maximum warning level
   @vtype   int
   @vio     in
   @endvar
   @var     data
   @vdesc
   @vtype   int*
   @vio     in
   @endvar
   @var     function
   @vdesc   Callback function (see cctk_WarnLevel.h)
   @vtype   cctk_infofunc
   @vio     in
   @endvar
@@*/

int CCTK_InfoCallbackRegister(void *data, cctk_infofunc callback)

{
  int retval;
  t_infocallback *newcallback;

  newcallback = (t_infocallback *)malloc(sizeof(t_infocallback));

/* Create a one way chain for all registered callbacks */
  if(newcallback) 
  {
    newcallback->next = infocallbacks;
    infocallbacks = newcallback;
    newcallback->function = callback;
    newcallback->data = data;
    retval = 0;
  }
  
  else
  {
    retval = -1;
  }
  return retval;
}


/*@@
   @routine    CCTK_MessageFormat
   @date       Mon Jul 26 19:51:26 1999
   @author     Tom Goodale
   @desc
               Stores a format for messages from Fortran.
   @enddesc
   @calls      StoreKeyedData

   @var        format
   @vdesc      Format string
   @vtype      Fortran string macro
   @vio        in
   @endvar

   @returntype int
   @returndesc
               number of previous formats
   @endreturndesc
@@*/
#if defined(SPEC_NO_UNDERSCORE)
int CCTK_FCALL cctk_messageformat
#else
int CCTK_FCALL cctk_messageformat_
#endif
                          (ONE_FORTSTRING_ARG)
{
ONE_FORTSTRING_CREATE (format)

  StoreKeyedData (&formatlist, n_formats , format);

  return (n_formats++);
}


/********************************************************************
 *********************     CCTKi Routines   *************************
 ********************************************************************/

 /*@@
   @routine    CCTKi_SetParameterLevel
   @date       Wed Feb 21 2001
   @author     Gabrielle Allen
   @desc
               Sets the parameter checking level
   @enddesc
   @calls      CCTK_VWarn

   @var        level
   @vdesc      level to set
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
               parameter checking level now being used
   @endreturndesc
@@*/
int CCTKi_SetParameterLevel (int level)
{
  if (level == CCTK_PARAMETER_STRICT ||
      level == CCTK_PARAMETER_NORMAL ||
      level == CCTK_PARAMETER_RELAXED)
  {
    parameter_level = level;
  }
  else
  {
    CCTK_VWarn (1, __LINE__, __FILE__, "Cactus", "CCTKi_SetParameterLevel: "
                "Parameter checking level %d not recognised, level unchanged",
                level);
  }

  return (parameter_level);
}


 /*@@
   @routine    CCTKi_SetLogLevel
   @date       Thu Dec 30 2004
   @author     Erik Schnetter
   @desc
               Sets the logging level
   @enddesc
   @calls      CCTK_VWarn
               CCTK_VInfo

   @var        level
   @vdesc      logging level to set
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 - increased logging level <BR>
                0 - logging level unchanged <BR>
               -1 - decreased logging level
   @endreturndesc
@@*/
int CCTKi_SetLogLevel (int level)
{
  int retval;


  if (logging_level != level)
  {
    retval = level > logging_level ? +1 : -1;
    CCTK_VInfo ("Cactus", "%s logging level from %d to %d",
                retval > 0 ? "Increasing" : "Decreasing", logging_level, level);
    logging_level = level;
  }
  else
  {
    CCTK_VInfo ("Cactus",
                "Logging level is already %d", level);
    retval = 0;
  }

  return (retval);
}


 /*@@
   @routine    CCTKi_SetWarnLevel
   @date       Wed Feb 17 00:42:16 1999
   @author     Tom Goodale
   @desc
               Sets the warning level
   @enddesc
   @calls      CCTK_VWarn
               CCTK_VInfo

   @var        level
   @vdesc      warning level to set
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 - increased warning level <BR>
                0 - warning level unchanged <BR>
               -1 - decreased warning level
   @endreturndesc
@@*/
int CCTKi_SetWarnLevel (int level)
{
  int retval;


  if (warning_level != level)
  {
    retval = level > warning_level ? +1 : -1;
    CCTK_VInfo ("Cactus", "%s warning level from %d to %d",
                retval > 0 ? "Increasing" : "Decreasing", warning_level, level);
    warning_level = level;
  }
  else
  {
    CCTK_VInfo ("Cactus",
                "Warning level is already %d", level);
    retval = 0;
  }

  if (warning_level < error_level)
  {
    error_level = warning_level;
    CCTK_VWarn (2, __LINE__, __FILE__, "Cactus",
                "Decreasing error level to new warning level %d",warning_level);
  }

  return (retval);
}


/*@@
   @routine    CCTKi_SetErrorLevel
   @date       Wed Feb 17 00:48:02 1999
   @author     Tom Goodale
   @desc
               Sets the error level
   @enddesc
   @calls      CCTK_VWarn
               CCTK_VInfo

   @var        level
   @vdesc      error level to set
   @vtype      int
   @vio        in
   @endvar

   @returntype int
   @returndesc
                1 - increased error level <BR>
                0 - error level unchanged <BR>
               -1 - decreased error level
   @endreturndesc
@@*/
int CCTKi_SetErrorLevel (int level)
{
  int retval;

  if (level < 0)
  {
    CCTK_VWarn (3, __LINE__, __FILE__, "Cactus",
                "Error level cannot be negative (%d requested)", level);
    retval = 0;
  }
  else if (level <= warning_level)
  {
    if (error_level != level)
    {
      retval = level > error_level ? +1 : -1;
      CCTK_VInfo ("Cactus", "%s error level from %d to %d",
                  retval > 0 ? "Increasing" : "Decreasing", error_level, level);
      error_level = level;
    }
    else
    {
      CCTK_VWarn (3, __LINE__, __FILE__, "Cactus",
                  "Error level is already %d", level);
      retval = 0;
    }
  }
  else
  {
    retval = level > error_level ? +1 : -1;
    error_level = level;
    CCTK_VInfo ("Cactus",
                "Increasing warning level from %d to match error level %d",
                warning_level,error_level);
    warning_level = level;
    retval = 0;
  }

  return (retval);
}


 /*@@
   @routine    ExitIfParamCheckOnly
   @date       Mon May 16 2012
   @author     Steven R. Brandt
   @desc
               Responsible for exiting after paramcheck
   @enddesc

@@*/
static void CCTKi_ExitIfParamCheckOnly(void)
{
    if(CCTKi_ExitAfterParamCheck()) {
        printf("Exit after param check requested using the --exit-after-param-check option.\n");
        CCTK_Exit(NULL, 0);
    }
}


/*@@
   @routine    CCTKi_FinaliseParamWarn
   @date       June 1999
   @author     Gabrielle Allen
   @desc
               Die if errors were encountered during param check
   @enddesc
   @calls      CCTK_ParameterGet
@@*/
void CCTKi_FinaliseParamWarn (void)
{
  int param_type;
  const CCTK_INT *cctk_strong_param_check_ptr;
  CCTK_INT cctk_strong_param_check;


  /*
   * Let all processors catch up before continuing.
   *
   * In case of parameter errors this should avoid racing problems
   * in parallel simulations using a certain MPI implementation where
   * the run would die prematurely because some processors called CCTK_Abort()
   * earlier than others, and in the logfile one couldn't easily find the real
   * reason for the abort anymore.
   *
   * (see http://www.cactuscode.org/old/pipermail/developers/2007-December/005480.html)
   */
  CCTK_Barrier(NULL);

  if (param_errors)
  {
    cctk_strong_param_check_ptr =
      CCTK_ParameterGet ("cctk_strong_param_check", "Cactus", &param_type);
    /* Default to yes */
    cctk_strong_param_check =
      cctk_strong_param_check_ptr && *cctk_strong_param_check_ptr;

    if (cctk_strong_param_check)
    {
      fprintf (stderr, "\nFailed parameter check (%d errors)\n\n", param_errors);
      CCTK_Abort (NULL, 99);
    }
    else
    {
      if (param_errors==1)
      {
        fprintf (stderr, "\nThere was 1 parameter warning\n\n");
      }
      else
      {
        fprintf (stderr, "\nThere were %d parameter warnings\n\n", param_errors);
      }
    }
  }
  CCTKi_ExitIfParamCheckOnly();
}


/*@@
   @routine    CCTKi_NotYetImplemented
   @date       July 1999
   @author     Gabrielle Allen
   @desc
               Report on features not yet added to code
   @enddesc

   @var        message
   @vdesc      message to be printed
   @vtype      const char *
   @vio        in
   @endvar
@@*/
void CCTKi_NotYetImplemented (const char *message)
{
#if 0
  CCTK_VWarn (0, __LINE__, __FILE__, "Cactus",
              "%s: this planned feature is not yet implemented in the code.\n"
              "If you need this feature please contact the "
              "Cactus maintainers.", message);
#endif
  CCTK_VWarn (0, __LINE__, __FILE__, "Cactus", "%s: feature not implemented",
              message);
}

#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctki_notyetimplemented
#else
void CCTK_FCALL cctki_notyetimplemented_
#endif
                           (ONE_FORTSTRING_ARG)
{
ONE_FORTSTRING_CREATE (message)
  CCTKi_NotYetImplemented (message);
  free (message);
}


#if 0
 /*@@
   @routine    CCTK_VINFO
   @date       Wed Oct  4 21:03:08 2000
   @author     Tom Goodale
   @desc
   Fortran version of CCTK_VInfo
   @enddesc
   @calls
   @calledby
   @history

   @endhistory
   @var     format_number
   @vdesc   format string handle
   @vtype   int
   @vio     in
   @vcomment

   @endvar
   @var     ...
   @vdesc   arguments for format string
   @vtype   multiple arguments
   @vio     in
   @vcomment

   @endvar

   @returntype int
   @returndesc
   FIXME - UNFINISHED
   @endreturndesc
@@*/
#if defined(SPEC_NO_UNDERSCORE)
void CCTK_FCALL cctk_vinfo
#else
void CCTK_FCALL cctk_vinfo_
#endif
                           (int format_number,
                            ...)
{
  char *format_string;
  char *message;
  int message_length;
  int current_place;

  if (format_number < n_formats)
  {
    format_string = (char *)GetKeyedData (formatlist, format_number);

    /* Pick an arbitrary starting length for the message */
    message_length=5*strlen (format_string);

    message = (char *)malloc (message_length);

    /* Loop through the format string */
    for (current_place=0; format_string; format_string++)
    {
      if (*format_string != '%')
      {
        message[current_place] = *format_string;
        current_place++;
        if (current_place >= message_length)
        {
          message = (char *)realloc (message, message_length*2);
          message_length *=2;
        }
      }
      else
      {
        /* FIXME */
      }
    }

  }

}

#endif

/*@@
   @routine    CCTKi_WarnCallbacksCall
   @date       05/17/2005
   @author     Jian Tao
   @desc
            Call callback functions
   @enddesc
   @var     level
   @vdesc   Warning level
   @vtype   int
   @vio     in
   @endvar
   @var     line
   @vdesc   Line number of warning in originating file
   @vtype   int
   @vio     in
   @endvar
   @var     file
   @vdesc   Name of originating file
   @vtype   const char *
   @vio     in
   @endvar
   @var     thorn
   @vdesc   Name of originating thorn
   @vtype   const char *
   @vio     in
   @endvar
   @var     message
   @vdesc   warning message to output to stderr
   @vtype   const char *
   @vio     in
   @endvar   
@@*/

static void CCTKi_WarnCallbacksCall(int level,
                                    int line,
                                    const char *file,
                                    const char *thorn,
                                    const char *message)
{
  t_warncallback *current;

/*
 * Go through all the registered call back functions 
 * static variable warncallbacks has already been checked to be not empty 
 */

  for(current=warncallbacks; current; current=current->next)
  {

/* Check valid level */
    if(level >= current->minlevel && level <= current->maxlevel)
    {
      current->function(level,line,file,thorn,message,current->data);
    }

  }
  return;
}

/*@@
   @routine    CCTKi_InfoCallbacksCall
   @date       05/17/2005
   @author     Jian Tao
   @desc
            Call callback functions
   @enddesc
   @var     thorn
   @vdesc   Name of originating thorn
   @vtype   const char *
   @vio     in
   @endvar
   @var     message
   @vdesc   warning message to output to stderr
   @vtype   const char *
   @vio     in
   @endvar   
@@*/

static void CCTKi_InfoCallbacksCall(const char *thorn, const char *message)
{
  t_infocallback *current;

/*
 * Go through all the registered call back functions 
 * static variable infocallbacks has already been checked to be not empty 
 */

  for(current=infocallbacks; current; current=current->next)
  { 
    current->function(thorn,message,current->data);
  }
  return;
}
