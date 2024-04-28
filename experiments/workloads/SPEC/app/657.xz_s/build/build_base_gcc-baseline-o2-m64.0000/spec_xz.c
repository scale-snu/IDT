/* xz utility functions for SPEC CPU
 *
 * $Id: spec_xz.c 5024 2016-11-17 21:57:46Z CloyceS $
 */
#if !defined(SPEC_NO_STDBOOL_H)
# include <stdbool.h>
#elif !defined(__bool_true_false_are_defined)
# if ! HAVE__BOOL
typedef unsigned char _Bool;
# endif
# if defined(SPEC)
#  undef bool
#  undef true
#  undef false
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif
#include "spec_mem_io.h"
#include "lzma.h"

static bool init_decoder(lzma_stream *strm);
bool init_encoder(lzma_stream *strm, lzma_options_lzma *options, uint32_t preset, lzma_check check);

static bool init_decoder(lzma_stream *strm) {
    const char *msg;
    lzma_ret rc = lzma_stream_decoder(strm, UINT64_MAX, LZMA_CONCATENATED | LZMA_TELL_UNSUPPORTED_CHECK);
    switch(rc) {
        case LZMA_OK:
            return true;
        case LZMA_UNSUPPORTED_CHECK:
            printf("lzma_stream_decoder reports an unsupported check method: ");
            switch(lzma_get_check(strm)) {
                case LZMA_CHECK_NONE:
                    printf("NONE\n"); // Won't happen - none is always supported
                    break;
                case LZMA_CHECK_CRC32:
                    printf("CRC32\n"); // Won't happen - crc32 is always supported
                    break;
                case LZMA_CHECK_CRC64:
                    printf("CRC64\n");
                    break;
                case LZMA_CHECK_SHA256:
                    printf("SHA256\n");
                    break;
                default:
                    printf("unknown!  This shouldn't happen.\n");
                    break;
            }
            // But it's still okay
            return true;
        case LZMA_MEM_ERROR:
            msg = "Memory allocation failed";
            break;
        case LZMA_OPTIONS_ERROR:
            msg = "Unsupported decompression flags";
            break;
        default:
            msg = "Unknown return code";
            break;
    }
    printf("Error initializing decoder: %s (code %u)\n", msg, rc);
    return false;
}

bool init_encoder(lzma_stream *strm, lzma_options_lzma *options, uint32_t preset, lzma_check check) {
    const char *msg;
    lzma_filter filters[LZMA_FILTERS_MAX + 1];
    lzma_options_lzma lzma_local_options;
    if (options == NULL)
        options = &lzma_local_options;

    if ((unsigned int)check > LZMA_CHECK_ID_MAX)
        check = LZMA_CHECK_CRC64;       // XZ program default

    lzma_lzma_preset(options, preset);
    filters[0].id = LZMA_FILTER_LZMA2;
    filters[0].options = options;
    filters[1].id = LZMA_VLI_UNKNOWN;

    lzma_ret rc = lzma_stream_encoder(strm, filters, check);

    switch(rc) {
        case LZMA_OK:
            return true;
        case LZMA_UNSUPPORTED_CHECK:
            switch(lzma_get_check(strm)) {
                case LZMA_CHECK_NONE:
                    // Won't happen - none is always supported
                    msg = "unsupported check method NONE\n";
                    break;
                case LZMA_CHECK_CRC32:
                    // Won't happen - CRC32 is always supported
                    msg = "unsupported check method CRC32\n";
                    break;
                case LZMA_CHECK_CRC64:
                    msg = "unsupported check method CRC64\n";
                    break;
                case LZMA_CHECK_SHA256:
                    msg = "unsupported check method SHA256\n";
                    break;
                default:
                    msg = "unknown check method!  This shouldn't happen.\n";
                    break;
            }
        case LZMA_MEM_ERROR:
            msg = "Memory allocation failed";
            break;
        case LZMA_OPTIONS_ERROR:
            msg = "Specified preset is not supported";
            break;
        default:
            msg = "Unknown error";
            break;
    }
    printf("Error initializing encoder with preset=%u check=%u: %s (code %u)\n", preset, (unsigned int)check, msg, rc);
    return false;
}

