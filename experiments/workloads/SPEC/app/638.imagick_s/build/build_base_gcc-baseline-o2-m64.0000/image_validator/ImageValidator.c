/* Calculate the structural similarity (SSIM) index between two images.
 * https://en.wikipedia.org/wiki/Structural_similarity
 *
 * Copyright 2015 Standard Performance Evaluation Corporation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#if defined(SPEC_WINDOWS)
#define STRCMP(x,y) _stricmp((x), (y))
#else
#define STRCMP(x,y) strcmp((x), (y))
#endif

static const char *svnrev = "$LastChangedRevision: 4696 $";

typedef struct {
    unsigned char y;
    unsigned char u;
    unsigned char v;
} YUV;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char attr;
} Pixel;

typedef struct {
    unsigned char  idlength;
    unsigned char  colormaptype;
    /* Targa data type codes: (3 bits; 4th bit is RLE, which we don't do)
        0 no image data is present
        1 uncompressed color-mapped image
        2 uncompressed true-color image
        3 uncompressed black-and-white (grayscale) image
    */
    enum { NODATA = 0, COLORMAP, TRUECOLOR, GRAYSCALE } datatypecode;
    unsigned short colormaporigin;
    unsigned short colormaplength;
    unsigned char  colormapdepth;
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char  bitsperpixel;
    unsigned char  imagedescriptor;
    unsigned char  image_id[256];
    Pixel         *colormapdata;
    Pixel        **imagedata;
    unsigned char  alphabits;
} TargaImage;

typedef struct {
    /* Input files */
    char         *file1;
    TargaImage   *img1;
    char         *file2;
    TargaImage   *img2;
    /* Pathname of input files with file extension removed */
    char         *base1;
    char         *base2;
    /* Output files */
    char         *diff;
    TargaImage   *imgdiff;
    TargaImage   *img1luma;
    TargaImage   *img2luma;
    /* Flags for boolean options (all default to off) */
    unsigned int  dumpfile     : 1;
    unsigned int  dumpheader   : 1;
    unsigned int  dumpdiff     : 1;
    unsigned int  dumpluma     : 1;
    unsigned int  avg_only     : 1;
    unsigned int  printbuckets : 1;
    unsigned int  debug        : 3;
    enum { REC709, REC601, HSP } conversion;
    double        threshold;
    unsigned int  maxthresh;
    double        maxthreshpct;
    unsigned int  numbuckets;
    unsigned int *buckets;
    unsigned int  width;
    unsigned int  height;
} params;

#define check_malloc(var, type, size)                                   \
    var = (type) malloc( size );                                        \
    if (var == NULL) {                                                  \
        fprintf(stderr, "Failed to allocate %lu bytes at %s:%d\n",      \
                        (unsigned long)(size), __FILE__, __LINE__);     \
        exit(1);                                                        \
    }

#define basename(var, fname)                                            \
    var = (strrchr(fname, '/') == 0) ? strrchr(fname, '\\') : strrchr(fname, '/'); \
    var = (var == 0) ? fname : var + 1;

TargaImage *init_targa(TargaImage *ref, params param);
char       *load_image_file(const char *fname, TargaImage *img, params param);
void        read_tga_file (const char *fname, TargaImage *tga);
void        read_yuv_file(const char *fname, TargaImage *tga, params param);
void        dump_header(const char *fname, TargaImage *tga);
void        dump_tga(const char *fname, TargaImage *tga);
int         computeSSIM(params param);
double      mean(double *values, int n);
double      variance(double *values, double mean, int n);
double      covariance(double *values1, double *values2, double mean1, double mean2, int n);
char       *spec_strdup(const char *str);
char       *get_version(void);
void        printUsage(char *progname);

