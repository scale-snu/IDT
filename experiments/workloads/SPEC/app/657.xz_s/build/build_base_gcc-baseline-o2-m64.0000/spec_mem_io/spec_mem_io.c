/* In-memory I/O shims for SPEC benchmarks
 *
 * $Id: spec_mem_io.c 5572 2017-04-10 22:49:59Z JohnHenning $
 */

#if defined(__cplusplus)
extern "C" {
#endif

#define DEBUG
#define ALREADY_SPEC

#include <stdint.h>
#if defined(SPEC_WINDOWS) && defined(SPEC_NEED_INTTYPES_H)
# include "win32/inttypes.h"
#else
# include <inttypes.h>
#endif
#include "spec_mem_io.h"
#include "sha512.h"

spec_fd_t *spec_fd;
#if defined(SPEC_XZ_DEBUG) && SPEC_XZ_DEBUG > 3
int dbglvl = SPEC_XZ_DEBUG;
#else
int dbglvl = 3;
#endif

#if defined(SPEC_BZIP)
/* Control variables from libbzip2 */
extern unsigned char smallMode;
extern int     verbosity;
extern int     bsStream;
extern int     workFactor, blockSize100k;

/* Prototypes for stuff in bzip2.c */
void allocateCompressStructures ( void );
unsigned char uncompressStream ( int zStream, int stream );
void compressStream ( int zStream, int stream );

#elif defined(SPEC_XZ)
# include "lzma.h"

/* Prototypes for stuff in spec_xz.c */
void uncompressStream ( int zStream, int stream );
void compressStream ( int stream, int zStream, uint32_t preset );

/* Support functions */
static unsigned char hex_char_to_num ( char digit );
static void fd_grow( spec_fd_t *fds, int fd, const char *funcname );

#endif

static void dump_fd(spec_fd_t *fds, unsigned fd_limit, int fd) {
    if (fd < 0 || (unsigned)fd >= fd_limit) {
        fprintf(stderr, "dump_fd: in-mem fd %d@%p is out of range: < 0 || >= fd_limit %u\n", fd, (void *)fds, fd_limit);
        exit(1);
    }
    printf("dump_fd: fd %d@%p (start addr %p) is %s\n", fd, (void *)fds, (void *)&fds[fd], fds[fd].open ? "open" : "CLOSED");
    printf("\tlimit = %" PRId64 "\n", fds[fd].limit);
    printf("\tlen = %" PRId64 "\n", fds[fd].len);
    printf("\tpos = %" PRId64 "\n", fds[fd].pos);
    printf("\tbuf = %p - %p\n", fds[fd].buf, fds[fd].buf + fds[fd].limit);
    fflush(stdout);
}

int spec_mem_init (spec_fd_t *fds, unsigned fd_limit, unsigned touch_all) {
    unsigned i;
    int64_t j;
    SPEC_MEMOP_PAUSE_SETUP
    debug(4, "spec_mem_init(fds=0x%p, fd_limit=%u, touch_all=%d)\n", fds == spec_fd ? NULL : (void *)fds, fd_limit, touch_all);

    /* Clear the fds structure */

    /* Allocate some memory */
    for (i = 0; i < fd_limit; i++) {
	int64_t limit = fds[i].limit;
        memset(&fds[i], 0, sizeof(spec_fd_t));
	fds[i].limit = limit;
	fds[i].open = 0;
	if (limit < 0) {
	    fprintf(stderr, "spec_mem_init: Overflow in requested size (%" PRId64 ") for fd %d\n", limit+FUDGE_BUF, i);
	    exit(1);
	}
        SPEC_PREALLOC_PAUSE("spec_mem_init", "malloc", limit + FUDGE_BUF)
	fds[i].buf = (unsigned char *)malloc(limit+FUDGE_BUF);
	if (fds[i].buf == NULL) {
	    fprintf(stderr, "spec_mem_init: Error mallocing %" PRId64 " bytes for fd %d@%p!\n", limit+FUDGE_BUF, i, (void *)fds);
	    exit(1);
	}
        SPEC_POSTALLOC_PAUSE("spec_mem_init", "malloc", limit + FUDGE_BUF)
        debug(4, " fd[%d] limit is %" PRId64 " bytes (%" PRId64 " bytes allocated)\n", i, fds[i].limit, limit+FUDGE_BUF);
        if (touch_all)
            for (j = 0; j < limit; j += 1024) {
                fds[i].buf[j] = 0;
            }
    }
    return 0;
}

unsigned char *spec_mem_sum (spec_fd_t *fd, unsigned char *sum, int bits) {
    sha512_state ctx;
    int64_t currpos = 0;
    long chunk = 0;

    // stupid
    if (bits != 512) {
      fprintf(stderr, "spec_mem_sum: Only SHA-512 is available\n");
      exit(1);
    }

    if (sum != NULL) {
        sha_init(&ctx);
        for(currpos = 0, chunk = INT32_MAX; currpos < fd->len; currpos += chunk) {
            if (fd->len - currpos < chunk)
                chunk = fd->len - currpos;
            sha_process(&ctx, fd->buf + currpos, chunk);
        }
        sha_done(&ctx, sum);
    }
    return sum;
}

unsigned char *spec_mem_load (spec_fd_t *fds, unsigned fd_limit, int num, char *filename, int64_t size, int tmpfd, unsigned char *sum, int bits) {
    int fd;
    int64_t i, rc;
    unsigned char *gen_sum;
    SPEC_MEMOP_PAUSE_SETUP

    SPEC_PREALLOC_PAUSE("spec_mem_load", "malloc", sizeof(char) * bits / 8);
    gen_sum = (unsigned char *)malloc(sizeof(char) * bits / 8);
    if (gen_sum == NULL) {
        fprintf(stderr, "spec_mem_load: couldn't allocate %d chars for SHA-%d sum\n", bits / 8, bits);
        exit(1);
    }
    SPEC_POSTALLOC_PAUSE("spec_mem_load", "malloc", sizeof(char) * bits / 8);

    if (tmpfd < 0 || (unsigned)tmpfd >= fd_limit) {
        fprintf(stderr, "spec_mem_load: in-mem fd tmpfd %d@%p is out of range: < 0 || >= fd_limit %u\n", tmpfd, (void *)fds, fd_limit);
        exit(1);
    }
    if (num < 0 || (unsigned)num >= fd_limit) {
        fprintf(stderr, "spec_mem_load: in-mem fd num %d@%p is out of range: < 0 || >= fd_limit %u\n", num, (void *)fds, fd_limit);
        exit(1);
    }

    /* Read file named by filename into the file buffer specified by tmpfd,
     * decompress it into the file buffer specified by num, verify its SHA-2
     * sum, and duplicate it until the desired size is achieved.
     */
    fd = open(filename, O_RDONLY|O_BINARY);
    if (fd < 0) {
	fprintf(stderr, "Can't open file %s: %s\n", filename, strerror(errno));
	exit (0);
    }

    fds[tmpfd].pos = fds[tmpfd].len = fds[tmpfd].open = 0;
    while((rc = read(fd, fds[tmpfd].buf + fds[tmpfd].len, FILE_CHUNK)) > 0) {
	fds[tmpfd].len += rc;
        if (rc < FILE_CHUNK) {
            /* This condition normally indicates that the end of the file has
             * been read, but it can also happen when the read() is
             * interrupted by a signal.  Since we don't know the size of the
             * input file (and I don't want to do a stat() to find out),
             * that's just going to tank everything.  But that's okay...
             * benchmark processes shouldn't be receiving signals anyway.
             */
             break;
        }
        if (fds[tmpfd].len + FILE_CHUNK > fds[tmpfd].limit) {
            /* FILE_CHUNK is big enough that this shouldn't happen.  But
             * just in case...
             */
            fd_grow(fds, tmpfd, "spec_mem_load");
        }
    }
    if (rc < 0) {
        fprintf(stderr, "Error reading from %s: %s\n", filename, strerror(errno));
        exit (1);
    }
    close(fd);

    /* Now do the decompression */
    fds[num].pos = fds[num].len = 0;
    spec_uncompress(tmpfd, num);
    debug(1, "Compressed size: %" PRId64 "; Uncompressed size: %" PRId64 "\n", fds[tmpfd].len, fds[num].len);
    fds[tmpfd].pos = fds[tmpfd].len = 0;

    spec_mem_sum(fds + num, gen_sum, bits);
    if (compare_sum(sum, gen_sum, bits)) {
        fprintf(stderr, "Computed SHA-%d sum for decompressed input did not match expected\n", bits);
        fprintf(stderr, "  Generated: ");
        print_sum(stderr, gen_sum, bits);
        fprintf(stderr, "\n   Expected: ");
        print_sum(stderr, sum, bits);
        fprintf(stderr, "\n");
        exit (1);
    }
    debug(1, "SHA-%d of decompressed data compared successfully!\n", bits);

    if (fds[num].len > size)
    {
        debug(1, "Decompressed data too large; truncating to requested size %" PRId64 "\n", size);
        fds[num].len = size;
    }
    fds[num].pos = 0;

    while (fds[num].len < size) {
	int64_t tmp = size - fds[num].len;
	if (tmp > fds[num].len) tmp = fds[num].len;
	debug(3, "Duplicating %" PRId64 " bytes\n", tmp);
	memcpy(fds[num].buf+fds[num].len, fds[num].buf, tmp);
	fds[num].len += tmp;
    }

    /* Compute the SHA-2 sum of the full input */
    spec_mem_sum(fds + num, gen_sum, bits);
    printf("SHA-%d of input file: ", bits);
    print_sum(stdout, gen_sum, bits);
    printf("\n");

    return gen_sum;
}

int64_t spec_mem_read (spec_fd_t *fds, unsigned fd_limit, int fd, unsigned char *buf, int64_t size) {
    int rc = 0;
    debug(5, "spec_mem_read: %d@%p, %p, %" PRId64 " = ", fd, (void *)fds, (void *)buf, size);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_read: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
        fprintf(stderr, "spec_mem_read(%p, %u, %d, %p, %" PRId64 ") => EBADF\n", (void *)fds, fd_limit, fd, buf, size);
	errno = EBADF;
	return -1;
    }
    if (fds[fd].pos >= fds[fd].len) {
	debug(4,"pos (%" PRId64 ") => len (%" PRId64 "); returning EOF (0)\n", fds[fd].pos, fds[fd].len);
	return 0;
    }
    if (fds[fd].pos + size >= fds[fd].len) {
	rc = fds[fd].len - fds[fd].pos;
    } else {
	rc = size;
    }
    memcpy(buf, &(fds[fd].buf[fds[fd].pos]), rc);
    fds[fd].pos += rc;
    debug(5, "%d\n", rc);
    debug(5, "spec_mem_read: new pos for fd %d@%p is %" PRId64 "\n", fd, (void *)fds, fds[fd].pos);
    return rc;
}

