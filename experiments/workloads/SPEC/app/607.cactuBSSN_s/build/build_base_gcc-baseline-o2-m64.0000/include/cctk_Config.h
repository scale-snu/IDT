/* cctk_Config.h.  Generated automatically by configure.  */
#ifndef _CCTK_CONFIG_H_
#define _CCTK_CONFIG_H_

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

/* Define if standard C headers are available. */
#define STDC_HEADERS 1

/* Modifier for Fortran function definitions. */
#define CCTK_FCALL 

/* What debugging options to use */
/* #undef CCTK_DEBUG */
/* #undef CCTK_TRACEMEMORY */

/* Various library functions */
#ifndef SPEC
#define HAVE_GETHOSTBYNAME 1
#endif
#define HAVE_GETOPT_LONG_ONLY 1
#define HAVE___CXA_DEMANGLE 1
#define HAVE_DLADDR 1
#define HAVE_BACKTRACE 1
#define HAVE_BACKTRACE_SYMBOLS 1
#if defined(SPEC) && !defined(__cplusplus)
#define HAVE_CRYPT 1
#define HAVE_FINITE 1
#define HAVE_COPYSIGN 1
#define HAVE_FPCLASSIFY 1
#define HAVE_ISFINITE 1
#define HAVE_ISINF 1
#define HAVE_ISNAN 1
#define HAVE_ISNORMAL 1
#define HAVE_SIGNBIT 1
#endif /* end !__cplusplus */
#define HAVE_MKSTEMP 1
#define HAVE_VA_COPY 1

/* Do we have mode_t ? */
#define HAVE_MODE_T 1

/* Do we have SOCKET ? */
/* #undef HAVE_SOCKET_TYPE */

/* Do we have socklen_t ? Default to 'int' if not. */
#define HAVE_SOCKLEN_T 1
#ifdef HAVE_SOCKLEN_T
#  define CCTK_SOCKLEN_T socklen_t
#else
#  define CCTK_SOCKLEN_T int
#endif

/* Do we have hrtime_t ? */
/* #undef HAVE_HRTIME_T */

/* Some timing functions */
/* #undef HAVE_GETHRTIME */
/* #undef HAVE_READ_REAL_TIME */
/* #undef HAVE_TIME_BASE_TO_TIME */
/* #undef HAVE_CLOCK_GETTIME */
/* #undef HAVE_MACH_ABSOLUTE_TIME */

/* Cray UNICOS _rtc() (real-time clock) intrinsic */
/* #undef HAVE__RTC */

/* Some include things */
#define HAVE_TIME_H 1
/* #undef HAVE_SYS_FILIO_H */
#ifdef SPEC_WINDOWS
#include <crtdefs.h>
//#define access _access
//#define dup2 _dup2
//#define execve _execve
//#define ftruncate _chsize
//#define unlink _unlink
//#define fileno _fileno
//#define getcwd _getcwd
//#define chdir _chdir
//#define isatty _isatty
//#define lseek _lseek
//#define ssize_t int
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
//typedef __int8            int8_t;
//typedef __int16           int16_t; 
//typedef __int32           int32_t;
//typedef __int64           int64_t;
//typedef unsigned __int8   uint8_t;
//typedef unsigned __int16  uint16_t;
//typedef unsigned __int32  uint32_t;
//typedef unsigned __int64  uint64_t;
#else
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_UNISTD_H 1
#endif
#define HAVE_STRING_H 1
#define HAVE_ASSERT_H 1
#define HAVE_TGMATH_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_GETOPT_H 1
#define HAVE_REGEX_H 1
#define HAVE_NETINET_IN_H 1
#ifndef SPEC_WINDOWS
#define HAVE_NETDB_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_CRYPT_H 1
#define HAVE_DIRENT_H 1
#define HAVE_MALLOC_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_MALLINFO 1
#define HAVE_MALLOPT 1
#define HAVE_M_MMAP_THRESHOLD_VALUE 1
#define HAVE_SCHED_H 1
#define HAVE_EXECINFO_H 1
#define HAVE_SCHED_GETAFFINITY 1
#define HAVE_GETPID 1
#else
#define HAVE_WINSOCK2_H 1
#endif 
/* #undef HAVE_C_ASM_H */
/* #undef HAVE_INTRINSICS_H */
/* #undef HAVE_MACH_MACH_TIME_H */