int main(int argc, char *argv[]) {

    char *fname;
    unsigned int i;
    params param;
    char *ver = get_version();
    fprintf(stderr, "ImageValidator %s\n", ver);
    free(ver);

    /* Initialize the program parameters */
    memset(&param, 0, sizeof(param));
    param.threshold    = 0.0;
    param.maxthreshpct = 0.0;
    param.numbuckets   = 20;
    param.width        = 1280;
    param.height       = 720;
    param.conversion   = REC709;
    int bad = 0;
    int usage = 0;
    for(int arg = 1; arg < argc; arg++) {
            if (!STRCMP("-debug", argv[arg])) {
                if (param.debug < 8) {
                    ++param.debug;
                }
        } else
            if (!STRCMP("-dumpfile", argv[arg])) {
                param.dumpfile = 1;
        } else
            if (!STRCMP("-dumpheader", argv[arg])) {
                param.dumpheader = 1;
        } else
            if (!STRCMP("-dumpluma", argv[arg])) {
                param.dumpluma = 1;
        } else
            if (!STRCMP("-avg", argv[arg])) {
                param.avg_only = 1;
        } else
            if (!STRCMP("-width", argv[arg])) {
                ++arg;
                param.width = (unsigned)strtol(argv[arg], NULL, 10);
        } else
            if (!STRCMP("-height", argv[arg])) {
                ++arg;
                param.height = (unsigned)strtol(argv[arg], NULL, 10);
        } else
            if (!STRCMP("-buckets", argv[arg])) {
                ++arg;
                param.numbuckets = strtol(argv[arg], NULL, 10);
                param.printbuckets = 1;
        } else
            if (!STRCMP("-maxthreshold", argv[arg])) {
                ++arg;
                if (strchr(argv[arg], '%') != NULL)
                    param.maxthreshpct = strtod(argv[arg], NULL) / 100.0;
                else if (strchr(argv[arg], '.') != NULL)
                    param.maxthreshpct = strtod(argv[arg], NULL);
                else
                    param.maxthresh = (int)strtol(argv[arg], NULL, 10);
                fprintf(stderr, "INFO: max threshold set to %d absolute, %f relative\n",
                        param.maxthresh, param.maxthreshpct);
        } else
            if (!STRCMP("-diff", argv[arg])) {
                param.dumpdiff = 1;
                ++arg;
                param.diff = spec_strdup(argv[arg]);
        } else
            if (!STRCMP("-threshold", argv[arg])) {
                ++arg;
                param.threshold = (float)strtod(argv[arg], NULL);
                fprintf(stderr, "INFO: threshold set to %f\n", param.threshold);
        } else
            if (!STRCMP("-conv", argv[arg])) {
                ++arg;
                if (!STRCMP("709", argv[arg]) || !STRCMP("rec709", argv[arg])
                        || !STRCMP("REC709", argv[arg])) {
                    param.conversion = REC709;
                    fprintf(stderr, "INFO: ITU-R Rec. 709 (HDTV) luma conversion selected\n");
                } else
                if (!STRCMP("601", argv[arg]) || !STRCMP("rec601", argv[arg])
                        || !STRCMP("REC601", argv[arg])) {
                    param.conversion = REC601;
                    fprintf(stderr, "INFO: ITU-R Rec. 601 (HDTV) conversion selected\n");
                } else
                if (!STRCMP("HSP", argv[arg]) || !STRCMP("HSP299", argv[arg])
                    || !STRCMP("hsp", argv[arg]) || !STRCMP("hsp299", argv[arg])) {
                    param.conversion = HSP;
                    fprintf(stderr, "INFO: HSP299 (http://alienryderflex.com/hsp.html) conversion selected\n");
                } else {
                    fprintf(stderr, "\nERROR: Unrecognized conversion formula '%s'\n",
                            argv[arg]);
                    bad = 1;
                }
        } else
            if (!STRCMP("-help", argv[arg]) || !STRCMP("-h", argv[arg])) {
                usage = 1;
        } else {
            if (param.file1 == NULL) {
                param.file1 = spec_strdup(argv[arg]);
            } else if (param.file2 == NULL) {
                param.file2 = spec_strdup(argv[arg]);
            } else if (*(argv[arg]) != '-') {
                fprintf(stderr, "\nERROR: Unrecognized argument '%s'\n", argv[arg]);
                bad = 1;
            } else {
                fprintf(stderr, "\nERROR: More than two files given.\n");
                bad = 1;
            }
        }
    }

    if (usage) {
        printUsage(argv[0]);
        exit(0);
    }

    /* Validate arguments */
    if (param.file1 == NULL || param.file2 == NULL) {
        fprintf(stderr, "\n\nERROR: Missing input file name.\n");
        bad = 1;
    }
    if (param.dumpdiff == 1 && param.diff == NULL) {
        fprintf(stderr, "\n\nERROR: Missing diff output file name.\n");
        bad = 1;
    }
    if (param.threshold < 0.0 || param.threshold > 1.0) {
        fprintf(stderr, "\n\nERROR: threshold must be between 0.0 and 1.0\n");
        bad = 1;
    }
    if (param.maxthreshpct >= 1) {
        fprintf(stderr, "\n\nERROR: maximum threshold percentage must be less than 100%%\n");
        bad = 1;
    }
    if (param.numbuckets < 2 || param.numbuckets > 100 || param.numbuckets % 2 != 0) {
        printf("\n\nERROR: The number of buckets must be an even number between 2 and 100.\n");
        bad = 1;
    }
    if (bad) {
        printUsage(argv[0]);
        exit(-1);
    }

    if ( param.numbuckets > 0) {
        check_malloc( param.buckets, unsigned int *, sizeof(unsigned int) * (param.numbuckets + 1));
        for (i=0; i <= param.numbuckets; ++i) {
            param.buckets[i] = 0;
        }
    }

    check_malloc( param.img1, TargaImage *, sizeof(TargaImage) );
    memset(param.img1, 0, sizeof(TargaImage));
    check_malloc( param.img2, TargaImage *, sizeof(TargaImage) );
    memset(param.img2, 0, sizeof(TargaImage));

    char *tmpbase = load_image_file(param.file1, param.img1, param);
    basename(param.base1, tmpbase);
    tmpbase = load_image_file(param.file2, param.img2, param);
    basename(param.base2, tmpbase);
    if (!STRCMP(param.base1, param.base2)) {
        /* Base names are the same, so tack on a little to make sure that
         * there aren't clashes when we use them to make new filenames
         * later.
         * This is safe because spec_strdup() adds an extra 10 bytes
         * to each allocation.
         */
        strcat(param.base1, ".1");
        strcat(param.base2, ".2");
    }

    if (param.dumpluma || param.dumpdiff) {
        param.img1luma = init_targa(param.img1, param);
        /* Reset things that are not the same */
        param.img1luma->colormaptype     = 0;
        param.img1luma->datatypecode     = GRAYSCALE;
        param.img1luma->bitsperpixel     = 8;
        /* Greyscale images have no alpha, so 3:0 of imagedescriptor are 0 */
        param.img1luma->imagedescriptor &= 0xF0;
        param.img1luma->alphabits        = 0;
    }
    if (param.dumpluma) {
        param.img2luma = init_targa(param.img2, param);
        /* Reset things that are not the same */
        param.img2luma->colormaptype     = 0;
        param.img2luma->datatypecode     = GRAYSCALE;
        param.img2luma->bitsperpixel     = 8;
        /* Greyscale images have no alpha, so 3:0 of imagedescriptor are 0 */
        param.img2luma->imagedescriptor &= 0xF0;
        param.img2luma->alphabits        = 0;
    }

    if (param.dumpdiff) {
        param.imgdiff = init_targa(param.img1, param);
        /* Reset things that may not be the same */
        param.imgdiff->colormaptype    = 0;
        param.imgdiff->datatypecode    = TRUECOLOR;
        param.imgdiff->bitsperpixel    = 24;
        param.imgdiff->imagedescriptor = 0;
        param.imgdiff->alphabits       = 0;
    }

    if (param.dumpfile || param.dumpluma) {
        int len = strlen(param.base1) > strlen(param.base2) ? strlen(param.base1) : strlen(param.base2);
        check_malloc( fname, char *, (sizeof(char) * (len + 11)) );

        if (param.dumpfile) {
            // sprintf() is safe because we know fname is big enough to hold it all
            sprintf(fname, "%s.new.tga", param.base1);
            dump_tga(fname, param.img1);

            sprintf(fname, "%s.new.tga", param.base2);
            dump_tga(fname, param.img2);
        }

        if (param.dumpluma) {
            sprintf(fname, "%s.luma.tga", param.base1);
            dump_tga(fname, param.img1luma);

            sprintf(fname, "%s.luma.tga", param.base2);
            dump_tga(fname, param.img2luma);
        }

        free(fname);
    }

    if (param.dumpheader) {
        dump_header(param.file1, param.img1);
        dump_header(param.file2, param.img2);
    }

    int rc = computeSSIM(param);

    if (param.dumpdiff) {
        dump_tga(param.diff, param.imgdiff);
    }
    exit(rc);
}