int64_t spec_mem_fread (spec_fd_t *fds, unsigned fd_limit, unsigned char *buf, int64_t size, int num, int fd) {
    int64_t rc = 0;
    debug(5, "spec_mem_fread(%p, %u, %p, (%" PRId64 " x %d) fd %d) =", (void *)fds, fd_limit, (void *)buf, size, num, fd);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_fread: fd=%d@%p out of range: < 0 || > fd_limit %d!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
	fprintf(stderr, "spec_mem_fread: fd=%d@%p is not open\n", fd, (void *)fds);
	exit(1);
    }
    if (fds[fd].pos >= fds[fd].len) {
	debug(5, "EOF\n");
	return 0; // It's EOF, but the caller has to call feof() to find out
    }
    if (fds[fd].pos + (size*num) >= fds[fd].len) {
	rc = (fds[fd].len - fds[fd].pos) / size;
    } else {
	rc = num;
    }
    memcpy(buf, &(fds[fd].buf[fds[fd].pos]), rc);
    fds[fd].pos += rc * size;
    debug(5, "%" PRId64 "\n", (rc * size));
    debug(5, "spec_mem_fread: new pos for fd %d@%p is %" PRId64 "\n", fd, (void *)fds, fds[fd].pos);
    return rc;
}

int spec_mem_getc (spec_fd_t *fds, unsigned fd_limit, int fd) {
    int rc = 0;
    debug(6, "spec_mem_getc(%p, %u, %d) = ", (void *)fds, fd_limit, fd);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_getc: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
	fprintf(stderr, "spec_mem_getc: fd=%d@%p is not open\n", fd, (void *)fds);
	exit(1);
    }
    if (fds[fd].pos >= fds[fd].len) {
	debug(6, "EOF\n");
	return EOF;
    }
    rc = fds[fd].buf[fds[fd].pos++];
    debug(6, "%d\n", rc);
    debug(5, "spec_mem_getc: new pos for fd %d@%p is %" PRId64 "\n", fd, (void *)fds, fds[fd].pos);
    return rc;
}