#ifndef SPEC
#define TIME_WITH_SYS_TIME 1
#endif

/* Which format does the C++ STL on this machine provide. */
#define HAVE_VECTOR 1
/* #undef HAVE_VECTOR_H */

/* Timing stuff */
/* #undef HAVE_TIME_GETTIMEOFDAY */
/* #undef GETTIMEOFDAY_NEEDS_TIMEZONE */
/* #undef HAVE_TIME_GETRUSAGE */ 
/* #undef HAVE_TIME__FTIME */

/* Cache stuff */
#define CCTK_CACHELINE_BYTES 64
#define CCTK_CACHE_SIZE 6144*1024

/* The name of the NULL device for redirecting things to */
#define NULL_DEVICE "/dev/null"

/* Define the machine architecture for the build */
#define CCTK_BUILD_OS "linux-gnu"
#define CCTK_BUILD_CPU "x86_64"
#define CCTK_BUILD_VENDOR "unknown"

/******************************************************************************/

/* Size info for various datatypes */
#define SIZEOF_SHORT_INT 2
#define SIZEOF_INT 4
#if defined(SPEC_LP64) 
#define SIZEOF_LONG_INT 8
#else
#define SIZEOF_LONG_INT 4
#endif
#define SIZEOF_LONG_LONG 8
#define SIZEOF_LONG_DOUBLE 16
#define SIZEOF_DOUBLE 8
#define SIZEOF_FLOAT 4
#if defined(SPEC_LP64) || defined(SPEC_P64)
#define SIZEOF_CHAR_P 8
#else
#define SIZEOF_CHAR_P 4
#endif

/* The chosen CCTK precision */

/* Floating point precision */
/* #undef CCTK_REAL_PRECISION_16 */
#define CCTK_REAL_PRECISION_8 1
/* #undef CCTK_REAL_PRECISION_4 */

/* Integer precision */
/* #undef CCTK_INTEGER_PRECISION_8 */
#define CCTK_INTEGER_PRECISION_4 1
/* #undef CCTK_INTEGER_PRECISION_2 */
/* #undef CCTK_INTEGER_PRECISION_1 */

/* Integer sizes */
#define HAVE_CCTK_INT8 1
#define HAVE_CCTK_INT4 1
#define HAVE_CCTK_INT2 1
#define HAVE_CCTK_INT1 1

/* Float sizes */
/* #define HAVE_CCTK_REAL16 1 */
#define HAVE_CCTK_REAL8 1
#define HAVE_CCTK_REAL4 1

/******************************************************************************/

#ifdef CCODE

/* CCTK C/C++ Integer datatypes */
#define CCTK_INT8 long int
#define CCTK_INT4 int
#define CCTK_INT2 short int
#define CCTK_INT1 signed char

/* CCTK C/C++ Float datatypes */
/* #define CCTK_REAL16 long double */
#define CCTK_REAL8 double
#define CCTK_REAL4 float

/* Disable 'restrict' for compiler versions known to be buggy */
/* Note that at the time of writing this comment (2013-10-19), only
 * versions up to and including 20130313 have been tested. Newer
 * versions might actually not show this bug and the workaround could
 * then be removed. At the moment the workaround is enabled for any
 * version >= 20121010 to be on the safe side. */
#if (defined __INTEL_COMPILER                &&                 \
     __INTEL_COMPILER_BUILD_DATE >= 20121010 &&                 \
     !defined CCTK_INTEL_COMPILER_DONT_DISABLE_RESTRICT)
#  define CCTK_DISABLE_RESTRICT 1
#endif

/****************************************************************************/
/* C specific stuff */
/****************************************************************************/
#ifndef __cplusplus

/* Define to empty if the 'inline' keyword does not work. */
#define HAVE_CCTK_C_INLINE 1
//#define inline __inline__ __attribute__((__gnu_inline__))

/* Define to 'static' if the 'static inline' keyword combination does
   not work. */