int computeSSIM(params param) {
    TargaImage *img1     = param.img1;
    TargaImage *img1luma = param.img1luma;
    TargaImage *img2     = param.img2;
    TargaImage *img2luma = param.img2luma;
    TargaImage *diff     = param.imgdiff;

    unsigned int height   = img1->height;
    unsigned int width    = img1->width;
    if (width != img2->width || height != img2->height) {
        fprintf(stderr, "ERROR: Image sizes don't match (%ux%u vs %ux%u)\n",
                width, height,
                img2->width, img2->height);
        return 0; /* Not -1; This is a VE, not an RE */
    }

    /* The ACTUAL number of bits per pixel isn't necessarily what's in the
     * bitsperpixel field; Targa-16 images have 15 bits of color data per
     * pixel, and Targa-32 images have 24.
     */
    int bpp = (img1->bitsperpixel == 16) ? 15 : (img1->bitsperpixel == 32) ? 24 : img1->bitsperpixel;

    /* For images, SSIM is applied only to luma, and our luma calculations
     * result in values [0,255], so there's effectively 8 bits of dynamic
     * range, regardless of how many bits per pixel the original images have.
     */
    /* c = (k * L)^2 */
    double c1 = 6.5025;         /* k1 = 0.01 */
    double c2 = 58.5255;        /* k2 = 0.03 */

    double       sum      = 0.0;
    double       minssim  = 1.0;
    unsigned int cnt      = 0;
    unsigned int numbelow = 0;

    /* Compute the SSIM of each 8x8 pixel area for this image against the
     * ref image.
     */
    double *i1_lumas, *i2_lumas;
    check_malloc( i1_lumas, double *, sizeof(double) * 8 * 8 );
    check_malloc( i2_lumas, double *, sizeof(double) * 8 * 8 );
    for (unsigned int i = 0; i < height; i += 8 ) {
        for (unsigned int j = 0; j < width; j += 8 ) {
            unsigned int nx = ((i + 8) > height) ? (height - i) : 8;
            unsigned int ny = ((j + 8) > width) ? (width - j) : 8;
            unsigned int n = 0;

            /* The RGBtoLuma macro will compute luma (brightness) value for
             * each pixel using the ITU-R Rec. 601 standard-def digital TV
             * formula.
             * http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC11
             */
#define RGBto601Luma(pixel) (  (0.299 * (pixel).red)   \
                             + (0.587 * (pixel).green) \
                             + (0.114 * (pixel).blue)  \
                            )

            /* The RGBtoLuma macro will compute luma (brightness) value for
             * each pixel using the ITU-R Rec. 709 high-def digital TV
             * formula.
             * http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC9
             */
#define RGBto709Luma(pixel) (  (0.2126 * (pixel).red)   \
                             + (0.7152 * (pixel).green) \
                             + (0.0722 * (pixel).blue)  \
                            )

            /* The RGBtoP macro compute brightness value for each pixel using
             * the "HSP299" formula from
             * http://alienryderflex.com/hsp.html
             */
#define RGBtoP(pixel)   sqrt( \
                             (0.299 * (pixel).red   * (pixel).red)   \
                           + (0.587 * (pixel).green * (pixel).green) \
                           + (0.114 * (pixel).blue  * (pixel).blue)  \
                        )

            for (unsigned int ix = 0; ix < nx; ix++ ) {
                for (unsigned int jx = 0; jx < ny; jx++ ) {
                    if (bpp == 8) {
                        /* The scale of the luma value is [0,255], so just use the
                         * red (gray) channel directly.
                         */
                        i1_lumas[n] = img1->imagedata[i + ix][j + jx].red;
                        i2_lumas[n] = img2->imagedata[i + ix][j + jx].red;
                    } else if (param.conversion == REC709) {
                        i1_lumas[n] = RGBto709Luma(img1->imagedata[i + ix][j + jx]);
                        i2_lumas[n] = RGBto709Luma(img2->imagedata[i + ix][j + jx]);
                    } else if (param.conversion == REC601) {
                        i1_lumas[n] = RGBto601Luma(img1->imagedata[i + ix][j + jx]);
                        i2_lumas[n] = RGBto601Luma(img2->imagedata[i + ix][j + jx]);
                    } else if (param.conversion == HSP) {
                        i1_lumas[n] = RGBtoP(img1->imagedata[i + ix][j + jx]);
                        i2_lumas[n] = RGBtoP(img2->imagedata[i + ix][j + jx]);
                    } else {
                        fprintf(stderr, "ERROR: Unknown luma conversion method '%d' specified\n",
                                (int)param.conversion);
                        exit(1);
                    }
                    if (img1luma != NULL) {
                        img1luma->imagedata[i + ix][j + jx].red = (unsigned char)i1_lumas[n];
                    }
                    if (img2luma != NULL) {
                        img2luma->imagedata[i + ix][j + jx].red = (unsigned char)i2_lumas[n];
                    }
                    if (param.debug > 3) {
                        fprintf(stderr, "(%5d,%5d) (%3d, %3d, %3d) => %0.5g (%u)\n",
                                i+ix, j+jx,
                                img1->imagedata[i+ix][j+jx].red,
                                img1->imagedata[i+ix][j+jx].green,
                                img1->imagedata[i+ix][j+jx].blue,
                                i1_lumas[n],
                                img1luma ? img1luma->imagedata[i + ix][j + jx].red : 0);
                        fprintf(stderr, "              (%3d, %3d, %3d) => %0.5g (%u)\n",
                                img2->imagedata[i+ix][j+jx].red,
                                img2->imagedata[i+ix][j+jx].green,
                                img2->imagedata[i+ix][j+jx].blue,
                                i2_lumas[n],
                                img2luma ? img2luma->imagedata[i + ix][j + jx].red : 0);
                    }
                    n++;
                }
            }
            double i1_mean = mean(i1_lumas, n);
            double i2_mean = mean(i2_lumas, n);
            double i1_var  = variance(i1_lumas, i1_mean, n);
            double i2_var  = variance(i2_lumas, i2_mean, n);
            double covar   = covariance(i1_lumas, i2_lumas, i1_mean, i2_mean, n);
            double ssim    = ((2 * i1_mean * i2_mean + c1) * (2 * covar + c2)) /
                             ((i1_mean * i1_mean + i2_mean * i2_mean + c1) * (i1_var + i2_var + c2));
            ++cnt;
            sum += ssim;

            if (!param.avg_only) {
                printf("(%d,%d)  SSIM = %10.9f\n", j, i, ssim);
                if (param.debug)
                    fflush(stdout);
            }
            if (ssim < minssim) {
                minssim = ssim;
            }
            if (param.numbuckets > 0) {
                int bucket = (ssim * ((float)param.numbuckets / 2)) + (param.numbuckets / 2);
                /* 1.0 SSIM gets its own bucket, so no adjustment needed */
                assert(bucket <= (int)param.numbuckets && bucket >= 0);
                param.buckets[bucket]++;
            }
            if (param.threshold > 0.0 && ssim < param.threshold) {
                ++numbelow;
            }

            if (diff != NULL) {
                for (unsigned int ix = 0; ix < nx; ix++ ) {
                    for (unsigned int jx = 0; jx < ny; jx++ ) {
                        unsigned char luma = img1luma->imagedata[i + ix][j + jx].red;
                        diff->imagedata[i + ix][j + jx].red   = luma;
                        if (ssim >= 1) {
                            /* Identical, so no coloring needed.  Copy the luma
                             * data into all the channels to make a greyscale.
                             */
                            diff->imagedata[i + ix][j + jx].blue  =
                            diff->imagedata[i + ix][j + jx].green = luma;
                        } else {
                            /* Scale the SSIM to [0,luma-16] and put it in
                             * the blue and green channels.  As SSIM approaches
                             * -1, the pixels will appear more and more red.
                             */
                            if (luma < 16) {
                                luma = 16;
                            }
                            unsigned char reduction = (luma - 16) * ((ssim + 1) / 2);
                            diff->imagedata[i + ix][j + jx].blue  =
                            diff->imagedata[i + ix][j + jx].green = reduction;
                        }
                    }
                }
            }

            if (param.debug > 1) {
                fprintf(stderr, "means:\timg1: %0.40g\n\timg2: %0.40g\n",
                        i1_mean, i2_mean);
                fprintf(stderr, "variances:\timg1: %0.40g\n\t\timg2: %0.40g\n",
                        i1_var, i2_var);
                fprintf(stderr, "covariance: %0.40g\n", covar);
                fprintf(stderr, "(%d,%d) ssim: %0.40g (%u)\n", j, i, ssim,
                        diff ? diff->imagedata[i][j].red : 0);
            }

        }
    }
    free(i1_lumas);
    free(i2_lumas);

    FILE *tableout = stderr;
    unsigned int maxthresh = param.maxthresh;
    /* The bar for passing should be the lesser of maxthresh and
     * maxthreshpct*cnt, if both are set.
     */
    if (param.maxthreshpct > 0) {
        if (param.maxthresh <= 0
                ||
                param.maxthresh > (param.maxthreshpct * cnt)) {
            maxthresh = param.maxthreshpct * cnt;
        }
    }
