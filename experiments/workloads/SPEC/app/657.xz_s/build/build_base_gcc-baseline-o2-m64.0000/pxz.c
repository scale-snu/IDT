/*
 * Parallel XZ 4.999.9beta,
 * runs LZMA compression simultaneously on multiple cores.
 *
 * Copyright (C) 2009-2014 Jindrich Novy (jnovy@users.sourceforge.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define __STDC_FORMAT_MACROS

#if (defined(_OPENMP) || defined(SPEC_OPENMP)) && !defined(SPEC_SUPPRESS_OPENMP) && !defined(SPEC_AUTO_SUPPRESS_OPENMP)
#if !defined(SPEC)
#include <string.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <utime.h>
#include <signal.h>
#include <getopt.h>
#include <fcntl.h>
#else
# include <stdint.h>
# include <string.h>
#if !defined(SPEC_NO_STDBOOL_H)
# include <stdbool.h>
#elif !defined(__bool_true_false_are_defined)
# if !defined(HAVE__BOOL)
typedef unsigned char _Bool;
# endif
# if defined(SPEC)
#   undef bool
#   undef true
#   undef false
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif
# include <stdarg.h>
# if defined(SPEC_MEM_IO)
#  include "spec_mem_io.h"
# endif
#include <lzma.h>
#include <omp.h>
#define BUFFSIZE (1 * MiB)

void pxz_error(int status, int errnum, const char *format, ...);
#define error pxz_error
bool init_encoder(lzma_stream *strm, lzma_options_lzma *options, uint32_t preset, lzma_check check);
static double opt_context_size = 3.0;
static size_t page_size = 1 * 0x100000; // should cause good enough alignment
static unsigned char buf[BUFSIZ];
#endif // SPEC

#if !defined(SPEC)
#ifndef XZ_BINARY
#define XZ_BINARY "xz"
#endif

#define ADD_OPT(c) \
do { \
	size_t __s = strlen(xzcmd); \
	if (__s+3 >= xzcmd_max) { \
		fprintf(stderr,"xz command line too long\n"); \
		exit(EXIT_FAILURE); \
	} \
	xzcmd[__s] = ' '; \
	xzcmd[__s+1] = '-'; \
	xzcmd[__s+2] = c; \
	xzcmd[__s+3] = '\0';\
} while (0);

FILE **ftemp;
char str[0x100];
char buf[BUFFSIZE];
char *xzcmd;
size_t xzcmd_max;

unsigned opt_complevel = 6, opt_stdout, opt_keep, opt_threads, opt_verbose;
unsigned opt_force, opt_stdout;
double opt_context_size = 3;
FILE *fi, *fo;
char **file;
int files;

const char short_opts[] = "cC:defF:hHlkM:qQrS:tT:D:vVz0123456789";

const struct option long_opts[] = {
	// Operation mode
	{ "compress",       no_argument,       NULL,  'z' },
	{ "decompress",     no_argument,       NULL,  'd' },
	{ "uncompress",     no_argument,       NULL,  'd' },
	{ "test",           no_argument,       NULL,  't' },
	{ "list",           no_argument,       NULL,  'l' },
	// Operation modifiers
	{ "keep",           no_argument,       NULL,  'k' },
	{ "force",          no_argument,       NULL,  'f' },
	{ "stdout",         no_argument,       NULL,  'c' },
	{ "to-stdout",      no_argument,       NULL,  'c' },
	{ "suffix",         required_argument, NULL,  'S' },
//	{ "files",          optional_argument, NULL,  OPT_FILES },
//	{ "files0",         optional_argument, NULL,  OPT_FILES0 },
	// Basic compression settings
	{ "format",         required_argument, NULL,  'F' },
	{ "check",          required_argument, NULL,  'C' },
	{ "memory",         required_argument, NULL,  'M' },
	{ "threads",        required_argument, NULL,  'T' },
	{ "context-size",   required_argument, NULL,  'D' },
	{ "extreme",        no_argument,       NULL,  'e' },
	{ "fast",           no_argument,       NULL,  '0' },
	{ "best",           no_argument,       NULL,  '9' },
	// Filters
/*	{ "lzma1",          optional_argument, NULL,  OPT_LZMA1 },
	{ "lzma2",          optional_argument, NULL,  OPT_LZMA2 },
	{ "x86",            optional_argument, NULL,  OPT_X86 },
	{ "powerpc",        optional_argument, NULL,  OPT_POWERPC },
	{ "ia64",           optional_argument, NULL,  OPT_IA64 },
	{ "arm",            optional_argument, NULL,  OPT_ARM },
	{ "armthumb",       optional_argument, NULL,  OPT_ARMTHUMB },
	{ "sparc",          optional_argument, NULL,  OPT_SPARC },
	{ "delta",          optional_argument, NULL,  OPT_DELTA },
	{ "subblock",       optional_argument, NULL,  OPT_SUBBLOCK },
*/	// Other options
	{ "quiet",          no_argument,       NULL,  'q' },
	{ "verbose",        no_argument,       NULL,  'v' },
	{ "no-warn",        no_argument,       NULL,  'Q' },
	{ "help",           no_argument,       NULL,  'h' },
	{ "long-help",      no_argument,       NULL,  'H' },
	{ "version",        no_argument,       NULL,  'V' },
	{ NULL,             0,                 NULL,   0 }
};