#define CCTK_C_STATIC_INLINE static inline
#define CCTK_STATIC_INLINE CCTK_C_STATIC_INLINE

/* Define to empty if the 'const' keyword does not work. */
/* #undef const */

/* Define to empty if the 'restrict' keyword does not work. */
#ifdef CCTK_DISABLE_RESTRICT
#  define CCTK_C_RESTRICT
#else
/* #  undef HAVE_CCTK_C_RESTRICT */
/* #  undef CCTK_C_RESTRICT */
#endif

#ifdef CCTK_C_RESTRICT
#  define restrict CCTK_C_RESTRICT
#endif

/* Allow the use of CCTK_RESTRICT as a qualifier always. */
#ifdef CCTK_C_RESTRICT
#  define CCTK_RESTRICT CCTK_C_RESTRICT
#else
#  define CCTK_RESTRICT restrict
#endif

/* Disable _Pragma if unsupported */
#ifndef SPEC_NO_C__PRAGMA
#define HAVE_CCTK_C__PRAGMA 1
#endif
#ifndef HAVE_CCTK_C__PRAGMA
#  define _Pragma(x)
#endif

/* Some C compilers (e.g. C99 ones) define bool */
/* #undef HAVE_CCTK_C_BOOL */

/* deprecated in beta15 */
#ifdef HAVE_CCTK_C_BOOL
#  define CCTK_HAVE_C_BOOL
#endif

/* Since this is non-standard leave commented out for the moment */
#if 0
#ifndef HAVE_CCTK_C_BOOL
typedef enum {false, true} bool;
#endif /* HAVE_CCTK_C_BOOL */
#endif

/* Whether copysign exists, and how it should be called */
#ifdef HAVE_COPYSIGN
#  define HAVE_CCTK_C_COPYSIGN HAVE_COPYSIGN
#  define CCTK_C_COPYSIGN copysign
#  define HAVE_CCTK_COPYSIGN HAVE_CCTK_C_COPYSIGN
#  define CCTK_COPYSIGN CCTK_C_COPYSIGN
#endif

/* Whether fpclassify exists, and how it should be called */
#ifdef HAVE_FPCLASSIFY
#  define HAVE_CCTK_C_FPCLASSIFY HAVE_FPCLASSIFY
#  define CCTK_C_FPCLASSIFY fpclassify
#  define HAVE_CCTK_FPCLASSIFY HAVE_CCTK_C_FPCLASSIFY
#  define CCTK_FPCLASSIFY CCTK_C_FPCLASSIFY
#endif

/* Whether isfinite exists, and how it should be called */
#ifdef HAVE_ISFINITE
#  define HAVE_CCTK_C_ISFINITE HAVE_ISFINITE
#  define CCTK_C_ISFINITE isfinite
#  define HAVE_CCTK_ISFINITE HAVE_CCTK_C_ISFINITE
#  define CCTK_ISFINITE CCTK_C_ISFINITE
#endif

/* Whether isinf exists, and how it should be called */
#ifdef HAVE_ISINF
#  define HAVE_CCTK_C_ISINF HAVE_ISINF
#  define CCTK_C_ISINF isinf
#  define HAVE_CCTK_ISINF HAVE_CCTK_C_ISINF
#  define CCTK_ISINF CCTK_C_ISINF
#endif

/* Whether isnan exists, and how it should be called */
#ifdef HAVE_ISNAN
#  define HAVE_CCTK_C_ISNAN HAVE_ISNAN
#  define CCTK_C_ISNAN isnan
#  define HAVE_CCTK_ISNAN HAVE_CCTK_C_ISNAN
#  define CCTK_ISNAN CCTK_C_ISNAN
#endif

/* Whether isnormal exists, and how it should be called */
#ifdef HAVE_ISNORMAL
#  define HAVE_CCTK_C_ISNORMAL HAVE_ISNORMAL
#  define CCTK_C_ISNORMAL isnormal
#  define HAVE_CCTK_ISNORMAL HAVE_CCTK_C_ISNORMAL
#  define CCTK_ISNORMAL CCTK_C_ISNORMAL
#endif