#define dumptable(stream, format, ...) \
    fprintf(stream, format, __VA_ARGS__);     \
    if (stream != stderr) {                   \
        if (param.debug)                      \
            fflush(stdout);                   \
        fprintf(stderr, format, __VA_ARGS__); \
    }
    if (numbelow > maxthresh) {
        printf("%s ERROR:\n", param.file1);
        printf("The maximum number of SSIM below the threshold has been reached. \n");
        tableout = stdout;
    } else {
        dumptable(tableout, "%s: INFO:\n", param.file1);
    }

    if (param.threshold > 0.0) {
        dumptable(tableout, "  Blocks below threshold: %d blocks of %d allowed (%d total)\n", numbelow, maxthresh, cnt);
    }
    if (cnt > 0) {
        dumptable(tableout, "                AVG SSIM: %10.9f\n", sum/cnt);
    }
    dumptable(tableout, "                MIN SSIM: %10.9f\n", minssim);
    param.printbuckets = 1;
    if (param.avg_only) {
        if (cnt > 0) {
            printf("%s: AVG SSIM = %10.9f\n", param.file1, sum/cnt);
        } else {
            printf("%s: ERROR: Count is zero.\n", param.file1);
        }
    }
    if (param.debug)
        fflush(stdout);

    if (param.printbuckets) {
        dumptable(tableout, "       SSIM distribution:%c", '\n');
        dumptable(tableout, "         [        % 4.3f]: %d\n", 1.0, param.buckets[param.numbuckets]);
        for (int i = param.numbuckets - 1; i >= 0; --i) {
            float r1 = (((float) i - (param.numbuckets / 2))     / (float)param.numbuckets) * 2;
            float r2 = (((float) i - (param.numbuckets / 2) + 1) / (float)param.numbuckets) * 2;
            dumptable(tableout, "         [% 4.3f, % 4.3f): %d\n", r1, r2, param.buckets[i]);
        }
    }
    return 0;
}