int spec_mem_ungetc (spec_fd_t *fds, unsigned fd_limit, int ch, int fd) {
    int rc = 0;
    debug(6, "spec_mem_ungetc(%p, %u, 0x%02x, %d) = ", (void *)fds, fd_limit, (unsigned char)ch, fd);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_ungetc: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
	fprintf(stderr, "spec_mem_ungetc: fd=%d@%p is not open\n", fd, (void *)fds);
	exit(1);
    }
    if (fds[fd].pos <= 0) {
	fprintf(stderr, "spec_mem_ungetc: pos %" PRId64 " <= 0\n", fds[fd].pos);
	exit(1);
    }
    if (fds[fd].buf[--fds[fd].pos] != ch) {
	fprintf(stderr, "spec_mem_ungetc: can't unget something that wasn't what was in the buffer!\n");
	exit(1);
    }
    debug(6, "%d\n", rc);
    debug(5, "spec_mem_ungetc: new pos for fd %d@%p is %" PRId64 "\n", fd, (void *)fds, fds[fd].pos);
    return ch;
}

int64_t spec_mem_lseek(spec_fd_t *fds, unsigned fd_limit, int fd, int64_t offset, int whence) {
    int64_t cur_pos = -1;

    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_lseek: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
        fprintf(stderr, "spec_mem_lseek(%p, %u, %d, %" PRId64 ", %d) => EBADF\n", (void *)fds, fd_limit, fd, offset, whence);
	errno = EBADF;
	return -1;
    }

    cur_pos = fds[fd].pos;
    debug(4, "spec_mem_lseek(%p, %u, fd=%d, offset=%" PRId64 ", whence=0x%x): cur pos (%" PRId64 ") => new pos ", (void *)fds, fd_limit, fd, offset, whence, cur_pos);
    if (whence == SEEK_SET)
        fds[fd].pos = offset;
    else if (whence == SEEK_CUR)
        fds[fd].pos += offset;
    else if (whence == SEEK_END)
        fds[fd].pos = fds[fd].len + offset;
    else {
        debug(4, "(%" PRId64 ") (WHENCE WAS BAD): EINVAL\n", fds[fd].pos);
        errno = EINVAL;
        return -1;
    }

    if (fds[fd].pos < 0) {
        debug(4, "(%" PRId64 ") (NEW POS (%" PRId64 ") WAS < 0): EINVAL\n", cur_pos, fds[fd].pos);
        fds[fd].pos = cur_pos; 
        errno = EINVAL;
        return -1;
    }
      
    debug(4, "(%" PRId64 ")\n", fds[fd].pos);
    return fds[fd].pos;
}