/* Whether signbit exists, and how it should be called */
#ifdef HAVE_SIGNBIT
#  define HAVE_CCTK_C_SIGNBIT HAVE_SIGNBIT
#  define CCTK_C_SIGNBIT signbit
#  define HAVE_CCTK_SIGNBIT HAVE_CCTK_C_SIGNBIT
#  define CCTK_SIGNBIT CCTK_C_SIGNBIT
#endif

/* Whether __attribute__((const)) exists. */
#undef HAVE_CCTK_C_ATTRIBUTE_CONST 
#ifdef HAVE_CCTK_C_ATTRIBUTE_CONST
#  define CCTK_ATTRIBUTE_CONST __attribute__((__const__))
#else
#  define CCTK_ATTRIBUTE_CONST
#endif

/* Whether __attribute__((pure)) exists. */
#undef HAVE_CCTK_C_ATTRIBUTE_PURE
#ifdef HAVE_CCTK_C_ATTRIBUTE_PURE
#  define CCTK_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#  define CCTK_ATTRIBUTE_PURE
#endif

/* Whether __attribute__((noinline)) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_NOINLINE 1
#undef HAVE_CCTK_C_ATTRIBUTE_NOINLINE 
#ifdef HAVE_CCTK_C_ATTRIBUTE_NOINLINE
#  define CCTK_ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#else
#  define CCTK_ATTRIBUTE_NOINLINE
#endif

/* Whether __attribute__((always_inline)) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_ALWAYS_INLINE 1
#undef HAVE_CCTK_C_ATTRIBUTE_ALWAYS_INLINE 
#ifdef HAVE_CCTK_C_ATTRIBUTE_ALWAYS_INLINE
#  define CCTK_ATTRIBUTE_ALWAYS_INLINE __attribute__((__always_inline__))
#else
#  define CCTK_ATTRIBUTE_ALWAYS_INLINE
#endif

/* Whether __attribute__((unused)) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_UNUSED 1
#undef HAVE_CCTK_C_ATTRIBUTE_UNUSED 
#ifdef HAVE_CCTK_C_ATTRIBUTE_UNUSED
#  define CCTK_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#  define CCTK_ATTRIBUTE_UNUSED
#endif

/* Whether __attribute__((aligned(...))) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_ALIGNED 1
#undef HAVE_CCTK_C_ATTRIBUTE_ALIGNED 
#ifdef HAVE_CCTK_C_ATTRIBUTE_ALIGNED
#  define CCTK_ATTRIBUTE_ALIGNED(x) __attribute__((__aligned__(x)))
#else
#  define CCTK_ATTRIBUTE_ALIGNED(x)
#endif

/* Whether __attribute__((cold)) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_COLD 1
#undef HAVE_CCTK_C_ATTRIBUTE_COLD 
#ifdef HAVE_CCTK_C_ATTRIBUTE_COLD
#  define CCTK_ATTRIBUTE_COLD __attribute__((__cold__))
#else
#  define CCTK_ATTRIBUTE_COLD
#endif

/* Whether __attribute__((hot)) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_HOT 1
#undef HAVE_CCTK_C_ATTRIBUTE_HOT 
#ifdef HAVE_CCTK_C_ATTRIBUTE_HOT
#  define CCTK_ATTRIBUTE_HOT __attribute__((__hot__))
#else
#  define CCTK_ATTRIBUTE_HOT
#endif

/* Whether __attribute__((format(...))) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_FORMAT 1
#undef HAVE_CCTK_C_ATTRIBUTE_FORMAT 
#ifdef HAVE_CCTK_C_ATTRIBUTE_FORMAT
#  define CCTK_ATTRIBUTE_FORMAT(archetype, format, firstarg) __attribute__((__format__(archetype, format, firstarg)))
#else
#  define CCTK_ATTRIBUTE_FORMAT(archetype, format, firstarg)
#endif

/* Whether __attribute__((noreturn)) exists. */
//#define HAVE_CCTK_C_ATTRIBUTE_NORETURN 1
#undef HAVE_CCTK_C_ATTRIBUTE_NORETURN 
#ifdef HAVE_CCTK_C_ATTRIBUTE_NORETURN
#  define CCTK_ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#else
#  define CCTK_ATTRIBUTE_NORETURN
#endif