double mean(double *values, int n) {

    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += values[i];
    return sum / (double)n;
}

double variance(double *values, double mean, int n) {

    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += (values[i] - mean) * (values[i] - mean);
    return sum / (double)(n - 1);

}

double covariance(double *values1, double *values2, double mean1, double mean2, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += (values1[i] - mean1) * (values2[i] - mean2);
    return sum / (double)(n - 1);
}

char *load_image_file(const char *fname, TargaImage *img, params param) {
    char *base    = spec_strdup(fname);
    char *findstr = strrchr(base, '.');
    if (findstr == NULL || (STRCMP(findstr, ".tga") && STRCMP(findstr, ".yuv"))) {
        free(base);
        printf("'%s' is an unknown file type.\n", fname);
        printUsage(NULL);
        exit(1);
    } else if (!STRCMP(findstr, ".yuv")) {
        *findstr = 0;
        read_yuv_file(fname, img, param);
    } else {
        *findstr = 0;
        read_tga_file(fname, img);
    }

    /* Look for (and chop off) a trailing .org or .orig to avoid faciliating
     * the creation of filenames like "foo.orig.new.tga".
     */
    findstr = strrchr(base, '.');
    if (findstr != NULL &&
            (!STRCMP(findstr, ".org") || !STRCMP(findstr, ".orig"))) {
        *findstr = 0;
    }

    return(base);
}

/* Values in TGA headers are _supposed_ to be little-endian, according to
 * v2.0 of the spec.
 */
#define read_le8(stream, dest)  dest = fgetc(stream)
#define read_le16(stream, dest)                      \
    {                                                \
        unsigned char tmprawdata[2];                 \
        fread(tmprawdata, 2, 1, stream);             \
        dest = tmprawdata[0] | (tmprawdata[1] << 8); \
    }
/* The storage format for 16-bit color pixels or colormap entries is
 * ARRRRRGGGGGBBBBB; the 5 bit color values are converted to color
 * components by shifting left 3 bits.
 */
#define read_16bit_pixel(stream, dest)        \
    {                                         \
        unsigned int tmpmap = 0;              \
        read_le16(stream, tmpmap);            \
        dest.blue  = (tmpmap & 0x001f) << 3;  \
        dest.green = (tmpmap & 0x03e0) >> 2;  \
        dest.red   = (tmpmap & 0x7c00) >> 7;  \
        dest.attr  = (tmpmap & 0x8000) >> 15; \
    }
#define read_pixel(stream, dest, depth)             \
    {                                               \
        if (depth == 8) {                           \
            dest.attr = dest.blue = dest.green = 0; \
            read_le8(stream, dest.red);             \
        } else if (depth == 16) {                   \
            read_16bit_pixel(stream, dest);         \
        } else {                                    \
            read_le8(stream, dest.blue);            \
            read_le8(stream, dest.green);           \
            read_le8(stream, dest.red);             \
            if (depth == 32) {                      \
                read_le8(stream, dest.attr);        \
            } else {                                \
                dest.attr = 0;                      \
            }                                       \
        }                                           \
    }