int spec_mem_rewind(spec_fd_t *fds, unsigned fd_limit, int fd) {
    debug(4, "spec_rewind(%p, %u, %d) called\n", (void *)fds, fd_limit, fd);
    spec_mem_lseek(fds, fd_limit, fd, 0, SEEK_SET);
    return 0;
}

int spec_mem_reset(spec_fd_t *fds, unsigned fd_limit, int fd) {
    debug(4, "spec_mem_reset(%p, %u, %d) called\n", (void *)fds, fd_limit, fd);
    memset(fds[fd].buf, 0, fds[fd].len);
    fds[fd].pos = fds[fd].len = 0;
    debug(5, "spec_mem_reset: new pos for fd %d@%p is %" PRId64 "\n", fd, (void *)fds, fds[fd].pos);
    return 0;
}

int64_t spec_mem_get_length(spec_fd_t *fds, unsigned fd_limit, int fd) {
    if (fds[fd].open != 1) {
	fprintf(stderr, "spec_get_length(%p, %u, %d): file not open!\n", (void *)fds, fd_limit, fd);
	return -1;
    }
    debug(4, "spec_mem_get_length(%p, %u, %d) => %" PRId64 "\n", (void *)fds, fd_limit, fd, fds[fd].len);
    return fds[fd].len;
}