/* Whether __builtin_expect exists. */
//#define HAVE_CCTK_C_BUILTIN_EXPECT 1
#undef HAVE_CCTK_C_BUILTIN_EXPECT 
#ifdef HAVE_CCTK_C_BUILTIN_EXPECT
#  define CCTK_BUILTIN_EXPECT __builtin_expect
#else
#  define CCTK_BUILTIN_EXPECT(x,y) (x)
#endif

/* Whether __builtin_unreachable exists. */
#undef HAVE_CCTK_C_BUILTIN_UNREACHABLE 
#ifdef HAVE_CCTK_C_BUILTIN_UNREACHABLE
#  define CCTK_BUILTIN_UNREACHABLE __builtin_unreachable
#else
#  define CCTK_BUILTIN_UNREACHABLE() CCTK_Abort(0, 1)
#endif

/* Whether __builtin_assume_aligned exists. */
#undef HAVE_CCTK_C_BUILTIN_ASSUME_ALIGNED 
#ifdef HAVE_CCTK_C_BUILTIN_ASSUME_ALIGNED
#  define CCTK_BUILTIN_ASSUME_ALIGNED __builtin_assume_aligned
#else
#  define CCTK_BUILTIN_ASSUME_ALIGNED(expr, ...) (expr)
#endif

/* OpenMP collapse clause */
#if (defined CCTK_DISABLE_OMP_COLLAPSE ||                               \
     (defined __IBMC__ && defined _ARCH_450D) ||                        \
     (defined __INTEL_COMPILER && __INTEL_COMPILER_BUILD_DATE < 20100801))
/* see http://software.intel.com/en-us/articles/intel-professional-edition-compilers-111-fixes-list/ */
#  define collapse(N)
#  ifndef CCTK_DISABLE_OMP_COLLAPSE
#    error "OpenMP collapse directive disabled for C, but enabled for Fortran -- likely an error in the option list. Please add -DCCTK_DISABLE_OMP_COLLAPSE to FPPFLAGS and CPPFLAGS."
#  endif
#else
/* #  undef collapse */
#endif

#endif /* ! defined __cplusplus */
/****************************************************************************/

/****************************************************************************/
/* C++ specific stuff */
/****************************************************************************/
#ifdef __cplusplus

#define CCTK_STATIC_INLINE static inline

/* Disable _Pragma if unsupported */
#ifndef SPEC_NO_CXX__PRAGMA
#define HAVE_CCTK_CXX__PRAGMA 1
#endif
#ifndef HAVE_CCTK_CXX__PRAGMA
#  define _Pragma(x)
#endif

/* Some C compilers (e.g. C99 ones) define bool */

/* Some C++ compilers don't have bool */
#define HAVE_CCTK_CXX_BOOL 1

#ifndef HAVE_CCTK_CXX_BOOL
typedef enum {false, true} bool;
#else
  /* deprecated in beta15 */
#  define CCTK_HAVE_CXX_BOOL
#endif

#if defined(SPEC_NO_C99_MATH_IN_CXX)
# undef HAVE_CCTK_CXX_COPYSIGN
# undef HAVE_CCTK_CXX_FPCLASSIFY
# undef HAVE_CCTK_CXX_ISFINITE
# undef HAVE_CCTK_CXX_ISINF
# undef HAVE_CCTK_CXX_ISNAN
# undef HAVE_CCTK_CXX_ISNORMAL
# undef HAVE_CCTK_CXX_SIGNBIT
#else
# define HAVE_CCTK_CXX_COPYSIGN 1
# define HAVE_CCTK_CXX_FPCLASSIFY 1
# define HAVE_CCTK_CXX_ISFINITE 1
# define HAVE_CCTK_CXX_ISINF 1
# define HAVE_CCTK_CXX_ISNAN 1
# define HAVE_CCTK_CXX_ISNORMAL 1
# define HAVE_CCTK_CXX_SIGNBIT 1
#endif