void read_tga_file (const char *fname, TargaImage *tga) {
    /* Code to read Targa files was originally cribbed from
     * http://www.paulbourke.net/dataformats/tga/
     * and has been extensively modified since then.
     */

    /* Open the input tga file */
    FILE *fin = fopen(fname, "rb");

    if (fin == NULL) {
        fprintf(stderr, "'%s': Could not open for read\n", fname);
        exit(-1);
    }

    /* Read the required header fields */
    /* TODO: Actually validate this data */
    read_le8 (fin, tga->idlength);
    read_le8 (fin, tga->colormaptype);
    read_le8 (fin, tga->datatypecode);
    read_le16(fin, tga->colormaporigin);
    read_le16(fin, tga->colormaplength);
    read_le8 (fin, tga->colormapdepth);
    read_le16(fin, tga->x_origin);
    read_le16(fin, tga->y_origin);
    read_le16(fin, tga->width);
    read_le16(fin, tga->height);
    read_le8 (fin, tga->bitsperpixel);
    read_le8 (fin, tga->imagedescriptor);
    tga->alphabits = tga->imagedescriptor & 0x0F;

    /* Read the optional image ID data */
    if (tga->idlength > 0) {
        fread(tga->image_id, tga->idlength, 1, fin);
    }

    /* Read the color map data, if any */
    if (tga->colormaplength > 0) {
        check_malloc( tga->colormapdata, Pixel *, (sizeof(Pixel) * tga->colormaplength) );
        memset(tga->colormapdata, 0, sizeof(Pixel) * tga->colormaplength);
        for(unsigned int i = 0; i < tga->colormaplength; i++) {
            read_pixel(fin, tga->colormapdata[i], tga->colormapdepth);
        }
    }

    /* TODO: Make this work for colormap images (easy)
     * TODO: Make this work for RLE images (also not too hard)
     */
    check_malloc( tga->imagedata, Pixel **, (sizeof(Pixel *) * tga->height) );
    for (unsigned int h = 0; h < tga->height; ++h) {
        check_malloc( tga->imagedata[h], Pixel *, (sizeof(Pixel) * tga->width) );
        for (unsigned int w = 0; w < tga->width; ++w) {
            read_pixel(fin, tga->imagedata[h][w], tga->bitsperpixel);
        }
    }
    fclose(fin);
}

/* Read a YUV420p file and convert it to RGB.
 * Raw YUV files have no header, so the image dimensions are taken
 * from parameters supplied on the command-line.  If there are none,
 * the the default of 1280x720 is used.
 */
#define clampval(dest, tmptype, min, max, val) \
    {                                   \
        tmptype clampedtmp = (val);     \
        if (clampedtmp < min) {         \
            dest = min;                 \
        } else if (clampedtmp > max) {  \
            dest = max;                 \
        } else {                        \
            dest = clampedtmp;          \
        }                               \
    }
void read_yuv_file (const char * fname, TargaImage * tga, params param) {
    int i, j, h, w, nb;
    unsigned char *buffer;
    YUV **pixels;
    int i2 = 0;

    /* Set default TGA header values */
    tga->idlength        = 0;
    tga->colormaptype    = 0;
    tga->datatypecode    = TRUECOLOR;
    tga->colormaporigin  = 0;
    tga->colormaplength  = 0;
    tga->colormapdepth   = 0;
    tga->x_origin        = 0;
    tga->y_origin        = 0;
    tga->width           = param.width;
    tga->height          = param.height;
    tga->bitsperpixel    = 24;
    tga->imagedescriptor = 0;
    tga->alphabits       = 0;

    /* Open the input yuv file */
    FILE *fin = fopen(fname, "rb");
    if (fin == NULL) {
        fprintf(stderr, "'%s': Could not open for read\n", fname);
        exit(-1);
    }

    /* Allocate space for the final RGB image data, the YUV input data, and
     * the buffer to hold the mishmash of data in the file.
     */
    check_malloc( tga->imagedata, Pixel **, (sizeof(Pixel *) * tga->height) );
    check_malloc( pixels, YUV **, tga->height * sizeof(YUV *) );
    check_malloc( buffer, unsigned char *, tga->width * sizeof(unsigned char) );
    for (h = 0; h < tga->height; ++h) {
        check_malloc( tga->imagedata[h], Pixel *, sizeof(Pixel) * tga->width );
        check_malloc( pixels[h], YUV *, tga->width * sizeof(YUV) );
    }

    /* Read in the Y, which are the first WxH bytes in the file */
    for (i = 0;i < tga->height; i++) {
        int nb = fread(buffer, 1, tga->width, fin);
        if (nb < tga->width) {
            fprintf(stderr, "%s: Error reading Y data: Only got %d of %d\n",
                    fname, nb, tga->width);
            exit(1);
        }
        for (j = 0; j < tga->width; j++) {
            pixels[i][j].y = buffer[j];
        }
    }

    /* Read in the U.  There's one value for every 2x2 block of pixels,
     * so there are WxH/4 bytes total
     */
    for (i = 0; i < tga->height; i++) {
        if (i % 2 == 0) {       /* Only read data every other row */
            nb = fread(buffer, 1, tga->width / 2, fin);
            if (nb < tga->width / 2) {
                fprintf(stderr, "%s: Error reading U data: Only got %d of %d\n",
                        fname, nb, tga->width / 2);
                exit(1);
            }
        }
        for (i2 = 0, j = 0; j < tga->width; ++i2, j += 2) {
            pixels[i][j    ].u = buffer[i2];
            pixels[i][j + 1].u = buffer[i2];
        }
    }

    /* Read in the V.  As with U, there's one value for every 2x2 block of
     * pixels, so there are WxH/4 bytes total
     */
    for (i = 0; i < tga->height; i++) {
        if (i % 2 == 0) {       /* Only read data every other row */
            nb = fread(buffer, 1, tga->width / 2, fin);
            if (nb < tga->width / 2) {
                fprintf(stderr, "%s: Error reading V data: Only got %d of %d\n",
                        fname, nb, tga->width / 2);
                exit(1);
            }
        }
        for (i2 = 0, j = 0; j < tga->width; ++i2, j += 2) {
            pixels[i][j    ].v = buffer[i2];
            pixels[i][j + 1].v = buffer[i2];
        }
    }

    /* Now do the YUV444 to RGB888 conversion */
    h=0;
    for (i = tga->height - 1; i >= 0; i--) {
        w=0;
        for (j = 0;j < tga->width; j++) {
            clampval( tga->imagedata[h][w].blue,  int, 16, 255,
                      1.164 * (pixels[i][j].y - 16) + 2.018 * (pixels[i][j].u - 128) );
            clampval( tga->imagedata[h][w].green, int, 16, 255,
                      1.164 * (pixels[i][j].y - 16) - 0.812 * (pixels[i][j].v - 128) - 0.391 * (pixels[i][j].u - 128) );
            clampval( tga->imagedata[h][w].red,   int, 16, 255,
                      1.164 * (pixels[i][j].y - 16) + 1.596 * (pixels[i][j].v - 128) );
            ++w;
        }
        ++h;
    }

    fclose(fin);

    for (h = 0; h < tga->height; ++h) {
        free(pixels[h]);
    }
    free(buffer);
    free(pixels);
}