void uncompressStream ( int zStream, int stream ) {
    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_action action = LZMA_RUN;

    uint8_t in[BUFSIZ];
    uint8_t out[BUFSIZ];

    if (!init_decoder(&strm)) {
        // Error message has already been printed, and the state of the
        // "files" will show that something wasn't right, so...
        return;
    }

    // Do decompression; adapted from decompress() in the 02_decompress.c
    // example file
    strm.next_in = NULL;
    strm.avail_in = 0;
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    while(true) {
        if (strm.avail_in == 0 && !feof(zStream)) {
            strm.next_in = in;
            strm.avail_in = fread(in, 1, sizeof(in), zStream);

            if (feof(zStream)) {
                // End of input; tell the decoder
                action = LZMA_FINISH;
            }
        }

        lzma_ret rc = lzma_code(&strm, action);

        if (strm.avail_out == 0 || rc == LZMA_STREAM_END) {
            size_t write_size = sizeof(out) - strm.avail_out;
            if ((size_t)fwrite(out, 1, write_size, stream) != write_size) {
                printf("Decompress write error on %d: %s\n", stream, strerror(errno));
                return;
            }

            strm.next_out = out;
            strm.avail_out = sizeof(out);
        }

        if (rc != LZMA_OK) {
            const char *msg;
            switch(rc) {
                case LZMA_STREAM_END:
                    // Done!
                    lzma_end(&strm);
                    return;
                case LZMA_MEM_ERROR:
                    msg = "Memory allocation failed";
                    break;
                case LZMA_FORMAT_ERROR:
                    msg = "Input not in XZ format";
                    break;
                case LZMA_OPTIONS_ERROR:
                    msg = "Unsupported compression options";
                    break;
                case LZMA_DATA_ERROR:
                    msg = "Compressed data is corrupt";
                    break;
                case LZMA_BUF_ERROR:
                    msg = "Compressed data is truncated or otherwise corrupt";
                    break;
                default:
                    // Most likely LZMA_PROG_ERROR, it says
                    msg = "Unknown return code";
                    break;
            }
            printf("Decompression decoder error: %s (code %u)\n", msg, rc);
            return;
        }
    }

    // Free up memory
    lzma_end(&strm);


    return;
}

#if !(defined(_OPENMP) || defined(SPEC_OPENMP)) || defined(SPEC_SUPPRESS_OPENMP) || defined(SPEC_AUTO_SUPPRESS_OPENMP)
void compressStream ( int stream, int zStream, uint32_t preset ) {
    lzma_stream strm = LZMA_STREAM_INIT;
    lzma_action action = LZMA_RUN;

    uint8_t in[BUFSIZ];
    uint8_t out[BUFSIZ];

    // Figure out which check method to use based on whether the extreme
    // encoding bit is set.  CRC64 is default for the command line tool,
    // but let's check SHA256 as well.
    lzma_check check = (preset & LZMA_PRESET_EXTREME) ? LZMA_CHECK_SHA256 : LZMA_CHECK_CRC64;

    if (!init_encoder(&strm, NULL, preset, check)) {
        // Error message has already been printed, and the state of the
        // "files" will show that something wasn't right, so...
        return;
    }

    // Do compression; adapted from compress() in the 01_compress_easy.c
    // example file
    strm.next_in = NULL;
    strm.avail_in = 0;
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    while(true) {
        if (strm.avail_in == 0 && !feof(stream)) {
            strm.next_in = in;
            strm.avail_in = fread(in, 1, sizeof(in), stream);

            if (feof(stream)) {
                // End of input; tell the decoder
                action = LZMA_FINISH;
            }
        }

        lzma_ret rc = lzma_code(&strm, action);

        if (strm.avail_out == 0 || rc == LZMA_STREAM_END) {
            size_t write_size = sizeof(out) - strm.avail_out;
            if ((size_t)fwrite(out, 1, write_size, zStream) != write_size) {
                printf("Compress write error on %d: %s\n", zStream, strerror(errno));
                return;
            }

            strm.next_out = out;
            strm.avail_out = sizeof(out);
        }

        if (rc != LZMA_OK) {
            const char *msg;
            switch(rc) {
                case LZMA_STREAM_END:
                    // Done!
                    lzma_end(&strm);
                    return;
                case LZMA_MEM_ERROR:
                    msg = "Memory allocation failed";
                    break;
                case LZMA_DATA_ERROR:
                    msg = "File size limits exceeded";
                    break;
                default:
                    // Most likely LZMA_PROG_ERROR, it says
                    msg = "Unknown return code";
                    break;
            }
            printf("Compression encoder error: %s (code %u)\n", msg, rc);
            return;
        }
    }

    // Free up memory
    lzma_end(&strm);


    return;
}
#endif // SPEC_SUPPRESS_OPENMP, !SPEC_OPENMP