/* Whether copysign exists, and how it should be called */
#define CCTK_CXX_COPYSIGN copysign
#ifdef HAVE_CCTK_CXX_COPYSIGN
#  define HAVE_CCTK_COPYSIGN HAVE_CCTK_CXX_COPYSIGN
#  define CCTK_COPYSIGN CCTK_CXX_COPYSIGN
#endif

/* Whether fpclassify exists, and how it should be called */
#ifndef SPEC_IS_MACROS
#  define CCTK_CXX_FPCLASSIFY std::fpclassify
#else
#  define CCTK_CXX_FPCLASSIFY fpclassify
#endif
#ifdef HAVE_CCTK_CXX_FPCLASSIFY
#  define HAVE_CCTK_FPCLASSIFY HAVE_CCTK_CXX_FPCLASSIFY
#  define CCTK_FPCLASSIFY CCTK_CXX_FPCLASSIFY
#endif

/* Whether isfinite exists, and how it should be called */
#ifndef SPEC_IS_MACROS
#  define CCTK_CXX_ISFINITE std::isfinite
#else
#  define CCTK_CXX_ISFINITE isfinite
#endif
#ifdef HAVE_CCTK_CXX_ISFINITE
#  define HAVE_CCTK_ISFINITE HAVE_CCTK_CXX_ISFINITE
#  define CCTK_ISFINITE CCTK_CXX_ISFINITE
#endif

/* Whether isinf exists, and how it should be called */
#ifndef SPEC_IS_MACROS
#  define CCTK_CXX_ISINF std::isinf
#else
#  define CCTK_CXX_ISINF isinf
#endif
#ifdef HAVE_CCTK_CXX_ISINF
#  define HAVE_CCTK_ISINF HAVE_CCTK_CXX_ISINF
#  define CCTK_ISINF CCTK_CXX_ISINF
#endif

/* Whether isnan exists, and how it should be called */
#ifndef SPEC_IS_MACROS
#  define CCTK_CXX_ISNAN std::isnan
#else
#  define CCTK_CXX_ISNAN isnan
#endif
#ifdef HAVE_CCTK_CXX_ISNAN
#  define HAVE_CCTK_ISNAN HAVE_CCTK_CXX_ISNAN
#  define CCTK_ISNAN CCTK_CXX_ISNAN
#endif

/* Whether isnormal exists, and how it should be called */
#ifndef SPEC_IS_MACROS
#  define CCTK_CXX_ISNORMAL std::isnormal
#else
#  define CCTK_CXX_ISNORMAL isnormal
#endif
#ifdef HAVE_CCTK_CXX_ISNORMAL
#  define HAVE_CCTK_ISNORMAL HAVE_CCTK_CXX_ISNORMAL
#  define CCTK_ISNORMAL CCTK_CXX_ISNORMAL
#endif

/* Whether signbit exists, and how it should be called */
#ifndef SPEC_IS_MACROS
#  define CCTK_CXX_SIGNBIT std::signbit
#else
#  define CCTK_CXX_SIGNBIT signbit
#endif
#ifdef HAVE_CCTK_CXX_SIGNBIT
#  define HAVE_CCTK_SIGNBIT HAVE_CCTK_CXX_SIGNBIT
#  define CCTK_SIGNBIT CCTK_CXX_SIGNBIT
#endif

/* Whether __attribute__((const)) exists. */
#undef HAVE_CCTK_CXX_ATTRIBUTE_CONST 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_CONST
#  define CCTK_ATTRIBUTE_CONST __attribute__((__const__))
#else
#  define CCTK_ATTRIBUTE_CONST
#endif
#undef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_CONST 
#ifdef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_CONST
#  define CCTK_MEMBER_ATTRIBUTE_CONST __attribute__((__const__))
#else
#  define CCTK_MEMBER_ATTRIBUTE_CONST
#endif

/* Whether __attribute__((pure)) exists. */
#undef HAVE_CCTK_CXX_ATTRIBUTE_PURE 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_PURE
#  define CCTK_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#  define CCTK_ATTRIBUTE_PURE
#endif
#undef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_PURE 
#ifdef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_PURE
#  define CCTK_MEMBER_ATTRIBUTE_PURE __attribute__((__pure__))
#else
#  define CCTK_MEMBER_ATTRIBUTE_PURE
#endif