int spec_mem_fopen(spec_fd_t *fds, unsigned fd_limit, const char *filename, const char *mode) {
    int fd = (int)strtol(filename, (char **)NULL, 10); /* filename == fd */
    if (fd < 0 || (unsigned)fd >= fd_limit) {
        errno = ENOENT;
        return -1;
    }
    debug(4, "spec_mem_fopen(%p, %u, '%s', '%s') = %d\n", (void *)fds, fd_limit, filename, mode, fd);
    fds[fd].open = 1;
    /* We can make some assumptions about this because all instances of use
     * can be easily inspected
     */
    if (mode[0] == 'w') {
        if (mode[1] == '+')
            spec_mem_lseek(fds, fd_limit, fd, fds[fd].pos, SEEK_SET);
        else
            spec_mem_reset(fds, fd_limit, fd);
    } else if (mode[0] == 'r')
        spec_mem_rewind(fds, fd_limit, fd);

    return fd;
}

int spec_mem_open(spec_fd_t *fds, unsigned fd_limit, char *filename, int mode) {
    int fd = (int)strtol(filename, (char **)NULL, 10); /* filename == fd */
    if (fd < 0 || (unsigned)fd >= fd_limit) {
        errno = ENOENT;
        return -1;
    }
    debug(4, "spec_mem_open(%p, %u, '%s', 0x%02x) = %d\n", (void *)fds, fd_limit, filename, mode, fd);
    fds[fd].open = 1;
    if ((mode & O_WRONLY) || (mode & O_RDWR)) {
        if (mode & O_TRUNC)
            spec_mem_reset(fds, fd_limit, fd);
        if (mode & O_APPEND)
            spec_mem_lseek(fds, fd_limit, fd, fds[fd].pos, SEEK_SET);
        else
            spec_mem_rewind(fds, fd_limit, fd);
    } else
        spec_mem_rewind(fds, fd_limit, fd);

    return fd;
}

int64_t spec_mem_write(spec_fd_t *fds, unsigned fd_limit, int fd, unsigned char *buf, int64_t size) {
    debug(5, "spec_mem_write(fds=0x%p, fd_limit=%u, fd=%d, buf=0x%p, size=%" PRId64 ")\n", (void *)fds, fd_limit, fd, (void *)buf, size);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_write: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
        fprintf(stderr, "spec_mem_write(%p, %u, %d, %p, %" PRId64 ") => EBADF\n", (void *)fds, fd_limit, fd, buf, size);
	errno = EBADF;
	return -1;
    }
    while (fds[fd].pos + size > fds[fd].limit) {
        fd_grow(fds, fd, "spec_mem_write");
    }
    memcpy(fds[fd].buf + fds[fd].pos, buf, size); 
    fds[fd].pos += size;
    if (fds[fd].pos > fds[fd].len)
        fds[fd].len = fds[fd].pos;
    debug(5,"  spec_mem_write returns %" PRId64" (len=%" PRId64 " pos=%" PRId64 " limit=%" PRId64 ")\n", size, fds[fd].len, fds[fd].pos, fds[fd].limit);
    return size;
}

int64_t spec_mem_fwrite(spec_fd_t *fds, unsigned fd_limit, unsigned char *buf, int64_t size, int num, int fd) {
    debug(5, "spec_mem_fwrite(fds=0x%p, fd_limit=%u, buf=0x%p, size=%" PRId64 ", num=%d, fd=%d)\n", (void *)fds, fd_limit, (void *)buf, size, num, fd);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_fwrite: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
	fprintf(stderr, "spec_mem_fwrite: fd=%d@%p is not open\n", fd, (void *)fds);
	exit(1);
    }
    while (fds[fd].pos + (size * num) > fds[fd].limit) {
        fd_grow(fds, fd, "spec_mem_fwrite");
    }
    memcpy(fds[fd].buf + fds[fd].pos, buf, size*num); 
    fds[fd].pos += size*num;
    if (fds[fd].pos > fds[fd].len)
        fds[fd].len = fds[fd].pos;
    debug(5,"  spec_mem_fwrite returns %d (len=%" PRId64 " pos=%" PRId64 " limit=%" PRId64 ")\n", num, fds[fd].len, fds[fd].pos, fds[fd].limit);
    return num;
}