void __attribute__((noreturn)) run_xz( char **argv ) {
	execvp(XZ_BINARY, argv);
	error(0, errno, "execution of "XZ_BINARY" binary failed");
	exit(EXIT_FAILURE);
}

void parse_args( int argc, char **argv ) {
	int c;
	
        opterr = 0;
	while ((c = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
		switch (c) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				opt_complevel = c - '0';
				ADD_OPT(c);
				break;
			case 'v':
				opt_verbose = 1;
				break;
			case 'f':
				opt_force = 1;
				break;
			case 'e':
			case 'q':
			case 'Q':
				ADD_OPT(c);
				break;
			case 'c':
				opt_stdout = 1;
				break;
			case 'k':
				opt_keep = 1;
				break;
			case 'T':
				opt_threads = atoi(optarg);
				break;
			case 'D':
				opt_context_size = atof(optarg);
				if ( opt_context_size <= 0 ) {
					error(EXIT_FAILURE, 0, "Invalid context size specified");
				}
				break;
			case 'h':
			case 'H':
				printf("Parallel PXZ-"PXZ_VERSION"-"PXZ_BUILD_DATE", by Jindrich Novy <jnovy@users.sourceforge.net>\n\n"
					"Options:\n"
					"  -T, --threads       maximum number of threads to run simultaneously\n"
					"  -D, --context-size  per-thread compression context size as a multiple\n"
					"                      of dictionary size. Default is 3.\n\n"
					"Usage and other options are same as in XZ:\n\n");
				run_xz(argv);
				break;
			case 'V':
				printf("Parallel PXZ "PXZ_VERSION" (build "PXZ_BUILD_DATE")\n");
				run_xz(argv);
				break;
			case 'd':
			case 't':
			case 'l':
			case '?':
				run_xz(argv);
			default:
				break;
		}
	}
	
	if (!argv[optind]) {
		file = malloc(sizeof(*file));
		*file = "-";
		files = 1;
	} else {
		file = &argv[optind];
		files = argc-optind;
		for (c=0; c<files; c++) {
			struct stat s;
			
			if ( file[c][0] == '-' && file[c][1] == '\0' ) {
				continue;
			}
			
			if ( stat(file[c], &s)) {
				error(EXIT_FAILURE, errno, "can't stat '%s'", file[c]);
			}
		}
	}
}

