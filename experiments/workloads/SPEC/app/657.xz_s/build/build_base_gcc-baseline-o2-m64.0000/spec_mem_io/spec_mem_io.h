/* Prototypes and other setup for SPEC's in-memory I/O shim
 *
 * $Id: spec_mem_io.h 5572 2017-04-10 22:49:59Z JohnHenning $
 */
#if !defined(_SPEC_MEM_IO_H)
# define _SPEC_MEM_IO_H

#include <stdio.h>
#include <stdint.h>
#if defined(SPEC_WINDOWS) && defined(SPEC_NEED_INTTYPES_H)
# include "win32/inttypes.h"
#else
# include <inttypes.h>
#endif

#if defined(SPEC_NO_SSIZE_T)
# if defined(SPEC_WINDOWS)
#  include <windows.h>
#  undef ssize_t
#  define ssize_t SSIZE_T
# else
#  if defined(SPEC_P64) || defined(SPEC_LP64) || defined(SPEC_ILP64)
typedef int64_t ssize_t;
#  else
typedef long ssize_t;
#  endif
# endif
#endif

# if defined(__cplusplus)
extern "C" {
# endif 

typedef struct {
    int64_t limit;
    int64_t len;
    int64_t pos;
    int open;
    unsigned char *buf;
} spec_fd_t;
extern spec_fd_t *spec_fd;

spec_fd_t *    spec_mem_alloc_fds (unsigned fd_limit);
void           spec_mem_free_fds (spec_fd_t *fds, unsigned fd_limit);
void           spec_initbufs();
void           spec_compress(int in, int out, unsigned int level);
void           spec_uncompress(int in, int out);
int            spec_mem_init   (spec_fd_t *fds, unsigned fd_limit, unsigned touch_all);
unsigned char *spec_mem_load   (spec_fd_t *fds, unsigned fd_limit, int num, char *filename, int64_t size, int tmpfd, unsigned char *sum, int bits);
int            spec_mem_open   (spec_fd_t *fds, unsigned fd_limit, char *filename, int mode);
int            spec_mem_fopen  (spec_fd_t *fds, unsigned fd_limit, const char *filename, const char *mode);
int64_t        spec_mem_read   (spec_fd_t *fds, unsigned fd_limit, int fd, unsigned char *buf, int64_t size);
int            spec_mem_reset  (spec_fd_t *fds, unsigned fd_limit, int fd);
int64_t        spec_mem_get_length (spec_fd_t *fds, unsigned fd_limit, int fd);
int64_t        spec_mem_lseek  (spec_fd_t *fds, unsigned fd_limit, int fd, int64_t offset, int whence);
int64_t        spec_mem_write  (spec_fd_t *fds, unsigned fd_limit, int fd, unsigned char *buf, int64_t size);
int            spec_mem_getc   (spec_fd_t *fds, unsigned fd_limit, int fd);
int            spec_mem_ungetc (spec_fd_t *fds, unsigned fd_limit, int ch, int fd);
int64_t        spec_mem_fread  (spec_fd_t *fds, unsigned fd_limit, unsigned char *buf, int64_t size, int num, int fd);
int64_t        spec_mem_fwrite (spec_fd_t *fds, unsigned fd_limit, unsigned char *buf, int64_t size, int num, int fd);
int            spec_mem_rewind (spec_fd_t *fds, unsigned fd_limit, int fd);
int            spec_mem_putc   (spec_fd_t *fds, unsigned fd_limit, int ch, int fd);
int            spec_mem_feof   (spec_fd_t *fds, unsigned fd_limit, int fd);
int            spec_mem_close  (spec_fd_t *fds, unsigned fd_limit, int fd);
int            spec_empty (int fd);
int            debug_time(void);
int            compare_sum(unsigned char *ref_sum, unsigned char *generated, int bits);
unsigned char *sum_str_to_hex (char *sum, int bits);
void           print_sum (FILE *out, unsigned char *sum, int bits);
unsigned char *spec_mem_sum (spec_fd_t *fd, unsigned char *sum, int bits);

#if defined(SPEC_MEM_IO)
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <fcntl.h>
# include <ctype.h>
# if defined(SPEC_WINDOWS)
#  include <io.h>
# endif
# if defined(TIMING_OUTPUT)
#  include <time.h>
#  include <sys/time.h>
# endif /* TIMING_OUTPUT */
# include <string.h>
# if !defined(SPEC_WINDOWS)
#  include <unistd.h>
# endif

#ifndef O_BINARY
#define O_BINARY 0
#endif
#define MiB (1024 * 1024)
#define FILE_CHUNK (254 * MiB)

#if defined(DEBUG)
#define debug(level, ...)           { if (level<dbglvl) { printf(__VA_ARGS__); fflush(stdout); } }
#else
#define debug(level, ...)
#endif

#if defined(SPEC_MEMOP_PAUSE)
# define SPEC_MEMOP_PAUSE_SETUP char pausebuf[2];
# define SPEC_PREALLOC_PAUSE( func,op,size) DO_SPEC_MEMOP_PAUSE(func, "About to request " op " of %lu bytes", size)
# define SPEC_POSTALLOC_PAUSE(func,op,size) DO_SPEC_MEMOP_PAUSE(func, op " of %lu bytes successful", size)
# define SPEC_PREFREE_PAUSE(  func,op,ptr)  DO_SPEC_MEMOP_PAUSE(func, "About to perform " op " on 0x%p", (void *)ptr);
# define SPEC_POSTFREE_PAUSE( func,op,ptr)  DO_SPEC_MEMOP_PAUSE(func, "Successfully performed " op " on 0x%p", (void *)ptr);
# define DO_SPEC_MEMOP_PAUSE(func, ...) \
    fprintf(stderr, func " [%s:%d]: ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, " (press enter)"); \
    fread(pausebuf, 1, 1, stdin);
#else
# define SPEC_MEMOP_PAUSE_SETUP
# define SPEC_PREALLOC_PAUSE( func,op,size)
# define SPEC_POSTALLOC_PAUSE(func,op,size)
# define SPEC_PREFREE_PAUSE(  func,op,ptr)
# define SPEC_POSTFREE_PAUSE( func,op,ptr)
#endif

#define FUDGE_BUF (1 * MiB)
#define VALIDATE_SKIP 1027
#define MAX_SPEC_FD 3

/* Map from old spec_fd-only code to new stuff */
#define spec_init()		spec_mem_init	(spec_fd, MAX_SPEC_FD, 1)
#define spec_load(...)		spec_mem_load	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_open(...)		spec_mem_open	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_fopen(...)		spec_mem_fopen	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_read(...)		spec_mem_read	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_reset(...)		spec_mem_reset	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_get_length(...)	spec_mem_get_length(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_lseek(...)		spec_mem_lseek	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_write(...)		spec_mem_write	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_getc(...)		spec_mem_getc	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_ungetc(...)	spec_mem_ungetc	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_fread(...)		spec_mem_fread	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_fwrite(...)	spec_mem_fwrite	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_rewind(...)	spec_mem_rewind	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_putc(...)		spec_mem_putc	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_feof(...)		spec_mem_feof	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_close(...)		spec_mem_close	(spec_fd, MAX_SPEC_FD, __VA_ARGS__)
#define spec_sum(fd, ...)	if (fd < 0 || (unsigned)fd > MAX_SPEC_FD) \
					fprintf(stderr, "spec_sum: in-mem fd %d@%p is out of range: <0 || >= fd_limit %u\n", fd, (void *)spec_fd, MAX_SPEC_FD); \
                                else \
                                	spec_mem_sum	(spec_fd + fd, __VA_ARGS__)

#if !defined(ALREADY_SPEC)
#  ifndef SPEC_STDIN
#    define SPEC_STDIN 0
#  endif
#  ifndef SPEC_STDOUT
#    define SPEC_STDOUT 1
#  endif
#  ifndef SPEC_STDERR
#    define SPEC_STDERR 1
#  endif
#  ifndef SPEC_NULLCAST
#    define SPEC_NULLCAST (int)
#  endif
  /* Munge things for SPEC CPU2017 */
   #include <sys/types.h>

   #define SET_BINARY_MODE(fd) /**/
   #define NORETURN /**/

  /* Fix-up fopen() */
   #ifdef fopen
   #undef fopen
   #endif
   #define fopen(a,b) spec_fopen(a,b)

  /* Fix-up open() */
   #ifdef open
   #undef open
   #endif
   #define open(a,b,...) spec_open(a,b)

  /* Fix-up lseek() */
   #ifdef lseek
   #undef lseek
   #endif
   #define lseek(a,b,c) spec_lseek(a,b,c)

  /* Fix-up rewind() */
   #ifdef rewind
   #undef rewind
   #endif
   #define rewind(a) spec_rewind(a)

  /* Fix-up read() */
   #ifdef read
   #undef read
   #endif
   #define read(a,b,c) spec_read(a,b,c)

  /* Fix-up write() */
   #ifdef write
   #undef write
   #endif
   #define write(a,b,c) spec_write(a,b,c)

  /* Fix-up fread() */
   #ifdef fread
   #undef fread
   #endif
   #define fread(a,b,c,d) spec_fread(a,b,c,d)

  /* Fix-up fwrite() */
   #ifdef fwrite
   #undef fwrite
   #endif
   #define fwrite(a,b,c,d) spec_fwrite(a,b,c,d)

  /* Fix-up ferror() */
   #ifdef ferror
   #undef ferror
   #endif
   #define ferror(x) spec_empty(x)

  /* Fix-up fflush() */
   #ifdef fflush
   #undef fflush
   #endif
   #define fflush(x) spec_empty(x)

  /* Fix-up fclose() */
   #ifdef fclose
   #undef fclose
   #endif
   #define fclose(x) spec_close(x)

  /* Fix-up close() */
   #ifdef close
   #undef close
   #endif
   #define close(x) spec_close(x)

  /* Fix-up getc() */
   #ifdef getc
   #undef getc
   #endif
   #define getc(f) spec_getc((int)f)

  /* Fix-up fgetc() */
   #ifdef fgetc
   #undef fgetc
   #endif
   #define fgetc(f) spec_getc((int)f)

  /* Fix-up fputc() */
   #ifdef fputc
   #undef fputc
   #endif
   #define fputc(c, f) spec_putc(c, (int)f)

  /* Fix-up putc() */
   #ifdef putc
   #undef putc
   #endif
   #define putc(c, f) spec_putc(c, (int)f)

  /* Fix-up ungetc() */
   #ifdef ungetc
   #undef ungetc
   #endif
   #define ungetc(c,f) spec_ungetc(c,(int)f)

  /* Fix-up feof() */
   #ifdef feof
   #undef feof
   #endif
   #define feof(f) spec_feof(f)

#else
   extern int dbglvl;
   #ifndef SPEC_STDIN
   #  define SPEC_STDIN stdin
   #endif
   #ifndef SPEC_STDOUT
   #  define SPEC_STDOUT stdout
   #endif
   #ifndef SPEC_NULLCAST
   #  define SPEC_NULLCAST 
   #endif
#endif /* ALREADY_SPEC */

#if !defined(STDIN_FILENO)
# define STDIN_FILENO SPEC_STDIN
#endif
#if !defined(STDOUT_FILENO)
# define STDOUT_FILENO SPEC_STDOUT
#endif
#if !defined(STDERR_FILENO)
# define STDERR_FILENO SPEC_STDERR
#endif

#endif /* SPEC_MEM_IO */

# if defined(__cplusplus)
}
# endif 
#endif 