/* Whether __attribute__((noinline)) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_NOINLINE 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_NOINLINE 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_NOINLINE
#  define CCTK_ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#else
#  define CCTK_ATTRIBUTE_NOINLINE
#endif
//#define HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_NOINLINE 1
#undef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_NOINLINE 
#ifdef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_NOINLINE
#  define CCTK_MEMBER_ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#else
#  define CCTK_MEMBER_ATTRIBUTE_NOINLINE
#endif

/* Whether __attribute__((always_inline)) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_ALWAYS_INLINE 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_ALWAYS_INLINE 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_ALWAYS_INLINE
#  define CCTK_ATTRIBUTE_ALWAYS_INLINE __attribute__((__always_inline__))
#else
#  define CCTK_ATTRIBUTE_ALWAYS_INLINE
#endif
//#define HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_ALWAYS_INLINE 1
#undef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_ALWAYS_INLINE 
#ifdef HAVE_CCTK_CXX_MEMBER_ATTRIBUTE_ALWAYS_INLINE
#  define CCTK_MEMBER_ATTRIBUTE_ALWAYS_INLINE __attribute__((__always_inline__))
#else
#  define CCTK_MEMBER_ATTRIBUTE_ALWAYS_INLINE
#endif

/* Whether __attribute__((unused)) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_UNUSED 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_UNUSED 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_UNUSED
#  define CCTK_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#  define CCTK_ATTRIBUTE_UNUSED
#endif

/* Whether __attribute__((aligned(...))) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_ALIGNED 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_ALIGNED 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_ALIGNED
#  define CCTK_ATTRIBUTE_ALIGNED(x) __attribute__((__aligned__(x)))
#else
#  define CCTK_ATTRIBUTE_ALIGNED(x)
#endif

/* Whether __attribute__((cold)) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_COLD 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_COLD 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_COLD
#  define CCTK_ATTRIBUTE_COLD __attribute__((__cold__))
#else
#  define CCTK_ATTRIBUTE_COLD
#endif

/* Whether __attribute__((hot)) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_HOT 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_HOT 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_HOT
#  define CCTK_ATTRIBUTE_HOT __attribute__((__hot__))
#else
#  define CCTK_ATTRIBUTE_HOT
#endif

/* Whether __attribute__((format(...))) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_FORMAT 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_FORMAT 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_FORMAT
#  define CCTK_ATTRIBUTE_FORMAT(archetype, format, firstarg) __attribute__((__format__(archetype, format, firstarg)))
#else
#  define CCTK_ATTRIBUTE_FORMAT(archetype, format, firstarg)
#endif

/* Whether __attribute__((noreturn)) exists. */
//#define HAVE_CCTK_CXX_ATTRIBUTE_NORETURN 1
#undef HAVE_CCTK_CXX_ATTRIBUTE_NORETURN 
#ifdef HAVE_CCTK_CXX_ATTRIBUTE_NORETURN
#  define CCTK_ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#else
#  define CCTK_ATTRIBUTE_NORETURN
#endif

/* Whether __builtin_expect exists. */
//#define HAVE_CCTK_CXX_BUILTIN_EXPECT 1
#undef HAVE_CCTK_CXX_BUILTIN_EXPECT 
#ifdef HAVE_CCTK_CXX_BUILTIN_EXPECT
#  define CCTK_BUILTIN_EXPECT __builtin_expect
#else
#  define CCTK_BUILTIN_EXPECT(x,y) (x)
#endif

/* Whether __builtin_unreachable exists. */
#undef HAVE_CCTK_CXX_BUILTIN_UNREACHABLE 
#ifdef HAVE_CCTK_CXX_BUILTIN_UNREACHABLE
#  define CCTK_BUILTIN_UNREACHABLE __builtin_unreachable
#else
#  define CCTK_BUILTIN_UNREACHABLE() CCTK_Abort(0, 1)
#endif