int spec_mem_putc(spec_fd_t *fds, unsigned fd_limit, int ch, int fd) {
    debug(4,"spec_putc(fds=0x%p, fd_limit=%u, ch=0x%x, fd=%d) = ", (void *)fds, fd_limit, ch, fd);
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_putc: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
	fprintf(stderr, "spec_mem_putc: fd=%d@%p is not open\n", fd, (void *)fds);
	exit(1);
    }
    fds[fd].buf[fds[fd].pos++] = ch;
    if (fds[fd].pos > fds[fd].len) 
        fds[fd].len = fds[fd].pos;
    debug(4, "%x\n", ch);
    debug(5, "spec_mem_putc: new pos for fd %d@%p is %" PRId64 "\n", fd, (void *)fds, fds[fd].pos);
    return ch;
}

int spec_mem_feof(spec_fd_t *fds, unsigned fd_limit, int fd) {
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_feof: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    debug(4, "spec_mem_feof(fds=0x%p, fd_limit=%u, fd=%d) = %d (len = %" PRId64 "; pos = %" PRId64 "; limit = %" PRId64 ")\n", (void *)fds, fd_limit, fd, fds[fd].pos >= fds[fd].len, fds[fd].len, fds[fd].pos, fds[fd].limit);
    return (fds[fd].pos >= fds[fd].len);
}

int spec_mem_close(spec_fd_t *fds, unsigned fd_limit, int fd) {
    if (fd < 0 || (unsigned)fd >= fd_limit) {
	fprintf(stderr, "spec_mem_close: fd=%d@%p out of range: < 0 || > fd_limit %u!\n", fd, (void *)fds, fd_limit);
	exit(1);
    }
    if (fds[fd].open != 1) {
        fprintf(stderr, "spec_mem_close(%p, %u, %d) => EBADF\n", (void *)fds, fd_limit, fd);
	errno = EBADF;
	return -1;
    }
    fds[fd].open = 0;

    return 0;
}

int spec_empty(int fd) {
    return fd - fd;
}

void spec_initbufs(void) {
#if defined(SPEC_BZIP)
   smallMode               = 0;
   verbosity               = 0;
   blockSize100k           = 9;
/*   bsStream                = 0; */
   workFactor              = 30;
/*   allocateCompressStructures(); */
#endif
}

void spec_compress(int in, int out, unsigned int lev) {
    /* Mark the files as open */
    int in_open = spec_fd[in].open;
    int out_open = spec_fd[out].open;
    spec_fd[in].open = spec_fd[out].open = 1;
#if defined(SPEC_BZIP)
    blockSize100k           = lev;
    compressStream ( in, out );
#elif defined(SPEC_XZ)
    compressStream ( in, out, lev );
#else
#   error no compressStream implementation
#endif
    /* Mark the files as closed (maybe) */
    spec_fd[in].open = in_open;
    spec_fd[out].open = out_open;
}

void spec_uncompress(int in, int out) {
    /* Mark the files as open */
    int in_open = spec_fd[in].open;
    int out_open = spec_fd[out].open;
    spec_fd[in].open = spec_fd[out].open = 1;
#if defined(SPEC_BZIP)
    blockSize100k           = 0;
#endif
    uncompressStream( in, out );
    /* Mark the files as closed (maybe) */
    spec_fd[in].open = in_open;
    spec_fd[out].open = out_open;
}

int debug_time(void) {
# if defined(TIMING_OUTPUT)
    static int last = 0;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    debug(2, "Time: %10d, %10d\n", tv.tv_sec, tv.tv_sec-last);
    last = tv.tv_sec;
# endif
    return 0;
}

void print_sum ( FILE *out, unsigned char *sum, int bits )
{
  for(int i = 0; i < bits/8; i++) {
    fprintf(out, "%02x", sum[i]);
  }
}