void dump_header(const char *fname, TargaImage *tga) {
    fprintf(stderr, "HEADER DUMP FOR %s\n", fname);
    if (tga == NULL) {
        fprintf(stderr, "BAD TargaImage STRUCTURE\n");
    } else {
        char imgtype[100];
        memset(imgtype, 0, sizeof(imgtype));
        switch(tga->datatypecode & 0x07) {
            case NODATA:    sprintf(imgtype, "No image data");
                            break;
            case TRUECOLOR: if (tga->alphabits > 0) {
                                sprintf(imgtype, "%s%u-bit TrueColor image; %d bits of alpha",
                                        (tga->datatypecode & 0x08) ? "RLE-encoded " : "",
                                        tga->bitsperpixel, tga->alphabits);
                            } else {
                                sprintf(imgtype, "%s%u-bit TrueColor image; no alpha",
                                        (tga->datatypecode & 0x08) ? "RLE-encoded " : "",
                                        tga->bitsperpixel);
                            }
                            break;
            case GRAYSCALE: sprintf(imgtype, "%s%u-bit grayscale image",
                                    (tga->datatypecode & 0x08) ? "RLE-encoded " : "",
                                    tga->bitsperpixel);
                            break;
            case COLORMAP:  sprintf(imgtype, "%s%u-bit color-mapped image",
                                    (tga->datatypecode & 0x08) ? "RLE-encoded " : "",
                                    tga->colormapdepth);
                            break;
            default:        sprintf(imgtype, "UNSUPPORTED/UNKNOWN image");
                            break;
        }
        fprintf(stderr, "IDLENGTH=%u\n",          tga->idlength);
        fprintf(stderr, "COLORMAPTYPE=%u\n",      tga->colormaptype);
        fprintf(stderr, "DATATYPECODE=%u (%s)\n", tga->datatypecode, imgtype);
        fprintf(stderr, "COLORMAPORIGIN=%u\n",    tga->colormaporigin);
        fprintf(stderr, "COLORMAPLENGTH=%u\n",    tga->colormaplength);
        fprintf(stderr, "COLORMAPDEPTH=%u\n",     tga->colormapdepth);
        fprintf(stderr, "X_ORIGIN=%u\n",          tga->x_origin);
        fprintf(stderr, "Y_ORIGIN=%u\n",          tga->y_origin);
        fprintf(stderr, "WIDTH=%u\n",             tga->width);
        fprintf(stderr, "HEIGHT=%u\n",            tga->height);
        fprintf(stderr, "BITSPERPIXEL=%u\n",      tga->bitsperpixel);
        fprintf(stderr, "IMAGEDESCRIPTOR=%u\n",   tga->imagedescriptor);
    }
}

/* Yeah, I know that with 8 bits there's no little- or big-endian */
#define dump_le8(stream, val)  fputc(((val) & 0x00FF), (stream))
#define dump_le16(stream, val) dump_le8((stream), (val)); \
                               fputc(((val) & 0xFF00) >> 8, (stream))
#define write_pixel(stream, pixel, depth)     \
    if (depth == 8) {                         \
        dump_le8(stream, pixel.red);          \
    } else if (depth == 16) {                 \
        unsigned int tmpmap = 0;              \
        tmpmap  = (pixel.attr  & 0x01) << 15; \
        tmpmap |= (pixel.red   & 0x1F) << 10; \
        tmpmap |= (pixel.green & 0x1F) << 5;  \
        tmpmap |= (pixel.blue  & 0x1F);       \
        dump_le16(stream, tmpmap);            \
    } else {                                  \
        dump_le8(stream, pixel.blue);         \
        dump_le8(stream, pixel.green);        \
        dump_le8(stream, pixel.red);          \
        if (depth == 32) {                    \
            dump_le8(stream, pixel.attr);     \
        }                                     \
    }