/* Whether __builtin_assume_aligned exists. */
#undef HAVE_CCTK_CXX_BUILTIN_ASSUME_ALIGNED 
#ifdef HAVE_CCTK_CXX_BUILTIN_ASSUME_ALIGNED
#  define CCTK_BUILTIN_ASSUME_ALIGNED __builtin_assume_aligned
#else
#  define CCTK_BUILTIN_ASSUME_ALIGNED(expr, ...) (expr)
#endif

/* Whether static_assert exists. */
#ifdef SPEC_NO_CXX_STATIC_ASSERT
#undef HAVE_CCTK_CXX_STATIC_ASSERT 
#else
#define HAVE_CCTK_CXX_STATIC_ASSERT
#endif
#ifdef HAVE_CCTK_CXX_STATIC_ASSERT
#  define CCTK_STATIC_ASSERT(cond,msg) static_assert(cond,msg)
#else
#  define CCTK_STATIC_ASSERT(cond,msg) do { typedef int ai[(cond) ? 1 : -1]; } while(0)
#  define static_assert(cond,msg) CCTK_STATIC_ASSERT(cond,msg)
#endif

/* Some C++ compilers recognise the restrict keyword */
/* Define to empty if the keyword does not work. */
#define CCTK_DISABLE_RESTRICT 1
#ifdef CCTK_DISABLE_RESTRICT
#  define CCTK_CXX_RESTRICT
#else
#  define HAVE_CCTK_CXX_RESTRICT 1
#  define CCTK_CXX_RESTRICT __restrict__
#endif

#ifdef CCTK_CXX_RESTRICT
#  define restrict CCTK_CXX_RESTRICT
#endif

/* Allow the use of CCTK_RESTRICT as a qualifier always. */
#ifdef CCTK_CXX_RESTRICT
#  define CCTK_RESTRICT CCTK_CXX_RESTRICT
#else
#  define CCTK_RESTRICT restrict
#endif

/* OpenMP collapse clause */
#if (defined CCTK_DISABLE_OMP_COLLAPSE ||                               \
     (defined __IBMCPP__ && defined _ARCH_450D) ||                      \
     ( defined __INTEL_COMPILER && __INTEL_COMPILER_BUILD_DATE < 20100801))
/* see http://software.intel.com/en-us/articles/intel-professional-edition-compilers-111-fixes-list/ */
#  define collapse(N)
#else
/* #  undef collapse */
#endif

#ifdef SPEC
#include <math.h>
#  ifndef INFINITY
#    ifdef SPEC_USE_BUILTIN
#      define INFINITY __builtin_infinity
#    else
#      define INFINITY std::numeric_limits<double>::infinity()
#    endif
#  endif
#  ifndef NAN
#    ifdef SPEC_USE_BUILTIN
#      define NAN __builtin_nan
#    else
#      define NAN std::numeric_limits<double>::quiet_NaN()
#    endif
#  endif
#endif

#endif /* __cplusplus */

/****************************************************************************/

#endif /* CCODE */

#ifdef FCODE

#define HAVE_CCTK_FORTRAN_REAL4 1
#define HAVE_CCTK_FORTRAN_REAL8 1
/* #define HAVE_CCTK_FORTRAN_REAL16 1 */

#define HAVE_CCTK_FORTRAN_COMPLEX8 1
#define HAVE_CCTK_FORTRAN_COMPLEX16 1
/*#define HAVE_CCTK_FORTRAN_COMPLEX32 1 */

#define CCTK_REAL16_KIND 8
#define CCTK_COMPLEX32_KIND 16 

/* OpenMP collapse clause */
#ifdef CCTK_DISABLE_OMP_COLLAPSE
/* see http://software.intel.com/en-us/articles/intel-professional-edition-compilers-111-fixes-list/ */
#  define collapse(N)
#  define COLLAPSE(N)
#else
/* #  undef collapse */
/* #  undef COLLAPSE */
#endif

#endif /* FCODE */

/* Now include the code to pick an appropriate precison for reals and ints */
#include "cctk_Types.h"

/* Include any other stuff which is specific to this architecture */
#include "cctk_Archdefs.h"

/* Include any extra stuff from optional extra packages. */
#include "cctk_Extradefs.h"

#endif /* _CCTK_CONFIG_H_ */