static void fd_grow( spec_fd_t *fds, int fd, const char *funcname )
{
    SPEC_MEMOP_PAUSE_SETUP

    fds[fd].limit *= 2;
    SPEC_PREALLOC_PAUSE("fd_grow", "realloc", fds[fd].limit);
    fds[fd].buf = realloc(fds[fd].buf, fds[fd].limit);
    if (fds[fd].buf == NULL)
    {
	fprintf(stderr, "%s: Error growing buffer for fd %d@%p: %s\n", funcname, fd, (void *)fds, strerror(errno));
	exit(1);
    }
    SPEC_POSTALLOC_PAUSE("fd_grow", "realloc", fds[fd].limit);
    debug(5, "  grew buffer for fd%d@0x%p to %" PRId64 "\n", fd, (void *)fds, fds[fd].limit);
}

static unsigned char hex_char_to_num ( char digit )
{
    if (digit - '0' > 9)
      return tolower(digit) - 'a' + 10;
    else
      return digit - '0';
}

unsigned char *sum_str_to_hex ( char *sum, int bits )
{
    int i = 0;
    unsigned char *gen_sum;
    SPEC_MEMOP_PAUSE_SETUP

    SPEC_PREALLOC_PAUSE("sum_str_to_hex", "malloc", sizeof(unsigned char) * bits / 8);
    gen_sum = (unsigned char *)malloc(sizeof(unsigned char) * bits / 8);
    if (gen_sum == NULL) {
        fprintf(stderr, "sum_str_to_hex: couldn't allocate %d chars for hash\n", bits / 8);
        exit(1);
    }
    SPEC_POSTALLOC_PAUSE("sum_str_to_hex", "malloc", sizeof(unsigned char) * bits / 8);
    for(i = 0; i < bits / 8; i++) {
        gen_sum[i] = (hex_char_to_num(sum[i*2]) << 4) | hex_char_to_num(sum[i*2 + 1]);
    }

    return gen_sum;
}

int compare_sum ( unsigned char *ref_sum, unsigned char *generated, int bits )
{
    if (ref_sum == NULL || generated == NULL)
      return -1;

    for(int i = 0; i < bits / 8; i++) {
        if (ref_sum[i] != generated[i]) {
            fprintf(stderr, "Hash mismatch at %d: '%02x' (expected) != '%02x' (generated)\n", i, ref_sum[i], generated[i]);
            return( (ref_sum[i] < generated[i]) ? -1 : 1 );
        }
    }

    return 0;
}

spec_fd_t *spec_mem_alloc_fds (unsigned fd_limit) {
    spec_fd_t *fds;
    SPEC_MEMOP_PAUSE_SETUP

    debug(3, "spec_mem_alloc_fds(%s)\n", fd_limit == MAX_SPEC_FD ? "MAX_SPEC_FD" : "<variable>");
    SPEC_PREALLOC_PAUSE("spec_mem_alloc", "malloc", sizeof(spec_fd_t) * (fd_limit + 1));
    fds = (spec_fd_t *)malloc(sizeof(spec_fd_t) * (fd_limit + 1));
    if (fds == NULL) {
        fprintf(stderr, "Error mallocing space for list of spec_fd_t structs\n");
        return NULL;
    }
    SPEC_POSTALLOC_PAUSE("spec_mem_alloc", "malloc", sizeof(spec_fd_t) * (fd_limit + 1));
    memset(fds, 0, sizeof(spec_fd_t) * (fd_limit + 1));
    debug(4, "spec_mem_alloc_fds(%u) => %p\n", fd_limit, (void *)fds);

    return fds;
}

void spec_mem_free_fds (spec_fd_t *fds, unsigned fd_limit) {
    unsigned i;
    SPEC_MEMOP_PAUSE_SETUP

    debug(3, "spec_mem_free_fds(%s)\n", fds == spec_fd ? "spec_fd" : "<variable>");
    debug(4, "  +-> fds=%p fd_limit=%u\n", (void *)fds, fd_limit);
    if (fds != NULL)
        for(i = 0; i < fd_limit; i++) {
            if (fds[i].buf != NULL) {
                SPEC_PREFREE_PAUSE("spec_mem_free_fds", "free", fds[i].buf);
                free(fds[i].buf);
                SPEC_POSTFREE_PAUSE("spec_mem_free_fds", "free", fds[i].buf);
                fds[i].buf = NULL;
            }
        }

    SPEC_PREFREE_PAUSE("spec_mem_free_fds", "free", fds);
    free(fds);
    SPEC_POSTFREE_PAUSE("spec_mem_free_fds", "free", fds);
}


#if defined(__cplusplus)
}
#endif