void dump_tga(const char *fname, TargaImage *tga) {

    /* Open the input tga file */
    FILE *fout = fopen(fname, "wb");
    if (fout == NULL) {
        fprintf(stderr, "'%s': Could not open for writing\n", fname);
        exit(-1);
    }

    /* Set the header fields */
    dump_le8 (fout, tga->idlength);
    dump_le8 (fout, tga->colormaptype);
    dump_le8 (fout, tga->datatypecode);
    dump_le16(fout, tga->colormaporigin);
    dump_le16(fout, tga->colormaplength);
    dump_le8 (fout, tga->colormapdepth);
    dump_le16(fout, tga->x_origin);
    dump_le16(fout, tga->y_origin);
    dump_le16(fout, tga->width);
    dump_le16(fout, tga->height);
    dump_le8 (fout, tga->bitsperpixel);
    dump_le8 (fout, tga->imagedescriptor);

    /* Dump the image data */
    for (unsigned int i = 0; i < tga->height; i++) {
        for (unsigned int j = 0; j < tga->width; j++) {
            if (tga->datatypecode == TRUECOLOR || tga->datatypecode == GRAYSCALE) {
                write_pixel(fout, tga->imagedata[i][j], tga->bitsperpixel);
            }
        }
    }
    fclose(fout);
}

TargaImage *init_targa(TargaImage *ref, params param) {
    TargaImage *img;
    check_malloc( img, TargaImage *, sizeof(TargaImage) );
    if (ref != NULL) {
        memcpy(img, ref, sizeof(TargaImage));
    } else {
        memset(img, 0, sizeof(TargaImage));
        /* Now set the things that might not be 0 */
        img->datatypecode    = TRUECOLOR;
        img->width           = param.width;
        img->height          = param.height;
        img->bitsperpixel    = 24;
    }

    /* Allocate the colormap, if necessary */
    if (img->colormaplength > 0) {
        check_malloc( img->colormapdata, Pixel *, sizeof(Pixel) * img->colormaplength );
        memset(img->colormapdata, 0, (sizeof(Pixel) * img->colormaplength) );
    }

    /* Allocate space for the image data */
    check_malloc( img->imagedata, Pixel **, (sizeof(Pixel *) * img->height) );
    for (unsigned int h = 0; h < img->height; ++h) {
        check_malloc( img->imagedata[h], Pixel *, (sizeof(Pixel) * img->width) );
        memset(img->imagedata[h], 0, sizeof(Pixel) * img->width);
    }

    return(img);
}

char *spec_strdup(const char * str) {
    int len = strlen(str);
    char *newstr;
    check_malloc( newstr, char *, sizeof(char) * (len + 10) );
    strncpy(newstr, str, len + 10);
    newstr[len] = '\0';
    return(newstr);
}

char *get_version(void) {
    char *start = strrchr(svnrev, ':');
    if (start == NULL) {
        /* No LastChangedRevision? */
        return spec_strdup("vUNKNOWN");
    }

    /* Get a copy of the static string */
    char *vers = spec_strdup(svnrev);

    /* Find the character before the last $ and make it the end */
    char *last = strrchr(vers, '$');
    if (last == NULL || last == vers) {
        /* This should really not happen */
        free(vers);
        return spec_strdup("vUNKNOWN");
    }
    *(last - 1) = '\0';

    /* Replace the first space with an 'r' */
    ptrdiff_t numoffset = start - svnrev + 1;
    *(vers + numoffset) = 'r';

    /* Make a free()able copy of the version string */
    char *ret = spec_strdup(vers + numoffset);
    free(vers);

    return ret;
}

void printUsage(char *progname) {
    if (progname == NULL) {
        progname = "ImageValidator";
    }
    printf("Compare two images using the Structural SIMilarity (SSIM) index method.\n"
           "SSIM values are calculated for blocks of 8x8 pixels.\n"
          );
    printf("\nUsage: %s [options] <file1> <file2>\n", progname);
    printf("  <file1> and <file2> must be either uncompressed Targa (TGA) format or raw\n"
           "                      YUV-422 data.\n"
           "Options:\n"
           "  -avg              Do not print block-by-block similarity scores; instead, \n"
           "                    just print the average SSIM.\n"
           "  -buckets <n>      Print SSIM histogram with <n> buckets\n"
           "  -conv <method>    Select the method to convert RGB to brightness, one of:\n"
           "                      REC709  ITU-R Rec. 709 (HDTV) coeffiecients (default)\n"
           "                      REC601  ITU-R Rec. 601 (SDTV) coeffiecients\n"
           "                      HSP     P from HSP http://alienryderflex.com/hsp.html\n"
           "  -debug            Print debug information to stderr; may be repeated\n"
           "  -diff <file>      Print the pixel differences to a Targa file\n"
           "  -dumpfile         Dump the input files to Targa file\n"
           "  -dumpheader       Dump the image file Targa header info to stderr\n"
           "  -dumpluma         Dump the luma data to Targa files\n"
           "  -height           Height of the image (YUV input only). Default 720\n"
           "  -help             Print this message\n"
           "  -maxthreshold <n> Report an error if the number of blocks below the \n"
           "                    threshold is greater than <n>, where <n> can be an \n"
           "                    absolute number of blocks, or a fraction, or a \n"
           "                    percent (e.g. \"1%%\" or \"0.01\").\n"
           "  -threshold <n>    Count the number of blocks with SSIM below <n>, \n" 
           "                    where <n> is a number between 0 and 1\n"
           "  -width            Width of the image (YUV input only). Default is 1280\n"
          );
}

/* Editor settings: (please leave this at the end of the file)
 * vim: set filetype=c syntax=c shiftwidth=4 tabstop=8 expandtab nosmarttab ai:
 */
