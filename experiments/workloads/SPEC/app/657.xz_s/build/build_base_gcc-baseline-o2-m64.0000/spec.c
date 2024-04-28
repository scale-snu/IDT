/* xz driver code for SPEC CPU
 *
 * $Id: spec.c 5572 2017-04-10 22:49:59Z JohnHenning $
 */

#ifdef __cplusplus
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
#include <stdlib.h>
#include "spec_mem_io.h"
#include "sha512.h"
#include "lzma.h"

#define SHASIZE 512

int main (int argc, char *argv[]) {
    int level = 0, i = 0;
    unsigned extreme = 0;
    int64_t input_size = 0;
    int64_t compressed_min = -1,
            compressed_max = -1;
    char *input_name = NULL;
    unsigned char *sha = NULL;
    unsigned char *input_sha = NULL, *check_sha = NULL;
# if defined(DEBUG_DUMP)
    int fd = -1;
# endif
    SPEC_MEMOP_PAUSE_SETUP

    SPEC_PREALLOC_PAUSE("main", "malloc", sizeof(unsigned char) * SHASIZE/8);
    check_sha = (unsigned char *)malloc(sizeof(unsigned char) * SHASIZE/8);
    if (check_sha == NULL) {
        printf ("main: Error mallocing memory for SHA-%d sum!\n", SHASIZE);
        exit(1);
    }
    SPEC_POSTALLOC_PAUSE("main", "malloc", sizeof(unsigned char) * SHASIZE/8);

    if (argc > 1) input_name      = argv[1];
    if (argc > 2) input_size      = strtol(argv[2], NULL, 10);
    if (argc > 3) {
      if (strlen(argv[3]) * 4 != SHASIZE) {
        fprintf(stderr, "Error: Supplied original hash value is not the correct length to be SHA-%d\n", SHASIZE);
        exit(1);
      } else
                  sha             = sum_str_to_hex(argv[3], strlen(argv[3]) * 4);
    }
    if (argc > 4) compressed_min  = strtol(argv[4], NULL, 10);
    if (argc > 5) compressed_max  = strtol(argv[5], NULL, 10);
    if (compressed_max < 0)
      compressed_max = compressed_min;

    if (input_name == NULL || input_size <= 0 || sha == NULL || compressed_min == 0 || compressed_max == 0 || argc < 6) {
        printf("Usage: %s <input filename> <input size> <input raw SHA-%d> <compressed minimum> <compressed maximum> <compression level, ...>\n", argv[0], SHASIZE);
        exit(1);
    }
    printf("SPEC CPU XZ driver: input=%s insize=%" PRId64 "\n", input_name, input_size);

    spec_fd = spec_mem_alloc_fds(MAX_SPEC_FD);
    if (spec_fd == NULL) {
        fprintf(stderr, "Error allocating in-memory file descriptors\n");
        exit(1);
    }

    // Initial buffer size is <input_size> MiB or FILE_CHUNK, whichever is
    // larger (will be grown if necessary, but has to be able to hold at
    // least one read's worth of data)
    if (input_size * MiB < FILE_CHUNK) {
        spec_fd[0].limit = spec_fd[1].limit = FILE_CHUNK;
    } else {
        spec_fd[0].limit = spec_fd[1].limit = input_size * MiB;
    }
    spec_init();

    debug_time();
    printf("Loading Input Data\n");
    input_sha = spec_load(0, input_name, input_size * MiB, 1, sha, SHASIZE);
    printf("Input data %" PRId64 " bytes in length\n", spec_fd[0].len);

#if defined(DEBUG_DUMP)
    fd = open ("out.uncompressed", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    write(fd, spec_fd[0].buf, spec_fd[0].len);
    close(fd);
#endif

    /* Mark the input files as open, as open() is never actually called */
    spec_fd[0].open = spec_fd[1].open = spec_fd[2].open = 1;

    spec_initbufs();

    // Loop through requested compression levels
    for(i = 6; i < argc; i++) {

        level = argv[i][0] - '0';
        if (argv[i][1] == 'e')
            extreme = LZMA_PRESET_EXTREME;
        else
            extreme = 0;

        if (level < 0 || level > 9) {
            printf("Error: compression level '%s' is not valid\n", argv[i]);
            printf("Usage: %s <input filename> <input size> <input raw SHA-%d> <compressed minimum> <compressed maximum> <compression level, ...>\n", argv[0], SHASIZE);
            exit(1);
        }

        debug_time();
        printf("Compressing Input Data, level %d%s\n", level, extreme ? " (extreme)" : "");

        spec_compress(0,1, level | extreme);

        debug_time();
        fprintf(stderr, "Compressed data %" PRId64 " bytes in length\n", spec_fd[1].len);
        if (compressed_min > 0) {
          if (spec_fd[1].len >= compressed_min && spec_fd[1].len <= compressed_max) {
            if (compressed_min == compressed_max)
              printf("Compressed data length is %" PRId64 " bytes in length\n",
                    compressed_min);
            else
              printf("Compressed data is between %" PRId64 " and %" PRId64 " bytes in length\n",
                    compressed_min, compressed_max);
          } else {
            if (compressed_min == compressed_max)
              printf("Compressed data length of %" PRId64 " does not match expected length of %" PRId64 " bytes\n",
                  spec_fd[1].len, compressed_min);
            else
              printf("Compressed data length of %" PRId64 " is outside the allowable range of %" PRId64 " to %" PRId64 " bytes\n",
                  spec_fd[1].len, compressed_min, compressed_max);
          }
        }
        printf("Finished compressing data\n");

#if defined(DEBUG_DUMP)
        {
            char buf[256];
            sprintf(buf, "out.uncompress.%d%s", level, extreme ? "e" : "");
            fd = open (buf, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            write(fd, spec_fd[1].buf, spec_fd[1].len);
            close(fd);
        }
#endif

        spec_reset(0);
        spec_rewind(1);

        debug_time();
        printf("Uncompressing previously compressed data\n");
        spec_uncompress(1, 0);
        debug_time();
        printf("Uncompressed data %" PRId64 " bytes in length\n", spec_fd[0].len);

#if defined(DEBUG_DUMP)
        {
            char buf[256];
            sprintf(buf, "out.uncompress.%d%s", level, extreme ? "e" : "");
            fd = open (buf, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            write(fd, spec_fd[0].buf, spec_fd[0].len);
            close(fd);
        }
#endif

        spec_sum(0, check_sha, SHASIZE);
        if (compare_sum(input_sha, check_sha, SHASIZE)) {
            printf("Computed SHA-%d sum for decompression step did not match expected\n", SHASIZE);
            printf("  Generated: ");
            print_sum(stdout, check_sha, SHASIZE);
            printf("\n");
            printf("   Expected: ");
            print_sum(stdout, input_sha, SHASIZE);
            printf("\n");
            exit (0);
        }
        debug_time();
        printf("Uncompressed data compared correctly\n");
        spec_reset(1);
        spec_rewind(0);
        printf("Tested %" PRId64 " MiB buffer: OK!\n", input_size);
        fflush(stdout);
    }

    return 0;
}

#if defined(__cplusplus)
}
#endif
