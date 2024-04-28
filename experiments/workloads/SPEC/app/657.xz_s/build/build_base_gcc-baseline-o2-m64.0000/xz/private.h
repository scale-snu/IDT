///////////////////////////////////////////////////////////////////////////////
//
/// \file       private.h
/// \brief      Common includes, definions, and prototypes
//
//  Author:     Lasse Collin
//
//  This file has been put into the public domain.
//  You can do whatever you want with this file.
//
///////////////////////////////////////////////////////////////////////////////

#if defined(SPEC_MEM_IO)
# include "spec_mem_io.h"
#endif
#include "sysdefs.h"
#include "mythread.h"
#include "lzma.h"

#if !defined(SPEC)
#include <sys/types.h>
#include <sys/stat.h>
#endif // SPEC
#include <errno.h>
#if !defined(SPEC)
#include <signal.h>
#include <locale.h>
#include <unistd.h>
#endif // SPEC
#include <stdio.h>
#if (defined(SPEC_WINDOWS) || defined(SPEC_NEED__SNPRINTF)) && !defined(SPEC_HAVE_SNPRINTF)
# define snprintf _snprintf
#endif

#include "tuklib_gettext.h"
#include "tuklib_progname.h"
#include "tuklib_exit.h"
#include "tuklib_mbstr.h"

#if defined(_WIN32) && !defined(__CYGWIN__)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#ifndef STDIN_FILENO
#	define STDIN_FILENO (fileno(stdin))
#endif

#ifndef STDOUT_FILENO
#	define STDOUT_FILENO (fileno(stdout))
#endif

#ifndef STDERR_FILENO
#	define STDERR_FILENO (fileno(stderr))
#endif

#include "main.h"
#include "coder.h"
#include "message.h"
#include "args.h"
#include "hardware.h"
#include "file_io.h"
#include "options.h"
#include "signals.h"
#if !defined(SPEC)
#include "suffix.h"
#endif 
#include "util.h"
#include "list.h"