void __attribute__((noreturn) )term_handler( int sig __attribute__ ((unused)) ) {
	if ( fo != stdout && unlink(str) ) {
		error(0, errno, "error deleting corrupted target archive %s", str);
	}
	exit(EXIT_FAILURE);
}
#endif // !SPEC

#if defined(SPEC_MEM_IO)
int close_stream( int f )
#else
int close_stream( FILE *f )
#endif
{
	if ( ferror(f) ) {
		if ( !fclose(f) ) {
			errno = 0;
		}
		return EOF;
	}
	
#if !defined(SPEC_MEM_IO)
	if ( fclose(f) && (__fpending(f) || errno != EBADF) ) {
		return EOF;
	}
#endif // !SPEC_MEM_IO
	
	return 0;
}

#if defined(SPEC)
void compressStream ( int fi, int fo, uint32_t preset )
#else
int main( int argc, char **argv )
#endif // !SPEC
{
#if !defined(SPEC)
	uint64_t p, threads, chunk_size;
	uint8_t *m;
	ssize_t rd, ts = 0;
	int i;
	struct stat s;
	size_t page_size;
	struct sigaction new_action, old_action;
	struct utimbuf u;
	lzma_filter filters[LZMA_FILTERS_MAX + 1];
	lzma_options_lzma lzma_options;
	
	xzcmd_max = sysconf(_SC_ARG_MAX);
	page_size = sysconf(_SC_PAGE_SIZE);
	xzcmd = malloc(xzcmd_max);
	snprintf(xzcmd, xzcmd_max, XZ_BINARY);
	
	parse_args(argc, argv);

	lzma_lzma_preset(&lzma_options, opt_complevel);

	filters[0].id = LZMA_FILTER_LZMA2;
	filters[0].options = &lzma_options;
	filters[1].id = LZMA_VLI_UNKNOWN;
#else
	int64_t p, threads, chunk_size;
	uint8_t *m;
	ssize_t rd, ts = 0;
	int i;
	lzma_options_lzma lzma_options;
        spec_fd_t *tmpfds;

        // Figure out which check method to use based on whether the extreme
        // encoding bit is set.  CRC64 is default for the command line tool,
        // but let's check SHA256 as well.
        lzma_check check = (preset & LZMA_PRESET_EXTREME) ? LZMA_CHECK_SHA256 : LZMA_CHECK_CRC64;

        // This will get re-done in init_encoder, but it needs to be done here
        // in order to properly initialize the dictionary size
	lzma_lzma_preset(&lzma_options, preset);

        threads = omp_get_max_threads();
#endif // SPEC

	
#if !defined(SPEC)
	for (i=0; i<files; i++) {
		int std_in = file[i][0] == '-' && file[i][1] == '\0';
#ifdef _OPENMP
		threads = omp_get_max_threads();
#else
		threads = 1;
#endif
		if ( (rd=strlen(file[i])) >= 3 && !strncmp(&file[i][rd-3], ".xz", 3) ) {
			if (opt_verbose) {
				error(EXIT_FAILURE, 0, "ignoring '%s', it seems to be already compressed", file[i]);
			}
			continue;
		}
		
		if ( !std_in ) {
			if ( stat(file[i], &s)) {
				error(EXIT_FAILURE, errno, "can't stat '%s'", file[i]);
			}
		}
#endif // !SPEC
		
		chunk_size = opt_context_size * lzma_options.dict_size;
		chunk_size = (chunk_size + page_size)&~(page_size-1);
#if defined(SPEC)
                if ((int64_t)(threads * chunk_size) > spec_get_length(fi) &&
                    threads > (spec_get_length(fi) / chunk_size) + 1) {
                  // Trim down the number of threads so that we don't allocate
                  // a bunch of memory that won't be used.
                  threads = (spec_get_length(fi) / chunk_size) + 1;
                  fprintf(stderr, "requested threads limited to only those that will do actual work: %" PRIu64 " / %lu => %lu\n",
                                  (uint64_t)spec_get_length(fi), (unsigned long)chunk_size, (unsigned long)threads);
                } else {
                  fprintf(stderr, "work available for up to %" PRIu64 " / %lu => %lu threads\n",
                                  (uint64_t)spec_get_length(fi), (unsigned long)chunk_size,
                                  (unsigned long)(spec_get_length(fi) / chunk_size) + 1);
                }

                // Set up temporary output files, one per thread.  It seems
                // reasonable to assume that each thread will not output more
                // than chunk_size output for chunk_size input
                tmpfds = spec_mem_alloc_fds(threads);
                for(i = 0; (unsigned)i < threads; i++) {
                        tmpfds[i].limit = chunk_size;
                }
                spec_mem_init(tmpfds, threads, 0);
                fprintf(stderr, "context size per thread: %u KB\n", (unsigned)(chunk_size >> 10));

#else
		if ( opt_verbose ) {
			fprintf(stderr, "context size per thread: %"PRIu64" B\n", chunk_size);
		}
		
		if ( opt_threads && (threads > opt_threads || opt_force) ) {
			threads = opt_threads;
		}
		
		fo = stdout;
		if ( std_in ) {
			fi = stdin;
		} else {
			if ( !(fi=fopen(file[i], "rb")) ) {
				error(EXIT_FAILURE, errno, "can't open '%s' for reading", file[i]);
			}
			if ( !opt_stdout ) {
				snprintf(str, sizeof(str), "%s.xz", file[i]);
				if ( !(fo=fopen(str, "wb")) ) {
					error(EXIT_FAILURE, errno, "error creating target archive '%s'", str);
				}
			}
		}
		
		if ( opt_verbose ) {
			if ( fo != stdout ) {
				fprintf(stderr, "%s -> %"PRIu64"/%"PRIu64" thread%c: [", file[i], threads, (s.st_size+chunk_size-1)/chunk_size, threads != 1 ? 's' : ' ');
			} else {
				fprintf(stderr, "%"PRIu64" thread%c: [", threads, threads != 1 ? 's' : ' ');
			}
			fflush(stderr);
		}
		
                SPEC_MEMOP_PAUSE_SETUP
                SPEC_PREALLOC_PAUSE("compressStream", "malloc", threads * chunk_size)
		m  = malloc(threads*chunk_size);
                SPEC_POSTALLOC_PAUSE("compressStream", "malloc", threads * chunk_size)
		
		new_action.sa_handler = term_handler;
		sigemptyset (&new_action.sa_mask);
		new_action.sa_flags = 0;
		
		sigaction(SIGINT, NULL, &old_action);
		if (old_action.sa_handler != SIG_IGN) sigaction(SIGINT, &new_action, NULL);
		sigaction(SIGHUP, NULL, &old_action);
		if (old_action.sa_handler != SIG_IGN) sigaction(SIGHUP, &new_action, NULL);
		sigaction(SIGTERM, NULL, &old_action);
		if (old_action.sa_handler != SIG_IGN) sigaction(SIGTERM, &new_action, NULL);
		
                SPEC_PREALLOC_PAUSE("compressStream", "malloc", threads * sizeof(ftemp[0]))
		ftemp = malloc(threads*sizeof(ftemp[0]));
                SPEC_POSTALLOC_PAUSE("compressStream", "malloc", threads * sizeof(ftemp[0]))
#endif

#if defined(SPEC)
                /* This sort of direct access really breaks the abstraction,
                 * but since the "file" is already in memory, it doesn't
                 * seem worth doubling the memory requirement for the sake
                 * of a pretty idea.
                 */
		m  = spec_fd[fi].buf; // Pointer to temp. input buffer, shared by all

		while ( m < spec_fd[fi].buf + spec_get_length(fi) ) {
#else                  
		
		while ( !feof(fi) ) {
			size_t actrd;
#endif // !SPEC
			
			for (p=0; p<threads; p++) {
#if defined(SPEC)
                                // Reset the temporary output "files"
                                tmpfds[p].open = 1;
                                spec_mem_reset(tmpfds, threads, p);
                                spec_mem_rewind(tmpfds, threads, p);
			}
                        // Set the pointers for the input "file"
                        if (spec_get_length(fi) - (m - spec_fd[fi].buf) <= (int64_t)(threads * chunk_size)) {
                          rd = spec_get_length(fi) - (m - spec_fd[fi].buf);
                        } else {
                          rd = threads * chunk_size;
                        }
#else
				ftemp[p] = tmpfile();
			}
			
			for ( actrd=rd=0; !feof(fi) && !ferror(fi) && (uint64_t)rd < threads*chunk_size; rd += actrd) {
				actrd = fread(&m[rd], 1, threads*chunk_size-actrd, fi);
			}
			if (ferror(fi)) {
				error(EXIT_FAILURE, errno, "error in reading input");
			}
#endif // !SPEC

#pragma omp parallel for private(p) num_threads((unsigned)threads)
			for ( p=0; p<(rd+chunk_size-1)/chunk_size; p++ ) {
				off_t pt, len = rd-p*chunk_size >= chunk_size ? chunk_size : rd-p*chunk_size;
				uint8_t *mo;
				lzma_stream strm = LZMA_STREAM_INIT;
				lzma_ret ret;
				
                                // No SPEC_PREALLOC_PAUSE in the parallel section
				mo = malloc(BUFFSIZE);
				
#if defined(SPEC)
                                if (mo == NULL)
                                        error(EXIT_FAILURE, errno, "error allocating thread input buffers");

                                if (!init_encoder(&strm, &lzma_options, preset, check)) {
                                    // Error message has already been printed,
                                    // but we're in the middle of an OpenMP
                                    // block, so just give up
                                    exit(1);
                                }
#else
				if ( lzma_stream_encoder(&strm, filters, LZMA_CHECK_CRC64) != LZMA_OK ) {
					error(EXIT_FAILURE, errno, "unable to initialize LZMA encoder");
				}
#endif // !SPEC
				for (pt=0; pt<len; pt+=BUFFSIZE) {
					strm.next_in = &m[p*chunk_size+pt];
					strm.avail_in = len-pt >= BUFFSIZE ? BUFFSIZE : len-pt;
					strm.next_out = mo;
					strm.avail_out = BUFFSIZE;
					do {
						ret = lzma_code(&strm, LZMA_RUN);
						if ( ret != LZMA_OK ) {
							error(EXIT_FAILURE, 0, "error in LZMA_RUN");
						}
						if ( BUFFSIZE - strm.avail_out > 0 ) {
#if defined(SPEC)
							if ( !spec_mem_fwrite(tmpfds, threads, mo, 1, BUFFSIZE - strm.avail_out, p) )
#else
							if ( !fwrite(mo, 1, BUFFSIZE - strm.avail_out, ftemp[p]) )
#endif // !SPEC
                                                        {
								error(EXIT_FAILURE, errno, "writing to temp file failed");
							}
							strm.next_out = mo;
							strm.avail_out = BUFFSIZE;
						}
					} while ( strm.avail_in );
				}
				
				strm.next_out = mo;
				strm.avail_out = BUFFSIZE;
				do {
					ret = lzma_code(&strm, LZMA_FINISH);
					if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
						error(EXIT_FAILURE, 0, "error in LZMA_FINISH");
					}
					if ( BUFFSIZE - strm.avail_out > 0 ) {
#if defined(SPEC)
						if ( !spec_mem_fwrite(tmpfds, threads, mo, 1, BUFFSIZE - strm.avail_out, p) )
#else
						if ( !fwrite(mo, 1, BUFFSIZE - strm.avail_out, ftemp[p]) )
#endif // !SPEC
                                                {
							error(EXIT_FAILURE, errno, "writing to temp file failed");
						}
						strm.next_out = mo;
						strm.avail_out = BUFFSIZE;
					}
				} while ( ret == LZMA_OK );
				lzma_end(&strm);
				
                                // No SPEC_PREFREE_PAUSE in the parallel section
				free(mo);
				
#if !defined(SPEC)
				if ( opt_verbose ) {
					fprintf(stderr, "%"PRIu64" ", p);
					fflush(stderr);
				}
#endif // !SPEC
			}
#if defined(SPEC)
                        m += rd;        // Advance the "file pointer"
#endif // SPEC
			
			for ( p=0; p<threads; p++ ) {
#if defined(SPEC)
				spec_mem_rewind(tmpfds, threads, p);
				while ( (rd=spec_mem_fread(tmpfds, threads, buf, 1, sizeof(buf), p)) > 0 )
#else
				rewind(ftemp[p]);
				while ( (rd=fread(buf, 1, sizeof(buf), ftemp[p])) > 0 )
#endif // !SPEC
                                {
					if ( fwrite(buf, 1, rd, fo) != rd ) {
						error(0, errno, "writing to archive failed");
#if !defined(SPEC)
						if ( fo != stdout && unlink(str) ) {
							error(0, errno, "error deleting corrupted target archive %s", str);
						}
#endif // !SPEC
						exit(EXIT_FAILURE);
					} else ts += rd;
				}
				if (rd < 0) {
					error(0, errno, "reading from temporary file failed");
#if !defined(SPEC)
					if ( fo != stdout && unlink(str) ) {
						error(0, errno, "error deleting corrupted target archive %s", str);
					}
#endif // !SPEC
					exit(EXIT_FAILURE);
				}
#if defined(SPEC)
				if ( spec_mem_close(tmpfds, threads, p) )
#else
				if ( close_stream(ftemp[p]) )
#endif // !SPEC
                                {
					error(0, errno, "I/O error in temp file");
				}
			}
		}
		
#if defined(SPEC)
		if ( close_stream(fi) )
#else
		if ( fi != stdin && close_stream(fi) )
#endif // !SPEC
                {
			error(0, errno, "I/O error in input file");
		}
		
#if defined(SPEC)
                spec_mem_free_fds(tmpfds, threads);
                if ( close_stream(fo) ) {
                        error(0, errno, "I/O error in target archive");
                }

	return;
#else
		if ( opt_verbose ) {
			fprintf(stderr, "] ");
		}

		free(ftemp);
		
		if ( fo != stdout ) {
			if ( close_stream(fo) ) {
				error(0, errno, "I/O error in target archive");
			}
		} else return 0;
		
		if ( chmod(str, s.st_mode) ) {
			error(0, errno, "warning: unable to change archive permissions");
		}

		u.actime = s.st_atime;
		u.modtime = s.st_mtime;
		
		if ( utime(str, &u) ) {
			error(0, errno, "warning: unable to change archive timestamp");
		}
		
		sigaction(SIGINT, &old_action, NULL);
		sigaction(SIGHUP, &old_action, NULL);
		sigaction(SIGTERM, &old_action, NULL);
		
		if ( opt_verbose ) {
			fprintf(stderr, "%"PRIu64" -> %zd %3.3f%%\n", s.st_size, ts, ts*100./s.st_size);
		}
		
		if ( !opt_keep && unlink(file[i]) ) {
			error(0, errno, "error deleting input file %s", file[i]);
		}
	}
	
	return 0;
#endif // !SPEC

}

#if defined(SPEC)
void pxz_error(int status, int errnum, const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fprintf(stderr, "spec_oxz: ");
  vfprintf(stderr, format, ap);
  va_end(ap);

  if (errnum) {
    fprintf(stderr, ": %s", strerror(errnum));
  }
  fprintf(stderr, "\n");
  if (status) {
    exit(status);
  }
}
#endif // SPEC
#else
static int nonparallel_xz = 1;
#endif // !(_OPENMP || SPEC_OPENMP) || SPEC_SUPPRESS_OPENMP
